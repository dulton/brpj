// DLGnormal.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGnormal.h"

//////////////////////////////////
#include "SmartVillageDlg.h"
extern CSmartVillageDlg *DlgMain;
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
	DDX_Control(pDX, IDC_BUTTON_OPEN_CARDETECT_ALL, m_b_CarDetect_openall);
	DDX_Control(pDX, IDC_BUTTON_OPEN_CARDETECT, m_b_CarDetect_open);

	DDX_Control(pDX, IDC_BUTTON_HISTORY_VIDEO, m_b_history_video);
	DDX_Control(pDX, IDC_BUTTON_HISTORY_CARDETECT, m_b_history_CarDetect);
	DDX_Control(pDX, IDC_BUTTON_HISTORY_CARALARM, m_b_history_CarAlarm);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_RECORD_ALL, m_b_record_closeall);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_PREVIEW_ALL, m_b_preview_closeall);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_CARDETECT_ALL, m_b_CarDetect_closeall);

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
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CARDETECT, OnButtonOpenCarDetect)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CARDETECT_ALL, OnButtonOpenCarDetectAll)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_CARDETECT_ALL, OnButtonCloseCarDetectAll)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RECORD, OnButtonOpenRecord)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RECORD_ALL, OnButtonOpenRecordAll)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_RECORD_ALL, OnButtonCloseRecordAll)

	ON_BN_CLICKED(IDC_BUTTON_CAPBMP, OnButtonCapbmp)

	ON_BN_CLICKED(IDC_BUTTON_SET_BLACK, OnButtonSetBlack)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_VIDEO, OnButtonHistoryVideo)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_CARDETECT, OnButtonHistoryCarDetect)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_CARALARM, OnButtonHistoryCarAlarm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGnormal message handlers
