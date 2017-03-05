#pragma once

class CCoreSpam;

class CComPort
{
public:
	CComPort(CCoreSpam* pUser, HANDLE hCom = NULL);
	CComPort(void);
	~CComPort(void);

	int InitPhoneCom(const char* szComName);
	
	bool OpenPort(CString portname);
	void ClosePort();
	bool ReadByte(BYTE &resp);
	int WriteByte(const char* szMessage);
	int WriteByteNoReturn(const char* szMessage);
	int WriteBytes(BYTE* msg, int len);
	int SpamSMS(const char* szMessage);

	int SpamWAP(const char* szMsg, int len);
	
	bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout,DWORD ReadTotalTimeoutMultiplier,DWORD ReadTotalTimeoutConstant,DWORD WriteTotalTimeoutMultiplier,DWORD WriteTotalTimeoutConstant);
	bool ConfigurePort(DWORD BaudRate,BYTE ByteSize,DWORD fParity,BYTE  Parity,BYTE StopBits);

	void NotifySpam(const char* szMsg);
	void NotifyStatus(const char* szMsg);

	void SetListNoPtr(int nStart, int nCount);

	char* GetNumberPtr(int nPos);
	char* GetContextPtr();
	char* GetTextVisible();

	void StartSpam();
	void StopSpam() ;

	static UINT SpamSMSThread(void* pUser);
	static UINT SpamWAPThread(void* pUser);
	static UINT ResultThread(void *pUser);

public:

	CCoreSpam* m_pCoreSpam;

	bool	m_bAllowSpam;

	char	m_szComName[10];

	HANDLE m_hComm;
	DCB      m_dcb;
	COMMTIMEOUTS m_CommTimeouts;
	bool     m_bPortReady;
	bool     bWriteRC;
	bool     bReadRC;
	DWORD iBytesWritten;
	DWORD iBytesRead;
	DWORD dwBytesRead;

	//Luu danh sach dau so
	char* m_pNumberList;
	int m_nTotalNumber;
	int m_nStartPos;

	int m_nSuccessSMS;
	int m_nSentCount;
};
