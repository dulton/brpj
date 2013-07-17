// DlgSerialCfg.cpp : implementation file
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "DlgSerialCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialCfg dialog
CDlgSerialCfg::CDlgSerialCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSerialCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSerialCfg)
	m_addr = 0;
	m_bitrate = -1;
	m_checkbit = -1;
	m_databit = -1;
	m_overflow = -1;
	m_stopbit = -1;
	//}}AFX_DATA_INIT
}


void CDlgSerialCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSerialCfg)
	DDX_Text(pDX, IDC_ADDR, m_addr);
	DDV_MinMaxByte(pDX, m_addr, 0, 254);
	DDX_CBIndex(pDX, IDC_BITRATE, m_bitrate);
	DDX_CBIndex(pDX, IDC_CHECKBIT, m_checkbit);
	DDX_CBIndex(pDX, IDC_DATABIT, m_databit);
	DDX_CBIndex(pDX, IDC_OVERFLOW, m_overflow);
	DDX_CBIndex(pDX, IDC_STOPBIT, m_stopbit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSerialCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgSerialCfg)
	ON_CBN_SELCHANGE(IDC_CHANNUM, OnSelchangeChannum)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_BN_CLICKED(IDC_BTN_PTZFILESET, OnBtnPtzfileset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialCfg message handlers

BOOL CDlgSerialCfg::OnInitDialog() 
{
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);

	CDialog::OnInitDialog();

	DWORD s_channums = 0,s_alarmin = 0,s_alarmout = 0;
	if(VSNET_ClientGetChannelAmount(m_servername,m_url,&s_channums,&s_alarmin,&s_alarmout,m_port))
	{
		DWORD i;
		CString strch;
		for(i=0;i<s_channums;i++)
		{
			strch.Format(IDS_STRING178,i+1);
			((CComboBox*)GetDlgItem(IDC_CHANNUM))->AddString(strch);
			memset(&s_param485[i],0,sizeof(SERIAL485_PARAM));
			VSNET_ClientGetSerial485Param(m_servername,m_url,(char)i,m_username,m_password,&s_param485[i],m_port);
			m_lpptzcmdname[i][0] = '\0';
			VSNET_ClientGetPTZCommandName(m_servername,m_url,(char)i,m_username,m_password,m_lpptzcmdname[i],m_port);
		}
		((CComboBox*)GetDlgItem(IDC_CHANNUM))->SetCurSel(0);
		if(s_channums == 1)
		{
			((CComboBox*)GetDlgItem(IDC_CHANNUM))->EnableWindow(FALSE);
		}
		OnSelchangeChannum();
		GetDlgItem(IDC_BTN_PTZFILESET)->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSerialCfg::OnOK()
{	
	int curch = ((CComboBox*)GetDlgItem(IDC_CHANNUM))->GetCurSel();
	UpdateData(TRUE);

	s_param485[curch].decoderAddress = m_addr;
	if(m_bitrate == 0) s_param485[curch].m_baudrate      = 2400;
	else if(m_bitrate == 1) s_param485[curch].m_baudrate = 4800;
	else if(m_bitrate == 2) s_param485[curch].m_baudrate = 9600;
	s_param485[curch].m_checkbit = m_checkbit;
	if(m_databit == 0) s_param485[curch].m_databit = 5;
	else if(m_databit == 1) s_param485[curch].m_databit = 6;
	else if(m_databit == 2) s_param485[curch].m_databit = 7;
	else if(m_databit == 3) s_param485[curch].m_databit = 8;

	//s_param485[curch].decoderModel  = m_dectype;
	s_param485[curch].m_flowcontrol   = m_overflow;
	if(m_stopbit == 0) s_param485[curch].m_stopbit = 1;
	else							     m_stopbit = 2;

	VSNET_ClientSetSerial485Param(m_servername,m_url,(char)curch,m_username,m_password,&s_param485[curch],m_port);
}

void CDlgSerialCfg::OnSelchangeChannum() 
{
	int curch  = ((CComboBox*)GetDlgItem(IDC_CHANNUM))->GetCurSel();

	m_addr = s_param485[curch].decoderAddress;
	if(s_param485[curch].m_baudrate == 2400) m_bitrate = 0;
	else if(s_param485[curch].m_baudrate == 4800) m_bitrate = 1;
	else if(s_param485[curch].m_baudrate == 9600) m_bitrate = 2;
	else										  m_bitrate = 1;
	m_checkbit = s_param485[curch].m_checkbit;
	if(s_param485[curch].m_databit == 5) m_databit = 0;
	else if(s_param485[curch].m_databit == 6) m_databit = 1;
	else if(s_param485[curch].m_databit == 7) m_databit = 2;
	else if(s_param485[curch].m_databit == 8) m_databit = 3;
	else									  m_databit = 3;
	m_overflow = s_param485[curch].m_flowcontrol;
	if(s_param485[curch].m_stopbit == 1) m_stopbit = 0;
	else							     m_stopbit = 1;
		
	GetDlgItem(IDC_EDIT_PTZNAME)->SetWindowText(m_lpptzcmdname[curch]);
	UpdateData(FALSE);
}

void CDlgSerialCfg::OnBtnFile() 
{
	CFileDialog dlgFile(
		TRUE, 
		_T("open PTZ Cmmmand File"),
		"",
		OFN_HIDEREADONLY,
		"ptz Files(*.ptz)|*.ptz||",
		NULL);
	char lpszTitle[]={""};
	dlgFile.m_ofn.lpstrTitle = lpszTitle;
	
	if (dlgFile.DoModal() != IDOK)
	{
		
		return;
	}
	CString lpfile = dlgFile.GetPathName();
	GetDlgItem(IDC_EDIT_PTZFILE)->SetWindowText(lpfile);
}

void CDlgSerialCfg::OnBtnPtzfileset() 
{
	int iRet;
	CString lpfile;
	CString str;
	char lppztfile[MAX_PATH];
	GetDlgItem(IDC_EDIT_PTZFILE)->GetWindowText(lpfile);
	if(lpfile.IsEmpty())
	{
		GetDlgItem(IDC_BTN_FILE)->SetFocus();
		return;
	}
	sprintf(lppztfile,"%s",lpfile);
	int curch  = ((CComboBox*)GetDlgItem(IDC_CHANNUM))->GetCurSel();
	iRet = VSNET_ClientSetPTZCommandFile(m_servername,m_url,(char)curch,m_username,m_password,lppztfile,m_port);
	if(iRet == 0)
	{
		VSNET_ClientGetPTZCommandName(m_servername,m_url,(char)curch,m_username,m_password,m_lpptzcmdname[curch],m_port);
		GetDlgItem(IDC_EDIT_PTZNAME)->SetWindowText(m_lpptzcmdname[curch]);
		str.LoadString(IDS_STRING186);
		AfxMessageBox(str);
	}
	else
	{
		str.LoadString(IDS_STRING187);
		AfxMessageBox(str);
	}
}
