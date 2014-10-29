// FrmFaceEnroll.cpp : implementation file
//

#include "stdafx.h"
#include "FaceLoginOCX.h"
#include "FrmFaceEnroll.h"
#include "base64_codec.h"
#include "YUV2RGB.h"

#define FACE_ENROLL_TIMER 2

#include "FaceLoginOCXCtrl.h"
extern CFaceLoginOCXCtrl *DlgFaceLoginOCXCtrl;

IMGLIST FacePicList[PICTURE_MAX];



//********************************线程**********************************
//视频显示线程
DWORD WINAPI EnrollDisplayThread(void *p)
{
	CFrmFaceEnroll * pFrmFaceEnroll = (CFrmFaceEnroll*)p;
	
	long  bitmapSize;
	char * buffer= NULL;

	pFrmFaceEnroll->m_bIsClose = false;

	while(pFrmFaceEnroll->m_bThreadWork)
	{
		try
		{
			QueryFrameCB(&bitmapSize,&buffer);

			if(bitmapSize > 0)
			{
				BITMAPINFO bmpInfo = {0};   
				bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   
				bmpInfo.bmiHeader.biWidth = pFrmFaceEnroll->m_lVideoWidth;   
				bmpInfo.bmiHeader.biHeight = pFrmFaceEnroll->m_lVideoHeight;
				bmpInfo.bmiHeader.biPlanes = 1;   
				bmpInfo.bmiHeader.biBitCount = 24;  
				try
				{
					if(!pFrmFaceEnroll->b_listLock)
					{
						//int tempw=pFrmFaceEnroll->m_lVideoWidth*3;
						//for(int y=0; y < pFrmFaceEnroll->m_lVideoHeight; y++)
						//{
						//	memcpy(&pFrmFaceEnroll->tempRGB[(pFrmFaceEnroll->m_lVideoHeight-1-y)*tempw],
						//		&buffer[y*tempw],tempw);
						//}
						memcpy(pFrmFaceEnroll->tempRGB,buffer,bitmapSize*sizeof(unsigned char));

						CString result = pFrmFaceEnroll->m_Detect.RGBtoBase64((unsigned char *)buffer,
																				bitmapSize,
																				pFrmFaceEnroll->m_lVideoWidth,
																				pFrmFaceEnroll->m_lVideoHeight);
						if(!result.IsEmpty())
						{
							pFrmFaceEnroll->m_DetectList.push(result);
						}
						//memcpy(pFrmFaceEnroll->tempImage.data,buffer,bitmapSize*sizeof(unsigned char));
						//pFrmFaceEnroll->tempImage.w = pFrmFaceEnroll->m_lVideoWidth;
						//pFrmFaceEnroll->tempImage.h = pFrmFaceEnroll->m_lVideoHeight;
						//pFrmFaceEnroll->tempImage.size = bitmapSize;
						//pFrmFaceEnroll->m_DetectList.push(pFrmFaceEnroll->tempImage);
					}

					//显示摄像头画面及人脸框叠加
					pFrmFaceEnroll->m_common->DrawCtrlImage((CStatic *)pFrmFaceEnroll->GetDlgItem(IDC_STATIC_MAIN), bmpInfo, 
															buffer, bitmapSize,pFrmFaceEnroll->face_Count,pFrmFaceEnroll->face_Rect_List,
															pFrmFaceEnroll->DrawRect,pFrmFaceEnroll->DrawScale);
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

	pFrmFaceEnroll->m_bIsClose = true;
	return 0;
}

//人脸检测线程
DWORD WINAPI EnrollDetectThread(void *p)
{
	CFrmFaceEnroll * pFrmFaceEnroll = (CFrmFaceEnroll*)p;

	pFrmFaceEnroll->m_bIsClose = false;

	while(pFrmFaceEnroll->m_bThreadWork)
	{
		try
		{
			if(pFrmFaceEnroll->m_DetectList.size() > 0)
			{
				pFrmFaceEnroll->b_listLock = true;
				CString strRequest = pFrmFaceEnroll->m_DetectList.front();
				pFrmFaceEnroll->m_DetectList.pop();
				pFrmFaceEnroll->m_Detect.FaceDetect(strRequest,&pFrmFaceEnroll->face_Count,pFrmFaceEnroll->face_Rect_List,pFrmFaceEnroll->Image_list);
				if(pFrmFaceEnroll->m_bCapture)
				{
					if(pFrmFaceEnroll->face_Count > 0)
					{
						pFrmFaceEnroll->FaceCapture(pFrmFaceEnroll->tempRGB,pFrmFaceEnroll->m_lVideoWidth,pFrmFaceEnroll->m_lVideoHeight);
					}
					pFrmFaceEnroll->m_bCapture = false;
				}
				pFrmFaceEnroll->b_listLock = false;

//////////////////////////////////////////////////图像压缩后再进行检测//////////////////////////////////////////////////////////
				//pFrmFaceEnroll->b_listLock = true;

				//int tempw=3*pFrmFaceEnroll->m_DetectList.front().w;

				//for(int y=0;y<pFrmFaceEnroll->m_DetectList.front().h/2;y++)
				//{
				//	for(int x=0;x<tempw/2;x++)
				//	{
				//		pFrmFaceEnroll->tempRGB[x+y*(tempw/2)] = pFrmFaceEnroll->m_DetectList.front().data[2*x+y*2*tempw];
				//	}
				//}

				//CString strRequest = pFrmFaceEnroll->m_Detect.RGBtoBase64(pFrmFaceEnroll->tempRGB,
				//														  pFrmFaceEnroll->m_DetectList.front().size/4,
				//														  pFrmFaceEnroll->m_DetectList.front().w/2,
				//														  pFrmFaceEnroll->m_DetectList.front().h/2);
				//pFrmFaceEnroll->m_DetectList.pop();
				//pFrmFaceEnroll->m_Detect.FaceDetect(strRequest,&pFrmFaceEnroll->face_Count,pFrmFaceEnroll->face_Rect_List);
				//pFrmFaceEnroll->b_listLock = false;
//////////////////////////////////////////////////图像压缩后再进行检测//////////////////////////////////////////////////////////

			}
		}
		catch(...)
		{
			continue;
		}
		
		Sleep(10);
	}

	pFrmFaceEnroll->m_bIsClose = true;
	return 0;
}
//********************************线程**********************************

// CFrmFaceEnroll dialog

IMPLEMENT_DYNAMIC(CFrmFaceEnroll, CDialog)

CFrmFaceEnroll::CFrmFaceEnroll(CWnd* pParent /*=NULL*/)
	: CDialog(CFrmFaceEnroll::IDD, pParent)
{
	b_listLock = false;
	m_bCapture = false;
	m_iCapIndex = 0;
	EnrollLog = _T("");
	EnrollResult = -1;
	tempRGB = NULL;
	for(int i=0;i<PICTURE_MAX;i++)
	{
		FacePicList[i].choose=false;
		FacePicList[i].display=false;
	}


	CfaceBGA[0].LoadBitmap(IDB_BITMAP_FACE1);    
	CfaceBGA[0].GetBitmap(&faceBGA[0]);
	CfaceBGA[1].LoadBitmap(IDB_BITMAP_FACE2);    
	CfaceBGA[1].GetBitmap(&faceBGA[1]);
	CfaceBGA[2].LoadBitmap(IDB_BITMAP_FACE3);    
	CfaceBGA[2].GetBitmap(&faceBGA[2]);
	CfaceBGA[3].LoadBitmap(IDB_BITMAP_FACE4);    
	CfaceBGA[3].GetBitmap(&faceBGA[3]);

	CfaceBGB[0].LoadBitmap(IDB_BITMAP_FACE1_CHOOSE);    
	CfaceBGB[0].GetBitmap(&faceBGB[0]);
	CfaceBGB[1].LoadBitmap(IDB_BITMAP_FACE2_CHOOSE);    
	CfaceBGB[1].GetBitmap(&faceBGB[1]);
	CfaceBGB[2].LoadBitmap(IDB_BITMAP_FACE3_CHOOSE);    
	CfaceBGB[2].GetBitmap(&faceBGB[2]);
	CfaceBGB[3].LoadBitmap(IDB_BITMAP_FACE4_CHOOSE);    
	CfaceBGB[3].GetBitmap(&faceBGB[3]);


}

CFrmFaceEnroll::~CFrmFaceEnroll()
{
	if(tempRGB)
	{
		free(tempRGB);
		tempRGB = NULL;
	}
}

void CFrmFaceEnroll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEV, m_cbDevice);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_BTN_CAPTURE, m_btnCapture);
	DDX_Control(pDX, IDC_BTN_ENROLL, m_btnEnroll);
	DDX_Control(pDX, IDC_BTN_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnclose);
}


BEGIN_MESSAGE_MAP(CFrmFaceEnroll, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_START, &CFrmFaceEnroll::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_CAPTURE, &CFrmFaceEnroll::OnBnClickedBtnCapture)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CFrmFaceEnroll::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_ENROLL, &CFrmFaceEnroll::OnBnClickedBtnEnroll)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_STATIC_FACE1, &CFrmFaceEnroll::OnStnClickedStaticFace1)
	ON_STN_CLICKED(IDC_STATIC_FACE2, &CFrmFaceEnroll::OnStnClickedStaticFace2)
	ON_STN_CLICKED(IDC_STATIC_FACE3, &CFrmFaceEnroll::OnStnClickedStaticFace3)
	ON_STN_CLICKED(IDC_STATIC_FACE4, &CFrmFaceEnroll::OnStnClickedStaticFace4)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CFrmFaceEnroll::OnBnClickedButtonClose)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CFrmFaceEnroll message handlers

