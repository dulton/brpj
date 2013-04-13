// DLGVideoList.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGVideoList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoList dialog


CDLGVideoList::CDLGVideoList(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGVideoList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGVideoList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLGVideoList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGVideoList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGVideoList, CDialog)
	//{{AFX_MSG_MAP(CDLGVideoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoList message handlers
BOOL CDLGVideoList::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGVideoList::OnOK() 
{
	// TODO: Add extra validation here
	

}
