// LAUMp4Test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "LAUMp4TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLAUMp4TestApp

BEGIN_MESSAGE_MAP(CLAUMp4TestApp, CWinApp)
	//{{AFX_MSG_MAP(CLAUMp4TestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLAUMp4TestApp construction

CLAUMp4TestApp::CLAUMp4TestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLAUMp4TestApp object

CLAUMp4TestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLAUMp4TestApp initialization

BOOL CLAUMp4TestApp::InitInstance()
{
	/*
	m_hLonDll = NULL;
	char currdir[MAX_PATH];
	GetModuleFileName(NULL, currdir, MAX_PATH);
	CString str;
	str.Format("%s", currdir);
	str = str.Left(str.ReverseFind(_T('\\')));
	sprintf(currdir, "%s", str);
*/
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif



	CLAUMp4TestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
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

int CLAUMp4TestApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_hLonDll != NULL)
	{
		FreeLibrary(m_hLonDll);
		m_hLonDll = NULL;
	}
	return CWinApp::ExitInstance();
}
