// DlgUserIP.cpp : implementation file
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "DlgUserIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUserIP dialog

CDlgUserIP::CDlgUserIP(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUserIP)
	m_usernums = 0;
	//}}AFX_DATA_INIT
	m_channs = 0;
	memset(&m_connectip,0,sizeof(CONNECTIP));
}


void CDlgUserIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUserIP)
	DDX_Control(pDX, IDC_LISTUER, m_ListCam);
	DDX_Text(pDX, IDC_EDIT_USERNUM, m_usernums);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUserIP, CDialog)
	//{{AFX_MSG_MAP(CDlgUserIP)
	ON_CBN_SELCHANGE(IDC_CHANN, OnSelchangeChann)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUserIP message handlers

void CDlgUserIP::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CDlgUserIP::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	DWORD i;
	char strch[30];
	m_ListCam.InsertColumn(0, "IP"  , LVCFMT_LEFT, 180);

	DWORD s_alarmin = 0,s_alarmout = 0;
	if(VSNET_ClientGetChannelAmount(m_sername,m_url,&m_channs,&s_alarmin,&s_alarmout,m_port))
	{
		for(i=0;i<m_channs;i++)
		{
			str.LoadString(IDS_STRING178);
			sprintf(strch,str,i+1);
			((CComboBox*)GetDlgItem(IDC_CHANN))->AddString(strch);
		}
		((CComboBox*)GetDlgItem(IDC_CHANN))->SetCurSel(0);
		if(m_channs == 1)
			GetDlgItem(IDC_CHANN)->EnableWindow(FALSE);

		memset(&m_connectip,0,sizeof(CONNECTIP));
		if(VSNET_ClientGetConnectIP(m_sername,m_url,m_username,m_password,&m_connectip,m_port))
		{	
			OnSelchangeChann();	
		}
		else
		{
			AfxMessageBox(IDS_STRING216);
		}
	}
	else
	{
		AfxMessageBox(IDS_STRING217);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUserIP::OnSelchangeChann() 
{
	int nItem;
	int i;
	CString strip;
	int ch = ((CComboBox*)GetDlgItem(IDC_CHANN))->GetCurSel();
	if(ch == -1) return;
	m_ListCam.DeleteAllItems();
	m_usernums = m_connectip.m_chanuser[ch];
	for(i=0;i<m_connectip.m_chanuser[ch];i++)
	{
		strip.Format("%d.%d.%d.%d",
			m_connectip.m_chanIP[ch][i][0],
			m_connectip.m_chanIP[ch][i][1],
			m_connectip.m_chanIP[ch][i][2],
			m_connectip.m_chanIP[ch][i][3]);
		nItem = m_ListCam.InsertItem(0,strip);
	}
	UpdateData(FALSE);
}
