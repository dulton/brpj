// zogvmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zogvm.h"
#include "zogvmDlg.h"

#include "DLGFileType.h"

#include "SqliteOperate.h"
extern CSqliteOperate SQLDB;

#include "DLGHdd.h"
#include "DLGhdddelete.h"
#include "video.h"
#include "DLGYYETS.h"

#include "rhashinclude\rhash.h"

CZogvmDlg *pZogvmDlg;

list<struct FILE_VIEW_ST> outputList;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZogvmDlg dialog

CZogvmDlg::CZogvmDlg(CWnd* pParent /*=NULL*/)
: CDialog(CZogvmDlg::IDD, pParent)
, m_sreach(_T(""))
, m_c_delete_trash(FALSE)
{
	//{{AFX_DATA_INIT(CZogvmDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_page = 1;
	pZogvmDlg=this;
	ModeFlag=MODE_FIND;
}

void CZogvmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZogvmDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SREACH, m_sreach);
	DDV_MaxChars(pDX, m_sreach, 100);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_first_button);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_pre_button);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_next_button);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_last_button);
	DDX_Control(pDX, IDC_BUTTON_JUMP, m_jump_button);
	DDX_Check(pDX, IDC_CHECK_DELETE_TRASH, m_c_delete_trash);
}

BEGIN_MESSAGE_MAP(CZogvmDlg, CDialog)
	//{{AFX_MSG_MAP(CZogvmDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENUITEM_SET_TYPE, OnMenuitemSetType)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnButtonBuild)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_COMMAND(ID_MENUITEM_SET_HDD, OnMenuitemSetHdd)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &CZogvmDlg::OnLvnColumnclickList)
	ON_BN_CLICKED(IDC_BUTTON_DOUBLE, &CZogvmDlg::OnBnClickedButtonDouble)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CZogvmDlg::OnNMCustomdrawList)
	ON_COMMAND(ID_HDD_DELETE, &CZogvmDlg::OnHddDelete)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUTEXECL, &CZogvmDlg::OnBnClickedButtonOutputexecl)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST, &CZogvmDlg::OnLvnItemActivateList)
	ON_BN_CLICKED(IDC_BUTTON_TRASH, &CZogvmDlg::OnBnClickedButtonTrash)
	ON_BN_CLICKED(IDC_BUTTON_TRASH_CLEAN, &CZogvmDlg::OnBnClickedButtonTrashClean)
	ON_COMMAND(ID_YYETS, &CZogvmDlg::OnYyets)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZogvmDlg message handlers
//获取文件后缀,并变小写
CString GetFileExt(CString fileName)
{
	return (fileName.Right(fileName.GetLength()-fileName.ReverseFind('.'))).MakeLower();
}

//获取文件相对路径。去除盘符和名字
CString GetFilePathNoName(CString filepath)
{
	CString path=filepath.Right(filepath.GetLength()-filepath.Find('\\')-1);
	return path.Left(path.ReverseFind('\\')+1);
}
//比较后缀
int checkExt(list<struct FILETYPE_ST> typeList,char *fileext) 
{
	list<struct FILETYPE_ST>::iterator beglist;

	for(beglist=typeList.begin();beglist!=typeList.end();beglist++)
	{
		if(beglist->enable && 0== strcmp(beglist->type,fileext) )
		{
			return beglist->maintype ;
		}
	}
	// -1为无效
	return -1;
}

struct STR_SPLITE_S
{
	char s[256];
};

//分割字符串
void StrSplite(list<struct STR_SPLITE_S> &strList,char *src) 
{
	struct STR_SPLITE_S a={0};
	int i=0;
	strList.clear();

	while(*src)
	{
		if(' '==(*src) || '.'==(*src) || '-'==(*src) || '_'==(*src)|| '+'==(*src))
		{
			//小写
			strlwr(a.s);
			strList.push_back(a);
			memset(&a,0,sizeof(struct STR_SPLITE_S));
			i=0;
		}
		else
		{
			a.s[i]=(*src);
			i++;
		}
		src++;

	}
	strlwr(a.s);
	strList.push_back(a);
}

//比较字符串
void checkStr(struct FILE_ST &data,list<struct ZIDIAN_ST> zidianList,list<struct STR_SPLITE_S> strList) 
{
	list<struct STR_SPLITE_S>::iterator beglistA;
	list<struct ZIDIAN_ST>::iterator beglistB;

	for(beglistA=strList.begin();beglistA!=strList.end();beglistA++)
	{
		if(strlen(beglistA->s) <=16)
		{
			for(beglistB=zidianList.begin();beglistB!=zidianList.end();beglistB++)
			{
				if(0==strcmp(beglistA->s,beglistB->lowzidian))
				{
					if(ZIDIAN_YAZHI == beglistB->mainzidian)
					{
						strcpy(data.zidian_yazhi,beglistB->zidian);
					}
					else if(ZIDIAN_HUAZHI == beglistB->mainzidian)
					{
						strcpy(data.zidian_huazhi,beglistB->zidian);
					}
					else if(ZIDIAN_FENBIANLV == beglistB->mainzidian)
					{
						strcpy(data.zidian_fenbianlv,beglistB->zidian);
					}
					else if(ZIDIAN_3D == beglistB->mainzidian)
					{
						strcpy(data.zidian_3d,beglistB->zidian);
					}
					else if(ZIDIAN_YEAR == beglistB->mainzidian)
					{
						strcpy(data.zidian_year,beglistB->zidian);
					}
					break;
				}
			}
		}
	}

}


