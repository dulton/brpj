// video2detect.h : main header file for the VIDEO2DETECT application
//

#if !defined(AFX_VIDEO2DETECT_H__72A29EBA_E73A_484A_879F_3C861DC8CEEE__INCLUDED_)
#define AFX_VIDEO2DETECT_H__72A29EBA_E73A_484A_879F_3C861DC8CEEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVideo2detectApp:
// See video2detect.cpp for the implementation of this class
//

class CVideo2detectApp : public CWinApp
{
public:
	CVideo2detectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideo2detectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVideo2detectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEO2DETECT_H__72A29EBA_E73A_484A_879F_3C861DC8CEEE__INCLUDED_)
