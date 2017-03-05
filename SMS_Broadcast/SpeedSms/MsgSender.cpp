#include "StdAfx.h"
#include "MsgSender.h"
#include "UsbInterface.h"
#include "Reporter.h"
#include "Utils.h"
#include <bitset>
#include <vector>
#include <algorithm>
#include <sstream>


CMsgSender::CMsgSender(void)
{
	memset(m_szProtocolId, 0, sizeof(m_szProtocolId));

	m_serialComm = NULL;
	bReadySms = false;
	bReadyWap = false;

	srand(time(NULL));
	this->tranID = rand() % 100;

	m_nMode = 0;

	convertTableSingle[0x24] = 0x02;
	convertTableSingle[0x5F] = 0x11;

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
}

int CMsgSender::getTranId()
{
	return this->tranID;
}

CMsgSender::~CMsgSender(void)
{
	bReadySms = false;
	bReadyWap = false;
}

int CMsgSender::ConfigCom(SerialCommunication *comm)
{
	assert(comm != NULL);
	m_serialComm = comm;

	bReadySms = false;
	bReadyWap = false;

	return 0;
}

int CMsgSender::ConfigMode(int nMode)
{
	m_nMode = nMode;

	//if(nMode == SMS_MODE)
	//{
	//	std::string smsCommand = "AT+CMGF=1\r";
	//	m_serialComm->Write(smsCommand.c_str(), smsCommand.length());

	//	char result[MAX_BUFFER_SIZE] = {0};
	//	int nLen = m_serialComm->Read(result, MAX_BUFFER_SIZE);
	//	
	//	char* res = strstr(result, "OK");
	//	if(res ==NULL) 
	//	{
	//		//if(strstr(result, "\r\n>") == NULL)
	//		return -1;
	//	}

	//	bReadySms = true;
	//	bReadyWap = false;
	//}
	//else
	//{
		std::string wapCommand = "AT+CMGF=0\r";
		m_serialComm->Write(wapCommand.c_str(), wapCommand.length());

		char result[MAX_BUFFER_SIZE] = {0};
		int nLen = m_serialComm->Read(result, MAX_BUFFER_SIZE);
		
		if((strstr(result, "OK") ==NULL)/* || (strstr(result, "\r\n>") !=NULL)*/)
			return -1;

		bReadySms = false;
		bReadyWap = true;
	//}

	return 0;
}

void CMsgSender::SetProtocolId(const char* szProtocolId)
{
	sprintf(m_szProtocolId, "%s", szProtocolId);
}

void CMsgSender::setMsgText(string text)
{
	m_sMsgText.clear();
	m_sMsgText = text;

}

void CMsgSender::setUrlText(string url)
{
	m_sUrlText.clear();
	m_sUrlText = url;

}

int CMsgSender::Send(string dstNumber)
{
	if(m_nMode == SMS_MODE)
		//return SendSms(dstNumber);
		return SendSmsPdu(dstNumber);
	else
		return SendWap(dstNumber);
}

void CMsgSender::exit()
{
	if(m_serialComm)
	{
		m_serialComm->Close();
		//delete m_serialComm;
		//m_serialComm = NULL;
	}
}
int CMsgSender::SendSms(std::string dstNumber)
{
	if(!bReadySms)
		return -1;

	char szMsg[MAX_BUFFER_SIZE] = {0};
	char result[MAX_BUFFER_SIZE] = {0};
	sprintf_s(szMsg, "AT+CMGS=\"%s\"\r", dstNumber.c_str());
	m_serialComm->Write(szMsg, strlen(szMsg));
	Sleep(100);
	m_serialComm->Read(result, MAX_BUFFER_SIZE);

	char* res = strstr(result, "OK");
	if(res ==NULL) 
	{
		if(strstr(result, "\r\n>") == NULL)
			return -1;
	}
	
	memset(szMsg, 0, MAX_BUFFER_SIZE);
	memset(result, 0, MAX_BUFFER_SIZE);
	sprintf_s(szMsg, "%s%c\r", this->m_sMsgText.c_str(), (char)26);
	m_serialComm->Write(szMsg, strlen(szMsg));
	
	/*m_serialComm->Read(result, MAX_BUFFER_SIZE);

	res = strstr(result, "OK");
	if(res ==NULL) 
	{
		if(strstr(result, "\r\n>") == NULL)
		{
			Reporter::instance()->Add2Report(dstNumber, false);
			return -1;
		}
	}*/

	Reporter::instance()->Add2Report(dstNumber, true);

	return 0;
}

