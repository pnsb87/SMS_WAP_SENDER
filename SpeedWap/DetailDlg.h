#pragma once


// CDetailDlg dialog

class CDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetailDlg)

public:
	CDetailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDetailDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
