// DLGscreen.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGscreen.h"

////////////////////////////////
#include "IO.h"
extern IO OracleIO;
//////////////////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;
//////////////////////////////////
#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RECORD_TIMER 1

/////////////////////////////////////////////////////////////////////////////
// CDLGscreen dialog


CDLGscreen::CDLGscreen(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGscreen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGscreen)
		// NOTE: the ClassWizard will add member initialization here
	m_curScreen = 0;
	m_recordtimer = 0;
	//}}AFX_DATA_INIT
	/*
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		CarSet[i].RedRect=1;
		CarSet[i].Deskew=1;
		CarSet[i].JPGquality=75;
		CarSet[i].MinWidth=60;
		CarSet[i].MaxWidth=180;

		CarSet[i].RangeRate.x0=0;
		CarSet[i].RangeRate.x1=100;
		CarSet[i].RangeRate.y0=10;
		CarSet[i].RangeRate.y1=90;
		memcpy(CarSet[i].DefaultChar,"闽",CAR_STR_MAX);
		for(int j=0;j<CAR_MASK_MAX;j++)
			CarSet[i].Mask[j]=1;
		CarSet[i].Mask[26]=0;
		CarSet[i].Reliability=50;
		CarSet[i].CarColor=1;
	}
	*/
}


void CDLGscreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGscreen)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGscreen, CDialog)
	//{{AFX_MSG_MAP(CDLGscreen)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGscreen message handlers
BOOL CDLGscreen::OnInitDialog()
{
	CDialog::OnInitDialog();

	//在这初始化
	//播放窗口
	m_screenPannel.Create(
		NULL,
		NULL,
		WS_CHILD|WS_VISIBLE, 
		CRect(0,0,0,0), 
		this, 
		1981);
	//屏蔽
	m_screenPannel.ShowWindow(SW_SHOW);

	m_screenPannel.SetShowPlayWin(SCREEN_16MODE, m_curScreen);	
/*******************lynn*****************/
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_videoInfo[i].subtype = 0;		//主码流
		m_videoInfo[i].isplay = false;
		m_videoInfo[i].isRecord = false;
		m_videoInfo[i].area = "";
		m_videoInfo[i].name = "";
		m_videoInfo[i].user = "";
		m_videoInfo[i].psw = "";
		m_videoInfo[i].recordPath = "";
		m_videoInfo[i].enableDetect = false;
		m_videoInfo[i].enableAlarm = false;
		m_videoInfo[i].playHandle = -1;
	}
	//定时1s
	m_recordtimer = SetTimer(RECORD_TIMER,1000,NULL);

/*******************lynn*****************/
	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGscreen::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//需要变化在这添加
	m_screenPannel.MoveWindow(rc);

	Invalidate();
}

void CDLGscreen::OnCancel()
{
	DlgMain->OnCancel();
}
void CDLGscreen::OnOK()
{

}


//多画面切换
void CDLGscreen::SwitchMultiWnd(int nSplit)
{
	if (SCREEN_1MODE == nSplit)
	{
		m_screenPannel.SetMultiScreen(FALSE);
		return ;
	}
	else
	{
		m_screenPannel.SetMultiScreen(TRUE);
		m_screenPannel.SetShowPlayWin(nSplit, m_curScreen);
	}
}

//设置选择的窗口
void CDLGscreen::SetCurWindId(int nCuWinID)
{
	m_curScreen = nCuWinID;
}

//获取当前选择的窗口
int CDLGscreen::GetCurWindId()
{
	return m_curScreen;
}
//根据播放句柄获取窗口ID
int CDLGscreen::GetHandleWindID(int RealHandle)
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_videoInfo[i].playHandle == RealHandle)
		{
			return i;
		}
	}
	return -1;
}
//获取当前窗口的摄像机播放状态
bool CDLGscreen::GetCurWindPlayState(int nCuWinID)
{
	return m_videoInfo[nCuWinID].isplay;
}

//获取当前窗口的摄像机录像状态
bool CDLGscreen::GetCurWindRecordState(int nCuWinID)
{
	return m_videoInfo[nCuWinID].isRecord;
}

//获取当前窗口的播放句柄
long CDLGscreen::GetCurWindPlayHandle(int nCuWinID)
{
	return m_videoInfo[nCuWinID].playHandle;
}

//获取当前窗口的摄像机的码流类型
int CDLGscreen::GetCurWindSubType(int nCuWinID)
{
	return m_videoInfo[nCuWinID].subtype;
}

