// video2detectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "video2detect.h"
#include "video2detectDlg.h"

#ifdef __cplusplus
extern "C" 
{
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
}
#endif

#include "IO.h"
IO OracleIO;

#if ALLTAB_DETECT_MODE
#include "CarDetect.h"
CCarDetect CarDetect;
#endif

CVideo2detectDlg *DlgMain;


bool playflag=false;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideo2detectDlg dialog

CVideo2detectDlg::CVideo2detectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideo2detectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideo2detectDlg)
	m_edit_file = _T("");
	m_ip = _T("");
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideo2detectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideo2detectDlg)
	DDX_Control(pDX, IDC_LIST, m_ListCar);
	DDX_Control(pDX, IDC_STATIC_PLAYWIN, m_playwin);
	DDX_Control(pDX, IDC_PROGRESS_FILE, m_progress_file);
	DDX_Text(pDX, IDC_EDIT_FILE, m_edit_file);
	DDV_MaxChars(pDX, m_edit_file, 260);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDV_MaxChars(pDX, m_ip, 32);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideo2detectDlg, CDialog)
	//{{AFX_MSG_MAP(CVideo2detectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_DETECT, OnButtonDetect)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST,OnLvnItemActivateList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideo2detectDlg message handlers

BOOL CVideo2detectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	
	EnablePlayState();
	GetCurrentDirectory(MAX_PATH, CurrentDir);
	
	sprintf(ResultDir,"%s\\Result",CurrentDir);
	CreateDirectory(ResultDir, NULL);

	playwin_hdc=m_playwin.GetDC()->GetSafeHdc();
	m_playwin.GetClientRect(&playwin_rect);

	SetStretchBltMode(playwin_hdc, COLORONCOLOR );


	ListMainInit();

	DlgMain=this;

#if YRVM_PINGTAI_MODE
	int OracleError = OracleIO.YRVM_ConnectionOracleDBTXT("YRVMDataBaseConfig.txt");
	if(OracleError == ReadFile_FAIL)
	{
		MessageBox("YRVM读数据库配置文件失败");
		return FALSE;
	}
	else if(OracleError == Instance_FAIL)
	{
		MessageBox("YRVM数据库ADO初始化失败,缺少AfxOleInit?");
		return FALSE;
	}
	else if(OracleError == ContOpen_FAIL)
	{
		MessageBox("YRVM数据库无法连接:账号密码IP异常 或文件夹带()");
		return FALSE;
	}
	else if(OracleError == Connectd_DONE)
	{
		MessageBox("连接数据库成功");
	}
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideo2detectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideo2detectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideo2detectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVideo2detectDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CVideo2detectDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnButtonStop();

	CDialog::OnCancel();
}

void CVideo2detectDlg::OnButtonFile() 
{
	// TODO: Add your control notification handler code here
	char szFilter[]="h264 Files (*.avi)|*.avi|*.*|*.*||";
	CFileDialog dlg(TRUE,"avi","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_edit_file=dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
//    assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

    BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

    memset( bmih, 0, sizeof(*bmih));

    bmih->biSize = sizeof(BITMAPINFOHEADER);

    bmih->biWidth = width;

    bmih->biHeight = origin ? abs(height) : -abs(height);

    bmih->biPlanes = 1;

    bmih->biBitCount = (unsigned short)bpp;

    bmih->biCompression = BI_RGB;

    if( bpp == 8 )
    {
        RGBQUAD* palette = bmi->bmiColors;

        int i;

        for( i = 0; i < 256; i++ )
        {
            palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;

            palette[i].rgbReserved = 0;
        }
    }
}

//////////////////////////////////////////////
void SaveFrame(CVideo2detectDlg *pDlg,AVFrame *pFrame, int width, int height, int iFrame) 
{
#if 0
	FILE *pFile;
	char szFilename[32];
	int  y;
	
	// Open file
	sprintf(szFilename, "frame%d.ppm", iFrame);
	pFile=fopen(szFilename, "wb");
	if(pFile==NULL)
		return;
	
	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);
	
	// Write pixel data
	for(y=0; y<height; y++)
		fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
	
/*
	YUV
	memcpy(data,pFrame->data[0],pFrame->linesize[0]*height);
	memcpy(&data[pFrame->linesize[0]*height],pFrame->data[1],pFrame->linesize[1]*height/2);
	memcpy(&data[pFrame->linesize[0]*height+pFrame->linesize[1]*height/2],
			pFrame->data[2],pFrame->linesize[2]*height/2);
	
	*/

	// Close file
	fclose(pFile);
#endif

#if ALLTAB_DETECT_MODE
	//拷贝数值
				CarDetect.m_playhandle=0;
				
				CarDetect.alarmflag=0;
				
				CarDetect.camid=iFrame;
				
				strcpy(CarDetect.cam_name,"视频流");
				
				strcpy(CarDetect.l_ipaddr,pDlg->m_ip.GetBuffer(0));
				//颜色LC_VIDEO_FORMAT_I420
				CarDetect.Start(LC_VIDEO_FORMAT_BGR24,\
					pFrame->data[0],width, height,pFrame->linesize[0]*height);
				
				CarDetect.Result();
#endif

	///////////以下用来显示/////////////////////////

	//  unsigned char  buffer[sizeof(BITMAPINFOHEADER) + 1024];

    //    BITMAPINFO* bmi = (BITMAPINFO*)buffer;

	BITMAPINFO bmi;
	
	FillBitmapInfo( &bmi,width, height,24, 0 );
		  
	::StretchDIBits(
		pDlg->playwin_hdc,
		0,0,
		pDlg->playwin_rect.right- pDlg->playwin_rect.left,
		pDlg->playwin_rect.bottom - pDlg->playwin_rect.top,
		0,0,width, height,
		pFrame->data[0], &bmi, DIB_RGB_COLORS, SRCCOPY );


}


