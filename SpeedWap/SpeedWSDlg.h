
// SpeedWSDlg.h : header file
//

#pragma once

#include "resource.h"

class CCoreSpam;


// CSpeedWSDlg dialog
class CSpeedWSDlg : public CDialog
{
// Construction
public:
	CSpeedWSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SPEEDWS_DIALOG };

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
	int m_nSpamMode; // 0 la SMS, 1 la Wappush
	bool m_bOnSpam;

	char m_szMsg[1024];

	CCoreSpam* m_pCoreSpam;

	bool m_bMainThreadRun;

public:
	afx_msg void OnBnClickedSmsChoice();
	afx_msg void OnBnClickedWapChoice();
	afx_msg void OnBnClickedStartSend();
	afx_msg void OnBnClickedStopSend();
	afx_msg void OnBnClickedComDetail();

	static UINT MainThread(void* pUser);

	void NotifySpam(const char* szMsg);
	void NotifyStatus(const char* szMsg);
	afx_msg void OnDestroy();

	//virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
