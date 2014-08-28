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
//命令行
void CYrCarDetectAIOUpdateApp::GetCommandLineAndToArgv(int &argc,CStringArray &argv)
{
	argc=0;    argv.RemoveAll();    //初始化
	CString commandLine=::GetCommandLine();
	CString argument;
	int pos,strLen;
	pos=commandLine.ReverseFind(' ');
	while(pos!=-1)
	{
		strLen=commandLine.GetLength();
		argument=commandLine.Right(strLen-pos-1);
		if(!argument.IsEmpty())
		{
			argc++;
			argv.InsertAt(0,argument);
		}
		commandLine=commandLine.Left(pos);
		pos=commandLine.ReverseFind(' ');
	}

	pos=commandLine.Find('\"');    //过滤掉" ",因为如果ShellExecute等函数调用时路径会掉" "
	while(pos!=-1)
	{
		commandLine.Delete(pos);
		pos=commandLine.Find('\"');
	}
	argc++;
	argv.InsertAt(0,commandLine);
}


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
		
	int argc;
	CStringArray argv; 

	LPWSTR *szArglist = NULL;  

	GetCommandLineAndToArgv(argc,argv);
	if( argc >1)  
	{  
		char tempstr[260];
		strcpy(tempstr,argv[1].GetBuffer(0));

		if(strlen(tempstr)>5)
		{
			//自拷贝模式
			if(tempstr[0] == 'c' &&
				tempstr[1] == 'o' &&
				tempstr[2] == 'p' &&
				tempstr[3] == 'y' )
			{
				dlg.copyMyselfFlag =true;
				dlg.m_ip =&(tempstr[4]);
			}
			else
				dlg.m_ip =argv[1];
		}
		else
			dlg.m_ip =argv[1];
		
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
