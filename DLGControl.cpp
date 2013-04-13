// DLGControl.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGControl.h"

#include "DLGSettings.h"
extern CDLGSettings DlgSettings;

#include "DLGVideoList.h"
#include "DLGProductList.h"

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
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SHOTFRAME, OnButtonShotframe)
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, OnButtonSettings)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOLIST, OnButtonVideolist)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCTLIST, OnButtonProductlist)
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
	
}

void CDLGControl::OnButtonPause() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGControl::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGControl::OnButtonShotframe() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGControl::OnButtonSettings() 
{
	// TODO: Add your control notification handler code here
	DlgSettings.DoModal();
}

void CDLGControl::OnButtonVideolist() 
{
	// TODO: Add your control notification handler code here
	//打开文件所在的文件夹
	ShellExecute(this->m_hWnd,
		"open",
		"explorer.exe",
		"/e,/select,D:\\GGG\\YiRongCarDetectAIO\\IO定义\\历史识别报警表 数据库IO规则.txt",
		NULL,SW_NORMAL);

	CDLGVideoList DlgVideoList;
	DlgVideoList.DoModal();
}

void CDLGControl::OnButtonProductlist() 
{
	// TODO: Add your control notification handler code here
	CDLGProductList DlgProductList;
	DlgProductList.DoModal();
}