int CMsgSender::SendSmsPdu(std::string dstNumber)
{
	if(!bReadyWap)
		return -1;

	char szMsg[MAX_BUFFER_SIZE] = {0};
	char result[MAX_BUFFER_SIZE] = {0};

	int number_len = 0;
	std::string bcd_number = Utils::string2BCD(dstNumber, number_len);

	if(m_sMsgText.size() <=160)
	{
		CString text(m_sMsgText.c_str());

		CString encodeText;
		int nLen =0;

		encodeText = Encode7bit(text, &nLen);

		CString body;

		body.Append("00"); //Indicates that we don’t supply a SMSC number. The one that is configured in the device will be used.
		body.Append("21"); //Request status report
		body.Append("00"); //Our message reference
		body.AppendFormat("%02X", number_len); //Size of destination telephone number (in digits)
		body.Append("91");
		body.Append(bcd_number.c_str()); //msisdn number in BCD format
		body.Append(m_szProtocolId); //Protocol identifier.
		body.Append("00"); //This represents the Data Coding Scheme
		body.AppendFormat("%02X", nLen/*encodeText.GetLength()/2 +1*/);
		body.Append(encodeText.GetString());

		sprintf_s(szMsg, "AT+CMGS=%d\r", body.GetLength()/2 -1);

		m_serialComm->Write(szMsg, strlen(szMsg));
		Sleep(500);
		/*m_serialComm->Read(result, MAX_BUFFER_SIZE);

		char* res = strstr(result, "OK");
		if(res ==NULL) 
		{
			if(strstr(result, "\r\n>") == NULL)
			{
				Reporter::instance()->Add2Report(dstNumber, false);
				return -1;
			}
		}

		memset(szMsg, 0, MAX_BUFFER_SIZE);
		memset(result, 0, MAX_BUFFER_SIZE);
		*/
		sprintf_s(szMsg, "%s%c\r", body.GetString(), (char)26);
		m_serialComm->Write(szMsg, strlen(szMsg));
		Sleep(500);
		/*Sleep(1000);
		m_serialComm->Read(result, MAX_BUFFER_SIZE);

		res = strstr(result, "OK");
		if(res ==NULL) 
		{
			if(strstr(result, "\r\n>") == NULL)
			{
				Reporter::instance()->Add2Report(dstNumber, false);
				return -1;
			}
		}*/

		//Reporter::instance()->Add2Report(dstNumber, true);
	}
	else
	{
		int nNumberSms = m_sMsgText.size()/153;
		if(m_sMsgText.size() - nNumberSms*153 >0)
			nNumberSms++;
		
		int nLen2Copy = 0;

		char* pSzSms = new char[1000];

		sprintf(pSzSms, "%s", m_sMsgText.c_str());

		//Gui tung phan mot
		for(int i=0; i < nNumberSms; i++)
		{
			CString body;
			CString body2;
			CString body3;

			//Dung 153 byte de vua du add vao moi part( sau khi encode la 134 byte + 6 byte UDH + 20 byte header  =160 byte)
			if(m_sMsgText.size() - i*153 >153)
				nLen2Copy = 153;
			else
				nLen2Copy = m_sMsgText.size() - 153;

			char szPart[500];
			memset(szPart, 0, sizeof(szPart));
			memcpy(szPart, pSzSms + i*153, nLen2Copy);

			CString text(szPart);
			//CString text("nguyen huu ngoc");
			CString encodeText;
			int nLen = 0;
			int size = 0;

			encodeText = Encode7bit(text, &nLen);
			
			//body.Append("0041000B914809942639F0001018050003010301986F79B96D4DDBC320A33CEC1EBF");
								   	
			body.Append("00"); //Indicates that we don’t supply a SMSC number. The one that is configured in the device will be used.
			body.Append("41"); //User Data header present (UDH indicator)
			body.Append("00"); //Our message reference
			body.AppendFormat("%02X", number_len); //Size of destination telephone number (in digits)
			body.Append("91");
			body.Append(bcd_number.c_str()); //msisdn number in BCD format
			body.Append(m_szProtocolId); //Protocol identifier.
			body.Append("00"); //This represents the Data Coding Scheme
			body.AppendFormat("%02X", 1 + 5 + nLen +1); //UDL = UDHL + UDH + len_text_septet
			body.Append("05"); //User data header length
			body.Append("00"); //IEI = 0×00
			body.Append("03"); //IEDL = 0×03
			body.Append("01"); //Ref nr = 0×00
			body.AppendFormat("%02X", nNumberSms); //Total  = 0×03
			body.AppendFormat("%02X", i+1); //This part

			size = body.GetLength()/2;
			size = encodeText.GetLength()/2;

			body.Append(encodeText.GetString());

			size = body.GetLength()/2;

			sprintf_s(szMsg, "AT+CMGS=%d\r", body.GetLength()/2 -1);
			m_serialComm->Write(szMsg, strlen(szMsg));
			Sleep(500);
			//m_serialComm->Read(result, MAX_BUFFER_SIZE);

			//char* res = strstr(result, "OK");
			//if(res ==NULL) 
			//{
			//	if(strstr(result, "\r\n>") == NULL)
			//	{
			//		Reporter::instance()->Add2Report(dstNumber, false);
			//		return -1;
			//	}
			//}

			//memset(szMsg, 0, MAX_BUFFER_SIZE);
			//memset(result, 0, MAX_BUFFER_SIZE);
			//
			sprintf_s(szMsg, "%s%c\r", body.GetString(), (char)26);
			m_serialComm->Write(szMsg, strlen(szMsg));
			Sleep(500);
			//m_serialComm->Read(result, MAX_BUFFER_SIZE);

			//res = strstr(result, "OK");
			//if(res ==NULL) 
			//{
			//	if(strstr(result, "\r\n>") == NULL)
			//	{
			//		Reporter::instance()->Add2Report(dstNumber, false);
			//		return -1;
			//	}
			//}

			//body2.Append("0041000B914809942639F000101905000301030240EEF79C2EB7A6ED6190511E768FD");

			//sprintf_s(szMsg, "AT+CMGS=%d\r", body2.GetLength()/2 -1);
			//m_serialComm->Write(szMsg, strlen(szMsg));
			//Sleep(500);

			//sprintf_s(szMsg, "%s%c\r", body2.GetString(), (char)26);
			//m_serialComm->Write(szMsg, strlen(szMsg));
			//Sleep(500);

			//body3.Append("0041000B914809942639F0001018050003010303986F79B96D4DDBC320A33CEC1EB");
			//sprintf_s(szMsg, "AT+CMGS=%d\r", body3.GetLength()/2 -1);
			//m_serialComm->Write(szMsg, strlen(szMsg));
			//Sleep(500);

			//sprintf_s(szMsg, "%s%c\r", body3.GetString(), (char)26);
			//m_serialComm->Write(szMsg, strlen(szMsg));
			//Sleep(500);
		}

		delete[] pSzSms;
	}
	

	return 0;
}

