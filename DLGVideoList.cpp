// DLGVideoList.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGVideoList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;
extern bool IsChinese;

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoList dialog


CDLGVideoList::CDLGVideoList(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGVideoList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGVideoList)
	m_page = 0;
	m_CheckTime=FALSE;
	m_runnum = _T("");
	m_desc = _T("");
	m_hmnum = _T("");
	//}}AFX_DATA_INIT
	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;
	m_EndMon=nowtime;
	m_EndHour=nowtime;

	ListTotal=0;
	ListNow=0;
	ListChoose=-1;
}


void CDLGVideoList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGVideoList)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_TIME, m_CheckTime);
	DDX_Text(pDX, IDC_EDIT_RUNNUM, m_runnum);
	DDV_MaxChars(pDX, m_runnum, 260);
	DDX_Text(pDX, IDC_EDIT_DESC, m_desc);
	DDV_MaxChars(pDX, m_desc, 260);
	DDX_Text(pDX, IDC_EDIT_HMNUM, m_hmnum);
	DDV_MaxChars(pDX, m_hmnum, 260);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGVideoList, CDialog)
	//{{AFX_MSG_MAP(CDLGVideoList)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST,OnLvnItemActivateList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoList message handlers
BOOL CDLGVideoList::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();

	m_list.InsertColumn(0, _T("No.") , LVCFMT_LEFT, 65);
	m_list.InsertColumn(1, _T("Running Number") , LVCFMT_LEFT, 150);
	m_list.InsertColumn(2, _T("Carton TAG" ), LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, _T("Hm Number" ), LVCFMT_LEFT, 120);
	m_list.InsertColumn(4, _T("Description" ), LVCFMT_LEFT, 220);
	m_list.InsertColumn(5, _T("Start Time" ), LVCFMT_LEFT, 130);
	m_list.InsertColumn(6, _T("End Time" ), LVCFMT_LEFT, 130);
	m_list.InsertColumn(7, _T("File Size" ), LVCFMT_LEFT, 80);
	m_list.InsertColumn(8, _T("File Path" ), LVCFMT_LEFT, 0);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGVideoList::OnOK() 
{
	// TODO: Add extra validation here
	

}

void CDLGVideoList::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	char STime[32]={0};
	char ETime[32]={0};

	sprintf(STime,"%04d-%02d-%02d %02d:%02d:%02d",		
		m_StartMon.GetYear(),
		m_StartMon.GetMonth(),
		m_StartMon.GetDay(),
		m_StartHour.GetHour(),
		m_StartHour.GetMinute(),
		m_StartHour.GetSecond());

	sprintf(ETime,"%04d-%02d-%02d %02d:%02d:%02d",	
		m_EndMon.GetYear(),
		m_EndMon.GetMonth(),
		m_EndMon.GetDay(),
		m_EndHour.GetHour(),
		m_EndHour.GetMinute(),
		m_EndHour.GetSecond());

	ListTotal=0;

	searchFlag=0;

	if(!m_runnum.IsEmpty())
	{
		searchFlag |= 0x01;
	}
	if(!m_hmnum.IsEmpty())
	{
		searchFlag |= 0x02;
	}
	if(!m_desc.IsEmpty())
	{
		searchFlag |= 0x04;
	}
	if(m_CheckTime)
	{
		searchFlag |= 0x08;
	}

	memset(SqlStr,0,1024);
	ListTotal=pCMainDlg->SQLiteIO.Video_GetCount(
								   m_runnum.GetBuffer(0),
								   m_hmnum.GetBuffer(0),
								   m_desc.GetBuffer(0),
								   STime,ETime,
								   searchFlag,
								   SqlStr);
	ListNow=0;
	DisplayerList();
}

void CDLGVideoList::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
	
}

void CDLGVideoList::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-VIDEO_INFO_MAX_NUM) >= 0)
		ListNow-=VIDEO_INFO_MAX_NUM;

	DisplayerList();

}

void CDLGVideoList::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+VIDEO_INFO_MAX_NUM) < ListTotal)
		ListNow+=VIDEO_INFO_MAX_NUM;

	DisplayerList();
}

