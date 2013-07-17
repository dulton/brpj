#ifndef __READ_CONFIG_TXT_H
#define __READ_CONFIG_TXT_H
#ifndef _AFXDLL
 #define _AFXDLL
#endif

#include <afx.h>

class ReadConfigTxt
{
public:
//	ReadConfigTxt();
	//virtual ~ReadConfigTxt();
public:
	TCHAR Ip[256];
	TCHAR Port[256];
	TCHAR User[256];
	TCHAR Psw[256];
	TCHAR DataBaseName[256];
public:
	BOOL ReadFile(TCHAR* FileName);

};

#endif