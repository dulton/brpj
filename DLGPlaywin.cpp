// DLGPlaywin.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGPlaywin.h"

#include "Yaanlib/launet.h"
#include "Yaanlib/megaplay.h"

#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;

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
BOOL CDLGPlaywin::GetWindPlayState(int screenNo)
{
	return m_video.m_bplay[screenNo];
}

//开始播放
bool CDLGPlaywin::StartPlay()
{
	CWnd* pWnd = m_screenPannel.GetPage(m_curScreen);
	if (!pWnd)
	{
		return false;
	}
	//m_video.LinPlay("10.142.50.249",0,"摄像机",3000,"888888","888888",3,this->m_hWnd);
	m_video.StartPlay(m_curScreen,"摄像机","10.142.50.249",3000,"888888","888888",pWnd->m_hWnd);
	return false;
}

void CDLGPlaywin::StopPlay()
{
	m_video.StopPlay(m_curScreen);
}

//抓拍图像
void CDLGPlaywin::CapturePic(char *filename)
{
	m_video.CapturePic(m_curScreen,filename);
}

//开始录像
void CDLGPlaywin::StartRecord(LPCSTR filename)
{
	m_video.StartRecord(m_curScreen,filename);
}

//停止录像
void CDLGPlaywin::StopRecord()
{
	m_video.StopRecord(m_curScreen);
}


