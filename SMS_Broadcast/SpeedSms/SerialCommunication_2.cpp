#include "StdAfx.h"
#include "SerialCommunication.h"
#include "UsbInterface.h"
#include <string>

using namespace std;

SerialCommunication::SerialCommunication()
{
	m_serialParam.hCom = INVALID_HANDLE_VALUE;
}

string SerialCommunication::GetNumber()
{
	string sPhoneNumber;
		
	DWORD nWriten = 0;
	DWORD nRead = 0;
	char buffer[MAX_BUFFER_SIZE] = {0};
	string sRequest = "AT+CNUM=?\r";
	//string sReply;

	this->Write(sRequest.c_str(), sRequest.length());
	//WriteFile(m_serialParam.hCom,(LPCVOID)sRequest.c_str(), sRequest.length(), &nWriten, NULL);
	//ReadFile(m_serialParam.hCom, (LPVOID)sReply.c_str(), 100, &nRead, NULL);
	this->Read(buffer, MAX_BUFFER_SIZE);
	std::string sReply(buffer);

	sReply.erase(0, 8);
	sReply.erase(sReply.find("\""), sReply.length()-1);

	sPhoneNumber = sReply;

	return sPhoneNumber;
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
	m_dcb.StopBits = ONE5STOPBITS;
	m_dcb.fParity = 0;
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

	if(WriteWaitReply("AT\r") != 0)
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

int SerialCommunication::WriteWaitReply(const char *data)
{

	char szReply[MAX_BUFFER_SIZE] = {0};

	memset(szReply, 0, sizeof(szReply));
	int ret = 0;
	string szSend(data);


	ret = Write(szSend.c_str(), szSend.length());
	ret = Read(szReply, MAX_BUFFER_SIZE);

	if(strstr(szReply, "CMGW:") != NULL)
	{
		int nMsgPos = atoi(strstr(szReply, ":") + 2);
		return nMsgPos;
	}
	else if(strstr(szReply, "RSSI") != NULL)
		return -1;
	else if((strstr(szReply, "OK") !=NULL) || (strstr(szReply, "\r\n>") !=NULL))
		return 0;
	else 
		return -2;

	return 0;
}

int SerialCommunication::Read(char* dataout, int dataout_len)
{
	DWORD nRead;
	ReadFile(m_serialParam.hCom, (LPVOID)dataout, dataout_len, &nRead, NULL);
	return nRead;
}

int SerialCommunication::SendSms(const char *szText, const char *szNumber)
{
	char szSend[50];
	memset(szSend, 0, sizeof(szSend));

	//gui dau so truoc
	sprintf(szSend, "AT+CMGS=\"%s\"", szNumber);
	
	if(WriteWaitReply(szSend) != 0)
		return -1;

	//gui noi dung
	sprintf(szSend, "%s%c", szText, (char)26);

	if(WriteWaitReply(szSend) != 0)
		return -1;

	return 0;
}

int SerialCommunication::SendWap(const char *szText, const char *szUrl, const char *szNumber)
{

	return 0;
}

int SerialCommunication::ChangeSmsMode()
{
	if(WriteWaitReply("AT+CMGF=1") != 0)
		return -1;
	
	return 0;
}

int SerialCommunication::ChangeWapMode()
{
	if(WriteWaitReply("AT+CMGF=0") != 0)
		return -1;

	return 0;
}

SerialCommunication::~SerialCommunication(void)
{
}
