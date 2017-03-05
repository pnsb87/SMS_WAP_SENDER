#include "StdAfx.h"
#include "PDULib.h"
#include "constants.h"


//////////////////////////////////////////////////////////////////////
// Enable debug memory manager

#ifdef _DEBUG

#ifdef THIS_FILE
#undef THIS_FILE
#endif

static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW

#endif


CPDULib::CPDULib()
{
	// convert table for double character
	convertTable[0x0C] = 0x0A;
	convertTable[0x5E] = 0x14;
	convertTable[0x7B] = 0x28;
	convertTable[0x7D] = 0x29;
	convertTable[0x5C] = 0x2F;
	convertTable[0x5B] = 0x3C;
	convertTable[0x7E] = 0x3D;
	convertTable[0x5D] = 0x3E;
	convertTable[0x7C] = 0x40;

	// Reverse convert table
	reserveConvertTable[0x0A] = 0x0C;
	reserveConvertTable[0x14] = 0x5E;
	reserveConvertTable[0x28] = 0x7B;
	reserveConvertTable[0x29] = 0x7D;
	reserveConvertTable[0x2F] = 0x5C;
	reserveConvertTable[0x3C] = 0x5B;
	reserveConvertTable[0x3D] = 0x7E;
	reserveConvertTable[0x3E] = 0x5D;
	reserveConvertTable[0x40] = 0x7C;

	// convert table for single character
	//convertTableSingle[0x40] = 0x00; // manual convert
	convertTableSingle[0x24] = 0x02;
	convertTableSingle[0x5F] = 0x11;

	reserveConvertTableSingle[0x00] = 0x40;
	reserveConvertTableSingle[0x02] = 0x24;
	reserveConvertTableSingle[0x11] = 0x5F;
}
CPDULib::~CPDULib()
{
}
CStringA CPDULib::ConvertSpecial(CStringA msg)
{
	map<byte, byte>::iterator it;
	CStringA oneChar;
	int iLen = msg.GetLength();
	for (it = convertTableSingle.begin(); it != convertTableSingle.end(); it++)
	{
		oneChar.Format("%c", (*it).second);
		msg.Replace((CStringA)(wchar_t)(*it).first, oneChar);
	}
	iLen = msg.GetLength();

	for (it = convertTable.begin(); it != convertTable.end(); it++)
	{
		oneChar.Format("%c%c", 0x1B, (*it).second);
		msg.Replace((CStringA)(wchar_t)(*it).first, oneChar);
	}
	return msg;
}
CString CPDULib::ReverseConvertSpecial(CStringA msg, int msgLen /*=-1*/) // dungtx: add real len
{
	int i = 0;
	CStringA ret;
	CStringA oneChar;

	int iLen = msg.GetLength();
	if (msgLen > 0 && msgLen < iLen) iLen = msgLen;	// dungtx: add real len

	while (i < iLen)
	{
		if (msg[i] == 0x1B) // escape character
		{
			ret += (char)reserveConvertTable[msg[++i]];
		}
		else if ((msg[i] == 0x02 || msg[i] == 0x11 || msg[i] == 0x00))// && i != msg.GetLength()-1)
			ret += (char)reserveConvertTableSingle[msg[i]];
		else
			ret += msg[i];
		i++;
	}
	return (CString)ret;
}
CStringA CPDULib::CreatePDUMessage(CStringA phoneNumber, CString message, bool statusReport /* = false */, bool useUnicode /* = true */, CStringA smscNumber /* = */ )
{
	CStringA ret= "00"; // non MSC number, use ME default
	// Enabled status report?
	// Ignore validity
	CStringA pduMsg;
	if (statusReport)
	{
		ret += "31";
	}
	else ret += "11";
	ret += "00"; // ignore reference number
	ret += EncodePDUPhoneNumber(phoneNumber);
	// Ignore Protocol ID
	ret += "00";
	CStringA len;
	if (useUnicode)
	{
		ret += "08"; // Data coding scheme : UCS-2
		ret += "FF"; // maximum validity
		pduMsg = EncodeUCS2(message);
		len.Format("%02X", message.GetLength() * 2); // 2 bytes for each character
		ret += len; // Add message length
		ret += pduMsg; // Encoded message
	}
	else 
	{
		ret += "00";  // data coding scheme: IRA
		ret += "FF"; // maximum validity
		int tempLen = 0;
		pduMsg = Encode7bit(message, &tempLen);
		//len.Format("%02X", pduMsg.GetLength() / 2); // 1 bytes for each character

		// Haint - add real len
		//len.Format("%02X", message.GetLength() ); // 1 bytes for each character
		len.Format("%02X", tempLen ); 
		ret += len; // Add message length
		ret += pduMsg; // Encoded message
	}
	return ret;
}
CSMSEntry CPDULib::FetchSMS(int slotID, int messageType, CStringA message)
{
	//ParseSMSPara(0);
	int offset = 0;
	int len = strtol(message.Mid(offset, 2), NULL,16);
	offset += 2; // jump over MSC len
	CStringA phoneNumber = DecodePDUPhoneNumber(message.Mid(offset, len*2));
	// Haint - check valid sms
	if (phoneNumber.IsEmpty())
	{
		return CSMSEntry();
	}
	offset += len * 2; // jump over MSC number
	CStringA smsType = message.Mid(offset, 2);
	offset += 2; // jump over SMS type
	ParseSMSPara((byte)strtol(smsType, NULL, 16));
	if (messageType < 2) // sms received
	{
		//smsType = (CStringA)smsType[1] +  (CStringA)smsType[0]; // reverse octet
		//ParseSMSPara((byte)strtol(smsType, NULL, 16));
		if (m_bTP_SRI)
		{
			return GetStatusReport(slotID, message); // bypass ref number
		}
	}
	else // if sms received
	{
		offset +=2; // jump over reference number
	}
	//CStringA body = message.Mid(offset);
	return GetMessageContent(slotID, messageType, message.Mid(offset), message);
}
void CPDULib::ParseSMSPara(byte para)
{
	std::bitset<8> bit (para);
	m_bTP_RP	= !bit[7];			// TODO: make sure this message meaning
	m_bTP_UDHI	= bit[6];
	//m_bTP_SRI	= bit[5];
	m_bTP_MMS	= bit[2];
	if (bit[1] && !bit[0])
		m_bTP_SRI = true;
	else m_bTP_SRI = false;
	
}
CStringA CPDULib::EncodePDUPhoneNumber(CStringA phoneNumber)
{
	CStringA ret = "";
	
	// int'l format
	if (phoneNumber[0] == '+')
	{
		phoneNumber = phoneNumber.Mid(1);
		ret.Format("%02X", phoneNumber.GetLength());
		// 91 prefix for phone number
		ret += "91";
	}
	else 
	{
		ret.Format("%02X", phoneNumber.GetLength());
		ret += "81";
	}
	
	int len = phoneNumber.GetLength();
	// Padding 'F' if the len is odd
	if (len % 2 != 0)
	{
		phoneNumber += 'F';
		len ++;
	}
	for (int i = 0; i < len; i+=2)
	{
		ret = ret + phoneNumber[i + 1] + phoneNumber[i];
		//ret += phoneNumber[i];
	}
	return ret;
}

