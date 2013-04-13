// BarcodeRecord.h : main header file for the BARCODERECORD application
//

#if !defined(AFX_BARCODERECORD_H__25282BF8_3915_4853_9DE3_949AFFC7235E__INCLUDED_)
#define AFX_BARCODERECORD_H__25282BF8_3915_4853_9DE3_949AFFC7235E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBarcodeRecordApp:
// See BarcodeRecord.cpp for the implementation of this class
//

class CBarcodeRecordApp : public CWinApp
{
public:
	CBarcodeRecordApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcodeRecordApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBarcodeRecordApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define  ZOG_MAX_PATH_STR 260
void Set_AllPath(void);
TCHAR* Language_GetIniPath(void);
CString Language_ConvertString(CString strText);
void Language_SetWndStaticText(CWnd * pWnd);
void Language_ConvertComboBox(CComboBox &stuComboBox);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODERECORD_H__25282BF8_3915_4853_9DE3_949AFFC7235E__INCLUDED_)
