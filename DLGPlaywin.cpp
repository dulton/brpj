// DLGPlaywin.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGPlaywin.h"

#include "Yaanlib/launet.h"
#include "Yaanlib/megaplay.h"

#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;

#include "DLGSettings.h"
extern CDLGSettings DlgSettings;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGPlaywin dialog


CDLGPlaywin::CDLGPlaywin(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGPlaywin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGPlaywin)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
	m_curScreen = 0;
}


void CDLGPlaywin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGPlaywin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGPlaywin, CDialog)
	//{{AFX_MSG_MAP(CDLGPlaywin)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGPlaywin message handlers
BOOL CDLGPlaywin::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();
	
	LC_PLAYM4_Initial((long)this->m_hWnd);

	m_video.SDKinit();

	m_screenPannel.Create(
		NULL,
		NULL,
		WS_CHILD|WS_VISIBLE, 
		CRect(0,0,0,0), 
		this, 
		1981);
	//屏蔽
	m_screenPannel.ShowWindow(SW_SHOW);

	m_screenPannel.SetShowPlayWin(SCREEN_4MODE, m_curScreen);	

	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		StartPlay(i,\
				  pCMainDlg->DlgNewDevice.device[i].name,\
				  pCMainDlg->DlgNewDevice.device[i].ip,\
				  pCMainDlg->DlgNewDevice.device[i].port,\
				  pCMainDlg->DlgNewDevice.device[i].user,\
				  pCMainDlg->DlgNewDevice.device[i].psw);
	}

	AutoSize();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGPlaywin::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);

	//以下给视频分配窗口
	m_screenPannel.MoveWindow(rc);


	Invalidate();
}

void CDLGPlaywin::OnCancel()
{
}

void CDLGPlaywin::OnOK()
{

}
//设置选择的窗口
void CDLGPlaywin::SetCurWindId(int nCuWinID)
{
	m_curScreen = nCuWinID;
}

//获取当前选择的窗口
int CDLGPlaywin::GetCurWindId()
{
	return m_curScreen;
}


//获取当前选择的窗口的播放状态
BOOL CDLGPlaywin::GetCurWndPlayState()
{
	return m_video.m_bplay[m_curScreen];
}

//获取当前选择的窗口的录像状态
BOOL CDLGPlaywin::GetCurWndRecordState()
{
	return m_video.m_brecord[m_curScreen];
}

//开始播放
bool CDLGPlaywin::StartPlay(int screenNo,char *name,char *ip,WORD port,char *user,char *psw)
{
	CWnd* pWnd = m_screenPannel.GetPage(screenNo);
	if (!pWnd)
	{
		return false;
	}
	if(strlen(ip))
	{
		bool ret = m_video.StartPlay(screenNo,name,ip,port,user,psw,pWnd->m_hWnd);
		return ret;
	}
	else
	{
		m_video.StopPlay(screenNo);
		CWnd* pWnd = m_screenPannel.GetPage(screenNo);
		if (pWnd)
		{
			pWnd->PostMessage(VIDEO_REPAINT);
		}
	}
	return false;
}

void CDLGPlaywin::StopPlay()
{
	if(GetCurWndRecordState())
	{
		StopRecord();
	}

	m_video.StopPlay(m_curScreen);

	CWnd* pWnd = m_screenPannel.GetPage(m_curScreen);
	if (pWnd)
	{
		pWnd->PostMessage(VIDEO_REPAINT);
	}
}

//抓拍图像
void CDLGPlaywin::CapturePic()
{
	if(pCMainDlg->DlgProductInfo.data[m_curScreen].nid > 0)
	{
		char sRecFileName[260];
		CTime nowtime=CTime::GetTickCount();
		char path[260];

		switch(DlgSettings.m_comboRecord)
		{
			case HMNUM_FACTORYITEM_DATETIME:
				sprintf(path,"%s\\%s",DlgSettings.m_shotframe.GetBuffer(0),pCMainDlg->DlgProductInfo.temp.HmNum);
				CreateDirectory(path,NULL);
				sprintf(sRecFileName, "%s\\%s\\%s %04d-%02d-%02d %02d-%02d-%02d Screen%d.bmp", \
										DlgSettings.m_shotframe.GetBuffer(0), \
										pCMainDlg->DlgProductInfo.data[m_curScreen].HmNum, \
										pCMainDlg->DlgProductInfo.data[m_curScreen].FactoryItem, \
										nowtime.GetYear(), \
										nowtime.GetMonth(), \
										nowtime.GetDay(), \
										nowtime.GetHour(), \
										nowtime.GetMinute(), \
										nowtime.GetSecond(), \
										m_curScreen);
				break;
			case FACTORYITEM_HMNUM_DATETIME:
				sprintf(path,"%s\\%s",DlgSettings.m_shotframe.GetBuffer(0),pCMainDlg->DlgProductInfo.temp.FactoryItem);
				CreateDirectory(path,NULL);
				sprintf(sRecFileName, "%s\\%s\\%s %04d-%02d-%02d %02d-%02d-%02d Screen%d.bmp", \
										DlgSettings.m_shotframe.GetBuffer(0), \
										pCMainDlg->DlgProductInfo.data[m_curScreen].FactoryItem, \
										pCMainDlg->DlgProductInfo.data[m_curScreen].HmNum, \
										nowtime.GetYear(), \
										nowtime.GetMonth(), \
										nowtime.GetDay(), \
										nowtime.GetHour(), \
										nowtime.GetMinute(), \
										nowtime.GetSecond(), \
										m_curScreen);
				break;
		}
		m_video.CapturePic(m_curScreen,sRecFileName);
	}
}

