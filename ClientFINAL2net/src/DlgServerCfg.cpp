// DlgServerCfg.cpp : implementation file
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "DlgServerCfg.h"

#include "DlgSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgServerCfg dialog
static WHOLEPARAM s_serWholePara;
static WHOLE_USER s_seruser;
static VSREMOTEHOST s_remotehost;

CDlgServerCfg::CDlgServerCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgServerCfg)
	m_sername = _T("");
	m_serip = _T("");
	m_submask = _T("");
	m_dns = _T("");
	m_getway = _T("");
	m_multiip = _T("");
	m_serport = 0;
	m_multiport = 0;
	m_webport = 0;
	m_wldz = _T("");
	m_version = _T("");
	m_nndsenable = FALSE;
	m_ispal = -1;
	m_launage = -1;
	m_adminname = _T("");
	m_adminpass = _T("");
	m_ddnsip = _T("");
	m_ddnsport = 0;
	m_ddnsmapport = 0;
	m_hipass = _T("");
	m_hiusername = _T("");
	m_opername = _T("");
	m_operpass = _T("");
	m_alarmport = 0;
	m_alarmurl = _T("");
	m_recordport = 0;
	m_recordurl = _T("");
	//}}AFX_DATA_INIT
}


void CDlgServerCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgServerCfg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_sername);
	DDV_MaxChars(pDX, m_sername, 24);
	DDX_Text(pDX, IDC_EDIT_IP, m_serip);
	DDV_MaxChars(pDX, m_serip, 15);
	DDX_Text(pDX, IDC_EDIT_SUBMASK, m_submask);
	DDV_MaxChars(pDX, m_submask, 15);
	DDX_Text(pDX, IDC_EDIT_DNS, m_dns);
	DDV_MaxChars(pDX, m_dns, 15);
	DDX_Text(pDX, IDC_EDIT_GETWAY, m_getway);
	DDV_MaxChars(pDX, m_getway, 15);
	DDX_Text(pDX, IDC_EDIT_MULTIIP, m_multiip);
	DDV_MaxChars(pDX, m_multiip, 15);
	DDX_Text(pDX, IDC_EDIT_SERPORT, m_serport);
	DDX_Text(pDX, IDC_EDIT_MULTIPORT, m_multiport);
	DDX_Text(pDX, IDC_EDIT_WEBPORT, m_webport);
	DDX_Text(pDX, IDC_EDIT_WLDZ, m_wldz);
	DDX_Text(pDX, IDC_EDIT_VERSION, m_version);
	DDX_Check(pDX, IDC_CHECK_DDNS, m_nndsenable);
	DDX_CBIndex(pDX, IDC_COMBO_ISPAL, m_ispal);
	DDX_CBIndex(pDX, IDC_COMBO_LAUNAGE, m_launage);
	DDX_Text(pDX, IDC_EDIT_ADMINNAME, m_adminname);
	DDV_MaxChars(pDX, m_adminname, 10);
	DDX_Text(pDX, IDC_EDIT_ADMINPASS, m_adminpass);
	DDV_MaxChars(pDX, m_adminpass, 10);
	DDX_Text(pDX, IDC_EDIT_DDNSIP, m_ddnsip);
	DDV_MaxChars(pDX, m_ddnsip, 15);
	DDX_Text(pDX, IDC_EDIT_DDNSMAPORT, m_ddnsport);
	DDX_Text(pDX, IDC_EDIT_DDNSPORT, m_ddnsmapport);
	DDX_Text(pDX, IDC_EDIT_HIPASS, m_hipass);
	DDV_MaxChars(pDX, m_hipass, 10);
	DDX_Text(pDX, IDC_EDIT_HIUSERNAME, m_hiusername);
	DDV_MaxChars(pDX, m_hiusername, 10);
	DDX_Text(pDX, IDC_EDIT_OPERNAME, m_opername);
	DDV_MaxChars(pDX, m_opername, 10);
	DDX_Text(pDX, IDC_EDIT_OPERPASS, m_operpass);
	DDV_MaxChars(pDX, m_operpass, 10);
	DDX_Text(pDX, IDC_EDIT_ALARMPORT, m_alarmport);
	DDV_MinMaxUInt(pDX, m_alarmport, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_ALARMURL, m_alarmurl);
	DDV_MaxChars(pDX, m_alarmurl, 36);
	DDX_Text(pDX, IDC_EDIT_RECORDPORT, m_recordport);
	DDV_MinMaxUInt(pDX, m_recordport, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_RECORDURL, m_recordurl);
	DDV_MaxChars(pDX, m_recordurl, 36);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgServerCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgServerCfg)
	ON_BN_CLICKED(IDC_BTN_SETTIME, OnBtnSettime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgServerCfg message handlers

BOOL CDlgServerCfg::OnInitDialog() 
{
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);
	CDialog::OnInitDialog();
	CString str;
	// TODO: Add extra initialization here
	if(VSNET_ClientGetUser(m_servername,m_url,m_username,m_password,&s_seruser,m_port))
	{
		m_adminname = s_seruser.m_admin.m_username;
		m_adminpass = s_seruser.m_admin.m_password;
		m_hipass    = s_seruser.hl_operator[0].m_password;
		m_hiusername = s_seruser.hl_operator[0].m_username;
		m_opername = s_seruser.m_operator[0].m_username;
		m_operpass = s_seruser.m_operator[0].m_password;
	}
	if(VSNET_ClientGetWholePara(m_servername,m_url,m_username,m_password,&s_serWholePara,m_port))
	{
		m_sername = s_serWholePara.m_servername;
		m_serip   = s_serWholePara.m_serverip;
		m_submask = s_serWholePara.m_servermask;
		m_dns     = s_serWholePara.m_dnsaddr;
		m_getway  = s_serWholePara.m_gatewayAddr;
		m_multiip = s_serWholePara.m_multiAddr;
		m_serport = s_serWholePara.m_serport;
		m_multiport = s_serWholePara.m_mulport;
		m_webport = s_serWholePara.m_webport;
		m_wldz.Format("%02X-%02X-%02X-%02X-%02X-%02X",
			s_serWholePara.m_phyAddr[0],s_serWholePara.m_phyAddr[1],s_serWholePara.m_phyAddr[2],
			s_serWholePara.m_phyAddr[3],s_serWholePara.m_phyAddr[4],s_serWholePara.m_phyAddr[5]);
		m_version    = s_serWholePara.m_version.pStrAPPVer;
		m_nndsenable = s_serWholePara.m_ddns.bUseDDNS;
		m_ispal      = s_serWholePara.m_isPAL;
		m_launage    = s_serWholePara.m_launage - 1;
		m_ddnsip     = s_serWholePara.m_ddns.DDNSSerIp;
		m_ddnsport   = s_serWholePara.m_ddns.DDNSSerPort;
		m_ddnsmapport = s_serWholePara.m_ddns.LocalMapPort;
	}
	else
	{
		str.LoadString(IDS_STRING188);
		AfxMessageBox(str);
	}

	memset(&s_remotehost,0,sizeof(VSREMOTEHOST));
	if(!VSNET_ClientGetRemoteHost(m_servername,m_url,m_username,m_password,&s_remotehost,m_port))
	{
		str.LoadString(IDS_STRING188);
		AfxMessageBox(str);
	}
	else
	{
		m_alarmport = s_remotehost.m_alarserverport;
		m_alarmurl.Format("%s",s_remotehost.m_alarservermurl);
		m_recordport = s_remotehost.m_hostserverport;
		m_recordurl.Format("%s",s_remotehost.m_hostserverurl);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgServerCfg::OnOK() 
{	
	UpdateData(TRUE);

	sprintf(s_seruser.m_admin.m_username,"%s",m_adminname);
	sprintf(s_seruser.m_admin.m_password,"%s",m_adminpass);
	sprintf(s_seruser.hl_operator[0].m_password,"%s",m_hipass);
	sprintf(s_seruser.hl_operator[0].m_username,"%s",m_hiusername);
	sprintf(s_seruser.m_operator[0].m_username,"%s",m_opername);
	sprintf(s_seruser.m_operator[0].m_password,"%s",m_operpass);

	sprintf(s_serWholePara.m_servername,"%s",m_sername);
	sprintf(s_serWholePara.m_serverip,"%s",m_serip);
	sprintf(s_serWholePara.m_servermask,"%s",m_submask);
	sprintf(s_serWholePara.m_dnsaddr,"%s",m_dns);
	sprintf(s_serWholePara.m_gatewayAddr,"%s",m_getway);
	sprintf(s_serWholePara.m_multiAddr,"%s",m_multiip);
	s_serWholePara.m_serport = m_serport;
	s_serWholePara.m_mulport = m_multiport;
	s_serWholePara.m_webport = m_webport;
	s_serWholePara.m_ddns.bUseDDNS = m_nndsenable;
	s_serWholePara.m_isPAL = m_ispal;
	s_serWholePara.m_launage = m_launage + 1;
	sprintf(s_serWholePara.m_ddns.DDNSSerIp,"%s",m_ddnsip);
	s_serWholePara.m_ddns.DDNSSerPort = m_ddnsport;
	s_serWholePara.m_ddns.LocalMapPort  = m_ddnsmapport;

	s_remotehost.m_alarserverport = m_alarmport;
	sprintf(s_remotehost.m_alarservermurl,"%s",m_alarmurl);

	s_remotehost.m_hostserverport = m_recordport;
	sprintf(s_remotehost.m_hostserverurl,"%s",m_recordurl);

	VSNET_ClientSetUser(m_servername,m_url,m_username,m_password,&s_seruser,m_port);
	VSNET_ClientSetWholePara(m_servername,m_url,m_username,m_password,&s_serWholePara,m_port);
	VSNET_ClientSetRemoteHost(m_servername,m_url,m_username,m_password,&s_remotehost,m_port);
}

void CDlgServerCfg::OnBtnSettime() 
{	
	SYSTEMTIME dwTime = {0};
	TIME_PARAM ptimepara;

	CString str;
	GetLocalTime(&dwTime);
	ptimepara.m_year  = dwTime.wYear;
	ptimepara.m_month = (BYTE)dwTime.wMonth;
	ptimepara.m_dayofmonth = (BYTE)dwTime.wDay;
	ptimepara.m_dayofweek  = (BYTE)dwTime.wDayOfWeek;
	ptimepara.m_hour       = (BYTE)dwTime.wHour;
	ptimepara.m_minute     = (BYTE)dwTime.wMinute;
	ptimepara.m_second     = (BYTE)dwTime.wSecond;
	if(VSNET_ClientSetSystime(m_servername,m_url,m_username,m_password,&ptimepara,m_port))
	{
		str.LoadString(IDS_STRING189);
		AfxMessageBox(str);
	}
	else
	{
		str.LoadString(IDS_STRING190);
		AfxMessageBox(str);
	}
}
