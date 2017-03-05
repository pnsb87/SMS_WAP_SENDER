#ifndef __REPORTER_H
#define __REPORTER_H
#include <deque>
#include <Windows.h>
class Reporter
{
public:
	static Reporter* instance();
	void Add2Report(std::string number, bool bSuccess);
	int GenerateReport();
	void clear();
	void start();
	void stop();
	void clearLogFile();
	~Reporter(void);
	
private:
	static DWORD WINAPI ReportThread(LPVOID param);
	Reporter(void);
	static Reporter* instance_;
	std::deque<std::string> lstSendSuccess;
	std::deque<std::string> lstSendFailed;
	HANDLE hThread;
	HANDLE safeguard__;
	bool isRunning;
	
};
#endif

