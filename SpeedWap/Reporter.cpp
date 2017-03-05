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

int Reporter::GenerateReport()
{
	return 0;
}
Reporter::Reporter(void)
{
	safeguard__ = CreateMutex(NULL, FALSE, NULL);
}


Reporter::~Reporter(void)
{
}
