// DLGWarnning.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
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
	DDX_Control(pDX, IDOK, m_b_ok);
	DDX_Control(pDX, IDCANCEL, m_b_cancel);
}


BEGIN_MESSAGE_MAP(CDLGWarnning, CDialog)
	//{{AFX_MSG_MAP(CDLGWarnning)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGWarnning message handlers
BOOL CDLGWarnning::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_wintxt);

	m_b_ok.LoadBitmaps(IDB_OK_BUTTON,IDB_OK_BUTTON_MOVE,NULL,NULL);
	m_b_ok.SizeToContent();		//自适应图片大小

	m_b_cancel.LoadBitmaps(IDB_CANCEL_BUTTON,IDB_CANCEL_BUTTON_MOVE,NULL,NULL);
	m_b_cancel.SizeToContent();		//自适应图片大小

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
void CDLGWarnning::OnPaint()
{

	CPaintDC dc(this); // device context for painting
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_FIND_BACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	CDialog::OnPaint();

}
//静态文本控件 透明
HBRUSH CDLGWarnning::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); // 设置透明背景
		// TODO: Change any attributes of the DC here
		pDC->SetTextColor(RGB(0, 0, 0)); // 设置文本颜色
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		hbr=(HBRUSH)GetStockObject(HOLLOW_BRUSH); // 返回透明画刷	
		// TODO: Return a different brush if the default is not desired
	}

	return hbr;
}