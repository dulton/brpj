// DLGVideoDetect.cpp : implementation file
//


#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGVideoDetect.h"

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



#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

#include "DLGpictureView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDLGVideoDetect *DlgVDmain;

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoDetect dialog


CDLGVideoDetect::CDLGVideoDetect(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGVideoDetect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGVideoDetect)
	m_check_writedb = FALSE;
	m_edit_file = _T("");
	m_camip = _T("");
	m_static_endtime = _T("00:00:00");
	m_static_starttime = _T("00:00:00");
	m_StartHour = 0;
	m_StartMon = 0;
	//}}AFX_DATA_INIT
	camid =0;
	AreaStr= _T("");
	CamStr=_T("");

	speedflag=true;
	playflag=PLAY_FLAG_STOP;

	sliderSeekflag=false;
	sliderSeek=0;

	cutpic=false;

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;

	m_ListCarTotal=0;
}


void CDLGVideoDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGVideoDetect)
	DDX_Control(pDX, IDC_COMBO_CAM, m_cam);
	DDX_Control(pDX, IDC_COMBO_AREA, m_area);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_STATIC_PLAYWIN, m_playwin);
	DDX_Check(pDX, IDC_CHECK_WRITE_DB, m_check_writedb);
	DDX_Text(pDX, IDC_EDIT_FILE, m_edit_file);
	DDX_Text(pDX, IDC_STATIC_CAMIP, m_camip);
	DDV_MaxChars(pDX, m_camip, 260);
	DDX_Text(pDX, IDC_STATIC_ENDTIME, m_static_endtime);
	DDX_Text(pDX, IDC_STATIC_STARTTIME, m_static_starttime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGVideoDetect, CDialog)
	//{{AFX_MSG_MAP(CDLGVideoDetect)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_CUTPIC, OnButtonCutpic)
	ON_BN_CLICKED(IDC_BUTTON_SPEED, OnButtonSpeed)
	ON_BN_CLICKED(IDC_BUTTON_STOP2, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_CBN_CLOSEUP(IDC_COMBO_AREA, OnCloseupComboArea)
	ON_CBN_CLOSEUP(IDC_COMBO_CAM, OnCloseupComboCam)
	ON_BN_CLICKED(IDC_CHECK_WRITE_DB, OnCheckWriteDb)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST,OnLvnItemActivateList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, OnNMCustomdrawList)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER, OnReleasedcaptureSlider)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoDetect message handlers



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

void ZOGDramBMP(char *path,unsigned char *buffer, int w, int h)
{
	FILE *file_out;
	int len;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	memset( &bfh, 0, sizeof( bfh ) );
	memset( &bih, 0, sizeof( bih ) );

	file_out = fopen( path, "wb" );

	if( file_out == 0 )
	{
		return;
	}
	len = w*h*3;
	bfh.bfType = 'MB';
	bfh.bfSize = sizeof( bfh ) + len + sizeof( BITMAPINFOHEADER );
	bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
	fwrite( &bfh, sizeof(bfh), 1, file_out );

	bih.biSize = sizeof( bih );
	bih.biWidth = w;
	bih.biHeight = -h;
	bih.biPlanes = 1;
	bih.biBitCount = 8*3;

	fwrite( &bih, sizeof(bih), 1, file_out );
	fwrite( buffer, len, 1, file_out );
	fclose(file_out);
}