BOOL CFrmFaceEnroll::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitChildWindow();

	StartEnrollThread();

/*
	char  str[128];
	sprintf(str,"%d %d %d",BBitbmp.bmHeight,BBitbmp.bmWidthBytes,BBitbmp.bmBitsPixel);
	MessageBox(str);
*/




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFrmFaceEnroll::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class


	StopEnrollThread();
	CDialog::OnCancel();
}

void CFrmFaceEnroll::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	EnrollLog.Format(_T("{\
						\"ret\":\"fail\",\
						\"user\":\"%s\",\
						\"sysID\":\"%d\",\
						\"content\":\"User_Cancle\"\
						}"),DlgFaceLoginOCXCtrl->EnrollUser,DlgFaceLoginOCXCtrl->EnrollSysID);

	EnrollResult = OCX_ERROR_USER_CANCLE;
		
	StopEnrollThread();
	CDialog::OnClose();
}

void CFrmFaceEnroll::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == FACE_ENROLL_TIMER)
	{
		if(EnrollTimeOut++ > OCX_TIMEOUT_MAX)
		{
			EnrollLog.Format(_T("{\
								\"ret\":\"fail\",\
								\"user\":\"%s\",\
								\"sysID\":\"%d\",\
								\"content\":\"Time_Out\"\
								}"),DlgFaceLoginOCXCtrl->EnrollUser,DlgFaceLoginOCXCtrl->EnrollSysID);

			EnrollResult = OCX_ERROR_TIME_OUT;

			OnCancel();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CFrmFaceEnroll::OnBnClickedBtnStart()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if(m_cbDevice.GetCount()==0)
	{
		return;
	}
	//if(m_cbDevice.GetCurSel() == 0)
	//{
	//	MessageBox(_T("请从列表中选择一个摄像头"), _T("提示"));
	//	return;
	//}

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
			m_pThreadDisplay=CreateThread(NULL,0,EnrollDisplayThread,this,0,NULL);
			m_pThreadDetect=NULL;
			m_pThreadDetect=CreateThread(NULL,0,EnrollDetectThread,this,0,NULL);
		}
		else
		{
			MessageBox("未找到设备","提示");
		}
	}
}

