
#ifndef _YUV2RGBIPL_H_
#define _YUV2RGBIPL_H_


void RGBup2down(unsigned char* pRGB24, int iWidth, int iHeight,unsigned char *RGBW);
void RGBup2downOUT(unsigned char* pRGB24, int iWidth, int iHeight,unsigned char *RGBdst);

void RGB32to24(unsigned char *src,unsigned char *dst,long w,long h);


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

BOOL EncodeToJPEGBufferNo(
						  BYTE *lpRawBuffer, 
						  DWORD dwWidth, 
						  DWORD dwHeight,  
						  BYTE *lpJPEGBuffer,
						  DWORD *lpjpgBufferSize);
#endif