CStringA CPDULib::DecodePDUPhoneNumber(CStringA PDUPhoneNumber)
{
	CStringA ret;
	// int'l format
	if (PDUPhoneNumber.Left(2) == "91")
	{
		// 91 prefix for phone number
		ret += "+";
		//pos = 1;
	}
	//else ret += "81";
	/*CStringA h = phoneNumber.Mid(pos, 2);

	if (phoneNumber.Left(2) == "84")
	{
	pos += 2;
	}*/


	for (int i = 2; i < (int)PDUPhoneNumber.GetLength(); i += 2)
	{
		ret = ret + PDUPhoneNumber[i + 1] + PDUPhoneNumber[i];
		//ret += phoneNumber[i];
	}
	ret.Remove('F');
	return ret;
}

CStringA CPDULib::Encode7bit(CString message, int *len)
{
	CStringA msg = ConvertSpecial((CStringA) message);// for ANSI encoding only
	CStringA oneChar;
	int iLeft = 0;
	std::vector<long> vect;
	int iLen = msg.GetLength();

	if (iLen < message.GetLength()) iLen = message.GetLength();
	*len = iLen;

	int iCount = iLen % 8;
	if (iCount == 0) iCount = 8;
	for (int i = iLen - 1; i >= 0; --i)
	{
		char currentChar = (char)msg[i];
		if (currentChar == 0x40) // @ character
		{
			if (i == 0 || (i > 0 && msg[i-1] != 0x1B)) currentChar = 0x00;
		}

		std::bitset<8> bit(currentChar); //(msg[i]);

		std::bitset<8> bitTemp = bit << (8 - iCount + 1);

		bit >>= (iCount - 1);
		std::bitset<8> bitLeft(iLeft);
		bit |= bitLeft;

		iLeft = bitTemp.to_ulong();
		if (iCount != 8)
			vect.push_back(bit.to_ulong());

		iCount--;
		if (iCount == 0) iCount = 8;
	}

	std::reverse(vect.begin(), vect.end());
	msg.Empty();
	iLen = vect.size();
	for (int i = 0; i < iLen; ++i)
	{	
		oneChar.Format("%02X", vect[i]);
		msg += oneChar;
	}
	return msg;
}
CString CPDULib::Decode7bit(CStringA message, int startIndex /* = 0 */, int msgLen /*=-1*/) // Dungtx: add msgLen: real mesesage len
{
	bool isSpace = false;
	CStringA ret;
	long iLeft = 0;
	std::stringstream ss;
	int iCount = 0, iChar = 0;

	int iLen = message.GetLength() / 2;

	for (int i = 0; i < iLen; i++)
	{
		ss.str((LPCSTR)message.Mid(i * 2, 2));	 // read 2 chars at a time
		ss >> std::hex >> iChar;
		ss.clear();
		iCount++;
		if (iCount > 7) iCount = 1;

		std::bitset<8> bit(iChar);
		std::bitset<8> bitTemp = bit >> (8 - iCount);
		std::bitset<8> bitLeft(iLeft);
		bit <<= iCount;
		bit >>= 1;
		bit |= bitLeft;
		iLeft = bitTemp.to_ulong();
	
		char charTemp = (char)bit.to_ulong();
		if (i >= startIndex)
			ret += charTemp;
		if (iCount == 7)	// output the extra byte
		{
			if (i + 1 >= startIndex)
				ret += (char)bitTemp.to_ulong();
			iLeft = 0;
		}
	}
	return (CString)ReverseConvertSpecial(ret, msgLen);
}
CTime CPDULib::GetTimeStamp(CStringA pduTimeStamp)
{
	int year, month, day, hour, minute, sec;	
	year  = atoi(pduTimeStamp.Mid(0,2).MakeReverse()); // year
	month = atoi(pduTimeStamp.Mid(2,2).MakeReverse()); // mon
	//timeStamp.Format("%s%s" , pduTimeStamp[5], pduTimeStamp[4]);
	day   = atoi(pduTimeStamp.Mid(4,2).MakeReverse());// day
	//timeStamp.Format("%s%s" , pduTimeStamp[7], pduTimeStamp[6]);
	hour  = atoi(pduTimeStamp.Mid(6,2).MakeReverse()); // hour
	//timeStamp.Format("%s%s" , pduTimeStamp[9], pduTimeStamp[8]);
	minute= atoi(pduTimeStamp.Mid(8,2).MakeReverse()); // min
	sec   = atoi(pduTimeStamp.Mid(10,2).MakeReverse()); // sec

	// Check for valid time
	if (day < 1 || day > 31)
	{
		 day = 1;
	}
	if (month < 1 || month > 12)
	{
		month = 1;
	}
	if (hour < 0 || hour > 24)
	{
		hour = 0;
	}
	if (minute < 0 || minute > 60)
	{
		minute = 0;
	}
	if (sec < 0 || sec > 60)
	{
		sec = 0;
	}
	return CTime(year + 2000 , month, day, hour, minute, sec);
}
CStringA CPDULib::GetPDUTimeStamp(CTime timestamp)
{
	CString ret = timestamp.Format("%y").MakeReverse() 
		 + timestamp.Format("%m").MakeReverse() 
		 + timestamp.Format("%d").MakeReverse() 
		 + timestamp.Format("%H").MakeReverse() 
		 + timestamp.Format("%M").MakeReverse() 
		 + timestamp.Format("%S").MakeReverse();
	return (CStringA) ret;
}
CStringA CPDULib::EncodeUCS2(CString message)
{
	CStringA ret;
	CStringA oneOctet;
	int len = message.GetLength();
	//ret.Format("%02X", len * 2);
	for (int i = 0; i < len; i++)
	{
		oneOctet.Format("%04X", message[i]);
		ret += oneOctet;
	}
	return ret;
}

