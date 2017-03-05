// InstantMsg.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedSms.h"
#include "InstantMsg.h"
#include "ThreadManager.h"
#include "Log.h"

#include "CoreManager.h"

// CInstantMsg dialog

IMPLEMENT_DYNAMIC(CInstantMsg, CDialog)

CInstantMsg::CInstantMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstantMsg::IDD, pParent)
{

}

CInstantMsg::~CInstantMsg()
{
}

void CInstantMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COM, m_ListCom);
	DDX_Control(pDX, IDC_MOBILE_NO, m_MobileNo);
	DDX_Control(pDX, IDC_MSG_TXT, m_Msg);
}


BEGIN_MESSAGE_MAP(CInstantMsg, CDialog)
	ON_BN_CLICKED(IDC_SEND, &CInstantMsg::OnBnClickedSend)
END_MESSAGE_MAP()


// CInstantMsg message handlers

BOOL CInstantMsg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_PROTOCOL_ID, "00");

	return TRUE;
}

void CInstantMsg::UpdateNumberList(/*vector<string> vListCom*/)
{
	ThreadManager* pManager = m_pCoreManager->GetUsbManager();

	m_ListCom.ResetContent();
	for(int i=0; i<pManager->GetSize(); i++)
	{
		m_ListCom.AddString(pManager->GetThread(i)->GetName().c_str());
	}
}

void CInstantMsg::SetCoreManager(CCoreManager* pCore)
{
	m_pCoreManager = pCore;
	AfxBeginThread(MainThread, this);
}


void CInstantMsg::OnBnClickedSend()
{
	GetDlgItem(IDC_SEND)->EnableWindow(false);

	SetDlgItemText(IDC_STATUS, "START SEND SMS");

	int ret = 0;
	CString sMsg;
	CString	sMobileNo;
	CString sComName;
	CString sProID;
	//vector<string> vListNumber; 

	//Lay text
	GetDlgItemText(IDC_MSG_TXT, sMsg);
	//Lay dau so
	GetDlgItemText(IDC_MOBILE_NO, sMobileNo);
	//vListNumber.insert(vListNumber.end(), string(sMobileNo.GetString()));

	GetDlgItemText(IDC_PROTOCOL_ID, sProID);
	
	//Lay COM Name
	int nItem = m_ListCom.GetCurSel();
	m_ListCom.GetLBText(nItem, sComName);
	
	ret = m_pCoreManager->SendInstantMsg(sComName.GetString(), sMsg.GetString(), sMobileNo.GetString(), sProID.GetString());
	if(ret != 0)
		LogOnScreen("SEND SMS TO %s FAIL", sMobileNo.GetString());
	else
		LogOnScreen("SMS HAS BEEN SENT TO %s", sMobileNo.GetString());

	GetDlgItem(IDC_SEND)->EnableWindow(true);

}

UINT CInstantMsg::MainThread(void* pUser)
{
	CInstantMsg* pThis = (CInstantMsg*)pUser;

	return 0;
}

void CInstantMsg::LogOnScreen(char *psz,...)
{
	char lst[256];
	va_list marker;
	va_start(marker,psz);
	vsprintf(lst,psz,marker);
	va_end(marker);

	SetDlgItemText(IDC_SMS_STATUS, lst);
	m_pCoreManager->m_pLog->Log(lst);
}