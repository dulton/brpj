// DLGHdd.cpp : implementation file
//

#include "stdafx.h"
#include "zogvm.h"
#include "DLGHdd.h"
#include "video.h"

// http://hi.baidu.com/chenxiong01/item/0302e8a3b5651ade5bf1912b
// http://univasity.iteye.com/category/127688
// http://univasity.iteye.com/blog/860847
// http://univasity.iteye.com/blog/805235
// http://univasity.iteye.com/blog/805234
// https://msdn.microsoft.com/en-us/library/aa364993(VS.85).aspx
// http://download.microsoft.com/download/0/6/7/0678184e-905e-4783-9511-d4dca1f492b4/changejournal2.exe
// http://blog.csdn.net/xexiyong/article/details/17200827
// https://www.microsoft.com/msj/1099/journal2/journal2.aspx  (本文遍历文件夹是超这个的)

#include <winioctl.h>


#include "ntfs.h"

#if 0

public enum UsnJournalReturnCode
{
	INVALID_HANDLE_VALUE = -1,
	USN_JOURNAL_SUCCESS = 0,
	ERROR_INVALID_FUNCTION = 1,
	ERROR_FILE_NOT_FOUND = 2,
	ERROR_PATH_NOT_FOUND = 3,
	ERROR_TOO_MANY_OPEN_FILES = 4,
	ERROR_ACCESS_DENIED = 5,
	ERROR_INVALID_HANDLE = 6,
	ERROR_INVALID_DATA = 13,
	ERROR_HANDLE_EOF = 38,
	ERROR_NOT_SUPPORTED = 50,
	ERROR_INVALID_PARAMETER = 87,
	ERROR_JOURNAL_DELETE_IN_PROGRESS = 1178,
	USN_JOURNAL_NOT_ACTIVE = 1179,
	ERROR_JOURNAL_ENTRY_DELETED = 1181,
	ERROR_INVALID_USER_BUFFER = 1784,
	USN_JOURNAL_INVALID = 17001,
	VOLUME_NOT_NTFS = 17003,
	INVALID_FILE_REFERENCE_NUMBER = 17004,
	USN_JOURNAL_ERROR = 17005
}

public enum UsnReasonCode
{
	USN_REASON_DATA_OVERWRITE = 0x00000001,
	USN_REASON_DATA_EXTEND = 0x00000002,
	USN_REASON_DATA_TRUNCATION = 0x00000004,
	USN_REASON_NAMED_DATA_OVERWRITE = 0x00000010,
	USN_REASON_NAMED_DATA_EXTEND = 0x00000020,
	USN_REASON_NAMED_DATA_TRUNCATION = 0x00000040,
	USN_REASON_FILE_CREATE = 0x00000100,
	USN_REASON_FILE_DELETE = 0x00000200,
	USN_REASON_EA_CHANGE = 0x00000400,
	USN_REASON_SECURITY_CHANGE = 0x00000800,
	USN_REASON_RENAME_OLD_NAME = 0x00001000,
	USN_REASON_RENAME_NEW_NAME = 0x00002000,
	USN_REASON_INDEXABLE_CHANGE = 0x00004000,
	USN_REASON_BASIC_INFO_CHANGE = 0x00008000,
	USN_REASON_HARD_LINK_CHANGE = 0x00010000,
	USN_REASON_COMPRESSION_CHANGE = 0x00020000,
	USN_REASON_ENCRYPTION_CHANGE = 0x00040000,
	USN_REASON_OBJECT_ID_CHANGE = 0x00080000,
	USN_REASON_REPARSE_POINT_CHANGE = 0x00100000,
	USN_REASON_STREAM_CHANGE = 0x00200000,
	USN_REASON_CLOSE = -1
}

#endif
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

//https://msdn.microsoft.com/en-us/library/aa365736%28v=VS.85%29.aspx
//#include <winioctl.h>
bool IsNtfs(char *str)
{
	if (0==strcmp(str, "NTFS" ))
		return true;
	else
		return false;
}

