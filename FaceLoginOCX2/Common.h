
#ifndef _COMMON_H_
#define _COMMON_H_

#include "DirectShow.h"
#pragma comment(lib,"DirectShow.lib") 

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
	char * EncodeMd5(char * input);


	bool InitialDevice(CComboBox &m_cbDevice, LONG * w, LONG * h);
	int GetCamNum();
	CString GetReg(CString lpValueKey);
	void SetReg(CString lpValueKey, CString lpValueName);

	void DrawCtrlImage(CStatic * m_picBox, BITMAPINFO bmpInfo,
							char * buffer, int bufferSize,
							int face_Count, CRect *face_Rect_List, 
							CRect rect,float scale);

	void DrawFaceImageMin(CStatic *m_picBox,unsigned char *image,unsigned long int size);

void CCommon::DrawFaceImage(CStatic *m_picBox,CBitmap    *cbitmap,    BITMAP *bitbmp);
	CRect SetDrawSize(CStatic * m_picBox,CRect old_DrawRect,int bmpw,int bmph,float *scale) ;

};



#endif