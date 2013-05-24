// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E931DB0D_8E3F_4BCE_864E_57BB22323E77__INCLUDED_)
#define AFX_STDAFX_H__E931DB0D_8E3F_4BCE_864E_57BB22323E77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//汽车=1 电动车=0
#define ALLTAB_DETECT_CAR_MODE 1
//写入YRVM平台
#define YRVM_PINGTAI_MODE	1

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E931DB0D_8E3F_4BCE_864E_57BB22323E77__INCLUDED_)
