// DLGshortcut.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGshortcut.h"

//////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

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
	AlarmEnable=false;
	PreviewEnable=false;
	RecordEnable=false;
	screenmode=SCREEN_16MODE;
}


void CDLGshortcut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGshortcut)
	DDX_Control(pDX, IDC_SET_CAR, m_setcar);
	DDX_Control(pDX, IDC_BUTTON_ALARM, m_alarm);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_record);
	DDX_Control(pDX, IDC_BUTTON_PREVIEW, m_preview);
	DDX_Control(pDX, IDC_BUTTON_MODE9, m_9mode);
	DDX_Control(pDX, IDC_BUTTON_MODE4, m_4mode);
	DDX_Control(pDX, IDC_BUTTON_MODE16, m_16mode);
	DDX_Control(pDX, IDC_BUTTON_MODE1, m_1mode);
	DDX_Control(pDX, IDC_BUTTON_CAPBMP, m_Capbmp);
	DDX_Control(pDX, IDC_BUTTON_CAR, m_Car);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGshortcut, CDialog)
	//{{AFX_MSG_MAP(CDLGshortcut)
	ON_BN_CLICKED(IDC_BUTTON_CAR, OnButtonCar)
	ON_BN_CLICKED(IDC_BUTTON_CAPBMP, OnButtonCapbmp)
	ON_BN_CLICKED(IDC_BUTTON_MODE1, OnButtonMode1)
	ON_BN_CLICKED(IDC_BUTTON_MODE4, OnButtonMode4)
	ON_BN_CLICKED(IDC_BUTTON_MODE9, OnButtonMode9)
	ON_BN_CLICKED(IDC_BUTTON_MODE16, OnButtonMode16)
	ON_BN_CLICKED(IDC_BUTTON_ALARM, OnButtonAlarm)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SET_CAR, OnSetCar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGshortcut message handlers
