// FaceLoginOCXCtrl.cpp : Implementation of the CFaceLoginOCXCtrl ActiveX Control class.

#include "stdafx.h"
#include "FaceLoginOCX.h"
#include "FaceLoginOCXCtrl.h"
#include "FaceLoginOCXPropPage.h"
#include "base64_codec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFaceLoginOCXCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CFaceLoginOCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_MOVING()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CFaceLoginOCXCtrl, COleControl)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "GetImageTest", dispidGetImageTest, GetImageTest, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "GetFaceImage", dispidGetFaceImage, GetFaceImage, VT_BSTR, VTS_I2)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "GetCamNum", dispidGetCamNum, GetCamNum, VT_I2, VTS_NONE)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "StartCamera", dispidStartCamera, StartCamera, VT_I2, VTS_NONE)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "EndCamera", dispidEndCamera, EndCamera, VT_I2, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CFaceLoginOCXCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CFaceLoginOCXCtrl, 1)
PROPPAGEID(CFaceLoginOCXPropPage::guid)
END_PROPPAGEIDS(CFaceLoginOCXCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CFaceLoginOCXCtrl, "FACELOGINOCX.FaceLoginOCXCtrl.1",
					   0xaefa1f7c, 0xf5a9, 0x44b3, 0x86, 0xb8, 0x53, 0xd5, 0x86, 0xca, 0xaf, 0xab)



					   // Type library ID and version

					   IMPLEMENT_OLETYPELIB(CFaceLoginOCXCtrl, _tlid, _wVerMajor, _wVerMinor)



					   // Interface IDs

					   const IID BASED_CODE IID_DFaceLoginOCX =
{ 0xBB80CCC6, 0x8390, 0x477C, { 0x91, 0xD5, 0x2C, 0xF3, 0x35, 0x48, 0x94, 0x2 } };
const IID BASED_CODE IID_DFaceLoginOCXEvents =
{ 0xF73D1D70, 0x33B6, 0x4CBD, { 0xB8, 0x70, 0xA7, 0xD1, 0x43, 0xA1, 0xA9, 0xAD } };



// Control type information

static const DWORD BASED_CODE _dwFaceLoginOCXOleMisc =
OLEMISC_ACTIVATEWHENVISIBLE |
OLEMISC_SETCLIENTSITEFIRST |
OLEMISC_INSIDEOUT |
OLEMISC_CANTLINKINSIDE |
OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CFaceLoginOCXCtrl, IDS_FACELOGINOCX, _dwFaceLoginOCXOleMisc)


/////////////ZOGNA//////DOWN////////安全//否则JSP无法调用////////
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{

	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (FAILED(hr))
		return hr;

	// Make sure the HKCR\Component Categories\{..catid...}

	// key is registered

	CATEGORYINFO catinfo;
	catinfo.catid = catid;
	catinfo.lcid = 0x0409 ; // english


	// Make sure the provided description is not too long.

	// Only copy the first 127 characters if it is

	int len = wcslen(catDescription);
	if (len>127)
		len = 127;
	wcsncpy(catinfo.szDescription, catDescription, len);
	// Make sure the description is null terminated

	catinfo.szDescription[len] = '\0';

	hr = pcr->RegisterCategories(1, &catinfo);
	pcr->Release();

	return hr;
}


HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	// Register your component categories information.

	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Register this category as being "implemented" by

		// the class.

		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}

HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Unregister this category as being "implemented" by

		// the class.

		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}
BOOL CFaceLoginOCXCtrl::CFaceLoginOCXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
	{
		HRESULT hr = S_OK ;

		// register as safe for scripting

		hr = CreateComponentCategory(CATID_SafeForScripting,
			L"Controls that are safely scriptable");

		if (FAILED(hr))
			return FALSE;

		hr = RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);

		if (FAILED(hr))
			return FALSE;

		// register as safe for initializing

		hr = CreateComponentCategory(CATID_SafeForInitializing,
			L"Controls safely initializable from persistent data");

		if (FAILED(hr))
			return FALSE;

		hr = RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);

		if (FAILED(hr))
			return FALSE;

		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_FACELOGINOCX,
			IDB_FACELOGINOCX,
			afxRegInsertable | afxRegApartmentThreading,
			_dwFaceLoginOCXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	}
	else
	{
		HRESULT hr = S_OK ;

		hr = UnRegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);

		if (FAILED(hr))
			return FALSE;

		hr = UnRegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);

		if (FAILED(hr))
			return FALSE;

		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}
}

