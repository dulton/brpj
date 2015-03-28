#include "stdafx.h"
#include "video.h"

#if OPEN_FFMEPG
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
int VideoPlay(TCHAR* filePath,long *sec,int *w,int *h) 
{
	AVFormatContext *pFormatCtx=NULL;
	unsigned int             i;
	int videoStream;
	AVCodecContext  *pCodecCtx=NULL;
	
	// Register all formats and codecs
	//av_register_all();
    // Open video file
	if(av_open_input_file(&pFormatCtx, filePath, NULL, 0, NULL)!=0)
		return -1; // Couldn't open file
	
	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0)
	{
		av_close_input_file(pFormatCtx);
		return -1; // Couldn't find stream information
	}
	
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
	{
		av_close_input_file(pFormatCtx);
		return -1; // Didn't find a video stream
	}
	// Get a pointer to the codec context for the video stream
	pCodecCtx=pFormatCtx->streams[videoStream]->codec;

	if(0==pCodecCtx->width || 0==pCodecCtx->height)
	{
		av_close_input_file(pFormatCtx);
		fprintf(stderr, "codec w h unkown!\n");
		return -1;
	}
	/*
	// 计算总时长
	unsigned long long tns, thh, tmm, tss,us;
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
	*/

	(*sec)=pFormatCtx->duration / AV_TIME_BASE;
	(*w)=pCodecCtx->width ;
	(*h)=pCodecCtx->height;

	// Close the video file
	av_close_input_file(pFormatCtx);
	
	pFormatCtx=NULL;
	pCodecCtx=NULL;

	return 0;
}

void VideoInit(void)
{
	av_register_all();
}
int AudioPlay(TCHAR* filePath,long *sec) 
{
	AVFormatContext *pFormatCtx=NULL;
	// Register all formats and codecs
	//av_register_all();
	// Open video file
	if(av_open_input_file(&pFormatCtx, filePath, NULL, 0, NULL)!=0)
		return -1; // Couldn't open file

	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0)
	{
		av_close_input_file(pFormatCtx);
		return -1; // Couldn't find stream information
	}

	// Dump information about file onto standard error
	dump_format(pFormatCtx, 0, filePath, 0);
	/*
	// 计算总时长
	unsigned long long tns, thh, tmm, tss,us;
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
	*/

	(*sec)=pFormatCtx->duration / AV_TIME_BASE;

	// Close the video file
	av_close_input_file(pFormatCtx);

	pFormatCtx=NULL;

	return 0;
}


#endif

long long filesize(TCHAR* filePath)
{
	FILE *fp;

	long long s=-1;
	fp=fopen(filePath,"rb");
	if(fp)
	{
		_fseeki64(fp,0,SEEK_END );
		s=_ftelli64(fp);
		fclose(fp);
	}
	return s;
}

void CHAR2UTF8(const char *str,char *utf8,unsigned long int len)
{
	wchar_t * wbuff=(wchar_t *)calloc(len*2+4/*+4*/,sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, len+2);  
	// buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);  
	//  utf8 = new char[buffLen+1];  
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)utf8, len*2+4, 0, 0); 
	free(wbuff);
	wbuff=NULL;
}

void UTF82CHAR(const char *utf8,char *str,unsigned long int len)
{
	wchar_t * wbuff=(wchar_t *)calloc(len*2+4,sizeof(wchar_t));
	//	int buffLen = 0;  
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wbuff, len+2);  
	//	buffLen = WideCharToMultiByte(CP_ACP, 0, wbuff, -1, NULL, 0, 0, 0);  
	//	m_gb2312 = new char[buffLen+1];  
	WideCharToMultiByte(CP_ACP, 0, wbuff, -1, (LPSTR)str, len*2+4, 0, 0);  
	free(wbuff);
	wbuff=NULL;
}

//将字符串放到剪切板
int AppendBufferToClipboard( char *cBuffer, int nSize )
{
	HGLOBAL hClipboard = NULL;
	char *pClipBuf = NULL;

	if( OpenClipboard( NULL ) )
	{ 
		hClipboard = GlobalAlloc( GMEM_DDESHARE, nSize+1 );
		pClipBuf = (char*)GlobalLock(hClipboard);
		memset( pClipBuf, 0, nSize+1 );
		memcpy( pClipBuf, cBuffer, nSize );
		GlobalUnlock( hClipboard );
		SetClipboardData( CF_TEXT, hClipboard );
		CloseClipboard();
	}
	else
	{
		return -1;
	}

	return 0;
}
//从剪切板获取字符串
int GetBufferFromClipboard( char *cBuffer, int nMaxSize )
{
	int nNeedSize = 0;
	HANDLE hClipboard = NULL;
	char *pClipBuf = NULL;

	if ( OpenClipboard( NULL ) )
	{
		hClipboard = GetClipboardData( CF_TEXT );
		nNeedSize = GlobalSize( hClipboard );
		if ( nNeedSize > nMaxSize+1 )
		{
			return -1;
		}

		pClipBuf = (char*)GlobalLock( hClipboard );
		memset( cBuffer, 0, nMaxSize );
		memcpy( cBuffer, pClipBuf, nNeedSize );
		GlobalUnlock( hClipboard );
		CloseClipboard();

		return nNeedSize;

	}
	else
	{
		return 0;
	}

	return -1;
}
//清空剪切板
int ClearClipboradBuffer(void)
{
	HGLOBAL hClipboard = NULL;

	if( OpenClipboard( NULL ) )
	{ 
		hClipboard = GetClipboardData( CF_TEXT );
		GlobalFree( hClipboard );
		EmptyClipboard();
		CloseClipboard();
		return 0;
	}
	else
	{
		return -1;
	}
}