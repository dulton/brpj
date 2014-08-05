// DLGhelp.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGhelp.h"

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