//遍历所有文件
void FindAllFile(long long hdd_nid,CString hdd_area,
				 list<struct FILETYPE_ST> typeList,list<struct ZIDIAN_ST> zidianList)
{

	CTime cTime;
	CTime lTime;
	CString fileName;
	CString fileExt;

	struct FILE_ST data;
	list<STR_SPLITE_S> strList;

	CFileFind fileFinder;
	CString filePath = hdd_area + _T("//*.*");

	BOOL bFinished = fileFinder.FindFile(filePath);

	while(bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();

		if(fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
		{
			FindAllFile(hdd_nid,fileFinder.GetFilePath(),typeList,zidianList);
		}
		else  //再判断是否为txt文件
		{
			//获取文件类型
			fileName = fileFinder.GetFileName();
			fileExt=GetFileExt(fileName);

			memset(&data,0,sizeof(struct FILE_ST ));
			//有效后缀
			data.maintype=checkExt(typeList,fileExt.GetBuffer(0));

			if(data.maintype >=0)
			{
				if(SQLDB.File_CheckDoublePos(fileName.GetBuffer(0),
					GetFilePathNoName(fileFinder.GetFilePath()).GetBuffer(0),
					hdd_nid))
					continue;

				fileFinder.GetCreationTime(cTime);
				fileFinder.GetLastWriteTime(lTime);

				strcpy(data.name,fileName.GetBuffer(0)	);
				strcpy(data.path,GetFilePathNoName(fileFinder.GetFilePath()).GetBuffer(0));

				data.hdd_nid=hdd_nid;
				strcpy(data.type,fileExt.GetBuffer(0));
				data.filesize=fileFinder.GetLength();

				data.CreationTime=cTime.GetTime();
				data.LastWriteTime=lTime.GetTime();

			}
			if(MAINTYPE_OTHER == data.maintype )
			{
				strList.clear();
				StrSplite(strList,fileName.GetBuffer(0));
				checkStr(data,zidianList,strList); 
				strList.clear();
			}
			else if( MAINTYPE_SUB == data.maintype)
			{
				NULL;
			}
			else  if(MAINTYPE_VIDEO == data.maintype )
			{
				#if OPEN_FFMEPG
				VideoPlay(fileFinder.GetFilePath().GetBuffer(0),
					&data.filetime,&data.resolutionW,&data.resolutionH);
#endif
				strList.clear();
				StrSplite(strList,fileName.GetBuffer(0));
				checkStr(data,zidianList,strList); 
				strList.clear();
			}
			else  if(MAINTYPE_MUSIC == data.maintype )
			{
				#if OPEN_FFMEPG
				AudioPlay(fileFinder.GetFilePath().GetBuffer(0),&data.filetime) ;
#endif
				NULL;
			}
			if(data.maintype >=0)
			{
				SQLDB.File_Add(data);
			}
		}
	}

	fileFinder.Close();
}
void CheckAllDoubleFile(long long hdd_nid)
{
	long long doublenid;
	CString strid;
	char cstrid[64];

	list<struct FILE_VIEW_LITE_ST> fileviewList;
	list<struct FILE_VIEW_LITE_ST>::iterator beglist;
	list<struct FILE_VIEW_LITE_ST> fileviewDoubleList;
	list<struct FILE_VIEW_LITE_ST>::iterator Doublebeglist;
	fileviewList.clear();
	fileviewDoubleList.clear();

	if(SQLDB.File_ReadHddAllwithVideoAudio(fileviewList,hdd_nid))
	{
		for(beglist=fileviewList.begin();beglist!=fileviewList.end();beglist++)
		{
			fileviewDoubleList.clear();
			SQLDB.File_CheckDouble(beglist->file_maintype,beglist->filesize,
				beglist->file_name,beglist->file_md5,
				fileviewDoubleList);

			//查看重复 必须多于1条
			if(fileviewDoubleList.size()>1)
			{
				//发现重复的
				doublenid=0;
				//看看之前有没ID。 复用他
				for(Doublebeglist=fileviewDoubleList.begin();Doublebeglist!=fileviewDoubleList.end();Doublebeglist++)
				{
					if(Doublebeglist->double_nid>0)
					{
						doublenid=Doublebeglist->double_nid;
						break;
					}
				}
				//新增ID
				if(doublenid <=0 )
				{
					SQLDB.Double_Add(beglist->file_nid);
					//获取一个DOUBLEID 
					SQLDB.Double_ReadOne(&doublenid,beglist->file_nid);
				}
				//写到各个文件里
				if(doublenid >0)	
				{
					strid="";
					for(Doublebeglist=fileviewDoubleList.begin();Doublebeglist!=fileviewDoubleList.end();Doublebeglist++)
					{
						if(Doublebeglist!=fileviewDoubleList.begin())
							sprintf(cstrid,",%I64u",Doublebeglist->file_nid);
						else
							sprintf(cstrid,"%I64u",Doublebeglist->file_nid);
						strid+=cstrid;
					}

					SQLDB.File_SetDouble(doublenid,strid.GetBuffer(0));
				}
			}
		
		}
	}
	fileviewList.clear();
	fileviewDoubleList.clear();

}
void CheckAllIsFile(long long hdd_nid)
{
	FILE *fp=NULL;
	char str[512];

	list<struct FILE_VIEW_LITE_ST> fileviewList;
	list<struct FILE_VIEW_LITE_ST>::iterator beglist;
	
	fileviewList.clear();
	//查看文件是否存在
	if(SQLDB.File_ReadHddAllandOld(fileviewList,hdd_nid))
	{
			
		for(beglist=fileviewList.begin();beglist!=fileviewList.end();beglist++)
		{
			sprintf(str,"%s%s%s",beglist->hdd_area,beglist->file_path,beglist->file_name);
			fp=fopen(str,"rb");
			if(fp)
			{
				//文件存在
				fclose(fp);
				fp=NULL;
			}
			else
			{
				//文件不存在
				SQLDB.File_Delete(beglist->file_nid);
			}
		}
	}
	//重置全部文件为0
	SQLDB.File_SetNoFlagOne(hdd_nid);
	fileviewList.clear();

}

//生成ED2K连接。
void GetEd2kUrl(char* path,char *name,char *dst)
{
	unsigned char digest[64]="";
	char outputed2k[256]="";
	char outputaich[256]="";

	strcpy(dst,"");
	long long sizemax=1024*1024;
	sizemax*=1024*4;

	long long size=filesize(path);
	if(size >= sizemax)
	{
		//大于4G 退出
		return ;
	}
	rhash_library_init(); 

	rhash_file(RHASH_ED2K,path, digest);
	rhash_print_bytes(outputed2k, digest, rhash_get_digest_size(RHASH_ED2K),(RHPR_HEX));


	rhash_file(RHASH_AICH,path, digest);
	rhash_print_bytes(outputaich, digest, rhash_get_digest_size(RHASH_AICH),(RHPR_BASE32 ));

	sprintf(dst,"ed2k://|file|%s|%I64u|%s|h=%s",name,size,outputed2k,outputaich);

//	rhash_file(RHASH_TTH,filepath, digest);
//	rhash_print_bytes(output, digest, rhash_get_digest_size(RHASH_TTH),(RHPR_BASE32 ));

//	rhash_file(RHASH_SHA1,filepath, digest);
//	rhash_print_bytes(output, digest, rhash_get_digest_size(RHASH_SHA1 ),RHPR_HEX);

}
//////////////////////////////////////////////////////////////////////////////
DWORD WINAPI PlayThreadPROC(LPVOID lpParameter)
{
	struct HDD_ST  hddst;
	memcpy(&hddst,(struct HDD_ST *)lpParameter,sizeof(struct HDD_ST));

	char tempstr[256];

	list<struct FILETYPE_ST> typeList;
	typeList.clear();
	SQLDB.Type_Read(typeList); 

	list<struct ZIDIAN_ST> zidianList;
	zidianList.clear();
	SQLDB.ZiDian_Read(zidianList); 

	sprintf(tempstr,"%s %s ：建目录中..",hddst.serno,hddst.mark);
	pZogvmDlg->GetDlgItem(IDC_STATIC_MSG)->SetWindowText(tempstr);

	//建目录
	FindAllFile(hddst.hdd_nid,hddst.area,typeList,zidianList);
	
	sprintf(tempstr,"%s %s ：判断文件是否存在..",hddst.serno,hddst.mark);
	pZogvmDlg->GetDlgItem(IDC_STATIC_MSG)->SetWindowText(tempstr);

	//查看文件是否存在
	CheckAllIsFile(hddst.hdd_nid);

	//缺少 关联 IDX代码


	//设置自己查找完毕
	list<struct HDD_ST>::iterator beglist;

	for(beglist=pZogvmDlg->hddList.begin();beglist!=pZogvmDlg->hddList.end();beglist++)
	{
		if(beglist->hdd_nid == hddst.hdd_nid)
		{
			beglist->dealstep=DEALSTEP_FIND_END;
			break;
		}
	}
	//判断是否全部完结
	bool flag=true;
	//等待全部完结后 查重
	while(flag)
	{
		flag=false;
		for(beglist=pZogvmDlg->hddList.begin();beglist!=pZogvmDlg->hddList.end();beglist++)
		{
			if(DEALSTEP_START  == beglist->dealstep)
			{
				flag=true;
				Sleep(1000);
				break;
			}
		}
	}
	
	sprintf(tempstr,"%s %s ：判断重复文件..",hddst.serno,hddst.mark);
	pZogvmDlg->GetDlgItem(IDC_STATIC_MSG)->SetWindowText(tempstr);

	//查看重复文件
	CheckAllDoubleFile(hddst.hdd_nid);

	sprintf(tempstr,"%s %s ：扫描完毕",hddst.serno,hddst.mark);
	pZogvmDlg->GetDlgItem(IDC_STATIC_MSG)->SetWindowText(tempstr);
	
	//设置自己完毕
	for(beglist=pZogvmDlg->hddList.begin();beglist!=pZogvmDlg->hddList.end();beglist++)
	{
		if(beglist->hdd_nid == hddst.hdd_nid)
		{
			beglist->dealstep=DEALSTEP_ALL_END;
			break;
		}
	}
		
	//判断是否全部完结
	flag=true;
	for(beglist=pZogvmDlg->hddList.begin();beglist!=pZogvmDlg->hddList.end();beglist++)
	{
		if(DEALSTEP_ALL_END  != beglist->dealstep)
		{
			flag=false;
			break;
		}
	}
	
	if(flag)
	{
		sprintf(tempstr,"全部扫描完毕~么么哒",hddst.serno,hddst.mark);
		pZogvmDlg->GetDlgItem(IDC_STATIC_MSG)->SetWindowText(tempstr);
		pZogvmDlg->GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);
	}

	return 0;
}

