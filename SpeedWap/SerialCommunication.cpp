#include "StdAfx.h"
#include "SerialCommunication.h"


SerialCommunication::SerialCommunication()
{
	m_serialParam.hCom = INVALID_HANDLE_VALUE;
}

int SerialCommunication::Open(CString name)
{
	m_serialParam.szName = name;
	m_serialParam.hCom = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(m_serialParam.hCom == INVALID_HANDLE_VALUE)
		return -1;
	BOOL isReady = SetupComm(m_serialParam.hCom, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE);
	if(!isReady)
		return -1;

	DCB m_dcb;
	isReady = GetCommState(m_serialParam.hCom, &m_dcb);
	m_dcb.BaudRate = 9600;
	m_dcb.ByteSize = 8;
	m_dcb.Parity = NOPARITY;
	m_dcb.StopBits = ONESTOPBIT;
	m_dcb.fAbortOnError = 0;

	isReady = SetCommState(m_serialParam.hCom, &m_dcb);
	if(!isReady)
		return -1;
	isReady = GetCommTimeouts (m_serialParam.hCom, &m_serialParam.m_CommTimeouts);
	if(!isReady)
		return -1;
	m_serialParam.m_CommTimeouts.ReadIntervalTimeout = 50;
	m_serialParam.m_CommTimeouts.ReadTotalTimeoutConstant = 50;
	m_serialParam.m_CommTimeouts.ReadTotalTimeoutMultiplier = 10;
	m_serialParam.m_CommTimeouts.WriteTotalTimeoutConstant = 50;
	m_serialParam.m_CommTimeouts.WriteTotalTimeoutMultiplier = 10;

	isReady = SetCommTimeouts (m_serialParam.hCom, &m_serialParam.m_CommTimeouts);
	if(!isReady)
		return -1;

	return 0;
}

int SerialCommunication::Close()
{
	if(m_serialParam.hCom != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_serialParam.hCom);
		m_serialParam.hCom = INVALID_HANDLE_VALUE;
	}
	return 0;
}

int SerialCommunication::Write(const char* data, int len)
{
	DWORD nWriten = 0;
	WriteFile(m_serialParam.hCom,(LPCVOID)data, len, &nWriten, NULL);
	return nWriten;
}

int SerialCommunication::Read(char* dataout, int dataout_len)
{
	DWORD nRead;
	ReadFile(m_serialParam.hCom, (LPVOID)dataout, dataout_len, &nRead, NULL);
	return nRead;
}

SerialCommunication::~SerialCommunication(void)
{
}
