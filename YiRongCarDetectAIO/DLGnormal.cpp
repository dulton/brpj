// DLGnormal.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGnormal.h"

//////////////////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;
//////////////////////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

//////////////////////////////////
#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGnormal dialog


CDLGnormal::CDLGnormal(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGnormal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGnormal)
	m_stream = 0;
	m_screen = 3;
	//}}AFX_DATA_INIT
}


void CDLGnormal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGnormal)
	DDX_Control(pDX, IDC_BUTTON_SET_CAR, m_b_set_car);
	DDX_Control(pDX, IDC_BUTTON_SET_BLACK, m_b_set_black);
	DDX_Control(pDX, IDC_BUTTON_OPENCHANNEL, m_b_channel);
	DDX_Control(pDX, IDC_BUTTON_OPEN_RECORD_ALL, m_b_record_openall);
	DDX_Control(pDX, IDC_BUTTON_OPEN_RECORD, m_b_record_open);
	DDX_Control(pDX, IDC_BUTTON_OPEN_PREVIEW_ALL, m_b_preview_openall);
	DDX_Control(pDX, IDC_BUTTON_OPEN_PREVIEW, m_b_preview_open);
	DDX_Control(pDX, IDC_BUTTON_OPEN_DETECT_ALL, m_b_detect_openall);
	DDX_Control(pDX, IDC_BUTTON_OPEN_DETECT, m_b_detect_open);
	DDX_Control(pDX, IDC_BUTTON_OPEN_ALARM_ALL, m_b_alarm_openall);
	DDX_Control(pDX, IDC_BUTTON_OPEN_ALARM, m_b_alarm_open);
	DDX_Control(pDX, IDC_BUTTON_HISTORY_VIDEO, m_b_history_video);
	DDX_Control(pDX, IDC_BUTTON_HISTORY_DETECT, m_b_history_detect);
	DDX_Control(pDX, IDC_BUTTON_HISTORY_ALARM, m_b_history_alarm);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_RECORD_ALL, m_b_record_closeall);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_PREVIEW_ALL, m_b_preview_closeall);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_DETECT_ALL, m_b_detect_closeall);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_ALARM_ALL, m_b_alarm_closeall);
	DDX_Control(pDX, IDC_BUTTON_CAPBMP, m_b_capbmp);
	DDX_CBIndex(pDX, IDC_COMBO_STREAM, m_stream);
	DDX_CBIndex(pDX, IDC_COMBO_SCREEN, m_screen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGnormal, CDialog)
	//{{AFX_MSG_MAP(CDLGnormal)
	ON_WM_PAINT()
	ON_CBN_CLOSEUP(IDC_COMBO_SCREEN, OnCloseupComboScreen)
	ON_CBN_CLOSEUP(IDC_COMBO_STREAM, OnCloseupComboStream)
	ON_BN_CLICKED(IDC_BUTTON_FULL_SCREEN, OnButtonFullScreen)
	ON_BN_CLICKED(IDC_BUTTON_OPENCHANNEL, OnButtonOpenchannel)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PREVIEW, OnButtonOpenPreview)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PREVIEW_ALL, OnButtonOpenPreviewAll)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_PREVIEW_ALL, OnButtonClosePreviewAll)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DETECT, OnButtonOpenDetect)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DETECT_ALL, OnButtonOpenDetectAll)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_DETECT_ALL, OnButtonCloseDetectAll)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RECORD, OnButtonOpenRecord)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RECORD_ALL, OnButtonOpenRecordAll)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_RECORD_ALL, OnButtonCloseRecordAll)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_ALARM, OnButtonOpenAlarm)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_ALARM_ALL, OnButtonOpenAlarmAll)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_ALARM_ALL, OnButtonCloseAlarmAll)
	ON_BN_CLICKED(IDC_BUTTON_CAPBMP, OnButtonCapbmp)
	ON_BN_CLICKED(IDC_BUTTON_SET_CAR, OnButtonSetCar)
	ON_BN_CLICKED(IDC_BUTTON_SET_BLACK, OnButtonSetBlack)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_VIDEO, OnButtonHistoryVideo)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_DETECT, OnButtonHistoryDetect)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_ALARM, OnButtonHistoryAlarm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGnormal message handlers