BOOL CZogvmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//设置列表图标
	m_SmallIcon.Create(16,16, 1, 10, 4);
	m_SmallIcon.Add(AfxGetApp()->LoadIcon(IDI_GREEN));
	m_SmallIcon.Add(AfxGetApp()->LoadIcon(IDI_RED));
	m_SmallIcon.Add(AfxGetApp()->LoadIcon(IDI_VIDEO));
	m_SmallIcon.Add(AfxGetApp()->LoadIcon(IDI_AUDIO));
	m_SmallIcon.Add(AfxGetApp()->LoadIcon(IDI_SUB));
	m_SmallIcon.Add(AfxGetApp()->LoadIcon(IDI_ISO));

	m_list.SetImageList(&m_SmallIcon,LVSIL_SMALL);  

  //m_NullIcon.Create(1,1, 1, 10, 4);
 //m_NullIcon.Add(AfxGetApp()->LoadIcon(IDI_NULL));
 //m_list.SetImageList(&m_NullIcon,LVSIL_SMALL);  

	m_list.InsertColumn(0, _T("硬盘序列号") , LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, _T("标签") , LVCFMT_LEFT, 80);
	m_list.InsertColumn(2, _T("分区") , LVCFMT_LEFT, 40);
	m_list.InsertColumn(3, _T("序号") , LVCFMT_LEFT, 50);
	m_list.InsertColumn(4, _T("路径") , LVCFMT_LEFT, 200);
	m_list.InsertColumn(5, _T("文件名") , LVCFMT_LEFT, 300);
	m_list.InsertColumn(6, _T("类型") , LVCFMT_LEFT, 40);
	m_list.InsertColumn(7, _T("后缀") , LVCFMT_LEFT, 50);
	m_list.InsertColumn(8, _T("创建时间") , LVCFMT_LEFT, 130);
	m_list.InsertColumn(9, _T("修改时间") , LVCFMT_LEFT,130);
	m_list.InsertColumn(10, _T("文件大小") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(11, _T("分辨率") , LVCFMT_LEFT, 0);
	m_list.InsertColumn(12, _T("播放长度") , LVCFMT_LEFT, 0);
	m_list.InsertColumn(13, _T("硬盘接驳") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(14, _T("3D") , LVCFMT_LEFT,30);
	m_list.InsertColumn(15, _T("画质") , LVCFMT_LEFT, 50);
	m_list.InsertColumn(16, _T("分辨率") , LVCFMT_LEFT, 50);
	m_list.InsertColumn(17, _T("压制组") , LVCFMT_LEFT, 80);
	m_list.InsertColumn(18, _T("年代") , LVCFMT_LEFT, 40);
	m_list.InsertColumn(19, _T("效验值") , LVCFMT_LEFT, 0);
	m_list.InsertColumn(20, _T("重复ID") , LVCFMT_LEFT, 50);
	//带复选框 LVS_EX_CHECKBOXES 带图标 LVS_EX_SUBITEMIMAGES
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES);

//	char output[1024]="";
//	GetEd2kUrl("C:\\paramsettemp.jpg","paramsettemp.jpg",output);
	// TODO: Add extra initialization here
#if OPEN_FFMEPG
	VideoInit();
#endif

	//初始即最大化
	ShowWindow(SW_MAXIMIZE);   

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CZogvmDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CZogvmDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CZogvmDlg::OnOK() 
{
	// TODO: Add extra validation here

	//	CDialog::OnOK();
}

void CZogvmDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CZogvmDlg::OnMenuitemSetType() 
{
	// TODO: Add your command handler code here
	CDLGFileType DlgFileType;
	DlgFileType.DoModal();

}

void CZogvmDlg::OnMenuitemSetHdd() 
{
	// TODO: Add your command handler code here
	CDLGHdd DlgHdd;
	DlgHdd.DoModal();
}


void CZogvmDlg::OnButtonBuild() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(FALSE);


	SQLDB.Begin();
	SQLDB.Hdd_SetNonsert();
	CDLGHdd::Add27HDDid();
	SQLDB.Commit();

	hddList.clear();
	//几个硬盘开几个线程
	SQLDB.Hdd_Read(hddList);
	
	bool flag=false;
	list<struct HDD_ST>::iterator beglist;

	for(beglist=hddList.begin();beglist!=hddList.end();beglist++)
	{
		beglist->dealstep=DEALSTEP_ALL_END;
		if(beglist->insertflag && beglist->enable)
		{
			flag=true;
			beglist->dealstep=DEALSTEP_START;
			CreateThread(NULL,0,PlayThreadPROC,(void*)&(*beglist),0,NULL);
		}
	}

	if(false == flag)
		GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);

}

