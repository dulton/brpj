// DLGTabCar.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGTabCar.h"
#include "SmartVillageDlg.h"
#include "DLGWarnning.h"

////////////////lynn/////////////////
extern CYRSVMySQL MySqlIO;
extern CSmartVillageDlg *DlgMain;
////////////////lynn/////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

#include "DLGpictureView.h"

// CDLGTabCar dialog

IMPLEMENT_DYNAMIC(CDLGTabCar, CDialog)

CDLGTabCar::CDLGTabCar(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGTabCar::IDD, pParent)

{
	m_ip = _T("");
	m_name = _T("");
	m_page = 1;
	m_plate = _T("");
	m_platetype = 0;
	m_platecolor = 0;
	m_direction = 0;
	m_carcolor = 0;
	m_lib=TAB_CAR_FLAG_CAR;
	m_CheckTime=FALSE;
	//}}AFX_DATA_INIT


	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime.GetTime();
	m_StartHour=nowtime.GetTime();

	CTime cstime(
		nowtime.GetYear(),
		nowtime.GetMonth(),
		nowtime.GetDay(),
		0,	0,	0);

	m_StartHour=cstime.GetTime();

	nowtime+=3600;
	m_EndMon=nowtime.GetTime();
	m_EndHour=nowtime.GetTime();

	ListTotal=0;
	ListNow=0;

	PicdisplayFlag=false;
}

CDLGTabCar::~CDLGTabCar()
{
}

void CDLGTabCar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
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
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_search);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_first_button);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_pre_button);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_next_button);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_last_button);
	DDX_Control(pDX, IDC_BUTTON_JUMP, m_jump_button);
	DDX_CBIndex(pDX, IDC_COMBO_LIB, m_lib);
}


BEGIN_MESSAGE_MAP(CDLGTabCar, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGTabCar::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGTabCar::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST,OnLvnItemActivateList)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CDLGTabCar::OnNMClickList)
END_MESSAGE_MAP()

