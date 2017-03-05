#pragma once

#include "actionthread.h"
#include "SerialCommunication.h"
#include <vector>
#include <string>
#include "msgSender.h"

using namespace std;

class CCoreManager;
class CLog;

class CUsbInterface : public ActionThread
{
public:
	CUsbInterface(CCoreManager* pManager);
	CUsbInterface(void);
	~CUsbInterface(void);

	virtual void run();

	int InitPort(const char* szName);
	void SetRange(int nRange){ m_nSmsRange = nRange;};

	void SetNumberList(std::vector<string> vListNumber);
	void SetSimNumber(const char* szNumber);
	void SetProtocolId(const char* szProtocolID);
	void exit();
	char* getNumber(){return m_szNumber;};
	int getTotalSms(){return m_nTotalSms;};
	int getSmsOk(){return m_nSmsOK;};
	int getSmsFail(){return m_nSmsFail;}
	int getSmsRange(){return m_nSmsRange;};

	void SetConfig(int nMode, const char* szSmsText, const char* szWapUrl);

	void LogOnScreen(char *psz,...);

	void startStatusReport();
	int SendInstantSms(string sText, string sDestNum);
	int SendInstantWap(string sText, string url, string sDestNum);
	static DWORD WINAPI StatusReportThread(LPVOID arg);
	static void GetStatusReport(CStringA message, CString& number, int& isSuccess);
	static CStringA DecodePDUPhoneNumber(CStringA PDUPhoneNumber);
	void ProcessSMSResponse(CString content);
public:
	CCoreManager*	m_pManager;
	CLog*			m_pLog;
	CMsgSender		m_Sender;

	char	m_szNumber[20];
	int		m_nTotalSms;
	int		m_nSmsOK;
	int		m_nSmsFail;
	int		m_nSmsRange;
	HANDLE  hThread;
	bool	isReading;
	SerialCommunication m_ComPort;

	std::vector<string>		m_vListNumber;
};
