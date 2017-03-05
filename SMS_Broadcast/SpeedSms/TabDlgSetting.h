#pragma once
#include "afxcmn.h"
#include "resource.h"

class CCoreManager;

// CTabDlgSetting dialog

class CTabDlgSetting : public CDialog
{
	DECLARE_DYNAMIC(CTabDlgSetting)

public:
	CTabDlgSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabDlgSetting();

// Dialog Data
	enum { IDD = IDD_WAP_DLG };

	virtual BOOL OnInitDialog();

	void SetCoreManager(CCoreManager* pCore);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CListCtrl m_ListCom;

	bool	m_bDoUpdate;
	int		m_nCurrentFocus;

	afx_msg void OnBnClickedScan();

	static UINT MainThread(void* pUser);

	CCoreManager* m_pCoreManager;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMClickListCom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedApply();
};
