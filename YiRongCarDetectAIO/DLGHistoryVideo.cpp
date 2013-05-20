// DLGHistoryVideo.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGHistoryVideo.h"

#include "DLGscreen.h"

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

#include "DLGWarnning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern IO OracleIO;


/////////////////////////////////////////////////////////////////////////////
// CDLGHistoryVideo dialog


CDLGHistoryVideo::CDLGHistoryVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGHistoryVideo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGHistoryVideo)
	m_page = 1;
	m_name = _T("");
	m_ip = _T("");
	m_CheckTime = FALSE;
	//}}AFX_DATA_INIT

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;
	m_EndMon=nowtime;
	m_EndHour=nowtime;

	ListChoose=-1;
	ListTotal=0;
	ListNow=0;
}


void CDLGHistoryVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGHistoryVideo)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 260);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDV_MaxChars(pDX, m_ip, 32);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_TIME, m_CheckTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGHistoryVideo, CDialog)
	//{{AFX_MSG_MAP(CDLGHistoryVideo)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST,OnLvnItemActivateList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGHistoryVideo message handlers
BOOL CDLGHistoryVideo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 60);
	m_List.InsertColumn(1, _T("设备名称" ), LVCFMT_LEFT, 140);
	m_List.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, 140);
	m_List.InsertColumn(3, _T("文件格式"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(4, _T("文件大小"), LVCFMT_LEFT, 90);
	m_List.InsertColumn(5, _T("起始时间"), LVCFMT_LEFT, 130);
	m_List.InsertColumn(6, _T("结束时间"), LVCFMT_LEFT, 130);
	m_List.InsertColumn(7, _T("文件路径"), LVCFMT_LEFT, 50);
	m_List.InsertColumn(8, _T("编号"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(9, _T("厂家"), LVCFMT_LEFT, 0);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	ChooseEnable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDLGHistoryVideo::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CDLGHistoryVideo::DisplayerList(void)
{
	list_history_video.clear();
	m_List.DeleteAllItems();
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
	ListChoose=-1;
	if(0==ListTotal)
		return ;

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+HISTORY_VIDEO_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+HISTORY_VIDEO_PAGE_MAX_NUM ;
	else
		ei=ListTotal;
	//查询
	OracleIO.VIDEO_Read(SqlStr,searchFlag,si,ei,list_history_video);
	//显示
	int nItem;
	char str[128];
	int i=0;

	list<struct HISTORY_VIDEO_ST>::iterator beglist;

	for(beglist=list_history_video.begin();beglist!=list_history_video.end();beglist++)
	{
		sprintf(str,"%07d",ListNow+1+i);
		nItem = m_List.InsertItem(0,str);
		i++;
	
		m_List.SetItemText(nItem,1,beglist->name);
		m_List.SetItemText(nItem,2,beglist->ip);
		m_List.SetItemText(nItem,3,beglist->format);

		sprintf(str,"%d",beglist->size);
		m_List.SetItemText(nItem,4,str);

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
			beglist->StartYear,
			beglist->StartMon,
			beglist->StartDay,
			beglist->StartHour,
			beglist->StartMin,
			beglist->StartSec);
		m_List.SetItemText(nItem,5,str);

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
			beglist->EndYear,
			beglist->EndMon,
			beglist->EndDay,
			beglist->EndHour,
			beglist->EndMin,
			beglist->EndSec);
		m_List.SetItemText(nItem,6,str);

		m_List.SetItemText(nItem,7,beglist->path);

		sprintf(str,"%d",beglist->nid);
		m_List.SetItemText(nItem,8,str);

		sprintf(str,"%d",beglist->venderID);
		m_List.SetItemText(nItem,9,str);
	}

	if(ListTotal%HISTORY_VIDEO_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/HISTORY_VIDEO_PAGE_MAX_NUM+1,ListTotal/HISTORY_VIDEO_PAGE_MAX_NUM+1);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/HISTORY_VIDEO_PAGE_MAX_NUM+1,ListTotal/HISTORY_VIDEO_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);

	ChooseEnable();

}

void CDLGHistoryVideo::OnButtonSearch() 
{
	UpdateData(TRUE);

	char STime[32]={0};
	char ETime[32]={0};

	sprintf(STime,"%04d%02d%02d%02d%02d%02d",		
		m_StartMon.GetYear(),
		m_StartMon.GetMonth(),
		m_StartMon.GetDay(),
		m_StartHour.GetHour(),
		m_StartHour.GetMinute(),
		m_StartHour.GetSecond());

	sprintf(ETime,"%04d%02d%02d%02d%02d%02d",		
		m_EndMon.GetYear(),
		m_EndMon.GetMonth(),
		m_EndMon.GetDay(),
		m_EndHour.GetHour(),
		m_EndHour.GetMinute(),
		m_EndHour.GetSecond());

	ListTotal=0;

	searchFlag=0;
	if(!m_name.IsEmpty())
	{
		searchFlag |= 0x01;
	}
	if(!m_ip.IsEmpty())
	{
		searchFlag |= 0x02;
	}
	if(m_CheckTime)
	{
		searchFlag |= 0x04;
	}

	memset(SqlStr,0,1024);
	ListTotal=OracleIO.VIDEO_GetNum(m_name.GetBuffer(0),
								   m_ip.GetBuffer(0),
								   STime,ETime,
								   searchFlag,
								   SqlStr);
	ListNow=0;
	DisplayerList();
}

void CDLGHistoryVideo::ChooseEnable(void)
{
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

void CDLGHistoryVideo::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	if(ListChoose<0)
		return ;

	int venderID;
	char str[260];
	m_List.GetItemText(ListChoose,9,str,260);
	sscanf(str,"%d",&venderID);

	m_List.GetItemText(ListChoose,7,str,260);
		// TODO: Add your message handler code here
	switch(venderID)
	{
	case VENDER_TYPE_HAIKANG:		//海康
		ShellExecute(this->m_hWnd,
			"open",
			DlgSetSystem.m_path_haikang,
			str,
			NULL,SW_NORMAL);
		break;
	case VENDER_TYPE_DAHUA:		//大华
		ShellExecute(this->m_hWnd,
			"open",
			DlgSetSystem.m_path_dahua,
			str,
			NULL,SW_NORMAL);
		break;
	case VENDER_TYPE_YAAN:
		ShellExecute(this->m_hWnd,
			"open",
			DlgSetSystem.m_path_yaan,
			str,
			NULL,SW_NORMAL);
		break;
	default:
		break;
	}
}

void CDLGHistoryVideo::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGHistoryVideo::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-HISTORY_VIDEO_PAGE_MAX_NUM) >= 0)
		ListNow-=HISTORY_VIDEO_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryVideo::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+HISTORY_VIDEO_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_VIDEO_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryVideo::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+HISTORY_VIDEO_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_VIDEO_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryVideo::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/HISTORY_VIDEO_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+HISTORY_VIDEO_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+HISTORY_VIDEO_PAGE_MAX_NUM) < (HISTORY_VIDEO_PAGE_MAX_NUM*m_page))
			)
			ListNow+=HISTORY_VIDEO_PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);

}


