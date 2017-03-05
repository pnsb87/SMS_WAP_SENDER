// Scan.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedSms.h"
#include "Scan.h"


// CScan dialog

IMPLEMENT_DYNAMIC(CScan, CDialog)

CScan::CScan(CWnd* pParent /*=NULL*/)
	: CDialog(CScan::IDD, pParent)
{

}

CScan::~CScan()
{
}

void CScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScan, CDialog)
END_MESSAGE_MAP()


// CScan message handlers

void CScan::OnOk()
{
	CDialog::OnOK();
}