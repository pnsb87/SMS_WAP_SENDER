#include "StdAfx.h"
#include "ActionThread.h"


ActionThread::ActionThread(void)
{
	isRunning__ = false;
	bPaused = false;
	hThread = INVALID_HANDLE_VALUE;
}

int ActionThread::Init()
{
	if(hThread != INVALID_HANDLE_VALUE)
		return -1;
	hThread = CreateThread(NULL, 0, Entry, this, CREATE_SUSPENDED, &dwThreadId);
	return 0;
}
bool ActionThread::isRunning()
{
	return this->isRunning__;
}

bool ActionThread::isPaused()
{
	return bPaused;
}

void ActionThread::pause(bool flag)
{
	bPaused = flag;
}

int ActionThread::Start()
{
	if(isRunning__)
		return -1;
	assert(hThread != INVALID_HANDLE_VALUE);
	isRunning__ = true;
	ResumeThread(hThread);
	return 0;
}

DWORD ActionThread::Entry(void* arg)
{
	ActionThread* pThis = (ActionThread*)arg;
	pThis->run();
	/*if(pThis->isRunning__)
	{
		pThis->isRunning__ = false;
		if(pThis->hThread)
		{
			WaitForSingleObject(pThis->hThread, 5000);
			CloseHandle(pThis->hThread);
			pThis->hThread = INVALID_HANDLE_VALUE;
		}
	}*/
	return 0;
}

int ActionThread::Stop()
{
	if(!isRunning__)
		return -1;
	
	isRunning__ = false;
	if(hThread != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(hThread, 5000);
		CloseHandle(hThread);
		hThread = INVALID_HANDLE_VALUE;
	}
	return 0;

}
ActionThread::~ActionThread(void)
{
	if(isRunning__)
	{
		Stop();
	}
}

string ActionThread::GetName()
{
	return m_sThreadName;
}

void ActionThread::setName(string sName)
{
	m_sThreadName = sName;
}
