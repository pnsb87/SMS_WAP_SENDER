#pragma once


// CScan dialog

class CScan : public CDialog
{
	DECLARE_DYNAMIC(CScan)

public:
	CScan(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScan();

// Dialog Data
	enum { IDD = IDD_SCAN };

	void OnOk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