CString CPDULib::DecodeUCS2(CStringA message)
{
	CString ret;
	CStringA oneOctet;
	//int len = strtol(message.Left(2), NULL, 16);
	//ret.Format("%02X", len * 2);
	
	for (int i = 0; i < message.GetLength() / 4; i++)
	{
		ret += (wchar_t)strtol(message.Mid(i * 4 , 4), NULL, 16);
		//ret += oneOctet;
	}
	return ret;
}

CSMSEntry CPDULib::GetStatusReport(int slotID, CStringA message)
{
	//07914889200030F0061C0B914889043315F0901161519330829011615193808200
	int offset = 0;
	int len = strtol(message.Mid(offset, 2), NULL, 16);
	offset = 2 + len * 2; // jump over MSC len + MSC number
	offset += 2; // jump over SMS type indicator
	offset += 2; // jump over msg reference number
	len = strtol(message.Mid(offset, 2), NULL,16) + 2; // plus + sign, international format
	offset += 2; // jump over phone len
	if (len % 2 != 0) 
		len++;
	CStringA phoneNumber = DecodePDUPhoneNumber(message.Mid(offset, len));
	offset += len;
	offset += 14; // jump over the first time stamp
	CTime dateTime = GetTimeStamp(message.Mid(offset, 14));
	if (message.Right(2) == "00")
	{
		len = SMS_DELIVERY_SUCCESSED;
	}
	else len = SMS_DELIVERY_FAILED;
	return CSMSEntry(slotID, len, phoneNumber, _T("") , dateTime, _T(SIM), message);
}