void CFrmFaceEnroll::OnBnClickedBtnCapture()
{
	// TODO: Add your control notification handler code here
	if(!m_bCapture)
		m_bCapture = true;
}

void CFrmFaceEnroll::OnBnClickedBtnClear()
{
	// TODO: Add your control notification handler code here
	

#if 0
	while(m_FaceList.size() > 0)
	{
		m_FaceList.pop();
	}

	m_iCapIndex = 0;
	//关联图片ID
	HBITMAP hBitmap1 =(HBITMAP)LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(IDB_BITMAP_FACE1),
								IMAGE_BITMAP,0,0,
								LR_LOADMAP3DCOLORS);

	((CStatic*)GetDlgItem(IDC_STATIC_FACE1))->SetBitmap(hBitmap1);

	HBITMAP hBitmap2 =(HBITMAP)LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(IDB_BITMAP_FACE2),
								IMAGE_BITMAP,0,0,
								LR_LOADMAP3DCOLORS);
	((CStatic*)GetDlgItem(IDC_STATIC_FACE2))->SetBitmap(hBitmap2);

	HBITMAP hBitmap3 =(HBITMAP)LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(IDB_BITMAP_FACE3),
								IMAGE_BITMAP,0,0,
								LR_LOADMAP3DCOLORS);
	((CStatic*)GetDlgItem(IDC_STATIC_FACE3))->SetBitmap(hBitmap3);

	HBITMAP hBitmap4 =(HBITMAP)LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(IDB_BITMAP_FACE4),
								IMAGE_BITMAP,0,0,
								LR_LOADMAP3DCOLORS);
	((CStatic*)GetDlgItem(IDC_STATIC_FACE4))->SetBitmap(hBitmap4);
