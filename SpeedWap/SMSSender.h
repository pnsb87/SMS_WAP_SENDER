#ifndef __SMSSENDER_H
#define __SMSSENDER_H

#include "SerialCommunication.h"
#include <string>
class SMSSender
{
public:
	SMSSender(void);
	virtual int Config(SerialCommunication* comm);
	virtual int Send(std::string dstNumber);
	virtual ~SMSSender(void);
	void setSMSText(std::string text__);
	std::string getSMSText();
protected:
	std::string smsText;
	SerialCommunication* m_serialComm;
};
#endif

