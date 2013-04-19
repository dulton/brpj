// DLGHistroyDetect.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGHistroyDetect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern IO OracleIO;

/////////////////////////////////////////////////////////////////////////////
// CDLGHistroyDetect dialog


CDLGHistroyDetect::CDLGHistroyDetect(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGHistroyDetect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGHistroyDetect)
	m_ip = _T("");
	m_name = _T("");
	m_page = 0;
	m_plate = _T("");
	m_platetype = 0;
	m_platecolor = 0;
	m_direction = 0;
	m_carcolor = 0;
	m_CheckTime=FALSE;
	//}}AFX_DATA_INIT
	flag=0;

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;
	m_EndMon=nowtime;
	m_EndHour=nowtime;

	ListTotal=0;
	ListNow=0;
}


void CDLGHistroyDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGHistroyDetect)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDV_MaxChars(pDX, m_ip, 32);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 260);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_Text(pDX, IDC_EDIT_PLATE, m_plate);
	DDV_MaxChars(pDX, m_plate, 32);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_TIME, m_CheckTime);
	DDX_CBIndex(pDX, IDC_COMBO_PLATETYPE, m_platetype);
	DDX_CBIndex(pDX, IDC_COMBO_PLATECOLOR, m_platecolor);
	DDX_CBIndex(pDX, IDC_COMBO_DIRECTION, m_direction);
	DDX_CBIndex(pDX, IDC_COMBO_CARCOLOR, m_carcolor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGHistroyDetect, CDialog)
	//{{AFX_MSG_MAP(CDLGHistroyDetect)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST,OnLvnItemActivateList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGHistroyDetect message handlers
BOOL CDLGHistroyDetect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//必须预定义后再弹窗口
	switch(flag)
	{
	case HISTORY_DETECT_FLAG_CAR :
		SetWindowText("历史识别查询");
		break;
	case HISTORY_DETECT_FLAG_ALARM :
		SetWindowText("历史报警查询");
		break;
	default:
		MessageBox("BOOL CDLGHistroyDetect::OnInitDialog error",MESSAGEBOX_TITLE);
		return true;
	}

