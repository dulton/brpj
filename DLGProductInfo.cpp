// DLGProductInfo.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGProductInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGProductInfo dialog


CDLGProductInfo::CDLGProductInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGProductInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGProductInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLGProductInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGProductInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGProductInfo, CDialog)
	//{{AFX_MSG_MAP(CDLGProductInfo)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGProductInfo message handlers

BOOL CDLGProductInfo::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();
	
	AutoSize();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGProductInfo::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);


	Invalidate();
}

void CDLGProductInfo::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	
}