int VideoPlay(char * filePath,CVideo2detectDlg *pDlg) 
{
	AVFormatContext *pFormatCtx;
	int             i, videoStream;
	AVCodecContext  *pCodecCtx;
	AVCodec         *pCodec;
	AVFrame         *pFrame; 
	AVFrame         *pFrameRGB;
	AVPacket        packet;
	int             frameFinished;
	int             numBytes;
	uint8_t         *buffer;
	static struct SwsContext *img_convert_ctx;
	
	// Register all formats and codecs
	av_register_all();
    // Open video file
	if(av_open_input_file(&pFormatCtx, filePath, NULL, 0, NULL)!=0)
		return -1; // Couldn't open file
	
	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0)
		return -1; // Couldn't find stream information
	
	// Dump information about file onto standard error
	dump_format(pFormatCtx, 0, filePath, 0);
    // Find the first video stream
	videoStream=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO) 
		{
			videoStream=i;
			break;
		}
	}
	
	if(videoStream==-1)
		return -1; // Didn't find a video stream
	// Get a pointer to the codec context for the video stream
	pCodecCtx=pFormatCtx->streams[videoStream]->codec;
	
	// Find the decoder for the video stream
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL) 
	{
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}
	// Open codec
	if(avcodec_open(pCodecCtx, pCodec)<0)
		return -1; // Could not open codec
	
	// Allocate video frame
	pFrame=avcodec_alloc_frame();
	
	// Allocate an AVFrame structure
	pFrameRGB=avcodec_alloc_frame();
	if(pFrameRGB==NULL)
		return -1;

	// Determine required buffer size and allocate buffer
	numBytes=avpicture_get_size(PIX_FMT_BGR24 , pCodecCtx->width,pCodecCtx->height);
	buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	
	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_BGR24,pCodecCtx->width, pCodecCtx->height);

	// 计算总时长
	int tns, thh, tmm, tss,us;
	//秒
	tns  = pFormatCtx->duration / AV_TIME_BASE;
	//微秒
	us = pFormatCtx->duration % AV_TIME_BASE * 100 / AV_TIME_BASE;
	//时
	thh  = tns / 3600;
	//分
	tmm  = (tns % 3600) / 60;
	//秒
	tss  = (tns % 60);

	int total=pCodecCtx->frame_number;
	//计算的帧数 不一定准
	int fps=pFormatCtx->streams[0]->r_frame_rate.num/pFormatCtx->streams[0]->r_frame_rate.den;
	if(0 == total )
		total=(tns+us/100.0)*fps+1;

	//设置进度条
	pDlg->m_progress_file.SetRange32(0,total);
	pDlg->m_progress_file.SetStep(1);
	pDlg->m_progress_file.SetPos(0);

	//定位到N帧 关键 lStart/fps= 秒 
	int lStart=390;
