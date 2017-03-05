#pragma once

#include "wappush.h"
<<<<<<< .mine
#include "bitencoder.h"
#include "bytebuffer.h"
=======
#include "BitEncoder.h"
#include "ByteBuffer.h"
>>>>>>> .r8

#include <string>
#include <iostream>

typedef std::string Str;
typedef std::wstring WStr;

class CWapForm
{
public:
	CWapForm(void);
	~CWapForm(void);

	void WapPushInit(gn_wap_push *wp);
	char* WapPushInit(const char* szMobile, const char* szText, const char* szUrl);
	int WapPushEncode(gn_wap_push *wp);

	char* encode_attr_inline_string(char token, char *string, int *data_len);
	char* encode_indication(gn_wap_push *wp, int *data_len);
	char* encode_si(gn_wap_push *wp, int *data_len);
	int gn_wap_push_encode(gn_wap_push *wp); 

	char* GetPDU(const char* szMobile, const char* szText, const char* szUrl);
	void wstrToUtf8(Str& dest, const WStr& src);

	int getUTF8(const char* szSrcBuf, const char* szDesBuf);
	void charPtr2Wstring(const char* szBuffer, WStr &szWstring);

	char* char2HexString(const char* szBuffer);
	char* string2BCD(const char* szMobile);

public:
	int m_nWapLen;
};
