#include "StdAfx.h"
#include "ThreadManager.h"
#include "UsbInterface.h"

ThreadManager* ThreadManager::instance_ = NULL;
ThreadManager::ThreadManager(void)
{
	m_vThreadMap.clear();
	bPaused = false;
}

ThreadManager* ThreadManager::instance()
{
	if(instance_ == NULL)
		instance_ = new ThreadManager();
	return instance_;
}

void ThreadManager::Add(ActionThread* thread)
{
	assert(thread != NULL);
	THREAD_MAP_MANAGER_ITER iter = m_ThreadMap.find(thread->getThreadID());
	if(iter == m_ThreadMap.end())
		return;
	m_ThreadMap[thread->getThreadID()] = thread;
}

void ThreadManager::AddThread(ActionThread *thread)
{
	m_vThreadMap.insert(m_vThreadMap.end(), thread);
}

ActionThread* ThreadManager::Get(DWORD dwThreadID)
{
	THREAD_MAP_MANAGER_ITER iter = m_ThreadMap.find(dwThreadID);
	if(iter == m_ThreadMap.end())
		return NULL;
	ActionThread* thread = iter->second;
	return thread;
}

ActionThread* ThreadManager::GetThread(int nPos)
{
	return m_vThreadMap.at(nPos);
}

ActionThread* ThreadManager::GetEnableThread(int nPos)
{
	return m_vThreadMapEnable.at(nPos);
}

ActionThread* ThreadManager::GetThread(string szName)
{
	for(int i=0; i<m_vThreadMap.size(); i++)
	{
		if(szName == m_vThreadMap.at(i)->GetName())
			return m_vThreadMap.at(i);
	}

	return NULL;
}

ActionThread* ThreadManager::GetEnableThread(string szName)
{
	for(int i=0; i<m_vThreadMapEnable.size(); i++)
	{
		if(szName == m_vThreadMapEnable.at(i)->GetName())
			return m_vThreadMapEnable.at(i);
	}

	return NULL;	
}

int ThreadManager::GetSize()
{
	return m_vThreadMap.size();
}

int ThreadManager::GetNoThreadEnable()
{
	int nNumberThread = 0;

	for(int i=0; i< GetSize(); i++)
	{
		if(m_vThreadMap.at(i)->GetStatus())
			nNumberThread++;
	}
	
	return nNumberThread;
}

void ThreadManager::UpdateThreadList()
{
	m_vThreadMapEnable.clear();
	for(int i=0; i< GetSize(); i++)
	{
		if(m_vThreadMap.at(i)->GetStatus())
			m_vThreadMapEnable.insert(m_vThreadMapEnable.end(), m_vThreadMap.at(i));
	}
}

void ThreadManager::StartAll()
{
	//THREAD_MAP_MANAGER_ITER iter;
	//for(iter = m_ThreadMap.begin(); iter != m_ThreadMap.end(); iter++)
	//{
	//	ActionThread* thread = iter->second;
	//	if(thread)
	//	{
	//		thread->Start();
	//	}
	//}

	for(int i=0; i< GetSize(); i++)
	{
		CUsbInterface* pThread = (CUsbInterface*)GetThread(i);
		pThread->Init();
		pThread->Start();
		//pThread->startStatusReport();
		
	}
}

void ThreadManager::Exit()
{
	for(int i=0; i< GetSize(); i++)
	{
		CUsbInterface* pThread = (CUsbInterface*)GetThread(i);
		pThread->exit();
		pThread->Stop();
	}
}

void ThreadManager::StopAll()
{
	//THREAD_MAP_MANAGER_ITER iter;
	//for(iter = m_ThreadMap.begin(); iter != m_ThreadMap.end(); iter++)
	//{
	//	ActionThread* thread = iter->second;
	//	if(thread)
	//	{
	//		thread->Stop();
	//	}
	//}

	for(int i=0; i< GetSize(); i++)
	{
		CUsbInterface* pThread = (CUsbInterface*)GetThread(i);
		//pThread->exit();
		pThread->Stop();
	}
}
void ThreadManager::pause(bool flag)
{
	bPaused = flag;

	for(int i=0; i< GetSize(); i++)
	{
		CUsbInterface* pThread = (CUsbInterface*)GetThread(i);
		if(pThread->isRunning())
		{
			pThread->pause(flag);
		}
	}
}
bool ThreadManager::isPaused()
{
	return bPaused;
}
bool ThreadManager::IsRunning()
{
	for(int i=0; i<GetSize(); i++)
	{
		if(m_vThreadMap.at(i)->isRunning())
			return true;
	}

	return false;
}



ThreadManager::~ThreadManager(void)
{
	StopAll();
	for(int i=0; i< GetSize(); i++)
	{
		ActionThread* pThread = GetThread(i);
		delete pThread;
		pThread = NULL;
	}
}
