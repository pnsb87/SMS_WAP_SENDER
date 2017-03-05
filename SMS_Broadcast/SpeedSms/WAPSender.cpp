#include "StdAfx.h"
#include "WAPSender.h"
#include "Reporter.h"

WAPSender::WAPSender(void)
{
	srand(time(NULL));
	this->tranID = rand() % 100;
}

int WAPSender::getTranId()
{
	return this->tranID;
}
int WAPSender::Config(SerialCommunication* comm)
{
	assert(comm != NULL);
	bReady = false;
	m_serialComm = comm;

	std::string wapCommand = "AT+CMGF=0\r";
	m_serialComm->Write(wapCommand.c_str(), wapCommand.length());

	char result[MAX_BUFFER_SIZE] = {0};
	int nLen = m_serialComm->Read(result, MAX_BUFFER_SIZE);
	
	if((strstr(result, "OK") !=NULL)/* || (strstr(result, "\r\n>") !=NULL)*/)
	{
		bReady = true;
		return 0;
	}
	return -1;
}

void WAPSender::setUrl(std::string url_)
{
	this->strURL = url_;
}

std::string WAPSender::getUrl()
{
	return this->strURL;
}

int WAPSender::Send(std::string dstNumber)
{
	if(!bReady)
		return -1;
	char szMsg[MAX_BUFFER_SIZE] = {0};
	char szReturn[MAX_BUFFER_SIZE] = {0};

	std::string result = "";
	int nLen = 0;
	nLen = WAPPushPDU::getPDU(dstNumber, this->strURL, this->smsText, this->tranID, result);

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
	
	//Second: Write bytes to serial
	sprintf_s(szMsg, "%s%c\r", result.c_str(), (char)26);
	m_serialComm->Write(szMsg, strlen(szMsg));
	nLen = m_serialComm->Read(szReturn, MAX_BUFFER_SIZE);
	
	if((strstr(szReturn, "OK") !=NULL) || (strstr(szReturn, "\r\n>") !=NULL))
	{
		Reporter::instance()->Add2Report(dstNumber, true);
		return 0;
	}
	Reporter::instance()->Add2Report(dstNumber, false);
	return -1;
}

WAPSender::~WAPSender(void)
{
}