/////////////ZOGNA////////UP//////安全//否则JSP无法调用////////
/*
// CFaceLoginOCXCtrl::CFaceLoginOCXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CFaceLoginOCXCtrl

BOOL CFaceLoginOCXCtrl::CFaceLoginOCXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
// TODO: Verify that your control follows apartment-model threading rules.
// Refer to MFC TechNote 64 for more information.
// If your control does not conform to the apartment-model rules, then
// you must modify the code below, changing the 6th parameter from
// afxRegApartmentThreading to 0.

if (bRegister)
return AfxOleRegisterControlClass(
AfxGetInstanceHandle(),
m_clsid,
m_lpszProgID,
IDS_FACELOGINOCX,
IDB_FACELOGINOCX,
afxRegApartmentThreading,
_dwFaceLoginOCXOleMisc,
_tlid,
_wVerMajor,
_wVerMinor);
else
return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}
*/


// CFaceLoginOCXCtrl::CFaceLoginOCXCtrl - Constructor

CFaceLoginOCXCtrl::CFaceLoginOCXCtrl()
{
	InitializeIIDs(&IID_DFaceLoginOCX, &IID_DFaceLoginOCXEvents);
	// TODO: Initialize your control's instance data here.
}



// CFaceLoginOCXCtrl::~CFaceLoginOCXCtrl - Destructor

CFaceLoginOCXCtrl::~CFaceLoginOCXCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CFaceLoginOCXCtrl::OnDraw - Drawing function

void CFaceLoginOCXCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	//	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//	pdc->Ellipse(rcBounds);
	CBrush tempBrush(RGB(0,0,0));
	pdc->FillRect(rcBounds, &tempBrush);
}



// CFaceLoginOCXCtrl::DoPropExchange - Persistence support

void CFaceLoginOCXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CFaceLoginOCXCtrl::OnResetState - Reset control to default state

void CFaceLoginOCXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CFaceLoginOCXCtrl::AboutBox - Display an "About" box to the user

void CFaceLoginOCXCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_FACELOGINOCX);
	dlgAbout.DoModal();
}

int CFaceLoginOCXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	faceMatch.Create(IDD_FACEMATCH,this);
	faceMatch.ShowWindow(SW_SHOW);

	return 0;
}


// CFaceLoginOCXCtrl message handlers
/************************************
* 获取摄像头个数
*************************************/
int CFaceLoginOCXCtrl::GetCamNum()
{
	return m_Common.GetCamNum();
}

/************************************
* 获取人脸图片
*************************************/
char *CFaceLoginOCXCtrl::GetFaceImage(int ImageNum)
{
	if(faceMatch.FaceImage[ImageNum] == NULL)
	{
		return "";
	}
	else
	{
		return faceMatch.FaceImage[ImageNum];
	}
}

/************************************
* base64获取测试
*************************************/
BSTR CFaceLoginOCXCtrl::GetImageTest()
{
	FILE *fp=fopen("D:\\test.bmp","rb");
	if(fp != NULL)
	{
		fseek(fp,0,SEEK_END);
		unsigned long int size = ftell(fp);
		char * buffer= NULL;
		buffer = (char *)calloc(size ,sizeof(char));

		fseek(fp,0,SEEK_SET);
		fread(buffer,sizeof(char),size,fp);
		fclose(fp);
		char *base64buf = (char *)calloc(OUT_ENBASE64_SIZE(size) ,sizeof(char));

		if(base64buf == NULL)
		{
			free(buffer);
			buffer = NULL;
			return L"";
		}
		if(base64_encode(base64buf, sizeof(char) * OUT_ENBASE64_SIZE(size),
			(unsigned char *)buffer, size) == 0)
		{
			free(buffer);
			buffer = NULL;
			free(base64buf);
			base64buf = NULL;
			return L"";
		}
		free(buffer);
		buffer = NULL;
		CString strResult=base64buf;
		free(base64buf);
		base64buf=NULL;

		return strResult.AllocSysString();

	}
	else
		return L"";
}

void CFaceLoginOCXCtrl::OnClose(DWORD dwSaveOption)
{
	// TODO: Add your specialized code here and/or call the base class

	COleControl::OnClose(dwSaveOption);
}


void CFaceLoginOCXCtrl::OnDestroy()
{
	faceMatch.StopThread();
	COleControl::OnDestroy();
	// TODO: Add your message handler code here
}

void CFaceLoginOCXCtrl::OnMove(int x, int y)
{
	COleControl::OnMove(x, y);
	Invalidate();
	// TODO: Add your message handler code here
}

void CFaceLoginOCXCtrl::OnMoving(UINT fwSide, LPRECT pRect)
{
	COleControl::OnMoving(fwSide, pRect);
	Invalidate();
	// TODO: Add your message handler code here
}

int CFaceLoginOCXCtrl::StartCamera(void)
{	
	return faceMatch.StartThread();
}

int CFaceLoginOCXCtrl::EndCamera(void)
{	
	return faceMatch.StopThread();
}