void CDLGHistoryVideo::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;
	//选中的赋值
	ListChoose= p->iItem;
	//没中
	if(ListChoose <0 )
		ListChoose=-1 ;

	ChooseEnable();

	*pResult = 0;
}

void CDLGHistoryVideo::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];
	m_List.GetItemText(pNMIA->iItem,7,str,260);
	char cmdstr[512];

	sprintf(cmdstr,"/e,/select,%s",str);

	//打开文件所在的文件夹
	ShellExecute(this->m_hWnd,
		"open",
		"explorer.exe",
		cmdstr,
		NULL,SW_NORMAL);

	*pResult = 0;
}

void CDLGHistoryVideo::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	if(ListChoose<0)
		return ;

	//删除数据库
	unsigned long int nid;
	char str[260];
	m_List.GetItemText(ListChoose,0,str,260);
	CString temp=str;

	//警告框
	CDLGWarnning dlgw;
	dlgw.m_wintxt="删除该视频 序号:"+temp;	//窗口标题
	m_List.GetItemText(ListChoose,7,str,260);
	temp=str;
	dlgw.m_warntxt="请再次确定是否删除:"+temp;	//窗口内容
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{
		m_List.GetItemText(ListChoose,8,str,260);
		
		sscanf(str,"%d",&nid);
		OracleIO.VIDEO_DeleteVideo(nid);
		
		//删除文件
		m_List.GetItemText(ListChoose,7,str,260);
		DeleteFile(str);
		
		ListNow=0;
		DisplayerList();
	}
}
