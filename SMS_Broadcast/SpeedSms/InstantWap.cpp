// InstantWap.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedSms.h"
#include "InstantWap.h"
#include "Log.h"
#include "ThreadManager.h"
#include "CoreManager.h"

// CInstantWap dialog

IMPLEMENT_DYNAMIC(CInstantWap, CDialog)

CInstantWap::CInstantWap(CWnd* pParent /*=NULL*/)
	: CDialog(CInstantWap::IDD, pParent)
{

}

CInstantWap::~CInstantWap()
{
}

void CInstantWap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COM, m_ListCom);
}


BEGIN_MESSAGE_MAP(CInstantWap, CDialog)
	ON_BN_CLICKED(IDC_SEND, &CInstantWap::OnBnClickedSend)
END_MESSAGE_MAP()


// CInstantWap message handlers

BOOL CInstantWap::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void CInstantWap::UpdateNumberList(/*vector<string> vListCom*/)
{
	ThreadManager* pManager = m_pCoreManager->GetUsbManager();

	m_ListCom.ResetContent();
	for(int i=0; i<pManager->GetSize(); i++)
	{
		m_ListCom.AddString(pManager->GetThread(i)->GetName().c_str());
	}
}

void CInstantWap::SetCoreManager(CCoreManager* pCore)
{
	m_pCoreManager = pCore;
	AfxBeginThread(MainThread, this);
}


void CInstantWap::OnBnClickedSend()
{
	GetDlgItem(IDC_SEND)->EnableWindow(false);

	SetDlgItemText(IDC_STATUS, "START SEND WAP");

	int ret = 0;
	CString sTitle;
	CString sUrl;
	CString	sMobileNo;
	CString sComName;
	//vector<string> vListNumber; 

	//Lay Title
	GetDlgItemText(IDC_TITLE, sTitle);
	//Lay Url
	GetDlgItemText(IDC_URL, sUrl);
	//Lay dau so
	GetDlgItemText(IDC_MOBILE_NO, sMobileNo);
	//vListNumber.insert(vListNumber.end(), string(sMobileNo.GetString()));
	
	//Lay COM Name
	int nItem = m_ListCom.GetCurSel();
	m_ListCom.GetLBText(nItem, sComName);
	
	ret = m_pCoreManager->SendInstantWap(sComName.GetString(), sTitle.GetString(), sUrl.GetString(), sMobileNo.GetString());
	if(ret != 0)
		LogOnScreen("SEND WAP TO %s FAIL", sMobileNo.GetString());
	else
		LogOnScreen("WAP HAS BEEN SENT TO %s", sMobileNo.GetString());

	GetDlgItem(IDC_SEND)->EnableWindow(true);
}

UINT CInstantWap::MainThread(void* pUser)
{
	CInstantMsg* pThis = (CInstantMsg*)pUser;

	return 0;
}

void CInstantWap::LogOnScreen(char *psz,...)
{
	char lst[256];
	va_list marker;
	va_start(marker,psz);
	vsprintf(lst,psz,marker);
	va_end(marker);

	SetDlgItemText(IDC_SMS_STATUS, lst);
	m_pCoreManager->m_pLog->Log(lst);
}