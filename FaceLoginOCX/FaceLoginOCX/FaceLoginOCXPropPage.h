#pragma once

// FaceLoginOCXPropPage.h : Declaration of the CFaceLoginOCXPropPage property page class.


// CFaceLoginOCXPropPage : See FaceLoginOCXPropPage.cpp for implementation.

class CFaceLoginOCXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CFaceLoginOCXPropPage)
	DECLARE_OLECREATE_EX(CFaceLoginOCXPropPage)

// Constructor
public:
	CFaceLoginOCXPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_FACELOGINOCX };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

