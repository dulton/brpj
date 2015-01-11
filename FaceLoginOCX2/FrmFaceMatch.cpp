// FrmFaceMatch.cpp : implementation file
//

#include "stdafx.h"
#include "FaceLoginOCX.h"
#include "FrmFaceMatch.h"
#include "YUV2RGB.h"

#define FACE_MACTH_TIMER 1
#include "FaceLoginOCXCtrl.h"
extern CFaceLoginOCXCtrl *DlgFaceLoginOCXCtrl;

//********************************线程**********************************
//人脸比对线程
DWORD WINAPI MacthThread(void *p)
{
	CFrmFaceMatch * pFrmFaceMatch = (CFrmFaceMatch*)p;

	pFrmFaceMatch->m_bIsClose = false;

	while(pFrmFaceMatch->m_bThreadWork)
	{
		pFrmFaceMatch->MacthProcess();
		Sleep(2);
	}

	pFrmFaceMatch->m_bIsClose = true;
	return 0;
}

//视频显示线程
DWORD WINAPI MacthDisplayThread(void *p)
{
	CFrmFaceMatch * pFrmFaceMatch = (CFrmFaceMatch*)p;
	
	long  bitmapSize;
	char * buffer= NULL;

	pFrmFaceMatch->m_bIsClose = false;

	while(pFrmFaceMatch->m_bThreadWork)
	{
		try
		{
			QueryFrameCB(&bitmapSize,&buffer);

			if(bitmapSize > 0)
			{
				BITMAPINFO bmpInfo = {0};   
				bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   
				bmpInfo.bmiHeader.biWidth = pFrmFaceMatch->m_lVideoWidth;   
				bmpInfo.bmiHeader.biHeight = pFrmFaceMatch->m_lVideoHeight;
				bmpInfo.bmiHeader.biPlanes = 1;   
				bmpInfo.bmiHeader.biBitCount = 24;  
				try
				{

					if(pFrmFaceMatch->m_Detect.enableLive)
					{
#if LIVE_FACE_TEST	

						if(pFrmFaceMatch->matchFlag)
						{
							RGBup2downOUT((unsigned char *)buffer,pFrmFaceMatch->m_lVideoWidth, pFrmFaceMatch->m_lVideoHeight,pFrmFaceMatch->tempwidgetimage.imagedata);

							pFrmFaceMatch->tempwidgetimage.image_size=bitmapSize;
							pFrmFaceMatch->tempwidgetimage.width=pFrmFaceMatch->m_lVideoWidth;
							pFrmFaceMatch->tempwidgetimage.height=pFrmFaceMatch->m_lVideoHeight;
							pFrmFaceMatch->tempwidgetimage.widthStep=pFrmFaceMatch->m_lVideoWidth*3;

							int liveresult=0;

							long long time;
							if(pFrmFaceMatch->ns_timeflag)
							{
								LARGE_INTEGER ns_curtime;
								QueryPerformanceCounter(&ns_curtime);
								time=(long long) ((double)(ns_curtime.QuadPart - pFrmFaceMatch->ns_firsttime.QuadPart)/(double)pFrmFaceMatch->ns_Freq.QuadPart*1000);
							}
							else
								time =GetTickCount() - pFrmFaceMatch->firsttime;

							int fwld_re=FaceWidget_LiveDetector(pFrmFaceMatch->widget_handle,eye_alive,pFrmFaceMatch->tempwidgetimage,time,1.0,liveresult);

							//	CString strlog;
							//	strlog.Format(_T("FaceWidget_LiveDetector：fwld_re=%d,liveresult=%d,%d"),fwld_re,liveresult,GetTickCount()-aa);
							//	DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

							if(0==fwld_re && 1==liveresult)
							{
								pFrmFaceMatch->liveflag=1;
								//活体以后再一次判断是本人。防止照片后 非本人的活体
								pFrmFaceMatch->matchFlag=0;
							}
						}
#endif
					}


					if(!pFrmFaceMatch->b_listLock)
					{
						CString result = pFrmFaceMatch->m_Detect.RGBtoBase64((unsigned char *)buffer,
																				bitmapSize,
																				pFrmFaceMatch->m_lVideoWidth,
																				pFrmFaceMatch->m_lVideoHeight);
						if(!result.IsEmpty())
						{
							pFrmFaceMatch->m_DetectList.push(result);
						}
					}
				
					if(pFrmFaceMatch->matchFlag && pFrmFaceMatch->m_Detect.enableLive)
					{
						//显示摄像头画面及人脸框叠加
						pFrmFaceMatch->m_common->DrawCtrlImage((CStatic *)pFrmFaceMatch->GetDlgItem(IDC_STATIC_MAIN), bmpInfo, 
															buffer, bitmapSize,pFrmFaceMatch->face_Count,pFrmFaceMatch->face_Rect_List,
															pFrmFaceMatch->DrawRect,pFrmFaceMatch->DrawScale,3);
					}
					else
					{
						//显示摄像头画面及人脸框叠加
						pFrmFaceMatch->m_common->DrawCtrlImage((CStatic *)pFrmFaceMatch->GetDlgItem(IDC_STATIC_MAIN), bmpInfo, 
															buffer, bitmapSize,pFrmFaceMatch->face_Count,pFrmFaceMatch->face_Rect_List,
															pFrmFaceMatch->DrawRect,pFrmFaceMatch->DrawScale,2);
					}

				}

				catch(...)
				{
					continue;
				}
			}
			else
			{			
				Sleep(100);
			}

		}
		catch(...)
		{
			continue;
		}
	}

	pFrmFaceMatch->m_bIsClose = true;
	return 0;
}