#if ALLTAB_DETECT_CAR_MODE
	m_list.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_list.InsertColumn(2, _T("摄像头名称" ), LVCFMT_LEFT, 140);
	m_list.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(7, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(8, _T("车牌类型"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(9, _T("车身颜色"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(10, _T("图片路径"), LVCFMT_LEFT, 0);
#else
	m_list.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_list.InsertColumn(2, _T("摄像头名称" ), LVCFMT_LEFT, 140);
	m_list.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 140);
	m_list.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(7, _T("图片路径"), LVCFMT_LEFT, 0);

	//屏蔽车牌类型 车牌颜色 车身颜色
	GetDlgItem(IDC_COMBO_PLATETYPE)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_PLATECOLOR)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_CARCOLOR)->ShowWindow(FALSE);
#endif

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDLGHistroyDetect::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CDLGHistroyDetect::OnButtonSearch() 
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

	CString directionstr="";
	CString platecolorstr="";
	CString platetypestr="";
	CString carcolorstr="";

	if(!m_name.IsEmpty())
	{
		searchFlag |= 0x01;
	}
	if(!m_ip.IsEmpty())
	{
		searchFlag |= 0x02;
	}
	if(!m_plate.IsEmpty())
	{
		searchFlag |= 0x04;
	}
	if(m_CheckTime)
	{
		searchFlag |= 0x08;
	}
	if(m_direction >0)
	{
		searchFlag |= 0x10;
		((CComboBox *)GetDlgItem(IDC_COMBO_DIRECTION))->GetLBText(m_direction,directionstr);
	}

#if ALLTAB_DETECT_CAR_MODE
//汽车
	if(m_platecolor >0)
	{
		searchFlag |= 0x20;
		((CComboBox *)GetDlgItem(IDC_COMBO_PLATECOLOR))->GetLBText(m_platecolor,platecolorstr);
	}
	if(m_platetype >0)
	{
		searchFlag |= 0x40;
		((CComboBox *)GetDlgItem(IDC_COMBO_PLATETYPE))->GetLBText(m_platetype,platetypestr);
	}
	if(m_carcolor >0)
	{
		searchFlag |= 0x80;
		((CComboBox *)GetDlgItem(IDC_COMBO_CARCOLOR))->GetLBText(m_carcolor,carcolorstr);	
	}

	ListTotal=0;

	switch(flag)
	{
	case HISTORY_DETECT_FLAG_CAR :
		ListTotal=OracleIO.CAR_MatchResult_GetNum(
			m_name.GetBuffer(0),
			m_ip.GetBuffer(0),
			m_plate.GetBuffer(0),
			STime,
			ETime,
			directionstr.GetBuffer(0),
			platecolorstr.GetBuffer(0),
			platetypestr.GetBuffer(0),
			carcolorstr.GetBuffer(0),
			searchFlag,
			SqlStr);
		break;
	case HISTORY_DETECT_FLAG_ALARM :
		ListTotal=OracleIO.CAR_AlarmResult_GetNum(
			m_name.GetBuffer(0),
			m_ip.GetBuffer(0),
			m_plate.GetBuffer(0),
			STime,
			ETime,
			directionstr.GetBuffer(0),
			platecolorstr.GetBuffer(0),
			platetypestr.GetBuffer(0),
			carcolorstr.GetBuffer(0),
			searchFlag,
			SqlStr);
		break;
	default:
		MessageBox("CDLGHistroyDetect::OnButtonSearch A error",MESSAGEBOX_TITLE);
		return ;
	}

#else
//电动车
	ListTotal=0;

	switch(flag)
	{
	case HISTORY_DETECT_FLAG_CAR :
		ListTotal=OracleIO.ELECAR_MatchResult_GetNum(		
			m_name.GetBuffer(0),
			m_ip.GetBuffer(0),
			m_plate.GetBuffer(0),
			STime,
			ETime,
			directionstr.GetBuffer(0),
			searchFlag,
			SqlStr);
		break;
	case HISTORY_DETECT_FLAG_ALARM :
		ListTotal=OracleIO.ELECAR_AlarmResult_GetNum(
			m_name.GetBuffer(0),
			m_ip.GetBuffer(0),
			m_plate.GetBuffer(0),
			STime,
			ETime,
			directionstr.GetBuffer(0),
			searchFlag,
			SqlStr);
		break;
	default:
		MessageBox("CDLGHistroyDetect::OnButtonSearch B error",MESSAGEBOX_TITLE);
		return ;
	}

#endif
	ListNow=0;

	DisplayerList();
}

void CDLGHistroyDetect::DisplayerList(void)
{
	list_history_detect.clear();
	m_list.DeleteAllItems();
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");

	if(0==ListTotal)
		return ;

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+HISTORY_DETECT_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+HISTORY_DETECT_PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	//查询数据库
#if ALLTAB_DETECT_CAR_MODE
//汽车
	switch(flag)
	{
	case HISTORY_DETECT_FLAG_CAR :
		OracleIO.CAR_MatchResult_Read(SqlStr,searchFlag,si,ei,list_history_detect);
		break;
	case HISTORY_DETECT_FLAG_ALARM :
		OracleIO.CAR_AlarmResult_Read(SqlStr,searchFlag,si,ei,list_history_detect);
		break;
	default:
		MessageBox("CDLGHistroyDetect::DisplayerList A error",MESSAGEBOX_TITLE);
		return ;
	}

#else
//电动车
	switch(flag)
	{
	case HISTORY_DETECT_FLAG_CAR :
		OracleIO.ELECAR_MatchResult_Read(SqlStr,searchFlag,si,ei,list_history_detect);
		break;
	case HISTORY_DETECT_FLAG_ALARM :
		OracleIO.ELECAR_AlarmResult_Read(SqlStr,searchFlag,si,ei,list_history_detect);
		break;
	default:
		MessageBox("CDLGHistroyDetect::DisplayerList B error",MESSAGEBOX_TITLE);
		return ;
	}

#endif

	int nItem;
	char str[128];
	int i=0;

	list<struct HISTORY_DETECT_ST>::iterator beglist;

	for(beglist=list_history_detect.begin();beglist!=list_history_detect.end();beglist++)
	{
		sprintf(str,"%07d",ListNow+1+i);
		nItem = m_list.InsertItem(0,str);
		i++;

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
			beglist->year,
			beglist->mon,
			beglist->day,
			beglist->hour,
			beglist->min,
			beglist->sec);
		m_list.SetItemText(nItem,1,str);

		m_list.SetItemText(nItem,2,beglist->name);
		m_list.SetItemText(nItem,3,beglist->ip);
		m_list.SetItemText(nItem,4,beglist->plate);
	
		sprintf(str,"%d",beglist->reliability);
		m_list.SetItemText(nItem,5,str);

		m_list.SetItemText(nItem,6,beglist->direction);

#if ALLTAB_DETECT_CAR_MODE
//汽车
		m_list.SetItemText(nItem,7,beglist->platecolor);
		m_list.SetItemText(nItem,8,beglist->platetype);
		m_list.SetItemText(nItem,9,beglist->carcolor);
		m_list.SetItemText(nItem,10,beglist->path);

#else
//电动车
		m_list.SetItemText(nItem,7,beglist->path);

#endif

	}

	if(ListTotal%HISTORY_DETECT_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/HISTORY_DETECT_PAGE_MAX_NUM+1,ListTotal/HISTORY_DETECT_PAGE_MAX_NUM+1);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/HISTORY_DETECT_PAGE_MAX_NUM+1,ListTotal/HISTORY_DETECT_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
}

void CDLGHistroyDetect::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGHistroyDetect::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-HISTORY_DETECT_PAGE_MAX_NUM) >= 0)
		ListNow-=HISTORY_DETECT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistroyDetect::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+HISTORY_DETECT_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_DETECT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistroyDetect::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+HISTORY_DETECT_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_DETECT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistroyDetect::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/HISTORY_DETECT_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+HISTORY_DETECT_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+HISTORY_DETECT_PAGE_MAX_NUM) < (HISTORY_DETECT_PAGE_MAX_NUM*m_page))
			)
			ListNow+=HISTORY_DETECT_PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);

}

void CDLGHistroyDetect::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];

#if ALLTAB_DETECT_CAR_MODE
//汽车
	m_list.GetItemText(pNMIA->iItem,10,str,260);
#else
//电动车
	m_list.GetItemText(pNMIA->iItem,7,str,260);
#endif

	ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	*pResult = 0;
}