//	av_seek_frame(pFormatCtx, -1, lStart*AV_TIME_BASE, AVSEEK_FLAG_BACKWARD);
		av_seek_frame(pFormatCtx, -1, lStart/fps*AV_TIME_BASE, AVSEEK_FLAG_BACKWARD);

 // avcodec_flush_buffers(pFormatCtx->streams[videoStream]->codec);

   // avcodec_flush_buffers(pFormatCtx->streams[audio_stream]->codec);

	// Read frames and save first five frames to disk
	i=0;
	char rate[10]={0};
	while(av_read_frame(pFormatCtx, &packet)>=0) 
	{
		Sleep(2);
		if(packet.stream_index==videoStream) 
		{
			// Decode video frame
			avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,packet.data, packet.size);
			
			if(frameFinished)
			{
				// Convert the image from its native format to RGB
				img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
				
				// Convert the image from its native format to RGB
				sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize,0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

				//	if(++i<=5)
				++i;
				SaveFrame(pDlg,pFrameRGB, pCodecCtx->width, pCodecCtx->height,i);

				//增加这句来释放
				sws_freeContext(img_convert_ctx);

				//增加进度条
				pDlg->m_progress_file.StepIt();
			//	sprintf(rate,"%d%%",(int)((double)i/(double)(total-1)*100));
				//第N帧
				sprintf(rate,"%d",packet.dts);
				pDlg->GetDlgItem(IDC_STATIC_RATE)->SetWindowText(rate);
				
			}
		}
		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
					
		//停止播放
		if(false == playflag)	
			break;
	}
	// Free the RGB image
	av_free(buffer);
	av_free(pFrameRGB);
	
	// Free the YUV frame
	av_free(pFrame);
	
	// Close the codec
	avcodec_close(pCodecCtx);
	
	// Close the video file
	av_close_input_file(pFormatCtx);
	
	return 0;
}

DWORD WINAPI PlayThreadPROC(LPVOID lpParameter)
{
	CVideo2detectDlg *pDlg= (CVideo2detectDlg*)lpParameter;

	if(VideoPlay(pDlg->m_edit_file.GetBuffer(0),pDlg) <0)
		pDlg->MessageBox("播放文件失败");

	playflag=false;
//	pDlg->EnablePlayState();

	return 0;
}

///////////////////////////////////////////////

void CVideo2detectDlg::OnButtonDetect() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_ip.IsEmpty() ||
		m_edit_file.IsEmpty() )
	{
		MessageBox("文件名或者IP地址为空");
		return ;
	}

	playflag=true;
	pthread=CreateThread(NULL,0,PlayThreadPROC,this,0,NULL);

	EnablePlayState();
}


void CVideo2detectDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here

	playflag=false;
	Sleep(100);
	if(pthread)
		TerminateThread(pthread,0);

	EnablePlayState();
}

void CVideo2detectDlg::EnablePlayState() 
{
	if(playflag)
	{
		GetDlgItem(IDC_BUTTON_DETECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DETECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(TRUE);
	}
}

void CVideo2detectDlg::ListMainInit(void)
{

#if ALLTAB_DETECT_CAR_MODE

	m_ListCar.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 40);
	m_ListCar.InsertColumn(1, _T("文件名称" ), LVCFMT_LEFT, 140);
	m_ListCar.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, 110);
	m_ListCar.InsertColumn(3, _T("帧数"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(7, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(8, _T("车牌类型"), LVCFMT_LEFT, 100);
	m_ListCar.InsertColumn(9, _T("车身颜色"), LVCFMT_LEFT, 60);
	m_ListCar.InsertColumn(10, _T("图片路径"), LVCFMT_LEFT, 0);
#else
	m_ListCar.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 40);
	m_ListCar.InsertColumn(1, _T("文件名称" ), LVCFMT_LEFT, 140);
	m_ListCar.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, 110);
	m_ListCar.InsertColumn(3, _T("帧数"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(7, _T("图片路径"), LVCFMT_LEFT, 0);

#endif
	m_ListCar.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListCarTotal=0;

}
//双击显示大图
void CVideo2detectDlg::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];

#if ALLTAB_DETECT_CAR_MODE
//汽车
	m_ListCar.GetItemText(pNMIA->iItem,10,str,260);
#else
//电动车
	m_ListCar.GetItemText(pNMIA->iItem,7,str,260);
#endif

	ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	*pResult = 0;
}