char*  MainType2Str(int type)
{
	switch(type)
	{
	case MAINTYPE_OTHER:return "其他";
	case MAINTYPE_SUB:return "字幕";
	case MAINTYPE_VIDEO:return "视频";
	case MAINTYPE_MUSIC:return "音频";
	default:return "";
	}
}
void CZogvmDlg::DisplaySize(long long size,char* str)
{
	long long temp=size/1024;

	if(temp==0)
	{
		sprintf(str,"%I64u",size);
		return ;
	}
	temp/=1024;
	if(temp==0)
	{
		sprintf(str,"%I64u K",size/1024);
		return ;
	}
	temp/=1024;
	if(temp==0)
	{
		sprintf(str,"%I64u M",size/(1024*1024));
		return ;
	}

	sprintf(str,"%.02lf G",(double)(size/1024/1024)/1024.0);
}

void CZogvmDlg::DisplayerList()
{
	m_list.DeleteAllItems();
	WinfileviewList.clear();

	if(0==ListTotal)
	{
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
		InvalidateRect(printf_Rect, TRUE);
		return ;
	}

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	switch(ModeFlag)
	{
	case MODE_DOUBLE:
		SQLDB.File_ReadDouble(WinfileviewList,true,si-1,ei-(si-1));
		break;
	case MODE_FIND:
		if(m_sreach.IsEmpty())
			SQLDB.File_ReadPath(WinfileviewList,true,si-1,ei-(si-1));
		else
			SQLDB.File_ReadPathForSearch(m_sreach.GetBuffer(0),WinfileviewList,true,si-1,ei-(si-1));
		break;
	case MODE_TRASH:
		SQLDB.File_ReadTrash(WinfileviewList,true,si-1,ei-(si-1));
		break;
	default:return ;

	}

	if(WinfileviewList.size()<=0)
		return ;

	list<struct FILE_VIEW_ST>::iterator beglist;
	char str[128];
	int i=0;
	CTime temptime;

	for(beglist=WinfileviewList.begin();beglist!=WinfileviewList.end();beglist++)
	{
		//带复选框 LVS_EX_CHECKBOXES
		if(MAINTYPE_OTHER == beglist->file_data.maintype )
		{
			m_list.InsertItem( i,beglist->hdd_data.serno,5);
		}
		else if( MAINTYPE_SUB == beglist->file_data.maintype)
		{
			m_list.InsertItem( i,beglist->hdd_data.serno,4);
		}
		else  if(MAINTYPE_VIDEO == beglist->file_data.maintype )
		{
			m_list.InsertItem( i,beglist->hdd_data.serno,2);
		}
		else  if(MAINTYPE_MUSIC == beglist->file_data.maintype )
		{
			m_list.InsertItem( i,beglist->hdd_data.serno,3);
		}
		m_list.SetItemText(i,1,beglist->hdd_data.mark);
		m_list.SetItemText(i,2,beglist->hdd_data.area);

		sprintf(str,"%I64u",beglist->file_data.file_nid);
		m_list.SetItemText(i,3,str);
		m_list.SetItemText(i,4,beglist->file_data.path);
		m_list.SetItemText(i,5,beglist->file_data.name);

		m_list.SetItemText(i,6,	MainType2Str(beglist->file_data.maintype));

		m_list.SetItemText(i,7,beglist->file_data.type);

		temptime=beglist->file_data.CreationTime;

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d", 
			temptime.GetYear(),temptime.GetMonth(),temptime.GetDay(),
			temptime.GetHour(),temptime.GetMinute(),temptime.GetSecond());
		m_list.SetItemText(i,8,str);

		temptime=beglist->file_data.LastWriteTime;

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d", 
			temptime.GetYear(),temptime.GetMonth(),temptime.GetDay(),
			temptime.GetHour(),temptime.GetMinute(),temptime.GetSecond());
		m_list.SetItemText(i,9,str);

		//sprintf(str,"%I64u",);
		DisplaySize(beglist->file_data.filesize,str);
		m_list.SetItemText(i,10,str);

		if(beglist->file_data.resolutionW && beglist->file_data.resolutionH)
		{
			sprintf(str,"%d*%d",beglist->file_data.resolutionW,beglist->file_data.resolutionH);
			m_list.SetItemText(i,11,str);
		}
		else
			m_list.SetItemText(i,11,"");

		if(beglist->file_data.filetime)
		{
			sprintf(str,"%d:%02d:%02d", 
				beglist->file_data.filetime / 3600,
				(beglist->file_data.filetime  % 3600) / 60,
				beglist->file_data.filetime % 60);
			m_list.SetItemText(i,12,str);
		}
		else
			m_list.SetItemText(i,12,"");

		if(beglist->hdd_data.insertflag)
		{
			m_list.SetItemText(i,13,"接入");
			m_list.SetItem(i, 13, LVIF_IMAGE, NULL, 0, 0, 0, 0);
		}
		else
		{
			m_list.SetItemText(i,13,"脱机");
			m_list.SetItem(i, 13, LVIF_IMAGE, NULL, 1, 0, 0, 0);
		}

		/*
		//名称
		LV_ITEM lvitem;
		memset ((char *) &lvitem, 0, sizeof (LV_ITEM));
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE  | LVIF_STATE;
		lvitem.iItem = i;
		lvitem.iSubItem = 12;
		lvitem.stateMask = 0;
		lvitem.iImage =1;          //显示不同的图标时，可以把一个int 变量赋给这个属性值
		lvitem.pszText = "a";
		m_list.SetItem (&lvitem);

		m_list.SetItem(i, 12, LVIF_IMAGE, NULL, 0, 0, 0, 0);
*/

		m_list.SetItemText(i,14,beglist->file_data.zidian_3d);
		m_list.SetItemText(i,15,beglist->file_data.zidian_huazhi);
		m_list.SetItemText(i,16,beglist->file_data.zidian_fenbianlv);
		m_list.SetItemText(i,17,beglist->file_data.zidian_yazhi);
		m_list.SetItemText(i,18,beglist->file_data.zidian_year);

		m_list.SetItemText(i,19,beglist->file_data.md5);
		if(MODE_DOUBLE == ModeFlag)
		{
			sprintf(str,"%I64u",beglist->file_data.double_nid);
			m_list.SetItemText(i,20,str);
		}
		else
			m_list.SetItemText(i,20,"0");

		i++;
	}

	if(ListTotal%PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/PAGE_MAX_NUM+1,ListTotal/PAGE_MAX_NUM+1);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/PAGE_MAX_NUM+1,ListTotal/PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	InvalidateRect(printf_Rect, TRUE);

	m_c_delete_trash=FALSE;
	UpdateData(FALSE);
}



