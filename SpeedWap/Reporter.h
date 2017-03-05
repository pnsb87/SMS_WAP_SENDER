#ifndef __REPORTER_H
#define __REPORTER_H
#include <vector>
#include <Windows.h>
class Reporter
{
public:
	static Reporter* instance();
	void Add2Report(std::string number, bool bSuccess);
	int GenerateReport();
	~Reporter(void);
	
private:
	Reporter(void);
	static Reporter* instance_;
	std::vector<std::string> lstSendSuccess;
	std::vector<std::string> lstSendFailed;
	HANDLE safeguard__;
};
#endif

