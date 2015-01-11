// FaceLoginOCXCtrl.cpp : Implementation of the CFaceLoginOCXCtrl ActiveX Control class.

#include "stdafx.h"
#include "FaceLoginOCX.h"
#include "FaceLoginOCXCtrl.h"
#include "FaceLoginOCXPropPage.h"
#include "FrmFaceMatch.h"
#include "FrmFaceEnroll.h"

//用来其他类引用的主窗口类
CFaceLoginOCXCtrl *DlgFaceLoginOCXCtrl;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFaceLoginOCXCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CFaceLoginOCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CFaceLoginOCXCtrl, COleControl)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "StartFaceMacth", dispidFaceMacth, StartFaceMacth, VT_BSTR, VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "StartFaceEnroll", dispidFaceEnroll, StartFaceEnroll, VT_I2, VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CFaceLoginOCXCtrl, "GetCamNum", dispidGetCamNum, GetCamNum, VT_I2, VTS_NONE)
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

//// CFaceLoginOCXCtrl::CFaceLoginOCXCtrlFactory::UpdateRegistry -
//// Adds or removes system registry entries for CFaceLoginOCXCtrl
//
//BOOL CFaceLoginOCXCtrl::CFaceLoginOCXCtrlFactory::UpdateRegistry(BOOL bRegister)
//{
//	// TODO: Verify that your control follows apartment-model threading rules.
//	// Refer to MFC TechNote 64 for more information.
//	// If your control does not conform to the apartment-model rules, then
//	// you must modify the code below, changing the 6th parameter from
//	// afxRegApartmentThreading to 0.
//
//	if (bRegister)
//		return AfxOleRegisterControlClass(
//			AfxGetInstanceHandle(),
//			m_clsid,
//			m_lpszProgID,
//			IDS_FACELOGINOCX,
//			IDB_FACELOGINOCX,
//			afxRegApartmentThreading,
//			_dwFaceLoginOCXOleMisc,
//			_tlid,
//			_wVerMajor,
//			_wVerMinor);
//	else
//		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
//}

//获取程序目录
bool GetCurExePath(char *path, int size)
{
	char charRet[512] = "0";
	GetModuleFileNameA(NULL, charRet, 512);
	int l = strlen(charRet);
	if ((l == 0) || (size < l)) {
		return false;
	}
	int pos = 0;
	for (int i = strlen(charRet) - 1; i >= 0; i--) {
		if ((charRet[i] == '\\') || (charRet[i] == '/')) {
			pos = i;
			break;
		}
	}
	if (pos == 0) {
		return false;
	}
	memset(path, 0, size);
	memcpy(path, charRet, (pos <= size) ? pos : size);

	return true;
}

// CFaceLoginOCXCtrl::CFaceLoginOCXCtrl - Constructor

CFaceLoginOCXCtrl::CFaceLoginOCXCtrl()
{
	InitializeIIDs(&IID_DFaceLoginOCX, &IID_DFaceLoginOCXEvents);
	// TODO: Initialize your control's instance data here.
	DlgFaceLoginOCXCtrl = this;

	//GetCurrentDirectory(260,curpath);

	GetCurExePath(curpath,260);
	//ShowOcxLog(curpath);
}



// CFaceLoginOCXCtrl::~CFaceLoginOCXCtrl - Destructor

CFaceLoginOCXCtrl::~CFaceLoginOCXCtrl()
{
	// TODO: Cleanup your control's instance data here.
#if 0
	CString tempstr;
	tempstr.Format("%s\\%s",curpath,"net.lic");
	DeleteFile(tempstr);
	tempstr.Format("%s\\%s",curpath,"calibf.txt");
	DeleteFile(tempstr);
	tempstr.Format("%s\\%s",curpath,"fft_size_cachef.txt");
	DeleteFile(tempstr);
	tempstr.Format("%s\\%s",curpath,"model_calib.dat");
	DeleteFile(tempstr);
#endif
}



