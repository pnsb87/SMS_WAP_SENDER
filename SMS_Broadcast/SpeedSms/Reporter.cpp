#include "StdAfx.h"
#include "Reporter.h"

Reporter* Reporter::instance_ = NULL;
Reporter* Reporter::instance()
{
	if(instance_ == NULL)
		instance_ = new Reporter();
	return instance_;
}

void Reporter::Add2Report(std::string number, bool bSuccess)
{
	WaitForSingleObject(safeguard__, INFINITE);

	if(bSuccess)
		lstSendSuccess.push_back(number);
	else
		lstSendFailed.push_back(number);

	ReleaseMutex(safeguard__);
}

void Reporter::clear()
{
	WaitForSingleObject(safeguard__, INFINITE);
	lstSendSuccess.clear();
	lstSendFailed.clear();
	ReleaseMutex(safeguard__);
}

void Reporter::start()
{
	if(!isRunning)
	{
		isRunning = true;
		DWORD dwId;
		hThread = CreateThread(NULL, 0, ReportThread, this, 0, &dwId);
	}
}

void Reporter::stop()
{
	if(isRunning)
	{
		isRunning = false;
		WaitForSingleObject(hThread, 5000);
		CloseHandle(hThread);
		hThread = INVALID_HANDLE_VALUE;
	}
}

void Reporter::clearLogFile()
{
	FILE* m_pReportOk;
	FILE* m_pReportFail;
	m_pReportOk = NULL;
	m_pReportFail = NULL;

	char szReportName[200];

	time_t now = time(0);
	tm *gmtm = gmtime(&now);

	sprintf(szReportName, "Report_Sent_OK_%d_%d_%d.txt"
	, gmtm->tm_mday, gmtm->tm_mon +1, gmtm->tm_year + 1900);

	m_pReportOk = fopen(szReportName, "w");

	sprintf(szReportName, "Report_Sent_Failed_%d_%d_%d.txt"
	, gmtm->tm_mday, gmtm->tm_mon +1, gmtm->tm_year + 1900);
	m_pReportFail = fopen(szReportName, "w");
	fclose(m_pReportOk);
	fclose(m_pReportFail);
}

DWORD Reporter::ReportThread(LPVOID param)
{
	Reporter* pThis = (Reporter*)param;
	FILE* m_pReportOk;
	FILE* m_pReportFail;
	pThis->clearLogFile();
	while(pThis->isRunning)
	{
		m_pReportOk = NULL;
		m_pReportFail = NULL;

		char szReportName[200];

		time_t now = time(0);
		tm *gmtm = gmtime(&now);

		sprintf(szReportName, "Report_Sent_OK_%d_%d_%d.txt"
		, gmtm->tm_mday, gmtm->tm_mon +1, gmtm->tm_year + 1900);

		m_pReportOk = fopen(szReportName, "a");
		if(!m_pReportOk)
		{
			Sleep(1000);
			continue;
		}

		sprintf(szReportName, "Report_Sent_Failed_%d_%d_%d.txt"
		, gmtm->tm_mday, gmtm->tm_mon +1, gmtm->tm_year + 1900);

		m_pReportFail = fopen(szReportName, "a");
		if(!m_pReportFail)
		{
			Sleep(1000);
			continue;
		}
		
		WaitForSingleObject(pThis->safeguard__, INFINITE);
		if(!pThis->lstSendSuccess.empty())
		{
			std::string number  = pThis->lstSendSuccess.front();
			pThis->lstSendSuccess.pop_front();
			fprintf(m_pReportOk, "%s\n", number.c_str());
		}
		if(!pThis->lstSendFailed.empty())
		{
			std::string number  = pThis->lstSendFailed.front();
			pThis->lstSendFailed.pop_front();
			fprintf(m_pReportFail, "%s\n", number.c_str());

		}

		ReleaseMutex(pThis->safeguard__);
		fclose(m_pReportOk);
		fclose(m_pReportFail);
		Sleep(10);
	}
	return 0;
}

int Reporter::GenerateReport()
{
	this->stop();
	return 0;
}

Reporter::Reporter(void)
{
	safeguard__ = CreateMutex(NULL, FALSE, NULL);
	isRunning = false;
}


Reporter::~Reporter(void)
{
}
