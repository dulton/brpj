// YiRongCarDetectAIO.h : main header file for the YIRONGCARDETECTAIO application
//

#if !defined(AFX_YIRONGCARDETECTAIO_H__4141C2DE_DA24_433F_8F54_ACFBEA7AB0DC__INCLUDED_)
#define AFX_YIRONGCARDETECTAIO_H__4141C2DE_DA24_433F_8F54_ACFBEA7AB0DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


#define SCREEN_1MODE	1
#define SCREEN_4MODE	4
#define SCREEN_9MODE	9
#define SCREEN_16MODE	16

#define ZOG_MAX_PATH_STR 260
#define ZOG_MAX_NAME_STR 32
/////////////////////////////////////////////////////////////////////////////
// CYiRongCarDetectAIOApp:
// See YiRongCarDetectAIO.cpp for the implementation of this class
//

class CYiRongCarDetectAIOApp : public CWinApp
{
public:
	CYiRongCarDetectAIOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYiRongCarDetectAIOApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CYiRongCarDetectAIOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//º”Ω‚√‹
void ZogEnCode(char *src,char *dst);
void ZogDeCode(char *src,char *dst);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YIRONGCARDETECTAIO_H__4141C2DE_DA24_433F_8F54_ACFBEA7AB0DC__INCLUDED_)
