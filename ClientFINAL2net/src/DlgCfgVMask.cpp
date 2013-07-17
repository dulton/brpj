// DlgCfgVMask.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgCfgVMask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgVMask dialog


CDlgCfgVMask::CDlgCfgVMask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgVMask::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCfgVMask)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCfgVMask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCfgVMask)
	DDX_Control(pDX, IDC_STATIC_WND, m_videownd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCfgVMask, CDialog)
	//{{AFX_MSG_MAP(CDlgCfgVMask)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgVMask message handlers

void CDlgCfgVMask::OnBtnApply()
{
	float w_off,h_off;
	RECT rect;
	m_videownd.GetClientRect(&rect);
	w_off = (float)(rect.right  - rect.left) / (float)704;
	h_off = (float)(rect.bottom - rect.top)  / (float)576;
	if(m_vmask.m_width < 0)
	{
		m_vmask.m_x = m_vmask.m_x + m_vmask.m_width;
		m_vmask.m_width = - m_vmask.m_width;
	}
	if(m_vmask.m_height < 0)
	{
		m_vmask.m_y = m_vmask.m_y + m_vmask.m_height;
		m_vmask.m_height = - m_vmask.m_height;
	}
	
	m_vmask.m_x = m_vmask.m_x < 0 ? - m_vmask.m_x : m_vmask.m_x;
	m_vmask.m_y = m_vmask.m_y < 0 ? - m_vmask.m_y : m_vmask.m_y;
	
	TRACE("x:%d,y:%d,width:%d,height:%d\n",
		m_vmask.m_x,m_vmask.m_y,
		m_vmask.m_width,m_vmask.m_height);
	
	if((m_vmask.m_x + m_vmask.m_width) > rect.right)
	{
		m_vmask.m_width = rect.right - m_vmask.m_x;
	}
	if((m_vmask.m_y + m_vmask.m_height) > rect.bottom)
	{
		m_vmask.m_height = rect.bottom - m_vmask.m_y;
	}
	
	pvmask->m_x = (short)((float)m_vmask.m_x / w_off);
	pvmask->m_y = (short)((float)m_vmask.m_y / h_off);
	pvmask->m_width  = (short)((float)m_vmask.m_width / w_off);
	pvmask->m_height = (short)((float)m_vmask.m_height / h_off);
	VSNET_ClientSetVideoMask(m_servername,m_url,m_ch,m_username,m_password,pvmask,m_port);
}

BOOL CDlgCfgVMask::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	CHANNEL_CLIENTINFO m_chinfo;
	m_chinfo.m_buffnum    = 5;
	m_chinfo.m_ch         = m_ch;
	m_chinfo.m_hChMsgWnd  = NULL;
	m_chinfo.m_hVideohWnd = GetDlgItem(IDC_STATIC_WND)->m_hWnd;
	m_chinfo.m_nChmsgid   = 0;
	m_chinfo.m_password   = m_password;
	m_chinfo.m_playstart  = TRUE;
	m_chinfo.m_sername    = m_servername;
	m_chinfo.m_tranType   = 1;
	m_chinfo.m_useoverlay = FALSE;
	m_chinfo.m_username   = m_username;
	m_chinfo.context      = NULL;
	m_chinfo.m_messagecallback = NULL;
	m_hchann = VSNET_ClientStart(m_url,&m_chinfo,m_port);
	if(m_hchann == -1)
	{
		str.LoadString(IDS_STRING180);
		AfxMessageBox(str,MB_ICONWARNING);
	}
	
	RECT rect;
	m_videownd.m_ImageWidth  = 704;
	m_videownd.m_ImageHeight = 576;
	m_videownd.m_HCount = m_videownd.m_ImageWidth / 32;
	m_videownd.m_VCount = m_videownd.m_ImageHeight / 32;
	m_videownd.m_curmouse_x = m_videownd.m_curmouse_y = 0;
	m_videownd.GetClientRect(&rect);
	m_videownd.m_wblock = (rect.right  - rect.left)/(m_videownd.m_ImageWidth/32);
	m_videownd.m_hblock = (rect.bottom - rect.top)/(m_videownd.m_VCount);

	float w_off,h_off;
	w_off = (float)(rect.right  - rect.left) / (float)704;
	h_off = (float)(rect.bottom - rect.top)  / (float)576;
	m_videownd.m_maskmax_w  = (int)((float)352  * w_off);
	m_videownd.m_maskmax_h  = (int)((float)288 * h_off);

	memset(&m_vmask,0,sizeof(CHANNVIDEOMASK));
	m_videownd.pvmask   = &m_vmask;
	m_videownd.m_bIsArea= FALSE;
	m_videownd.m_bLinkOk= TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCfgVMask::OnDestroy() 
{
	if(m_hchann != -1)
	{
		VSNET_ClientStop(m_hchann);
		m_hchann = -1;
	}
	CDialog::OnDestroy();
}

