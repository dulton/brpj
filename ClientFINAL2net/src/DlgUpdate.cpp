// DlgUpdate.cpp : implementation file
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "DlgUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdate dialog

CDlgUpdate::CDlgUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpdate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUpdate)
	m_status = _T("状态");
	m_status.LoadString(IDS_STRING210);
	//}}AFX_DATA_INIT
	m_updatesock = -1;
	m_hEndEvent  = NULL;
	m_bUpdateNow = FALSE;
}


void CDlgUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUpdate)
	DDX_Text(pDX, IDC_STATUS, m_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUpdate, CDialog)
	//{{AFX_MSG_MAP(CDlgUpdate)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdate message handlers

void CDlgUpdate::OnCancel() 
{
	if(m_bUpdateNow) return;
	if(m_hEndEvent)
		CloseHandle(m_hEndEvent);
	CDialog::OnCancel();
}

void CDlgUpdate::OnBtnOk() 
{
	char filename[MAX_PATH];
	GetDlgItem(IDC_EDIT_FILE)->GetWindowText(filename,MAX_PATH);
	FILE *fpFile = fopen(filename,"rb");
	if(fpFile == NULL)
	{
		AfxMessageBox(IDS_STRING211);
		return;
	}
	fclose(fpFile);
	if(m_hEndEvent) CloseHandle(m_hEndEvent);
	m_hEndEvent = CreateEvent(0,FALSE,FALSE,0);
	m_updatesock = VSNET_ClientUpdateStart(m_url,m_username,m_password,filename,m_hEndEvent,m_port);
	if(m_updatesock == -1) 
	{
		AfxMessageBox(IDS_STRING212);
		return;
	}
	m_status.LoadString(IDS_STRING213);
	SetTimer(5,1000,NULL);
	UpdateData(FALSE);
	m_bUpdateNow = TRUE;
}

void CDlgUpdate::OnBtnFile() 
{
	//Debug模式下会报错
	CFileDialog dlg(TRUE,
		_T("itm文件"),
		"",
		OFN_HIDEREADONLY,
		"itm文件(*.itm)|*.itm||",
		NULL);
	CString sPath;
	if(dlg.DoModal() == IDOK && (sPath = dlg.GetPathName()) != "")
	{
		GetDlgItem(IDC_EDIT_FILE)->SetWindowText(sPath);
	}
}

void CDlgUpdate::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 5)
	{
		if(WaitForSingleObject(m_hEndEvent,0) == WAIT_OBJECT_0)
		{
			int ret;
			KillTimer(5);
			ret = VSNET_ClientUpdateStop(m_updatesock);
			m_updatesock = -1;
			if(ret == 0)
			{
				m_status.LoadString(IDS_STRING214);// = "状态：升级成功";
			}
			else
			{
				m_status.Format(IDS_STRING215,ret);
			}
			UpdateData(FALSE);
			m_bUpdateNow = FALSE;
		}
	}
	CDialog::OnTimer(nIDEvent);
}
