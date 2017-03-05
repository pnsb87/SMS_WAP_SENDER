#include "StdAfx.h"
#include "SMSSender.h"
#include "Reporter.h"

SMSSender::SMSSender(void)
{
	m_serialComm = NULL;
	bReady = false;
}

SMSSender::SMSSender(SerialCommunication* comm)
{
	assert(comm != NULL);
	m_serialComm = comm;
	bReady = false;
}

int SMSSender::Config(SerialCommunication* comm)
{
	assert(comm != NULL);
	bReady = false;
	m_serialComm = comm;

	std::string smsCommand = "AT+CMGF=1\r";
	m_serialComm->Write(smsCommand.c_str(), smsCommand.length());

	char result[MAX_BUFFER_SIZE] = {0};
	int nLen = m_serialComm->Read(result, MAX_BUFFER_SIZE);
	
	char* res = strstr(result, "OK");
	if(res ==NULL) 
	{
		//if(strstr(result, "\r\n>") == NULL)
		return -1;
	}
	bReady = true;
	return 0;
}

int SMSSender::Send(std::string dstNumber)
{
	if(!bReady)
		return -1;

	char szMsg[MAX_BUFFER_SIZE] = {0};
	char result[MAX_BUFFER_SIZE] = {0};
	sprintf_s(szMsg, "AT+CMGS=\"%s\"\r", dstNumber.c_str());
	m_serialComm->Write(szMsg, strlen(szMsg));
	m_serialComm->Read(result, MAX_BUFFER_SIZE);

	char* res = strstr(result, "OK");
	if(res ==NULL) 
	{
		if(strstr(result, "\r\n>") == NULL)
			return -1;
	}
	
	memset(szMsg, 0, MAX_BUFFER_SIZE);
	memset(result, 0, MAX_BUFFER_SIZE);
	sprintf_s(szMsg, "%s%c\r", this->smsText.c_str(), (char)26);
	m_serialComm->Write(szMsg, strlen(szMsg));
	m_serialComm->Read(result, MAX_BUFFER_SIZE);

	res = strstr(result, "OK");
	if(res ==NULL) 
	{
		if(strstr(result, "\r\n>") == NULL)
		{
			Reporter::instance()->Add2Report(dstNumber, false);
			return -1;
		}
	}

	Reporter::instance()->Add2Report(dstNumber, true);

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
	if(m_serialComm)
	{
		m_serialComm->Close();
		delete m_serialComm;
		m_serialComm = NULL;
	}
}
