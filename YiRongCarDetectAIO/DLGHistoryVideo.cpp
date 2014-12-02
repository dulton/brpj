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

	CTime cstime(
		nowtime.GetYear(),
		nowtime.GetMonth(),
		nowtime.GetDay(),
		0,	0,	0);

	m_StartHour=cstime;

	nowtime+=3600;
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
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_search);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_first_button);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_pre_button);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_next_button);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_last_button);
	DDX_Control(pDX, IDC_BUTTON_JUMP, m_jump_button);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_b_delete);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_b_play);
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
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGHistoryVideo message handlers
BOOL CDLGHistoryVideo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 60);
	m_List.InsertColumn(1, _T("摄像头名称" ), LVCFMT_LEFT, 140);
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

	OnCheckTime();

	GetDlgItem(IDC_STATIC_INFO)->GetWindowRect(printf_Rect);
	ScreenToClient(printf_Rect);

	m_search.LoadBitmaps(IDB_FIND_BUTTON,IDB_FIND_BUTTON_MOVE,NULL,NULL);
	m_search.SizeToContent();		//自适应图片大小

	m_first_button.LoadBitmaps(IDB_FIRST_BUTTON,IDB_FIRST_BUTTON_MOVE,NULL,NULL);
	m_first_button.SizeToContent();		//自适应图片大小

	m_pre_button.LoadBitmaps(IDB_PRE_BUTTON,IDB_PRE_BUTTON_MOVE,NULL,NULL);
	m_pre_button.SizeToContent();		//自适应图片大小

	m_next_button.LoadBitmaps(IDB_NEXT_BUTTON,IDB_NEXT_BUTTON_MOVE,NULL,NULL);
	m_next_button.SizeToContent();		//自适应图片大小

	m_last_button.LoadBitmaps(IDB_LAST_BUTTON,IDB_LAST_BUTTON_MOVE,NULL,NULL);
	m_last_button.SizeToContent();		//自适应图片大小

	m_jump_button.LoadBitmaps(IDB_JUMP_BUTTON,IDB_JUMP_BUTTON_MOVE,NULL,NULL);
	m_jump_button.SizeToContent();		//自适应图片大小


	m_b_play.LoadBitmaps(IDB_PLAY_BUTTON,IDB_PLAY_BUTTON_MOVE,NULL,IDB_PLAY_BUTTON_DIS);
	m_b_play.SizeToContent();		//自适应图片大小

	m_b_delete.LoadBitmaps(IDB_DEL_BUTTON,IDB_DEL_BUTTON_MOVE,NULL,IDB_DEL_BUTTON_DIS);
	m_b_delete.SizeToContent();		//自适应图片大小


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
	ListChoose=-1;

	if(0==ListTotal)
	{
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
		InvalidateRect(printf_Rect, TRUE);
		return ;
	}

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

	//	sprintf(str,"%d",beglist->size);
		if(beglist->size <= 1024 )
			sprintf(str,"%d B",beglist->size);
		else if(beglist->size > 1024 && beglist->size <= 1048576)
			sprintf(str,"%.2f KB",beglist->size/1024.0);
		else if(beglist->size > 1048576 && beglist->size <= 1073741824)
			sprintf(str,"%.2f MB",beglist->size/1048576.0);
		else if(beglist->size > 1073741824)
			sprintf(str,"%.2f GB",beglist->size/1073741824.0);
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
	InvalidateRect(printf_Rect, TRUE);

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
	
		CTime cstime(
			m_StartMon.GetYear(),
			m_StartMon.GetMonth(),
			m_StartMon.GetDay(),
			m_StartHour.GetHour(),
			m_StartHour.GetMinute(),
			m_StartHour.GetSecond());
		
		CTime cetime(
			m_EndMon.GetYear(),
			m_EndMon.GetMonth(),
			m_EndMon.GetDay(),
			m_EndHour.GetHour(),
			m_EndHour.GetMinute(),
			m_EndHour.GetSecond());
		
		if(cstime>cetime)
		{
			MessageBox("起始时间 不得大于 结束时间");
			return ;
		}
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

	sprintf(cmdstr,"/e,/select,\"%s\"",str);

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

void CDLGHistoryVideo::OnCheckTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_CheckTime)
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDHOUR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDHOUR)->EnableWindow(FALSE);
	}
}


void CDLGHistoryVideo::OnPaint()
{

	CPaintDC dc(this); // device context for painting
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_FIND_BACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	CDialog::OnPaint();

}
//静态文本控件 透明
HBRUSH CDLGHistoryVideo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); // 设置透明背景
		// TODO: Change any attributes of the DC here
		pDC->SetTextColor(RGB(0, 0, 0)); // 设置文本颜色
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		hbr=(HBRUSH)GetStockObject(HOLLOW_BRUSH); // 返回透明画刷	
		// TODO: Return a different brush if the default is not desired
	}

	return hbr;
}