//人脸检测线程
DWORD WINAPI MacthDetectThread(void *p)
{
	CFrmFaceMatch * pFrmFaceMatch = (CFrmFaceMatch*)p;

	pFrmFaceMatch->m_bIsClose = false;

	while(pFrmFaceMatch->m_bThreadWork)
	{
		try
		{
			if(pFrmFaceMatch->m_DetectList.size() > 0)
			{
				pFrmFaceMatch->b_listLock = true;
				CString strRequest = pFrmFaceMatch->m_DetectList.front();
				pFrmFaceMatch->m_DetectList.pop();
				pFrmFaceMatch->m_Detect.FaceDetect(strRequest,&pFrmFaceMatch->face_Count,pFrmFaceMatch->face_Rect_List,pFrmFaceMatch->Image_list);
				if(pFrmFaceMatch->face_Count>0)
				{
					if(!pFrmFaceMatch->b_macthLock)
					{
						pFrmFaceMatch->b_macthLock = true;
						for(int i=0;i<pFrmFaceMatch->face_Count;i++)
						{
							pFrmFaceMatch->m_MacthList.push(pFrmFaceMatch->Image_list[i]);
						}
						pFrmFaceMatch->b_macthLock = false;
					}
				}
				pFrmFaceMatch->b_listLock = false;
			}
		}
		catch(...)
		{
			continue;
		}
		Sleep(2);
	}

	pFrmFaceMatch->m_bIsClose = true;
	return 0;
}
//********************************线程**********************************

// CFrmFaceMatch dialog

IMPLEMENT_DYNAMIC(CFrmFaceMatch, CDialog)

CFrmFaceMatch::CFrmFaceMatch(CWnd* pParent /*=NULL*/)
	: CDialog(CFrmFaceMatch::IDD, pParent)
{
	b_listLock = false;
	b_macthLock = false;
#if LIVE_FACE_TEST
	tempwidgetimage.imagedata=(unsigned char *)calloc(3*VIDEO_WIDTH*VIDEO_HEIGHT,1);
#endif
}

CFrmFaceMatch::~CFrmFaceMatch()
{
#if LIVE_FACE_TEST
	free(tempwidgetimage.imagedata);
#endif
}

void CFrmFaceMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEV, m_cbDevice);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnclose);
}


BEGIN_MESSAGE_MAP(CFrmFaceMatch, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_START, &CFrmFaceMatch::OnBnClickedBtnStart)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CFrmFaceMatch::OnBnClickedButtonClose)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CFrmFaceMatch message handlers

