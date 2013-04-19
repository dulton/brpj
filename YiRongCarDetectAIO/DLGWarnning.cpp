// DLGWarnning.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGWarnning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGWarnning dialog


CDLGWarnning::CDLGWarnning(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGWarnning::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGWarnning)
	m_warntxt = _T("");
	//}}AFX_DATA_INIT
	m_wintxt=_T("");
}


void CDLGWarnning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGWarnning)
	DDX_Text(pDX, IDC_STATIC_WARNTXT, m_warntxt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGWarnning, CDialog)
	//{{AFX_MSG_MAP(CDLGWarnning)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGWarnning message handlers
BOOL CDLGWarnning::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_wintxt);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
///警告框 用在再次确认提示上
/*
	CDLGWarnning dlgw;
	dlgw.m_wintxt="系统设置导入";					//窗口标题
	dlgw.m_warntxt="导入设置文件将使原设置被覆盖";	//窗口内容
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{
		///
	}
	else if (nResponse == IDCANCEL)
		return ;
	else
		return ;
*/