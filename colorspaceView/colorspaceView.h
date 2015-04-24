// colorspaceView.h : main header file for the COLORSPACEVIEW application
//

#if !defined(AFX_COLORSPACEVIEW_H__09CD5920_D8E1_4C92_8A8E_B9C0CDAC2C52__INCLUDED_)
#define AFX_COLORSPACEVIEW_H__09CD5920_D8E1_4C92_8A8E_B9C0CDAC2C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CColorspaceViewApp:
// See colorspaceView.cpp for the implementation of this class
//

class CColorspaceViewApp : public CWinApp
{
public:
	CColorspaceViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorspaceViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CColorspaceViewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSPACEVIEW_H__09CD5920_D8E1_4C92_8A8E_B9C0CDAC2C52__INCLUDED_)