BOOL CFrmFaceMatch::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitChildWindow();

	StartMacthThread();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CFrmFaceMatch::InitLive()
{
#if LIVE_FACE_TEST


	ns_timeflag=QueryPerformanceFrequency(&ns_Freq);
	if(ns_timeflag)
		 QueryPerformanceCounter(&ns_firsttime);
	else
		firsttime=GetTickCount();

	/*
	int ip[4]={0};
	int port=0;
	sscanf(m_Detect.HostInfo.GetBuffer(0),"http://%d.%d.%d.%d:%d",&ip[0],&ip[1],&ip[2],&ip[3],&port);
	char ipstr[32]="";
	sprintf(ipstr,"%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
*/

	module_handle=NULL;
	if(Face_InitModule(LIVE_FACE_MODEL_PATH,m_Detect.liveServer,m_Detect.livePort,&module_handle)!=0)
	{
		MacthLog.Format(_T("{\
						   \"ret\":\"fail\",\
						   \"user\":\"%s\",\
						   \"sysID\":\"%d\",\
						   \"content\":\"LiveDetect_InitModuleFail\"\
						   }"),DlgFaceLoginOCXCtrl->MatchUser,DlgFaceLoginOCXCtrl->MatchSysID);

		MacthResult.Format(_T("%d"),OCX_ERROR_LIVEFACE_INIT_FAIL);

		return FALSE;
	}

	widget_handle=NULL;
	widget_handle=FaceWidget_Init(module_handle);
	if(NULL==widget_handle)
	{
		MacthLog.Format(_T("{\
						   \"ret\":\"fail\",\
						   \"user\":\"%s\",\
						   \"sysID\":\"%d\",\
						   \"content\":\"LiveDetect_InitWidgetFail\"\
						   }"),DlgFaceLoginOCXCtrl->MatchUser,DlgFaceLoginOCXCtrl->MatchSysID);

		MacthResult.Format(_T("%d"),OCX_ERROR_LIVEFACE_WIDGET_FAIL);

		Face_UnInitModule();

		return FALSE;
	}

	return TRUE;
#endif
	return TRUE;
}

void CFrmFaceMatch::unInitLive()
{
#if LIVE_FACE_TEST
	if(widget_handle)
		FaceWidget_UnInit(widget_handle);

	Face_UnInitModule();
#endif
}

void CFrmFaceMatch::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CFrmFaceMatch::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if(0==MacthResult.GetLength())
	{
		MacthLog.Format(_T("{\
						   \"ret\":\"fail\",\
						   \"user\":\"%s\",\
						   \"sysID\":\"%d\",\
						   \"content\":\"User_Cancel\"\
						   }"),DlgFaceLoginOCXCtrl->MatchUser,DlgFaceLoginOCXCtrl->MatchSysID);

		MacthResult.Format(_T("%d"),OCX_ERROR_USER_CANCEL);
	}

	StopMacthThread();
	CDialog::OnCancel();
}

