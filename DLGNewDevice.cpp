// DLGNewDevice.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGNewDevice.h"

#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGNewDevice dialog


CDLGNewDevice::CDLGNewDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGNewDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGNewDevice)
	m_ip = _T("");
	m_name = _T("");
	m_port = 0;
	m_pwd = _T("");
	m_user = _T("");
	m_playwin = _T("");
	//}}AFX_DATA_INIT
	ListChoose=-1;
}


void CDLGNewDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGNewDevice)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDV_MaxChars(pDX, m_ip, 32);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 32);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_PWD, m_pwd);
	DDV_MaxChars(pDX, m_pwd, 32);
	DDX_Text(pDX, IDC_EDIT_USER, m_user);
	DDV_MaxChars(pDX, m_user, 32);
	DDX_Text(pDX, IDC_STATIC_PLAYWIN, m_playwin);
	DDV_MaxChars(pDX, m_playwin, 5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGNewDevice, CDialog)
	//{{AFX_MSG_MAP(CDLGNewDevice)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnButtonClean)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGNewDevice message handlers
BOOL CDLGNewDevice::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();

	m_list.InsertColumn(0, _T("PlayWin") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(1, _T("Camera Name") , LVCFMT_LEFT, 120);
	m_list.InsertColumn(2, _T("IPaddr" ), LVCFMT_LEFT, 120);
	m_list.InsertColumn(3, _T("port" ), LVCFMT_LEFT, 50);
	m_list.InsertColumn(4, _T("Username" ), LVCFMT_LEFT, 80);
	m_list.InsertColumn(5, _T("Password" ), LVCFMT_LEFT, 80);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	int i,nItem;
	char str[8];
	for(i=0;i<MAX_PLAYWIN;i++)
	{
		sprintf(str,"%d",i+1);
		nItem = m_list.InsertItem(0,str);
		display(nItem,i);
	}
	ChooseEnable();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGNewDevice::ChooseEnable() 
{
	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PWD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CLEAN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PWD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLEAN)->EnableWindow(FALSE);
	}
}

void CDLGNewDevice::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDLGNewDevice::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDLGNewDevice::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;
	//选中的赋值
	ListChoose= p->iItem;
	if(ListChoose<0)
		return ;

	m_ip = device[ListChoose].ip;
	m_name = device[ListChoose].name;
	m_port = device[ListChoose].port;
	m_pwd = device[ListChoose].user;
	m_user = device[ListChoose].psw;

	char str[8];
	sprintf(str,"%d",ListChoose+1);
	m_playwin = str;

	UpdateData(FALSE);

	ChooseEnable();

	*pResult = 0;
}

void CDLGNewDevice::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	strcpy(device[ListChoose].ip,m_ip.GetBuffer(0));
	strcpy(device[ListChoose].name,m_name.GetBuffer(0));
	strcpy(device[ListChoose].user,m_user.GetBuffer(0));
	strcpy(device[ListChoose].psw,m_pwd.GetBuffer(0));
	device[ListChoose].port=m_port;

	//修改
	pCMainDlg->SQLiteIO.Camera_Modify(device[ListChoose]);
	display(ListChoose,ListChoose);
}

void CDLGNewDevice::OnButtonClean() 
{
	// TODO: Add your control notification handler code here
	m_ip = _T("");
	m_name = _T("");
	m_port = 0;
	m_pwd = _T("");
	m_user = _T("");
	UpdateData(FALSE);
	
	memset(device[ListChoose].ip,0,sizeof(device[ListChoose].ip));
	memset(device[ListChoose].name,0,sizeof(device[ListChoose].name));
	memset(device[ListChoose].user,0,sizeof(device[ListChoose].user));
	memset(device[ListChoose].psw,0,sizeof(device[ListChoose].psw));
	device[ListChoose].port=0;
	//修改
	pCMainDlg->SQLiteIO.Camera_Modify(device[ListChoose]);
	display(ListChoose,ListChoose);

}

void CDLGNewDevice::read()
{
	//读数据库 拷贝到数组
	pCMainDlg->SQLiteIO.Camera_Read(MAX_PLAYWIN,cameraList);

	list<struct CAMERA_INFO_ST>::iterator beglist;

	int i=0;
	for(beglist=cameraList.begin();beglist!=cameraList.end();beglist++)
	{	
		device[i].nid=beglist->nid;
		strcpy(device[i].ip,beglist->ip);
		strcpy(device[i].name,beglist->name);
		device[i].port=beglist->port;
		strcpy(device[i].user,beglist->user);
		strcpy(device[i].psw,beglist->psw);

		i++;
	}
}

void CDLGNewDevice::display(int nItem,int i)
{
	char str[8];
	//显示到列表
	m_list.SetItemText(nItem,1,device[i].name);
	m_list.SetItemText(nItem,2,device[i].ip);

	sprintf(str,"%d",device[i].port);
	m_list.SetItemText(nItem,3,str);

	m_list.SetItemText(nItem,4,device[i].user);
	m_list.SetItemText(nItem,5,device[i].psw);

}