bool GetVolumesHandle(char c,HANDLE *hVol)
{
	char path[32];
	// 传入的文件名必须为\\.\C:的形式  
	sprintf(path,"\\\\.\\%c:",c);
	
	(*hVol)=NULL;
	// 调用该函数需要管理员权限  
	(*hVol) = CreateFileA(path, 
		GENERIC_READ | GENERIC_WRITE, // 可以为0  
		//0,
		FILE_SHARE_READ | FILE_SHARE_WRITE, // 必须包含有FILE_SHARE_WRITE  
		NULL, // 这里不需要  
		OPEN_EXISTING, // 必须包含OPEN_EXISTING, CREATE_ALWAYS可能会导致错误  
		FILE_ATTRIBUTE_READONLY, // FILE_ATTRIBUTE_NORMAL可能会导致错误  
		//FILE_FLAG_BACKUP_SEMANTICS,
		NULL); // 这里不需要  

	if(INVALID_HANDLE_VALUE!=(*hVol)) 
		return true;
	else
		return false;
}

bool  OpenUSNJournal(HANDLE hVol) 
{
	DWORD br;  

	CREATE_USN_JOURNAL_DATA cujd;  
	cujd.MaximumSize = 0; // 0表示使用默认值  
	cujd.AllocationDelta = 0; // 0表示使用默认值  
	BOOL status = DeviceIoControl(hVol,  
		FSCTL_CREATE_USN_JOURNAL,  
		&cujd,  
		sizeof(CREATE_USN_JOURNAL_DATA),  
		NULL,  
		0,  
		&br,  
		NULL);  

	if(0!=status)
		return true;
	else
		return false;
}

bool QueryUSNJournal(HANDLE hVol,USN_JOURNAL_DATA *UsnInfo)
{
	DWORD br;  
	BOOL status = DeviceIoControl(hVol,  
		FSCTL_QUERY_USN_JOURNAL,  
		NULL,  
		0,  
		UsnInfo,  
		sizeof(USN_JOURNAL_DATA),  
		&br,  
		NULL);  

	if(0!=status)
		return true;
	else
		return false;
}


bool QueryUSNJournalVolume(HANDLE hVol, NTFS_VOLUME_DATA_BUFFER *ntfsVolData)
{
	DWORD br;             
	BOOL status =DeviceIoControl(hVol,
		FSCTL_GET_NTFS_VOLUME_DATA, 
		NULL, 0, 
		ntfsVolData, sizeof(NTFS_VOLUME_DATA_BUFFER), 
		&br, NULL);

	if(0!=status)
		return true;
	else
		return false;

	//g_BytesPerCluster[iDri]=ntfsVolData.BytesPerCluster;
//	g_FileRecSize[iDri]=sizeof(NTFS_FILE_RECORD_OUTPUT_BUFFER)-1+ntfsVolData.BytesPerFileRecordSegment;
//	g_pOutBuffer[iDri]=g_MemoryMgr.GetMemory(g_FileRecSize[iDri]);

}

unsigned long long filetreetotal=0;
unsigned long long filetreemax=0;
struct FileTree
{
	DWORDLONG FileReferenceNumber;  
	DWORDLONG ParentFileReferenceNumber;
	DWORD FileAttributes;
	char name[260];
	bool flag;
};
struct FileTree *filetree;


unsigned long long Dirtreetotal=0;
unsigned long long Dirtreemax=0;
struct FileTree *Dirtree;