//////////////////////////////////////////////
void SaveFrame(CDLGVideoDetect *pDlg,AVFrame *pFrame, int width, int height, 
			   long long totalsec,long long iFrame) 
{
	//获取时间。此句不可以提前
	long int ticktime=GetTickCount();

	
#if OPEN_CARDETECT_CODE
	//拷贝数值	
	
	pDlg->VideoCarDetect.totalsec=totalsec;
				
	//拷贝数据
	if(pDlg->m_check_writedb)
	{
		pDlg->VideoCarDetect.writeDBflag=true;
		pDlg->VideoCarDetect.starttime=pDlg->StartTime;
		
		pDlg->VideoCarDetect.camid=pDlg->camid;
		
		strcpy(pDlg->VideoCarDetect.cam_name,pDlg->CamStr.GetBuffer(0));
		
		strcpy(	pDlg->VideoCarDetect.l_ipaddr,pDlg->m_camip.GetBuffer(0));
	}
	else
	{
		pDlg->VideoCarDetect.writeDBflag=false;
		strcpy(pDlg->VideoCarDetect.cam_name,pDlg->filename);
	}

	//颜色LC_VIDEO_FORMAT_I420
	pDlg->VideoCarDetect.Start(LC_VIDEO_FORMAT_BGR24,\
		pFrame->data[0],width, height,pFrame->linesize[0]*height);
				
	if(pDlg->m_check_writedb)
		pDlg->VideoCarDetect.WriteDBResult();
	else
		pDlg->VideoCarDetect.NoWriteDBResult();



#endif
	

	///////////以下用来显示/////////////////////////
	//  unsigned char  buffer[sizeof(BITMAPINFOHEADER) + 1024];
    //    BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	//显示
	BITMAPINFO bmi;
	
	FillBitmapInfo( &bmi,width, height,24, 0 );
		  
	::StretchDIBits(
		pDlg->playwin_hdc,
		0,0,
		pDlg->playwin_rect.right- pDlg->playwin_rect.left,
		pDlg->playwin_rect.bottom - pDlg->playwin_rect.top,
		0,0,width, height,
		pFrame->data[0], &bmi, DIB_RGB_COLORS, SRCCOPY );
	
	//抓图
	if(pDlg->cutpic)
	{
		char pathstr[260];
		sprintf(pathstr,"%s\\[%s]%d-%02d-%02d-%d.bmp",
					DlgSetSystem.m_path_capbmp.GetBuffer(0),
					pDlg->filename,
					totalsec/3600,(totalsec % 3600)/60,totalsec%60,
					iFrame);

		ZOGDramBMP(pathstr,pFrame->data[0],width, height);
		pDlg->cutpic=false;
	}

	//速度控制
	if(pDlg->speedflag)
	{
		ticktime=1000/pDlg->fps-(GetTickCount()-ticktime);
		if(ticktime>0)
			Sleep(ticktime);
	}


}


int VideoPlay(char * filePath,CDLGVideoDetect *pDlg) 
{
	AVFormatContext *pFormatCtx;
	unsigned int             i;
	int videoStream;
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
	if(pFrame==NULL)
	{
		// Close the codec
		avcodec_close(pCodecCtx);
		av_close_input_file(pFormatCtx);
		return -1;
	}

	// Allocate an AVFrame structure
	pFrameRGB=avcodec_alloc_frame();
	if(pFrameRGB==NULL)
	{
		av_free(pFrame);
		avcodec_close(pCodecCtx);
		// Close the video file
		av_close_input_file(pFormatCtx);
		return -1;
	}
	// Determine required buffer size and allocate buffer
	numBytes=avpicture_get_size(PIX_FMT_BGR24 , pCodecCtx->width,pCodecCtx->height);
	buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	
	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_BGR24,pCodecCtx->width, pCodecCtx->height);

	// 计算总时长
	unsigned long long  tns, thh, tmm, tss,us;
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


	unsigned long long totalframe=pCodecCtx->frame_number;
	if(0 == totalframe )
		totalframe=pFormatCtx->streams[videoStream]->duration/3600;

	if(0!=pCodecCtx->time_base.num)
		//计算的帧数
		pDlg->fps=pCodecCtx->time_base.den/pCodecCtx->time_base.num;
	else
		pDlg->fps=pFormatCtx->streams[videoStream]->duration/3600/(tns+us/100.0+1)+1;

	//设置进度条
	//pDlg->m_slider.SetRange(0,total,TRUE);
	//秒模式
	pDlg->m_slider.SetRange(0,tns,TRUE);
	pDlg->m_slider.ClearSel(TRUE);
	pDlg->m_slider.SetPos(0);
	//重设跳转地
	pDlg->sliderSeekflag=false;
	pDlg->sliderSeek=0;
	//速度设置
	pDlg->speedflag=true;

	char time[64]="";
	_stprintf(time,_T("%02I64u:%02I64u:%02I64u"),thh,tmm,tss);
	pDlg->GetDlgItem(IDC_STATIC_ENDTIME)->SetWindowText(time);
	unsigned long long nowtime;
	unsigned long long nowtimeshow=0;

	//定位到N帧 关键 lStart/fps= 秒 