BOOL CDLGnormal::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch(DlgMain->DlgShortCut.screenmode)
	{
	case SCREEN_1MODE:
		m_screen=0;
		break;
	case SCREEN_4MODE:
		m_screen=1;
		break;
	case SCREEN_9MODE:
		m_screen=2;
		break;
	case SCREEN_16MODE:
		m_screen=3;
		break;
	default:
		DlgMain->ShowRuntimeMessage("CDLGnormal::OnInitDialog error",1);

		break;
	}

	/**********设置查询项*********/
	m_b_capbmp.LoadBitmaps(IDB_NORMAL_CAPBMP,IDB_NORMAL_CAPBMP_PUSH,NULL,NULL);
	m_b_capbmp.SizeToContent();		//自适应图片大小

	m_b_set_black.LoadBitmaps(IDB_NORMAL_BLACK_SET,IDB_NORMAL_BLACK_SET_PUSH,NULL,NULL);
	m_b_set_black.SizeToContent();		//自适应图片大小

	m_b_set_car.LoadBitmaps(IDB_NORMAL_CAR_SET,IDB_NORMAL_CAR_SET_PUSH,NULL,NULL);
	m_b_set_car.SizeToContent();		//自适应图片大小

	m_b_channel.LoadBitmaps(IDB_NORMAL_CHANNEL,IDB_NORMAL_CHANNEL_PUSH,NULL,NULL);
	m_b_channel.SizeToContent();		//自适应图片大小

	m_b_history_alarm.LoadBitmaps(IDB_NORMAL_HISTORY_ALARM,IDB_NORMAL_HISTORY_ALARM_PUSH,NULL,NULL);
	m_b_history_alarm.SizeToContent();		//自适应图片大小

	m_b_history_detect.LoadBitmaps(IDB_NORMAL_HISTORY_RECOGNIZE,IDB_NORMAL_HISTORY_RECOGNIZE_PUSH,NULL,NULL);
	m_b_history_detect.SizeToContent();		//自适应图片大小
	
	m_b_history_video.LoadBitmaps(IDB_NORMAL_HISTORY_VIDEO,IDB_NORMAL_HISTORY_VIDEO_PUSH,NULL,NULL);
	m_b_history_video.SizeToContent();		//自适应图片大小

	/*********全部开启关闭项*********/

	m_b_alarm_closeall.LoadBitmaps(IDB_NORMAL_ALARM_CLOSEALL,IDB_NORMAL_ALARM_CLOSEALL_PUSH,NULL,NULL);
	m_b_alarm_closeall.SizeToContent();		//自适应图片大小

	m_b_alarm_openall.LoadBitmaps(IDB_NORMAL_ALARM_OPENALL,IDB_NORMAL_ALARM_OPENALL_PUSH,NULL,NULL);
	m_b_alarm_openall.SizeToContent();		//自适应图片大小

	m_b_preview_closeall.LoadBitmaps(IDB_NORMAL_PREVIEW_CLOSEALL,IDB_NORMAL_PREVIEW_CLOSEALL_PUSH,NULL,NULL);
	m_b_preview_closeall.SizeToContent();		//自适应图片大小

	m_b_preview_openall.LoadBitmaps(IDB_NORMAL_PREVIEW_OPENALL,IDB_NORMAL_PREVIEW_OPENALL_PUSH,NULL,NULL);
	m_b_preview_openall.SizeToContent();		//自适应图片大小

	m_b_detect_closeall.LoadBitmaps(IDB_NORMAL_RECOGNIZE_CLOSEALL,IDB_NORMAL_RECOGNIZE_CLOSEALL_PUSH,NULL,NULL);
	m_b_detect_closeall.SizeToContent();		//自适应图片大小

	m_b_detect_openall.LoadBitmaps(IDB_NORMAL_RECOGNIZE_OPENALL,IDB_NORMAL_RECOGNIZE_OPENALL_PUSH,NULL,NULL);
	m_b_detect_openall.SizeToContent();		//自适应图片大小
	
	m_b_record_closeall.LoadBitmaps(IDB_NORMAL_RECORD_CLOSEALL,IDB_NORMAL_RECORD_CLOSEALL_PUSH,NULL,NULL);
	m_b_record_closeall.SizeToContent();		//自适应图片大小

	m_b_record_openall.LoadBitmaps(IDB_NORMAL_RECORD_OPENALL,IDB_NORMAL_RECORD_OPENALL_PUSH,NULL,NULL);
	m_b_record_openall.SizeToContent();		//自适应图片大小

	/*****开启 关闭******/

	m_b_record_open.LoadBitmaps(IDB_NORMAL_RECORD_OPEN,IDB_NORMAL_RECORD_OPEN_PUSH,NULL,NULL);
	m_b_record_open.SizeToContent();		//自适应图片大小

	m_b_preview_open.LoadBitmaps(IDB_NORMAL_PREVIEW_OPEN,IDB_NORMAL_PREVIEW_OPEN_PUSH,NULL,NULL);
	m_b_preview_open.SizeToContent();		//自适应图片大小

	m_b_detect_open.LoadBitmaps(IDB_NORMAL_RECOGNIZE_OPEN,IDB_NORMAL_RECOGNIZE_OPEN_PUSH,NULL,NULL);
	m_b_detect_open.SizeToContent();		//自适应图片大小

	m_b_alarm_open.LoadBitmaps(IDB_NORMAL_ALARM_OPEN,IDB_NORMAL_ALARM_OPEN_PUSH,NULL,NULL);
	m_b_alarm_open.SizeToContent();		//自适应图片大小

	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGnormal::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