BOOL CDLGshortcut::OnInitDialog()
{
	CDialog::OnInitDialog();

#if ALLTAB_CLIENT_MODE

	GetDlgItem(IDC_BUTTON_CAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ALARM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SET_CAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RECORD)->EnableWindow(FALSE);

#endif

	//识别按钮图片
	CarBMP();
	m_Car.SizeToContent();		//自适应图片大小

	//抓图按钮图片
	m_Capbmp.LoadBitmaps(IDB_CAPBMP_OPEN,IDB_CAPBMP_MOVE,NULL,NULL);
	m_Capbmp.SizeToContent();		//自适应图片大小

	//报警按钮图片
	AlarmBMP();
	m_alarm.SizeToContent();		//自适应图片大小

	//录制按钮图片
	RecordBMP();
	m_record.SizeToContent();		//自适应图片大小

	//预览按钮图片
	PreviewBMP();
	m_preview.SizeToContent();		//自适应图片大小

	//窗口按钮图片
	m_1mode.LoadBitmaps(IDB_MODE1_CLOSE,IDB_MODE1_MOVE,NULL,NULL);
	m_4mode.LoadBitmaps(IDB_MODE4_CLOSE,IDB_MODE4_MOVE,NULL,NULL);
	m_9mode.LoadBitmaps(IDB_MODE9_CLOSE,IDB_MODE9_MOVE,NULL,NULL);
	m_16mode.LoadBitmaps(IDB_MODE16_CLOSE,IDB_MODE16_MOVE,NULL,NULL);
	m_1mode.SizeToContent();		//自适应图片大小
	m_4mode.SizeToContent();		//自适应图片大小
	m_9mode.SizeToContent();		//自适应图片大小
	m_16mode.SizeToContent();		//自适应图片大小

	ModeBMP(SCREEN_16MODE,SCREEN_16MODE);

	//车牌识别设置
	m_setcar.LoadBitmaps(IDB_SETCAR_OPEN,IDB_SETCAR_MOVE,NULL,IDB_SETCAR_CLOSE);
	m_setcar.SizeToContent();
	//提示
	m_ToolTip.Create(this);     
	m_ToolTip.AddTool(&m_setcar,"车牌识别设置");  
	m_ToolTip.AddTool(&m_alarm,"启动/关闭报警");  
	m_ToolTip.AddTool(&m_record,"启动/关闭录制");  
	m_ToolTip.AddTool(&m_preview,"启动/关闭预览");  
	m_ToolTip.AddTool(&m_9mode,"九画面模式");  
	m_ToolTip.AddTool(&m_4mode,"四画面模式");  
	m_ToolTip.AddTool(&m_16mode,"十六画面模式");  
	m_ToolTip.AddTool(&m_1mode,"单画面模式");  
	m_ToolTip.AddTool(&m_Capbmp,"立即抓拍");  
	m_ToolTip.AddTool(&m_Car,"启动/关闭识别");  

	//放在最后
	AutoSize();

 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGshortcut::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);


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
		m_1mode.LoadBitmaps(IDB_MODE1_CLOSE,IDB_MODE1_MOVE,NULL,NULL);
		m_1mode.Invalidate();
		break;
	case SCREEN_4MODE:
		m_4mode.LoadBitmaps(IDB_MODE4_CLOSE,IDB_MODE4_MOVE,NULL,NULL);
		m_4mode.Invalidate();
		break;
	case SCREEN_9MODE:
		m_9mode.LoadBitmaps(IDB_MODE9_CLOSE,IDB_MODE9_MOVE,NULL,NULL);
		m_9mode.Invalidate();
		break;
	case SCREEN_16MODE:
		m_16mode.LoadBitmaps(IDB_MODE16_CLOSE,IDB_MODE16_MOVE,NULL,NULL);
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
		m_1mode.LoadBitmaps(IDB_MODE1_OPEN,IDB_MODE1_MOVE,NULL,NULL);
		m_1mode.Invalidate();
		break;
	case SCREEN_4MODE:
		m_4mode.LoadBitmaps(IDB_MODE4_OPEN,IDB_MODE4_MOVE,NULL,NULL);
		m_4mode.Invalidate();
		break;
	case SCREEN_9MODE:
		m_9mode.LoadBitmaps(IDB_MODE9_OPEN,IDB_MODE9_MOVE,NULL,NULL);
		m_9mode.Invalidate();
		break;
	case SCREEN_16MODE:
		m_16mode.LoadBitmaps(IDB_MODE16_OPEN,IDB_MODE16_MOVE,NULL,NULL);
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
	DlgMain->DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgNormal.m_screen=0;
	DlgMain->DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::OnButtonMode4() 
{
	// TODO: Add your control notification handler code here
	//设置按钮图片
	ModeBMP(screenmode,SCREEN_4MODE);
	screenmode=SCREEN_4MODE;

	//以下处理其他事情
	DlgMain->DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgNormal.m_screen=1;
	DlgMain->DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::OnButtonMode9() 
{
	// TODO: Add your control notification handler code here
	//设置按钮图片
	ModeBMP(screenmode,SCREEN_9MODE);
	screenmode=SCREEN_9MODE;

	//以下处理其他事情
	DlgMain->DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgNormal.m_screen=2;
	DlgMain->DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::OnButtonMode16() 
{
	// TODO: Add your control notification handler code here
	//设置按钮图片
	ModeBMP(screenmode,SCREEN_16MODE);
	screenmode=SCREEN_16MODE;

	//以下处理其他事情
	DlgMain->DlgScreen.SwitchMultiWnd(screenmode);
	DlgMain->DlgNormal.m_screen=3;
	DlgMain->DlgNormal.UpdateData(FALSE);
}

void CDLGshortcut::PreviewBMP()
{
	//设置按钮图片
	if(true==PreviewEnable)
		m_preview.LoadBitmaps(IDB_PREVIEW_OPEN,IDB_PREVIEW_MOVE,NULL,NULL);
	else
		m_preview.LoadBitmaps(IDB_PREVIEW_CLOSE,IDB_PREVIEW_MOVE,NULL,NULL);
	
	m_preview.Invalidate();
}

void CDLGshortcut::OnButtonPreview() 
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgNormal.OnButtonOpenPreview();
	
	//以下处理其他事情
}

void CDLGshortcut::CarBMP() 
{
	//设置按钮
	if(true==CarEnable)
		m_Car.LoadBitmaps(IDB_CAR_OPEN,IDB_CAR_MOVE,NULL,IDB_CAR_DISABLE);
	else
		m_Car.LoadBitmaps(IDB_CAR_CLOSE,IDB_CAR_MOVE,NULL,IDB_CAR_DISABLE);
	m_Car.Invalidate();
}

void CDLGshortcut::OnButtonCar() 
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgNormal.OnButtonOpenDetect();
	//以下处理其他事情
}

void CDLGshortcut::AlarmBMP() 
{
	//设置按钮图片
	if(true==AlarmEnable)
		m_alarm.LoadBitmaps(IDB_ALARM_OPEN,IDB_ALARM_MOVE,NULL,IDB_ALARM_DISABLE);
	else
		m_alarm.LoadBitmaps(IDB_ALARM_CLOSE,IDB_ALARM_MOVE,NULL,IDB_ALARM_DISABLE);

	m_alarm.Invalidate();
}

void CDLGshortcut::OnButtonAlarm() 
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgNormal.OnButtonOpenAlarm();

	//以下处理其他事情
}

void CDLGshortcut::OnButtonCapbmp() 
{
	// TODO: Add your control notification handler code here
	//以下处理其他事情
	DlgMain->DlgNormal.OnButtonCapbmp();

}

void CDLGshortcut::RecordBMP() 
{
	//设置按钮图片
	if(true==RecordEnable)
		m_record.LoadBitmaps(IDB_RECORD_OPEN,IDB_RECORD_MOVE,NULL,IDB_RECORD_DISABLE);
	else
		m_record.LoadBitmaps(IDB_RECORD_CLOSE,IDB_RECORD_MOVE,NULL,IDB_RECORD_DISABLE);
	m_record.Invalidate();
}

void CDLGshortcut::OnButtonRecord() 
{
	DlgMain->DlgNormal.OnButtonOpenRecord();
}

void CDLGshortcut::OnSetCar() 
{
	// TODO: Add your control notification handler code here
	DlgMain->DlgNormal.OnButtonSetCar();
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

