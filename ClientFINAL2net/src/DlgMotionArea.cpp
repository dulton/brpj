// DlgMotionArea.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgMotionArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMotionArea dialog


CDlgMotionArea::CDlgMotionArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMotionArea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMotionArea)
	m_motiontest = FALSE;
	//}}AFX_DATA_INIT
	m_hchann = -1;
}


void CDlgMotionArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMotionArea)
	DDX_Control(pDX, IDC_SLIDER1, m_MotionVal);
	DDX_Control(pDX, IDC_STATIC_VWND, m_VideoWnd);
	DDX_Check(pDX, IDC_CHECK_TEST, m_motiontest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMotionArea, CDialog)
	//{{AFX_MSG_MAP(CDlgMotionArea)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_ALL, OnBtnAll)
	ON_BN_CLICKED(IDC_BTN_CLEAN, OnBtnClean)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_CHECK_TEST, OnCheckTest)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMotionArea message handlers

BOOL CDlgMotionArea::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	CRect rect;
	CString str;
	//m_VideoWnd.MoveWindow(10,10,640,480,TRUE);
	m_VideoWnd.m_ChanMotion  = m_ChanMotion;
	m_VideoWnd.m_ImageWidth  = 704;
	m_VideoWnd.m_ImageHeight = 576;
	m_VideoWnd.m_HCount = m_VideoWnd.m_ImageWidth / 32;
	m_VideoWnd.m_VCount = m_VideoWnd.m_ImageHeight / 32 + ((m_VideoWnd.m_ImageHeight%32) ? 1 : 0);
	m_VideoWnd.GetClientRect(rect);
	m_VideoWnd.m_wblock = rect.Width()/(m_VideoWnd.m_ImageWidth/32);
	m_VideoWnd.m_hblock = rect.Height()/(m_VideoWnd.m_VCount);
	m_VideoWnd.m_bready = TRUE;
	m_VideoWnd.Invalidate();

	m_MotionVal.SetPos(m_ChanMotion->m_alarmsenstive);
	CString strpos;
	strpos.Format("%d",m_MotionVal.GetPos());
	GetDlgItem(IDC_STATIC_VAL)->SetWindowText(strpos);

	CHANNEL_CLIENTINFO m_chinfo;
	m_chinfo.m_buffnum    = 5;
	m_chinfo.m_ch         = m_ch;
	m_chinfo.m_hChMsgWnd  = NULL;
	m_chinfo.m_hVideohWnd = m_VideoWnd.m_hWnd;
	m_chinfo.m_nChmsgid   = 0;
	m_chinfo.m_password   = m_password;
	m_chinfo.m_playstart  = TRUE;
	m_chinfo.m_sername    = m_servername;
	m_chinfo.m_tranType   = 1;
	m_chinfo.m_useoverlay = FALSE;//TRUE;
	m_chinfo.m_username   = m_username;
	m_chinfo.context      = NULL;
	m_chinfo.m_messagecallback = NULL;
	m_hchann = VSNET_ClientStart(m_url,&m_chinfo,m_port);
	if(m_hchann == -1)
	{
		str.LoadString(IDS_STRING183);
		AfxMessageBox(str,MB_ICONWARNING);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMotionArea::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar == (CScrollBar*)GetDlgItem(IDC_SLIDER1))
	{
		if(    (nSBCode == SB_LEFT)
			|| (nSBCode == SB_LINELEFT)
			|| (nSBCode == SB_LINERIGHT)
			|| (nSBCode == SB_PAGELEFT)
			|| (nSBCode == SB_PAGERIGHT)
			|| (nSBCode == SB_RIGHT)
			|| (nSBCode == SB_THUMBPOSITION)
			|| (nSBCode == SB_THUMBTRACK) )
		{
			CString strpos;
			strpos.Format("%d",m_MotionVal.GetPos());
			GetDlgItem(IDC_STATIC_VAL)->SetWindowText(strpos);
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgMotionArea::OnBtnAll() 
{
	memset(m_ChanMotion->m_detect,1,18*22);
	m_VideoWnd.Invalidate();
}

void CDlgMotionArea::OnBtnClean() 
{
	memset(m_ChanMotion->m_detect,0,18*22);
	m_VideoWnd.Invalidate();
}

void CDlgMotionArea::OnBtnApply() 
{
	CString str;
	m_ChanMotion->m_alarmsenstive = m_MotionVal.GetPos();
	if(!VSNET_ClientSetMotionPara(m_servername,m_url,m_ch,m_username,m_password,m_ChanMotion,m_port))
	{
		str.LoadString(IDS_STRING184);
		AfxMessageBox(str);
	}
}

void CDlgMotionArea::OnBtnOk() 
{
	OnCancel();
}

void CDlgMotionArea::OnCancel() 
{	
	UpdateData(TRUE);
	if(m_motiontest)
	{
		m_motiontest = FALSE;
		UpdateData(FALSE);
		OnCheckTest();
	}
	CDialog::OnCancel();
}

void CDlgMotionArea::OnCheckTest() 
{	
	UpdateData(TRUE);
	CString str;
	if(!VSNET_ClientMotionTest(m_servername,m_url,m_ch,m_username,m_password,m_motiontest,m_port))
	{
		str.LoadString(IDS_STRING185);
		AfxMessageBox(str);
	}
}

void CDlgMotionArea::OnDestroy() 
{
	if(m_hchann != -1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_TEST))->GetCheck())
		{
			((CButton*)GetDlgItem(IDC_CHECK_TEST))->SetCheck(FALSE);
			OnCheckTest();
		}
		VSNET_ClientStop(m_hchann);
		m_hchann = -1;
	}
	CDialog::OnDestroy();	
}
