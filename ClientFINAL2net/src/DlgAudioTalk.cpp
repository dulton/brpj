// DlgAudioTalk.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgAudioTalk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAudioTalk dialog
static void WINAPI s_captureCallBack(char *pbuff,int size,void *userdata)
{
	LONG handle;
	CDlgAudioTalk *param = (CDlgAudioTalk*)userdata;
	for(int i = 0 ; i < param->m_arrTalkHandle.GetSize() ; i ++)
	{
		handle = param->m_arrTalkHandle.GetAt(i);
		if(handle != 0)
		{
			WaitForSingleObject(param->m_hTalkMutex, INFINITE);
			VSNET_ClientTalkSendData(handle,pbuff,size);
			ReleaseMutex(param->m_hTalkMutex);
		}
	}
}

CDlgAudioTalk::CDlgAudioTalk(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAudioTalk::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAudioTalk)
	m_addr = _T("");
	m_password = _T("");
	m_port = 3000;
	m_username = _T("");
	m_sername = _T("");
	//}}AFX_DATA_INIT
	m_hTalkMutex = NULL;
}


void CDlgAudioTalk::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAudioTalk)
	DDX_Control(pDX, IDC_LIST_TALK, m_list);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_addr);
	DDV_MaxChars(pDX, m_addr, 30);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 30);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_username);
	DDV_MaxChars(pDX, m_username, 30);
	DDX_Text(pDX, IDC_EDIT_SERNAME, m_sername);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAudioTalk, CDialog)
	//{{AFX_MSG_MAP(CDlgAudioTalk)
	ON_BN_CLICKED(IDC_BTN_REV, OnBtnRev)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TALK, OnClickListTalk)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MSG_AUDIOCAPEND, OnCaptureAudio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAudioTalk message handlers

void CDlgAudioTalk::OnBtnRev() 
{
	CString lpstr;
	char sername[40],username[40],password[40],url[40],urlold[40];
	int port;
	CString str;
	
	LONG handle = -1;

	if(m_list.GetSelectedCount() == 0)   return;
	int nItem = m_list.GetNextItem(-1, LVNI_SELECTED);
	if(nItem == -1) return;	
	if(UpdateData(TRUE))
	{
		if(m_addr.IsEmpty() || m_username.IsEmpty() || m_password.IsEmpty())
		{
			str.LoadString(IDS_STRING171);
			AfxMessageBox(str);
			return;
		}
		// Close Old Param
		handle = (LONG)m_list.GetItemData(nItem);
		WaitForSingleObject(m_hTalkMutex, INFINITE);
		VSNET_ClientTalkStop(handle);
		DeleteHandle(handle);
		ReleaseMutex(m_hTalkMutex);

		lpstr = m_list.GetItemText(nItem,0);
		sprintf(urlold,"%s",lpstr);
		port = atoi(m_list.GetItemText(nItem,1));

		sprintf(sername, "%s", m_sername);
		sprintf(username,"%s",m_username);
		sprintf(password,"%s",m_password);
		sprintf(url,"%s",m_addr);

		if(VSNET_ClientTalkStart(url,username,password,3,&handle,m_port,sername) == 0)
		{
			WaitForSingleObject(m_hTalkMutex, INFINITE);
			m_arrTalkHandle.Add(handle);
			ReleaseMutex(m_hTalkMutex);
		}
		else
		{
			str.LoadString(IDS_STRING172);
			AfxMessageBox(str);
			return;
		}

		m_list.SetItemText(nItem, 0, m_sername);
		m_list.SetItemText(nItem,1,m_addr);
		lpstr.Format("%d",m_port);
		m_list.SetItemText(nItem,2,lpstr);
		m_list.SetItemText(nItem,3,m_username);
		m_list.SetItemText(nItem,4,m_password);
		m_list.SetItemData(nItem, (DWORD)handle);
	}
}