#else
	if(false== FacePicList[0].choose &&
		false== FacePicList[1].choose &&
		false== FacePicList[2].choose &&
		false== FacePicList[3].choose )
	{
		MessageBox("单击选择图片后进行清除","人脸注册");
		return ;
	}

	for(int i=0;i<PICTURE_MAX;i++)
	{
		if( FacePicList[i].choose )
		{
			FacePicList[i].display=false;
			FacePicList[i].choose =false;

			DisplayOne(i);
		}
	}
	

#endif

}

void CFrmFaceEnroll::OnBnClickedBtnEnroll()
{
	// TODO: Add your control notification handler code here
#if 0
	if(m_FaceList.size() < PICTURE_MAX)
	{
		MessageBox(_T("请采集四张人脸"), _T("提示"));
		return;
	}
#else
	if(!( FacePicList[0].display &&
		 FacePicList[1].display &&
		 FacePicList[2].display &&
		FacePicList[3].display ))
	{
		MessageBox(_T("请采集四张人脸"), _T("提示"));
		return;
	}
#endif	

	CString face_id[PICTURE_MAX];
	CString temp;
	int i=0;
#if 0
	while(m_FaceList.size())
	{
		int ret = m_Detect.FaceEnroll(m_FaceList.front().data,m_FaceList.front().size,&temp);
		m_FaceList.pop();
		if(ret == ERR_NO_ERROR)
		{
			face_id[i] = temp;
			i++;
		}
#else
	for(int i=0;i<PICTURE_MAX;i++)
	{
			int ret = m_Detect.FaceEnroll(FacePicList[i].data,FacePicList[i].size,&temp);
			
			if(ret == ERR_NO_ERROR)
			{
				face_id[i] = temp;
			}
#endif
		else if(ret == ERR_REG_FAIL)
		{
			EnrollLog.Format(_T("{\
								\"ret\":\"fail\",\
								\"user\":\"%s\",\
								\"sysID\":\"%d\",\
								\"content\":\"Enroll_Fail\"\
								}"),DlgFaceLoginOCXCtrl->EnrollUser,DlgFaceLoginOCXCtrl->EnrollSysID);

			EnrollResult = OCX_ERROR_ENROLL_FAIL;

			OnCancel();
			return;
		}
		else if(ret == ERR_BAD_PERSON)
		{
			EnrollLog.Format(_T("{\
								\"ret\":\"fail\",\
								\"user\":\"%s\",\
								\"sysID\":\"%d\",\
								\"content\":\"Bad_Personid\"\
								}"),DlgFaceLoginOCXCtrl->EnrollUser,DlgFaceLoginOCXCtrl->EnrollSysID);

			EnrollResult = OCX_ERROR_UNALLOW_USER;

			OnCancel();
			return;
		}
	}
	EnrollLog.Format(_T("{\
						\"ret\":\"success\",\
						\"user\":\"%s\",\
						\"sysID\":\"%d\",\
						\"face_id1\":\"%s\",\
						\"face_id2\":\"%s\",\
						\"face_id3\":\"%s\",\
						\"face_id4\":\"%s\"\
						}"),DlgFaceLoginOCXCtrl->EnrollUser,DlgFaceLoginOCXCtrl->EnrollSysID,
						face_id[0],face_id[1],face_id[2],face_id[3]);

	CString strlog;
	strlog.Format(_T("<TIPS><ENROLL> - Success - User<%s><%s><%s><%s><%s>"),DlgFaceLoginOCXCtrl->EnrollUser,face_id[0],face_id[1],face_id[2],face_id[3]);
	DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

	EnrollResult = OCX_ERROR_NO_ERROR;

	OnCancel();
}

/************************************
* 初始化窗口
*************************************/
void CFrmFaceEnroll::InitChildWindow(void)
{

	GetDlgItem(IDC_STATIC_MAIN)->MoveWindow(22,54,487,386);
	//移动之后显示偏了，直接指定视频显示区域
	old_DrawRect.left=22;
	old_DrawRect.top=54;
	old_DrawRect.right =509;
	old_DrawRect.bottom =440;

	//人脸头像
	GetDlgItem(IDC_STATIC_FACE1)->MoveWindow(527,50,96,112);
	GetDlgItem(IDC_STATIC_FACE2)->MoveWindow(631,50,96,112);
	GetDlgItem(IDC_STATIC_FACE3)->MoveWindow(527,168,96,112);
	GetDlgItem(IDC_STATIC_FACE4)->MoveWindow(631,168,96,112);

	//按钮
	m_btnStart.LoadBitmaps(IDB_BTN_START_NORMAL,NULL,NULL,NULL);
	m_btnStart.SizeToContent();		//自适应图片大小
	m_btnCapture.LoadBitmaps(IDB_BTN_CAPTURE_NORMAL,IDB_BTN_CAPTURE_PRESS,NULL);
	m_btnCapture.SizeToContent();		//自适应图片大小
	m_btnEnroll.LoadBitmaps(IDB_BTN_ENROLL_NORMAL,IDB_BTN_ENROLL_PRESS,NULL,NULL);
	m_btnEnroll.SizeToContent();		//自适应图片大小
	m_btnClear.LoadBitmaps(IDB_BTN_CLEAR_NORMAL,IDB_BTN_CLEAR_PRESS,NULL,NULL);
	m_btnClear.SizeToContent();		//自适应图片大小

	m_btnStart.MoveWindow(673,338,53,24);
	m_btnCapture.MoveWindow(527,382,58,57);
	m_btnEnroll.MoveWindow(597,382,58,57);
	m_btnClear.MoveWindow(667,382,58,57);

	//设备列表combo
	m_cbDevice.SetComboBitmap(IDB_BITMAP_LEFT,IDB_BITMAP_RIGHT,IDB_BITMAP_CEN);
	m_cbDevice.SetComboListBitmap(IDB_COMBO_LEFT, IDB_COMBO_RIGHT,IDB_COMBO_TOP,IDB_COMBO_BOTTOM);
	m_cbDevice.SetHighlightColor(RGB(0,41,41),RGB(255,255,255));
	m_cbDevice.SetNormalPositionColor(RGB(0,41,41),RGB(255,255,255));
	m_cbDevice.MoveWindow(528,337,145,100);
	//m_cbDevice.InsertString(0,"摄像头列表");
	//m_cbDevice.SetCurSel(0);

	m_btnclose.LoadBitmaps(IDB_BTN_CLOSE_NORMAL,IDB_BTN_CLOSE_PRESS,IDB_BTN_CLOSE_PRESS,NULL);
	m_btnclose.SizeToContent();		//自适应图片大小

	m_btnclose.MoveWindow(720,7,22,22);

}

/************************************
* 初始化参数
*************************************/
void CFrmFaceEnroll::InitParameters(void)
{
	m_pThreadDisplay=NULL;
	m_pThreadDetect=NULL;

	m_bIsClose = true;
	m_bThreadWork = false;
	m_lVideoWidth = 0;
	m_lVideoHeight = 0;

	EnrollLog = _T("");
	m_iCapIndex = 0;
	EnrollTimeOut = 0;
	m_enrollTimer = 0;
	m_enrollTimer = SetTimer(FACE_ENROLL_TIMER,1000,NULL);

	while(m_DetectList.size() > 0)
	{
		m_DetectList.pop();
	}
#if 0
	while(m_FaceList.size() > 0)
	{
		m_FaceList.pop();
	}
#else
	for(int i=0;i<PICTURE_MAX;i++)
	{
		FacePicList[i].choose=false;
		FacePicList[i].display=false;
	}
#endif

m_closefocus=false;

	

}

/************************************
* 启动人脸比对线程
*************************************/
int CFrmFaceEnroll::StartEnrollThread(void)
{
	InitParameters();
	if(m_common->InitialDevice(m_cbDevice, &m_lVideoWidth, &m_lVideoHeight))//设备初始化
	{	
		if(tempRGB)
		{
			free(tempRGB);
			tempRGB=NULL;
		}
		tempRGB = (unsigned char*)calloc(3*m_lVideoWidth*m_lVideoHeight,sizeof(unsigned char));

		//比例缩放
		DrawRect=m_common->SetDrawSize((CStatic *)GetDlgItem(IDC_STATIC_MAIN),
				old_DrawRect,m_lVideoWidth,m_lVideoHeight,&DrawScale);

		m_bThreadWork = true;

		m_pThreadDisplay=NULL;
		m_pThreadDisplay=CreateThread(NULL,0,EnrollDisplayThread,this,0,NULL);
		m_pThreadDetect=NULL;
		m_pThreadDetect=CreateThread(NULL,0,EnrollDetectThread,this,0,NULL);

		return 1;
	}
	return 0;
}

/************************************
* 停止人脸比对线程
*************************************/
int CFrmFaceEnroll::StopEnrollThread(void)
{
	if(m_enrollTimer) 
		KillTimer(m_enrollTimer); 
	m_enrollTimer = 0;

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
		}
		Sleep(100);
		CloseCamera();
		Invalidate();

		if(tempRGB)
		{
			free(tempRGB);
			tempRGB=NULL;
		}

		return 1;
	}
	return 0;
}