//开始录像
bool CDLGPlaywin::StartRecord()
{
	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		if(pCMainDlg->DlgProductInfo.temp.nid > 0)
		{
			char sRecFileName[260];
			char tempFileName[260];
			char path[260];

			CTime nowtime=CTime::GetTickCount();

			switch(DlgSettings.m_comboRecord)
			{
				case HMNUM_FACTORYITEM_DATETIME:
					sprintf(path,"%s\\%s",DlgSettings.m_record.GetBuffer(0),pCMainDlg->DlgProductInfo.temp.HmNum);
					CreateDirectory(path,NULL);
					sprintf(tempFileName, "%s\\%s\\%s %04d-%02d-%02d %02d-%02d-%02d Screen%d", \
											DlgSettings.m_record.GetBuffer(0), \
											pCMainDlg->DlgProductInfo.temp.HmNum, \
											pCMainDlg->DlgProductInfo.temp.FactoryItem, \
											nowtime.GetYear(), \
											nowtime.GetMonth(), \
											nowtime.GetDay(), \
											nowtime.GetHour(), \
											nowtime.GetMinute(), \
											nowtime.GetSecond(), \
											i);
					break;
				case FACTORYITEM_HMNUM_DATETIME:
					sprintf(path,"%s\\%s",DlgSettings.m_record.GetBuffer(0),pCMainDlg->DlgProductInfo.temp.FactoryItem);
					CreateDirectory(path,NULL);
					sprintf(tempFileName, "%s\\%s\\%s %04d-%02d-%02d %02d-%02d-%02d Screen%d", \
											DlgSettings.m_record.GetBuffer(0), \
											pCMainDlg->DlgProductInfo.temp.FactoryItem, \
											pCMainDlg->DlgProductInfo.temp.HmNum, \
											nowtime.GetYear(), \
											nowtime.GetMonth(), \
											nowtime.GetDay(), \
											nowtime.GetHour(), \
											nowtime.GetMinute(), \
											nowtime.GetSecond(), \
											i);
					break;
			}
			sprintf(sRecFileName,"%s.mp4",tempFileName);
			int ret = m_video.StartRecord(i,sRecFileName);
			if(ret != 0)
			{
				pCMainDlg->DlgProductInfo.temp2data(i);

				sprintf(m_video.record[i].stime,"%4d-%02d-%02d %02d:%02d:%02d", \
												nowtime.GetYear(), \
												nowtime.GetMonth(), \
												nowtime.GetDay(), \
												nowtime.GetHour(), \
												nowtime.GetMinute(), \
												nowtime.GetSecond());
				m_video.record[i].starttime = nowtime;
				strcpy(m_video.record[i].MP4path,sRecFileName);
				strcpy(m_video.record[i].RunningNumber,pCMainDlg->DlgProductInfo.temp.RunningNumber);
				strcpy(m_video.record[i].tag,pCMainDlg->DlgProductInfo.temp.tag);
				strcpy(m_video.record[i].HmNum,pCMainDlg->DlgProductInfo.temp.HmNum);
				strcpy(m_video.record[i].Description,pCMainDlg->DlgProductInfo.temp.Description);
				sprintf(m_video.record[i].AVIpath,"%s.avi",tempFileName);
			}
		}
		else
		{
			MessageBox(Language_ConvertString("Please enter a Product code"));
			return false;
		}
	}
	return true;
}

//停止录像
void CDLGPlaywin::StopRecord()
{
	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		m_video.StopRecord(i);
		pCMainDlg->DlgProductInfo.Clean(i);
	}
//	pCMainDlg->DlgProductInfo.DisplayTemp();
}

void CDLGPlaywin::GetCurWndRecordTime(char *rtime)
{
	CTime nowtime=CTime::GetTickCount();
	CTimeSpan ts = nowtime - m_video.record[m_curScreen].starttime;
	sprintf(rtime,"%02d:%02d:%02d",ts.GetHours(),ts.GetMinutes(),ts.GetSeconds());
}

//更新窗口的产品信息
void CDLGPlaywin::UpdateWndProductInfo()
{
	if(GetCurWndRecordState() == TRUE)
	{
		pCMainDlg->DlgProductInfo.Display(m_curScreen);
	}
	else
	{
		pCMainDlg->DlgProductInfo.DisplayTemp();
	}
}