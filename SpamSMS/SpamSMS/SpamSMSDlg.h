
// SpamSMSDlg.h : header file
//

#pragma once


class CSerialCom;

// CSpamSMSDlg dialog
class CSpamSMSDlg : public CDialog
{
// Construction
public:
	CSpamSMSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SPAMSMS_DIALOG };

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
	CSerialCom* m_pSerialPort;
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedButton1();
};
