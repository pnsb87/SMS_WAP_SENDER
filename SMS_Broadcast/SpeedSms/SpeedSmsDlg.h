
// SpeedSmsDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "TabDlgSetting.h"
#include "TabDlgSms.h"
#include "InstantMsg.h"
#include "InstantWap.h"

#define _IDLE_		0
#define _SENDING_	1
#define _PAUSE_		2

#define _ACTIVE_	1
#define	_DEACTIVE_	0

class CCoreManager;
class CScan;

// CSpeedSmsDlg dialog
class CSpeedSmsDlg : public CDialog
{
// Construction
public:
	CSpeedSmsDlg(CWnd* pParent = NULL);	// standard constructor
	
// Dialog Data
	enum { IDD = IDD_SPEEDSMS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl			m_tabControl;
	CTabDlgSetting		m_tSettingTab;
	CTabDlgSms			m_tSmsTab;
	CInstantMsg			m_tInstantMsg;
	CInstantWap			m_tInstantWap;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);

	static UINT MainThread(void* pUser);

public:

	int				m_nStatus;
	CCoreManager*	m_pCoreManager;
	CScan*			m_pScanDlg;			
	afx_msg void OnClose();
};
