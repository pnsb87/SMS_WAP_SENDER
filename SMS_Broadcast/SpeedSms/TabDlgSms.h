#pragma once
#include "afxwin.h"

class CCoreManager;

// CTabDlgSms dialog

class CTabDlgSms : public CDialog
{
	DECLARE_DYNAMIC(CTabDlgSms)

public:
	CTabDlgSms(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabDlgSms();
	void setPauseButtonTitle(CString title);
// Dialog Data
	enum { IDD = IDD_SMS_DLG };

	virtual BOOL OnInitDialog();

	void SetCoreManager(CCoreManager* pCore);
	int getMode(){return m_nMode;};

	void LogOnScreen(char *psz,...);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendButton();
	afx_msg void OnBnClickedPauseButton();
	afx_msg void OnBnClickedStopButton();

	static UINT MainThread(void* pUser);

	int m_nMode;
	bool bPaused;
	afx_msg void OnBnClickedRadioSms();
	afx_msg void OnBnClickedRadioWap();
	CEdit m_SmsText;
	CEdit m_WapText;
	CListBox m_ListNotify;

	CCoreManager* m_pCoreManager;
	CString m_smsLength;
	afx_msg void OnEnChangeContext();
	CString m_txtSmsText;
	CString m_txtWapUrl;
	CButton m_StartButton;
	CButton m_PauseButton;
	CButton m_StopButton;
	afx_msg void OnBnClickedStopButton2();
	CButton m_reportButton;
};
