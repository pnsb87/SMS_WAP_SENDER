
// SpamSMSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpamSMS.h"
#include "SpamSMSDlg.h"
#include "SerialCom.h"

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


// CSpamSMSDlg dialog




CSpamSMSDlg::CSpamSMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpamSMSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpamSMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSpamSMSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SEND, &CSpamSMSDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BUTTON1, &CSpamSMSDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSpamSMSDlg message handlers

BOOL CSpamSMSDlg::OnInitDialog()
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
	m_pSerialPort = new CSerialCom();
	if(m_pSerialPort->OpenPort(CString("\\\\.\\COM27")))
		SetDlgItemText(IDC_STATUS, CString("COM is connected"));
	else
	{
		SetDlgItemText(IDC_STATUS, CString("COM is not connected"));
		delete m_pSerialPort;
		m_pSerialPort = NULL;

		return TRUE;
	}

	m_pSerialPort->ConfigurePort(9600, 8, 0, NOPARITY, ONE5STOPBITS);
	m_pSerialPort->SetCommunicationTimeouts(0,100,0,0,0);


	const char* szReturn = NULL;
	szReturn = m_pSerialPort->WriteByte("AT");

	if(szReturn)
		SetDlgItemText(IDC_STATUS, CString("Phone is connected"));
	else
		SetDlgItemText(IDC_STATUS, CString("Phone is not connected"));

	SetDlgItemText(IDC_RETURN, CString(szReturn));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpamSMSDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSpamSMSDlg::OnPaint()
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
HCURSOR CSpamSMSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSpamSMSDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	char szMessage[160];
	const char* szReturn = NULL;
	memset(&szMessage, 0, 160);



	//Check the seral COM port

	szReturn = m_pSerialPort->WriteByte("AT");
	if(szReturn == NULL)
	{
		SetDlgItemText(IDC_STATUS, CString("Phone is not connected"));
		return;
	}
	SetDlgItemText(IDC_STATUS, CString("Phone is connected"));
	SetDlgItemText(IDC_RETURN, CString(szReturn));

	m_pSerialPort->WriteByte("AT+CMGF=1");
	if(szReturn == NULL)
	{
		SetDlgItemText(IDC_STATUS, CString("Phone does not support sms text format"));
		return;
	}
	SetDlgItemText(IDC_STATUS, CString("Phone supports sms text format"));
	SetDlgItemText(IDC_RETURN, CString(szReturn));


	CString phoneNumber;
	GetDlgItemText(IDC_PHONE_NUMBER, phoneNumber);
	memset(&szMessage, 0, 160);
	sprintf(szMessage, "AT+CMGS=\"0904962930\"");

	m_pSerialPort->WriteByte(szMessage);
	if(szReturn == NULL)
	{
		SetDlgItemText(IDC_STATUS, CString("Failed to accept phoneNo"));
		return;		
	}
	SetDlgItemText(IDC_STATUS, CString("Phone number is accepted"));
	SetDlgItemText(IDC_RETURN, CString(szReturn));

	memset(&szMessage, 0, 160);
	CString message;
	GetDlgItemText(IDC_MESSAGE, message);
	int nEnd = 26;
	char end[1];
	memset(&end,0,1);
	end[0] = (char)nEnd;
	sprintf(szMessage, "thanh nien dx khoe ko%s", end);

	m_pSerialPort->WriteByte(szMessage);
	if(szReturn == NULL)
	{
		SetDlgItemText(IDC_STATUS, CString("Failed to send message"));
		return;		
	}
	SetDlgItemText(IDC_STATUS, CString("send message successfully"));
	SetDlgItemText(IDC_RETURN, CString(szReturn));
}

void CSpamSMSDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	char szMessage[140];
	memset(&szMessage, 0, 140);

	sprintf(szMessage, "AT+CUSD=1,\"2A31303123\",15 ");

	m_pSerialPort->WriteByte(szMessage);

}
