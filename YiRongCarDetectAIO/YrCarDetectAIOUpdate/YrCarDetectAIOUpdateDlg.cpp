// YrCarDetectAIOUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YrCarDetectAIOUpdate.h"
#include "YrCarDetectAIOUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum UPDATE_FLAG
{
	UF_CREAT_DIR=1,
	UF_DELETE_DIR=2,
	UF_CREAT_FILE=3,
	UF_DELETE_FILE=4,

};

/*
TOMCAT服务器要建立
\yrupdate 文件夹
里面topnew.txt 为最新版本号

\yrupdate\N\ 为要从N版本升级到M版本的 文件夹
\yrupdate\N\yrversion.txt 为本次升级到M版本的版本号

本机目录下要有yrversion.txt 为当前版本号
//UPDALIST里面以 空格来隔开，路径不要含有空格，
//如果 路径有空格以*来代替，
如以下
5				//共几行
1 ODAC ODAC		//新建文件夹
2 ODAC ODAC		//删除文件夹
3 ODAC/b.txt ODAC\b.txt	//新建文件
4 a.txt   a.txt			//删除文件
3 yrversion.txt yrversion.txt	//新建文件

*/



// CYrCarDetectAIOUpdateDlg dialog




CYrCarDetectAIOUpdateDlg::CYrCarDetectAIOUpdateDlg(CWnd* pParent /*=NULL*/)
: CDialog(CYrCarDetectAIOUpdateDlg::IDD, pParent)
, m_version(0)
, m_ip(_T("35.24.252.109:8089"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYrCarDetectAIOUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_version);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
}

BEGIN_MESSAGE_MAP(CYrCarDetectAIOUpdateDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CYrCarDetectAIOUpdateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CYrCarDetectAIOUpdateDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CYrCarDetectAIOUpdateDlg message handlers

BOOL CYrCarDetectAIOUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//////////////////////////////////////////////////////////////
	// TODO: Add extra initialization here
	GetCurrentDirectory(MAX_PATH, CurrentDir);
	sprintf(updatedir,"%s\\yrupdate",CurrentDir);

	sd.InitData();
	stopflag=true;
	realstop=true;

	char str[1024];
	sprintf(str,"%s\\yrversion.txt",CurrentDir);

	if(!ReadVersionFile(str))
	{
		MessageBox("无法读取到本地版本文件");
		m_version=0;
	}

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CYrCarDetectAIOUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CYrCarDetectAIOUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI PlayThreadPROC(LPVOID lpParameter)
{
	CYrCarDetectAIOUpdateDlg *pDlg= (CYrCarDetectAIOUpdateDlg*)lpParameter;

	pDlg->realstop=false;
	while(pDlg->CheckVersion())
	{
		if(!pDlg->ReadUpdateList())
			break;


		if(	pDlg->stopflag)
			break;
	}
	pDlg->realstop =true;

	pDlg->GetDlgItem(IDOK)->EnableWindow(TRUE);
	return 0;

}
void CYrCarDetectAIOUpdateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	UpdateData(true);

	stopflag=false;
	realstop=true;
	pthread=CreateThread(NULL,0,PlayThreadPROC,this,0,NULL);

	GetDlgItem(IDOK)->EnableWindow(FALSE);

	UpdateData(false);

}

void CYrCarDetectAIOUpdateDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if(false==realstop)
	{
		//等待线程退出
		stopflag=true;
		while(false ==realstop)
		{
			Sleep(100);
		}
	}
	else
	{
		sd.DestroyData();
		OnCancel();
	}
}


//读版本文件
bool CYrCarDetectAIOUpdateDlg::ReadVersionFile(TCHAR* FileName)
{
	FILE *fp=_tfopen(FileName,_T("r"));
	if(fp)
	{
		_ftscanf(fp,_T("%d"),&m_version);
		fclose(fp);

		return TRUE;
	}
	else
		return FALSE;
}

bool CYrCarDetectAIOUpdateDlg::CheckVersion()
{
	char url[1024];
	char url2[1024];
	char str[1024];
	char fail[1024];

	int new_version=0;

	sprintf(str,"%s\\yrversion.txt",CurrentDir);

	if(!ReadVersionFile(str))
	{
		MessageBox("无法读取到本地版本文件");
		m_version=0;
	}


	sprintf(url,"http://%s/yrupdate/topnew.txt",m_ip);

	EncodeURI(url,url2,1024);

	sprintf(str,"%s\\temp.txt",CurrentDir);

	if(!sd.HTTPDownload(url2,str,fail,10,0))
	{
		MessageBox("无法连接到更新服务器");
		return false;
	}

	FILE *fp=_tfopen(str,_T("r"));
	if(fp)
	{
		_ftscanf(fp,_T("%d"),&new_version);
		fclose(fp);

		if(new_version>m_version)
		{
			return true;
		}
		else
		{
			MessageBox("已经是最新版 无需更新");
			return false;
		}
	}
	else
	{
		MessageBox("无法读取temp文件");
		return false;
	}

}


bool CYrCarDetectAIOUpdateDlg::ReadUpdateList()
{
	char tempstr[1024];

	char url[1024];
	char url2[1024];
	char fail[1024];

	sprintf(tempstr,"rmdir /s /q \"%s\"",updatedir);
	WinExec(tempstr,SW_HIDE);

	CreateDirectory(updatedir, NULL);
	//跳到版本号文件夹
	sprintf(url,"http://%s/yrupdate/%d/updatelist.txt",m_ip,m_version);

	sprintf(tempstr,"%s\\updatelist.txt",updatedir);

	EncodeURI(url,url2,1024);

	if(!sd.HTTPDownload(url2,tempstr,fail,10,0))
	{
		UpdateData(false);
		MessageBox("无法连接到更新服务器 下载updatelist失败");
		return false;
	}

	updateListTotal=0;
	int i;
	FILE *fp=_tfopen(tempstr,_T("r"));
	if(fp)
	{
		_ftscanf(fp,_T("%d"),&updateListTotal);
		if(updateListTotal>=MAX_UPDATE_FILE)
		{	
			fclose(fp);
			MessageBox("更新文件超过最大值 无法自动更新 请联系技术员");
			return false;
		}
		//UPDALIST里面以 空格来隔开，路径不要含有空格，
		//如果 路径有空格以*来代替，
		for(i=0;i<updateListTotal;i++)
		{
			_ftscanf(fp,_T("%d%s%s"),
				&(updateList[i].flag),
				updateList[i].srcurl,
				updateList[i].dstpath);
		}
		/* 如以下
		5				//共几行
		1 ODAC ODAC		//新建文件夹
		2 ODAC ODAC		//删除文件夹
		3 ODAC/b.txt ODAC\b.txt	//新建文件
		4 a.txt   a.txt			//删除文件
		3 yrversion.txt yrversion.txt	//新建文件
		*/

		fclose(fp);
		return true;
	}
	else
	{
		MessageBox("读取updatelist失败");
		return false;
	}
}