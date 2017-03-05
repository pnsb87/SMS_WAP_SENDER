#pragma once

class CLog
{
public:
	CLog(void);
	~CLog(void);

	static CLog*	GetInstance();
	static void		RemoveInstance();

	void Log(char *psz,...);

public:
	static CLog*	m_pInstance;

	FILE*			m_pLogFile;
};