void CFrmFaceEnroll::FaceCapture(unsigned char *image,unsigned long int width,unsigned long int height)
{
	CRect ClipRect;
	m_Detect.Clip(face_Count,face_Rect_List,width,height,&ClipRect);

	int w = ClipRect.right - ClipRect.left;
	int h = ClipRect.bottom - ClipRect.top;
	unsigned char *ClipImage = (unsigned char*)calloc(3*w*h , sizeof(unsigned char));
	unsigned long int size;

	m_Detect.ClipFaceImage(image,width,height,ClipImage,&size,ClipRect);
#if 0
	switch(m_iCapIndex)
	{
		case 0:
			m_common->DrawFaceImage((CStatic *)GetDlgItem(IDC_STATIC_FACE1),ClipImage,size);
			break;
		case 1:
			m_common->DrawFaceImage((CStatic *)GetDlgItem(IDC_STATIC_FACE2),ClipImage,size);
			break;
		case 2:
			m_common->DrawFaceImage((CStatic *)GetDlgItem(IDC_STATIC_FACE3),ClipImage,size);
			break;
		case 3:
			m_common->DrawFaceImage((CStatic *)GetDlgItem(IDC_STATIC_FACE4),ClipImage,size);
		default:
			break;
	}

	memcpy(tempFaceImage.data,ClipImage,size);
	tempFaceImage.size = size;
	m_FaceList.push(tempFaceImage);
	if(m_FaceList.size() > PICTURE_MAX)
	{
		m_FaceList.pop();
	}
	
	m_iCapIndex++;
	if(m_iCapIndex > 3)
	{
		m_iCapIndex = 0;
	}
#else
	//自动选择
	int Index;
	int flag=false;
	
	if( FacePicList[0].display &&
		FacePicList[1].display &&
		FacePicList[2].display &&
		FacePicList[3].display )
	{
		flag=true;
		Index=m_iCapIndex;
	}
	else
	{
		for(int i=0;i<PICTURE_MAX;i++)
		{
			if(	false == FacePicList[i].display)
			{
				Index=i;
				break;
			}
		}
		m_iCapIndex=0;
		flag=false;
	}
	CRect rect; 
	GetDlgItem(IDC_STATIC_FACE1)->GetClientRect(&rect);

	memcpy(	FacePicList[Index].data,ClipImage,size);
	FacePicList[Index].size = size;
	FacePicList[Index].display=true;

	DisplayOne(Index);

	//满
	if(flag)
	{
		m_iCapIndex++;
		if(m_iCapIndex > (PICTURE_MAX-1))
		{
			m_iCapIndex = 0;
		}
	}

#endif
	free(ClipImage);
	ClipImage = NULL;
}


