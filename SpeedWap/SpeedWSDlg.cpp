
// SpeedWSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedWS.h"
#include "SpeedWSDlg.h"
#include "CoreSpam.h"
#include "DetailDlg.h"
#include "BitEncoder.h"
#include "WAPSender.h"
#include "SerialCommunication.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSpeedWSDlg dialog




CSpeedWSDlg::CSpeedWSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpeedWSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSpamMode = 0; // mac dinh la SMS
	
	m_bOnSpam = false;
	
	memset(&m_szMsg, 0, 1024);

	m_pCoreSpam = NULL;
}

void CSpeedWSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSpeedWSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SMS_CHOICE, &CSpeedWSDlg::OnBnClickedSmsChoice)
	ON_BN_CLICKED(IDC_WAP_CHOICE, &CSpeedWSDlg::OnBnClickedWapChoice)
	ON_BN_CLICKED(IDC_START_SEND, &CSpeedWSDlg::OnBnClickedStartSend)
	ON_BN_CLICKED(IDC_STOP_SEND, &CSpeedWSDlg::OnBnClickedStopSend)
	ON_BN_CLICKED(IDC_COM_DETAIL, &CSpeedWSDlg::OnBnClickedComDetail)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSpeedWSDlg message handlers

BOOL CSpeedWSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_SMS_CHOICE))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_CONTEXT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_TEXT_VISIBLE))->EnableWindow(false);

	
	//comm->Open("COM10");

	m_pCoreSpam = new CCoreSpam(this);

	m_bMainThreadRun = true;
	AfxBeginThread(MainThread, this);
		
	NotifyStatus("System is initializing.....");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpeedWSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpeedWSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpeedWSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSpeedWSDlg::OnBnClickedSmsChoice()
{
	// TODO: Add your control notification handler code here
	m_nSpamMode = 0;
}

void CSpeedWSDlg::OnBnClickedWapChoice()
{
	// TODO: Add your control notification handler code here
	m_nSpamMode = 1;
}

void CSpeedWSDlg::OnBnClickedStartSend()
{
	// TODO: Add your control notification handler code here


	CString szMessage;
	GetDlgItemText(IDC_CONTEXT, szMessage);

	//Gui dinh dang SMS hay WAP
	if(m_pCoreSpam)
		m_pCoreSpam->SetType(m_nSpamMode);

	//Gui noi dung can send
	GetDlgItemText(IDC_CONTEXT, szMessage);
	if(m_pCoreSpam)
	{
		int ret = m_pCoreSpam->SetContex(szMessage.GetString());
		if(ret == -1)
		{
			AfxMessageBox("Do not accept NULL context!!!!!", 0, 0);
			return;
		}
	}

	if(m_nSpamMode == WAP_MODE)
	{
		//Gui dinh dang danh cho WAP
		GetDlgItemText(IDC_TEXT_VISIBLE, szMessage);
		if(m_pCoreSpam)
			m_pCoreSpam->SetTextVisible(szMessage.GetString());
	}

	//Ra lenh Spam
	m_bOnSpam = true;
	m_pCoreSpam->StartSpam();

	return;
}

void CSpeedWSDlg::OnBnClickedStopSend()
{
	// TODO: Add your control notification handler code here
	m_bOnSpam = false;
}

void CSpeedWSDlg::OnBnClickedComDetail()
{
	// TODO: Add your control notification handler code here
	int nIndex;
	nIndex = ((CComboBox*)GetDlgItem(IDC_COM_LIST))->GetCurSel();

	char szComName[10];
	((CComboBox*)GetDlgItem(IDC_COM_LIST))->GetLBText(nIndex, szComName);

	//Get Port Info
	DWORD dwSize = 1;
	LPCOMMCONFIG lpCC = (LPCOMMCONFIG) new BYTE[1];
	BOOL ret = GetDefaultCommConfig(szComName, lpCC, &dwSize);
	delete [] lpCC;	

	lpCC = (LPCOMMCONFIG) new BYTE[dwSize];
	ret = GetDefaultCommConfig(szComName, lpCC, &dwSize);

	delete [] lpCC;

	//CDetailDlg* pDlg = new CDetailDlg(this);


	return;
}

