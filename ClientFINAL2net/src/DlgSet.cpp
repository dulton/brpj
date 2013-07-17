// DlgSet.cpp : implementation file
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "DlgSet.h"
#include "DlgTimedReset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSet dialog


CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSet)
	DDX_Control(pDX, IDC_TAB1, m_settab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSet, CDialog)
	//{{AFX_MSG_MAP(CDlgSet)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, OnBtnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSet message handlers

BOOL CDlgSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	str.LoadString(IDS_STRING191);
	m_settab.InsertItem(0, str);
	str.LoadString(IDS_STRING192);
	m_settab.InsertItem(1, str);
	str.LoadString(IDS_STRING193);
	m_settab.InsertItem(2, str);
	str.LoadString(IDS_STRING194);
	m_settab.InsertItem(3, str);
	str.LoadString(IDS_STRING195);
	m_settab.InsertItem(4, str);
	str.LoadString(IDS_STRING196);
	m_settab.InsertItem(5, str);

	DWORD m_chsnum,m_alarminnum,m_outputnum;
	m_chsnum = m_alarminnum = m_outputnum = 0;

	VSNET_ClientGetChannelAmount(m_sername,m_url,&m_chsnum,&m_alarminnum,&m_outputnum,m_port);
	strcpy(m_sercfg.m_servername,m_sername);
	strcpy(m_sercfg.m_url,m_url);
	strcpy(m_sercfg.m_username,m_username);
	strcpy(m_sercfg.m_password,m_password);
	m_sercfg.m_port = m_port;
	m_sercfg.Create(IDD_DIG_SERVERCFG,&m_settab);
	m_sercfg.ShowWindow(SW_SHOW);

	strcpy(m_chancfg.m_servername,m_sername);
	strcpy(m_chancfg.m_url,m_url);
	strcpy(m_chancfg.m_username,m_username);
	strcpy(m_chancfg.m_password,m_password);
	m_chancfg.m_port = m_port;
	m_chancfg.m_chnums = m_chsnum;
	m_chancfg.Create(IDD_DLG_CHANNEL,&m_settab);
	m_chancfg.ShowWindow(SW_HIDE);

	strcpy(m_serialcfg.m_servername,m_sername);
	strcpy(m_serialcfg.m_url,m_url);
	strcpy(m_serialcfg.m_username,m_username);
	strcpy(m_serialcfg.m_password,m_password);
	m_serialcfg.m_port = m_port;
	m_serialcfg.Create(IDD_DLG_SEAIAL,&m_settab);
	m_serialcfg.ShowWindow(SW_HIDE);

	strcpy(m_cfgalarm.m_servername,m_sername);
	strcpy(m_cfgalarm.m_url,m_url);
	strcpy(m_cfgalarm.m_username,m_username);
	strcpy(m_cfgalarm.m_password,m_password);
	m_cfgalarm.m_port = m_port;
	m_cfgalarm.m_chnums      = m_chsnum;
	m_cfgalarm.m_alarminnums = m_alarminnum;
	m_cfgalarm.m_outputnums  = m_outputnum;
	m_cfgalarm.Create(IDD_DLGCFG_ALARM,&m_settab);
	m_cfgalarm.ShowWindow(SW_HIDE);
	
	
	strcpy(m_timedreset.m_servername,m_sername);
	strcpy(m_timedreset.m_url,m_url);
	strcpy(m_timedreset.m_username,m_username);
	strcpy(m_timedreset.m_password,m_password);
	m_timedreset.m_port = m_port;
	m_timedreset.m_chnums     = m_chsnum;
	m_timedreset.Create(IDD_DIG_TIMED_RESET,&m_settab);
	m_timedreset.ShowWindow(SW_HIDE);

	strcpy(m_hostnotify.m_servername,m_sername);
	strcpy(m_hostnotify.m_url,m_url);
	strcpy(m_hostnotify.m_username,m_username);
	strcpy(m_hostnotify.m_password,m_password);
	m_hostnotify.m_port = m_port;
	m_hostnotify.m_chnums     = m_chsnum;
	m_hostnotify.Create(IDD_DLG_HOSTNOTIFY,&m_settab);
	m_hostnotify.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSet::OnOK() 
{
	CDialog::OnOK();
}

