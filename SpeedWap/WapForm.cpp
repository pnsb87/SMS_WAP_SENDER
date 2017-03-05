#include "StdAfx.h"
#include "WapForm.h"


std::ostream& operator<<(std::ostream& f, const WStr& s);
std::istream& operator>>(std::istream& f, WStr& s);


CWapForm::CWapForm(void)
{
	m_nWapLen = 0;
}

CWapForm::~CWapForm(void)
{
}

char* CWapForm::encode_attr_inline_string(char token, char *string, int *data_len) 
{
	char *data = NULL;
	
	/* we need 3 extra bytes for tags */
	*data_len = strlen(string) + 3;
	data = (char*)malloc(*data_len);

	if (!data) {
	    return NULL;
	}
	
	data[0] = token;
	data[1] = TAG_INLINE;
	memcpy(data + 2, string, strlen(string));
	data[*data_len - 1] = 0x00;

	return data;
}

char* CWapForm::encode_indication(gn_wap_push *wp, int *data_len) 
{
	char *data = NULL;
	char *attr = NULL;
	int attr_len = 0;
	int offset = 0;
	
	/* encode tag attribute */
	attr = encode_attr_inline_string(ATTR_HREF, wp->url, &attr_len);
	
	if (!attr || !attr_len) {
	    return NULL;
	}
	
	/* need 5 extra bytes for indication token & attributes */
	*data_len = attr_len + strlen(wp->text) + 5;
	data = (char*)malloc(*data_len);
	
	if (!data) {
	    return NULL;
	}
	
	/* indication tag token */
	data[offset++] = TOKEN_KNOWN_AC | TAG_INDICATION;
	
	/* attribute */
	memcpy(data + offset, attr, attr_len);
	offset += attr_len;
	data[offset++] = TAG_END;

	/* wappush text */
	data[offset++] = TAG_INLINE;
	memcpy(data + offset, wp->text, strlen(wp->text));
	offset += strlen(wp->text);
	data[offset++] = 0x00;
	
	/* tag end */
	data[offset++] = TAG_END;

	free(attr);

	return data;
}

char* CWapForm::encode_si(gn_wap_push *wp, int *data_len)
{
	char *data = NULL;
	char *child = NULL;
	int child_len = 0;
	
	child = encode_indication(wp, &child_len);

	if (!child || !data_len) {
	    return NULL;
	}

	/* we need extra 2 bytes for si token */
	*data_len = child_len + 2;
	data = (char*)malloc(*data_len);
	
	if (!data) {
	    free(child);
	    return NULL;
	}
	
	data[0] = TOKEN_KNOWN_C | TAG_SI;
	memcpy(data + 1, child, child_len);
	data[*data_len - 1] = TAG_END;
	
	free(child);
	
	return data;
}

int CWapForm::WapPushEncode(gn_wap_push *wp) 
{
	
	char *data = NULL;
	int data_len = 0;
	
	data = encode_si(wp, &data_len);
	
	if (!data || !data_len) {
	    return -1;
	}
	
	wp->data = (char*)malloc(data_len + sizeof(gn_wap_push_header));

	if (!wp->data) {
	    return -1;
	}
	strcpy(wp->data, (char*)&wp->header);
	memcpy(wp->data, &wp->header, sizeof(gn_wap_push_header));
	memcpy(wp->data + sizeof(gn_wap_push_header), data, data_len);
	
	wp->data_len = data_len + sizeof(gn_wap_push_header);

	return 0;
}

void CWapForm::WapPushInit(gn_wap_push *wp) 
{

	if (!wp) {
	    return;
	}

	//memset(wp, 0, sizeof(gn_wap_push));
	
	wp->header.wsp_tid 		= 0x00;
	wp->header.wsp_pdu 		= PDU_TYPE_Push;
	wp->header.wsp_hlen 		= 0x01;
	wp->header.wsp_content_type 	= CONTENT_TYPE;

	wp->header.version 	= WBXML_VERSION;
	wp->header.public_id 	= TAG_SI;
	wp->header.charset 	= WAPPush_CHARSET;
	wp->header.stl 		= 0x00; /* string table length */
}

char* CWapForm::WapPushInit(const char *szMobile, const char *szText, const char* szUrl)
{
	return NULL;
}

