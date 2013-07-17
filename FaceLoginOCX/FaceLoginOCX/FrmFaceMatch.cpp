// FrmFaceMatch.cpp : implementation file
//

#include "stdafx.h"
#include "FaceLoginOCX.h"
#include "FrmFaceMatch.h"
#include "base64_codec.h"

//检测人脸抽取模板线程
DWORD WINAPI DetectThread(void *p)
{
	CFrmFaceMatch * pFrmFaceMatch = (CFrmFaceMatch*)p;
	
	long  bitmapSize;
	char * buffer= NULL;
	int FaceImageCount=0;

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
					pFrmFaceMatch->m_common->DrawCtrlImage((CStatic *)pFrmFaceMatch->GetDlgItem(IDC_STATIC_MAIN), bmpInfo, buffer, bitmapSize);
				/*
					FaceImageCount++;
					if(FaceImageCount==10)
					{
						FaceImageCount = 0;
					}
					if(pFrmFaceMatch->FaceImage[FaceImageCount] == NULL)
					{
						pFrmFaceMatch->FaceImage[FaceImageCount] = (char *)calloc(OUT_ENBASE64_SIZE(bitmapSize) ,sizeof(char));
						if(pFrmFaceMatch->FaceImage[FaceImageCount] == NULL)
						{
							// 内存申请失败
							continue;
						}
					}
					if(base64_encode(pFrmFaceMatch->FaceImage[FaceImageCount], sizeof(char) * OUT_ENBASE64_SIZE(bitmapSize),
									(unsigned char *)buffer, bitmapSize) == 0)
					{
						free(pFrmFaceMatch->FaceImage[FaceImageCount]);
						pFrmFaceMatch->FaceImage[FaceImageCount] = NULL;
					}
					*/
				}
				catch(...)
				{
					continue;
				}
			}
			else
			{			
				Sleep(500);
			}
			
		}
		catch(...)
		{
			continue;
		}
	}
	if(pFrmFaceMatch->m_bIsClose)
	{

	}
	return 0;
}


// CFrmFaceMatch dialog

IMPLEMENT_DYNAMIC(CFrmFaceMatch, CDialog)

CFrmFaceMatch::CFrmFaceMatch(CWnd* pParent /*=NULL*/)
	: CDialog(CFrmFaceMatch::IDD, pParent)
{
	for(int i=0;i<10;i++)
	{
		FaceImage[i] = NULL;
	}
}

CFrmFaceMatch::~CFrmFaceMatch()
{
	//StopThread();
}

void CFrmFaceMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEV, m_cbDevice);
}


BEGIN_MESSAGE_MAP(CFrmFaceMatch, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CFrmFaceMatch::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//StartThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// CFrmFaceMatch message handlers
/************************************
* 初始化参数
*************************************/
void CFrmFaceMatch::InitParameters()
{
	m_bIsClose = false;
	m_bThreadWork = false;
	m_lVideoWidth = 0;
	m_lVideoHeight = 0;
}

/************************************
* 启动线程
*************************************/
int CFrmFaceMatch::StartThread()
{
	InitParameters();
	if(m_common->InitialDevice(m_cbDevice, &m_lVideoWidth, &m_lVideoHeight))//设备初始化
	{
		m_bThreadWork = true;
		int nPriority = THREAD_PRIORITY_BELOW_NORMAL;//默认为THREAD_PRIORITY_NORMAL
		UINT nStackSize = 0;//与创建它的线程堆栈大小相同
		DWORD dwCreateFlags = 0;//创建后立即执行
		m_pThreadDetect=CreateThread(NULL,0,DetectThread,this,0,NULL);
		//m_pThreadMatch  = AfxBeginThread(MatchThread,this, nPriority, nStackSize, dwCreateFlags);
		return 1;
	}
	return 0;
}

/************************************
* 停止线程
*************************************/
int CFrmFaceMatch::StopThread()
{
	if(m_bThreadWork)
	{
		m_bThreadWork = false;
		Sleep(500);
		TerminateThread(m_pThreadDetect,0);
		CloseCamera();
		Invalidate();

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
	FILE *fp=fopen("D:\\error.txt","a+");
	if(fp)
	{
		fprintf(fp,"Close window here!\n");
		fclose(fp);
	}
	m_bIsClose = true;
	StopThread();
}