// CFaceLoginOCXCtrl::OnDraw - Drawing function

void CFaceLoginOCXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
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



// CFaceLoginOCXCtrl message handlers
/************************************
* 人脸比对
*************************************/
BSTR CFaceLoginOCXCtrl::StartFaceMacth(LPCTSTR strFaceServer,int sysID,LPCTSTR user,LPCTSTR password)
{
/*
	strFaceServer="http://10.142.50.125:9080/frcs_new";
	sysID = 1;
	user="123";
	password="123456";
*/

	CString strlog;
	strlog.Format(_T("<TIPS><MATCH> - Start Match - User<%s>"),user);
	ShowOcxLog(strlog);

	CCommon m_Common;

#if MD5_OPEN
	CString sMd5;
	CString temp = password;
	sMd5 = m_Common.EncodeMd5(temp.GetBuffer());
#endif

	MatchUser = user;
	MatchSysID = sysID;
	CFrmFaceMatch faceMatch;

#if MD5_OPEN
	int checkReslut = faceMatch.m_Detect.CheckMatchInfo(strFaceServer,sysID,user,sMd5);
#else
	int checkReslut = faceMatch.m_Detect.CheckMatchInfo(strFaceServer,sysID,user,password);
#endif


	strlog.Format(_T("<TIPS><MATCH>222 - Start Match - User<%d>"),checkReslut);
	ShowOcxLog(strlog);

	if(checkReslut == ERR_WRONG_VERSION)
	{
		CString ret;
		ret.Format(_T("%d"),OCX_ERROR_VERSION_ERROR);
		return ret.AllocSysString();//OCX程序版本不对
	}
	else if(checkReslut == ERR_NONEED_MATCH)		//不需要人脸识别
	{
		faceMatch.MacthResult = faceMatch.m_Detect.Token;
		return faceMatch.MacthResult.AllocSysString();//返回令牌
	}
	else if(checkReslut == ERR_UNREGIST_FACE)		//未注册人脸
	{
		CString ret;
		ret.Format(_T("%d"),OCX_ERROR_UNREGIST_USER);
		return ret.AllocSysString();//未注册人脸直接返回
	}
	else if(checkReslut == ERR_BAD_USER)
	{
		CString ret;
		ret.Format(_T("%d"),OCX_ERROR_UNALLOW_USER);
		return ret.AllocSysString();//非法用户直接返回
	}
	else if(checkReslut == ERR_SER_REEOR)			//人脸平台异常
	{
		CString ret;
		ret.Format(_T("%d"),OCX_ERROR_FACESERVER_DOWN);
		return ret.AllocSysString();//平台异常直接返回
	}
	
	//判断摄像头
	if(m_Common.GetCamNum() < 1)
	{
		strlog = _T("<ERROR><MATCH> - No find camera!");
		ShowOcxLog(strlog);

		CString ret;
		ret.Format(_T("%d"),OCX_ERROR_NO_CAMERA);
		return ret.AllocSysString();//未检测到摄像头直接返回
	}


	if(faceMatch.m_Detect.GetFaceCloudState())
	{
		//long aa=GetTickCount();

		if(faceMatch.m_Detect.enableLive)
		{
#if LIVE_FACE_TEST
			if(faceMatch.InitLive())
#endif
			{
				//	CString strlog;
				//	strlog.Format(_T("InitLive：%d"),GetTickCount()-aa);
				//	DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

				faceMatch.DoModal();
			}
#if LIVE_FACE_TEST
			faceMatch.unInitLive();
#endif
		}
		else

		{
			faceMatch.DoModal();
		}
	}
	else
	{
		faceMatch.MacthLog.Format(_T("{\
											\"ret\":\"fail\",\
											\"user\":\"%s\",\
											\"sysID\":\"%d\",\
											\"content\":\"FaceServer_Out_of_Service\"\
											}"),user,sysID);
		CString ret;
		ret.Format(_T("%d"),OCX_ERROR_FACESERVER_DOWN);
		return ret.AllocSysString();//facecloud异常直接返回
	}

	faceMatch.m_Detect.SendMatchLog(strFaceServer,faceMatch.MacthLog);

	return faceMatch.MacthResult.AllocSysString();
}

