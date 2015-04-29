// DLGshortcut.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGshortcut.h"

//////////////////////
#include "SmartVillageDlg.h"
extern CSmartVillageDlg *DlgMain;
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDLGshortcut dialog


CDLGshortcut::CDLGshortcut(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGshortcut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGshortcut)
	//}}AFX_DATA_INIT
	CarEnable=false;

	PreviewEnable=false;
	RecordEnable=false;

	faceEnable=false;
	adjustEnable=false;
	screenmode=SCREEN_16MODE;
}


void CDLGshortcut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGshortcut)

	DDX_Control(pDX, IDC_BUTTON_PREVIEW, m_preview);
	DDX_Control(pDX, IDC_BUTTON_MODE9, m_9mode);
	DDX_Control(pDX, IDC_BUTTON_MODE4, m_4mode);
	DDX_Control(pDX, IDC_BUTTON_MODE16, m_16mode);
	DDX_Control(pDX, IDC_BUTTON_MODE1, m_1mode);
	DDX_Control(pDX, IDC_BUTTON_CAPBMP, m_Capbmp);
	DDX_Control(pDX, IDC_BUTTON_CAR, m_Car);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_AUTOADJUST, m_adjust);
	DDX_Control(pDX, IDC_BUTTON_FACE, m_face);
	DDX_Control(pDX, IDC_BUTTON_CAR_CLEAR, m_btnCarClear);
	DDX_Control(pDX, IDC_BUTTON_STOP_ALL, m_btnStopAll);
}


BEGIN_MESSAGE_MAP(CDLGshortcut, CDialog)
	//{{AFX_MSG_MAP(CDLGshortcut)
	ON_BN_CLICKED(IDC_BUTTON_CAR, OnButtonCar)
	ON_BN_CLICKED(IDC_BUTTON_CAPBMP, OnButtonCapbmp)
	ON_BN_CLICKED(IDC_BUTTON_MODE1, OnButtonMode1)
	ON_BN_CLICKED(IDC_BUTTON_MODE4, OnButtonMode4)
	ON_BN_CLICKED(IDC_BUTTON_MODE9, OnButtonMode9)
	ON_BN_CLICKED(IDC_BUTTON_MODE16, OnButtonMode16)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_WM_PAINT()

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_AUTOADJUST, &CDLGshortcut::OnBnClickedButtonAutoadjust)
	ON_BN_CLICKED(IDC_BUTTON_FACE, &CDLGshortcut::OnBnClickedButtonFace)
	ON_BN_CLICKED(IDC_BUTTON_CAR_CLEAR, &CDLGshortcut::OnBnClickedButtonCarClear)
	ON_BN_CLICKED(IDC_BUTTON_STOP_ALL, &CDLGshortcut::OnBnClickedButtonStopAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGshortcut message handlers
BOOL CDLGshortcut::OnInitDialog()
{
	CDialog::OnInitDialog();



	//识别按钮图片
	CarBMP();
	m_Car.SizeToContent();		//自适应图片大小

	//抓图按钮图片
	m_Capbmp.LoadBitmaps(IDB_CAPBMP_OPEN,IDB_CAPBMP_MOVE,NULL,NULL);
	m_Capbmp.SizeToContent();		//自适应图片大小

	m_btnStopAll.LoadBitmaps(IDB_CAPBMP_OPEN,IDB_CAPBMP_MOVE,NULL,NULL);

	m_btnCarClear.LoadBitmaps(IDB_CAR_CLEAR,IDB_CAR_CLEAR_PRESS,NULL,NULL);
	m_btnCarClear.SizeToContent();

	m_btnStopAll.LoadBitmaps(IDB_STOP_ALL,IDB_STOP_ALL_PRESS,NULL,NULL);
	m_btnStopAll.SizeToContent();

	 AdjustBMP();
	 m_adjust.SizeToContent();		//自适应图片大小
	 FaceBMP() ;
	 m_face.SizeToContent();		//自适应图片大小
	//预览按钮图片
	PreviewBMP();
	m_preview.SizeToContent();		//自适应图片大小

	//窗口按钮图片
	m_1mode.LoadBitmaps(IDB_MODE1_CLOSE,NULL,NULL,NULL);
	m_4mode.LoadBitmaps(IDB_MODE4_CLOSE,NULL,NULL,NULL);
	m_9mode.LoadBitmaps(IDB_MODE9_CLOSE,NULL,NULL,NULL);
	m_16mode.LoadBitmaps(IDB_MODE16_CLOSE,NULL,NULL,NULL);
	m_1mode.SizeToContent();		//自适应图片大小
	m_4mode.SizeToContent();		//自适应图片大小
	m_9mode.SizeToContent();		//自适应图片大小
	m_16mode.SizeToContent();		//自适应图片大小

	ModeBMP(SCREEN_16MODE,SCREEN_16MODE);


	//提示
	m_ToolTip.Create(this);     

	m_ToolTip.AddTool(&m_preview,"启动/关闭预览");  
	m_ToolTip.AddTool(&m_9mode,"九画面模式");  
	m_ToolTip.AddTool(&m_4mode,"四画面模式");  
	m_ToolTip.AddTool(&m_16mode,"十六画面模式");  
	m_ToolTip.AddTool(&m_1mode,"单画面模式");  
	m_ToolTip.AddTool(&m_Capbmp,"立即截图");  
	m_ToolTip.AddTool(&m_Car,"启动/关闭车牌识别");  
	m_ToolTip.AddTool(&m_adjust,"自动调整");  
	m_ToolTip.AddTool(&m_face,"启动/关闭人脸识别"); 
	m_ToolTip.AddTool(&m_btnCarClear,"清空当前车牌记录");
	m_ToolTip.AddTool(&m_btnStopAll,"停止全部");

	//放在最后
	AutoSize();

 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGshortcut::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);


	int w=16,g=10;
	int s=(rc.Width()-9*(w+g))/2;

	
	CRect Rect;
	Rect.top=	rc.top ;
	Rect.bottom=rc.top+w;
	Rect.left = rc.left+s;
	Rect.right = Rect.left+w;
	m_1mode.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_4mode.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_9mode.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_16mode.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_adjust.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_btnStopAll.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_preview.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_Car.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_face.MoveWindow(Rect);

	Rect.left = Rect.right+g;
	Rect.right = Rect.left+w;
	m_Capbmp.MoveWindow(Rect);

	Rect.left = rc.right - 10 - w;
	Rect.right = rc.right - 10;
	m_btnCarClear.MoveWindow(Rect);

	Invalidate();
}

