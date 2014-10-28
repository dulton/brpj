

#ifndef DIRECTSHOW_H
#define DIRECTSHOW_H

#ifdef DIRECTSHOW_EXPORTS
#define DIRECTSHOW_API __declspec(dllexport)
#else
#define DIRECTSHOW_API __declspec(dllimport)
#endif

#include <atlbase.h>
#include "qedit.h"
#include "dshow.h"

#define MYFREEMEDIATYPE(mt)	{if ((mt).cbFormat != 0)		\
					{CoTaskMemFree((PVOID)(mt).pbFormat);	\
					(mt).cbFormat = 0;						\
					(mt).pbFormat = NULL;					\
				}											\
				if ((mt).pUnk != NULL)						\
				{											\
					(mt).pUnk->Release();					\
					(mt).pUnk = NULL;						\
				}}	

extern "C" {

	DIRECTSHOW_API int GetCameraCount(void);

	DIRECTSHOW_API int GetCameraName(int nCamID, char* sName, int nBufferSize);
		
	DIRECTSHOW_API void CloseCamera();

	DIRECTSHOW_API bool OpenCameraCB(int nCamID, bool bDisplayProperties, LONG * nWidth, LONG * nHeight);

	DIRECTSHOW_API int QueryFrameCB(long * bufferSize, char ** buffer);

}






#endif