//数据插入
void FileTreeAdd( DWORDLONG FileReferenceNumber,
				 DWORDLONG ParentFileReferenceNumber,
				 DWORD FileAttributes,
				 char *name)
{
	if(FileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{	
		if(0==Dirtreetotal)
		{
			Dirtreemax=2000;
			Dirtree=(struct FileTree *)calloc(Dirtreemax,sizeof(struct FileTree));
		}
		if(Dirtreetotal>=Dirtreemax)
		{
			Dirtreemax+=500;
			Dirtree=(struct FileTree *)realloc(Dirtree,Dirtreemax*sizeof(struct FileTree));
		}
		Dirtree[Dirtreetotal].ParentFileReferenceNumber=ParentFileReferenceNumber;
		Dirtree[Dirtreetotal].FileReferenceNumber=FileReferenceNumber;
		strcpy(Dirtree[Dirtreetotal].name,name);
		Dirtree[Dirtreetotal].flag=false;
			Dirtree[Dirtreetotal].FileAttributes=FileAttributes;
		Dirtreetotal++;

	}
	else
	{

		if(0==filetreetotal)
		{
			filetreemax=2000;
			filetree=(struct FileTree *)calloc(filetreemax,sizeof(struct FileTree));
		}
		if(filetreetotal>=filetreemax)
		{
			filetreemax+=500;
			filetree=(struct FileTree *)realloc(filetree,filetreemax*sizeof(struct FileTree));
		}
		filetree[filetreetotal].ParentFileReferenceNumber=ParentFileReferenceNumber;
		filetree[filetreetotal].FileReferenceNumber=FileReferenceNumber;
		strcpy(filetree[filetreetotal].name,name);
		filetree[filetreetotal].flag=false;
		
		filetreetotal++;
	}
}

void BuildDirTree(DWORDLONG ParentFileReferenceNumber,
		   char *name)
{
	unsigned long long i;
	char tempstr[260];
	for( i=0;i<Dirtreetotal;i++)
	{
		if(false==Dirtree[i].flag)
		{
			if(Dirtree[i].ParentFileReferenceNumber==ParentFileReferenceNumber)
			{
				sprintf(tempstr,"%s\\%s",name,Dirtree[i].name);
				strcpy(Dirtree[i].name,tempstr);
				Dirtree[i].flag=true;
				BuildDirTree(Dirtree[i].FileReferenceNumber,Dirtree[i].name);
			}
		}
	}
}

void BuildFileTree(DWORDLONG RootFileReferenceNumber,
				   char *Rootname)
{
	unsigned long long i,j;
	char tempstr[260];
	for( i=0;i<filetreetotal;i++)
	{
		if(filetree[i].ParentFileReferenceNumber==RootFileReferenceNumber)
		{
			sprintf(tempstr,"%s\\%s",Rootname,filetree[i].name);
			strcpy(filetree[i].name,tempstr);
		}
		else
		{
			for( j=0;j<Dirtreetotal;j++)
			{
				if(filetree[i].ParentFileReferenceNumber==Dirtree[j].FileReferenceNumber)
				{
					sprintf(tempstr,"%s\\%s",Dirtree[j].name,filetree[i].name);
					strcpy(filetree[i].name,tempstr);
					break;
				}
			}
		}
	}
}


void GetUSNJournal(HANDLE hVol,USN_JOURNAL_DATA UsnInfo,NTFS_VOLUME_DATA_BUFFER ntfsVolData)
{
	MFT_ENUM_DATA med;
	med.StartFileReferenceNumber = 0;
	med.LowUsn = 0;
	med.HighUsn = UsnInfo.NextUsn;

	// Process MFT in 64k chunks
	BYTE pData[sizeof(DWORDLONG) + 0x10000];
	DWORDLONG fnLast = 0;
	DWORD cb;
	char fileName[MAX_PATH] = {0};  
	while (DeviceIoControl(hVol, FSCTL_ENUM_USN_DATA, &med, sizeof(med),
		pData, sizeof(pData), &cb, NULL) != FALSE)
	{

			PUSN_RECORD pRecord = (PUSN_RECORD) &pData[sizeof(USN)];
			while ((PBYTE) pRecord < (pData + cb))
			{
				/*
				if (0 != (pRecord->FileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					CString sz((LPCWSTR)
						((PBYTE) pRecord + pRecord->FileNameOffset),
						pRecord->FileNameLength / sizeof(WCHAR));
					//Add(pRecord->FileReferenceNumber, sz,
					//	pRecord->ParentFileReferenceNumber);
				}
				*/
				memset(fileName,0,MAX_PATH);
				WideCharToMultiByte(CP_OEMCP,NULL,pRecord->FileName,pRecord->FileNameLength/2,
									fileName,pRecord->FileNameLength,NULL,FALSE);  

				FileTreeAdd(pRecord-> FileReferenceNumber,
					pRecord->ParentFileReferenceNumber,
					pRecord->FileAttributes,
					fileName);

				pRecord = (PUSN_RECORD) ((PBYTE) pRecord + pRecord->RecordLength);
			}
			med.StartFileReferenceNumber = * (DWORDLONG *) pData;
	}
}
void BuildFullPath(char *Rootname)
{

	BuildDirTree(1407374883553285,Rootname);
	BuildFileTree(1407374883553285,Rootname);
	FILE *fp=fopen("aaa.txt","wb");

	unsigned long long i;

	for( i=0;i<Dirtreetotal;i++)
	{
		fprintf(fp,"%I64u,%I64u,%d,%s\n",
			Dirtree[i].ParentFileReferenceNumber,Dirtree[i].FileReferenceNumber,
			Dirtree[i].FileAttributes,Dirtree[i].name);
	}
	fclose(fp);

	fp=fopen("bbb.txt","wb");

	for( i=0;i<filetreetotal ;i++)
	{
		fprintf(fp,"%I64u,%I64u,%d,%s\n",
			filetree[i].ParentFileReferenceNumber,filetree[i].FileReferenceNumber,
			filetree[i].FileAttributes,filetree[i].name);
	}
	fclose(fp);
}

bool ColseUSNJournal(HANDLE hVol,USN_JOURNAL_DATA UsnInfo)
{
	DELETE_USN_JOURNAL_DATA dujd;  
	DWORD br;  
	dujd.UsnJournalID = UsnInfo.UsnJournalID;  
	dujd.DeleteFlags = USN_DELETE_FLAG_DELETE;  

	int status = DeviceIoControl(hVol,  
		FSCTL_DELETE_USN_JOURNAL,  
		&dujd,  
		sizeof (DELETE_USN_JOURNAL_DATA),  
		NULL,  
		0,  
		&br,  
		NULL);  

	if(0!=status)
		return true;
	else
		return false;
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

			//////////////////////////////////////////
		
			if(IsNtfs(FileName))
			{
				HANDLE hVol;
				if(GetVolumesHandle(c,&hVol))
				{
					if(OpenUSNJournal(hVol))
					{

						USN_JOURNAL_DATA UsnInfo;
						if(QueryUSNJournal(hVol,&UsnInfo))
						{
							NTFS_VOLUME_DATA_BUFFER ntfsVolData;
							QueryUSNJournalVolume(hVol, &ntfsVolData);

							GetUSNJournal(hVol,UsnInfo,ntfsVolData);
							BuildFullPath("c:");
						}

						ColseUSNJournal(hVol,UsnInfo);
					}
				}
			}
		
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

	m_list.InsertColumn(0, ConvertString("磁盘序列号") , LVCFMT_LEFT, 90);
	m_list.InsertColumn(1, ConvertString("分区") , LVCFMT_LEFT, 40);
	m_list.InsertColumn(2, ConvertString("标签") , LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, ConvertString("总空间") , LVCFMT_LEFT, 75);
	m_list.InsertColumn(4, ConvertString("剩余空间") , LVCFMT_LEFT, 70);
	m_list.InsertColumn(5, ConvertString("是否接驳") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(6, ConvertString("nid") , LVCFMT_LEFT, 0);

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
			m_list.SetItemText(i,5,ConvertString("接入<--"));
		else
			m_list.SetItemText(i,5,ConvertString("脱机-->"));
		
		sprintf(str,"%d",beglist->hdd_nid);
		m_list.SetItemText(i,6,str);
		
		m_list.SetCheck(i,beglist->enable);
		i++;

		tb+=beglist->TotalBytes;
	}
	sprintf(str,ConvertString("总容量：%.3f T"),(double)(tb/1024/1024/1024)/1024.0);
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