UINT CSpeedWSDlg::MainThread(void* pUser)
{
	CSpeedWSDlg* pThis = (CSpeedWSDlg*)pUser;

	int nResult;

	nResult = pThis->m_pCoreSpam->InitCore();

	if(nResult != 0)
	{
		pThis->NotifyStatus("Init Core Spam ERROR!!!!");
		return TRUE;
	}
	
	(pThis->GetDlgItem(IDC_SMS_CHOICE))->EnableWindow(true);
	(pThis->GetDlgItem(IDC_WAP_CHOICE))->EnableWindow(true);
	(pThis->GetDlgItem(IDC_COM_DETAIL))->EnableWindow(true);
	(pThis->GetDlgItem(IDC_COM_LIST))->EnableWindow(true);
	(pThis->GetDlgItem(IDC_START_SEND))->EnableWindow(true);
	(pThis->GetDlgItem(IDC_STOP_SEND))->EnableWindow(true);

	for(int i=0; i< pThis->m_pCoreSpam->m_nAvailablePort; i++)
	{
		CComPort pCOM;
		pCOM = pThis->m_pCoreSpam->m_ListCOM[i];
		pCOM.ClosePort();

		SerialCommunication* comm = new SerialCommunication();
		comm->Open(pCOM.m_szComName);
		WAPSender* sender = new WAPSender();
		
		sender->setSMSText("hello world");
		sender->setUrl("vietnamnet.vn");
		sender->Config(comm);
		sender->Send("0985788355");
		delete sender;
		((CComboBox*)pThis->GetDlgItem(IDC_COM_LIST))->AddString(CString(pCOM.m_szComName));
	}

	((CComboBox*)pThis->GetDlgItem(IDC_COM_LIST))->SetCurSel(0);

	pThis->NotifyStatus("Init Core Spam SUCCESSFULLY");
	pThis->m_pCoreSpam->SetType(SMS_MODE);
	
	while(pThis->m_bMainThreadRun)
	{
		if(pThis->m_bOnSpam)
		{
			((CButton*)pThis->GetDlgItem(IDC_SMS_CHOICE))->EnableWindow(false);
			((CButton*)pThis->GetDlgItem(IDC_WAP_CHOICE))->EnableWindow(false);

			((CButton*)pThis->GetDlgItem(IDC_START_SEND))->EnableWindow(false);

			((CButton*)pThis->GetDlgItem(IDC_CONTEXT))->EnableWindow(false);
			((CButton*)pThis->GetDlgItem(IDC_TEXT_VISIBLE))->EnableWindow(false);
		}
		else
		{
			((CButton*)pThis->GetDlgItem(IDC_CONTEXT))->EnableWindow(true);

			((CButton*)pThis->GetDlgItem(IDC_SMS_CHOICE))->EnableWindow(true);
			((CButton*)pThis->GetDlgItem(IDC_WAP_CHOICE))->EnableWindow(true);	

			((CButton*)pThis->GetDlgItem(IDC_START_SEND))->EnableWindow(true);

			if(pThis->m_nSpamMode == WAP_MODE)
				((CButton*)pThis->GetDlgItem(IDC_TEXT_VISIBLE))->EnableWindow(true);
			else
				((CButton*)pThis->GetDlgItem(IDC_TEXT_VISIBLE))->EnableWindow(false);
		}

		Sleep(1);
	}


	return 0;
}

void CSpeedWSDlg::NotifySpam(const char* szMsg)
{
	((CListBox*)GetDlgItem(IDC_RESULT))->InsertString(0,(LPCTSTR)szMsg);
}

void CSpeedWSDlg::NotifyStatus(const char* szMsg)
{
	SetDlgItemText(IDC_STATUS, (LPCTSTR)szMsg);
}

void CSpeedWSDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	m_bMainThreadRun = false;
}

//BOOL CSpeedWSDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	switch(message)
//	{
//		case WM_END_THREAD_SEND:
//			m_bOnSpam = false;
//			if(wParam == 1)
//				NotifySpam("Stop Spam SMS");
//			else if(wParam == 0)
//				NotifySpam("Stop Spam WAP");
//			break;
//		default:
//			break;
//	}
//
//	return TRUE;
//}
