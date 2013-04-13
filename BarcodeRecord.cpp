// BarcodeRecord.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "BarcodeRecordDlg.h"

#include "DLGSettings.h"
CDLGSettings DlgSettings;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarcodeRecordApp

BEGIN_MESSAGE_MAP(CBarcodeRecordApp, CWinApp)
	//{{AFX_MSG_MAP(CBarcodeRecordApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarcodeRecordApp construction

CBarcodeRecordApp::CBarcodeRecordApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBarcodeRecordApp object

CBarcodeRecordApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBarcodeRecordApp initialization

BOOL CBarcodeRecordApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//读系统配置表
	Set_AllPath();
	DlgSettings.Read2Dlg();


	CBarcodeRecordDlg dlg;
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

TCHAR CurrentDir[ZOG_MAX_PATH_STR]="";
TCHAR LanguageChsDir[ZOG_MAX_PATH_STR]="";
bool IsChinese=false;

void Set_AllPath(void)
{
	GetCurrentDirectory(ZOG_MAX_PATH_STR, CurrentDir);
	sprintf(LanguageChsDir, "%s\\langchs.ini", CurrentDir);
	//当前路径
	sprintf(DlgSettings.SystemIniPath,"%s\\systeminfo.ini",CurrentDir);

	FILE *fp=NULL;
	if(fp=fopen(LanguageChsDir,"r"))
	{
		IsChinese=true;
		fclose(fp);
	}
	else
		IsChinese=false;
}


//“英文字符串”转换为“当前语言类型的字符串”
CString Language_ConvertString(CString strText)
{
	if(!IsChinese)
		return strText;

	char val[ZOG_MAX_PATH_STR];
	CString strIniPath,strRet;

	memset(val,0,ZOG_MAX_PATH_STR);
	GetPrivateProfileString("String", strText, "", val, ZOG_MAX_PATH_STR, LanguageChsDir);
	strRet = val;
	if(strRet.GetLength()==0)
	{
		//如果ini文件中不存在对应的字符串，设置为默认值（英文）
		return strText;
	}
	return strRet;
}

//设置对话框中的静态文本(英文-->当前语言)   
void Language_SetWndStaticText(CWnd * pWnd)
{
	CString strCaption,strText;

	//设置主窗口的标题
	pWnd->GetWindowText(strCaption);
	if(strCaption.GetLength()>0)
	{
		strText=Language_ConvertString(strCaption);
		pWnd->SetWindowText(strText);
	}

	//设置子窗口的标题
	CWnd * pChild=pWnd->GetWindow(GW_CHILD);
	CString strClassName;
	while(pChild)
	{
		//////////////////////////////////////////////////////////////////////////		
		strClassName = ((CRuntimeClass*)pChild->GetRuntimeClass())->m_lpszClassName;
		if(strClassName == "CEdit")
		{
			//下一个子窗口
			pChild=pChild->GetWindow(GW_HWNDNEXT);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////	

		//设置子窗口的当前语言文本
		pChild->GetWindowText(strCaption);
		strText=Language_ConvertString(strCaption);
		pChild->SetWindowText(strText);

		//下一个子窗口
		pChild=pChild->GetWindow(GW_HWNDNEXT);
	}
}
/*
 *	组合框语言转换
 */
void Language_ConvertComboBox(CComboBox &stuComboBox)
{
	int nCount = stuComboBox.GetCount();
	if(nCount <= 0)
	{
		return;
	}
	CString* strText = new CString[nCount];
	int i;
	for( i = 0; i < nCount;i++)
	{
		stuComboBox.GetLBText(i, strText[i]);
		strText[i] = Language_ConvertString(strText[i]);
	}
	stuComboBox.ResetContent();
	for(i = 0; i< nCount; i++)
	{
		stuComboBox.AddString(strText[i]);
	}
	delete []strText;
}