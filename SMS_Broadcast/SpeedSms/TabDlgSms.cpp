// TabDlgSms.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedSms.h"
#include "TabDlgSms.h"
#include "CoreManager.h"
#include "Log.h"
#include "Reporter.h"

// CTabDlgSms dialog

IMPLEMENT_DYNAMIC(CTabDlgSms, CDialog)

CTabDlgSms::CTabDlgSms(CWnd* pParent /*=NULL*/)
	: CDialog(CTabDlgSms::IDD, pParent)
	, m_smsLength(_T(""))
	, m_txtSmsText(_T(""))
	, m_txtWapUrl(_T(""))
{
	 m_nMode = SMS_MODE;
	 bPaused = false;
}

CTabDlgSms::~CTabDlgSms()
{
}

void CTabDlgSms::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTEXT, m_SmsText);
	DDX_Control(pDX, IDC_WAP_TEXT, m_WapText);
	DDX_Control(pDX, IDC_LIST_NOTIFY, m_ListNotify);
	DDX_Text(pDX, IDC_EDIT2, m_smsLength);
	DDX_Text(pDX, IDC_CONTEXT, m_txtSmsText);
	DDX_Text(pDX, IDC_WAP_TEXT, m_txtWapUrl);
	DDX_Control(pDX, IDC_SEND_BUTTON, m_StartButton);
	DDX_Control(pDX, IDC_PAUSE_BUTTON, m_PauseButton);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_StopButton);
	DDX_Control(pDX, IDC_STOP_BUTTON2, m_reportButton);
}


BEGIN_MESSAGE_MAP(CTabDlgSms, CDialog)
	ON_BN_CLICKED(IDC_SEND_BUTTON, &CTabDlgSms::OnBnClickedSendButton)
	ON_BN_CLICKED(IDC_PAUSE_BUTTON, &CTabDlgSms::OnBnClickedPauseButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CTabDlgSms::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_RADIO_SMS, &CTabDlgSms::OnBnClickedRadioSms)
	ON_BN_CLICKED(IDC_RADIO_WAP, &CTabDlgSms::OnBnClickedRadioWap)
	ON_EN_CHANGE(IDC_CONTEXT, &CTabDlgSms::OnEnChangeContext)
	ON_BN_CLICKED(IDC_STOP_BUTTON2, &CTabDlgSms::OnBnClickedStopButton2)
END_MESSAGE_MAP()

BOOL CTabDlgSms::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CButton*)GetDlgItem(IDC_RADIO_SMS))->SetCheck(true);
	GetDlgItem(IDC_WAP_TEXT)->EnableWindow(false);
	m_PauseButton.EnableWindow(FALSE);
	m_StopButton.EnableWindow(FALSE);
	m_reportButton.EnableWindow(FALSE);

	SetDlgItemText(IDC_PROTOCOL_ID, "00");
	return TRUE;
}

void CTabDlgSms::SetCoreManager(CCoreManager* pCore)
{
	m_pCoreManager = pCore;
	AfxBeginThread(MainThread, this);
}

// CTabDlgSms message handlers

void CTabDlgSms::OnBnClickedSendButton()
{
	Reporter::instance()->clear();
	UpdateData(TRUE);
	if(m_txtSmsText.GetLength() <= 0)
	{
		AfxMessageBox(_T("Ban chua nhap noi dung SMS"));
		return;
	}
	int nSms = m_txtSmsText.GetLength()/160 + 1;
	if(nSms > 5)
	{
		AfxMessageBox(_T("Noi dung sms vuot qua gioi han cho phep"));
		return;
	}
	if(m_nMode != SMS_MODE)
	{
		if(m_txtWapUrl.GetLength() <= 0)
		{
			AfxMessageBox(_T("Ban chua nhap wap url"));
			return;
		}
		if(m_txtWapUrl.GetLength() > 128)
		{
			AfxMessageBox(_T("Wap url vuot qua gioi han cho phep. ban chi duoc phep nhap toi da 128 ky tu. Thanks"));
			return;
		}
	}
	CString SmsText;
	CString WapUrl;
	CString ProtocolID;

	GetDlgItemText(IDC_CONTEXT, SmsText);
	GetDlgItemText(IDC_WAP_TEXT, WapUrl);
	GetDlgItemText(IDC_PROTOCOL_ID, ProtocolID);

	m_pCoreManager->CoreConfig(m_nMode, ProtocolID.GetString(), SmsText.GetString(), WapUrl.GetString());

	m_pCoreManager->Start();

	m_StartButton.EnableWindow(FALSE);
	m_PauseButton.EnableWindow(TRUE);
	m_StopButton.EnableWindow(TRUE);
	Reporter::instance()->start();
}

