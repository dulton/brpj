// DLGBlackInOut.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGBlackInOut.h"

#include "SmartVillageDlg.h"
extern CSmartVillageDlg *DlgMain;

#include "xmlrw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDLGBlackInOut *pDlgBlackInout;
/////////////////////////////////////////////////////////////////////////////
// CDLGBlackInOut dialog


CDLGBlackInOut::CDLGBlackInOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGBlackInOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGBlackInOut)
	m_info = _T("");
	//}}AFX_DATA_INIT
	flag=0;
}


void CDLGBlackInOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGBlackInOut)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Text(pDX, IDC_STATIC_INFO, m_info);
	DDV_MaxChars(pDX, m_info, 260);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGBlackInOut, CDialog)
	//{{AFX_MSG_MAP(CDLGBlackInOut)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//线程
DWORD WINAPI BlackInout_ThreadPROC(LPVOID lpParameter)
{
	CDLGBlackInOut *pBio = (CDLGBlackInOut*)lpParameter;
	pBio->ThreadFlag=TRUE;

	XMLRW xmlrw;

	switch(pBio->flag)
	{
	case BLACK_INOUT_INPUT :
		if(xmlrw.ReadXml(pBio->path))
		{
			DlgMain->NewLogMessage("黑名单数据导入 完成");
			pBio->MessageBox("黑名单数据导入 完成",MESSAGEBOX_TITLE);
		}
		else
		{
			DlgMain->NewLogMessage("黑名单数据导入 XML文件出错");
			pBio->MessageBox("黑名单数据导入 XML文件出错",MESSAGEBOX_TITLE);
		}
		break;
	case BLACK_INOUT_OUTPUT :
		if(xmlrw.CreateXml(pBio->path))
		{
			DlgMain->NewLogMessage("黑名单数据导出 完成");
			pBio->MessageBox("黑名单数据导出 完成",MESSAGEBOX_TITLE);
		}
		else
		{
			DlgMain->NewLogMessage("黑名单数据导出 XML文件出错");
			pBio->MessageBox("黑名单数据导出 XML文件出错",MESSAGEBOX_TITLE);
		}
		break;
	default:
		DlgMain->NewLogMessage("CDLGBlackInOut::BlackInout_ThreadPROC error");
		pBio->MessageBox("CDLGBlackInOut::BlackInout_ThreadPROC error",MESSAGEBOX_TITLE);
		return true;
	}

	pBio->ThreadFlag=FALSE;
	return 0;
}

// CDLGBlackInOut message handlers
BOOL CDLGBlackInOut::OnInitDialog() 
{
	CDialog::OnInitDialog();

	pDlgBlackInout=this;

	//必须预定义后再弹窗口
	switch(flag)
	{
	case BLACK_INOUT_INPUT :
		SetWindowText("黑名单数据导入");
		break;
	case BLACK_INOUT_OUTPUT :
		SetWindowText("黑名单数据导出");
		break;
	default:
		DlgMain->NewLogMessage("CDLGBlackInOut::OnInitDialog error");
		MessageBox("CDLGBlackInOut::OnInitDialog error",MESSAGEBOX_TITLE);
		return true;
	}

	ThreadFlag=FALSE;
	pthread=NULL;

	pthread=CreateThread(NULL,0,BlackInout_ThreadPROC,this,0,NULL);

	if(NULL==pthread)
	{
		DlgMain->NewLogMessage("CDLGBlackInOut::OnInitDialog 线程错误");
		MessageBox("CDLGBlackInOut::OnInitDialog 线程错误",MESSAGEBOX_TITLE);
	}
	else
		ThreadFlag=TRUE;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGBlackInOut::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CDLGBlackInOut::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(FALSE !=	ThreadFlag)
	{
		ThreadFlag=FALSE;
		Sleep(1000);
		TerminateThread(pthread,0);
		MessageBox("已中断",MESSAGEBOX_TITLE);
		DlgMain->NewLogMessage("黑名单导入导出中断");
	}

	CDialog::OnCancel();
}

void CDLGBlackInOut::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	if(FALSE !=	ThreadFlag)
	{
		ThreadFlag=FALSE;
		Sleep(1000);
		TerminateThread(pthread,0);
		MessageBox("已中断",MESSAGEBOX_TITLE);
		DlgMain->NewLogMessage("黑名单导入导出中断");
	}
	else
		OnCancel();
}
