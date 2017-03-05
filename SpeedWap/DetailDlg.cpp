// DetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedWS.h"
#include "DetailDlg.h"


// CDetailDlg dialog

IMPLEMENT_DYNAMIC(CDetailDlg, CDialog)

CDetailDlg::CDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailDlg::IDD, pParent)
{

}

CDetailDlg::~CDetailDlg()
{
}

void CDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetailDlg, CDialog)
END_MESSAGE_MAP()


// CDetailDlg message handlers
