#ifndef __WAPSENDER_H
#define __WAPSENDER_H
#include "WAPPushPDU.h"
#include "SMSSender.h"


class WAPSender : public SMSSender
{
public:
	WAPSender(void);
	int Config(SerialCommunication* comm);
	void setUrl(std::string url_);
	std::string getUrl();
	int Send(std::string dstNumber);
	
	~WAPSender(void);
private:
	std::string strURL;
	bool bReady;
};
#endif

