#include "StdAfx.h"
#include "Log.h"
#include <ctime>

CLog* CLog::m_pInstance = NULL;
CLog::CLog(void)
{
	m_pLogFile = NULL;
}

CLog::~CLog(void)
{
	if(m_pLogFile)
		fclose(m_pLogFile);
}

CLog* CLog::GetInstance()
{
	if(!m_pInstance)
		m_pInstance = new CLog();

	return m_pInstance;	
}

void CLog::RemoveInstance()
{
	delete m_pInstance;
	m_pInstance = NULL;
}

void CLog::Log(char *psz,...)
{
	//Mo file log
	char szLogName[200];
	time_t now = time(0);
	tm *gmtm = gmtime(&now);

	sprintf(szLogName, "System_Log_Day %d_Month %d_Year %d____.txt"
		, gmtm->tm_mday, gmtm->tm_mon +1, gmtm->tm_year + 1900);

	m_pLogFile = fopen(szLogName, "a");

	char lst[256];
	va_list marker;
	va_start(marker,psz);
	vsprintf(lst,psz,marker);
	va_end(marker);

	//fwrite(lst, 1, sizeof(lst), m_pLogFile);
	fprintf(m_pLogFile, "%s\n", lst);

	fclose(m_pLogFile);
	
}