#pragma once
#include <string>
#include <vector>
#include "afxwin.h"

using namespace std;

class CCoreManager;


// CInstantWap dialog

class CInstantWap : public CDialog
{
	DECLARE_DYNAMIC(CInstantWap)

public:
	CInstantWap(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstantWap();

	// Dialog Data
	enum { IDD = IDD_INSTANT_WAP };

	virtual BOOL OnInitDialog();
	static UINT MainThread(void* pUser);
	void LogOnScreen(char *psz,...);

	void SetCoreManager(CCoreManager* pCore);

	void UpdateNumberList(/*vector<string> vListCom*/);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CCoreManager*	m_pCoreManager;
	afx_msg void OnBnClickedSend();
	CComboBox m_ListCom;
};