void CTabDlgSms::OnBnClickedPauseButton()
{
	bPaused = !bPaused;
	m_pCoreManager->Pause(bPaused);
	if(bPaused)
	{
		m_PauseButton.SetWindowTextA(_T("Continue"));
	}
	else
	{
		m_PauseButton.SetWindowTextA(_T("Pause"));
	}
}

void CTabDlgSms::OnBnClickedStopButton()
{
	m_pCoreManager->Stop();
	m_reportButton.EnableWindow(TRUE);
}

void CTabDlgSms::setPauseButtonTitle(CString title)
{
	m_PauseButton.SetWindowTextA(title);
}

void CTabDlgSms::OnBnClickedRadioSms()
{
	GetDlgItem(IDC_WAP_TEXT)->EnableWindow(false);
	m_nMode = SMS_MODE;
}

void CTabDlgSms::OnBnClickedRadioWap()
{
	GetDlgItem(IDC_WAP_TEXT)->EnableWindow(true);
	m_nMode = WAP_MODE;
}

UINT CTabDlgSms::MainThread(void* pUser)
{
	CTabDlgSms* pThis = (CTabDlgSms*)pUser;

	while(pThis->m_pCoreManager)
	{
		if(pThis->m_pCoreManager->IsRunning())
		{
			pThis->GetDlgItem(IDC_CONTEXT)->EnableWindow(false);
			pThis->GetDlgItem(IDC_WAP_TEXT)->EnableWindow(false);
			pThis->GetDlgItem(IDC_SEND_BUTTON)->EnableWindow(false);
			pThis->GetDlgItem(IDC_PROTOCOL_ID)->EnableWindow(false);
			pThis->m_PauseButton.EnableWindow(TRUE);
			pThis->m_StopButton.EnableWindow(TRUE);
			pThis->m_reportButton.EnableWindow(FALSE);
		}
		else
		{
			pThis->GetDlgItem(IDC_CONTEXT)->EnableWindow(true);
			pThis->GetDlgItem(IDC_SEND_BUTTON)->EnableWindow(true);
			if(pThis->getMode() == SMS_MODE)
				pThis->GetDlgItem(IDC_WAP_TEXT)->EnableWindow(false);
			else
				pThis->GetDlgItem(IDC_WAP_TEXT)->EnableWindow(true);

			pThis->GetDlgItem(IDC_PROTOCOL_ID)->EnableWindow(true);
			pThis->m_StartButton.EnableWindow(TRUE);
			pThis->m_PauseButton.EnableWindow(FALSE);
			pThis->m_StopButton.EnableWindow(FALSE);
			pThis->m_reportButton.EnableWindow(TRUE);
		}

		if(pThis->getMode() == SMS_MODE)
			pThis->SetDlgItemText(IDC_SMS_STATUS, "SMS TEXT");
		else
			pThis->SetDlgItemText(IDC_SMS_STATUS, "WAP TITLE");

		Sleep(100);
	}

	return 0;
}

void CTabDlgSms::LogOnScreen(char *psz,...)
{

	char lst[256];
	va_list marker;
	va_start(marker,psz);
	vsprintf(lst,psz,marker);
	va_end(marker);

	m_ListNotify.InsertString(0, lst);
	m_pCoreManager->m_pLog->Log(lst);
}

void CTabDlgSms::OnEnChangeContext()
{
	UpdateData(TRUE);
	int nSms = m_txtSmsText.GetLength()/160 + 1;
	
	m_smsLength.Format("%d/%d",m_txtSmsText.GetLength(), nSms);
	UpdateData(FALSE);
}


void CTabDlgSms::OnBnClickedStopButton2()
{
	Reporter::instance()->GenerateReport();
	AfxMessageBox(_T("Export report successfully"));
}
