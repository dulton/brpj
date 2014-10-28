// FaceLoginOCX.cpp : Implementation of CFaceLoginOCXApp and DLL registration.

#include "stdafx.h"
#include "FaceLoginOCX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFaceLoginOCXApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x3656C95A, 0xA6BA, 0x41FA, { 0x89, 0x71, 0x97, 0x9D, 0xEB, 0xB8, 0xD9, 0x6E } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CFaceLoginOCXApp::InitInstance - DLL initialization

BOOL CFaceLoginOCXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CFaceLoginOCXApp::ExitInstance - DLL termination

int CFaceLoginOCXApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
