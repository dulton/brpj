// DataBaseConfig.h : main header file for the DATABASECONFIG application
//

#if !defined(AFX_DATABASECONFIG_H__532D545C_D578_47B8_BFC5_E20ECDCA3639__INCLUDED_)
#define AFX_DATABASECONFIG_H__532D545C_D578_47B8_BFC5_E20ECDCA3639__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDataBaseConfigApp:
// See DataBaseConfig.cpp for the implementation of this class
//

class CDataBaseConfigApp : public CWinApp
{
public:
	CDataBaseConfigApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataBaseConfigApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDataBaseConfigApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASECONFIG_H__532D545C_D578_47B8_BFC5_E20ECDCA3639__INCLUDED_)