void CFrmFaceEnroll::DisplayOne(int i)
{
	long n;
	switch(i)
	{
	case 0: n=IDC_STATIC_FACE1;break;
	case 1: n=IDC_STATIC_FACE2;break;
	case 2: n=IDC_STATIC_FACE3;break;
	case 3: n=IDC_STATIC_FACE4;break;
	default:return ;
	}

	if(FacePicList[i].choose)
	{
		m_common->DrawFaceImage((CStatic *)GetDlgItem(n),&CfaceBGB[i],&faceBGB[i]);
	}
	else
	{
		m_common->DrawFaceImage((CStatic *)GetDlgItem(n),&CfaceBGA[i],&faceBGA[i]);
	}
	if(FacePicList[i].display)
	{	
		m_common->DrawFaceImageMin((CStatic *)GetDlgItem(n),FacePicList[i].data,FacePicList[i].size);	
	}
}

void CFrmFaceEnroll::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_BACKGROUND);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	for(int i=0;i<PICTURE_MAX;i++)
	{
		DisplayOne(i);
	}
}

void CFrmFaceEnroll::OnStnClickedStaticFace1()
{
	// TODO: Add your control notification handler code here
//	char  str[128];
//	sprintf(str,"%d %d",FacePicList[0].display,FacePicList[0].choose);
//	MessageBox(str);

	if(false ==FacePicList[0].display)
		return ;

	if(FacePicList[0].choose)
		FacePicList[0].choose=false;
	else
		FacePicList[0].choose=true;
//	sprintf(str,"%d %d",FacePicList[0].display,FacePicList[0].choose);
//	MessageBox(str);
	DisplayOne(0);
	
}

