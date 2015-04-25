// DLGAddArea.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGAddArea.h"


// CDLGAddArea dialog

IMPLEMENT_DYNAMIC(CDLGAddArea, CDialog)

CDLGAddArea::CDLGAddArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGAddArea::IDD, pParent)
	, m_AddArea(_T(""))
{

}

CDLGAddArea::~CDLGAddArea()
{
}

void CDLGAddArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDAREA, m_AddArea);
	DDV_MaxChars(pDX, m_AddArea, 32);
	DDX_Control(pDX, IDOK, m_b_ok);
	DDX_Control(pDX, IDCANCEL, m_b_cancel);
}


BEGIN_MESSAGE_MAP(CDLGAddArea, CDialog)

	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CDLGAddArea::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_b_ok.LoadBitmaps(IDB_OK_BUTTON,IDB_OK_BUTTON_MOVE,NULL,NULL);
	m_b_ok.SizeToContent();		//自适应图片大小

	m_b_cancel.LoadBitmaps(IDB_CANCEL_BUTTON,IDB_CANCEL_BUTTON_MOVE,NULL,NULL);
	m_b_cancel.SizeToContent();		//自适应图片大小

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDLGAddArea::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}


void CDLGAddArea::OnPaint()
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