//	int lStart=390;
//	av_seek_frame(pFormatCtx, -1, lStart*AV_TIME_BASE, AVSEEK_FLAG_BACKWARD);
//	av_seek_frame(pFormatCtx, -1, lStart/fps*AV_TIME_BASE, AVSEEK_FLAG_BACKWARD);

 // avcodec_flush_buffers(pFormatCtx->streams[videoStream]->codec);
   // avcodec_flush_buffers(pFormatCtx->streams[audio_stream]->codec);

	// Read frames and save first five frames to disk

	while(av_read_frame(pFormatCtx, &packet)>=0) 
	{
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


				/////////////////////////////////////////////////////////////
				//第N帧
			//	if(0==packet.dts%25) //少刷新几次。防止无法跳转
			//		pDlg->m_slider.SetPos(packet.dts);
			//帧模式
			//	pDlg->m_slider.SetSelection(0,packet.dts);
			//	pDlg->m_slider.SetRange(0,total,TRUE);
				//当前时间
				if(0==pFormatCtx->streams[videoStream]->first_dts)
					nowtime=packet.dts/pDlg->fps;
				else
					//仅在特定情况下使用 需要重新计算
					nowtime=(packet.dts-(pFormatCtx->streams[videoStream]->first_dts))/3600/pDlg->fps;

				_stprintf(time,_T("%02I64u:%02I64u:%02I64u"),
					nowtime/3600,(nowtime % 3600)/60,nowtime%60);
				pDlg->GetDlgItem(IDC_STATIC_STARTTIME)->SetWindowText(time);
				//秒模式
				pDlg->m_slider.SetSelection(0,nowtime);
				pDlg->m_slider.SetRange(0,tns,TRUE);
				if(0==packet.dts%(pDlg->fps))
					pDlg->m_slider.SetPos(nowtime);
				/////////////////////////////////////////////////////////////

				//传帧到回调里
				SaveFrame(pDlg,pFrameRGB, pCodecCtx->width, pCodecCtx->height,nowtime,packet.dts);

				//增加这句来释放
				sws_freeContext(img_convert_ctx);

			}
		}
		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
			
		//跳转
		if(pDlg->sliderSeekflag)
		{
			struct AVRational tempq={1, AV_TIME_BASE};
			//sliderSeek为秒;如果为帧 (sliderSeek/fps)
			int64_t t=pDlg->sliderSeek*AV_TIME_BASE;

			int64_t seek_target;
			//换算时间
			seek_target= av_rescale_q(pDlg->sliderSeek*AV_TIME_BASE, tempq, pFormatCtx->streams[videoStream]->time_base);
			//跳转
			av_seek_frame(pFormatCtx, videoStream, seek_target+pFormatCtx->streams[videoStream]->first_dts, AVSEEK_FLAG_BACKWARD);

			avcodec_flush_buffers(pFormatCtx->streams[videoStream]->codec);
			pDlg->sliderSeekflag=false;
		}

		//停止播放
		if(PLAY_FLAG_STOP == pDlg->playflag)	
			break;
		//暂停播放
		while(PLAY_FLAG_PAUSE == pDlg->playflag)
		{
			Sleep(10);
		}
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
	CDLGVideoDetect *pDlg= (CDLGVideoDetect*)lpParameter;

	if(VideoPlay(pDlg->m_edit_file.GetBuffer(0),pDlg) <0)
		pDlg->MessageBox("播放文件失败");
	
	if(	PLAY_FLAG_STOP != pDlg->playflag)
	{
		pDlg->playflag=PLAY_FLAG_STOP;
		pDlg->EnablePlayState();
	}
	
	return 0;
}


////////////////////////////////////////////////////
BOOL CDLGVideoDetect::OnInitDialog()
{
	CDialog::OnInitDialog();

	playwin_hdc=m_playwin.GetDC()->GetSafeHdc();
	m_playwin.GetClientRect(&playwin_rect);

	SetStretchBltMode(playwin_hdc, COLORONCOLOR );

	//添加设备列表的区域
	for(int i=0;i<DlgMain->DlgDeviceTree.DlgAddDevice.AreaCount;i++)
	{
		if(DlgMain->DlgDeviceTree.DlgAddDevice.AreaList[i].name != NULL)
			m_area.AddString(DlgMain->DlgDeviceTree.DlgAddDevice.AreaList[i].name);
	}
	
	EnablePlayState();


	ListMainInit();


	DlgVDmain=this;

	return TRUE;  // return TRUE  unless you set the focus to a control

}
void CDLGVideoDetect::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDLGVideoDetect::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnButtonStop();

	CDialog::OnCancel();
}


void CDLGVideoDetect::OnButtonFile() 
{
	// TODO: Add your control notification handler code here
	char szFilter[]="h264 Files (*.avi)|*.avi|h264 Files (*.mp4)|*.mp4|h264 Files (*.mkv)|*.mkv|*.*|*.*||";
	CFileDialog dlg(TRUE,"avi","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_edit_file=dlg.GetPathName();

		filename=strrchr(m_edit_file.GetBuffer(0),'\\');
		filename=filename+1;

		UpdateData(FALSE);
	}
}

