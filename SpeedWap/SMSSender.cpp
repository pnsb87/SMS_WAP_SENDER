#include "StdAfx.h"
#include "SMSSender.h"


SMSSender::SMSSender(void)
{

}

int SMSSender::Config(SerialCommunication* comm)
{
	return 0;
}

int SMSSender::Send(std::string dstNumber)
{
	return 0;
}

void SMSSender::setSMSText(std::string text__)
{
	this->smsText = text__;
}

std::string SMSSender::getSMSText()
{
	return this->smsText;
}

SMSSender::~SMSSender(void)
{
}
