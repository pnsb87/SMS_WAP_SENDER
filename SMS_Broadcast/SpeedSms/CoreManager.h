#pragma once

#include "UsbInterface.h"

#include<fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CSpeedSmsDlg;
class CTabDlgSetting;
class CTabDlgSms;
class CInstantMsg;
class CInstantWap;
class ThreadManager;

class CLog;

#define MAX_NUMBER_COM 40

class CCoreManager
{
public:
	CCoreManager(void);
	~CCoreManager(void);

	int Init();
	int InitCom();
	int InitNumberFile();
	void ExportLog();

	void SetSmsRange(const char* szName, int nRange);
	void SetComActive(const char* szName, int nActive);

	void SetParent(CSpeedSmsDlg* pDlg, CTabDlgSetting* pSettingDlg, CTabDlgSms* pSmsDlg
		, CInstantMsg* pInstSms, CInstantWap* pInstWap);

	void Start();
	void Stop();
	void Pause(bool flag);
	void Exit();
	bool IsRunning();
	void LogOnScreen(char *psz,...);

	void CoreConfig(int nMode, const char* szProtocolId, const char* szSmsText, const char* szWapUrl);

	int SendInstantMsg(const char* szCom, const char* szText, const char* szNumber, const char* szProID);
	int SendInstantWap(const char* szCom, const char* szText, const char* szUrl, const char* szNumber);

	ThreadManager*	GetUsbManager(){return m_pThreadManager;};

public:
	CSpeedSmsDlg*		m_pSpeedSms;
	CTabDlgSetting*		m_pSetting;
	CTabDlgSms*			m_pSMS;
	CInstantMsg*		m_pInstantMsg;
	CInstantWap*		m_pInstantWap;

	CUsbInterface		m_ListUsb[20];
	int					m_nNumberUsb;
	std::vector<string>		m_ListNumber;

	CLog*				m_pLog;
	ThreadManager*		m_pThreadManager;
};
