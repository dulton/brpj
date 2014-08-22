// YrCarDetectAIOUpdate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "YrCarDetectAIOUpdate.h"
#include "YrCarDetectAIOUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYrCarDetectAIOUpdateApp

BEGIN_MESSAGE_MAP(CYrCarDetectAIOUpdateApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CYrCarDetectAIOUpdateApp construction

CYrCarDetectAIOUpdateApp::CYrCarDetectAIOUpdateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CYrCarDetectAIOUpdateApp object

CYrCarDetectAIOUpdateApp theApp;


// CYrCarDetectAIOUpdateApp initialization

BOOL CYrCarDetectAIOUpdateApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CYrCarDetectAIOUpdateDlg dlg;
	m_pMainWnd = &dlg;
//////////////////////////////
	
	LPWSTR *szArglist = NULL;  
    int nArgs = 0;  
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);  
    if( nArgs > 1)  
    {  
		dlg.m_ip =szArglist[1];
	}
    //取得参数后，释放CommandLineToArgvW申请的空间  
    LocalFree(szArglist);  
//////////////////////////////


	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