//获取当前选择窗口的摄像机的详细信息
void CDLGscreen::GetCurWindCamInfo(int nCuWinID,struct DEVICE_INFO &Info)
{
	Info.area = m_videoInfo[nCuWinID].area;
	Info.camID = m_videoInfo[nCuWinID].camID;
	Info.enableDetect = m_videoInfo[nCuWinID].enableDetect;
	Info.ip = m_videoInfo[nCuWinID].ip;
	Info.isplay = m_videoInfo[nCuWinID].isplay;
	Info.name = m_videoInfo[nCuWinID].name;
	Info.playHandle = m_videoInfo[nCuWinID].playHandle;
	Info.port = m_videoInfo[nCuWinID].port;
	Info.psw = m_videoInfo[nCuWinID].psw;
	Info.subtype = m_videoInfo[nCuWinID].subtype;
	Info.user= m_videoInfo[nCuWinID].user;
	Info.venderID = m_videoInfo[nCuWinID].venderID;
}

//开启/关闭车牌识别
void CDLGscreen::EnableDetect(int nCuWinID,bool bEnable)
{
	m_videoInfo[nCuWinID].enableDetect = bEnable;
}

//开启/关闭报警
void CDLGscreen::EnableAlarm(int nCuWinID,bool bEnable)
{
	m_videoInfo[nCuWinID].enableAlarm = bEnable;
}
//开启/关闭录制
void CDLGscreen::EnableRecord(int nCuWinID,bool bEnable)
{
	m_videoInfo[nCuWinID].isRecord = bEnable;
}
//获取识别状态
bool CDLGscreen::GetDetectState(int nCuWinID)
{
	return m_videoInfo[nCuWinID].enableDetect;
}

//获取识别状态
bool CDLGscreen::GetAlarmState(int nCuWinID)
{
	return m_videoInfo[nCuWinID].enableAlarm;
}

//获取录像状态
bool CDLGscreen::GetRecordState(int nCuWinID)
{
	return m_videoInfo[nCuWinID].isRecord;
}

//车牌识别设置
void CDLGscreen::CarDetectSet(void)
{

#if ALLTAB_DETECT_CAR_MODE
	CDLGSetCar DlgSetCar;
#else
	CDLGSetElecar DlgSetCar;
#endif

#if OPEN_CARDETECT_CODE 

	DlgSetCar.ReadOldConfig(CarDetect[m_curScreen].CarSet);
	if(DlgSetCar.DoModal() == IDOK)
	{
		//设置车牌识别
		memcpy(&CarDetect[m_curScreen].CarSet,&DlgSetCar.CarSet,sizeof(CarSetStruct));
		CarDetect[m_curScreen].SetAll();
	}
#endif

}

//开始播放
bool CDLGscreen::StartPlay(int id,char *area,char *name,char *ip,int port,
						   char *user,char *psw,int screenNo,int subtype,int venderID)
{
	CWnd* pWnd = m_screenPannel.GetPage(screenNo);
	if (!pWnd)
	{
		return false;
	}

	if(m_videoInfo[screenNo].isplay == true)
	{
		StopPlay(screenNo);
	}

	bool ret = m_video.StartPlay(venderID,screenNo,name, ip, port, user, psw, pWnd->m_hWnd, subtype);
	if(ret)
	{
		m_videoInfo[screenNo].subtype = subtype;		//主码流
		m_videoInfo[screenNo].isplay = true;
		m_videoInfo[screenNo].area = area;
		m_videoInfo[screenNo].name = name;
		m_videoInfo[screenNo].ip = ip;
		m_videoInfo[screenNo].user = user;
		m_videoInfo[screenNo].psw = psw;
		m_videoInfo[screenNo].port = port;
		m_videoInfo[screenNo].camID = id;
		m_videoInfo[screenNo].playHandle = m_video.m_RealHandle[screenNo];
		m_videoInfo[screenNo].venderID = venderID;
	}
	return ret;

}