void CDLGVideoDetect::OnButtonCutpic() 
{
	// TODO: Add your control notification handler code here
	cutpic=true;
	//防止暂停无法截图
	if(	PLAY_FLAG_PAUSE == playflag )
	{
		playflag=PLAY_FLAG_GOON;
		Sleep(50);
		playflag=PLAY_FLAG_PAUSE;
	}
}

void CDLGVideoDetect::OnButtonSpeed() 
{
	// TODO: Add your control notification handler code here
	if(speedflag)
	{
		speedflag=false;
		GetDlgItem(IDC_BUTTON_SPEED)->SetWindowText("常速播放");
	}
	else
	{
		speedflag=true;
		GetDlgItem(IDC_BUTTON_SPEED)->SetWindowText("全速播放");
	}

}

void CDLGVideoDetect::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	playflag=PLAY_FLAG_STOP;
	Sleep(100);

	if(pthread)
	{
		TerminateThread(pthread,0);
		pthread=NULL;
	}
	EnablePlayState();

#if OPEN_CARDETECT_CODE
	VideoCarDetect.Stop();
#endif
}

void CDLGVideoDetect::OnButtonPause() 
{
	// TODO: Add your control notification handler code here

	switch(playflag)
	{	
	case PLAY_FLAG_START :
	case PLAY_FLAG_GOON :
		playflag =PLAY_FLAG_PAUSE;
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText("继续识别");
		break;
	case PLAY_FLAG_PAUSE :
		playflag =PLAY_FLAG_GOON;
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText("暂停识别");
		break;
	default:break;
	}
}

void CDLGVideoDetect::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	OnButtonStop();
	
	if(m_check_writedb && m_camip.IsEmpty())
	{
		MessageBox("未选择摄像头为空");
		return ;
	}

	if(m_edit_file.IsEmpty() )
	{
		MessageBox("文件名为空");
		return ;
	}
	//设定时间
	CTime	StartT(m_StartMon.GetYear(),m_StartMon.GetMonth(),m_StartMon.GetDay(),
			m_StartHour.GetHour(),m_StartHour.GetMinute(),m_StartHour.GetSecond());

	StartTime=StartT;

	//清空列表
	m_ListCarTotal=0;
	m_list.DeleteAllItems();
	CleanList();

	playflag=PLAY_FLAG_START;
	pthread=CreateThread(NULL,0,PlayThreadPROC,this,0,NULL);

	EnablePlayState();
}

void CDLGVideoDetect::EnablePlayState() 
{
	switch(playflag)
	{	
	case PLAY_FLAG_START :
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CUTPIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SPEED)->EnableWindow(TRUE);
/////////////////////////////////////////////////////
		GetDlgItem(IDC_COMBO_AREA)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_WRITE_DB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		break;
	case PLAY_FLAG_STOP:
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CUTPIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SPEED)->EnableWindow(FALSE);
/////////////////////////////////////////////////////
		GetDlgItem(IDC_CHECK_WRITE_DB)->EnableWindow(TRUE);

		if(m_check_writedb)
		{
			GetDlgItem(IDC_COMBO_AREA)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_CAM)->EnableWindow(TRUE);
			GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(TRUE);
			GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_COMBO_AREA)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_CAM)->EnableWindow(FALSE);
			GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(FALSE);
			GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		}

		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText("暂停识别");
		GetDlgItem(IDC_BUTTON_SPEED)->SetWindowText("全速播放");
		break;
	default:break;
	}
}
//设置摄像头名称
void CDLGVideoDetect::OnCloseupComboArea() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int areai=m_area.GetCurSel();
	if(-1==areai)
	{
		//全删
		m_cam.ResetContent();
		return ;
	}
	//获取区域字符串
	
	m_area.GetLBText(areai,AreaStr);
	
	//全删
	m_cam.ResetContent();
	m_camip.Empty();
	//重新添加
	for(int i=0;i<DlgMain->DlgDeviceTree.iptotal;i++)
	{
		//==当前选择的区域名
		if(DlgMain->DlgDeviceTree.iplist[i].area == AreaStr )
			m_cam.AddString(DlgMain->DlgDeviceTree.iplist[i].name ); 
	}
	UpdateData(FALSE);
}