//	GetParent()->GetClientRect(&rc);
//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
//	MoveWindow(rc);

	Invalidate();
}

void CDLGnormal::OnCancel()
{
	DlgMain->OnCancel();
}
void CDLGnormal::OnOK()
{

}

void CDLGnormal::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// Do not call CDialog::OnPaint() for painting messages
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     
	
	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_BACK_NORMAL);    
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

void CDLGnormal::OnCloseupComboScreen() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	switch(m_screen)
	{
	case 0:
		DlgMain->DlgShortCut.OnButtonMode1();
		break;
	case 1:
		DlgMain->DlgShortCut.OnButtonMode4();
		break;
	case 2:
		DlgMain->DlgShortCut.OnButtonMode9();
		break;
	case 3:
		DlgMain->DlgShortCut.OnButtonMode16();
		break;
	default:
		DlgMain->ShowRuntimeMessage("CDLGnormal::OnCloseupComboStream error",1);
		break;
	}
}

void CDLGnormal::OnCloseupComboStream() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CDLGnormal::OnButtonFullScreen() 
{
	// TODO: Add your control notification handler code here

}

void CDLGnormal::OnButtonOpenchannel() 
{
	if(!DlgLogin.CurrentUser.preview)
	{
		MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int curSreen = DlgMain->DlgScreen.GetCurWindId();
	StartPlay(curSreen);
}

//改变预览字和改变图片
void CDLGnormal::ChangePreviewFontPic(bool start) 
{
	//如果开启
	if(start)
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_PREVIEW)->SetWindowTextA("关闭预览");
		m_b_preview_open.LoadBitmaps(IDB_NORMAL_PREVIEW_CLOSE,IDB_NORMAL_PREVIEW_CLOSE_PUSH,NULL,NULL);
		//右侧 
		DlgMain->DlgShortCut.PreviewEnable=TRUE;
		DlgMain->DlgShortCut.PreviewBMP();
	}
	else
	{
		//如果关闭
	//	GetDlgItem(IDC_BUTTON_OPEN_PREVIEW)->SetWindowTextA("开启预览");
		m_b_preview_open.LoadBitmaps(IDB_NORMAL_PREVIEW_OPEN,IDB_NORMAL_PREVIEW_OPEN_PUSH,NULL,NULL);
		//右侧 
		DlgMain->DlgShortCut.PreviewEnable=FALSE;
		DlgMain->DlgShortCut.PreviewBMP();
	}
	m_b_preview_open.Invalidate();
}

