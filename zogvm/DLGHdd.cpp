// DLGHdd.cpp : implementation file
//

#include "stdafx.h"
#include "zogvm.h"
#include "DLGHdd.h"

extern CSqliteOperate SQLDB;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGHdd dialog


CDLGHdd::CDLGHdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGHdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGHdd)
	m_check = FALSE;
	//}}AFX_DATA_INIT
}


void CDLGHdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGHdd)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Check(pDX, IDC_CHECK, m_check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGHdd, CDialog)
	//{{AFX_MSG_MAP(CDLGHdd)
	ON_BN_CLICKED(IDC_CHECK, OnCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGHdd message handlers
DWORD CDLGHdd::GetHDDid(void)
{
	CString id; 
	char Name[MAX_PATH]=""; 
	DWORD serno; 
	DWORD length; 
	DWORD FileFlag; 
	char FileName[MAX_PATH]=""; 
	BOOL Ret; 
	Ret = GetVolumeInformation("c:\\\\", Name, MAX_PATH, &serno, &length, &FileFlag, FileName, MAX_PATH); 
	
	if (Ret) 
		return serno;
	else
		return 0;
}

void CDLGHdd::Add27HDDid(void)
{
	CString id; 
	char Name[MAX_PATH]=""; 
	DWORD serno; 
	DWORD length; 
	DWORD FileFlag; 
	char FileName[MAX_PATH]=""; 
	BOOL Ret; 
	char path[MAX_PATH]="";
	char serstr[MAX_PATH]="";

	unsigned long long i64FreeBytesToCaller;  
	unsigned long long i64TotalBytes;  
	unsigned long long i64FreeBytes;  

	char c;
	for(c='c';c<='z';c++)
	{
		sprintf(path,"%c:\\",c);
		//获取名称
		Ret = GetVolumeInformation(path, Name, MAX_PATH, &serno, &length, &FileFlag, FileName, MAX_PATH); 
		if (Ret) 
		{
			sprintf(serstr,"%X",serno);
			//获取容量
			i64FreeBytesToCaller=0;
			i64TotalBytes=0;

			GetDiskFreeSpaceEx (  
				path,  
				(PULARGE_INTEGER)&i64FreeBytesToCaller,  
				(PULARGE_INTEGER)&i64TotalBytes,  
				(PULARGE_INTEGER)&i64FreeBytes);  

			SQLDB.Hdd_Add(serstr,path,Name,i64TotalBytes,i64FreeBytesToCaller);
		}
	}
}


BOOL CDLGHdd::OnInitDialog()
{
	CDialog::OnInitDialog();
	hddList.clear();

	SQLDB.Begin();
	SQLDB.Hdd_SetNonsert();
	Add27HDDid();
	SQLDB.Commit();

	SQLDB.Hdd_Read(hddList);

	m_list.InsertColumn(0, _T("磁盘序列号") , LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, _T("分区") , LVCFMT_LEFT, 40);
	m_list.InsertColumn(2, _T("标签") , LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, _T("总空间") , LVCFMT_LEFT, 70);
	m_list.InsertColumn(4, _T("剩余空间") , LVCFMT_LEFT, 70);
	m_list.InsertColumn(5, _T("是否接驳") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(6, _T("nid") , LVCFMT_LEFT, 0);

	//带复选框 LVS_EX_CHECKBOXES
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	
	long long tb=0;

	char str[32];
	int i=0;
	list<struct HDD_ST>::iterator beglist;
	
	for(beglist=hddList.begin();beglist!=hddList.end();beglist++)
	{
		m_list.InsertItem(i,beglist->serno);
		
		m_list.SetItemText(i,1,beglist->area);
		m_list.SetItemText(i,2,beglist->mark);
		
		sprintf(str,"%.3fG",(double)(beglist->TotalBytes/1024/1024)/1024.0);
		m_list.SetItemText(i,3,str);
		sprintf(str,"%.3fG",(double)(beglist->FreeBytes/1024/1024)/1024.0);
		m_list.SetItemText(i,4,str);

		if(beglist->insertflag)
			m_list.SetItemText(i,5,"接入<--");
		else
			m_list.SetItemText(i,5,"脱机-->");
		
		sprintf(str,"%d",beglist->hdd_nid);
		m_list.SetItemText(i,6,str);
		
		m_list.SetCheck(i,beglist->enable);
		i++;

		tb+=beglist->TotalBytes;
	}
	sprintf(str,"总容量：%.3f T",(double)(tb/1024/1024/1024)/1024.0);
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGHdd::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	int i;
	
	char str[32];
	int check;
	
	SQLDB.Begin();
	for( i=0;i<m_list.GetItemCount();i++)
	{
		m_list.GetItemText(i,6,str,32);
		check=m_list.GetCheck(i);
		SQLDB.Hdd_ModifyEnable(str,check);
	}
		SQLDB.Commit();

		hddList.clear();
	CDialog::OnOK();
}

void CDLGHdd::OnCancel() 
{
	// TODO: Add extra cleanup here
		hddList.clear();
	CDialog::OnCancel();
}

void CDLGHdd::OnCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	for(int i=0;i<m_list.GetItemCount();i++)
		m_list.SetCheck(i,m_check);
}

