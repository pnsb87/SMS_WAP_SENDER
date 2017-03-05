#ifndef __ACTIONTHREAD_H
#define __ACTIONTHREAD_H
#include <string>

using namespace std;
class ActionThread
{
public:
	ActionThread(void);
	ActionThread(std::string name);
	int Init();
	int Start();
	virtual void run() = 0;
	int Stop();
	DWORD getThreadID()
	{
		return dwThreadId;
	}
	std::string GetName();
	void setName(string sName);
	bool isRunning();
	bool isPaused();
	void SetActive(bool bActive){ m_bActive = bActive;};
	bool GetStatus(){return m_bActive;};
	void pause(bool flag);
	~ActionThread(void);
protected:
	static DWORD WINAPI Entry(void* arg);
	HANDLE hThread;
	DWORD dwThreadId;
	bool isRunning__;
	bool bPaused;
	bool	m_bActive;

	std::string m_sThreadName;
};

#endif
