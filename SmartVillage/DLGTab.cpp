// DLGTab.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGTab.h"
#include "SmartVillageDlg.h"
#include "DLGWarnning.h"

////////////////lynn/////////////////
extern CYRSVMySQL MySqlIO;
extern CSmartVillageDlg *DlgMain;
////////////////lynn/////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

// CDLGTab dialog

IMPLEMENT_DYNAMIC(CDLGTab, CDialog)

CDLGTab::CDLGTab(CWnd* pParent /*=NULL*/)
: CDialog(CDLGTab::IDD, pParent)
{

}

CDLGTab::~CDLGTab()
{
}

void CDLGTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_B_VIDEO, m_b_video);
	DDX_Control(pDX, IDC_B_CAR, m_b_car);
	DDX_Control(pDX, IDC_B_FACE, m_b_face);
}


BEGIN_MESSAGE_MAP(CDLGTab, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGTab::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGTab::OnCancel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_B_VIDEO, &CDLGTab::OnBnClickedBVideo)
	ON_BN_CLICKED(IDC_B_CAR, &CDLGTab::OnBnClickedBCar)
	ON_BN_CLICKED(IDC_B_FACE, &CDLGTab::OnBnClickedBFace)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CDLGTab::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_b_video.LoadBitmaps(IDB_TAB_VIDEO,NULL,NULL,NULL);
	m_b_video.SizeToContent();		//自适应图片大小

	m_b_face.LoadBitmaps(IDB_TAB_FACE_DIS,NULL,NULL,NULL);
	m_b_face.SizeToContent();		//自适应图片大小

	m_b_car.LoadBitmaps(IDB_TAB_CAR_DIS,NULL,NULL,NULL);
	m_b_car.SizeToContent();		//自适应图片大小

	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGTab::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);

	int w=115,h=36;

	//用户栏
	CRect user_Rect;
	user_Rect.top=	rc.top+2 ;
	user_Rect.bottom=rc.top+18;
	user_Rect.left = rc.left+w*6;
	user_Rect.right = rc.right;
	GetDlgItem(IDC_STATIC_USERNAME)->MoveWindow(user_Rect);

	//打印日志条
	printf_Rect.top = 	user_Rect.bottom;
	printf_Rect.bottom = printf_Rect.top+16;
	printf_Rect.left = rc.left+w*6;
	printf_Rect.right = rc.right;
	//必须 样式=重叠，边框=调整大小
	GetDlgItem(IDC_STATIC_PRINTF)->MoveWindow(printf_Rect);


	CRect video_Rect;
	video_Rect.top = 	rc.top;
	video_Rect.bottom = video_Rect.top+h;
	video_Rect.left = rc.left ;
	video_Rect.right = video_Rect.left+w;
	//必须 样式=重叠，边框=调整大小
	m_b_video.MoveWindow(video_Rect);

	CRect face_Rect;
	face_Rect.top = 	rc.top;
	face_Rect.bottom = face_Rect.top+h;
	face_Rect.left = rc.left+w ;
	face_Rect.right = face_Rect.left+w;
	//必须 样式=重叠，边框=调整大小
	m_b_face.MoveWindow(face_Rect);


	CRect car_Rect;
	car_Rect.top = 	rc.top;
	car_Rect.bottom = car_Rect.top+h;
	car_Rect.left = rc.left+w+w ;
	car_Rect.right = car_Rect.left+w;
	//必须 样式=重叠，边框=调整大小
	m_b_car.MoveWindow(car_Rect);

	Invalidate();
}

// CDLGTab message handlers

void CDLGTab::OnOk()
{
	// TODO: Add your control notification handler code here
	//	CDialog::OnOK();
}

void CDLGTab::OnCancel()
{
	// TODO: Add your control notification handler code here
	DlgMain->OnCancel();
}

void CDLGTab::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_TAB_BACK);    
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

void CDLGTab::OnBnClickedBVideo()
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.ShowWindow(SW_SHOW);
	DlgMain->DlgTabCar.ShowWindow(SW_HIDE);
	DlgMain->DlgTabFace.ShowWindow(SW_HIDE);


	m_b_video.LoadBitmaps(IDB_TAB_VIDEO,NULL,NULL,NULL);
	m_b_video.SizeToContent();		//自适应图片大小
	m_b_video.Invalidate();
	m_b_face.LoadBitmaps(IDB_TAB_FACE_DIS,NULL,NULL,NULL);
	m_b_face.SizeToContent();		//自适应图片大小
	m_b_face.Invalidate();
	m_b_car.LoadBitmaps(IDB_TAB_CAR_DIS,NULL,NULL,NULL);
	m_b_car.SizeToContent();		//自适应图片大小
	m_b_car.Invalidate();

}

void CDLGTab::OnBnClickedBCar()
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.ShowWindow(SW_HIDE);
	DlgMain->DlgTabCar.ShowWindow(SW_SHOW);
	DlgMain->DlgTabFace.ShowWindow(SW_HIDE);


	m_b_video.LoadBitmaps(IDB_TAB_VIDEO_DIS,NULL,NULL,NULL);
	m_b_video.SizeToContent();		//自适应图片大小
	m_b_video.Invalidate();
	m_b_face.LoadBitmaps(IDB_TAB_FACE_DIS,NULL,NULL,NULL);
	m_b_face.SizeToContent();		//自适应图片大小
	m_b_face.Invalidate();
	m_b_car.LoadBitmaps(IDB_TAB_CAR,NULL,NULL,NULL);
	m_b_car.SizeToContent();		//自适应图片大小
	m_b_car.Invalidate();
}

void CDLGTab::OnBnClickedBFace()
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.ShowWindow(SW_HIDE);
	DlgMain->DlgTabCar.ShowWindow(SW_HIDE);
	DlgMain->DlgTabFace.ShowWindow(SW_SHOW);

	m_b_video.LoadBitmaps(IDB_TAB_VIDEO_DIS,NULL,NULL,NULL);
	m_b_video.SizeToContent();		//自适应图片大小
	m_b_video.Invalidate();
	m_b_face.LoadBitmaps(IDB_TAB_FACE,NULL,NULL,NULL);
	m_b_face.SizeToContent();		//自适应图片大小
	m_b_face.Invalidate();
	m_b_car.LoadBitmaps(IDB_TAB_CAR_DIS,NULL,NULL,NULL);
	m_b_car.SizeToContent();		//自适应图片大小
	m_b_car.Invalidate();
	DlgMain->DlgTabFace.Init();
}

//静态文本控件 透明
HBRUSH CDLGTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); // 设置透明背景
		// TODO: Change any attributes of the DC here
		pDC->SetTextColor(RGB(255, 255,255)); // 设置文本颜色
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		hbr=(HBRUSH)GetStockObject(HOLLOW_BRUSH); // 返回透明画刷	
		// TODO: Return a different brush if the default is not desired
	}

	return hbr;
}
