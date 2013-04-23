// DLGControl.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGControl.h"

#include "DLGSettings.h"
extern CDLGSettings DlgSettings;

////////////////////////////////////
#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;


#include "DLGVideoList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGControl dialog


CDLGControl::CDLGControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGControl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLGControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGControl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGControl, CDialog)
	//{{AFX_MSG_MAP(CDLGControl)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)

	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SHOTFRAME, OnButtonShotframe)
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, OnButtonSettings)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOLIST, OnButtonVideolist)
	ON_BN_CLICKED(IDC_BUTTON_IPTREE, OnButtonIptree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGControl message handlers

BOOL CDLGControl::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();
	
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGControl::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);


	Invalidate();
}

void CDLGControl::OnButtonRecord() 
{
	// TODO: Add your control notification handler code here
	pCMainDlg->DlgPlaywin.StartRecord();
}

void CDLGControl::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	pCMainDlg->DlgPlaywin.StopRecord();
}

void CDLGControl::OnButtonShotframe() 
{
	// TODO: Add your control notification handler code here
	pCMainDlg->DlgPlaywin.CapturePic();
}


void CDLGControl::OnButtonSettings() 
{
	// TODO: Add your control notification handler code here
	DlgSettings.DoModal();
}

void CDLGControl::OnButtonVideolist() 
{
	// TODO: Add your control notification handler code here

	CDLGVideoList DlgVideoList;
	DlgVideoList.DoModal();
}

void CDLGControl::OnButtonIptree() 
{
	// TODO: Add your control notification handler code here

	pCMainDlg->DlgNewDevice.DoModal();
	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		pCMainDlg->DlgPlaywin.StartPlay(i,\
										pCMainDlg->DlgNewDevice.device[i].name,\
										pCMainDlg->DlgNewDevice.device[i].ip,\
										pCMainDlg->DlgNewDevice.device[i].port,\
										pCMainDlg->DlgNewDevice.device[i].user,\
										pCMainDlg->DlgNewDevice.device[i].psw);
	}
}