void CDLGshortcut::OnCancel()
{
	DlgMain->OnCancel();
}
void CDLGshortcut::OnOK()
{

}


BOOL CDLGshortcut::PreTranslateMessage(MSG* pMsg) 
{
	//w为了TOOLTIP 增加
	m_ToolTip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CDLGshortcut::ModeBMP(int oldmode,int newmode) 
{
	//设置按钮图片
	switch(oldmode)
	{
	case SCREEN_1MODE:
		m_1mode.LoadBitmaps(IDB_MODE1_CLOSE,NULL,NULL,NULL);
		m_1mode.Invalidate();
		break;
	case SCREEN_4MODE:
		m_4mode.LoadBitmaps(IDB_MODE4_CLOSE,NULL,NULL,NULL);
		m_4mode.Invalidate();
		break;
	case SCREEN_9MODE:
		m_9mode.LoadBitmaps(IDB_MODE9_CLOSE,NULL,NULL,NULL);
		m_9mode.Invalidate();
		break;
	case SCREEN_16MODE:
		m_16mode.LoadBitmaps(IDB_MODE16_CLOSE,NULL,NULL,NULL);
		m_16mode.Invalidate();
		break;
	default:
		DlgMain->ShowRuntimeMessage("CDLGshortcut::ModeBMP() error OLD",1);
		break;
	}
	
	//设置按钮图片
	switch(newmode)
	{
	case SCREEN_1MODE:
		m_1mode.LoadBitmaps(IDB_MODE1_OPEN,NULL,NULL,NULL);
		m_1mode.Invalidate();
		break;
	case SCREEN_4MODE:
		m_4mode.LoadBitmaps(IDB_MODE4_OPEN,NULL,NULL,NULL);
		m_4mode.Invalidate();
		break;
	case SCREEN_9MODE:
		m_9mode.LoadBitmaps(IDB_MODE9_OPEN,NULL,NULL,NULL);
		m_9mode.Invalidate();
		break;
	case SCREEN_16MODE:
		m_16mode.LoadBitmaps(IDB_MODE16_OPEN,NULL,NULL,NULL);
		m_16mode.Invalidate();
		break;
	default:
		DlgMain->ShowRuntimeMessage("CDLGshortcut::ModeBMP() error NEW",1);
		break;
	}
}

void CDLGshortcut::OnButtonMode1() 
{
	// TODO: Add your control notification handler code here
	//设置按钮图片
	ModeBMP(screenmode,SCREEN_1MODE);
	screenmode=SCREEN_1MODE;

	//以下处理其他事情
	DlgMain->DlgTabVideo.DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgTabVideo.DlgNormal.m_screen=0;
	DlgMain->DlgTabVideo.DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::OnButtonMode4() 
{
	// TODO: Add your control notification handler code here
	//设置按钮图片
	ModeBMP(screenmode,SCREEN_4MODE);
	screenmode=SCREEN_4MODE;

	//以下处理其他事情
	DlgMain->DlgTabVideo.DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgTabVideo.DlgNormal.m_screen=1;
	DlgMain->DlgTabVideo.DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::OnButtonMode9() 
{
	// TODO: Add your control notification handler code here
	//设置按钮图片
	ModeBMP(screenmode,SCREEN_9MODE);
	screenmode=SCREEN_9MODE;

	//以下处理其他事情
	DlgMain->DlgTabVideo.DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgTabVideo.DlgNormal.m_screen=2;
	DlgMain->DlgTabVideo.DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::OnButtonMode16() 
{
	// TODO: Add your control notification handler code here
	//设置按钮图片
	ModeBMP(screenmode,SCREEN_16MODE);
	screenmode=SCREEN_16MODE;

	//以下处理其他事情
	DlgMain->DlgTabVideo.DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgTabVideo.DlgNormal.m_screen=3;
	DlgMain->DlgTabVideo.DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::PreviewBMP()
{
	//设置按钮图片
	if(true==PreviewEnable)
		m_preview.LoadBitmaps(IDB_STOP_PLAY,NULL,NULL,NULL);
	else
		m_preview.LoadBitmaps(IDB_START_PLAY,NULL,NULL,NULL);
	
	m_preview.Invalidate();
}

void CDLGshortcut::OnButtonPreview() 
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.DlgNormal.OnButtonOpenPreview();
	
	//以下处理其他事情
}

void CDLGshortcut::CarBMP() 
{
	//设置按钮
	if(true==CarEnable)
		m_Car.LoadBitmaps(IDB_CAR_OPEN,NULL,NULL,IDB_CAR_DISABLE);
	else
		m_Car.LoadBitmaps(IDB_CAR_CLOSE,NULL,NULL,IDB_CAR_DISABLE);
	m_Car.Invalidate();
}

void CDLGshortcut::OnButtonCar() 
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.DlgNormal.OnButtonOpenCarDetect();
	//以下处理其他事情
}

void CDLGshortcut::FaceBMP() 
{
	//设置按钮
	if(true==faceEnable)
		m_face.LoadBitmaps(IDB_MODE_FACE,NULL,NULL,IDB_CAR_DISABLE);
	else
		m_face.LoadBitmaps(IDB_MODE_FACE_DIS,NULL,NULL,IDB_CAR_DISABLE);
	m_face.Invalidate();
}

void CDLGshortcut::OnBnClickedButtonFace()
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.DlgNormal.OnButtonOpenFaceDetect();
}


void CDLGshortcut::OnButtonCapbmp() 
{
	// TODO: Add your control notification handler code here
	//以下处理其他事情
	DlgMain->DlgTabVideo.DlgNormal.OnButtonCapbmp();

}
void CDLGshortcut::AdjustBMP()
{
	//设置按钮图片
	if(true==adjustEnable)
		m_adjust.LoadBitmaps(IDB_ADJUST,NULL,NULL,NULL);
	else
		m_adjust.LoadBitmaps(IDB_ADJUST_DIS,NULL,NULL,NULL);

	m_adjust.Invalidate();
}


void CDLGshortcut::OnBnClickedButtonAutoadjust()
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.DlgScreen.m_screenPannel.SetAutoAdjustPos(!DlgMain->DlgTabVideo.DlgScreen.m_screenPannel.GetAutoAdjustPos());

	adjustEnable=DlgMain->DlgTabVideo.DlgScreen.m_screenPannel.GetAutoAdjustPos();

	AdjustBMP();

}

void CDLGshortcut::OnBnClickedButtonCarClear()
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.m_ListImg.ClearImageList();
	DlgMain->DlgTabVideo.m_ListCar.DeleteAllItems();
	DlgMain->DlgTabVideo.m_ListCarTotal=0;
}

void CDLGshortcut::OnBnClickedButtonStopAll()
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgTabVideo.DlgNormal.OnButtonClosePreviewAll();
}


void CDLGshortcut::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages

	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     
	
	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_BACK_SHORTCUT);    
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



