#pragma once

// FaceLoginOCXCtrl.h : Declaration of the CFaceLoginOCXCtrl ActiveX Control class.
#include "Common.h"
#include "FrmFaceMatch.h"

/////////////ZOGNA//////DOWN////////安全//否则JSP无法调用////////
#include "comcat.h"
#include <objsafe.h>
/////////////ZOGNA////////UP//////安全//否则JSP无法调用////////


// CFaceLoginOCXCtrl : See FaceLoginOCXCtrl.cpp for implementation.

class CFaceLoginOCXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CFaceLoginOCXCtrl)

// Constructor
public:
	CFaceLoginOCXCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CFaceLoginOCXCtrl();

	DECLARE_OLECREATE_EX(CFaceLoginOCXCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CFaceLoginOCXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CFaceLoginOCXCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CFaceLoginOCXCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidGetCamNum = 1L,
		dispidGetFaceImage = 2L,
		dispidGetImageTest = 3L,
		dispidStartCamera = 4L,
		dispidEndCamera= 5L,
	};
public:
	CCommon m_Common;
public:
	CFrmFaceMatch faceMatch;
	int GetCamNum();
	char *GetFaceImage(int ImageNum);
	BSTR GetImageTest();
	int StartCamera(void);
	int EndCamera(void);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnClose(DWORD dwSaveOption);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);

protected:

};

