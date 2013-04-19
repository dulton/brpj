// DLGHistoryVideo.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGHistoryVideo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGHistoryVideo dialog


CDLGHistoryVideo::CDLGHistoryVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGHistoryVideo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGHistoryVideo)
	m_page = 0;
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
	DDX_Control(pDX, IDC_LIST, m_list);
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGHistoryVideo message handlers
BOOL CDLGHistoryVideo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 50);
	m_list.InsertColumn(1, _T("设备名称" ), LVCFMT_LEFT, 140);
	m_list.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, 140);
	m_list.InsertColumn(3, _T("文件大小"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(4, _T("起始时间"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(5, _T("结束时间"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(6, _T("文件路径"), LVCFMT_LEFT, 140);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDLGHistoryVideo::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CDLGHistoryVideo::DisplayerList(void)
{
	int nItem;

	list<struct HISTORY_VIDEO_ST>::iterator beglist;

	char timestr[128];

	int i;
	int total=0;

//	list_history_video.DeleteAllItems();

	beglist=list_history_video.begin();
	for(i=0;i<ListNow;i++)
	{
		beglist++;
	}

	for(NULL;(beglist!=list_history_video.end()) && (total < HISTORY_VIDEO_PAGE_MAX_NUM);beglist++)
	{
		/*
		sprintf(timestr,"%05d",total+ListNow+1);
		nItem = m_zogHistoryList.InsertItem(0,timestr);

		sprintf(timestr,"%04d-%02d-%02d  %02d:%02d:%02d",
			beglist->year,
			beglist->mon,
			beglist->day,
			beglist->hour,
			beglist->min,
			beglist->sec);
*/
		total++;
	}
/*
	sprintf(timestr,"共%d条",ListTotal);
	GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText(timestr);
	
	sprintf(timestr,"%d/%d",ListNow/HL_PAGE_MAX_NUM+1,ListTotal/HL_PAGE_MAX_NUM+1);
	GetDlgItem(IDC_STATIC_PAGE)->SetWindowText(timestr);
	*/
}

void CDLGHistoryVideo::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

/*
	CTime	StartT(m_TimeStartMon.GetYear(),m_TimeStartMon.GetMonth(),m_TimeStartMon.GetDay(),	\
		m_TimeStartHour.GetHour(),m_TimeStartHour.GetMinute(),m_TimeStartHour.GetSecond());

	CTime	EndT(m_TimeEndMon.GetYear(),m_TimeEndMon.GetMonth(),m_TimeEndMon.GetDay(),	\
		m_TimeEndHour.GetHour(),m_TimeEndHour.GetMinute(),m_TimeEndHour.GetSecond());
*/
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
	ListNow=0;

	/*
	zoglisthistory.clear();

	/////////获取
	unsigned char getFlag = 0;

	if(m_TimeFlag)
	{
		getFlag |= 0x01;
	}
	if(m_BlackFlag)
	{
		getFlag |= 0x02;
	}
	if(m_CarnumberFlag && (!m_CarNumber.IsEmpty()))
	{
		getFlag |= 0x04;
	}
	if(m_IPflag && (!m_IPaddr.IsEmpty()))
	{
		getFlag |= 0x08;
	}
	//USHOW
	//m_maindlg->pio.getHistoryCarList(getFlag,zoglisthistory,STime,ETime,m_CarNumber.GetBuffer(),m_IPaddr.GetBuffer());
	////////

	ListTotal=zoglisthistory.size();
	DisplayerList();
	*/
}

void CDLGHistoryVideo::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

}

void CDLGHistoryVideo::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	ListNow=0;
	DisplayerList();
	
	ListChoose=-1;
}

void CDLGHistoryVideo::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if((ListNow-HISTORY_VIDEO_PAGE_MAX_NUM) >= 0)
		ListNow-=HISTORY_VIDEO_PAGE_MAX_NUM;

	DisplayerList();

		ListChoose=-1;
}

void CDLGHistoryVideo::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if((ListNow+HISTORY_VIDEO_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_VIDEO_PAGE_MAX_NUM;

	DisplayerList();
		ListChoose=-1;
}

void CDLGHistoryVideo::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	while((ListNow+HISTORY_VIDEO_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_VIDEO_PAGE_MAX_NUM;

	DisplayerList();
	ListChoose=-1;
}

void CDLGHistoryVideo::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
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
		ListChoose=-1;
	}
}

void CDLGHistoryVideo::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;
	//选中的赋值
	ListChoose= p->iItem;
	
	*pResult = 0;
}
