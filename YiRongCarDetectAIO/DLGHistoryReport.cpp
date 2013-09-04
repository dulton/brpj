// CDLGHistoryReport.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGHistoryReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern IO OracleIO;

/////////////////////////////////////////////////////////////////////////////
// DLGHistoryReport dialog


CDLGHistoryReport::CDLGHistoryReport(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGHistoryReport::IDD, pParent)
{

	//{{AFX_DATA_INIT(CDLGHistoryReport)
	m_page = 1;
	m_user = _T("");
	m_CheckTime = FALSE;
	//}}AFX_DATA_INIT

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;
	m_EndMon=nowtime;
	m_EndHour=nowtime;
	
	ListTotal=0;
	ListNow=0;
}


void CDLGHistoryReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGHistoryReport)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_Text(pDX, IDC_EDIT_USER, m_user);
	DDV_MaxChars(pDX, m_user, 32);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_TIME, m_CheckTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGHistoryReport, CDialog)
	//{{AFX_MSG_MAP(CDLGHistoryReport)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGHistoryReport message handlers
BOOL CDLGHistoryReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 60);
	m_List.InsertColumn(1, _T("用户名" ), LVCFMT_LEFT, 140);
	m_List.InsertColumn(2, _T("时间"), LVCFMT_LEFT, 140);
	m_List.InsertColumn(3, _T("日志信息"), LVCFMT_LEFT, 380);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	OnCheckTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGHistoryReport::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CDLGHistoryReport::DisplayerList(void)
{
	list_history_report.clear();
	m_List.DeleteAllItems();
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");

	if(0==ListTotal)
		return ;

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+HISTORY_REPORT_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+HISTORY_REPORT_PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	//查询数据库
	OracleIO.LOG_ReadSystemLog(sqlstr,searchFlag,si,ei,list_history_report);

	int nItem;
	char str[128];
	int i=0;

	list<struct HISTORY_REPORT_ST>::iterator beglist;

	for(beglist=list_history_report.begin();beglist!=list_history_report.end();beglist++)
	{
		sprintf(str,"%07d",ListNow+1+i);
		nItem = m_List.InsertItem(0,str);
		i++;

		m_List.SetItemText(nItem,1,beglist->user);

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
			beglist->year,
			beglist->mon,
			beglist->day,
			beglist->hour,
			beglist->min,
			beglist->sec);

		m_List.SetItemText(nItem,2,str);

		m_List.SetItemText(nItem,3,beglist->str);
	}

	if(ListTotal%HISTORY_REPORT_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/HISTORY_REPORT_PAGE_MAX_NUM+1,ListTotal/HISTORY_REPORT_PAGE_MAX_NUM+1);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/HISTORY_REPORT_PAGE_MAX_NUM+1,ListTotal/HISTORY_REPORT_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
}

void CDLGHistoryReport::OnButtonSearch() 
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

	/////////获取
	searchFlag = 0;

	if(!m_user.IsEmpty())
	{
		searchFlag |= 0x01;
	}
	if(m_CheckTime)
	{
		searchFlag |= 0x02;
	
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

	ListTotal=0;
	ListTotal=OracleIO.LOG_GetSystemLogNum(m_user.GetBuffer(0),STime,ETime,searchFlag,sqlstr);
	ListNow=0;

	DisplayerList();
}

void CDLGHistoryReport::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGHistoryReport::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-HISTORY_REPORT_PAGE_MAX_NUM) >= 0)
		ListNow-=HISTORY_REPORT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryReport::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+HISTORY_REPORT_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_REPORT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryReport::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+HISTORY_REPORT_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_REPORT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryReport::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/HISTORY_REPORT_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+HISTORY_REPORT_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+HISTORY_REPORT_PAGE_MAX_NUM) < (HISTORY_REPORT_PAGE_MAX_NUM*m_page))
			)
			ListNow+=HISTORY_REPORT_PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);
}


void CDLGHistoryReport::OnCheckTime() 
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
