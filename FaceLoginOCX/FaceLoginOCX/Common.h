
#ifndef _COMMON_H_
#define _COMMON_H_

#include "DirectShow.h"
#pragma comment(lib,"DirectShow.lib") 
#include "FaceCloudDLL.h"

#pragma once

class CCommon
{
public:
	CCommon(void);
	~CCommon(void);

public:
	CString Char2CString(char * str);
	char * CString2Char(CString str);
	CString GetSystemFolder();


	bool InitialDevice(CComboBox &m_cbDevice, LONG * w, LONG * h);
	int GetCamNum();
	CString GetReg(CString lpValueKey);

	void DrawCtrlImage(CStatic * m_picBox, BITMAPINFO bmpInfo,
							char * buffer, int bufferSize,
							int list_size, RwFaceRect *face_rect_list,
							CRect old_DrawRect,float scale);

	CRect SetDrawSize(CStatic * m_picBox,CRect old_DrawRect,int bmpw,int bmph,float *scale) ;

};



#endif