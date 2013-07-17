// en_resource.h : main header file for the EN_RESOURCE DLL
//

#if !defined(AFX_EN_RESOURCE_H__E2A0CDF7_40B0_4199_A861_D15F35E593FC__INCLUDED_)
#define AFX_EN_RESOURCE_H__E2A0CDF7_40B0_4199_A861_D15F35E593FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEn_resourceApp
// See en_resource.cpp for the implementation of this class
//

class CEn_resourceApp : public CWinApp
{
public:
	CEn_resourceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEn_resourceApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEn_resourceApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EN_RESOURCE_H__E2A0CDF7_40B0_4199_A861_D15F35E593FC__INCLUDED_)
