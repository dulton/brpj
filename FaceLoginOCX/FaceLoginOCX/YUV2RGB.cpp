
#include "stdafx.h"
#include "YUV2RGB.h"
#include "ijl.h"
#pragma comment(lib,"ijl15l.lib")

/************************************************************************
Function EncodeToJPEGBuffer:RGB图像数据压缩为JPEG文件保存
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
		DWORD *lpjpgBufferSize)
{
	BOOL	bres;
	IJLERR	jerr = IJL_OK;
	DWORD	rawBufSize;
	JPEG_CORE_PROPERTIES jcprops;

	*lpjpgBufferSize=0;

	bres = TRUE;
	jerr = ijlInit(&jcprops);
	if(jerr != IJL_OK){
		printf("INIT IJL ERR -- %s\n", ijlErrorStr(jerr));
		bres = FALSE;
		return bres;
	}

	rawBufSize = dwWidth * dwHeight * 3;

	jcprops.DIBWidth = dwWidth;
	jcprops.DIBHeight = dwHeight;
	jcprops.DIBBytes = lpRawBuffer;
	jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(jcprops.DIBWidth,3);
	jcprops.DIBChannels = 3;
	jcprops.DIBColor =IJL_BGR;
	//jcprops.DIBSubsampling = IJL_422;

	jcprops.JPGWidth = dwWidth;
	jcprops.JPGHeight = dwHeight;
	jcprops.JPGFile = NULL;
	jcprops.JPGBytes = lpJPEGBuffer;
	jcprops.JPGSizeBytes = rawBufSize;
	jcprops.JPGChannels = 3;
	jcprops.JPGColor = IJL_YCBCR;
	jcprops.JPGSubsampling = IJL_411;
	jcprops.jquality = 80;

	jerr = ijlWrite(&jcprops, IJL_JBUFF_WRITEWHOLEIMAGE);
	if(jerr != IJL_OK){
		printf("ijlWrite() failed -- %s\n", ijlErrorStr(jerr));
		bres = FALSE;
		goto ENCODE_ERROR;
	}else{
		*lpjpgBufferSize = jcprops.JPGSizeBytes;
	}
ENCODE_ERROR:
	ijlFree(&jcprops);
	return bres;
}