void CDlgSet::OnCancel() 
{	
	CDialog::OnCancel();
}

void CDlgSet::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_settab.GetCurSel() == 0)
	{
		m_sercfg.ShowWindow(SW_SHOW);
		m_chancfg.ShowWindow(SW_HIDE);
		m_serialcfg.ShowWindow(SW_HIDE);
		m_cfgalarm.ShowWindow(SW_HIDE);
		m_timedreset.ShowWindow(SW_HIDE);
		m_hostnotify.ShowWindow(SW_HIDE);
	}
	else if(m_settab.GetCurSel() == 1)
	{
		m_sercfg.ShowWindow(SW_HIDE);
		m_chancfg.ShowWindow(SW_SHOW);
		m_serialcfg.ShowWindow(SW_HIDE);
		m_cfgalarm.ShowWindow(SW_HIDE);
		m_timedreset.ShowWindow(SW_HIDE);
		m_hostnotify.ShowWindow(SW_HIDE);
	}
	else if(m_settab.GetCurSel() == 2)
	{
		m_serialcfg.ShowWindow(SW_SHOW);
		m_sercfg.ShowWindow(SW_HIDE);
		m_chancfg.ShowWindow(SW_HIDE);
		m_cfgalarm.ShowWindow(SW_HIDE);
		m_timedreset.ShowWindow(SW_HIDE);
		m_hostnotify.ShowWindow(SW_HIDE);
	}
	else if(m_settab.GetCurSel() == 3)
	{
		m_cfgalarm.ShowWindow(SW_SHOW);
		m_sercfg.ShowWindow(SW_HIDE);
		m_chancfg.ShowWindow(SW_HIDE);
		m_serialcfg.ShowWindow(SW_HIDE);
		m_timedreset.ShowWindow(SW_HIDE);
		m_hostnotify.ShowWindow(SW_HIDE);
	}
	else if(m_settab.GetCurSel() == 4)
	{
		m_cfgalarm.ShowWindow(SW_HIDE);
		m_sercfg.ShowWindow(SW_HIDE);
		m_chancfg.ShowWindow(SW_HIDE);
		m_serialcfg.ShowWindow(SW_HIDE);
		m_timedreset.ShowWindow(SW_SHOW);
		m_hostnotify.ShowWindow(SW_HIDE);
	}
	else if(m_settab.GetCurSel() == 5)
	{
		m_cfgalarm.ShowWindow(SW_HIDE);
		m_sercfg.ShowWindow(SW_HIDE);
		m_chancfg.ShowWindow(SW_HIDE);
		m_serialcfg.ShowWindow(SW_HIDE);
		m_timedreset.ShowWindow(SW_HIDE);
		m_hostnotify.ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

void CDlgSet::OnBtnSave() 
{
	CString str;
	if(!VSNET_ClientSaveServerPara(m_sername,m_url,m_username,m_password,m_port))
	{
		str.LoadString(IDS_STRING197);
		AfxMessageBox(str);
	}
	else
	{
		str.LoadString(IDS_STRING198);
		AfxMessageBox(str);
	}
}

void CDlgSet::OnBtnDefault() 
{
	CString str;
	str.LoadString(IDS_STRING199);
	if(MessageBox(str,"",MB_ICONQUESTION|MB_OKCANCEL) != IDOK)
		return;
	if(!VSNET_ClientDefaultPara(m_sername,m_url,m_username,m_password,m_port))
	{
		str.LoadString(IDS_STRING200);
		AfxMessageBox(str);
	}
	else
	{
		str.LoadString(IDS_STRING201);
		AfxMessageBox(str);
	}
}