BOOL CDLGnormal::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch(DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.screenmode)
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

	m_b_history_CarAlarm.LoadBitmaps(IDB_NORMAL_HISTORY_CARALARM,IDB_NORMAL_HISTORY_CARALARM_PUSH,NULL,NULL);
	m_b_history_CarAlarm.SizeToContent();		//自适应图片大小

	m_b_history_CarDetect.LoadBitmaps(IDB_NORMAL_HISTORY_RECOGNIZE,IDB_NORMAL_HISTORY_RECOGNIZE_PUSH,NULL,NULL);
	m_b_history_CarDetect.SizeToContent();		//自适应图片大小
	
	m_b_history_video.LoadBitmaps(IDB_NORMAL_HISTORY_VIDEO,IDB_NORMAL_HISTORY_VIDEO_PUSH,NULL,NULL);
	m_b_history_video.SizeToContent();		//自适应图片大小

	/*********全部开启关闭项*********/


	m_b_preview_closeall.LoadBitmaps(IDB_NORMAL_PREVIEW_CLOSEALL,IDB_NORMAL_PREVIEW_CLOSEALL_PUSH,NULL,NULL);
	m_b_preview_closeall.SizeToContent();		//自适应图片大小

	m_b_preview_openall.LoadBitmaps(IDB_NORMAL_PREVIEW_OPENALL,IDB_NORMAL_PREVIEW_OPENALL_PUSH,NULL,NULL);
	m_b_preview_openall.SizeToContent();		//自适应图片大小

	m_b_CarDetect_closeall.LoadBitmaps(IDB_NORMAL_RECOGNIZE_CLOSEALL,IDB_NORMAL_RECOGNIZE_CLOSEALL_PUSH,NULL,NULL);
	m_b_CarDetect_closeall.SizeToContent();		//自适应图片大小

	m_b_CarDetect_openall.LoadBitmaps(IDB_NORMAL_RECOGNIZE_OPENALL,IDB_NORMAL_RECOGNIZE_OPENALL_PUSH,NULL,NULL);
	m_b_CarDetect_openall.SizeToContent();		//自适应图片大小
	
	m_b_record_closeall.LoadBitmaps(IDB_NORMAL_RECORD_CLOSEALL,IDB_NORMAL_RECORD_CLOSEALL_PUSH,NULL,NULL);
	m_b_record_closeall.SizeToContent();		//自适应图片大小

	m_b_record_openall.LoadBitmaps(IDB_NORMAL_RECORD_OPENALL,IDB_NORMAL_RECORD_OPENALL_PUSH,NULL,NULL);
	m_b_record_openall.SizeToContent();		//自适应图片大小

	/*****开启 关闭******/

	m_b_record_open.LoadBitmaps(IDB_NORMAL_RECORD_OPEN,IDB_NORMAL_RECORD_OPEN_PUSH,NULL,NULL);
	m_b_record_open.SizeToContent();		//自适应图片大小

	m_b_preview_open.LoadBitmaps(IDB_NORMAL_PREVIEW_OPEN,IDB_NORMAL_PREVIEW_OPEN_PUSH,NULL,NULL);
	m_b_preview_open.SizeToContent();		//自适应图片大小

	m_b_CarDetect_open.LoadBitmaps(IDB_NORMAL_RECOGNIZE_OPEN,IDB_NORMAL_RECOGNIZE_OPEN_PUSH,NULL,NULL);
	m_b_CarDetect_open.SizeToContent();		//自适应图片大小



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
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode1();
		break;
	case 1:
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode4();
		break;
	case 2:
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode9();
		break;
	case 3:
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode16();
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
	int curSreen = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
	StopPlay(curSreen);
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
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.PreviewEnable=TRUE;
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.PreviewBMP();
	}
	else
	{
		//如果关闭
	//	GetDlgItem(IDC_BUTTON_OPEN_PREVIEW)->SetWindowTextA("开启预览");
		m_b_preview_open.LoadBitmaps(IDB_NORMAL_PREVIEW_OPEN,IDB_NORMAL_PREVIEW_OPEN_PUSH,NULL,NULL);
		//右侧 
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.PreviewEnable=FALSE;
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.PreviewBMP();
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

	int curSreen = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
	if(DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(curSreen))		//正在播放
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
		if(!(DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(i)))		//正在播放
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
		if(DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(i))		//正在播放
		{
			StopPlay(i);
		}
	}
}
//改变识别字和改变图片
void CDLGnormal::ChangeCarDetectFontPic(bool start) 
{
	if(start)
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_DETECT)->SetWindowTextA("关闭识别");
		m_b_CarDetect_open.LoadBitmaps(IDB_NORMAL_RECOGNIZE_CLOSE,IDB_NORMAL_RECOGNIZE_CLOSE_PUSH,NULL,NULL);

		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.CarEnable=TRUE;
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.CarBMP();
	}
	else
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_DETECT)->SetWindowTextA("开启识别");
		m_b_CarDetect_open.LoadBitmaps(IDB_NORMAL_RECOGNIZE_OPEN,IDB_NORMAL_RECOGNIZE_OPEN_PUSH,NULL,NULL);
	
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.CarEnable=FALSE;
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.CarBMP();
	}

	m_b_CarDetect_open.Invalidate();
}

//改变识别字和改变图片
void CDLGnormal::ChangeFaceDetectFontPic(bool start) 
{
	if(start)
	{
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.faceEnable=TRUE;
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.FaceBMP();
	}
	else
	{
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.faceEnable=FALSE;
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.FaceBMP();
	}
}

