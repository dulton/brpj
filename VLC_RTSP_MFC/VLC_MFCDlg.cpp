// VLC_MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VLC_MFC.h"
#include "VLC_MFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//用来其他类引用的主窗口类
CVLC_MFCDlg *DlgMain;

//垂直翻转
void FlipVertical(BYTE *pData, int image_width, int image_height, int bpp)
{
	int index = bpp/8;
	for (int h = 0; h < image_height/2; h++)
	{
		for (int w = 0; w < image_width; w++)
		{
			const int iCoordM = index*(h*image_width + w);
			const int iCoordN = index*((image_height - h -1)*image_width + w);
			BYTE Tmp = pData[iCoordM];
			pData[iCoordM] = pData[iCoordN];
			pData[iCoordN] = Tmp;
			Tmp = pData[iCoordM+1];
			pData[iCoordM + 1] = pData[iCoordN + 1]; 
			pData[iCoordN + 1] = Tmp;
			Tmp = pData[iCoordM + 2];
			pData[iCoordM + 2] = pData[iCoordN + 2];
			pData[iCoordN + 2] = Tmp;
		}
	}
}

int RGBtoBMP(unsigned char *image, int xsize, int ysize, char *filename)
{
	unsigned char header[54] = {0};
	header[0] = 0x42;		//"B"
	header[1] = 0x4d;		//"M"
	
	//header[2]~header[5]文件大小
	long file_size = (long)xsize * (long)ysize * 4 + 54;
	header[2] = (unsigned char)(file_size &0x000000ff);	
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;

	header[10] = 54;		//从文件开始到位图数据开始之间的数据(bitmap data)之间的偏移量
	header[14] = 40;		//位图信息头(Bitmap Info Header)的长度，用来描述位图的颜色、压缩方法等。下面的长度表示： 
							//28h - Windows 3.1x, 95, NT, ... 
							//0Ch - OS/2 1.x 
							//F0h - OS/2 2.x

	//宽
	long width = xsize;
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) &0x000000ff;
	header[20] = (width >> 16) &0x000000ff;
	header[21] = (width >> 24) &0x000000ff;
	
	//高
	long height = ysize;
	header[22] = height &0x000000ff;
	header[23] = (height >> 8) &0x000000ff;
	header[24] = (height >> 16) &0x000000ff;
	header[25] = (height >> 24) &0x000000ff;

	header[26] = 1;			//位图的位面数（注：该值将总是1）
	header[28] = 32;		//每个象素的位数

	char fname_bmp[128];
	sprintf(fname_bmp, "%s.bmp", filename);
	
	FILE *fp;
	if (!(fp = fopen(fname_bmp, "wb"))) 
		return -1;
	
	fwrite(header, sizeof(unsigned char), 54, fp);
	fwrite(image, sizeof(unsigned char), (size_t)(long)xsize * ysize * 4, fp);
	
	fclose(fp);
	return 0;
}

unsigned char buff[1920*1920*4]={NULL};
void *video_lock_cb(void *opaque, void **planes)

{
	*planes=opaque;
	return *planes;
}


void video_unlock_cb(void *opaque, void *picture,void *const *planes)
{

}


void video_display_cb(void *opaque, void *picture)
{
	FlipVertical((BYTE *)opaque,VIDEO_WIDTH,VIDEO_HEIGHT,32);
	DlgMain->DrawCtrlImage((char *)opaque,VIDEO_WIDTH*VIDEO_HEIGHT*4,DlgMain->DrawRect,DlgMain->DrawScale);
	RGBtoBMP((unsigned char *)opaque,VIDEO_WIDTH,VIDEO_HEIGHT,"D:\\1.bmp");
//	(unsigned char*)opaque,1280,720

//	return ;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVLC_MFCDlg dialog



CVLC_MFCDlg::CVLC_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVLC_MFCDlg::IDD, pParent)
	, m_Url(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bPlay = false;
	DlgMain=this;
}

void CVLC_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_URL, m_Url);
}