void CDLGnormal::OnButtonOpenPreview() 
{
	// TODO: Add your control notification handler code here
	if(!DlgLogin.CurrentUser.preview)
	{
		MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	int curSreen = DlgMain->DlgScreen.GetCurWindId();
	if(DlgMain->DlgScreen.GetCurWindPlayState(curSreen))		//正在播放
	{
		StopPlay(curSreen);
	}
	else														//停止播放
	{
		StartPlay(curSreen);
	}
}

void CDLGnormal::OnButtonOpenPreviewAll() 
{
	if(!DlgLogin.CurrentUser.preview)
	{
		MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(!(DlgMain->DlgScreen.GetCurWindPlayState(i)))		//正在播放
		{
			StartPlay(i);
		}
	}
}

void CDLGnormal::OnButtonClosePreviewAll() 
{
	if(!DlgLogin.CurrentUser.preview)
	{
		MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(DlgMain->DlgScreen.GetCurWindPlayState(i))		//正在播放
		{
			StopPlay(i);
		}
	}
}
//改变识别字和改变图片
void CDLGnormal::ChangeDetectFontPic(bool start) 
{
	if(start)
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_DETECT)->SetWindowTextA("关闭识别");
		m_b_detect_open.LoadBitmaps(IDB_NORMAL_RECOGNIZE_CLOSE,IDB_NORMAL_RECOGNIZE_CLOSE_PUSH,NULL,NULL);

		DlgMain->DlgShortCut.CarEnable=TRUE;
		DlgMain->DlgShortCut.CarBMP();
	}
	else
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_DETECT)->SetWindowTextA("开启识别");
		m_b_detect_open.LoadBitmaps(IDB_NORMAL_RECOGNIZE_OPEN,IDB_NORMAL_RECOGNIZE_OPEN_PUSH,NULL,NULL);
	
		DlgMain->DlgShortCut.CarEnable=FALSE;
		DlgMain->DlgShortCut.CarBMP();
	}

	m_b_detect_open.Invalidate();
}
void CDLGnormal::OnButtonOpenDetect() 
{
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	int curSreen = DlgMain->DlgScreen.GetCurWindId();
	bool bEnable = DlgMain->DlgScreen.GetDetectState(curSreen);
	if(bEnable)
	{
		CloseDetect(curSreen);
		//关闭识别也会关闭报警
		CloseAlarm(curSreen);
	}
	else
	{
		OpenDetect(curSreen);
	}
}

void CDLGnormal::OnButtonOpenDetectAll() 
{	
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		OpenDetect(i);
	}
}

void CDLGnormal::OnButtonCloseDetectAll() 
{ 
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		CloseDetect(i);
		//关闭识别也会关闭报警
		CloseAlarm(i);
	}
}

//改变录制字和改变图片
//
void CDLGnormal::OnButtonOpenRecord() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if(!DlgLogin.CurrentUser.record)
	{
		MessageBox("无 录像 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	int curSreen = DlgMain->DlgScreen.GetCurWindId();
	bool bRecord = DlgMain->DlgScreen.GetCurWindRecordState(curSreen);
	if(bRecord)
	{
		CloseRecord(curSreen);
	}
	else
	{
		OpenRecord(curSreen);
	}	
}

void CDLGnormal::OnButtonOpenRecordAll() 
{
	// TODO: Add your control notification handler code here
	if(!DlgLogin.CurrentUser.record)
	{
		MessageBox("无 录像 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		OpenRecord(i);
	}
}

void CDLGnormal::OnButtonCloseRecordAll() 
{
	if(!DlgLogin.CurrentUser.record)
	{
		MessageBox("无 录像 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		CloseRecord(i);
	}
}
//改变报警字和改变图片
void CDLGnormal::ChangeAlarmFontPic(bool start) 
{
	if(start)
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_ALARM)->SetWindowTextA("关闭报警");
		m_b_alarm_open.LoadBitmaps(IDB_NORMAL_ALARM_CLOSE,IDB_NORMAL_ALARM_CLOSE_PUSH,NULL,NULL);

		DlgMain->DlgShortCut.AlarmEnable=TRUE;
		DlgMain->DlgShortCut.AlarmBMP();
	}
	else
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_ALARM)->SetWindowTextA("开启报警");
		m_b_alarm_open.LoadBitmaps(IDB_NORMAL_ALARM_OPEN,IDB_NORMAL_ALARM_OPEN_PUSH,NULL,NULL);
	
		DlgMain->DlgShortCut.AlarmEnable=FALSE;
		DlgMain->DlgShortCut.AlarmBMP();
	}
	m_b_alarm_open.Invalidate();
}
//改变报警字和改变图片
void CDLGnormal::ChangeRecordFontPic(bool start) 
{
	if(start)
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_RECORD)->SetWindowTextA("关闭录像");
		m_b_record_open.LoadBitmaps(IDB_NORMAL_RECORD_CLOSE,IDB_NORMAL_RECORD_CLOSE_PUSH,NULL,NULL);

		DlgMain->DlgShortCut.RecordEnable=TRUE;
		DlgMain->DlgShortCut.RecordBMP();
	}
	else
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_RECORD)->SetWindowTextA("开启录像");
		m_b_record_open.LoadBitmaps(IDB_NORMAL_RECORD_OPEN,IDB_NORMAL_RECORD_OPEN_PUSH,NULL,NULL);
	
		DlgMain->DlgShortCut.RecordEnable=FALSE;
		DlgMain->DlgShortCut.RecordBMP();
	}
	m_b_record_open.Invalidate();
}

