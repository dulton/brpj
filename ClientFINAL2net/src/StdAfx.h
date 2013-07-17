// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D42DCE2C_9A37_46D5_BA8F_01628E23E5CB__INCLUDED_)
#define AFX_STDAFX_H__D42DCE2C_9A37_46D5_BA8F_01628E23E5CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>
#include <mmsystem.h>

#include "../lib/launet.h"
#define OVERLAY_KEY	RGB(192,0,192)

#define MAX_CHANNEL		4
#define ALARM_IN_NUM	4
#define	ALARM_OUT_NUM	4
#define	MAX_VIDEOWND	16
#define PTZ_STA_AUTO	0x10000
#define PTZ_STA_LEFT	0x01000
#define PTZ_STA_RIGHT	0x00100
#define PTZ_STA_UP		0x00010
#define PTZ_STA_DOWN	0x00001

#define WM_MSG_AUDIOCAPEND			WM_USER + 100

#pragma comment(lib, "winmm.lib")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_STDAFX_H__D42DCE2C_9A37_46D5_BA8F_01628E23E5CB__INCLUDED_)
