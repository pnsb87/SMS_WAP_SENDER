#pragma once
#include "afxwin.h"
#include <string>
#include <vector>

using namespace std;

class CCoreManager;

// CInstantMsg dialog

class CInstantMsg : public CDialog
{
	DECLARE_DYNAMIC(CInstantMsg)

public:
	CInstantMsg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstantMsg();

// Dialog Data
	enum { IDD = IDD_INSTANT_MSG };

	virtual BOOL OnInitDialog();
	static UINT MainThread(void* pUser);
	void LogOnScreen(char *psz,...);

	void SetCoreManager(CCoreManager* pCore);

	void UpdateNumberList(/*vector<string> vListCom*/);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSend();
	CComboBox m_ListCom;
	CEdit m_MobileNo;
	CEdit m_Msg;

	CCoreManager*	m_pCoreManager;
};