BOOL CDLGTabCar::OnInitDialog()
{
	CDialog::OnInitDialog();

#if ALLTAB_DETECT_CAR_MODE
	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 60);
	m_List.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_List.InsertColumn(2, _T("摄像头名称" ), LVCFMT_LEFT, 180);
	m_List.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_List.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(7, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(8, _T("车牌类型"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(9, _T("车身颜色"), LVCFMT_LEFT, 60);
	m_List.InsertColumn(10, _T("图片路径"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(11, _T("nid"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(12, _T("小图路径"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(13, _T("失主姓名"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(14, _T("品牌"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(15, _T("失主电话"), LVCFMT_LEFT, 100);

	GetDlgItem(IDC_COMBO_PLATETYPE)->ShowWindow(TRUE);
	GetDlgItem(IDC_COMBO_PLATECOLOR)->ShowWindow(TRUE);
	GetDlgItem(IDC_COMBO_CARCOLOR)->ShowWindow(TRUE);

#else
	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 60);
	m_List.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_List.InsertColumn(2, _T("摄像头名称" ), LVCFMT_LEFT, 190);
	m_List.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_List.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(7, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(8, _T("图片路径"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(9, _T("nid"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(10, _T("小图路径"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(11, _T("失主姓名"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(12, _T("品牌"), LVCFMT_LEFT, 70);
	m_List.InsertColumn(13, _T("失主电话"), LVCFMT_LEFT, 100);
	//屏蔽车牌类型 车牌颜色 车身颜色
	GetDlgItem(IDC_COMBO_PLATETYPE)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_PLATECOLOR)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_CARCOLOR)->ShowWindow(FALSE);
#endif

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CHeaderCtrl* pHeaderCtrl = (CHeaderCtrl*)m_List.GetHeaderCtrl();
	pHeaderCtrl->EnableWindow(FALSE);

	OnCheckTime();

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


	DlgTabPic.Create(IDD_TAB_PIC,this);
	DlgTabPic.ShowWindow(SW_SHOW);
	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGTabCar::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);
	
	if(rc.Width()<1280 )
	{
		DlgTabPic.ShowWindow(SW_HIDE);
		PicdisplayFlag=false;
	}
	else
	{
		DlgTabPic.ShowWindow(SW_SHOW);
		PicdisplayFlag=true;
	}
	
	int buttonh=25;
	int buttonbmpw=24,buttonbmph=18,buttongap=10;
	int toph=100;


	rc.top+=5;
	rc.bottom-=5;
	rc.left+=5;
	rc.right-=5;

	//列表
	CRect list_Rect;
	list_Rect.top = rc.top+toph ;
	list_Rect.bottom = rc.bottom-buttonh;

	if(PicdisplayFlag)
	{
		list_Rect.left = rc.left;
		list_Rect.right = 1000;
	}
	else
	{
		list_Rect.left = rc.left;
		list_Rect.right = rc.right;
	}
	//必须 样式=重叠，边框=调整大小
	m_List.MoveWindow(list_Rect);


	if(PicdisplayFlag)
	{
		CRect pic_Rect;
		pic_Rect.top = rc.top+10 ;
		pic_Rect.bottom = rc.bottom;
		pic_Rect.left = list_Rect.right+10;
		pic_Rect.right = rc.right;

		DlgTabPic.MoveWindow(pic_Rect);
		DlgTabPic.AutoSize();

	}


	CRect b_Rect;

	b_Rect.top = list_Rect.bottom+5 ;
	b_Rect.bottom =	b_Rect.top+buttonbmph;

	b_Rect.left = rc.left;
	b_Rect.right = rc.left+500;
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





	Invalidate();
}

// CDLGTabCar message handlers

void CDLGTabCar::OnOk()
{
	// TODO: Add your control notification handler code here
	//	CDialog::OnOK();
}

void CDLGTabCar::OnCancel()
{
	// TODO: Add your control notification handler code here
	DlgMain->OnCancel();
}

void CDLGTabCar::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);


	memset(SqlStr,0,1024);
	char STime[32]={0};
	char ETime[32]={0};

	if(	m_StartMon.GetYear() <1900  || m_StartMon.GetYear() >2100 ||
		m_EndMon.GetYear()<1900  || m_EndMon.GetYear() >2100) 
	{
		MessageBox("时间范围不能小于1900年，不大于2100年",MESSAGEBOX_TITLE);
		return ;
	}

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

		COleDateTime cstime(
			m_StartMon.GetYear(),
			m_StartMon.GetMonth(),
			m_StartMon.GetDay(),
			m_StartHour.GetHour(),
			m_StartHour.GetMinute(),
			m_StartHour.GetSecond());
		
		COleDateTime cetime(
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

	switch(m_lib)
	{
	case TAB_CAR_FLAG_CAR :
		ListTotal=MySqlIO.CAR_MatchResult_GetNum(
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
	case  TAB_CAR_FLAG_CARALARM:
		ListTotal=MySqlIO.CAR_AlarmResult_GetNum(
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
		MessageBox("CDLGTabCar::OnButtonSearch A error",MESSAGEBOX_TITLE);
		return ;
	}

#else
//电动车
	ListTotal=0;

	switch(m_lib)
	{
	case TAB_CAR_FLAG_CAR :
		ListTotal=MySqlIO.ELECAR_MatchResult_GetNum(		
			m_name.GetBuffer(0),
			m_ip.GetBuffer(0),
			m_plate.GetBuffer(0),
			STime,
			ETime,
			directionstr.GetBuffer(0),
			searchFlag,
			SqlStr);
		break;
	case  TAB_CAR_FLAG_CARALARM:
		ListTotal=MySqlIO.ELECAR_AlarmResult_GetNum(
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
		MessageBox("CDLGTabCar::OnButtonSearch B error",MESSAGEBOX_TITLE);
		return ;
	}

#endif
	ListNow=0;

	DisplayerList();
}

void CDLGTabCar::DisplayerList(void)
{
	list_history_CarDetect.clear();
	m_List.DeleteAllItems();
	

	if(0==ListTotal)
	{
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
		InvalidateRect(printf_Rect, TRUE);
		return ;
	}

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+TAB_CAR_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+TAB_CAR_PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	//查询数据库
#if ALLTAB_DETECT_CAR_MODE
//汽车
	switch(m_lib)
	{
	case TAB_CAR_FLAG_CAR :
		MySqlIO.CAR_MatchResult_Read(SqlStr,searchFlag,si,ei,list_history_CarDetect);
		break;
	case  TAB_CAR_FLAG_CARALARM:
		MySqlIO.CAR_AlarmResult_Read(SqlStr,searchFlag,si,ei,list_history_CarDetect);
		break;
	default:
		MessageBox("CDLGTabCar::DisplayerList A error",MESSAGEBOX_TITLE);
		return ;
	}

#else
//电动车
	switch(m_lib)
	{
	case TAB_CAR_FLAG_CAR :
		MySqlIO.ELECAR_MatchResult_Read(SqlStr,searchFlag,si,ei,list_history_CarDetect);
		break;
	case  TAB_CAR_FLAG_CARALARM:
		MySqlIO.ELECAR_AlarmResult_Read(SqlStr,searchFlag,si,ei,list_history_CarDetect);
		break;
	default:
		MessageBox("CDLGTabCar::DisplayerList B error",MESSAGEBOX_TITLE);
		return ;
	}

#endif

	int nItem;
	char str[128]={0};
	int i=0;

	list<struct HISTORY_CarDetect_ST>::iterator beglist;
	
	struct BLACK_DATA_ST blackdata;

	for(beglist=list_history_CarDetect.begin();beglist!=list_history_CarDetect.end();beglist++)
	{
		sprintf(str,"%07d",ListNow+1+i);
		nItem = m_List.InsertItem(0,str);
		i++;

		sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
			beglist->year,
			beglist->mon,
			beglist->day,
			beglist->hour,
			beglist->min,
			beglist->sec);
		m_List.SetItemText(nItem,1,str);

		m_List.SetItemText(nItem,2,beglist->name);
		m_List.SetItemText(nItem,3,beglist->ip);
		m_List.SetItemText(nItem,4,beglist->plate);
	
		sprintf(str,"%d",beglist->reliability);
		m_List.SetItemText(nItem,5,str);

		m_List.SetItemText(nItem,6,beglist->direction);

#if ALLTAB_DETECT_CAR_MODE
//汽车
		m_List.SetItemText(nItem,7,beglist->platecolor);
		m_List.SetItemText(nItem,8,beglist->platetype);
		m_List.SetItemText(nItem,9,beglist->carcolor);
		m_List.SetItemText(nItem,10,beglist->path);
	
		sprintf(str,"%d",beglist->nid);
		m_List.SetItemText(nItem,11,str);

		m_List.SetItemText(nItem,12,beglist->smallpath);

	if(  TAB_CAR_FLAG_CARALARM ==m_lib)
	{
		if(MySqlIO.CAR_BlackTable_ReadOneWithNid(beglist->blackid,blackdata))
		{
				m_List.SetItemText(nItem,13,blackdata.name);
				m_List.SetItemText(nItem,14,blackdata.brand);
				m_List.SetItemText(nItem,15,blackdata.Phone);
		}
	}
#else
//电动车
		m_List.SetItemText(nItem,7,beglist->platecolor);
		m_List.SetItemText(nItem,8,beglist->path);
	
		sprintf(str,"%d",beglist->nid);
		m_List.SetItemText(nItem,9,str);
		
		m_List.SetItemText(nItem,10,beglist->smallpath);

		if( TAB_CAR_FLAG_CARALARM ==m_lib)
		{
			if(MySqlIO.ELECAR_BlackTable_ReadOneWithNid(beglist->blackid,blackdata))
			{
				m_List.SetItemText(nItem,11,blackdata.name);
				m_List.SetItemText(nItem,12,blackdata.brand);
				m_List.SetItemText(nItem,13,blackdata.Phone);
			}
		}
#endif

	}

	if(ListTotal%TAB_CAR_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/TAB_CAR_PAGE_MAX_NUM+1,ListTotal/TAB_CAR_PAGE_MAX_NUM+1,TAB_CAR_PAGE_MAX_NUM);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/TAB_CAR_PAGE_MAX_NUM+1,ListTotal/TAB_CAR_PAGE_MAX_NUM,TAB_CAR_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	InvalidateRect(printf_Rect, TRUE);
}

void CDLGTabCar::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGTabCar::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-TAB_CAR_PAGE_MAX_NUM) >= 0)
		ListNow-=TAB_CAR_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGTabCar::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+TAB_CAR_PAGE_MAX_NUM) < ListTotal)
		ListNow+=TAB_CAR_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGTabCar::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+TAB_CAR_PAGE_MAX_NUM) < ListTotal)
		ListNow+=TAB_CAR_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGTabCar::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/TAB_CAR_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+TAB_CAR_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+TAB_CAR_PAGE_MAX_NUM) < (TAB_CAR_PAGE_MAX_NUM*m_page))
			)
			ListNow+=TAB_CAR_PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);

}

void CDLGTabCar::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[ZOG_MAX_PATH_STR]={0};

	//本地
#if ALLTAB_DETECT_CAR_MODE
	//汽车
	m_List.GetItemText(pNMIA->iItem,11,str,260);
#else
	//电动车
	m_List.GetItemText(pNMIA->iItem,9,str,260);
#endif

	struct BLACK_DATA_ST blackdata;

	CDLGpictureView dlgPicView;
	dlgPicView.m_txt="";

	long nid=atoi(str);

	list<struct HISTORY_CarDetect_ST>::iterator beglist;

	for(beglist=list_history_CarDetect.begin();beglist!=list_history_CarDetect.end();beglist++)
	{
		if(beglist->nid == nid)
		{
			dlgPicView.m_txt="时间:";

			sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",		
				beglist->year,
				beglist->mon,
				beglist->day,
				beglist->hour,
				beglist->min,
				beglist->sec);

			dlgPicView.m_txt+=str;
			dlgPicView.m_txt+="\n设备名称:";
			dlgPicView.m_txt+=beglist->name;
			dlgPicView.m_txt+="\n设备IP:";
			dlgPicView.m_txt+=beglist->ip;

			dlgPicView.m_txt+="\n车牌号:";
			dlgPicView.m_txt+=beglist->plate;

			dlgPicView.m_txt+="\n方向:";
			dlgPicView.m_txt+=beglist->direction;

			dlgPicView.m_txt+="\n车牌颜色:";
			dlgPicView.m_txt+=beglist->platecolor;

#if ALLTAB_DETECT_CAR_MODE

			dlgPicView.m_txt+="\n车牌类型:";
			dlgPicView.m_txt+=beglist->platetype;

			dlgPicView.m_txt+="\n车辆颜色:";
			dlgPicView.m_txt+=beglist->carcolor;
#endif
			if(  TAB_CAR_FLAG_CARALARM ==m_lib)
			{
				if(MySqlIO.CAR_BlackTable_ReadOneWithNid(beglist->blackid,blackdata))
				{
					dlgPicView.m_txt+="\n失主姓名:";
					dlgPicView.m_txt+=blackdata.name;
					dlgPicView.m_txt+="\n品牌:";
					dlgPicView.m_txt+=blackdata.brand;
					dlgPicView.m_txt+="\n车牌号:";
					dlgPicView.m_txt+=blackdata.plate;
					dlgPicView.m_txt+="\n失主电话:";
					dlgPicView.m_txt+=blackdata.Phone;
					dlgPicView.m_txt+="\n其他信息:";
					dlgPicView.m_txt+=blackdata.other;
				}
			}
			//标题
			dlgPicView.Titlestr=beglist->name;

			char *p=strrchr(beglist->path,'\\');
			if(p!=NULL)
				p++;
			else
				return ;

			dlgPicView.Titlestr+=p;
			//路径
			dlgPicView.srcfile=beglist->path;
			dlgPicView.srcsmallfile=beglist->smallpath;
			break;
		}
	}

	dlgPicView.DoModal();

	*pResult = 0;
}


void CDLGTabCar::OnCheckTime() 
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

void CDLGTabCar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
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
HBRUSH CDLGTabCar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

//单击LIST
void CDLGTabCar::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	NM_LISTVIEW* pNMIA = (NM_LISTVIEW*)pNMHDR;    

	if(pNMIA->iItem != -1)      
	{             
			if(PicdisplayFlag)
			{
	char str[ZOG_MAX_PATH_STR]={0};
	char smallstr[ZOG_MAX_PATH_STR]={0};
				//本地
#if ALLTAB_DETECT_CAR_MODE
				//汽车
				m_List.GetItemText(pNMIA->iItem,10,str,ZOG_MAX_PATH_STR);
				m_List.GetItemText(pNMIA->iItem,12,smallstr,ZOG_MAX_PATH_STR);
				
#else
				//电动车
				m_List.GetItemText(pNMIA->iItem,8,str,ZOG_MAX_PATH_STR);
				m_List.GetItemText(pNMIA->iItem,10,smallstr,ZOG_MAX_PATH_STR);
#endif

				char *p=strrchr(str,'\\');
				if(p!=NULL)
					p++;
				else
				{
					DlgTabPic.clear();
					DlgTabPic.Invalidate();
					return ;
				}

				DlgTabPic.srcfile=str;
				DlgTabPic.srcsmallfile=smallstr;
				DlgTabPic.Load();
				DlgTabPic.Invalidate();
			}

	}         

	*pResult = 0;
}