CSMSEntry CPDULib::GetMessageContent(int slotID, int messageType, CStringA message, CStringA org_msg)
{
	CStringA phoneNumber;
	int offset = 0;
	int len = strtol(message.Mid(offset, 2), NULL,16) + 2; // plus + sign, international format
	offset += 2; // jump over phone len
	if (len % 2 != 0) 
		len++;
	phoneNumber = DecodePDUPhoneNumber(message.Mid(offset, len));
	offset += len;
	offset += 2; // jump over protocol ID
	CStringA smsType = message.Mid(offset, 2);
	offset += 2; // jump over coding scheme
	CTime dateTime;
	if (messageType < 2)
	{
		dateTime = GetTimeStamp(message.Mid(offset, 14));
		offset += 14;
	}
	else
	{
		offset += 2; // jump over validity period
		dateTime = CTime::GetCurrentTime();
	}
	offset += 2 ; // jump over message content's length
	if (m_bTP_UDHI)
	{
		len = strtol(message.Mid(offset, 2), NULL,16) + 2; // plus UDHI length plus the next character
	}
	else len = 0;
	CString content;
	if (smsType == "08")
	{
		content = DecodeUCS2(message.Mid(offset));
	}
	// If 7 bit encoding
	else if (atoi(smsType) < 4)
	{
		int msgLen = strtol(message.Mid(offset-2, 2), NULL,16);
		content = Decode7bit(message.Mid(offset), len, msgLen);
	}
	for (len = 0; len < content.GetLength(); len++)
	{
		if (content[len] < 32)
		{
			content.Remove(content[len]);
		}
	}
	return CSMSEntry(slotID, messageType, phoneNumber, content, dateTime, _T(SIM), org_msg);
}

CStringA CPDULib::UTF16toUTF8(const CStringW& utf16)
{
   CStringA utf8;
   int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
   if (len>1)
   { 
      char *ptr = utf8.GetBuffer(len-1);
      if (ptr) WideCharToMultiByte(CP_UTF8, 0, utf16, -1, ptr, len, 0, 0);
      utf8.ReleaseBuffer();
   }
   return utf8;
}

CStringW CPDULib::UTF8toUTF16(const CStringA& utf8)
{
   CString utf16;
   int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
   if (len>1)
   { 
      wchar_t *ptr = utf16.GetBuffer(len-1);
      if (ptr) MultiByteToWideChar(CP_UTF8, 0, utf8, -1, ptr, len);
      utf16.ReleaseBuffer();
   }
   return utf16;
};


