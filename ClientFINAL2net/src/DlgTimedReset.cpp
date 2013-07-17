// DlgTimedReset.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgTimedReset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CHANNELHIDEALARM m_hidealarm[4];
/////////////////////////////////////////////////////////////////////////////
// CDlgTimedReset dialog

CDlgTimedReset::CDlgTimedReset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTimedReset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTimedReset)
	m_intervaltime = 0;
	m_resettime_hour = 0;
	m_resettime_minutes = 0;
	m_timedreset_enable = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTimedReset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTimedReset)
	DDX_Text(pDX, IDC_EDIT_INTERVAL_TIME, m_intervaltime);
	DDV_MinMaxInt(pDX, m_intervaltime, 10, 240);
	DDX_Text(pDX, IDC_EDIT_RESETTIME_HOUR, m_resettime_hour);
	DDV_MinMaxInt(pDX, m_resettime_hour, 0, 24);
	DDX_Text(pDX, IDC_EDIT_RESETTIME_MINUTES, m_resettime_minutes);
	DDV_MinMaxInt(pDX, m_resettime_minutes, 0, 60);
	DDX_Check(pDX, IDC_CHECK_TIMEDRESET_ENABLE, m_timedreset_enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTimedReset, CDialog)
	//{{AFX_MSG_MAP(CDlgTimedReset)
	ON_CBN_SELCHANGE(IDC_COMBO_CHSEL, OnSelchangeComboChsel)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTimedReset message handlers

BOOL CDlgTimedReset::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);

	VSTIMERRESET timereset;
	memset(&timereset,0,sizeof(VSTIMERRESET));
	VSNET_ClientGetTimerReset(m_servername,m_url,0,m_username,m_password,&timereset,m_port);
	m_timedreset_enable = timereset.m_benbale;
	m_intervaltime = timereset.m_invalidtime;
	m_resettime_hour = timereset.m_rsthour;
	m_resettime_minutes = timereset.m_rstminute;
	UpdateData(FALSE);


	int i;
	CString strch;
	memset(m_hidealarm,0,sizeof(CHANNELHIDEALARM));
	for(i = 0; i < m_chnums ; i++)
	{
		strch.Format(IDS_STRING178,i+1);
		((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->AddString(strch);
	}

	for(i = 0; i < m_chnums ; i++)
	{
		VSNET_ClientGetHideAlarm(m_servername,m_url,i,m_username,m_password,&m_hidealarm[i],m_port);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->SetCurSel(0);
	m_curchannel = -1;
	OnSelchangeComboChsel();

	((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->EnableWindow(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTimedReset::OnOK() 
{
	VSTIMERRESET timereset;
	UpdateData(TRUE);
	timereset.m_benbale = m_timedreset_enable;
	timereset.m_invalidtime = m_intervaltime;
	timereset.m_rsthour = m_resettime_hour;
	timereset.m_rstminute = m_resettime_minutes;
	VSNET_ClientSetTimerReset(m_servername,m_url,0,m_username,m_password,&timereset,m_port);

	CDialog::OnOK();
}

void CDlgTimedReset::OnSelchangeComboChsel() 
{
	char lpnum[3];
	int  ch,i;
	char str[2];

	if(m_curchannel != -1)
	{
		ch = m_curchannel;
		for(i=0;i<7;i++)
		{
			m_hidealarm[ch].pTimeList.pList[i].m_maskweek = ((CComboBox*)GetDlgItem(IDC_COMBO_TIME1 + i))->GetCurSel();
			GetDlgItem(IDC_STARTHOUR1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_hidealarm[ch].pTimeList.pList[i].m_starthour = atoi(lpnum);
			
			GetDlgItem(IDC_STARTMINUTE1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_hidealarm[ch].pTimeList.pList[i].m_startmin = atoi(lpnum);
			
			GetDlgItem(IDC_STOPHOUR1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_hidealarm[ch].pTimeList.pList[i].m_stophour = atoi(lpnum);
			
			GetDlgItem(IDC_STOPMINUTE1 + i)->GetWindowText(lpnum,sizeof(lpnum));
			m_hidealarm[ch].pTimeList.pList[i].m_stopmin = atoi(lpnum);
		}

		for(i=0;i<ALARM_OUT_NUM;i++)
		{
			m_hidealarm[ch].m_out[i] = ((CButton*)GetDlgItem(IDC_CHECK_ALARMOUT1 + i))->GetCheck();
		}

		m_hidealarm[ch].m_benable = ((CButton*)GetDlgItem(IDC_CHECK_HIDEALARM_ENABLE))->GetCheck();
		GetDlgItem(IDC_EDIT_HIDEALARM_VAL)->GetWindowText(str,sizeof(str));
		m_hidealarm[ch].m_decval = atoi(str);		
	}

	m_curchannel = ch = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	for(i=0;i<7;i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_TIME1 + i))->SetCurSel(m_hidealarm[ch].pTimeList.pList[i].m_maskweek);

		sprintf(lpnum,"%02d",m_hidealarm[ch].pTimeList.pList[i].m_starthour);
		GetDlgItem(IDC_STARTHOUR1 + i)->SetWindowText(lpnum);
		sprintf(lpnum,"%02d",m_hidealarm[ch].pTimeList.pList[i].m_startmin);
		GetDlgItem(IDC_STARTMINUTE1 + i)->SetWindowText(lpnum);

		sprintf(lpnum,"%02d",m_hidealarm[ch].pTimeList.pList[i].m_stophour);
		GetDlgItem(IDC_STOPHOUR1 + i)->SetWindowText(lpnum);
		sprintf(lpnum,"%02d",m_hidealarm[ch].pTimeList.pList[i].m_stopmin);
		GetDlgItem(IDC_STOPMINUTE1 + i)->SetWindowText(lpnum);
	}

	for(i=0;i<ALARM_OUT_NUM;i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ALARMOUT1 + i))->SetCheck(m_hidealarm[ch].m_out[i]);
	}
	((CButton*)GetDlgItem(IDC_CHECK_HIDEALARM_ENABLE))->SetCheck(m_hidealarm[ch].m_benable);
	sprintf(str,"%d",m_hidealarm[ch].m_decval);
	GetDlgItem(IDC_EDIT_HIDEALARM_VAL)->SetWindowText(str);
	VSNET_ClientSetHideAlarm(m_servername,m_url,ch,m_username,m_password,&m_hidealarm[m_curchannel],m_port);

}

void CDlgTimedReset::OnBtnSet() 
{
	VSTIMERRESET timereset;
	UpdateData(TRUE);
	timereset.m_benbale = m_timedreset_enable;
	timereset.m_invalidtime = m_intervaltime;
	timereset.m_rsthour = m_resettime_hour;
	timereset.m_rstminute = m_resettime_minutes;
	VSNET_ClientSetTimerReset(m_servername,m_url,0,m_username,m_password,&timereset,m_port);

	OnSelchangeComboChsel();
	
	int i;
	for(i = 0; i < m_chnums ; i++)
	{
		VSNET_ClientSetHideAlarm(m_servername,m_url,i,m_username,m_password,&m_hidealarm[i],m_port);
	}
}
