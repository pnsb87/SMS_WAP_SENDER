#include "StdAfx.h"
#include "ComPort.h"
#include "CoreSpam.h"
#include "WAPPushPDU.h"
CComPort::CComPort(CCoreSpam* pUser, HANDLE hCom)
{
	m_pCoreSpam = pUser;
	m_hComm = hCom;

	m_bAllowSpam = true;
	m_nSuccessSMS = 0;
	m_nSentCount = 0;
}

CComPort::CComPort(void)
{

}

CComPort::~CComPort(void)
{
	//CloseHandle(m_hComm);
	m_bAllowSpam = false;
}

void CComPort::SetListNoPtr(int nStart, int nCount)
{
	m_nStartPos = nStart;
	m_nTotalNumber = nCount;
}

char* CComPort::GetNumberPtr(int nPos)
{
	return m_pCoreSpam->GetNumberPtr(nPos);
}

char* CComPort::GetContextPtr()
{
	return m_pCoreSpam->GetContextPtr();
}

char* CComPort::GetTextVisible()
{
	return m_pCoreSpam->GetTextVisible();
}

int CComPort::InitPhoneCom(const char* szComName)
{
	char szNotify[50];
	//Send AT command to test
	memcpy(&m_szComName, szComName, 10);

	//if(!OpenPort(CString(szComName)))
	//	return -1;

	if(!ConfigurePort(9600, 8, 0, NOPARITY, ONE5STOPBITS))
		return -2;

	if(!SetCommunicationTimeouts(0,100,0,0,0))
		return -3;

	if(WriteByte("AT") != 0)
		return -4;

	if(WriteByte("AT+CMGF=1") != 0)
	{
		sprintf(szNotify, "%s not support SMS text.", m_szComName);
		return -5;
	}

	return 0;
}

bool CComPort::OpenPort(CString portname)
{
//portname= "//./" +portname;

	m_hComm = CreateFile(portname,
						  GENERIC_READ | GENERIC_WRITE,
						  0,
						  0,
						  OPEN_EXISTING,
						  0,
						  0);
			if(m_hComm==INVALID_HANDLE_VALUE){
			//MessageBox("Cannot open Communication Port.Please\nQuit the program and Re-start your PC.","Com Port Error",MB_OK+MB_ICONERROR);
			return false;}
			else
				return true;

}

bool CComPort::ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits)
{
	if((m_bPortReady = GetCommState(m_hComm, &m_dcb))==0)
	{
		CloseHandle(m_hComm);
		return false;
	}

	m_dcb.BaudRate =BaudRate;
	m_dcb.ByteSize = ByteSize;
	m_dcb.Parity =Parity ;
	m_dcb.StopBits =StopBits;
	m_dcb.fBinary=TRUE;
	m_dcb.fDsrSensitivity=false;
	m_dcb.fParity=fParity;
	m_dcb.fOutX=false;
	m_dcb.fInX=false;
	m_dcb.fNull=false;
	m_dcb.fAbortOnError=TRUE;
	m_dcb.fOutxCtsFlow=FALSE;
	m_dcb.fOutxDsrFlow=false;
	m_dcb.fDtrControl=DTR_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity=false;
	m_dcb.fRtsControl=RTS_CONTROL_DISABLE;
	m_dcb.fOutxCtsFlow=false;
	m_dcb.fOutxCtsFlow=false;

	m_bPortReady = SetCommState(m_hComm, &m_dcb);
	if(m_bPortReady ==0)
	{
		//MessageBox("SetCommState Error","Error",MB_OK+MB_ICONERROR);
 		CloseHandle(m_hComm);
		return false;
	}

	return true;
}

