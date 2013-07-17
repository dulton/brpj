// DlgCfgViparam.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgCfgViparam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgViparam dialog


CDlgCfgViparam::CDlgCfgViparam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgViparam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCfgViparam)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_hchann = -1;
}


void CDlgCfgViparam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCfgViparam)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCfgViparam, CDialog)
	//{{AFX_MSG_MAP(CDlgCfgViparam)
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgViparam message handlers

void CDlgCfgViparam::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDlgCfgViparam::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CDlgCfgViparam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	
	char lpnum[4];

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BRI))->SetLineSize(1);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BRI))->SetPageSize(5);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BRI))->SetRange(0,255);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_CON))->SetLineSize(1);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_CON))->SetPageSize(5);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_CON))->SetRange(0,255);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SAT))->SetLineSize(1);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SAT))->SetPageSize(5);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SAT))->SetRange(0,255);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE))->SetLineSize(1);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE))->SetPageSize(5);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE))->SetRange(0,255);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BRI))->SetPos(m_pviparam->m_bri);
	sprintf(lpnum,"%d",m_pviparam->m_bri);
	GetDlgItem(IDC_STATIC_BRI)->SetWindowText(lpnum);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_CON))->SetPos(m_pviparam->m_con);
	sprintf(lpnum,"%d",m_pviparam->m_con);
	GetDlgItem(IDC_STATIC_CON)->SetWindowText(lpnum);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SAT))->SetPos(m_pviparam->m_sat);
	sprintf(lpnum,"%d",m_pviparam->m_sat);
	GetDlgItem(IDC_STATIC_SAT)->SetWindowText(lpnum);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE))->SetPos(m_pviparam->m_hue);
	sprintf(lpnum,"%d",m_pviparam->m_hue);
	GetDlgItem(IDC_STATIC_HUE)->SetWindowText(lpnum);

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
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCfgViparam::OnDestroy() 
{
	if(m_hchann != -1)
	{
		VSNET_ClientStop(m_hchann);
		m_hchann = -1;
	}
	CDialog::OnDestroy();	
}

void CDlgCfgViparam::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char lpnum[4];
	if(((CScrollBar*)GetDlgItem(IDC_SLIDER_BRI)) == pScrollBar)
	{
		sprintf(lpnum,"%d",((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BRI))->GetPos());
		GetDlgItem(IDC_STATIC_BRI)->SetWindowText(lpnum);
	}
	else if(((CScrollBar*)GetDlgItem(IDC_SLIDER_CON)) == pScrollBar)
	{
		sprintf(lpnum,"%d",((CSliderCtrl*)GetDlgItem(IDC_SLIDER_CON))->GetPos());
		GetDlgItem(IDC_STATIC_CON)->SetWindowText(lpnum);
	}
	else if(((CScrollBar*)GetDlgItem(IDC_SLIDER_SAT)) == pScrollBar)
	{
		sprintf(lpnum,"%d",((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SAT))->GetPos());
		GetDlgItem(IDC_STATIC_SAT)->SetWindowText(lpnum);
	}
	else if(((CScrollBar*)GetDlgItem(IDC_SLIDER_HUE)) == pScrollBar)
	{
		sprintf(lpnum,"%d",((CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE))->GetPos());
		GetDlgItem(IDC_STATIC_HUE)->SetWindowText(lpnum);
	}
	if(m_hchann != -1)
	{
		m_pviparam->m_bri = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BRI))->GetPos();
		m_pviparam->m_con = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_CON))->GetPos();
		m_pviparam->m_sat = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SAT))->GetPos();
		m_pviparam->m_hue = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE))->GetPos();
		VSNET_ClientViAdjust(m_hchann,m_pviparam->m_bri,m_pviparam->m_con,m_pviparam->m_sat,m_pviparam->m_hue);
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