void CZogvmDlg::OnButtonFind() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ListTotal=0;

	if(m_sreach.IsEmpty())
		ListTotal=SQLDB.File_Number();
	else
		ListTotal=SQLDB.File_NumberForSearch(m_sreach.GetBuffer(0));

	ListNow=0;

	ModeFlag=MODE_FIND;
	DisplayerList();
}

void CZogvmDlg::OnBnClickedButtonDouble()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SQLDB.File_ResetDouble2Zero();

	ListTotal=0;
	ListTotal=SQLDB.File_DoubleNumber();
	ListNow=0;

	ModeFlag=MODE_DOUBLE;
	DisplayerList();
}

void CZogvmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect m_clientRect;
	GetClientRect(&m_clientRect);

	int height=70;

	CRect list_Rect;
	list_Rect.top = 	m_clientRect.top+height;
	list_Rect.bottom = m_clientRect.bottom-30;
	list_Rect.left = m_clientRect.left ;
	list_Rect.right = m_clientRect.right;
	//必须 样式=重叠，边框=调整大小
	m_list.MoveWindow(list_Rect);

	int buttonh=25;
	int buttonbmpw=24,buttonbmph=18,buttongap=10;

	CRect b_Rect;

	b_Rect.top = list_Rect.bottom+5 ;
	b_Rect.bottom =	b_Rect.top+buttonbmph;

	b_Rect.left = list_Rect.left+10;
	b_Rect.right = b_Rect.left+200;
	GetDlgItem(IDC_STATIC_INFO)->MoveWindow(b_Rect);

	printf_Rect=b_Rect;

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_first_button.MoveWindow(b_Rect);

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_pre_button.MoveWindow(b_Rect);

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_next_button.MoveWindow(b_Rect);

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_last_button.MoveWindow(b_Rect);
	//////////////

	b_Rect.top = list_Rect.bottom+8 ;
	b_Rect.left = b_Rect.right+buttongap*3;
	b_Rect.right = b_Rect.left+20;
	GetDlgItem(IDC_STATIC_DI)->MoveWindow(b_Rect);

	b_Rect.top = list_Rect.bottom+5 ;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+60;
	GetDlgItem(IDC_EDIT_PAGE)->MoveWindow(b_Rect);

	b_Rect.top = list_Rect.bottom+2 ;
	b_Rect.bottom =	b_Rect.top+24;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+20;
	m_jump_button.MoveWindow(b_Rect);

	b_Rect.top = list_Rect.bottom+8 ;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+20;
	GetDlgItem(IDC_STATIC_YE)->MoveWindow(b_Rect);


	b_Rect.top = list_Rect.bottom+5 ;
	b_Rect.left = b_Rect.right+20;
	b_Rect.right = b_Rect.left+300;
	GetDlgItem(IDC_STATIC_MSG)->MoveWindow(b_Rect);
	// TODO: Add your message handler code here
}
//列头点击事件
void CZogvmDlg::OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//sort_column = pNMLV->iSubItem;//点击的列

	// TODO: Add your control notification handler code here
	*pResult = 0;
}



