
#ifndef _YUV2RGBIPL_H_
#define _YUV2RGBIPL_H_

#include "ijl.h"
#pragma comment(lib, "ijl15l.lib")

bool YV12_to_RGB24(unsigned char* pYV12, unsigned char* pRGB24,int iWidth, int iHeight);
bool I420_to_RGB24(unsigned char* pI420, unsigned char* pRGB24,int iWidth, int iHeight);

bool YV12toRGB24UD(unsigned char* pYV12, unsigned char* pRGB24, int iWidth, int iHeight,unsigned char *RGBW);


void YUV4202RGB(unsigned char *yuv_image,unsigned char* rgb_image,int iWidth, int iHeight);
void YV122RGB(unsigned char *yuv_image,unsigned char* rgb_image,int iWidth, int iHeight);

void ZOGDramBMP(char *path,unsigned char *buffer, int w, int h);
/************************************************************************
Function CompressRGBToJPEG:RGB图像数据压缩为JPEG文件保存
  Input:
  VIDEO_FRAMEINFO *yuv_image        YUV图像数据
  unsigned char* rgb_image			RGB图像数据
return:
        无   
************************************************************************/
//void YUV2RGB(VIDEO_FRAMEINFO *yuv_image,unsigned char* rgb_image);

/************************************************************************
Function CompressRGBToJPEG:RGB图像数据压缩为JPEG文件保存
  Input:BYTE *lpImageRGB        RGB图像数据区
  int   originalWidth     原始图像宽度
  int   originalHeight    原始图像高度
  int   jpegQuality       JPEG压缩质量[1-100]
  char* jpegFileName      JPEG文件保存的名称 
  int   isNeedReversal    是否需要翻转
  int   isResizeImage     是否需要缩放图像的尺寸
return:
        成功返回0值,异常返回其它整数;
Update:
  Author         Date              Ver      Remark  
  Shimingjie     2005/01/06        1.0      Create         
************************************************************************/
int CompressRGBToJPEG(unsigned char *lpImageRGB,int originalWidth,int originalHeight,
					  int jpegQuality,char* jpegFileName,int isNeedReversal,int isResizeImage);

int CompressRGBAToJPEG(unsigned char *lpImageRGB,int originalWidth,int originalHeight,
					   int jpegQuality,char* jpegFileName,int isNeedReversal,int isResizeImage);

/************************************************************************
Function EncodeToJPEGBuffer:RGB图像数据压缩为JPEG图像数据
  Input:
	  BYTE *lpRawBuffer			RGB图像数据区
	  DWORD   dwWidth			原始图像宽度
	  DWORD   dwWidth			原始图像高度
	  BYTE *lpJPEGBuffer		JPEG图像数据区
	  DWORD *lpjpgBufferSize	JPEG图像大小

return:
        成功返回0值,异常返回其它整数;   
************************************************************************/
BOOL EncodeToJPEGBuffer(
		BYTE *lpRawBuffer, 
		DWORD dwWidth, 
		DWORD dwHeight,  
		BYTE *lpJPEGBuffer,
		DWORD *lpjpgBufferSize);


BOOL JPEGFile2BMP(char* lpszFilename,
				  unsigned char* RGBdata, int *w,int *h,int *channel,long *wstep,long *size);

BOOL JPEGData2BMP(unsigned char *Jpegdata,long JpegSize,
				   unsigned char* RGBdata, int *w,int *h,int *channel,long *wstep,long *size);

void BGRA32toRGB24(
				  unsigned char *RGB32data,
				  unsigned char *RGB24data,
				  int w,
				  int h,
				  int RGB32wstep,
				  int RGB24wstep);

#endif