bool CComPort::SetCommunicationTimeouts(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant)
{
	if((m_bPortReady = GetCommTimeouts (m_hComm, &m_CommTimeouts))==0)
	   return false;

	m_CommTimeouts.ReadIntervalTimeout =ReadIntervalTimeout;
	m_CommTimeouts.ReadTotalTimeoutConstant =ReadTotalTimeoutConstant;
	m_CommTimeouts.ReadTotalTimeoutMultiplier =ReadTotalTimeoutMultiplier;
	m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
	m_CommTimeouts.WriteTotalTimeoutMultiplier =WriteTotalTimeoutMultiplier;
	m_bPortReady = SetCommTimeouts (m_hComm, &m_CommTimeouts);
			
	if(m_bPortReady ==0)
	{
		//MessageBox("StCommTimeouts function failed","Com Port Error",MB_OK+MB_ICONERROR);
		CloseHandle(m_hComm);
		return false;
	}
	return true;
}
int CComPort::WriteBytes(BYTE* msg, int len)
{
	iBytesWritten=0;
	DWORD dwBytesTransferred=0;
	char szReturn[140];
	memset(&szReturn, 0, 140);
	if(WriteFile(m_hComm, (LPCVOID)msg, len/*buf.GetLength()*/, &iBytesWritten, NULL)==0)
		return -1;
	else 
	{
		if(ReadFile(m_hComm, (LPVOID)&szReturn, 140, &dwBytesTransferred, 0))
		{
			if(strstr(szReturn, "CMGW:") != NULL)
			{
				int nMsgPos = atoi(strstr(szReturn, ":") + 2);
				return nMsgPos;
			}
			else if(strstr(szReturn, "RSSI") != NULL)
				return -2;
			else if((strstr(szReturn, "OK") !=NULL) || (strstr(szReturn, "\r\n>") !=NULL))
				return 0;
			else 
				return -2;
		}
		else 
			return -3;
	}
}
int CComPort::WriteByteNoReturn(const char* szMessage)
{
	iBytesWritten=0;
	DWORD dwBytesTransferred=0;
	char szSend[320] = {0};

	sprintf(szSend, "%s\r",szMessage);
	int len = strlen(szSend);

	if(WriteFile(m_hComm, (LPCVOID)&szSend, len/*buf.GetLength()*/, &iBytesWritten, NULL)==0)
		return -1;
	return 0;
}

int CComPort::WriteByte(const char* szMessage)
{
	iBytesWritten=0;
	DWORD dwBytesTransferred=0;
	char szReturn[140];
	char szSend[320];
	memset(&szSend, 0, 140);
	memset(&szReturn, 0, 140);

	//CString buf(szMessage);
	
	sprintf(szSend, "%s\r",szMessage);
	int len = strlen(szSend);

	if(WriteFile(m_hComm, (LPCVOID)&szSend, len/*buf.GetLength()*/, &iBytesWritten, NULL)==0)
		return -1;
	else 
	{
		memset(&szSend, 0, 140);
		if(ReadFile(m_hComm, (LPVOID)&szReturn, 140, &dwBytesTransferred, 0))
		{
			if(strstr(szReturn, "CMGW:") != NULL)
			{
				int nMsgPos = atoi(strstr(szReturn, ":") + 2);
				return nMsgPos;
			}
			else if(strstr(szReturn, "RSSI") != NULL)
				return -2;
			else if((strstr(szReturn, "OK") !=NULL) || (strstr(szReturn, "\r\n>") !=NULL))
				return 0;
			else 
				return -2;
		}
		else 
			return -3;
	}

	return 0;
}

int CComPort::SpamSMS(const char* szMessage)
{
	iBytesWritten=0;
	char szSend[140];
	memset(&szSend, 0, 140);
	
	sprintf(szSend, "%s\r",szMessage);

	if(WriteFile(m_hComm, (LPCVOID)&szSend, strlen(szSend), &iBytesWritten, NULL)==0)
		return -1;

	return 0;
}

int CComPort::SpamWAP(const char* szMsg, int len)
{
	char szMessage[140];
	memset(&szMessage, 0, 140);

	if(WriteByte("AT+CMGF=0") != 0)
		return -1;

	sprintf(szMessage, "AT+CMGS=%d", len);

	if(WriteByte(szMessage) != 0)
		return -2;

	if(WriteByte(szMsg) !=0)
		return -3;

	return 0;
}