void CDLGnormal::OnButtonOpenCarDetect() 
{
	if(!DlgLogin.CurrentUser.Cardetect)
	{
		MessageBox("无 车牌识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	int curSreen = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
	bool bEnable = DlgMain->DlgTabVideo.DlgScreen.GetCarDetectState(curSreen);
	if(bEnable)
	{
		CloseCarDetect(curSreen);

	}
	else
	{
		OpenCarDetect(curSreen);
	}
}
void CDLGnormal::OnButtonOpenFaceDetect() 
{
	if(!DlgLogin.CurrentUser.Facedetect)
	{
		MessageBox("无 人脸识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	int curSreen = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
	bool bEnable = DlgMain->DlgTabVideo.DlgScreen.GetFaceDetectState(curSreen);
	if(bEnable)
	{
		CloseFaceDetect(curSreen);

	}
	else
	{
		OpenFaceDetect(curSreen);
	}
}


void CDLGnormal::OnButtonOpenCarDetectAll() 
{	
	if(!DlgLogin.CurrentUser.Cardetect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		OpenCarDetect(i);
	}
}

void CDLGnormal::OnButtonCloseCarDetectAll() 
{ 
	if(!DlgLogin.CurrentUser.Cardetect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
	// TODO: Add your control notification handler code here
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		CloseCarDetect(i);

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
	int curSreen = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();

	bool planRecord = DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlanRecordState(curSreen);
	//正在执行计划录制不允许手动停止
	if(planRecord == true)
	{
		MessageBox("系统正在执行 定时录制，请先关闭定时录制",MESSAGEBOX_TITLE);
		return;
	}

	bool bRecord = DlgMain->DlgTabVideo.DlgScreen.GetCurWindRecordState(curSreen);
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
void CDLGnormal::ChangeRecordFontPic(bool start) 
{
	if(start)
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_RECORD)->SetWindowTextA("关闭录像");
		m_b_record_open.LoadBitmaps(IDB_NORMAL_RECORD_CLOSE,IDB_NORMAL_RECORD_CLOSE_PUSH,NULL,NULL);

		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.RecordEnable=TRUE;

	}
	else
	{
	//	GetDlgItem(IDC_BUTTON_OPEN_RECORD)->SetWindowTextA("开启录像");
		m_b_record_open.LoadBitmaps(IDB_NORMAL_RECORD_OPEN,IDB_NORMAL_RECORD_OPEN_PUSH,NULL,NULL);
	
		DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.RecordEnable=FALSE;

	}
	m_b_record_open.Invalidate();
}



void CDLGnormal::OnButtonCapbmp() 
{
	// TODO: Add your control notification handler code here
	if(!DlgLogin.CurrentUser.capbmp)
	{
		MessageBox("无 抓图 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	int curSreen = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
	Capture(curSreen);
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

void CDLGnormal::OnButtonHistoryCarDetect() 
{
	// TODO: Add your control notification handler code here
	DlgMain->OnMenuitemHistoryCar();
}

void CDLGnormal::OnButtonHistoryCarAlarm() 
{
	// TODO: Add your control notification handler code here
	DlgMain->OnMenuitemHistoryCarAlarm();
}

void CDLGnormal::UpdateNormalWnd(void)
{
	int curSreen = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
	
	ChangePreviewFontPic(DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(curSreen));

	ChangeCarDetectFontPic(DlgMain->DlgTabVideo.DlgScreen.GetCarDetectState(curSreen));

ChangeFaceDetectFontPic(DlgMain->DlgTabVideo.DlgScreen.GetFaceDetectState(curSreen)) ;

	ChangeRecordFontPic(DlgMain->DlgTabVideo.DlgScreen.GetRecordState(curSreen));

	DlgMain->DlgTabVideo.DlgNormal.m_stream = DlgMain->DlgTabVideo.DlgScreen.GetCurWindSubType(curSreen);
	DlgMain->DlgTabVideo.DlgNormal.UpdateData(FALSE);
}
///////////////////////////////////////////////
void CDLGnormal::StartPlay(int screenNo)
{
	struct DEVICE_INFO CameraInfo={0};
	DlgMain->DlgTabVideo.DlgScreen.GetCurWindCamInfo(screenNo,CameraInfo);
	if((!CameraInfo.ip.IsEmpty()) || (!CameraInfo.Rtspurl.IsEmpty()) )
	{
		bool ret = false;
		ret = DlgMain->DlgTabVideo.DlgScreen.StartPlay(
							CameraInfo.camID,
							CameraInfo.area.GetBuffer(0),
							CameraInfo.name.GetBuffer(0),
							CameraInfo.ip.GetBuffer(0),
							CameraInfo.port,
							CameraInfo.channel,
							CameraInfo.user.GetBuffer(0),
							CameraInfo.psw.GetBuffer(0),
							screenNo,
							m_stream,
							CameraInfo.venderID,
							CameraInfo.Rtspurl.GetBuffer(0),
							CameraInfo.RTP,
							CameraInfo.DecodeTag);
		if(ret)
		{
			if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
				ChangePreviewFontPic(true);
		}
	}
}

void CDLGnormal::StopPlay(int screenNo)
{
	DlgMain->DlgTabVideo.DlgScreen.StopPlay(screenNo);
	if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
	{
		ChangePreviewFontPic(false);
		ChangeCarDetectFontPic(false);
	ChangeFaceDetectFontPic(false);
		ChangeRecordFontPic(false);
	}
}

bool CDLGnormal::OpenCarDetect(int screenNo)
{
	//zogcsmode
	bool isplay = DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		if(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableFaceDetect )
		{
			MessageBox("已经开启了人脸识别,无法再开",MESSAGEBOX_TITLE);
			return false;
		}

		DlgMain->DlgTabVideo.DlgScreen.EnableCarDetect(screenNo,true);
		if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
			ChangeCarDetectFontPic(true);
		return true;
	}
	return false;
}

void CDLGnormal::CloseCarDetect(int screenNo)
{
	DlgMain->DlgTabVideo.DlgScreen.EnableCarDetect(screenNo,false);

#if OPEN_CARDETECT_CODE

	DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].Stop();

#endif

	if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
		ChangeCarDetectFontPic(false);
}

bool CDLGnormal::OpenFaceDetect(int screenNo)
{
	//zogcsmode
	bool isplay = DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		if(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableCarDetect )
		{
			MessageBox("已经开启了车牌识别,无法再开",MESSAGEBOX_TITLE);
			return false;
		}

		DlgMain->DlgTabVideo.DlgScreen.EnableFaceDetect(screenNo,true);
		if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
			ChangeFaceDetectFontPic(true);
		return true;
	}
	return false;
}

void CDLGnormal::CloseFaceDetect(int screenNo)
{
	DlgMain->DlgTabVideo.DlgScreen.EnableFaceDetect(screenNo,false);
	Sleep(100);
#if OPEN_FACEDETECT_CODE

	DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].Stop();

#endif

	if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
		ChangeFaceDetectFontPic(false);
}



bool CDLGnormal::OpenRecord(int screenNo)
{
	bool isplay = DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		CString pathstr = "";
		CTime nowtime=CTime::GetTickCount();
		CString sip = DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].ip;
		long camid= DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].camID;
		if(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].venderID == VENDER_TYPE_DAHUA)
		{
			pathstr.Format(_T("%s\\%s %d %04d-%02d-%02d %02d-%02d-%02d %d.dav"),
				DlgSetSystem.m_path_record.GetBuffer(0),
				sip.GetBuffer(0),
				camid,
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				nowtime.GetHour(),
				nowtime.GetMinute(),
				nowtime.GetSecond(),
				GetTickCount()%50000);
		}
		else
		{
			pathstr.Format(_T("%s\\%s %d %04d-%02d-%02d %02d-%02d-%02d %d.mp4"),
				DlgSetSystem.m_path_record.GetBuffer(0),
				sip.GetBuffer(0),
				camid,
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				nowtime.GetHour(),
				nowtime.GetMinute(),
				nowtime.GetSecond(),
				GetTickCount()%50000);
		}

		int iRet = DlgMain->DlgTabVideo.DlgScreen.StartRecord(screenNo,pathstr.GetBuffer(0));
		if(iRet == 0)
		{
			DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].recordPath = pathstr;
			DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].startTime = nowtime.GetTime();

			if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
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
	DlgMain->DlgTabVideo.DlgScreen.StopRecord(screenNo);

	if(screenNo==DlgMain->DlgTabVideo.DlgScreen.GetCurWindId())
		ChangeRecordFontPic(false);
}

void CDLGnormal::Capture(int screenNo)
{
	bool isplay = DlgMain->DlgTabVideo.DlgScreen.GetCurWindPlayState(screenNo);
	if(isplay)				//正在播放
	{
		CString pathstr = "";
		CTime nowtime=CTime::GetTickCount();
		CString sip = DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].ip;

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

		DlgMain->DlgTabVideo.DlgScreen.Capture(pathstr.GetBuffer(0));
		ShellExecute(this->m_hWnd,NULL,pathstr.GetBuffer(0),NULL,NULL,SW_NORMAL);
	}

}