int CMsgSender::SendWap(std::string dstNumber)
{
	if(!bReadyWap)
		return -1;

	char szMsg[MAX_BUFFER_SIZE] = {0};
	char szReturn[MAX_BUFFER_SIZE] = {0};

	std::string result = "";
	int nLen = 0;
	nLen = WAPPushPDU::getPDU(dstNumber, this->m_sUrlText, this->m_sMsgText, this->tranID, result);

	//First: write length to serial
	sprintf_s(szMsg, "AT+CMGS=%d\r", nLen);
	m_serialComm->Write(szMsg, strlen(szMsg));

	/*nLen = m_serialComm->Read(szReturn, MAX_BUFFER_SIZE);
	if((strstr(szReturn, "OK") ==NULL) || (strstr(szReturn, "\r\n>") == NULL))
	{
		Reporter::instance()->Add2Report(dstNumber, false);
		return -1;
	}*/

	memset(szReturn, 0, MAX_BUFFER_SIZE);
	memset(szMsg, 0, MAX_BUFFER_SIZE);
	Sleep(1000);
	//Second: Write bytes to serial
	sprintf_s(szMsg, "%s%c\r", result.c_str(), (char)26);
	m_serialComm->Write(szMsg, strlen(szMsg));
	Sleep(100);
	/*nLen = m_serialComm->Read(szReturn, MAX_BUFFER_SIZE);
	
	if((strstr(szReturn, "OK") !=NULL) || (strstr(szReturn, "\r\n>") !=NULL))
	{
		Reporter::instance()->Add2Report(dstNumber, true);
		return 0;
	}
	Reporter::instance()->Add2Report(dstNumber, false);*/

	return 0;
}

CStringA CMsgSender::Encode7bit(CString message, int *len)
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

CStringA CMsgSender::ConvertSpecial(CStringA msg)
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