bool CComPort::ReadByte(BYTE &resp)
{
	BYTE rx;
	resp=0;

	DWORD dwBytesTransferred=0;

	if (ReadFile(m_hComm, &rx, 1, &dwBytesTransferred, 0))
	{
		 if (dwBytesTransferred == 1)
		 {
			 resp=rx;
			 return true;
		 }
	}
				  
	return false;
}

void CComPort::StartSpam()
{
	m_bAllowSpam = true;
	if(m_pCoreSpam->getSpamMode() == SMS_MODE)
		AfxBeginThread(SpamSMSThread, this);
	else
		AfxBeginThread(SpamWAPThread, this);
}

void CComPort:: StopSpam() 
{ 
	m_bAllowSpam = false;
	m_nSuccessSMS = 0;
	m_nSentCount = 0;
}

UINT CComPort::SpamSMSThread(void* pUser)
{
	CComPort* pThis = (CComPort*)pUser;
	int nRet = 0;
	int nMsgPos;
	char szNotify[50];
	char szMessage[50];

	//Gui dinh dang text 
	//nRet = pThis->WriteByte("AT+CMGF=1");
	//if(nRet)
	//{
	//	sprintf(szNotify, "%s not support SMS text.", pThis->m_szComName);
	//	return 0;
	//}

	//Strore SMS to SIM
	//sprintf(szMessage, "AT+CMGW=\"%s\"")

	nRet = pThis->WriteByte("AT+CGSMS=2");
	if(nRet)
	{
		sprintf(szNotify, "%s not support GPRS.", pThis->m_szComName);
		pThis->NotifySpam(szNotify);
		//return 0;
	}

	sprintf(szNotify, "%s is now store SMS to SIM....", pThis->m_szComName);
	pThis->NotifySpam(szNotify);

	nRet = pThis->WriteByte("AT+CMGW=\"0904962930\"");
	if(nRet)
	{
		sprintf(szNotify, "%s not support store SMS to SIM.", pThis->m_szComName);
		pThis->NotifySpam(szNotify);
		return 0;
	}

	//Store tin nhan luon
	nMsgPos = pThis->WriteByte(pThis->GetContextPtr());
	if(nMsgPos < 0)
	{
		sprintf(szNotify, "%s not support store SMS to SIM.", pThis->m_szComName);
		pThis->NotifySpam(szNotify);
		return 0;
	}

	sprintf(szNotify, "%s has store SMS to SIM, start spamming...", pThis->m_szComName);
	pThis->NotifySpam(szNotify);

	AfxBeginThread(ResultThread, pUser);

	while(pThis->m_bAllowSpam && (pThis->m_nSentCount < pThis->m_nTotalNumber))
	{
		//Lay so can gui tiep theo
		char* szNumber = pThis->GetNumberPtr(pThis->m_nStartPos + pThis->m_nSentCount);

		//sprintf(szMessage, "AT+CMGS=\"%s\"", szNumber);

		////Gui di dau so nhan SMS
		//nRet = pThis->WriteByte(szMessage);
		//if(nRet)
		//{
		//	sprintf(szNotify, "%s cant send SMS to %s.", pThis->m_szComName, szNumber);
		//	pThis->NotifySpam(szNotify);
		//	nSentCount++;

		//	continue;
		//}

		////Gui noi dung SMS
		//nRet = pThis->WriteByte(pThis->GetContextPtr());
		//if(nRet)
		//{
		//	sprintf(szNotify, "%s cant send SMS to %s !!!", pThis->m_szComName, szNumber);
		//	pThis->NotifySpam(szNotify);
		//	nSentCount++;

		//	continue;
		//}

		sprintf(szMessage, "AT+CMSS=%d,\"%s\"", nMsgPos, szNumber);

		nRet = pThis->SpamSMS(szMessage);
		if(nRet)
		{
			sprintf(szNotify, "%s cant send SMS to %s !!!", pThis->m_szComName, szNumber);
			pThis->NotifySpam(szNotify);
			pThis->m_nSentCount++;

			continue;
		}

		sprintf(szNotify, "%s send SMS to %s SUCCESSFULLY.", pThis->m_szComName, szNumber);
		pThis->NotifySpam(szNotify);

		pThis->m_nSentCount++;
	}

	pThis->m_nSentCount = 0;
	//::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_END_THREAD_SEND, 0, 0);

	return 0;
}

