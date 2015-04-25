// SmartVillage.h : main header file for the SmartVillage application
//

#if !defined(AFX_SmartVillage_H__4141C2DE_DA24_433F_8F54_ACFBEA7AB0DC__INCLUDED_)
#define AFX_SmartVillage_H__4141C2DE_DA24_433F_8F54_ACFBEA7AB0DC__INCLUDED_

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


#define ZOG_MAX_NAME_STR 32
/////////////////////////////////////////////////////////////////////////////
// CSmartVillageApp:
// See SmartVillage.cpp for the implementation of this class
//

class CSmartVillageApp : public CWinApp
{
public:
	CSmartVillageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmartVillageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSmartVillageApp)
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

#endif // !defined(AFX_SmartVillage_H__4141C2DE_DA24_433F_8F54_ACFBEA7AB0DC__INCLUDED_)
