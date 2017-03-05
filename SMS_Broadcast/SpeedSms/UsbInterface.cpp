#include "StdAfx.h"
#include "UsbInterface.h"
#include "coreManager.h"
#include "Log.h"
#include "Reporter.h"

CUsbInterface::CUsbInterface(CCoreManager* pManager)
{
	memset(m_szNumber, 0, sizeof(m_szNumber));

	m_nTotalSms = 0;
	m_nSmsOK = 0;
	m_nSmsFail = 0;

	m_nSmsRange = 1000;
	m_bActive = true;
	isReading = false;
	m_pManager = pManager;
	m_pLog = CLog::GetInstance();
}

CUsbInterface::CUsbInterface(void)
{
}

CUsbInterface::~CUsbInterface(void)
{
	m_ComPort.Close();
}
CStringA CUsbInterface::DecodePDUPhoneNumber(CStringA PDUPhoneNumber)
{
	CStringA ret;
	// int'l format
	if (PDUPhoneNumber.Left(2) == "91")
	{
		// 91 prefix for phone number
		ret += "+";
		//pos = 1;
	}
	//else ret += "81";
	/*CStringA h = phoneNumber.Mid(pos, 2);

	if (phoneNumber.Left(2) == "84")
	{
	pos += 2;
	}*/


	for (int i = 2; i < (int)PDUPhoneNumber.GetLength(); i += 2)
	{
		ret = ret + PDUPhoneNumber[i + 1] + PDUPhoneNumber[i];
		//ret += phoneNumber[i];
	}
	ret.Remove('F');
	return ret;
}

void CUsbInterface::GetStatusReport(CStringA message, CString& number, int& isSuccess)
{
	//07914889200030F0061C0B914889043315F0901161519330829011615193808200
	int offset = 0;
	int len = strtol(message.Mid(offset, 2), NULL, 16);
	offset = 2 + len * 2; // jump over MSC len + MSC number
	offset += 2; // jump over SMS type indicator
	offset += 2; // jump over msg reference number
	len = strtol(message.Mid(offset, 2), NULL,16) + 2; // plus + sign, international format
	offset += 2; // jump over phone len
	if (len % 2 != 0) 
		len++;
	number = DecodePDUPhoneNumber(message.Mid(offset, len));
	//number = CT2A(phoneNumber);
	offset += len;
	offset += 14; // jump over the first time stamp
//	CTime dateTime = GetTimeStamp(message.Mid(offset, 14));

	if (message.Right(2) == "00")
	{
		//len = SMS_DELIVERY_SUCCESSED;
		isSuccess = 1;
	}
	else 
		//len = SMS_DELIVERY_FAILED;
		isSuccess = 0;
	//return CSMSEntry(slotID, len, phoneNumber, _T("") , dateTime, _T(SIM), message);
}

DWORD CUsbInterface::StatusReportThread(LPVOID arg)
{
	CUsbInterface* pThis = (CUsbInterface*)arg;
	std::string ATcommand = "AT+CMGL=0\r";
	std::string ATDeleteSMS = "AT+CMGD=3\r";
	char buffer[MAX_BUFFER_SIZE] = {0};
	while(pThis->isRunning())
	{
		pThis->m_ComPort.Write(ATcommand.c_str(), ATcommand.length());
		int len = pThis->m_ComPort.Read(buffer, MAX_BUFFER_SIZE);
		if(len > 0)
		{
			pThis->ProcessSMSResponse(buffer);
		}
		memset(buffer, 0, MAX_BUFFER_SIZE);
		Sleep(10);
	}
	return 0;
}

