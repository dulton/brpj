// DLGhelp.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGhelp dialog


CDLGhelp::CDLGhelp(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGhelp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGhelp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLGhelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGhelp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGhelp, CDialog)
	//{{AFX_MSG_MAP(CDLGhelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGhelp message handlers


BOOL CDLGhelp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//////显示标题///////////////////
	CString modeflag;

	//结果输出到平台 模式 =1  单机版=0 
	if(YRVM_PINGTAI_MODE)
	{
		modeflag="P";

		if(0==ALLTAB_DETECT_CAR_MODE)
		{
			//电动车混合模式
			if(YRVM_PINGTAI_ELECAR_MIX_MODE)
				modeflag+="C";
			else
				modeflag+="E";
		}
	}
	else
		modeflag="S";


	CString str;
	str.Format("分支%s  构建时间:%s-%s",modeflag.GetBuffer(0),__DATE__,__TIME__);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDLGhelp::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDLGhelp::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
