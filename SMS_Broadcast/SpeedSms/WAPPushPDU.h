#ifndef __WAPPUSHPDU_H
#define __WAPPUSHPDU_H
#include <string>
//#include <stdint.h>
#define MAX_PDU_LEN 1000
#define MAX_MSISDN_LEN 20
#define MAX_TEXT_LEN 160
class WAPPushPDU
{
public:
	WAPPushPDU(void);
	static int getPDU(std::string dstNumber, std::string url, std::string text, int tranId, std::string& result);
	~WAPPushPDU(void);
};

#endif

