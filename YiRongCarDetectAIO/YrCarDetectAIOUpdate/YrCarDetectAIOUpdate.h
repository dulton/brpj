// YrCarDetectAIOUpdate.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CYrCarDetectAIOUpdateApp:
// See YrCarDetectAIOUpdate.cpp for the implementation of this class
//

class CYrCarDetectAIOUpdateApp : public CWinApp
{
public:
	CYrCarDetectAIOUpdateApp();
void GetCommandLineAndToArgv(int &argc,CStringArray &argv);
// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CYrCarDetectAIOUpdateApp theApp;