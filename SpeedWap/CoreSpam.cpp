#include "StdAfx.h"
#include "CoreSpam.h"
#include "SpeedWSDlg.h"
#include "ComPort.h"

#include<iostream>
#include<fstream>

using namespace std;

CCoreSpam::CCoreSpam(CSpeedWSDlg* pUser)
{
	m_pSpeedWS = pUser;

	memset(&m_szContext, 0, 1500);

	m_nPhoneNoCount = 0;
	m_nAvailablePort = 0;
}

CCoreSpam::~CCoreSpam(void)
{
}

int CCoreSpam::InitCore()
{
	char szNotify[50];

	//Get mobile phone number to spam
	ifstream readFile;
	readFile.open("Dau so.txt");

	char szNumber[20];
	memset(&szNumber, 0, 12);

	while(!readFile.eof())
	{
		readFile >> &m_szListNumber[m_nPhoneNoCount][0];
		if(strcmp(&m_szListNumber[m_nPhoneNoCount][0], "") == 0)
			continue;
		m_nPhoneNoCount++;
	}

	//Get COM port to Spam
	char szComName[10];
	HANDLE hCom = NULL;
	for(int i=0;i<MAX_NUMBER_COM_PORT;i++)
	{
		if(i<10)
			sprintf(szComName, "COM%d", i);
		else
			sprintf(szComName, "\\\\.\\COM%d", i);

		if(i == 25)
			TRACE("test");

		hCom = CreateFile(szComName,  
            GENERIC_READ|GENERIC_WRITE, // desired access should be read&write  
            0,                          // COM port must be opened in non-sharing mode  
            NULL,                       // don't care about the security  
            OPEN_EXISTING,              // IMPORTANT: must use OPEN_EXISTING for a COM port  
            0,                          // usually overlapped but non-overlapped for existance test  
            NULL);    

        if (INVALID_HANDLE_VALUE == hCom)  
		{
			//CloseHandle(hCom); 
			sprintf(szNotify, "%s can't not get handle.", szComName);
			NotifySpam(szNotify);
		}
        else  
        {   // Kiem tra COM co gui dc AT command ko
			//CloseHandle(hCom); 
			CComPort* pComPort = new CComPort(this, hCom);
			if(pComPort->InitPhoneCom(szComName) != 0)
			{	
				//Khoi tao loi, chuyen sang COM khac
				sprintf(szNotify, "%s Init ERROR!!!", szComName);
				NotifySpam(szNotify);

				delete pComPort;
				pComPort = NULL;

				//CloseHandle(hCom); 

				continue;
			}
			else
			{
				//Khoi tao thanh cong, cho vao danh sach
				m_ListCOM[m_nAvailablePort] = *pComPort;
				m_nAvailablePort++;

				sprintf(szNotify, "%s Init Successfully", szComName);
				NotifySpam(szNotify);
			}
        } 
	}

	return 0;
}

int CCoreSpam::SetType(int nMode)
{
	m_nMode = nMode;
	return 0;
}

int CCoreSpam::SetContex(const char* szMsg)
{
	if(strcmp(szMsg, "") == 0)
		return -1;

	if(m_nMode == SMS_MODE)
		sprintf(m_szContext, "%s%c", szMsg, (char)26);
	else if(m_nMode == WAP_MODE)
		sprintf(m_szContext, "%s", szMsg);

	return 0;
}

int CCoreSpam::SetTextVisible(const char* szText)
{
	if(strcmp(szText, "") == 0)
		sprintf(m_szTextVisible, "%s", "Check it out right now!!!!");
	else
		sprintf(m_szTextVisible, "%s", szText);

	return 0;
}

char* CCoreSpam::GetNumberPtr(int nPos)
{
	return &m_szListNumber[nPos][0];
}

const char* CCoreSpam::WrapWapMsg(const char* szMobile, const char* szText, const char* szUrl)
{
	//gn_wap_push wp;
	//memset(&wp, 0, sizeof(gn_wap_push));

	//wp.url = (char*)szMsg;
	//wp.text = "wappush test";

	////tao header cho wappush
	//m_WapForm.WapPushInit(&wp);

	//if(m_WapForm.WapPushEncode(&wp) == 0)
	//	return wp.data;

	return m_WapForm.GetPDU(szMobile, szText, szUrl);
}

int CCoreSpam::StartSpam()
{
	if(m_nAvailablePort == 0)
	{
		NotifyStatus("No port to spam");
		return -1;
	}

	int nStep = m_nPhoneNoCount/m_nAvailablePort;

	for(int i=0; i<m_nAvailablePort; i++)
	{
		m_ListCOM[i].SetListNoPtr(nStep*i, nStep);
		m_ListCOM[i].StartSpam();
	}

	return 0;
}

int CCoreSpam::StopSpam()
{
	for(int i=0; i<m_nAvailablePort; i++)
		m_ListCOM[i].StopSpam();

	return 0;
}

void CCoreSpam::NotifySpam(const char* szMsg)
{
	if(m_pSpeedWS)
		m_pSpeedWS->NotifySpam(szMsg);
}

void CCoreSpam::NotifyStatus(const char* szMsg)
{
	if(m_pSpeedWS)
		m_pSpeedWS->NotifyStatus(szMsg);
}