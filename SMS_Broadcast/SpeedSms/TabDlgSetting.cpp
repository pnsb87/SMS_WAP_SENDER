// TabDlgWap.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedSms.h"
#include "TabDlgSetting.h"
#include "ThreadManager.h"
#include "CoreManager.h"
#include "UsbInterface.h"

// CTabDlgSetting dialog

IMPLEMENT_DYNAMIC(CTabDlgSetting, CDialog)

CTabDlgSetting::CTabDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CTabDlgSetting::IDD, pParent)
{
	m_bDoUpdate = true;
	m_nCurrentFocus = -1;
}

CTabDlgSetting::~CTabDlgSetting()
{
}

void CTabDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COM, m_ListCom);
}


BEGIN_MESSAGE_MAP(CTabDlgSetting, CDialog)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COM, &CTabDlgSetting::OnLvnItemchangedListCom)
	ON_BN_CLICKED(IDC_SCAN, &CTabDlgSetting::OnBnClickedScan)
//	ON_NOTIFY(HDN_ITEMCLICK, 0, &CTabDlgSetting::OnHdnItemclickListCom)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabDlgSetting::OnBnClickedButton1)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CTabDlgSetting::OnHdnItemdblclickListCom)
//	ON_NOTIFY(HDN_ITEMKEYDOWN, 0, &CTabDlgSetting::OnHdnItemKeyDownListCom)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COM, &CTabDlgSetting::OnNMClickListCom)
	ON_BN_CLICKED(IDC_APPLY, &CTabDlgSetting::OnBnClickedApply)
END_MESSAGE_MAP()


// CTabDlgSetting message handlers

//void CTabDlgSetting::OnLvnItemchangedListCom(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

