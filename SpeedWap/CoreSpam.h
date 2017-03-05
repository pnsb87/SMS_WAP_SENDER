#pragma once

#include "comport.h"
#include "wapform.h"

#define MAX_NUMBER_COM_PORT 40
#define SMS_MODE 0
#define WAP_MODE 1

#define WM_END_THREAD_SEND WM_USER +1

class CSpeedWSDlg;

class CCoreSpam
{
public:
	CCoreSpam(CSpeedWSDlg* pUser);
	~CCoreSpam(void);

	int InitCore();

	int SetContex(const char* szMsg);
	int SetTextVisible(const char* szText);
	int SetType(int nMode);

	int StartSpam();
	int StopSpam();

	int getSpamMode() {return m_nMode;};

	const char * WrapWapMsg(const char* szMobile, const char* szText, const char* szUrl);

	char* GetNumberPtr(int nPos);
	char* GetContextPtr(){return m_szContext;};
	char* GetTextVisible(){return m_szTextVisible;};

	int GetWapLen() {return m_WapForm.m_nWapLen;};

	void NotifySpam(const char* szMsg);
	void NotifyStatus(const char* szMsg);

public:

	CSpeedWSDlg*	m_pSpeedWS;
	char	m_szContext[1500];
	char	m_szTextVisible[100];
	CWapForm m_WapForm;

	int m_nMode;

	//Lien quan den dau so
	int m_nPhoneNoCount;
	char m_szListNumber[300000][15];

	//Lien quan den quan ly COM
	int m_nAvailablePort;
	CComPort m_ListCOM[20];
};
