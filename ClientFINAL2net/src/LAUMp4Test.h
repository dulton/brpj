// LAUMp4Test.h : main header file for the LAUMP4TEST application
//

#if !defined(AFX_LAUMP4TEST_H__4EAD1384_0D57_4E68_BF3E_1EA8B853CCC8__INCLUDED_)
#define AFX_LAUMP4TEST_H__4EAD1384_0D57_4E68_BF3E_1EA8B853CCC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLAUMp4TestApp:
// See LAUMp4Test.cpp for the implementation of this class
//

class CLAUMp4TestApp : public CWinApp
{
public:
	CLAUMp4TestApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLAUMp4TestApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLAUMp4TestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
private:
	HINSTANCE m_hLonDll;
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUMP4TEST_H__4EAD1384_0D57_4E68_BF3E_1EA8B853CCC8__INCLUDED_)
