// FrmFaceMatch.cpp : implementation file
//

#include "stdafx.h"
#include "FaceLoginOCX.h"
#include "FrmFaceMatch.h"





//检测人脸抽取模板线程
DWORD WINAPI DetectThread(void *p)
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
					int tempw=pFrmFaceMatch->m_lVideoWidth*3;
					for(int y=0; y < pFrmFaceMatch->m_lVideoHeight; y++)
					{
						memcpy(&pFrmFaceMatch->tempRGB[(pFrmFaceMatch->m_lVideoHeight-1-y)*tempw],
							&buffer[y*tempw],tempw);
					}

					//清0上一次的人脸个数list_size
					pFrmFaceMatch->list_size = 0;
					//开始检测人脸
					YRDetectFace((unsigned char *)pFrmFaceMatch->tempRGB,bitmapSize,pFrmFaceMatch->m_lVideoWidth,pFrmFaceMatch->m_lVideoHeight,
								pFrmFaceMatch->list_size,pFrmFaceMatch->face_rect_list);

					//显示摄像头画面及人脸框叠加
					pFrmFaceMatch->m_common->DrawCtrlImage((CStatic *)pFrmFaceMatch->GetDlgItem(IDC_STATIC_MAIN), bmpInfo, 
															buffer, bitmapSize, 
															pFrmFaceMatch->list_size,pFrmFaceMatch->face_rect_list,
															pFrmFaceMatch->DrawRect,pFrmFaceMatch->DrawScale);
					
					if(pFrmFaceMatch->list_size>0)
					{
						if(!pFrmFaceMatch->b_getFace)
						{
							if(pFrmFaceMatch->face_image_list[pFrmFaceMatch->face_image_count] == NULL)
							{
								pFrmFaceMatch->face_image_list[pFrmFaceMatch->face_image_count] = (unsigned char*)calloc(bitmapSize,sizeof(unsigned char));
							}
							memcpy(pFrmFaceMatch->face_image_list[pFrmFaceMatch->face_image_count],pFrmFaceMatch->tempRGB,bitmapSize);
							pFrmFaceMatch->face_image_count++;
							if(pFrmFaceMatch->face_image_count == 10)
							{
								pFrmFaceMatch->face_image_count = 0;
							}
						}
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


// CFrmFaceMatch dialog

IMPLEMENT_DYNAMIC(CFrmFaceMatch, CDialog)

CFrmFaceMatch::CFrmFaceMatch(CWnd* pParent /*=NULL*/)
: CDialog(CFrmFaceMatch::IDD, pParent)
{
	for(int i=0;i<10;i++)
	{
		face_image_list[i] = NULL;
	}
	tempRGB=NULL;
	TCHAR CurrentDir[260]="D:\\FaceLoginOCX\\";
	//GetCurrentDirectory(260, CurrentDir);
	InitializeFaceCloud(CurrentDir);
}

CFrmFaceMatch::~CFrmFaceMatch()
{
	//StopThread();
	UnInitializeFaceCloud();
		
	for(int i=0;i<10;i++)
	{
		if(face_image_list[i] != NULL)
		{
			free(face_image_list[i]);
			face_image_list[i] = NULL;
		}
	}

	if(tempRGB)
	{
		free(tempRGB);
		tempRGB = NULL;
	}
}

void CFrmFaceMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEV, m_cbDevice);
}


BEGIN_MESSAGE_MAP(CFrmFaceMatch, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START, &CFrmFaceMatch::OnBnClickedBtnStart)
END_MESSAGE_MAP()

BOOL CFrmFaceMatch::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC_MAIN)->GetWindowRect(&old_DrawRect);
	//全部移到10 10的框
	int x=old_DrawRect.left-10;
	int y=old_DrawRect.top-10;

	old_DrawRect.left=10;
	old_DrawRect.top=10;
	old_DrawRect.right -=x;
	old_DrawRect.bottom -=y;

	// TODO:  Add extra initialization here
	//StartThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// CFrmFaceMatch message handlers
void CFrmFaceMatch::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	if(m_bThreadWork && m_pThreadDetect)
	{
		m_bThreadWork = false;
		Sleep(100);

		if(false == m_bIsClose)
		{
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
			m_pThreadDetect=NULL;
			m_pThreadDetect=CreateThread(NULL,0,DetectThread,this,0,NULL);
		}
		else
		{
			MessageBox("未找到设备","提示");
		}
	}
}

/************************************
* 初始化参数
*************************************/
void CFrmFaceMatch::InitParameters()
{
	m_pThreadDetect=NULL;
	m_bIsClose = true;
	m_bThreadWork = false;
	m_lVideoWidth = 0;
	m_lVideoHeight = 0;
	face_image_count = 0;
	b_getFace = false;
}

/************************************
* 启动线程
*************************************/
int CFrmFaceMatch::StartThread()
{
	InitParameters();
	if(m_common->InitialDevice(m_cbDevice, &m_lVideoWidth, &m_lVideoHeight))//设备初始化
	{	
		for(int i=0;i<10;i++)
		{
			if(face_image_list[i])
			{
				free(face_image_list[i]);
				face_image_list[i] = NULL;
			}
			//face_image_list[i] = (unsigned char*)calloc(3*m_lVideoWidth*m_lVideoHeight,sizeof(unsigned char));
		}

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

		m_pThreadDetect=NULL;
		m_pThreadDetect=CreateThread(NULL,0,DetectThread,this,0,NULL);
	
		return 1;
	}
	return 0;
}

/************************************
* 停止线程
*************************************/
int CFrmFaceMatch::StopThread()
{
	if(m_bThreadWork && m_pThreadDetect)
	{
		m_bThreadWork = false;
		Sleep(100);

		if(false == m_bIsClose)
		{
			TerminateThread(m_pThreadDetect,0);
			m_pThreadDetect=NULL;
		}
		Sleep(100);
		CloseCamera();
		Invalidate();

		for(int i=0;i<10;i++)
		{
			if(face_image_list[i])
			{
				free(face_image_list[i]);
				face_image_list[i] = NULL;
			}
		}
		if(tempRGB)
		{
			free(tempRGB);
			tempRGB=NULL;
		}
		
		return 1;
	}
	return 0;
}

void CFrmFaceMatch::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

void CFrmFaceMatch::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	CloseWindow();
	CDialog::OnClose();
}

void CFrmFaceMatch::CloseWindow()
{
	m_bIsClose = true;
	StopThread();
}