BEGIN_MESSAGE_MAP(CVLC_MFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_PLAY, &CVLC_MFCDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CVLC_MFCDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CVLC_MFCDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CVLC_MFCDlg message handlers

BOOL CVLC_MFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC_VIDEO)->GetWindowRect(&old_DrawRect);
	//全部移到10 10的框
	int x=old_DrawRect.left-10;
	int y=old_DrawRect.top-10;

	old_DrawRect.left=10;
	old_DrawRect.top=10;
	old_DrawRect.right -=x;
	old_DrawRect.bottom -=y;

	//比例缩放
	DrawRect=SetDrawSize((CStatic *)GetDlgItem(IDC_STATIC_VIDEO),
				old_DrawRect,VIDEO_WIDTH,VIDEO_HEIGHT,&DrawScale);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVLC_MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVLC_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVLC_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVLC_MFCDlg::OnBnClickedBtnPlay()
{
	// TODO: Add your control notification handler code here
	if(!m_bPlay)
	{
		UpdateData(true);

		m_bPlay = true;
		/* 加载 VLC 引擎 */  
		inst = libvlc_new (0, NULL);  

		/* Create a new item */  
		m = libvlc_media_new_path (inst, m_Url.GetBuffer());  

		/* Create a media player playing environement */  
		mp = libvlc_media_player_new_from_media (m);  

		/* No need to keep the media now */  
		libvlc_media_release (m);  

		libvlc_video_set_callbacks(mp,
                                 video_lock_cb,
                                 video_unlock_cb,
                                 video_display_cb,
                                 buff);

		libvlc_video_set_format(mp, "RV32", VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH*4);

		/* 下面的代码为具体平台上图像或声音渲染的句柄 */  
		//	libvlc_media_player_set_xdrawable (mp, xdrawable);  
		/* or on windows */  
//		CStatic *m_Video;
//		m_Video = (CStatic *)GetDlgItem(IDC_STATIC_VIDEO);
//		libvlc_media_player_set_hwnd (mp, m_Video->GetSafeHwnd()); 

		/* or on mac os */  
		//	libvlc_media_player_set_nsobject (mp, view);  
	}

	/* play the media_player */  
	libvlc_media_player_play (mp);
}

void CVLC_MFCDlg::OnBnClickedBtnPause()
{
	// TODO: Add your control notification handler code here

	libvlc_media_player_pause (mp);
}

void CVLC_MFCDlg::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	if(m_bPlay)
	{
		m_bPlay = false;

		/* Stop playing */  
		libvlc_media_player_stop (mp);  

		/* Free the media_player */  
		libvlc_media_player_release (mp);  

		libvlc_release (inst);  
	}
}

void CVLC_MFCDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_bPlay)
	{
		OnBnClickedBtnStop();
	}

	CDialog::OnCancel();
}

void CVLC_MFCDlg::DrawCtrlImage(char * buffer, long int bufferSize,CRect rect,float scale)
{
	BITMAPINFO bmpInfo = {0};   
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   
	bmpInfo.bmiHeader.biWidth = VIDEO_WIDTH;   
	bmpInfo.bmiHeader.biHeight = VIDEO_HEIGHT;
	bmpInfo.bmiHeader.biPlanes = 1;   
	bmpInfo.bmiHeader.biBitCount = 32;

	CStatic *m_picBox;
	m_picBox = (CStatic *)GetDlgItem(IDC_STATIC_VIDEO);

	CDC *pDC = m_picBox->GetDC();
	if(NULL == pDC)
		return;
	HDC hdc = pDC->GetSafeHdc();
	HDC hdcMem = CreateCompatibleDC(hdc); 

	LPBYTE   lpBits;
	HBITMAP hBmp = CreateDIBSection(hdcMem,&bmpInfo,DIB_RGB_COLORS,(void **)&lpBits,NULL,0);  
	memcpy(lpBits, buffer, bufferSize);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hBmp); 


	//SetStretchBltMode(hdc, HALFTONE);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchBlt(hdc, rect.left,rect.top,rect.Width(),rect.Height(), hdcMem, 0, 0, bmpInfo.bmiHeader.biWidth, bmpInfo.bmiHeader.biHeight, SRCCOPY);	


	SelectObject(hdcMem,hOldBmp);//复原兼容DC数据.
	DeleteDC(hdcMem);
	DeleteObject(hOldBmp);
	DeleteObject(hBmp);
//	free(lpBits);

	if(NULL != pDC)
	{
		m_picBox-> ReleaseDC(pDC);
	}
}

CRect CVLC_MFCDlg::SetDrawSize(CStatic * m_picBox,CRect old_DrawRect,int bmpw,int bmph,float *scale) 
{
	//////////////重设矩形框 否则出现未刷新BUG

	CRect rect;

	// 读取图片的宽和高
	int w = bmpw;
	int h = bmph;
	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;
	// 计算将图片缩放到TheImage区域所需的比例因子
	(*scale) = 1.0;
	int rw ,rh;
	rw = old_DrawRect.Width();// 求出图片控件的宽和高
	rh = old_DrawRect.Height();
	(*scale) = (float)rh/h <(float) rw/w ?(float)rh/h:(float) rw/w;
	// 缩放后图片的宽和高
	int nw = (int)( w* (*scale) );
	int nh = (int)( h*(*scale) );
	int iw = nw;//img->width;						// 读取图片的宽和高
	int ih = nh;//img->height;
	int tx = (int)(rw - iw)/2;					// 使图片的显示位置正好在控件的正中
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );

	CRect new_clientRect;
	new_clientRect.left	=	old_DrawRect.left+rect.left;
	new_clientRect.top	=	old_DrawRect.top+rect.top;
	new_clientRect.bottom	=	new_clientRect.top+rect.Height();
	new_clientRect.right	=	new_clientRect.left+rect.Width();

	//重置矩形
	m_picBox->MoveWindow(new_clientRect);

	//重置绘图矩形
	SetRect( rect, 0, 0, iw, ih );

	return rect;

}