//NMTVCUSTOMDRAW
//高亮
void CZogvmDlg::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)pNMHDR;
	*pResult=CDRF_DODEFAULT;

	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			*pResult=CDRF_NOTIFYITEMDRAW;
			return;
		}
	case CDDS_ITEMPREPAINT:
		{
			*pResult=CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		{
			char str[64];
			long long doublenid;
			COLORREF clrNewTextColor,clrNewBkColor;
			int nItem=static_cast<int>(lplvcd->nmcd.dwItemSpec);

			m_list.GetItemText(nItem,20,str,64);

			doublenid=_atoi64(str);

			if(doublenid>0)
			{
				clrNewBkColor=RGB(255,255,255);//红底白字
				lplvcd->clrTextBk=clrNewBkColor;
				switch(doublenid%10)
				{
				case 0:
					clrNewTextColor=RGB(255,0,0);	break;
				case 1:
					clrNewTextColor=RGB(128,0,128);	break;
				case 2:
					clrNewTextColor=RGB(255,0,64);	break;
				case 3:
					clrNewTextColor=RGB(0,128,0);	break;
				case 4:
					clrNewTextColor=RGB(0,0,255);	break;
				case 5:
					clrNewTextColor=RGB(0,128,128);	break;
				case 6:
					clrNewTextColor=RGB(128,0,255);	break;
				case 7:
					clrNewTextColor=RGB(128,64,0);	break;
				case 8:
					clrNewTextColor=RGB(0,128,255);	break;
				case 9:
					clrNewTextColor=RGB(128,128,0);	break;
				}
					lplvcd->clrText=clrNewTextColor;
			}

			*pResult=CDRF_DODEFAULT;
			return;
		}
	}
}

void CZogvmDlg::OnHddDelete()
{
	// TODO: Add your command handler code here
	CDLGhdddelete DlgHdddelete;
	DlgHdddelete.DoModal();
}

