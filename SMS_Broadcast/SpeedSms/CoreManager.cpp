#include "StdAfx.h"
#include "CoreManager.h"
#include "SpeedSmsDlg.h"
#include "tabDlgSetting.h"
#include "tabDlgSms.h"
#include "instantMsg.h"
#include "Log.h"
#include "ThreadManager.h"

CCoreManager::CCoreManager(void)
{
	m_pSpeedSms = NULL;
	m_pSetting = NULL;
	m_pSMS = NULL;
	m_pInstantMsg = NULL;
	m_pInstantWap = NULL;
	m_nNumberUsb = 0;

	m_pLog = CLog::GetInstance();
	m_pThreadManager = NULL;
}

CCoreManager::~CCoreManager(void)
{
	delete m_pThreadManager;
	m_pThreadManager = NULL;

	delete m_pLog;
}

void CCoreManager::SetParent(CSpeedSmsDlg* pDlg, CTabDlgSetting* pSettingDlg, CTabDlgSms* pSmsDlg
							 , CInstantMsg* pInstSms, CInstantWap* pInstWap)
{
	m_pSpeedSms = pDlg;
	m_pSetting = pSettingDlg;
	m_pSMS = pSmsDlg;	
	m_pInstantMsg = pInstSms;
	m_pInstantWap = pInstWap;
}

int CCoreManager::Init()
{
	if(InitNumberFile() != 0)
		return -1;

	if(InitCom() != 0)
		return -1;

	return 0;
}

int  CCoreManager::InitCom()
{
	m_pThreadManager = new ThreadManager();

	char szComName[20];

	for(int i =0; i< MAX_NUMBER_COM; i++)
	{
		if(i<10)
			sprintf(szComName, "COM%d", i);
		else
			sprintf(szComName, "\\\\.\\COM%d", i);

		CUsbInterface* pUsb = new CUsbInterface(this);

		if(pUsb->InitPort(szComName) == 0)
		{
			m_pThreadManager->AddThread(pUsb);
			m_nNumberUsb++;
		}
		else
		{
			delete pUsb;
			pUsb = NULL;
		}
	}

	if(m_nNumberUsb == 0)
		return -1;

	return 0;
}

int CCoreManager::InitNumberFile()
{
	m_ListNumber.clear();
	FILE* file;
	int nNumberOfPhone = 0;

	file = fopen("Number_to_send.txt", "r");
	
	//thuc hien bo dau so black list
	if(file == NULL)
	{
		vector<string> ListSend;
		vector<string> BlackList;

		FILE* file;
		file  = fopen("Number_to_send.txt", "w");

		//Mo file black list
		FILE* fileBlackList;
		fileBlackList = fopen("Number_black_list.txt", "r");
		if(!fileBlackList)
			return -1;

		while(!feof(fileBlackList))
		{
			char szNumber[20];

			fgets(szNumber, 12, fileBlackList);
			if(strcmp(szNumber, "") == 0)
				continue;

			if(strstr(szNumber, "\n") != 0)
			{
				char* pTemp = strstr(szNumber, "\n");
				memset(pTemp, 0, 1);
			}
			string strNumber(szNumber);

			//Luu vao danh sach
			BlackList.insert(BlackList.end(), strNumber);
		}

		//Mo file tat ca dau so
		FILE* fileSend;
		fileSend = fopen("Numer_all.txt", "r");
		if(!fileSend)
			return -1;

		while(!feof(fileSend))
		{
			//string strNumber;
			char szNumber[20];

			fgets(szNumber, 12, fileSend);
			if(strcmp(szNumber, "") == 0)
				continue;

			if(strstr(szNumber, "\n") != 0)
			{
				char* pTemp = strstr(szNumber, "\n");
				memset(pTemp, 0, 1);
			}
			string strNumber(szNumber);

			
			for(int i =0; i< BlackList.size(); i++)
			{
				//Neu co trong black list thi bo qua
				if(strNumber == BlackList.at(i))
					continue;
				else
				{
					//Neu ko thi ghi vao file moi vao cho vao danh sach
					m_ListNumber.insert(m_ListNumber.end(), strNumber);
					//fwrite(strNumber.c_str(), 1, strNumber.size(), file);
					fputs(strNumber.c_str(), file);
					fputs("\n", file);

					nNumberOfPhone++;
				}
			}
		}

		fclose(fileSend);
		fclose(fileBlackList);
		fclose(file);
	}
	else
	{
		//Cho vao mang quan ly dau so luon
		while(!feof(file))
		{
			char szNumber[20];

			fgets(szNumber, 12, file);
			if(strcmp(szNumber, "") == 0)
				continue;

			if(strstr(szNumber, "\n") != 0)
			{
				char* pTemp = strstr(szNumber, "\n");
				memset(pTemp, 0, 1);
			}
			string strNumber(szNumber);

			m_ListNumber.insert(m_ListNumber.end(), strNumber);
			nNumberOfPhone++;
		}
	}

	return 0;

}

