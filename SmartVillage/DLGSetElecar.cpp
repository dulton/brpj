// DLGSetElecar.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGSetElecar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGSetElecar dialog


CDLGSetElecar::CDLGSetElecar(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSetElecar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSetElecar)
	m_JPGquality = 75;
	m_RectDown = 90;
	m_RectLeft = 0;
	m_RectRight = 100;
	m_RectUp = 10;
	m_MaxWidth = 60;
	m_MinWidth = 200;
	//}}AFX_DATA_INIT

	m_BgBrush.CreateSolidBrush(RGB(216,216,216));         // 背景的颜色
}


void CDLGSetElecar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetElecar)
	DDX_Text(pDX, IDC_EDIT_JPG_QUALITY, m_JPGquality);
	DDV_MinMaxInt(pDX, m_JPGquality, 1, 100);
	DDX_Text(pDX, IDC_EDIT_DOWN, m_RectDown);
	DDV_MinMaxInt(pDX, m_RectDown, 0, 100);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_RectLeft);
	DDV_MinMaxInt(pDX, m_RectLeft, 0, 100);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_RectRight);
	DDV_MinMaxInt(pDX, m_RectRight, 0, 100);
	DDX_Text(pDX, IDC_EDIT_UP, m_RectUp);
	DDV_MinMaxInt(pDX, m_RectUp, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MAX_WIDTH, m_MaxWidth);
	DDV_MinMaxInt(pDX, m_MaxWidth, 60, 200);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH, m_MinWidth);
	DDV_MinMaxInt(pDX, m_MinWidth, 60, 200);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_b_ok);
	DDX_Control(pDX, IDCANCEL, m_b_cancel);
}


BEGIN_MESSAGE_MAP(CDLGSetElecar, CDialog)
	//{{AFX_MSG_MAP(CDLGSetElecar)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSetElecar message handlers

BOOL CDLGSetElecar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_CHECK_OUTPUT_RECT))->SetCheck(CarSet.RedRect);
	((CButton*)GetDlgItem(IDC_CHECK_DESKEW))->SetCheck(CarSet.Deskew);
	((CButton*)GetDlgItem(IDC_CHECK_CAR_COLOR))->SetCheck(CarSet.CarColor);

	m_b_ok.LoadBitmaps(IDB_OK_BUTTON,IDB_OK_BUTTON_MOVE,NULL,NULL);
	m_b_ok.SizeToContent();		//自适应图片大小

	m_b_cancel.LoadBitmaps(IDB_CANCEL_BUTTON,IDB_CANCEL_BUTTON_MOVE,NULL,NULL);
	m_b_cancel.SizeToContent();		//自适应图片大小


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGSetElecar::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	CarSet.RedRect = ((CButton*)GetDlgItem(IDC_CHECK_OUTPUT_RECT))->GetCheck();
	CarSet.Deskew = ((CButton*)GetDlgItem(IDC_CHECK_DESKEW))->GetCheck();
	CarSet.CarColor = ((CButton*)GetDlgItem(IDC_CHECK_CAR_COLOR))->GetCheck();
	CarSet.JPGquality = m_JPGquality;
	CarSet.MaxWidth = m_MaxWidth;
	CarSet.MinWidth = m_MinWidth;
	CarSet.RangeRate.x0 = m_RectLeft;
	CarSet.RangeRate.x1 = m_RectRight;
	CarSet.RangeRate.y0 = m_RectUp;
	CarSet.RangeRate.y1 = m_RectDown;
}

void CDLGSetElecar::ReadOldConfig(struct CarSetStruct OldCarSet)
{
	strcpy(CarSet.DefaultChar,OldCarSet.DefaultChar);
	CarSet.Reliability = OldCarSet.Reliability;
	CarSet.RedRect = OldCarSet.RedRect;
	CarSet.Deskew = OldCarSet.Deskew;
	CarSet.CarColor = OldCarSet.CarColor;
	m_JPGquality = OldCarSet.JPGquality;	
	m_MaxWidth = OldCarSet.MaxWidth;
	m_MinWidth = OldCarSet.MinWidth;
	m_RectLeft = OldCarSet.RangeRate.x0;
	m_RectRight = OldCarSet.RangeRate.x1;
	m_RectUp = OldCarSet.RangeRate.y0;
	m_RectDown = OldCarSet.RangeRate.y1;

	for (int i=0; i<CAR_MASK_MAX; i++)
	{
		CarSet.Mask[i] = OldCarSet.Mask[i];
	}
}

void CDLGSetElecar::OnPaint()
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
HBRUSH CDLGSetElecar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_OTHER)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);

		// 返回背景色的画刷
		return m_BgBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_WIDTH)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);

		// 返回背景色的画刷
		return m_BgBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_AREA)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);

		// 返回背景色的画刷
		return m_BgBrush;
	}
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