void CFrmFaceMatch::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CFrmFaceMatch::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == FACE_MACTH_TIMER)
	{
		
		//比对次数超过限制
		if(m_Detect.matchCount > m_Detect.matchTimes)
		{
			CString strlog;
			strlog.Format(_T("<TIPS><matchFail> - User<%s>"),DlgFaceLoginOCXCtrl->MatchUser);
			DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

			MacthResult.Format(_T("%d"),OCX_ERROR_MATCH_FAIL);
			OnCancel();
		}

		if(m_Detect.enableLive)
		{
#if LIVE_FACE_TEST

			if(matchFlag && liveflag)
#else
			if(matchFlag)
#endif
			{
				CString strlog;
				strlog.Format(_T("<TIPS><matchSuccess> - User<%s>"),DlgFaceLoginOCXCtrl->MatchUser);
				DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

				MacthResult = m_Detect.Token;//比对成功，返回令牌
				OnCancel();
			}
		}
		else

		{
			if(matchFlag)
			{
				CString strlog;
				strlog.Format(_T("<TIPS><matchSuccess> - User<%s>"),DlgFaceLoginOCXCtrl->MatchUser);
				DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

				MacthResult = m_Detect.Token;//比对成功，返回令牌
				OnCancel();
			}
		}

		if(MacthTimeOut++ > OCX_TIMEOUT_MAX)
		{
			if(MacthLog.IsEmpty())			//未检测到人脸，识别超时
			{
				MacthLog.Format(_T("{\
									\"ret\":\"fail\",\
									\"user\":\"%s\",\
									\"sysID\":\"%d\",\
									\"content\":\"Time_Out\"\
									}"),DlgFaceLoginOCXCtrl->MatchUser,DlgFaceLoginOCXCtrl->MatchSysID);

				DlgFaceLoginOCXCtrl->ShowOcxLog(MacthLog);
				MacthResult.Format(_T("%d"),OCX_ERROR_TIME_OUT);
			}
			else							//有识别，但是未成功
			{
				CString strlog;
				strlog.Format(_T("<TIPS><matchFail> - User<%s>"),DlgFaceLoginOCXCtrl->MatchUser);
				DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

				MacthResult.Format(_T("%d"),OCX_ERROR_MATCH_FAIL);
			}
			OnCancel();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CFrmFaceMatch::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	if(m_bThreadWork)
	{
		m_bThreadWork = false;
		Sleep(1000);

		if(false == m_bIsClose)
		{
			TerminateThread(m_pThreadDisplay,0);
			m_pThreadDisplay=NULL;
			TerminateThread(m_pThreadDetect,0);
			m_pThreadDetect=NULL;
			TerminateThread(m_pThreadMacth,0);
			m_pThreadMacth=NULL;
		}
		Sleep(100);
		CloseCamera();
		Invalidate();
	}

	if(m_cbDevice.GetCount()>0)
	{
		CString tmp;
		tmp.Format("%d", m_cbDevice.GetCurSel());
		m_common->SetReg("Camera", tmp);
		if(!OpenCameraCB(m_cbDevice.GetCurSel(), false , &m_lVideoWidth, &m_lVideoHeight)) //不弹出属性选择窗口，用代码制定图像宽和高
		{
			fprintf(stderr, "Can not open camera.\n");
			MessageBox("摄像头初始化失败","提示");
			CloseCamera();
			return;
		}

		Invalidate(true);
		if(m_lVideoWidth > 0)
		{
			//比例缩放
			DrawRect=m_common->SetDrawSize((CStatic *)GetDlgItem(IDC_STATIC_MAIN),
					old_DrawRect,m_lVideoWidth,m_lVideoHeight,&DrawScale);

			m_bThreadWork = true;

			m_pThreadDisplay=NULL;
			m_pThreadDisplay=CreateThread(NULL,0,MacthDisplayThread,this,0,NULL);
			m_pThreadDetect=NULL;
			m_pThreadDetect=CreateThread(NULL,0,MacthDetectThread,this,0,NULL);
			m_pThreadMacth=NULL;
			m_pThreadMacth=CreateThread(NULL,0,MacthThread,this,0,NULL);
		}
		else
		{
			MessageBox("未找到设备","提示");
		}
	}
}

/************************************
* 初始化窗口
*************************************/
void CFrmFaceMatch::InitChildWindow(void)
{

	GetDlgItem(IDC_STATIC_MAIN)->MoveWindow(22,24,487,386);
	//移动之后显示偏了，直接指定视频显示区域
	old_DrawRect.left=22;
	old_DrawRect.top=24;
	old_DrawRect.right =509;
	old_DrawRect.bottom =410;

	//按钮
	m_btnStart.LoadBitmaps(IDB_BTN_START_NORMAL,NULL,NULL,NULL);
	m_btnStart.SizeToContent();		//自适应图片大小
	m_btnStart.MoveWindow(448,426,53,24);

	//设备列表combo
	m_cbDevice.SetComboBitmap(IDB_BITMAP_LEFT,IDB_BITMAP_RIGHT,IDB_BITMAP_CEN);
	m_cbDevice.SetComboListBitmap(IDB_COMBO_LEFT, IDB_COMBO_RIGHT,IDB_COMBO_TOP,IDB_COMBO_BOTTOM);
	m_cbDevice.SetHighlightColor(SKIN_COMBO_COLOR,RGB(255,255,255));
	m_cbDevice.SetNormalPositionColor(SKIN_COMBO_COLOR,RGB(255,255,255));
	m_cbDevice.MoveWindow(303,425,145,100);
	//m_cbDevice.InsertString(0,"摄像头列表");
	//m_cbDevice.SetCurSel(0);
	m_btnclose.LoadBitmaps(IDB_BTN_CLOSE_NORMAL,IDB_BTN_CLOSE_PRESS,IDB_BTN_CLOSE_PRESS,NULL);
	m_btnclose.SizeToContent();		//自适应图片大小
	
	m_btnclose.MoveWindow(505,2,22,22);
}

/************************************
* 初始化参数
*************************************/
void CFrmFaceMatch::InitParameters(void)
{
	m_pThreadDisplay=NULL;
	m_pThreadDetect=NULL;
	m_pThreadMacth=NULL;
	
	MacthResult="";
	MacthLog = "";
	matchFlag = 0;
#if LIVE_FACE_TEST
	liveflag=0;
#endif
	m_bIsClose = true;
	m_bThreadWork = false;
	m_lVideoWidth = 0;
	m_lVideoHeight = 0;

	MacthTimeOut = 0;
	m_macthTimer = 0;
	m_macthTimer = SetTimer(FACE_MACTH_TIMER,1000,NULL);

	while(m_DetectList.size() > 0)
	{
		m_DetectList.pop();
	}
	while(m_MacthList.size() > 0)
	{
		m_MacthList.pop();
	}
	
	m_closefocus=false;
}

/************************************
* 启动人脸比对线程
*************************************/
int CFrmFaceMatch::StartMacthThread(void)
{
	InitParameters();
	if(m_common->InitialDevice(m_cbDevice, &m_lVideoWidth, &m_lVideoHeight))//设备初始化
	{	
		//比例缩放
		DrawRect=m_common->SetDrawSize((CStatic *)GetDlgItem(IDC_STATIC_MAIN),
				old_DrawRect,m_lVideoWidth,m_lVideoHeight,&DrawScale);

		m_bThreadWork = true;

		m_pThreadDisplay=NULL;
		m_pThreadDisplay=CreateThread(NULL,0,MacthDisplayThread,this,0,NULL);
		m_pThreadDetect=NULL;
		m_pThreadDetect=CreateThread(NULL,0,MacthDetectThread,this,0,NULL);
		m_pThreadMacth=NULL;
		m_pThreadMacth=CreateThread(NULL,0,MacthThread,this,0,NULL);

		return 1;
	}
	return 0;
}

/************************************
* 停止人脸比对线程
*************************************/
int CFrmFaceMatch::StopMacthThread(void)
{
	if(m_macthTimer) 
		KillTimer(m_macthTimer); 
	m_macthTimer = 0;

	m_bIsClose = true;
	if(m_bThreadWork)
	{
		m_bThreadWork = false;
		Sleep(1000);

		if(false == m_bIsClose)
		{
			TerminateThread(m_pThreadDisplay,0);
			m_pThreadDisplay=NULL;
			TerminateThread(m_pThreadDetect,0);
			m_pThreadDetect=NULL;
			TerminateThread(m_pThreadMacth,0);
			m_pThreadMacth=NULL;
		}
		Sleep(100);

		CloseCamera();
		Invalidate();

		return 1;
	}
	return 0;
}

/************************************
* 人脸比对
*************************************/
void CFrmFaceMatch::MacthProcess(void)
{
	if(m_MacthList.size()>0)
	{
		if(!b_macthLock)
		{
			b_macthLock = true;
			CString strRequest = m_MacthList.front();
			m_MacthList.pop();
			m_Detect.FaceMacth(strRequest,&MacthLog,&matchFlag);
			b_macthLock = false;
		}
	}
}


void CFrmFaceMatch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_MATCHBACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();
	// Do not call CDialog::OnPaint() for painting messages
}

void CFrmFaceMatch::OnBnClickedButtonClose()
{
	// TODO: Add your message handler code here and/or call default
	MacthLog.Format(_T("{\
					   \"ret\":\"fail\",\
					   \"user\":\"%s\",\
					   \"sysID\":\"%d\",\
					   \"content\":\"User_Cancel\"\
					   }"),DlgFaceLoginOCXCtrl->MatchUser,DlgFaceLoginOCXCtrl->MatchSysID);

	MacthResult.Format(_T("%d"),OCX_ERROR_USER_CANCEL);

	OnCancel();
}

void CFrmFaceMatch::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	bool old=m_closefocus;
	if(point.x <535 && point.x >500 &&point.y <35 && point.y >1 )
	{
		m_closefocus=true;
	}
	else
	{
		m_closefocus=false;
	}

	if( old != m_closefocus)
	{	
		if(m_closefocus)
			m_btnclose.SetFocus();
		else
			this->SetFocus();
	}

	CDialog::OnMouseMove(nFlags, point);
}