void CCoreManager::CoreConfig(int nMode, const char* szProtocolId, const char* szSmsText, const char* szWapUrl)
{
	//Cap nhat cac dau so dang active
	m_pThreadManager->UpdateThreadList();

	for(int i=0; i<m_pThreadManager->GetNoThreadEnable(); i++)
	{
		CUsbInterface* pUsb = (CUsbInterface*)m_pThreadManager->GetEnableThread(i);

		pUsb->SetConfig(nMode, szSmsText, szWapUrl);
		pUsb->SetProtocolId(szProtocolId);
	}
}

int CCoreManager::SendInstantMsg(const char* szCom, const char* szText, const char* szNumber, const char* szProID)
{
	int ret = 0;

	CUsbInterface* pUsb = (CUsbInterface*)m_pThreadManager->GetThread(string(szCom));
	if(pUsb == NULL)
	{
		m_pInstantMsg->LogOnScreen("%s: %s", szCom, "CANT FIND COM." );
		return -1;
	}
	if(pUsb->isRunning())
	{
		m_pInstantMsg->LogOnScreen("%s%s", szCom, "CANT FIND COM OR COM IS RUNNING" );
		return -1;
	}

	pUsb->SetProtocolId(szProID);
	ret = pUsb->SendInstantSms(string(szText), string(szNumber));

	return ret;
}

int CCoreManager::SendInstantWap(const char* szCom, const char* szText, const char* szUrl, const char* szNumber)
{
	int ret = 0;

	CUsbInterface* pUsb = (CUsbInterface*)m_pThreadManager->GetThread(string(szCom));
	if(pUsb == NULL)
	{
		m_pInstantWap->LogOnScreen("%s: %s", szCom, "CANT FIND COM." );
		return -1;
	}
	if(pUsb->isRunning())
	{
		m_pInstantWap->LogOnScreen("%s%s", szCom, "CANT FIND COM OR COM IS RUNNING" );
		return -1;
	}

	ret = pUsb->SendInstantWap(string(szText), string(szUrl), string(szNumber));

	return ret;
}

void CCoreManager::Start()
{
	if(m_pThreadManager->GetNoThreadEnable() == 0)
		return;

	int m_nNumberThread = m_pThreadManager->GetNoThreadEnable();

	//Dua cac dau so vao tung COM
	int nNumberRange = m_ListNumber.size()/m_nNumberThread;
	if(nNumberRange == 0)
	{
		//Truong hop danh sach it qua thi cho vao 1 COM gui
		((CUsbInterface*)m_pThreadManager->GetEnableThread(0))->SetNumberList(m_ListNumber);
	}
	else
	{
		//Neu ko thi chia deu
		for(int i=0; i< m_nNumberThread; i++)
		{
			//cac dau so le thi cho het vao COM cuoi cung
			if(m_ListNumber.size() - (i+1)*nNumberRange < nNumberRange)
			{
				std::vector<string> listNumber(m_ListNumber.begin() + i*nNumberRange, m_ListNumber.end());
				((CUsbInterface*)m_pThreadManager->GetEnableThread(i))->SetNumberList(listNumber);
			}
			else
			{
				std::vector<string> listNumber(m_ListNumber.begin() + i*nNumberRange, m_ListNumber.begin() + (i+1)*nNumberRange -1);
				((CUsbInterface*)m_pThreadManager->GetEnableThread(i))->SetNumberList(listNumber);
			}
		}
	}	

	m_pThreadManager->StartAll();
}

void CCoreManager::Pause(bool flag)
{
	m_pThreadManager->pause(flag);

}

void CCoreManager::Stop()
{
	m_pThreadManager->StopAll();
}

void CCoreManager::Exit()
{
	m_pThreadManager->Exit();
}

bool CCoreManager::IsRunning()
{
	return m_pThreadManager->IsRunning();
}

void CCoreManager::LogOnScreen(char *psz,...)
{

	char lst[256];
	va_list marker;
	va_start(marker,psz);
	vsprintf(lst,psz,marker);
	va_end(marker);

	m_pSMS->LogOnScreen(lst);
}