
// SpeedSmsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedSms.h"
#include "SpeedSmsDlg.h"
#include "CoreManager.h"
#include "Scan.h"

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


// CSpeedSmsDlg dialog


CSpeedSmsDlg::CSpeedSmsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpeedSmsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nStatus = _IDLE_;
}

void CSpeedSmsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabControl);
}

BEGIN_MESSAGE_MAP(CSpeedSmsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSpeedSmsDlg::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CSpeedSmsDlg::OnTcnSelchangingTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSpeedSmsDlg message handlers

BOOL CSpeedSmsDlg::OnInitDialog()
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
	m_pScanDlg = new CScan(this);
	m_pScanDlg->Create(IDD_SCAN, this);
	m_pScanDlg->ShowWindow(true);
	::PostMessage(m_pScanDlg->GetSafeHwnd(),WM_SETFOCUS,0,0);

	int nWindowsWidth = GetSystemMetrics(SM_CXSCREEN);
	int nWindowsHeight = GetSystemMetrics(SM_CYSCREEN);
	int x, y;
	int cx, cy;
	x = nWindowsWidth/2;
	y = nWindowsHeight/2;	
	cx = 250;
	cy = 100;
	x -= (cx/2);
	y -= (cy/2);
	m_pScanDlg->SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);

	//Setting tab
	m_tSettingTab.Create(CTabDlgSetting::IDD, &m_tabControl);
	m_tSmsTab.Create(CTabDlgSms::IDD, &m_tabControl);
	m_tInstantMsg.Create(CInstantMsg::IDD, &m_tabControl);
	m_tInstantWap.Create(CInstantWap::IDD, &m_tabControl);
	
	m_tabControl.InsertItem(0, _T("COM & Setting"), -1);
	m_tabControl.InsertItem(1, _T("SMS & WAP"), -1);
	m_tabControl.InsertItem(2, _T("Instant SMS"), -1);
	m_tabControl.InsertItem(3, _T("Instant WAP"), -1);

	m_tSettingTab.ShowWindow(SW_SHOW);
	m_tSmsTab.ShowWindow(SW_HIDE);
	m_tInstantMsg.ShowWindow(SW_HIDE);
	m_tInstantWap.ShowWindow(SW_HIDE);

	//tab rect
	//int nX, nY, nXc, nYc;
	CRect TabRect, ItemRect;	
	m_tabControl.GetClientRect(&TabRect);
	m_tabControl.GetItemRect(0, &ItemRect);

	m_tabControl.AdjustRect(FALSE, &TabRect);

	m_tSettingTab.MoveWindow(TabRect);
	m_tSmsTab.MoveWindow(TabRect);
	m_tInstantMsg.MoveWindow(TabRect);
	m_tInstantWap.MoveWindow(TabRect);

	m_tSettingTab.ShowWindow(SW_SHOW);
	//nX=ItemRect.left;
	//nY=ItemRect.bottom+1;
	//nXc=TabRect.right-ItemRect.left-1;
	//nYc=TabRect.bottom-nY-1;

	//m_tSettingTab.SetWindowPos(&m_tabControl.wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	//m_tSmsTab.SetWindowPos(&m_tabControl.wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	//m_tInstantMsg.SetWindowPos(&m_tabControl.wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

	//InitCore
	m_pCoreManager = new CCoreManager();
	m_pCoreManager->SetParent(this, &m_tSettingTab, &m_tSmsTab, &m_tInstantMsg, &m_tInstantWap);

	AfxBeginThread(MainThread, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

UINT CSpeedSmsDlg::MainThread(void* pUser)
{
	CSpeedSmsDlg* pThis = (CSpeedSmsDlg*)pUser;

	pThis->SetDlgItemTextA(IDC_STATUS, "Updating device....");
	pThis->m_pCoreManager->Init();

	pThis->m_tSettingTab.SetCoreManager(pThis->m_pCoreManager);
	pThis->m_tSmsTab.SetCoreManager(pThis->m_pCoreManager);
	pThis->m_tInstantMsg.SetCoreManager(pThis->m_pCoreManager);
	pThis->m_tInstantWap.SetCoreManager(pThis->m_pCoreManager);

	pThis->m_pScanDlg->ShowWindow(false);
	pThis->SetDlgItemTextA(IDC_STATUS, "Updating OK");

	return 0;
}

void CSpeedSmsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSpeedSmsDlg::OnPaint()
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
HCURSOR CSpeedSmsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//
void CSpeedSmsDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	switch(m_tabControl.GetCurSel())
	{
	case 0:
		m_tSettingTab.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_tSmsTab.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_tInstantMsg.ShowWindow(SW_SHOW);
		m_tInstantMsg.UpdateNumberList();
		break;
	case 3:
		m_tInstantWap.ShowWindow(SW_SHOW);
		m_tInstantWap.UpdateNumberList();
		break;
	}	
	*pResult = 0;
}

void CSpeedSmsDlg::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	switch(m_tabControl.GetCurSel())
	{
	case 0:
		m_tSettingTab.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_tSmsTab.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_tInstantMsg.ShowWindow(SW_HIDE);
		break;		
	case 3:
		m_tInstantWap.ShowWindow(SW_HIDE);
		break;	
	}
	*pResult = 0;
}


void CSpeedSmsDlg::OnClose()
{
	m_pCoreManager->Exit();
	CDialog::OnClose();
}