void CZogvmDlg::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CZogvmDlg::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-PAGE_MAX_NUM) >= 0)
		ListNow-=PAGE_MAX_NUM;

	DisplayerList();
}

void CZogvmDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+PAGE_MAX_NUM) < ListTotal)
		ListNow+=PAGE_MAX_NUM;

	DisplayerList();
}

void CZogvmDlg::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+PAGE_MAX_NUM) < ListTotal)
		ListNow+=PAGE_MAX_NUM;

	DisplayerList();
}

void CZogvmDlg::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+PAGE_MAX_NUM) < (PAGE_MAX_NUM*m_page))
			)
			ListNow+=PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围");

}


DWORD WINAPI ExeclThreadPROC(LPVOID lpParameter)
{
	char path[260];
	strcpy(path,(char *)lpParameter);

	unsigned short fontsize=256;
	workbook w;
	worksheet **sheet;
	long sheetTotal=0;
	long long *filenum;
	xf_t *xf = NULL;

	int i=0;
	long j,k,m;
	CTime temptime;
	int ret;

	char str[2048];
	wchar_t wbuff[2048];

	list<struct HDD_ST> MyhddList;

	SQLDB.Hdd_SetNonsert();
	CDLGHdd::Add27HDDid();

	MyhddList.clear();
	SQLDB.Hdd_Read(MyhddList);
	if(MyhddList.size()<=0)
	{
		MessageBox(NULL,"没有硬盘","导出EXECL",MB_OK);
		return 0;
	}
	//导出硬盘
	sheetTotal=1;
	sheet=(worksheet **)calloc(sheetTotal,sizeof(worksheet *));
	filenum=(long long *)calloc(MyhddList.size(),sizeof(long long));

	xf= w.xformat(); 
	//xf->SetLocked(true) ;无效代码

	sheet[0]=w.sheet(L"硬盘目录");
	sheet[0]->colwidth(0,10*fontsize);
	sheet[0]->label(0,0,L"磁盘序列号");
	sheet[0]->colwidth(1,10*fontsize);
	sheet[0]->label(0,1,L"分区");
	sheet[0]->colwidth(2,30*fontsize);
	sheet[0]->label(0,2,L"标签");
	sheet[0]->colwidth(3,20*fontsize);
	sheet[0]->label(0,3,L"总空间");
	sheet[0]->colwidth(4,20*fontsize);
	sheet[0]->label(0,4,L"剩余空间");

	list<struct HDD_ST>::iterator beglist;

	for(i=1,beglist=MyhddList.begin();beglist!=MyhddList.end();i++,beglist++)
	{
		MultiByteToWideChar(CP_ACP, 0, beglist->serno, -1, wbuff, 2048);  
		sheet[0]->label(i,0,wbuff);
		MultiByteToWideChar(CP_ACP, 0, beglist->area, -1, wbuff, 2048);  
		sheet[0]->label(i,1,wbuff);
		MultiByteToWideChar(CP_ACP, 0, beglist->mark, -1, wbuff, 2048);  
		sheet[0]->label(i,2,wbuff);
		sprintf(str,"%I64u",beglist->TotalBytes);
		MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048);  
		sheet[0]->label(i,3,wbuff);
		sprintf(str,"%I64u",beglist->FreeBytes);
		MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
		sheet[0]->label(i,4,wbuff);
		
		filenum[i-1]=SQLDB.File_NumberForHDD(beglist->hdd_nid);
		sheetTotal += (long)(filenum[i-1]/10000) +1;
		
	}
	
	//导出文件
	sheet=(worksheet **)realloc(sheet,sheetTotal*sizeof(worksheet *));

	outputList.clear();

	list<struct FILE_VIEW_ST>::iterator filebeglist;

	k=1;

	for(i=0,beglist=MyhddList.begin();beglist!=MyhddList.end();i++,beglist++)
	{
		if(filenum[i] <= 0)
			continue;

		for(j=0;j< (long)(filenum[i]/10000) +1;j++)
		{
			sprintf(str,"%s %s %d",beglist->serno,beglist->mark,j);
			MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
			sheet[k]=w.sheet(wbuff);
		
			sheet[k]->colwidth(0,10*fontsize);
			sheet[k]->label(0,0,L"序号");
			sheet[k]->colwidth(1,150*fontsize);
			sheet[k]->label(0,1,L"文件路径");
			sheet[k]->colwidth(2,5*fontsize);
			sheet[k]->label(0,2,L"类型");
			sheet[k]->colwidth(3,5*fontsize);
			sheet[k]->label(0,3,L"后缀");
			sheet[k]->colwidth(4,20*fontsize);
			sheet[k]->label(0,4,L"创建时间");
			sheet[k]->colwidth(5,20*fontsize);
			sheet[k]->label(0,5,L"修改时间");
			sheet[k]->colwidth(6,15*fontsize);
			sheet[k]->label(0,6,L"文件大小");
			sheet[k]->colwidth(7,10*fontsize);
			sheet[k]->label(0,7,L"分辨率");
			sheet[k]->colwidth(8,5*fontsize);
			sheet[k]->label(0,8,L"3D");

			outputList.clear();
			SQLDB.File_ReadHddPage(outputList,beglist->hdd_nid,true,j*10000,10000);

			if(outputList.size() <= 0)
				continue;

			for(m=1,filebeglist=outputList.begin();filebeglist!=outputList.end();m++,filebeglist++)
			{
				sprintf(str,"%I64u",filebeglist->file_data.file_nid);
				MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
				sheet[k]->label(m,0,wbuff);

				sprintf(str,"%s%s",filebeglist->file_data.path,filebeglist->file_data.name);
				MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
				sheet[k]->label(m,1,wbuff);

				sprintf(str,"%s",MainType2Str(filebeglist->file_data.maintype));
				MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
				sheet[k]->label(m,2,wbuff);

				MultiByteToWideChar(CP_ACP, 0, filebeglist->file_data.type, -1, wbuff, 2048); 
				sheet[k]->label(m,3,wbuff);

				temptime=filebeglist->file_data.CreationTime;

				sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d", 
					temptime.GetYear(),temptime.GetMonth(),temptime.GetDay(),
					temptime.GetHour(),temptime.GetMinute(),temptime.GetSecond());
				MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
				sheet[k]->label(m,4,wbuff);

				temptime=filebeglist->file_data.LastWriteTime;

				sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d", 
					temptime.GetYear(),temptime.GetMonth(),temptime.GetDay(),
					temptime.GetHour(),temptime.GetMinute(),temptime.GetSecond());
				MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
				sheet[k]->label(m,5,wbuff);

				sprintf(str,"%I64u",filebeglist->file_data.filesize);
				MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, 2048); 
				sheet[k]->label(m,6,wbuff);

				MultiByteToWideChar(CP_ACP, 0, filebeglist->file_data.zidian_fenbianlv , -1, wbuff, 2048); 
				sheet[k]->label(m,7,wbuff);
				
				MultiByteToWideChar(CP_ACP, 0, filebeglist->file_data.zidian_3d, -1, wbuff, 2048); 
				sheet[k]->label(m,8,wbuff);
			}

			k++;

		}
	}
	
	MyhddList.clear();
	outputList.clear();
	MultiByteToWideChar(CP_ACP, 0, path, -1, wbuff, 2048);  
	ret = w.Dump(wbuff);

	free(filenum);
	free(sheet);
	pZogvmDlg->GetDlgItem(IDC_BUTTON_OUTPUTEXECL)->EnableWindow(TRUE);

	if (ret != 0)
	{
		MessageBox(NULL,"导出失败","导出EXECL",MB_OK);
		return 0;
	}

	MessageBox(NULL,"导出完成","导出EXECL",MB_OK);

	return 0;
}

