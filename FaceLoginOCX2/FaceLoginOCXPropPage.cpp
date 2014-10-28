// FaceLoginOCXPropPage.cpp : Implementation of the CFaceLoginOCXPropPage property page class.

#include "stdafx.h"
#include "FaceLoginOCX.h"
#include "FaceLoginOCXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFaceLoginOCXPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CFaceLoginOCXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CFaceLoginOCXPropPage, "FACELOGINOCX.FaceLoginOCXPropPage.1",
	0x92cb4f16, 0x9f8b, 0x44b2, 0xba, 0x5, 0xb9, 0x65, 0x99, 0xf5, 0x57, 0x9e)



// CFaceLoginOCXPropPage::CFaceLoginOCXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CFaceLoginOCXPropPage

BOOL CFaceLoginOCXPropPage::CFaceLoginOCXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FACELOGINOCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CFaceLoginOCXPropPage::CFaceLoginOCXPropPage - Constructor

CFaceLoginOCXPropPage::CFaceLoginOCXPropPage() :
	COlePropertyPage(IDD, IDS_FACELOGINOCX_PPG_CAPTION)
{
}



// CFaceLoginOCXPropPage::DoDataExchange - Moves data between page and properties

void CFaceLoginOCXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CFaceLoginOCXPropPage message handlers
