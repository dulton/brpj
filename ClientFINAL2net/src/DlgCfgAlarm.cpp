// DlgCfgAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgCfgAlarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgAlarm dialog


CDlgCfgAlarm::CDlgCfgAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCfgAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCfgAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCfgAlarm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCfgAlarm, CDialog)
	//{{AFX_MSG_MAP(CDlgCfgAlarm)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_CBN_SELCHANGE(IDC_COMBO_CHSEL, OnSelchangeComboChsel)
	ON_CBN_SELCHANGE(IDC_COMBO_PTZCHSEL, OnSelchangeComboPtzchsel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgAlarm message handlers

BOOL CDlgCfgAlarm::OnInitDialog() 
{
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);
	CDialog::OnInitDialog();
	
	int i;
	CString strch;
	for(i = 0; i < m_chnums ; i++)
	{
		strch.Format(IDS_STRING178,i+1);
		((CComboBox*)GetDlgItem(IDC_COMBO_PTZCHSEL))->AddString(strch);
	}
	for(i = m_chnums;i<MAX_CHANNEL;i++)
		GetDlgItem(IDC_CHECK_RECV1 + i)->ShowWindow(SW_HIDE);
	for(i = m_outputnums;i<ALARM_OUT_NUM;i++)
		GetDlgItem(IDC_CHECK_ALARMOUT1 + i)->ShowWindow(SW_HIDE);
	
	for(i=0;i<m_alarminnums;i++)
	{
		strch.Format(IDS_STRING179,i+1);
		((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->AddString(strch);
	}
	if(m_chnums <= 1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_PTZCHSEL))->EnableWindow(FALSE);
	}
	memset(&m_alarmpara,0,sizeof(ALARMPARAM));
	VSNET_ClientGetAlarmRecord(m_servername,m_url,m_username,m_password,&m_alarmpara,m_port);

	m_cursenno = -1;
	m_curptzno = -1;
	((CButton*)GetDlgItem(IDC_CHECK_ON))->SetCheck(m_alarmpara.m_benab);
	((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->SetCurSel(0);
	OnSelchangeComboChsel();

	GetDlgItem(IDC_BTN_SET)->EnableWindow(TRUE);
	if(m_alarminnums > 1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCfgAlarm::OnBtnSet() 
{
	OnSelchangeComboChsel();
	m_alarmpara.m_benab = ((CButton*)GetDlgItem(IDC_CHECK_ON))->GetCheck();
	VSNET_ClientSetAlarmRecord(m_servername,m_url,m_username,m_password,&m_alarmpara,m_port);
}

void CDlgCfgAlarm::OnSelchangeComboChsel() 
{
	int i;
	char lpnum[3];
	int ch;
	if(m_cursenno != -1)
	{
		ch = m_cursenno;
		for(i=0;i<7;i++)
		{
			m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_maskweek = ((CComboBox*)GetDlgItem(IDC_COMBO_TIME1 + i))->GetCurSel();
			GetDlgItem(IDC_STARTHOUR1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_starthour = atoi(lpnum);
			
			GetDlgItem(IDC_STARTMINUTE1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_startmin = atoi(lpnum);
			
			GetDlgItem(IDC_STOPHOUR1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_stophour = atoi(lpnum);
			
			GetDlgItem(IDC_STOPMINUTE1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_stopmin = atoi(lpnum);
		}
		for(i=0;i<MAX_CHANNEL;i++)
		{
			m_alarmpara.m_alarmrec[ch].m_record[i] = ((CButton*)GetDlgItem(IDC_CHECK_RECV1 + i))->GetCheck();
		}
		for(i=0;i<ALARM_OUT_NUM;i++)
		{
			m_alarmpara.m_alarmrec[ch].m_out[i] = ((CButton*)GetDlgItem(IDC_CHECK_ALARMOUT1 + i))->GetCheck();
		}
		if(m_curptzno != -1)
		{
			m_alarmpara.m_alarmrec[ch].m_enpreno[m_curptzno] = ((CButton*)GetDlgItem(IDC_CHECK_PTZCALL))->GetCheck();
			GetDlgItem(IDC_EDIT_PTZNO)->GetWindowText(lpnum,sizeof(lpnum));
			m_alarmpara.m_alarmrec[ch].m_preno[m_curptzno] = atoi(lpnum);
		}
	}
	m_cursenno = ch = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	for(i=0;i<7;i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_TIME1 + i))->SetCurSel(m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_maskweek);

		sprintf(lpnum,"%02d",m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_starthour);
		GetDlgItem(IDC_STARTHOUR1 + i)->SetWindowText(lpnum);
		sprintf(lpnum,"%02d",m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_startmin);
		GetDlgItem(IDC_STARTMINUTE1 + i)->SetWindowText(lpnum);

		sprintf(lpnum,"%02d",m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_stophour);
		GetDlgItem(IDC_STOPHOUR1 + i)->SetWindowText(lpnum);
		sprintf(lpnum,"%02d",m_alarmpara.m_alarmrec[ch].m_timelist.pList[i].m_stopmin);
		GetDlgItem(IDC_STOPMINUTE1 + i)->SetWindowText(lpnum);
	}
	for(i=0;i<MAX_CHANNEL;i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_RECV1 + i))->SetCheck(m_alarmpara.m_alarmrec[ch].m_record[i]);
	}
	for(i=0;i<ALARM_OUT_NUM;i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ALARMOUT1 + i))->SetCheck(m_alarmpara.m_alarmrec[ch].m_out[i]);
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_PTZCHSEL))->SetCurSel(0);
	m_curptzno = -1;
	OnSelchangeComboPtzchsel();
}

void CDlgCfgAlarm::OnSelchangeComboPtzchsel() 
{
	char str[10];
	int senno = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	int chno  = ((CComboBox*)GetDlgItem(IDC_COMBO_PTZCHSEL))->GetCurSel();
	if(m_curptzno != -1)
	{
		m_alarmpara.m_alarmrec[senno].m_enpreno[m_curptzno] = ((CButton*)GetDlgItem(IDC_CHECK_PTZCALL))->GetCheck();
		GetDlgItem(IDC_EDIT_PTZNO)->GetWindowText(str,sizeof(str));
		m_alarmpara.m_alarmrec[senno].m_preno[m_curptzno] = atoi(str);
	}
	m_curptzno = chno;
	((CButton*)GetDlgItem(IDC_CHECK_PTZCALL))->SetCheck(m_alarmpara.m_alarmrec[senno].m_enpreno[m_curptzno]);
	sprintf(str,"%d",m_alarmpara.m_alarmrec[senno].m_preno[m_curptzno]);
	GetDlgItem(IDC_EDIT_PTZNO)->SetWindowText(str);
}

void CDlgCfgAlarm::OnOK() 
{	
	//CDialog::OnOK();
}


void CDlgCfgAlarm::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}