void CDLGVideoList::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+VIDEO_INFO_MAX_NUM) < ListTotal)
		ListNow+=VIDEO_INFO_MAX_NUM;

	DisplayerList();
}

void CDLGVideoList::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/VIDEO_INFO_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+VIDEO_INFO_MAX_NUM) < ListTotal)
			&& ((ListNow+VIDEO_INFO_MAX_NUM) < (VIDEO_INFO_MAX_NUM*m_page))
			)
			ListNow+=VIDEO_INFO_MAX_NUM;

		DisplayerList();
	}	
	else
	{
		if(IsChinese)
			MessageBox("不在页面范围");
		else
			MessageBox("This page can not be found");
	}
}

void CDLGVideoList::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;
	//选中的赋值
	ListChoose= p->iItem;
	//没中
	if( ListChoose < 0)
		ListChoose=-1;

	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}

	//更新
	UpdateData(FALSE);

	*pResult = 0;
}


void CDLGVideoList::DisplayerList(void)
{
	list_video.clear();
	m_list.DeleteAllItems();

	if(IsChinese)
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("Total 0 items. 1/1 Pages");
	else
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共 0 条. 1/1 页");

	ListChoose=-1;

	if(0==ListTotal)
		return ;

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+VIDEO_INFO_MAX_NUM) < ListTotal)
		ei=ListNow+VIDEO_INFO_MAX_NUM ;
	else
		ei=ListTotal;

	//查询数据库
	pCMainDlg->SQLiteIO.Video_GetHistory(SqlStr, searchFlag,si,ei,list_video);

	int nItem;
	char str[128];

	list<struct VIDEO_INFO_ST>::iterator beglist;

	for(beglist=list_video.begin();beglist!=list_video.end();beglist++)
	{
		sprintf(str,"%07d",beglist->nid);
		nItem = m_list.InsertItem(0,str);

		m_list.SetItemText(nItem,1,beglist->RunningNumber);
		m_list.SetItemText(nItem,2,beglist->tag);
		m_list.SetItemText(nItem,3,beglist->HmNum);
		m_list.SetItemText(nItem,4,beglist->Description);

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
			beglist->start_year,
			beglist->start_mon,
			beglist->start_day,
			beglist->start_hour,
			beglist->start_min,
			beglist->start_sec);
		m_list.SetItemText(nItem,5,str);

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
			beglist->end_year,
			beglist->end_mon,
			beglist->end_day,
			beglist->end_hour,
			beglist->end_min,
			beglist->end_sec);
		m_list.SetItemText(nItem,6,str);

		sprintf(str,"%.2f MB",beglist->size/1024.0/1024.0);
		m_list.SetItemText(nItem,7,str);

		m_list.SetItemText(nItem,8,beglist->path);
	
	}

	if(ListTotal%VIDEO_INFO_MAX_NUM)
	{
		if(IsChinese)
			sprintf(str,"共%d条 %d/%d页",
				ListTotal,
				ListNow/VIDEO_INFO_MAX_NUM+1,ListTotal/VIDEO_INFO_MAX_NUM+1);
		else
			sprintf(str,"Total %d items. %d/%d Pages",
				ListTotal,
				ListNow/VIDEO_INFO_MAX_NUM+1,ListTotal/VIDEO_INFO_MAX_NUM+1);
	}
	else
	{	
		if(IsChinese)
			sprintf(str,"共%d条 %d/%d页",
				ListTotal,
				ListNow/VIDEO_INFO_MAX_NUM+1,ListTotal/VIDEO_INFO_MAX_NUM);
		else
			sprintf(str,"Total %d items. %d/%d Pages",
				ListTotal,
				ListNow/VIDEO_INFO_MAX_NUM+1,ListTotal/VIDEO_INFO_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);

	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}
}

void CDLGVideoList::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];


//	m_list.GetItemText(pNMIA->iItem,10,str,260);

	/*
	//打开文件所在的文件夹
	ShellExecute(this->m_hWnd,
		"open",
		"explorer.exe",
		"/e,/select,D:\\GGG\\YiRongCarDetectAIO\\IO定义\\历史识别报警表 数据库IO规则.txt",
		NULL,SW_NORMAL);
		*/

	ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	*pResult = 0;
}
