#ifndef __THREADMANAGER_H
#define __THREADMANAGER_H
#include "ActionThread.h"
#include <map>
#include <vector>

typedef std::map<DWORD, ActionThread*> THREAD_MAP_MANAGER;
typedef std::map<DWORD, ActionThread*>::iterator THREAD_MAP_MANAGER_ITER;

typedef std::vector<ActionThread*>	THREAD_VECTOR_MANAGER;
class ThreadManager
{
public:
	static ThreadManager* instance();

	void Add(ActionThread* thread);
	void AddThread(ActionThread* thread);

	ActionThread* Get(DWORD dwThreadID);
	ActionThread* GetThread(int nPos);
	ActionThread* GetThread(string szName);
	ActionThread* GetEnableThread(int nPos);
	ActionThread* GetEnableThread(string szName);

	int GetNoThreadEnable();

	int GetSize();

	void StartAll();
	void StopAll();
	void Exit();
	void pause(bool flag);
	bool isPaused();
	bool IsRunning();

	ThreadManager(void);
	~ThreadManager(void);

	void UpdateThreadList();

private:
	static ThreadManager* instance_;
	THREAD_MAP_MANAGER m_ThreadMap;
	bool bPaused;
	THREAD_VECTOR_MANAGER	m_vThreadMap;
	THREAD_VECTOR_MANAGER	m_vThreadMapEnable;
};
#endif