char* CWapForm::GetPDU(const char* szMobile, const char* szText, const char* szUrl)
{
	char szSMS[320];
	memset(&szSMS,0,160);

	Str smsBuf;

	//lay BCD mobile, nho phai delete
	char* pMobile = string2BCD(szMobile);

	//Convert url sang utf 8, nho phai delete
	char* pUrl = char2HexString(szUrl);

	//Convert text sang hex
	const char* szMsg = "Check out Mobile Tidings!";
	char* pText = char2HexString(szMsg);

	////Send SL
	//smsBuf.append("0041000B91");
	//smsBuf.append(pMobile);
	////smsBuf.append("4809942639F0");
	//smsBuf.append("00042E0B05040B84C0020003F001010A060403B081EA02066A00850903");
	//smsBuf.append(pUrl);
	////smsBuf.append("09036D6F62696C65746964696E67732E636F6D");
	//smsBuf.append("2F0001");

	////SEND SI
	/*
	smsBuf.append("0041000B91");
	smsBuf.append(pMobile);
	smsBuf.append("00045E0B05040B84C0020003F001010B060403AE81EA02056A0045C60C03");
	smsBuf.append(pUrl);
	smsBuf.append("001103363533320008010AC30620090226162510C3062014030416250103");
	smsBuf.append(pText);
	smsBuf.append("000101");
	*/
	//SI sample

	smsBuf.append("0041000B914889758853F500045E0B05040B84C0020003F001010B060403AE81EA02056A0045C60C036D6F62696C65746964696E67732E636F6D2F000AC30620090226162510C3062009030416250103436865636B206F7574204D6F62696C6520546964696E677321000101");

	//smsBuf.append("0041000B91");
	//smsBuf.append(pMobile);
	//smsBuf.append("00045E0B05040B84C0020003F001010B060403AE81EA02056A0045C60C03");
	//smsBuf.append(pUrl);
	//smsBuf.append("0011033635333200080103");
	//smsBuf.append(pText);
	//smsBuf.append("000101");

	strcpy(szSMS, smsBuf.c_str());

	/*for(int i=0; i< smsBuf.length(); i++)
	{
		char c = szSMS[i];
		toupper(c);
	}*/

	m_nWapLen = smsBuf.length()/2;
	//m_nWapLen = 59;

	//delete pMobile;
	//delete pUrl;

	return szSMS;
}

char* CWapForm::string2BCD(const char* szMobile)
{
	Str mobile(szMobile);

	if(mobile.at(0) == '0')
		mobile.replace(0, 1, "84");	

	char* szBCDmobile = new char[mobile.length()];
	memset(szBCDmobile, 0, mobile.length());

	if(mobile.length() % 2)
		mobile.append("F");

	Str BCDmobile(mobile);

	for(int i=0; i< mobile.length(); i+=2)
	{
		BCDmobile.at(i) = mobile.at(i+1);
		BCDmobile.at(i+1) = mobile.at(i);
	}

	strcpy(szBCDmobile, BCDmobile.c_str());

	return szBCDmobile;
}

char hex[] = {
     '0', '1', '2', '3', '4', '5', '6', '7',
     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
 };

char* CWapForm::char2HexString(const char* szBuffer)
{
	Str buffer(szBuffer);
	Str HexString;

	char* szHexBuf = new char[buffer.length()*2];
	memset(szHexBuf, 0, buffer.length()*2);

	for(int i=0; i< buffer.length(); i++)
	{
		char temp[5];
		int hi = (szBuffer[i]>>4) & 0x0f;
		int low = (szBuffer[i]) & 0x0f;
		sprintf(temp, "%c%c", toupper(hex[hi]), toupper(hex[low]));

		HexString.append(temp);
	}

	strcpy(szHexBuf, HexString.c_str());

	return szHexBuf;
}

void CWapForm::wstrToUtf8(Str& dest, const WStr& src)
{
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07));
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}	
}

int CWapForm::getUTF8(const char* szBuffer, const char* szDesBuf)
{
	Str des;
	WStr src;

	charPtr2Wstring(szBuffer, src);
	wstrToUtf8(des, src);

	char* szRet = new char[des.size()];

	strcpy(szRet, des.c_str());

	szDesBuf = szRet;

	return des.size();
}

void CWapForm::charPtr2Wstring(const char* szBuffer, WStr &szWstring)
{
	Str s = szBuffer;

	WStr temp(s.begin(), s.end());

	szWstring = temp;
}

