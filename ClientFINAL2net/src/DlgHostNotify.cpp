// DlgHostNotify.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgHostNotify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHostNotify dialog


CDlgHostNotify::CDlgHostNotify(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHostNotify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHostNotify)
	m_data = _T("");
	m_intervaltime = 0;
	m_hosturl = _T("");
	m_hostport = 0;
	//}}AFX_DATA_INIT
	memset(&m_notify,0,sizeof(VSNETHOSTNOTIFY));
}


void CDlgHostNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHostNotify)
	DDX_Text(pDX, IDC_EDIT_DATA, m_data);
	DDV_MaxChars(pDX, m_data, 512);
	DDX_Text(pDX, IDC_EDIT_INTERVALTIME, m_intervaltime);
	DDV_MinMaxInt(pDX, m_intervaltime, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_HOSTURL, m_hosturl);
	DDV_MaxChars(pDX, m_hosturl, 40);
	DDX_Text(pDX, IDC_EDIT_HOSTPORT, m_hostport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHostNotify, CDialog)
	//{{AFX_MSG_MAP(CDlgHostNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHostNotify message handlers

BOOL CDlgHostNotify::OnInitDialog() 
{
	CString str;
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);
	CDialog::OnInitDialog();
	
	if(VSNET_ClientGetHostNotify(m_servername,m_url,m_username,m_password,&m_notify,m_port))
	{
		m_data.Format("%s",m_notify.m_senddata);
		m_hostport = m_notify.m_port;
		m_intervaltime = m_notify.m_intervaltime;
		m_hosturl.Format("%s",m_notify.m_serverIP);
	}
	else
	{
		str.LoadString(IDS_STRING181);
		AfxMessageBox(str);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHostNotify::OnOK() 
{
	if(!UpdateData(TRUE)) return;
	CString str;
	sprintf(m_notify.m_senddata,"%s",m_data);
	sprintf(m_notify.m_serverIP,"%s",m_hosturl);
	m_notify.m_port = m_hostport;
	m_notify.m_intervaltime = m_intervaltime;


	if(!VSNET_ClientSetHostNotify(m_servername,m_url,m_username,m_password,&m_notify,m_port))
	{
		str.LoadString(IDS_STRING182);
		AfxMessageBox(str);
	}
}
