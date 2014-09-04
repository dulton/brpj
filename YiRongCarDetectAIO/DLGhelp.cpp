// DLGhelp.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "YiRongCarDetectAIODlg.h"
#include "DLGhelp.h"


#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

////////////////////////////////////////
#include "URLencode.h"
#include "SignalDownload.h"
//当前路径
extern TCHAR CurrentDir[ZOG_MAX_PATH_STR];

extern CYiRongCarDetectAIODlg *DlgMain;
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGhelp dialog


CDLGhelp::CDLGhelp(CWnd* pParent /*=NULL*/)
: CDialog(CDLGhelp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGhelp)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLGhelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGhelp)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGhelp, CDialog)
	//{{AFX_MSG_MAP(CDLGhelp)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDLGhelp::OnBnClickedButtonUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGhelp message handlers


BOOL CDLGhelp::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//////显示标题///////////////////
	CString modeflag;

	//结果输出到平台 模式 =1  单机版=0 
	if(YRVM_PINGTAI_MODE)
	{
		modeflag="联机 ";

		if(0==ALLTAB_DETECT_CAR_MODE)
		{
			//电动车混合模式
			if(YRVM_PINGTAI_ELECAR_MIX_MODE)
				modeflag+="机";
			else
				modeflag+="电";
		}
	}
	else
		modeflag="单机 ";

	if(OPEN_CARDETECT_CODE)
		modeflag+="开识别 ";
	else
		modeflag+="无识别 ";

	if(ALLTAB_DETECT_CAR_MODE)
		modeflag+="机动车 ";
	else
		modeflag+="电动车 ";

	if(ALLTAB_CLIENT_MODE)
		modeflag+="客户端 ";
	else
		modeflag+="服务端 ";

	if(OPEN_VS2008_POCO_FTP)
		modeflag+="FTP ";

	if(IVMS_ORACLE_DEVICETREE)
		modeflag+="同步平台树 ";

	if(OPEN_TOMCAT_MODE)
		modeflag+="TOMCAT共享 \n";
	else
		modeflag+="数据库共享 \n";

	if(OPEN_YAAN_NEW_SDK)
		modeflag+="支持亚安新版 ";

	if(OPEN_YAAN_SDK)
		modeflag+="支持亚安 ";

	if(OPEN_DAHUA_SDK)
		modeflag+="支持大华 ";

	if(OPEN_HAIKANG_SDK)
		modeflag+="支持海康 ";

	if(OPEN_STREAM_CLIENT_SDK)
		modeflag+="支持流媒体 ";

	CString str;
	str.Format("%s\n构建时间:%s",modeflag.GetBuffer(0),__DATE__);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(str);

#if (!ALLTAB_CLIENT_MODE)
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(FALSE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDLGhelp::OnOK() 
{
	// TODO: Add extra validation here


	CDialog::OnOK();
}

void CDLGhelp::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CDLGhelp::OnBnClickedButtonUpdate()
{
	// TODO: Add your control notification handler code here

	char url[1024];
	char url2[1024];
	char str[1024];
	char fail[1024];
	sprintf(str,"%s\\yrversion.txt",CurrentDir);
	int cur_version=0;
	int new_version=0;
	FILE *fp=_tfopen(str,_T("r"));
	if(fp)
	{
		_ftscanf(fp,_T("%d"),&cur_version);
		fclose(fp);
	}
	else
	{
		MessageBox("无法读取到本地版本文件");
		return ;
	}


	SignalDownload sd;
	sd.InitData();

	sprintf(url,"http://%s/yrupdate/topnew.txt",DlgSetSystem.m_update_url);

	EncodeURI(url,url2,1024);

	sprintf(str,"%s\\temp.txt",DlgSetSystem.m_path_detect);

	if(!sd.HTTPDownload(url2,str,fail,10,0))
	{
		sd.DestroyData();
		MessageBox("无法连接到更新服务器");
		return ;
	}
	sd.DestroyData();

	fp=_tfopen(str,_T("r"));
	if(fp)
	{
		_ftscanf(fp,_T("%d"),&new_version);
		fclose(fp);

			
		if(cur_version!=0 && new_version!=0 && new_version>cur_version)
		{
			ShellExecute(NULL,NULL,"YrCarDetectAIOUpdate.exe",DlgSetSystem.m_update_url,NULL,SW_NORMAL);
			//退出本应用
			DlgLogin.SilentMode=1;
			DlgMain->OnCancel();
			exit(0);
		}
		else
		{
			MessageBox("已经是最新版 无需更新");
			return ;
		}
	}

}
