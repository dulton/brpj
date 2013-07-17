
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

	bool InitialDevice(CComboBox &m_cbDevice, LONG * w, LONG * h);
	int GetCamNum();
	CString GetReg(CString lpValueKey);
	void DrawCtrlImage(CStatic * m_picBox, BITMAPINFO bmpInfo, char * buffer, int bufferSize);
};



#endif