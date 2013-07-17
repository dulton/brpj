// DlgCfgTimelist.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgCfgTimelist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgTimelist dialog


CDlgCfgTimelist::CDlgCfgTimelist(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgTimelist::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCfgTimelist)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCfgTimelist::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCfgTimelist)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCfgTimelist, CDialog)
	//{{AFX_MSG_MAP(CDlgCfgTimelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgTimelist message handlers

void CDlgCfgTimelist::OnOK() 
{
	int i;
	char lpnum[3];
	for(i=0;i<7;i++)
	{
		m_timelist->pList[i].m_maskweek = ((CComboBox*)GetDlgItem(IDC_COMBO_TIME1 + i))->GetCurSel();
		GetDlgItem(IDC_STARTHOUR1 + i)->GetWindowText(lpnum,sizeof(lpnum));
		m_timelist->pList[i].m_starthour = atoi(lpnum);
		
		GetDlgItem(IDC_STARTMINUTE1 + i)->GetWindowText(lpnum,sizeof(lpnum));
		m_timelist->pList[i].m_startmin = atoi(lpnum);
		
		GetDlgItem(IDC_STOPHOUR1 + i)->GetWindowText(lpnum,sizeof(lpnum));
		m_timelist->pList[i].m_stophour = atoi(lpnum);
		
		GetDlgItem(IDC_STOPMINUTE1 + i)->GetWindowText(lpnum,sizeof(lpnum));
		m_timelist->pList[i].m_stopmin = atoi(lpnum);
	}
	if(!m_bIsReclist)
	{
		m_brec = ((CButton*)GetDlgItem(IDC_CHECK_RECV1))->GetCheck();
		for(i=0;i<ALARM_OUT_NUM;i++)
		{
			m_output[i] = ((CButton*)GetDlgItem(IDC_CHECK_ALARMOUT1 + i))->GetCheck();
		}
	}
	CDialog::OnOK();
}

void CDlgCfgTimelist::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CDlgCfgTimelist::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i;
	char lpnum[3];
	if(m_bIsReclist)
	{
		GetDlgItem(IDC_STATIC_REC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_OUTPUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_RECV1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ALARMOUT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ALARMOUT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ALARMOUT3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ALARMOUT4)->ShowWindow(SW_HIDE);
	}
	for(i=0;i<7;i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_TIME1 + i))->SetCurSel(m_timelist->pList[i].m_maskweek);

		sprintf(lpnum,"%02d",m_timelist->pList[i].m_starthour);
		GetDlgItem(IDC_STARTHOUR1 + i)->SetWindowText(lpnum);
		sprintf(lpnum,"%02d",m_timelist->pList[i].m_startmin);
		GetDlgItem(IDC_STARTMINUTE1 + i)->SetWindowText(lpnum);

		sprintf(lpnum,"%02d",m_timelist->pList[i].m_stophour);
		GetDlgItem(IDC_STOPHOUR1 + i)->SetWindowText(lpnum);
		sprintf(lpnum,"%02d",m_timelist->pList[i].m_stopmin);
		GetDlgItem(IDC_STOPMINUTE1 + i)->SetWindowText(lpnum);
	}
	if(!m_bIsReclist)
	{
		((CButton*)GetDlgItem(IDC_CHECK_RECV1))->SetCheck(m_brec);
		for(i=0;i<ALARM_OUT_NUM;i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_ALARMOUT1 + i))->SetCheck(m_output[i]);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