void CDlgAudioTalk::OnBtnAdd() 
{
	CString lpstr;
	CString str;
	char sername[40],username[40],password[40],url[40];
	LONG handle = -1;

	if(UpdateData(TRUE))
	{
		if(m_addr.IsEmpty() || m_username.IsEmpty() || m_password.IsEmpty())
		{
			str.LoadString(IDS_STRING171);
			AfxMessageBox(str);
			return;
		}
		{
			sprintf(sername, "%s", m_sername);
			sprintf(username,"%s",m_username);
			sprintf(password,"%s",m_password);
			sprintf(url,"%s",m_addr);
			if(VSNET_ClientTalkStart(url,username,password,3, &handle, m_port, sername) != 0)
			{
				str.LoadString(IDS_STRING173);
				AfxMessageBox(str);
				return;
			}
		}
		int nItem = m_list.InsertItem(m_list.GetItemCount(),m_sername);
		m_list.SetItemText(nItem, 1, m_addr);
		lpstr.Format("%d",m_port);
		m_list.SetItemText(nItem,2,lpstr);
		m_list.SetItemText(nItem,3,m_username);
		m_list.SetItemText(nItem,4,m_password);
		m_list.SetItemData(nItem, (DWORD)handle);
		WaitForSingleObject(m_hTalkMutex, INFINITE);
		m_arrTalkHandle.Add(handle);
		ReleaseMutex(m_hTalkMutex);
	}
}

void CDlgAudioTalk::OnBtnDel() 
{
	if(m_list.GetSelectedCount() == 0)   return;
	int nItem = m_list.GetNextItem(-1, LVNI_SELECTED);
	if(nItem == -1) return;

	LONG handle = (LONG)m_list.GetItemData(nItem);
	WaitForSingleObject(m_hTalkMutex, INFINITE);
	if (VSNET_ClientTalkStop(handle))
		AfxMessageBox("Í£Ö¹³É¹¦£¡");
	else
		AfxMessageBox("Í£Ö¹Ê§°Ü£¡");
	DeleteHandle(handle);
	ReleaseMutex(m_hTalkMutex);
	m_list.DeleteItem(nItem);
}

BOOL CDlgAudioTalk::DeleteHandle(LONG hTalkHandle)
{
	for (int i = 0 ; i < m_arrTalkHandle.GetSize() ; i ++)
	{
		if (m_arrTalkHandle.GetAt(i) == hTalkHandle)
		{
			m_arrTalkHandle.RemoveAt(i);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDlgAudioTalk::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	str.LoadString(IDS_STRING228);
	m_list.InsertColumn(0, str, LVCFMT_LEFT, 100);
	str.LoadString(IDS_STRING111);
	m_list.InsertColumn(1, str  , LVCFMT_LEFT, 100);
	str.LoadString(IDS_STRING174);
	m_list.InsertColumn(2, str  , LVCFMT_LEFT, 70);
	str.LoadString(IDS_STRING175);
	m_list.InsertColumn(3,str  , LVCFMT_LEFT, 100);
	str.LoadString(IDS_STRING176);
	m_list.InsertColumn(4, str  , LVCFMT_LEFT, 100);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_hTalkMutex = CreateMutex(NULL, FALSE, NULL);
	m_input.m_CaptureCallBack = s_captureCallBack;
	m_input.userdata = this;
	m_input.m_hWnd = m_hWnd;
	m_input.StartCapture();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAudioTalk::OnClickListTalk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_list.GetSelectedCount() == 0)   return;
	int nItem = m_list.GetNextItem(-1, LVNI_SELECTED);	
	if(nItem == -1) return;
	m_sername = m_list.GetItemText(nItem, 0);
	m_addr = m_list.GetItemText(nItem,1);
	m_username = m_list.GetItemText(nItem,3);
	m_password = m_list.GetItemText(nItem,4);
	m_port = atoi(m_list.GetItemText(nItem,2));
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgAudioTalk::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_hTalkMutex != NULL)
	{
		CloseHandle(m_hTalkMutex);
		m_hTalkMutex = NULL;
	}
	m_input.FreeDirectSound();
	VSNET_ClientTalkFreeAll();
}

LRESULT CDlgAudioTalk::OnCaptureAudio(WPARAM,LPARAM)
{
	m_input.FreeDirectSound();
	return 0;
}