void CPDULib::UTF16toUTF8(const CStringW& utf16,CStringA& utf8)
{   
   int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
   if (len>1)
   { 
      char *ptr = utf8.GetBuffer(len-1);
      if (ptr) WideCharToMultiByte(CP_UTF8, 0, utf16, -1, ptr, len, 0, 0);
      utf8.ReleaseBuffer();
   } 
}

void CPDULib::UTF8toUTF16(const CStringA& utf8,CStringW& utf16)
{   
   int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
   if (len>1)
   { 
      wchar_t *ptr = utf16.GetBuffer(len-1);
      if (ptr) MultiByteToWideChar(CP_UTF8, 0, utf8, -1, ptr, len);
      utf16.ReleaseBuffer();
   }   
};


int CPDULib::stringToHex(CStringA& input,CStringA& out){
	int sz = input.GetLength();	
	char* in = input.GetBuffer(sz-1);
	int nsz = sz << 1;
	char* ptr = (char*)out.GetBufferSetLength(nsz);
	ptr += nsz-1;
	for(int j = sz-1; j >=0; j--){
		char c = in[j];
		*(ptr--) = hexval[c & 0x0F];
		*(ptr--) = hexval[((c >> 4) & 0xF)];		
	}	
	input.ReleaseBuffer();
	out.ReleaseBuffer(nsz);
	return nsz;
}

int CPDULib::hexToString(CStringA& inp,CStringA & out){
	int len = inp.GetLength();
	char* input = inp.GetBuffer(len-1);
	len >>=1;		
	char* ptr = (char*)(out.GetBuffer(len-1));

	for (int i=0; i < len; i++)
		{
			char ch1, ch2;
			ch1 = *(input++);
			ch2 = *(input++);
			int dig1, dig2;
			if(ch1>='0' && ch1<='9' ) dig1 = ch1 - '0';
			else if(ch1>='A' && ch1<='F') dig1 = ch1 - 'A' + 10;
			else if(ch1>='a' && ch1<='f') dig1 = ch1 - 'a' + 10;

			if(ch2>='0' && ch2<='9') dig2 = ch2 - '0';
			else if(ch2>='A' && ch2<='F') dig2 = ch2 - 'A' + 10;
			else if(ch2>='a' && ch2<='f') dig2 = ch2 - 'a' + 10;
			*(ptr++) = (char)((dig1 << 4) + dig2);
		}
	inp.ReleaseBuffer();
	out.ReleaseBuffer(len);
	return len;
}


int CPDULib::stringToHex(char* input,int sz,char* out){
	for(int j = 0; j < sz; j++){
		char c = input[j];
		out[j<<1] = hexval[((c >> 4) & 0xF)];
		out[(j<<1) + 1] = hexval[c & 0x0F];
	}
	return sz<<1;
}

int CPDULib::hexToString(char* input,int len,char* out){
	len >>=1;

	for(int i=0; i < len; i++)
		{
			char ch1, ch2;
			ch1 = *(input++);
			ch2 = *(input++);
			int dig1, dig2;
			if(ch1>='0' && ch1<='9' ) dig1 = ch1 - '0';
			else if(ch1>='A' && ch1<='F') dig1 = ch1 - 'A' + 10;
			else if(ch1>='a' && ch1<='f') dig1 = ch1 - 'a' + 10;

			if(ch2>='0' && ch2<='9') dig2 = ch2 - '0';
			else if(ch2>='A' && ch2<='F') dig2 = ch2 - 'A' + 10;
			else if(ch2>='a' && ch2<='f') dig2 = ch2 - 'a' + 10;
			out[i] = (char)((dig1 << 4) + dig2);
		}
	return len;
}


void CPDULib::EncodeUCS2(const CString & message,CStringA& ret)
{	
	UTF16toUTF8(message,ret);
	stringToHex(ret,ret);
	int len = ret.GetLength();
	if (len != ((len >> 2) <<2)) ret += "00";
}

void CPDULib::DecodeUCS2(CStringA & message,CString& ret){	
	hexToString(message,message);
	UTF8toUTF16(message,ret);
};