void CZogvmDlg::OnBnClickedButtonOutputexecl()
{
	// TODO: Add your control notification handler code here
	char szFilter[]="Execl Files (*.xls)|*.xls|*.*|*.*||";
	CFileDialog dlg(FALSE,"xls","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()!=IDOK)
	{
		return ;
	}
	strcpy(OutputPath,dlg.GetPathName().GetBuffer(0));
	CreateThread(NULL,0,ExeclThreadPROC,OutputPath,0,NULL);
	GetDlgItem(IDC_BUTTON_OUTPUTEXECL)->EnableWindow(FALSE);

}


void CZogvmDlg::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
		LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char strarea[260];
		
	m_list.GetItemText(pNMIA->iItem,13,strarea,260);
	if(0!=strcmp(strarea,"接入"))
		return ;

	m_list.GetItemText(pNMIA->iItem,2,strarea,260);
	char strpath[260];
	m_list.GetItemText(pNMIA->iItem,4,strpath,260);
	char strname[260];
	m_list.GetItemText(pNMIA->iItem,5,strname,260);

	char cmdstr[512];
//	sprintf(cmdstr,"/e,/select,\"%s%s%s\"",strarea,strpath,strname);
	sprintf(cmdstr,"/select,\"%s%s%s\"",strarea,strpath,strname);

	//打开文件所在的文件夹
	ShellExecute(this->m_hWnd,
		"open",
		"explorer.exe",
		cmdstr,
		NULL,SW_NORMAL);

	*pResult = 0;
}

void CZogvmDlg::OnBnClickedButtonTrash()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ListTotal=0;
	ListTotal=SQLDB.File_TrashNumber();
	ListNow=0;

	ModeFlag=MODE_TRASH;
	DisplayerList();

}

void CZogvmDlg::OnBnClickedButtonTrashClean()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(	MODE_TRASH==ModeFlag)
	{
		if(!m_c_delete_trash)
		{
			MessageBox("不小心点到的吧？真的要删，请勾选 真的删");
			return ;
		}

		WinfileviewList.clear();

		SQLDB.File_ReadTrash(WinfileviewList,true,0,PAGE_MAX_NUM);

		if(WinfileviewList.size()>0)
		{
			list<struct FILE_VIEW_ST>::iterator beglist;
			char str[512];
			
			SQLDB.Begin();
			for(beglist=WinfileviewList.begin();beglist!=WinfileviewList.end();beglist++)
			{
				if(beglist->hdd_data.insertflag)
				{
					sprintf(str,"%s%s%s",beglist->hdd_data.area,beglist->file_data.path,beglist->file_data.name);
					DeleteFile(str);
					SQLDB.File_Delete(beglist->file_data.file_nid);
				}
			}
			SQLDB.Commit();
		}
		MessageBox("本页清理完毕");

	}
	else
		MessageBox("不是 垃圾文件 模式，按钮无效");

	m_c_delete_trash=FALSE;
	UpdateData(FALSE);
}

void CZogvmDlg::OnYyets()
{
	// TODO: Add your command handler code here
	CDLGYYETS DlgYyets;
	DlgYyets.DoModal();
}