/************************************
* 人脸注册
* strFaceServer -- 平台服务地址 格式为"http://10.142.50.125:9080"
* sysID			-- 所属系统ID
* user			-- 待注册用户名
* password		-- 带注册用户密码
* return		-- 成功或者失败
*************************************/
int CFaceLoginOCXCtrl::StartFaceEnroll(LPCTSTR strFaceServer,int sysID,LPCTSTR user,LPCTSTR password)
{
/*
	strFaceServer="http://10.142.50.125:9080/frcs_new";
	sysID = 13;
	user="linhp";
	password="96E79218965EB72C92A549DD5A330112";
*/

	CString strlog;
	strlog.Format(_T("<TIPS><ENROLL> - Start Enroll - User<%s>"),user);
	ShowOcxLog(strlog);

	//CString result = _T("");
	CCommon m_Common;
	
	int ret = 0;
	//CString sMd5;
	//CString temp = password;
	//sMd5 = m_Common.EncodeMd5(temp.GetBuffer());
	EnrollUser = user;
	EnrollSysID = sysID;
	CFrmFaceEnroll faceEnroll;
	int result = faceEnroll.m_Detect.CheckRegInfo(strFaceServer,sysID,user,password);

	if(result == ERR_WRONG_VERSION)
	{
		ret =  OCX_ERROR_VERSION_ERROR;
		return ret;//OCX程序版本不对
	}
	else if(result == ERR_BAD_USER)
	{
		ret =  OCX_ERROR_UNALLOW_USER;
		return ret;//非法用户直接返回
	}
	else if(result == ERR_SER_REEOR)
	{
		ret =  OCX_ERROR_FACESERVER_DOWN;
		return ret;//平台异常直接返回
	}

	//判断摄像头
	if(m_Common.GetCamNum() < 1)
	{
		strlog = _T("<ERROR><ENROLL> - No find camera!");
		ShowOcxLog(strlog);

		return OCX_ERROR_NO_CAMERA;
	}

	if(faceEnroll.m_Detect.GetFaceCloudState())
	{
		faceEnroll.DoModal();
		ret = faceEnroll.EnrollResult;
		
	}
	else
	{
		faceEnroll.EnrollLog.Format(_T("{\
											\"ret\":\"fail\",\
											\"user\":\"%s\",\
											\"sysID\":\"%d\",\
											\"content\":\"FaceServer_Out_of_Service\"\
											}"),user,sysID);
		ret =  OCX_ERROR_FACESERVER_DOWN;
	}

	faceEnroll.m_Detect.SendEnrollLog(strFaceServer,faceEnroll.EnrollLog);

	return ret;
}

/************************************
* 获取摄像头个数
*************************************/
int CFaceLoginOCXCtrl::GetCamNum(void)
{
	CCommon m_Common;
	return m_Common.GetCamNum();
}

/************************************
* 打印ocx日志
*************************************/
void CFaceLoginOCXCtrl::ShowOcxLog(CString log)
{
	CString strInfo;
	CTime tm = CTime::GetTickCount();

	strInfo.Format(_T("%04d-%02d-%02d %02d:%02d:%02d  :  %s\n"),tm.GetYear(),tm.GetMonth(),tm.GetDay(),
																tm.GetHour(),tm.GetMinute(),tm.GetSecond(),log);
	FILE *fp = NULL;
	fp = fopen(OCX_LOG_PATH,"a+");
	if(fp)
	{
		fprintf(fp,strInfo);
		fclose(fp);
	}
}