void CUsbInterface::ProcessSMSResponse(CString content)
{
	/*
	+CMGL: 0,0,,25
	07914889200047F2067A0B914879091520F1216071618435822160716184258200
	*/
	CString number = "";
	int isSuccess = 0;
	int iPos = 0;
	CString token;
	if(content.Find(_T("CMGL")) > 0)
	{
		token = content.Tokenize(_T("\r\n"), iPos);
		while(token != "")
		{
			token = content.Tokenize(_T("\r\n"), iPos);
			if(token.GetLength() > 0)
			{
				GetStatusReport(token, number, isSuccess);
				if(number.GetLength() > 0)
				{
					if(number.Find("+84", 0) >= 0)
						number.Replace("+84", "0");
					std::string dstNumber = CT2A(number);
					if(isSuccess)
						Reporter::instance()->Add2Report(dstNumber, true);
					else
						Reporter::instance()->Add2Report(dstNumber, false);
				}
			}
	
		}
		std::string ATDeleteSMS = "AT+CMGD=0,1\r";
		char buffer[1500] = {0};
		m_ComPort.Write(ATDeleteSMS.c_str(), ATDeleteSMS.length());
		int len = m_ComPort.Read(buffer, MAX_BUFFER_SIZE);
	}
}
int CUsbInterface::InitPort(const char* szName)
{
	//Khoi tao COM
	int result = 0;
	
	result = m_ComPort.Open(CString(szName));
	
	if(result == 0)
	{
		//strcpy(m_szNumber, m_ComPort.GetNumber().c_str());

		//Luu lai ten
		if(strstr(szName, "\\") != NULL)
			setName(string(szName +4));
		else
			setName(string(szName));

		m_Sender.ConfigCom(&m_ComPort);
	}

	return result;
}
void CUsbInterface::startStatusReport()
{
	if(isRunning())
	{
		DWORD dwThreadId = 0;
		isReading = true;
		hThread = CreateThread(NULL, 0, StatusReportThread, this, 0, &dwThreadId);
	}
}
void CUsbInterface::SetNumberList(std::vector<string> vListNumber)
{
	m_vListNumber.clear();
	m_vListNumber = vListNumber;
}

void CUsbInterface::SetSimNumber(const char* szNumber)
{
	sprintf(m_szNumber, "%s", szNumber);
}

void CUsbInterface::SetProtocolId(const char* szProtocolId)
{
	m_Sender.SetProtocolId(szProtocolId);
}

void CUsbInterface::SetConfig(int nMode, const char* szSmsText, const char* szWapUrl)
{
	m_Sender.ConfigMode(nMode);
	m_Sender.setMsgText(string(szSmsText));
	m_Sender.setUrlText(string(szWapUrl));

	LogOnScreen("%s%s%s%s",m_sThreadName.c_str(), " config ", nMode ? "PDU mode" : "SMS mode"  , " OK");
}

int CUsbInterface::SendInstantSms(string sText, string sDestNum)
{
	int ret = 0;

	if(m_nSmsRange == 0)
		return -1;

	m_Sender.setMsgText(sText);
	m_Sender.ConfigMode(SMS_MODE);
	//ret = m_Sender.SendSms(sDestNum);
	ret = m_Sender.SendSmsPdu(sDestNum);

	//gui thanh cong thi phai tru tin nhan
	if(ret == 0)
	{
		m_nSmsOK++;
		m_nSmsRange--;
	}
	else
		m_nSmsFail++;

	return ret;
}

int CUsbInterface::SendInstantWap(string sText, string sUrl, string sDestNum)
{
	int ret = 0;

	if(m_nSmsRange == 0)
		return -1;

	m_Sender.setMsgText(sText);
	m_Sender.setUrlText(sUrl);
	m_Sender.ConfigMode(WAP_MODE);
	ret = m_Sender.SendWap(sDestNum);

	//gui thanh cong thi phai tru tin nhan
	if(ret == 0)
	{
		m_nSmsOK++;
		m_nSmsRange--;
	}
	else
		m_nSmsFail++;

	return ret;
}

void CUsbInterface::run()
{

	int nNumberCount = 0;
	int ret = 0;
	
	while(isRunning() && (nNumberCount < m_vListNumber.size()) && m_nSmsRange >0)
	{
		
		string sPhoneNumber = m_vListNumber.at(nNumberCount);

		LogOnScreen("%s%s%s%s%s", m_sThreadName.c_str(), " start send ", m_Sender.m_nMode ? "WAP" : "SMS" ," to ", sPhoneNumber.c_str());
		ret = m_Sender.Send(sPhoneNumber);
		if(ret == 0)
		{
			LogOnScreen("%s%s%s%s%s%s", m_sThreadName.c_str(), " send ", m_Sender.m_nMode ? "WAP" : "SMS" ," to ", sPhoneNumber.c_str(), " OK");
			m_nSmsRange--;
			m_nSmsOK++;
		}
		else
		{
			LogOnScreen("%s%s%s%s%s%s", m_sThreadName.c_str(), " send ", m_Sender.m_nMode ? "WAP" : "SMS" ," to ", sPhoneNumber.c_str(), " FAIL");
			m_nSmsFail++;
		}

		nNumberCount++;
	}

	//isReading = false;
}

void CUsbInterface::exit()
{
	//m_ComPort.Close();
	m_Sender.exit();
	this->Stop();
}
void CUsbInterface::LogOnScreen(char *psz,...)
{
	char lst[256];
	va_list marker;
	va_start(marker,psz);
	vsprintf(lst,psz,marker);
	va_end(marker);

	m_pManager->LogOnScreen(lst);
}