BOOL CTabDlgSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	LVCOLUMN lvColumn;

	m_ListCom.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES /*| LVS_EX_CHECKBOXES*/);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 120;
	lvColumn.pszText = "Name";
	m_ListCom.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 120;
	lvColumn.pszText = "Number";
	m_ListCom.InsertColumn(1, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = "SMS OK";
	m_ListCom.InsertColumn(2, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = "SMS Fail";
	m_ListCom.InsertColumn(3, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = "Total SMS";
	m_ListCom.InsertColumn(4, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = "SMS Cap";
	m_ListCom.InsertColumn(5, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 80;
	lvColumn.pszText = "Status";
	m_ListCom.InsertColumn(6, &lvColumn);


	//LVITEM lvItem;
	//int nItem;

	//lvItem.mask = LVIF_TEXT;
	//lvItem.iItem = 0;
	//lvItem.iSubItem = 0;
	//lvItem.pszText = "Sandra C. Anschwitz";
	//nItem = m_ListCom.InsertItem(&lvItem);

	//m_ListCom.SetItemText(nItem, 1, (LPCTSTR)"Singer");
	//m_ListCom.SetItemText(nItem, 2, (LPCTSTR)"HandBall");
	//m_ListCom.SetItemText(nItem, 3, (LPCTSTR)"Beach");
	//m_ListCom.SetItemText(nItem, 4, (LPCTSTR)"Beach");
	//m_ListCom.SetItemText(nItem, 5, (LPCTSTR)"Beach");

	//lvItem.mask = LVIF_TEXT;
	//lvItem.iItem = 1;
	//lvItem.iSubItem = 0;
	//lvItem.pszText = "Nguyen Huu Ngoc";
	//nItem = m_ListCom.InsertItem(&lvItem);

	//m_ListCom.SetItemText(nItem, 1, (LPCTSTR)"Singer");
	//m_ListCom.SetItemText(nItem, 2, (LPCTSTR)"HandBall");
	//m_ListCom.SetItemText(nItem, 3, (LPCTSTR)"Beach");
	//m_ListCom.SetItemText(nItem, 4, (LPCTSTR)"Beach");
	//m_ListCom.SetItemText(nItem, 5, (LPCTSTR)"Beach");

	return TRUE;
}

void CTabDlgSetting::OnBnClickedScan()
{
	if(m_pCoreManager->IsRunning())
	{
		
	}
	// TODO: Add your control notification handler code here
}

void CTabDlgSetting::SetCoreManager(CCoreManager* pCore)
{
	m_pCoreManager = pCore;
	AfxBeginThread(MainThread, this);
}

UINT CTabDlgSetting::MainThread(void* pUser)
{
	CTabDlgSetting* pThis = (CTabDlgSetting*)pUser;

	ThreadManager* pUsbManager = pThis->m_pCoreManager->GetUsbManager();

	while(pThis->m_pCoreManager)
	{
		if(pThis->m_bDoUpdate)
		{
			pThis->m_ListCom.DeleteAllItems();

			for(int i=0; i< pUsbManager->GetSize(); i++)
			{
				char szSms[10];
				CUsbInterface* pUsb = (CUsbInterface*)pUsbManager->GetThread(i);

				LVITEM lvItem;
				int nItem;
				lvItem.pszText = new char[10];

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;
				lvItem.iSubItem = 0;
				strcpy(lvItem.pszText, pUsb->GetName().c_str());
				nItem = pThis->m_ListCom.InsertItem(&lvItem);

				pThis->m_ListCom.SetItemText(nItem, 1, pUsb->getNumber());

				sprintf(szSms, "%d", pUsb->getSmsOk()); 
				pThis->m_ListCom.SetItemText(nItem, 2, szSms);

				sprintf(szSms, "%d", pUsb->getSmsFail()); 
				pThis->m_ListCom.SetItemText(nItem, 3, szSms);

				sprintf(szSms, "%d", pUsb->getTotalSms()); 
				pThis->m_ListCom.SetItemText(nItem, 4, szSms);

				sprintf(szSms, "%d", pUsb->getSmsRange()); 
				pThis->m_ListCom.SetItemText(nItem, 5, szSms);		

				if(pUsb->GetStatus())
					pThis->m_ListCom.SetItemText(nItem, 6, "Enable");	
				else
					pThis->m_ListCom.SetItemText(nItem, 6, "Disable");	

				pThis->m_ListCom.SetCheck(nItem, pUsb->GetStatus());

				delete lvItem.pszText;
			}

			pThis->m_bDoUpdate = false;
		}

		Sleep(1000);
	}

	return 0;
}


void CTabDlgSetting::OnBnClickedButton1()
{
	m_bDoUpdate = true;
	m_pCoreManager->InitNumberFile();
	AfxMessageBox(_T("Load number file successfully"));
}

void CTabDlgSetting::OnNMClickListCom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	LVHITTESTINFO hitinfo;
	*pResult = 0;

	bool bChecked = false;

	 //Copy click point
	hitinfo.pt = pNMListView->ptAction;

	 //Make the hit test...
	m_nCurrentFocus = m_ListCom.HitTest(&hitinfo); 
	if(m_nCurrentFocus<0)
		return;

	CUsbInterface* pUsb = (CUsbInterface*)m_pCoreManager->m_pThreadManager->GetThread(m_nCurrentFocus);

	SetDlgItemText(IDC_COM_NAME, pUsb->GetName().c_str());
	SetDlgItemText(IDC_COM_NUMBER, pUsb->getNumber());
	SetDlgItemText(IDC_SIM_NUMBER, pUsb->getNumber());
	SetDlgItemInt(IDC_SMS_CAP, pUsb->getSmsRange());

	if(pUsb->GetStatus())
		((CButton*)GetDlgItem(IDC_COM_ENBALE))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_COM_ENBALE))->SetCheck(false);

	if(pUsb->isRunning())
	{
		GetDlgItem(IDC_SMS_CAP)->EnableWindow(false);
		GetDlgItem(IDC_COM_ENBALE)->EnableWindow(false);
		GetDlgItem(IDC_APPLY)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_SMS_CAP)->EnableWindow(true);
		GetDlgItem(IDC_COM_ENBALE)->EnableWindow(true);		
		GetDlgItem(IDC_APPLY)->EnableWindow(true);
	}
}

void CTabDlgSetting::OnBnClickedApply()
{	
	if(m_nCurrentFocus<0)
		return;

	// TODO: Add your control notification handler code here
	CUsbInterface* pUsb = (CUsbInterface*)m_pCoreManager->m_pThreadManager->GetThread(m_nCurrentFocus);
		
	CString sSmsCap;
	CString sSimNumber;
	bool bEnable;
	bool bAllCom;

	GetDlgItemText(IDC_SMS_CAP, sSmsCap);
	GetDlgItemText(IDC_SIM_NUMBER, sSimNumber);

	bEnable = ((CButton*)GetDlgItem(IDC_COM_ENBALE))->GetCheck();
	bAllCom = ((CButton*)GetDlgItem(IDC_ALL_COM))->GetCheck();

	pUsb->SetRange(atoi(sSmsCap.GetString()));	
	pUsb->SetActive(bEnable);
	pUsb->SetSimNumber(sSimNumber.GetString());

	if(bAllCom)
	{
		for(int i=0; i<m_pCoreManager->m_pThreadManager->GetSize(); i++)
		{
			CUsbInterface* pUsb = (CUsbInterface*)m_pCoreManager->m_pThreadManager->GetThread(i);
			pUsb->SetRange(atoi(sSmsCap.GetString()));
		}
	}
	
	//thuc hien update lai bang
	m_bDoUpdate = true;
}