void CFrmFaceEnroll::OnStnClickedStaticFace2()
{
	// TODO: Add your control notification handler code here
	if(false ==FacePicList[1].display)
		return ;

	if(FacePicList[1].choose)
		FacePicList[1].choose=false;
	else
		FacePicList[1].choose=true;

	DisplayOne(1);
}

void CFrmFaceEnroll::OnStnClickedStaticFace3()
{
	// TODO: Add your control notification handler code here
	if(false ==FacePicList[2].display)
		return ;

	if(FacePicList[2].choose)
		FacePicList[2].choose=false;
	else
		FacePicList[2].choose=true;
	DisplayOne(2);
}

void CFrmFaceEnroll::OnStnClickedStaticFace4()
{
	// TODO: Add your control notification handler code here
	if(false ==FacePicList[3].display)
		return ;

	if(FacePicList[3].choose)
		FacePicList[3].choose=false;
	else
		FacePicList[3].choose=true;
	DisplayOne(3);
}


void CFrmFaceEnroll::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	EnrollLog.Format(_T("{\
						\"ret\":\"fail\",\
						\"user\":\"%s\",\
						\"sysID\":\"%d\",\
						\"content\":\"User_Cancle\"\
						}"),DlgFaceLoginOCXCtrl->EnrollUser,DlgFaceLoginOCXCtrl->EnrollSysID);

	EnrollResult = OCX_ERROR_USER_CANCLE;

	StopEnrollThread();
	CDialog::OnCancel();
}

void CFrmFaceEnroll::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	bool old=m_closefocus;
	if(point.x <750 && point.x >710 &&point.y <40 && point.y >1 )
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
