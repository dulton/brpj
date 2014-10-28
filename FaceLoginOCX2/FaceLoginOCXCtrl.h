#pragma once

// FaceLoginOCXCtrl.h : Declaration of the CFaceLoginOCXCtrl ActiveX Control class.

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
		dispidFaceMacth = 1L,
		dispidFaceEnroll = 2L,
		dispidGetCamNum = 3L,
	};

public:
	CString strHost;
	int FaceDB;
	int macthScore;//比对阈值

	CString MatchUser;
	int MatchSysID;
	BSTR StartFaceMacth(LPCTSTR strFaceServer,int sysID,LPCTSTR user,LPCTSTR password);		//返回1比对成功，返回0比对失败
	CString EnrollUser;
	int EnrollSysID;
	int StartFaceEnroll(LPCTSTR strFaceServer,int sysID,LPCTSTR user,LPCTSTR password);		//返回注册后的faceID
	int GetCamNum(void);
	void ShowOcxLog(CString log);
};