void CDLGnormal::OnButtonOpenAlarm() 
{
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	int curSreen = DlgMain->DlgScreen.GetCurWindId();
	bool bEnable = DlgMain->DlgScreen.GetAlarmState(curSreen);
	if(bEnable)
	{
		CloseAlarm(curSreen);
	}
	else
	{
		//同时开启识别
		OpenDetect(curSreen);
		OpenAlarm(curSreen);
	}	
}

void CDLGnormal::OnButtonOpenAlarmAll() 
{
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		//同时开启识别
		OpenDetect(i);
		OpenAlarm(i);
	}
}

void CDLGnormal::OnButtonCloseAlarmAll() 
{
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		CloseAlarm(i);
	}
}

void CDLGnormal::OnButtonCapbmp() 
{
	// TODO: Add your control notification handler code here
	if(!DlgLogin.CurrentUser.capbmp)
	{
		MessageBox("无 抓图 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	int curSreen = DlgMain->DlgScreen.GetCurWindId();
	Capture(curSreen);
}

void CDLGnormal::OnButtonSetCar() 
{
	// TODO: Add your control notification handler code here
	if(!DlgLogin.CurrentUser.detectset)
	{
		MessageBox("无 识别设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	DlgMain->DlgScreen.CarDetectSet();
}

void CDLGnormal::OnButtonSetBlack() 
{
	// TODO: Add your control notification handler code here
	DlgMain->OnMenuitemSetBlack();
}

void CDLGnormal::OnButtonHistoryVideo() 
{
	// TODO: Add your control notification handler code here
	DlgMain->OnMenuitemHistoryVideo();
}

void CDLGnormal::OnButtonHistoryDetect() 
{
	// TODO: Add your control notification handler code here
	DlgMain->OnMenuitemHistoryCar();
}

void CDLGnormal::OnButtonHistoryAlarm() 
{
	// TODO: Add your control notification handler code here
	DlgMain->OnMenuitemHistoryAlarm();
}

void CDLGnormal::UpdateNormalWnd(void)
{
	int curSreen = DlgMain->DlgScreen.GetCurWindId();
	
	ChangePreviewFontPic(DlgMain->DlgScreen.GetCurWindPlayState(curSreen));

	ChangeDetectFontPic(DlgMain->DlgScreen.GetDetectState(curSreen));

	ChangeAlarmFontPic(DlgMain->DlgScreen.GetAlarmState(curSreen));

	ChangeRecordFontPic(DlgMain->DlgScreen.GetRecordState(curSreen));

	DlgMain->DlgNormal.m_stream = DlgMain->DlgScreen.GetCurWindSubType(curSreen);
	DlgMain->DlgNormal.UpdateData(FALSE);
}
///////////////////////////////////////////////
void CDLGnormal::StartPlay(int screenNo)
{
	struct DEVICE_INFO CameraInfo;
	DlgMain->DlgScreen.GetCurWindCamInfo(screenNo,CameraInfo);
	if(!(CameraInfo.ip.IsEmpty()))
	{
		bool ret = false;
		ret = DlgMain->DlgScreen.StartPlay(
							CameraInfo.camID,
							CameraInfo.area.GetBuffer(0),
							CameraInfo.name.GetBuffer(0),
							CameraInfo.ip.GetBuffer(0),
							CameraInfo.port,
							CameraInfo.user.GetBuffer(0),
							CameraInfo.psw.GetBuffer(0),
							screenNo,
							m_stream,
							CameraInfo.venderID);
		if(ret)
		{
			if(screenNo==DlgMain->DlgScreen.GetCurWindId())
				ChangePreviewFontPic(true);
		}
	}
}

void CDLGnormal::StopPlay(int screenNo)
{
	DlgMain->DlgScreen.StopPlay(screenNo);
	if(screenNo==DlgMain->DlgScreen.GetCurWindId())
	{
		ChangePreviewFontPic(false);
		ChangeDetectFontPic(false);
		ChangeAlarmFontPic(false);
		ChangeRecordFontPic(false);
	}
}

bool CDLGnormal::OpenDetect(int screenNo)
{
	bool isplay = DlgMain->DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		DlgMain->DlgScreen.EnableDetect(screenNo,true);
		if(screenNo==DlgMain->DlgScreen.GetCurWindId())
			ChangeDetectFontPic(true);
		return true;
	}
	return false;
}

void CDLGnormal::CloseDetect(int screenNo)
{
	DlgMain->DlgScreen.EnableDetect(screenNo,false);

#if OPEN_CARDETECT_CODE

	DlgMain->DlgScreen.CarDetect[screenNo].Stop();

#endif

	if(screenNo==DlgMain->DlgScreen.GetCurWindId())
		ChangeDetectFontPic(false);
}

bool CDLGnormal::OpenAlarm(int screenNo)
{
	bool isplay = DlgMain->DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		DlgMain->DlgScreen.EnableAlarm(screenNo,true);
		if(screenNo==DlgMain->DlgScreen.GetCurWindId())
			ChangeAlarmFontPic(true);
		return true;
	}
	return false;
}

void CDLGnormal::CloseAlarm(int screenNo)
{
	DlgMain->DlgScreen.EnableAlarm(screenNo,false);
	if(screenNo==DlgMain->DlgScreen.GetCurWindId())
		ChangeAlarmFontPic(false);
}

bool CDLGnormal::OpenRecord(int screenNo)
{
	bool isplay = DlgMain->DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		CString pathstr = "";
		CTime nowtime=CTime::GetTickCount();
		CString sip = DlgMain->DlgScreen.m_videoInfo[screenNo].ip;
		if(DlgMain->DlgScreen.m_videoInfo[screenNo].venderID == VENDER_TYPE_DAHUA)
		{
			pathstr.Format(_T("%s\\%s %04d-%02d-%02d %02d-%02d-%02d %d.dav"),
				DlgSetSystem.m_path_record.GetBuffer(0),
				sip.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				nowtime.GetHour(),
				nowtime.GetMinute(),
				nowtime.GetSecond(),
				GetTickCount());
		}
		else
		{
			pathstr.Format(_T("%s\\%s %04d-%02d-%02d %02d-%02d-%02d %d.mp4"),
				DlgSetSystem.m_path_record.GetBuffer(0),
				sip.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				nowtime.GetHour(),
				nowtime.GetMinute(),
				nowtime.GetSecond(),
				GetTickCount());
		}

		int iRet = DlgMain->DlgScreen.StartRecord(screenNo,pathstr.GetBuffer(0));
		if(iRet == 0)
		{
			DlgMain->DlgScreen.m_videoInfo[screenNo].recordPath = pathstr;
			DlgMain->DlgScreen.m_videoInfo[screenNo].startTime = nowtime;

			if(screenNo==DlgMain->DlgScreen.GetCurWindId())
				ChangeRecordFontPic(true);

			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void CDLGnormal::CloseRecord(int screenNo)
{
	DlgMain->DlgScreen.StopRecord(screenNo);

	if(screenNo==DlgMain->DlgScreen.GetCurWindId())
		ChangeRecordFontPic(false);
}

void CDLGnormal::Capture(int screenNo)
{
	bool isplay = DlgMain->DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		CString pathstr = "";
		CTime nowtime=CTime::GetTickCount();
		CString sip = DlgMain->DlgScreen.m_videoInfo[screenNo].ip;

		pathstr.Format(_T("%s\\%s %04d-%02d-%02d %02d-%02d-%02d %d.bmp"),
						DlgSetSystem.m_path_capbmp.GetBuffer(0),
						sip.GetBuffer(0),
						nowtime.GetYear(),
						nowtime.GetMonth(),
						nowtime.GetDay(),
						nowtime.GetHour(),
						nowtime.GetMinute(),
						nowtime.GetSecond(),
						GetTickCount());

		DlgMain->DlgScreen.Capture(pathstr.GetBuffer(0));
		ShellExecute(this->m_hWnd,NULL,pathstr.GetBuffer(0),NULL,NULL,SW_NORMAL);
	}

}