UINT CComPort::SpamWAPThread(void* pUser)
{
	CComPort* pThis = (CComPort*)pUser;
	int nRet = 0;
	int nMsgPos;
	char szNotify[50];
	char szWap[320];
	char szMsg[50];


	sprintf(szNotify, "%s config PDU mode....", pThis->m_szComName);
	pThis->NotifySpam(szNotify);

	if(pThis->WriteByte("AT+CMGF=0") != 0)
	{
		sprintf(szNotify, "%s not support PDU mode.", pThis->m_szComName);
		pThis->NotifySpam(szNotify);
		return -1;
	}

	sprintf(szNotify, "%s config PDU mode OK!!", pThis->m_szComName);
	pThis->NotifySpam(szNotify);

	while(pThis->m_bAllowSpam && (pThis->m_nSentCount < pThis->m_nTotalNumber))
	{
		//Lay so can gui tiep theo
		char* szNumber = pThis->GetNumberPtr(pThis->m_nStartPos + pThis->m_nSentCount);
		std::string pdu;
		int out_len = WAPPushPDU::getPDU(std::string(szNumber), std::string(pThis->GetContextPtr()), std::string(pThis->GetTextVisible()), pdu);
		//TRACE("%S\n", pdu.c_str());
		
		sprintf(szWap,"%s%c", pdu.c_str(), (char)26);
		
		sprintf(szMsg, "AT+CMGS=%d", out_len);
		sprintf(szNotify, "%s start send WAP to %s ....", pThis->m_szComName, szNumber);
		pThis->NotifySpam(szNotify);

		//nRet = pThis->WriteByte(szMsg);
		nRet = pThis->WriteByteNoReturn(szMsg);
		if(nRet)
		{
			sprintf(szNotify, "%s cant send WAP to %s !!!", pThis->m_szComName, szNumber);
			pThis->NotifySpam(szNotify);
			pThis->m_nSentCount++;

			continue;
		}

		nRet = pThis->WriteByte(szWap);
		if(nRet)
		{
			sprintf(szNotify, "%s cant send WAP to %s !!!", pThis->m_szComName, szNumber);
			pThis->NotifySpam(szNotify);
			pThis->m_nSentCount++;

			continue;
		}

		sprintf(szNotify, "%s send WAP to %s SUCCESSFULLY.", pThis->m_szComName, szNumber);
		pThis->NotifySpam(szNotify);

		pThis->m_nSentCount++;
	}

	pThis->m_nSentCount = 0;
	//::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_END_THREAD_SEND, 1, 0);

	return 0;
}

UINT CComPort::ResultThread(void* pUser)
{
	CComPort* pThis = (CComPort*)pUser;
	DWORD dwBytesTransferred=0;
	char szReturn[140];
	char szNotify[50];
	memset(&szReturn, 0, 140);
	
	while(pThis->m_bAllowSpam)
	{
		if(ReadFile(pThis->m_hComm, (LPVOID)&szReturn, 140, &dwBytesTransferred, 0))
		{
			if(strstr(szReturn, "OK") !=NULL)
				pThis->m_nSuccessSMS++;
			TRACE("\nSuccecss SMS: ");
		}	
	}

	return 0;
}

void CComPort::ClosePort()
{
	CloseHandle(m_hComm);
	return;
}

void CComPort::NotifySpam(const char* szMsg)
{
	if(m_pCoreSpam)
		m_pCoreSpam->NotifySpam(szMsg);
}

void CComPort::NotifyStatus(const char* szMsg)
{
	if(m_pCoreSpam)
		m_pCoreSpam->NotifyStatus(szMsg);
}