void CDLGVideoDetect::OnCloseupComboCam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int cami=m_cam.GetCurSel();
	if(-1==cami)
	{
		//全删
		m_camip.Empty();
		return ;
	}
	//获取区域字符串
	m_cam.GetLBText(cami,CamStr);
	
	//全删
	m_camip.Empty();
	//重新添加
	for(int i=0;i<DlgMain->DlgDeviceTree.iptotal;i++)
	{
		//==当前选择的区域名
		if(DlgMain->DlgDeviceTree.iplist[i].area ==AreaStr &&
			DlgMain->DlgDeviceTree.iplist[i].name == CamStr)
		{
			m_camip=DlgMain->DlgDeviceTree.iplist[i].ip;
			camid=DlgMain->DlgDeviceTree.iplist[i].camID;
			break;
		}
	}
	
	UpdateData(FALSE);
}


void CDLGVideoDetect::OnCheckWriteDb() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_check_writedb)
	{
		GetDlgItem(IDC_COMBO_AREA)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_AREA)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
	}
}

void CDLGVideoDetect::ListMainInit(void)
{
#if ALLTAB_DETECT_CAR_MODE

	m_list.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 40);
	m_list.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_list.InsertColumn(2, _T("车牌号"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(3, _T("置信度"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(4, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(5, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(6, _T("车牌类型"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(7, _T("车身颜色"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(8, _T("黑名单"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(9, _T("图片路径"), LVCFMT_LEFT, 0);
	m_list.InsertColumn(10, _T("服务器模式"), LVCFMT_LEFT, 0);

#else

	m_list.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 40);
	m_list.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_list.InsertColumn(2, _T("车牌号"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(3, _T("置信度"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(4, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(5, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(6, _T("黑名单"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(7, _T("图片路径"), LVCFMT_LEFT, 0);
	m_list.InsertColumn(8, _T("服务器模式"), LVCFMT_LEFT, 0);

#endif
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

}

void CDLGVideoDetect::CleanList(void)
{
	int total;
	char str[260];
	total=m_list.GetItemCount();
	
	//网络保存 则一并删除文件
	if(DlgSetSystem.m_check_savenet)
	{
		for(int i=0;i<total;i++)
		{
#if ALLTAB_DETECT_CAR_MODE
			//汽车
			m_list.GetItemText(i,10,str,260);
#else
			//电动车
			m_list.GetItemText(i,8,str,260);
#endif	
			//本地模式就不用删
			if(0==strcmp(str,"否"))
				continue;
			
			//删除文件
#if ALLTAB_DETECT_CAR_MODE
			//汽车
			m_list.GetItemText(i,9,str,260);
#else
			//电动车
			m_list.GetItemText(i,7,str,260);
#endif	
			DeleteFile(str);
			
		}	
	}
}

//双击显示大图
void CDLGVideoDetect::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];

#if ALLTAB_DETECT_CAR_MODE
//汽车
	m_list.GetItemText(pNMIA->iItem,9,str,260);
#else
//电动车
	m_list.GetItemText(pNMIA->iItem,7,str,260);
#endif

	//ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	CDLGpictureView dlgPicView;

	char *p=strrchr(str,'\\');
	if(p!=NULL)
		p++;
	else
		return ;

	dlgPicView.Titlestr=p;

	dlgPicView.srcfile=str;
	dlgPicView.DoModal();

	*pResult = 0;
}
//NMTVCUSTOMDRAW
//高亮
void CDLGVideoDetect::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)pNMHDR;
	*pResult=CDRF_DODEFAULT;

	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			*pResult=CDRF_NOTIFYITEMDRAW;
			return;
		}
	case CDDS_ITEMPREPAINT:
		{
			*pResult=CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		{
			char str[260];
			COLORREF clrNewTextColor,clrNewBkColor;
			int nItem=static_cast<int>(lplvcd->nmcd.dwItemSpec);

#if ALLTAB_DETECT_CAR_MODE
			//汽车
			m_list.GetItemText(nItem,8,str,260);
#else
			//电动车
			m_list.GetItemText(nItem,6,str,260);
#endif
		//	lplvcd->clrTextBk=RGB(232,248,254);
			if(NULL!=strstr(str,"是"))
			{
				clrNewBkColor=RGB(255,0,0);//红底白字
				lplvcd->clrTextBk=clrNewBkColor;
				clrNewTextColor=RGB(255,255,255);	
				lplvcd->clrText=clrNewTextColor;
			}
			*pResult=CDRF_DODEFAULT;
			return;
		}
	}
}

void CDLGVideoDetect::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	UpdateData(TRUE);

	sliderSeekflag=true;
	sliderSeek=m_slider.GetPos();
}