//停止播放
void CDLGscreen::StopPlay(int screenNo)
{
	if(m_videoInfo[screenNo].isRecord == true)
	{
		StopRecord(screenNo);
	}
	
	if(m_videoInfo[screenNo].isplay == true)
	{
		m_videoInfo[screenNo].isplay = false;
		m_video.StopPlay(m_videoInfo[screenNo].venderID,screenNo);
	}
	
	m_videoInfo[screenNo].enableAlarm = false;
	//停止识别
	if(m_videoInfo[screenNo].enableDetect == true)
	{
		m_videoInfo[screenNo].enableDetect = false;
#if OPEN_CARDETECT_CODE 

		CarDetect[screenNo].Stop();
#endif
	}

	CWnd* pWnd = m_screenPannel.GetPage(screenNo);
	if (pWnd)
	{
		pWnd->PostMessage(VIDEO_REPAINT);
	}
}
//云台控制
void CDLGscreen::PtzControl(int type, BOOL dwStop, int param)
{
	m_video.PtzControl(m_videoInfo[m_curScreen].venderID,m_curScreen,type,dwStop,param);
}

//开始录像
int CDLGscreen::StartRecord(int screenNo,char *filename)
{
	int ret = m_video.StartRecord(m_videoInfo[screenNo].venderID,screenNo,filename);
	if(ret == 0)
	{
		m_videoInfo[screenNo].isRecord = true;
	}
	return ret;
}

//停止录像
void CDLGscreen::StopRecord(int screenNo)
{
	if(m_videoInfo[screenNo].isRecord == false)
	{
		return;
	}

	char stime[32];
	char etime[32];
	sprintf(stime,"%04d-%02d-%02d %02d:%02d:%02d",
					m_videoInfo[screenNo].startTime.GetYear(),
					m_videoInfo[screenNo].startTime.GetMonth(),
					m_videoInfo[screenNo].startTime.GetDay(),
					m_videoInfo[screenNo].startTime.GetHour(),
					m_videoInfo[screenNo].startTime.GetMinute(),
					m_videoInfo[screenNo].startTime.GetSecond());

	CTime nowtime=CTime::GetTickCount();

	sprintf(etime,"%04d-%02d-%02d %02d:%02d:%02d",
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					nowtime.GetHour(),
					nowtime.GetMinute(),
					nowtime.GetSecond());

	m_video.StopRecord(m_videoInfo[screenNo].venderID,screenNo);
	m_videoInfo[screenNo].isRecord = false;

	FILE *fp;
	unsigned long int size=0;

	fp= fopen(m_videoInfo[screenNo].recordPath.GetBuffer(0),"rb");
	if(fp)
	{
		fseek(fp,0,SEEK_END);
		size = ftell(fp);
		fclose(fp);
	}

	OracleIO.VIDEO_AddNewVideo(m_videoInfo[screenNo].name.GetBuffer(0),
								m_videoInfo[screenNo].ip.GetBuffer(0),
								m_videoInfo[screenNo].venderID,
								"mp4",
								size,
								stime,
								etime,
								m_videoInfo[screenNo].recordPath.GetBuffer(0));
}


//抓拍图像
void CDLGscreen::Capture(char *filename)
{
	m_video.Capture(m_videoInfo[m_curScreen].venderID,m_videoInfo[m_curScreen].playHandle,filename);
}

//删除设备
void CDLGscreen::DeleteDevice(CString sip)
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_videoInfo[i].ip == sip)
		{
			StopPlay(i);
			m_videoInfo[i].subtype = 0;		//主码流
			m_videoInfo[i].isplay = false;
			m_videoInfo[i].area = "";
			m_videoInfo[i].name = "";
			m_videoInfo[i].user = "";
			m_videoInfo[i].psw = "";
			m_videoInfo[i].enableDetect = false;
			m_videoInfo[i].enableAlarm = false;
			m_videoInfo[i].playHandle = 0;
		}
	}
}

void CDLGscreen::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     
	
	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_SCREEN_BACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	
	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();
	
	CDialog::OnPaint();
	// Do not call CDialog::OnPaint() for painting messages
}

void CDLGscreen::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);

	if(nIDEvent == RECORD_TIMER)
	{
		RecordTimerEvent();
	}
}

void CDLGscreen::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	if(m_recordtimer) 
		KillTimer(m_recordtimer); 
	m_recordtimer = 0;


}

//录像定时事件
void CDLGscreen::RecordTimerEvent()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_videoInfo[i].isRecord == true)
		{
			CTime nowtime=CTime::GetTickCount();
			CTimeSpan ts = nowtime - m_videoInfo[i].startTime;
			int recMinutes = ts.GetMinutes();
			if(recMinutes >= DlgSetSystem.m_record_cuttime)
			{
				StopRecord(i);
				DlgMain->DlgNormal.OpenRecord(i);
			}
		}
	}
	
}