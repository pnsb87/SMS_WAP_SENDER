#ifndef __WAPPUSHPDU_H
#define __WAPPUSHPDU_H
#include <string>
//#include <stdint.h>
#include "ByteBuffer.h"
#include "BitEncoder.h"
#define MAX_PDU_LEN 1000
#define MAX_MSISDN_LEN 20
#define MAX_TEXT_LEN 160
class WAPPushPDU
{
public:
	WAPPushPDU(void);
	static int getPDU(std::string dstNumber, std::string url, std::string text, CByteBuffer& output);
	static int getPDU(std::string dstNumber, std::string url, std::string text, std::string& result);
	static int string2BCD(std::string szMobile, u8* output, int output_len);
	static int char2HexString(std::string text, u8* output, int output_len);
	~WAPPushPDU(void);
};

#endif

