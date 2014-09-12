// DLGSetRecord.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGSetRecord.h"
#include "DLGWarnning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

extern IO OracleIO;
/////////////////////////////////////////////////////////////////////////////
// CDLGSetRecord dialog


CDLGSetRecord::CDLGSetRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSetRecord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSetRecord)
	m_all = FALSE;
	m_date = FALSE;
	m_enable = FALSE;
	m_everyday = FALSE;
	m_week = FALSE;
	m_endweek = 0;
	m_startweek = 0;
	m_camip = _T("");
	camid=-1;
	//}}AFX_DATA_INIT

	AreaStr= _T("");
	CamStr=_T("");

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;
	m_EndMon=nowtime;
	m_EndHour=nowtime;

	rwLock=false;
	ListChoose=-1;
}


void CDLGSetRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetRecord)
	DDX_Control(pDX, IDC_COMBO_CAM, m_cam);
	DDX_Control(pDX, IDC_COMBO_AREA, m_area);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_ALL, m_all);
	DDX_Check(pDX, IDC_CHECK_DATE, m_date);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_enable);
	DDX_Check(pDX, IDC_CHECK_EVERYDAY, m_everyday);
	DDX_Check(pDX, IDC_CHECK_WEEK, m_week);
	DDX_CBIndex(pDX, IDC_COMBO_ENDWEEK, m_endweek);
	DDX_CBIndex(pDX, IDC_COMBO_STARTWEEK, m_startweek);
	DDX_Text(pDX, IDC_STATIC_CAMIP, m_camip);
	DDV_MaxChars(pDX, m_camip, 32);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_BUTTON_ENABLE, m_b_enable);
	DDX_Control(pDX, IDC_BUTTON_DISABLE, m_b_disable);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_b_add);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, m_b_edit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_b_delete);
	DDX_Control(pDX, IDC_BUTTON_DELETEALL, m_b_clear);
}


BEGIN_MESSAGE_MAP(CDLGSetRecord, CDialog)
	//{{AFX_MSG_MAP(CDLGSetRecord)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE, OnButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE, OnButtonDisable)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(HDN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL, OnButtonDeleteall)
	ON_CBN_CLOSEUP(IDC_COMBO_AREA, OnCloseupComboArea)
	ON_CBN_CLOSEUP(IDC_COMBO_CAM, OnCloseupComboCam)
	ON_BN_CLICKED(IDC_CHECK_DATE, OnCheckDate)
	ON_BN_CLICKED(IDC_CHECK_WEEK, OnCheckWeek)
	ON_BN_CLICKED(IDC_CHECK_EVERYDAY, OnCheckEveryday)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSetRecord message handlers
/////////////////
//警告：当删除设备时，要连同 定时录制的信息 一起删除
//定时设置 会和 用户名关联
////////////////
BOOL CDLGSetRecord::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 45);
	m_List.InsertColumn(1, _T("摄像头区域"), LVCFMT_LEFT, 95);
	m_List.InsertColumn(2, _T("摄像头名称"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(3, _T("摄像头IP"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(4, _T("起始日期"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(5, _T("起始时间"), LVCFMT_LEFT, 65);
	m_List.InsertColumn(6, _T("结束日期"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(7, _T("结束时间"), LVCFMT_LEFT, 65);
	m_List.InsertColumn(8, _T("状态"), LVCFMT_LEFT, 50);
	m_List.InsertColumn(9, _T("camid"), LVCFMT_LEFT, 0);
	m_List.InsertColumn(10, _T("nid"), LVCFMT_LEFT, 0);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	
	GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(FALSE);

	// TODO: Add extra initialization here
	//添加设备列表的区域
	for(int i=0;i<DlgMain->DlgDeviceTree.DlgAddDevice.AreaCount;i++)
	{
		if(DlgMain->DlgDeviceTree.DlgAddDevice.AreaList[i].name != NULL)
			m_area.AddString(DlgMain->DlgDeviceTree.DlgAddDevice.AreaList[i].name);
	}


	DisplayerList();

	GetDlgItem(IDC_STATIC_CAMIP)->GetWindowRect(printf_Rect);
	ScreenToClient(printf_Rect);


	///////////////////////////////////

	m_b_enable.LoadBitmaps(IDB_ENABLE_BUTTON,IDB_ENABLE_BUTTON_MOVE,NULL,NULL);
	m_b_enable.SizeToContent();		//自适应图片大小

	m_b_disable.LoadBitmaps(IDB_DISABLE_BUTTON,IDB_DISABLE_BUTTON_MOVE,NULL,NULL);
	m_b_disable.SizeToContent();		//自适应图片大小

	m_b_add.LoadBitmaps(IDB_ADD_BUTTON,IDB_ADD_BUTTON_MOVE,NULL,IDB_ADD_BUTTON_DIS);
	m_b_add.SizeToContent();		//自适应图片大小

	m_b_edit.LoadBitmaps(IDB_EDIT_BUTTON,IDB_EDIT_BUTTON_MOVE,NULL,IDB_EDIT_BUTTON_DIS);
	m_b_edit.SizeToContent();		//自适应图片大小

	m_b_delete.LoadBitmaps(IDB_DEL_BUTTON,IDB_DEL_BUTTON_MOVE,NULL,IDB_DEL_BUTTON_DIS);
	m_b_delete.SizeToContent();		//自适应图片大小

	m_b_clear.LoadBitmaps(IDB_CLEAR_BUTTON,IDB_CLEAR_BUTTON_MOVE,NULL,NULL);
	m_b_clear.SizeToContent();		//自适应图片大小


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGSetRecord::OnOK()
{
	// TODO: Add your control notification handler code here
}

//开程序时载入一次
void CDLGSetRecord::InitList(void)
{
	while(rwLock)
	{
		Sleep(100);
	}

	rwLock=true;

	list_record.clear();
	//查询数据库
	OracleIO.RECORD_PlanTable_ReadAll(list_record);

	rwLock=false;
}

void CDLGSetRecord::DisplayerList(void)
{
	while(rwLock)
	{
		Sleep(100);
	}

	rwLock=true;

	list_record.clear();
	m_List.DeleteAllItems();
		
	ListChoose=-1;
	Clear();
	//查询数据库
	OracleIO.RECORD_PlanTable_ReadAll(list_record);

	int nItem,total=0;

	list<struct SET_RECORD_DATA_ST>::iterator beglist;

	char str[128];

	for(beglist=list_record.begin();beglist!=list_record.end();beglist++)
	{
		sprintf(str,"%03d",total++);
		nItem = m_List.InsertItem(0,str);
	
		m_List.SetItemText(nItem,1,beglist->area);
		m_List.SetItemText(nItem,2,beglist->name);
		m_List.SetItemText(nItem,3,beglist->ip);
		
		DisplayTime(nItem,beglist);

		if(beglist->enable)
			m_List.SetItemText(nItem,8,"已开启");
		else
			m_List.SetItemText(nItem,8,"未开启");

		sprintf(str,"%d",beglist->camid);
		m_List.SetItemText(nItem,9,str);
		sprintf(str,"%d",beglist->nid);
		m_List.SetItemText(nItem,10,str);
	
	}

	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);

	rwLock=false;
}


void CDLGSetRecord::DisplayWeek(int nItem,int i,int dayofweek)
{
	switch(dayofweek)
	{
	case 0:	m_List.SetItemText(nItem,i,"每周日");break;
	case 1:	m_List.SetItemText(nItem,i,"每周一");break;
	case 2:	m_List.SetItemText(nItem,i,"每周二");break;
	case 3:	m_List.SetItemText(nItem,i,"每周三");break;
	case 4:	m_List.SetItemText(nItem,i,"每周四");break;
	case 5:	m_List.SetItemText(nItem,i,"每周五");break;
	case 6:	m_List.SetItemText(nItem,i,"每周六");break;
	default:break;
	}
}
void CDLGSetRecord::DisplayTime(int nItem,list<struct SET_RECORD_DATA_ST>::iterator beglist) 
{
	char str[260];
	CTime nowtime=CTime::GetTickCount();

	if(beglist->everyday)	//	每天
	{			
		m_List.SetItemText(nItem,4,"每天");
		sprintf(str,"%02d:%02d:%02d",
			beglist->StartHour,beglist->StartMin,beglist->StartSec);
		m_List.SetItemText(nItem,5,str);
		
		m_List.SetItemText(nItem,6,"每天");
		sprintf(str,"%02d:%02d:%02d",
			beglist->EndHour,beglist->EndMin,beglist->EndSec);
		m_List.SetItemText(nItem,7,str);
	}
	else if(beglist->useweek)	//周计划
	{
		DisplayWeek(nItem,4,beglist->StartWeek);
		sprintf(str,"%02d:%02d:%02d",
			beglist->StartHour,beglist->StartMin,beglist->StartSec);
		m_List.SetItemText(nItem,5,str);
		
		DisplayWeek(nItem,6,beglist->EndWeek);
		sprintf(str,"%02d:%02d:%02d",
			beglist->EndHour,beglist->EndMin,beglist->EndSec);
		m_List.SetItemText(nItem,7,str);

	}
	else if(beglist->usedate)	//日期计划
	{
		sprintf(str,"%04d年%02d月%02d日",
			beglist->StartYear,beglist->StartMon,beglist->StartDay);
		m_List.SetItemText(nItem,4,str);

		sprintf(str,"%02d:%02d:%02d",
			beglist->StartHour,beglist->StartMin,beglist->StartSec);
		m_List.SetItemText(nItem,5,str);
		
		sprintf(str,"%04d年%02d月%02d日",
			beglist->EndYear,beglist->EndMon,beglist->EndDay);
		m_List.SetItemText(nItem,6,str);

		sprintf(str,"%02d:%02d:%02d",
			beglist->EndHour,beglist->EndMin,beglist->EndSec);
		m_List.SetItemText(nItem,7,str);
	}
	else	//仅当日  代码同日期计划
	{	
		sprintf(str,"%04d年%02d月%02d日",
			beglist->StartYear,beglist->StartMon,beglist->StartDay);
		m_List.SetItemText(nItem,4,str);

		sprintf(str,"%02d:%02d:%02d",
			beglist->StartHour,beglist->StartMin,beglist->StartSec);
		m_List.SetItemText(nItem,5,str);
		
		sprintf(str,"%04d年%02d月%02d日",
			beglist->EndYear,beglist->EndMon,beglist->EndDay);
		m_List.SetItemText(nItem,6,str);

		sprintf(str,"%02d:%02d:%02d",
			beglist->EndHour,beglist->EndMin,beglist->EndSec);
		m_List.SetItemText(nItem,7,str);
	}
}


void CDLGSetRecord::Clear() 
{
	m_date = FALSE;
	m_enable = FALSE;
	m_everyday = FALSE;
	m_week = FALSE;
	m_endweek = 0;
	m_startweek = 0;
	m_camip = _T("");

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;
	m_EndMon=nowtime;
	m_EndHour=nowtime;

	m_area.SetCurSel(-1);
	m_cam.SetCurSel(-1);
	AreaStr= _T("");
	CamStr=_T("");
	m_all = FALSE;

	UpdateData(FALSE);

	GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(FALSE);
}

void CDLGSetRecord::Struct2M(void)
{
	camid =data.camid;
	int i;
	for(i=0;i<DlgMain->DlgDeviceTree.iptotal;i++)
	{
		if(	camid == DlgMain->DlgDeviceTree.iplist[i].camID)
		{
			AreaStr =DlgMain->DlgDeviceTree.iplist[i].area ;
			CamStr =DlgMain->DlgDeviceTree.iplist[i].name ;	
			m_camip =DlgMain->DlgDeviceTree.iplist[i].ip ;	
			break;
		}
	}
		
	//找区
	char str[260];
	for(i=0;i<m_area.GetCount();i++)
	{
		m_area.GetLBText(i,str);
		if(0==strcmp(AreaStr.GetBuffer(0),str))
		{
			m_area.SetCurSel(i);
			break;
		}
	}

	//清空
	m_cam.ResetContent();
	//重新添加
	for(i=0;i<DlgMain->DlgDeviceTree.iptotal;i++)
	{
		//==当前选择的区域名
		if(DlgMain->DlgDeviceTree.iplist[i].area == AreaStr )
			m_cam.AddString(DlgMain->DlgDeviceTree.iplist[i].name ); 
	}
	UpdateData(FALSE);
	InvalidateRect(printf_Rect, TRUE);

	//找摄像头
	for(i=0;i<m_cam.GetCount();i++)
	{
		m_cam.GetLBText(i,str);
		if(0==strcmp(CamStr.GetBuffer(0),str))
		{
			m_cam.SetCurSel(i);
			break;
		}
	}

	m_date =data.usedate;
	m_enable =data.enable;
	m_everyday = data.everyday;
	m_week =  data.useweek;
	m_endweek = data.EndWeek;
	m_startweek = data.StartWeek;

	CTime	StartT(data.StartYear,data.StartMon,data.StartDay,	\
					data.StartHour,data.StartMin,data.StartSec);
	m_StartMon=StartT;
	m_StartHour=StartT;

	CTime	EndT(data.EndYear,data.EndMon,data.EndDay,	\
					data.EndHour,data.EndMin,data.EndSec);
	m_EndMon=EndT;
	m_EndHour=EndT;

	if(m_week)
	{
		GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(FALSE);
	}

	if(m_date)
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
	}

}

void CDLGSetRecord::M2Struct(void)
{
	strcpy(data.area,AreaStr.GetBuffer(0));
	strcpy(data.name,CamStr.GetBuffer(0));
	data.camid	= camid ;
	strcpy(data.ip,m_camip.GetBuffer(0));
	
	data.usedate= m_date ;
	data.enable = m_enable ;
	data.everyday = m_everyday ;
	data.useweek =	m_week;
	data.EndWeek = m_endweek ;
	data.StartWeek = m_startweek;

	data.StartYear =m_StartMon.GetYear();
	data.StartMon=m_StartMon.GetMonth();
	data.StartDay=m_StartMon.GetDay();
	data.StartHour=m_StartHour.GetHour();
	data.StartMin=m_StartHour.GetMinute();
	data.StartSec=m_StartHour.GetSecond();

	data.EndYear=m_EndMon.GetYear();
	data.EndMon=m_EndMon.GetMonth();
	data.EndDay=m_EndMon.GetDay();
	data.EndHour=m_EndHour.GetHour();
	data.EndMin=m_EndHour.GetMinute();
	data.EndSec=m_EndHour.GetSecond();				
}

void CDLGSetRecord::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	for(int i=0;i<m_List.GetItemCount();i++)
		m_List.SetCheck(i,m_all);
}

unsigned long int  CDLGSetRecord::GetNid(long int ListChoose) 
{
	//获取NID
	if(ListChoose >=0)
	{
		char str[260];
		m_List.GetItemText(ListChoose,10,str,260);
		unsigned long int nid;
		sscanf(str,"%d",&nid);
		return nid;
	}
	return 0;
}

void CDLGSetRecord::OnButtonDeleteall() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//警告框
	CDLGWarnning dlgw;
	dlgw.m_wintxt="删除定时录制计划";					//窗口标题
	dlgw.m_warntxt="请再次确定是否删除";	//窗口内容
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{
		for(int i=0;i<m_List.GetItemCount();i++)
		{
			if(m_List.GetCheck(i))
			{
				//根据NID删除
				OracleIO.RECORD_PlanTable_DeleteWithNid(GetNid(i));
			}
		}
		DisplayerList();
	}
	else if (nResponse == IDCANCEL)
		return ;
	else
		return ;

}

void CDLGSetRecord::OnButtonEnable() 
{
	// TODO: Add your control notification handler code here
	
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetCheck(i))
		{
			///根据NID 修改启用状态
			OracleIO.RECORD_PlanTable_SetRecordState(GetNid(i),TRUE);
		}
	}
	DisplayerList();
}

void CDLGSetRecord::OnButtonDisable() 
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetCheck(i))
		{
			///根据NID 修改启用状态
			OracleIO.RECORD_PlanTable_SetRecordState(GetNid(i),FALSE);
		}
	}
	DisplayerList();
}

void CDLGSetRecord::OnButtonAdd() 
{
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	if(	-1 ==m_area.GetCurSel() ||
		-1 == m_cam.GetCurSel()	||
		m_camip.IsEmpty())
	{
		MessageBox("请选择一个摄像头",MESSAGEBOX_TITLE);
		return ;
	}

	//插入数据库
	M2Struct();
	
	if(TimeLimit())
	{
		MessageBox("起始时间必须小于结束时间",MESSAGEBOX_TITLE);
		return ;
	}

	if(!OracleIO.RECORD_PlanTable_AddNew(data))
	{
		MessageBox("数据库错误：无法增加",MESSAGEBOX_TITLE);
		return ;
	}

	DisplayerList();

}

void CDLGSetRecord::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(	-1 ==m_area.GetCurSel() ||
		-1 == m_cam.GetCurSel()	||
		m_camip.IsEmpty())
	{
		MessageBox("请选择一个摄像头",MESSAGEBOX_TITLE);
		return ;
	}

	//插入数据库

	M2Struct();

	if(TimeLimit())
	{
		MessageBox("起始时间必须小于结束时间",MESSAGEBOX_TITLE);
		return ;
	}
	
	if(!OracleIO.RECORD_PlanTable_Edit(data))
	{
		MessageBox("数据库错误：无法修改");
		return ;
	}
	
	DisplayerList();
}

bool CDLGSetRecord::TimeLimit(void) 
{
	CTime nowtime=CTime::GetTickCount();

	if(data.everyday)	//	每天
	{			
		CTime	StartT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
			data.StartHour,data.StartMin,data.StartSec);
		
		CTime	EndT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
			data.EndHour,data.EndMin,data.EndSec);
		
		if(StartT > EndT  )
			return true;
	}
	else if(data.useweek)	//周计划
	{
		CTime	StartT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
			data.StartHour,data.StartMin,data.StartSec);
		
		CTime	EndT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
			data.EndHour,data.EndMin,data.EndSec);
		
		if(data.StartWeek  > data.EndWeek 
			||	StartT > EndT)
			return true;
	}
	else if(data.usedate)	//日期计划
	{
		CTime	StartT(data.StartYear,data.StartMon,data.StartDay,	\
			data.StartHour,data.StartMin,data.StartSec);
		
		CTime	EndT(data.EndYear,data.EndMon,data.EndDay,	\
			data.EndHour,data.EndMin,data.EndSec);
		
		if(StartT > EndT  )
			return true;
	}
	else	//仅当日  代码同日期计划
	{	
		CTime	StartT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
			data.StartHour,data.StartMin,data.StartSec);
		
		CTime	EndT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
			data.EndHour,data.EndMin,data.EndSec);
		
		if(StartT > EndT  )
			return true;
	}
	
	return false;
}

void CDLGSetRecord::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//警告框
	CDLGWarnning dlgw;
	dlgw.m_wintxt="删除定时录制计划";					//窗口标题
	dlgw.m_warntxt="请再次确定是否删除";	//窗口内容
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{		
		//根据NID删除
		OracleIO.RECORD_PlanTable_DeleteWithNid(data.nid);
		DisplayerList();
	}
	else if (nResponse == IDCANCEL)
		return ;
	else
		return ;
}

void CDLGSetRecord::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;

	//选中的赋值
	ListChoose= p->iItem;
	//没中
	if(-1 == ListChoose)
		return ;

	//获取NID
	unsigned long int nid=GetNid(ListChoose);

	if(!OracleIO.RECORD_PlanTable_ReadOne(nid,&data))
	{
		MessageBox("无此用户名或数据库错误",MESSAGEBOX_TITLE);
		return ;
	}
	//拷贝
	Struct2M();

	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}

	//更新
	UpdateData(FALSE);

	*pResult = 0;
}

void CDLGSetRecord::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if((pNMListView->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
		&& (pNMListView->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ 
		) 
	{ //测试
	//	m_List.GetCheck(pNMListView->iItem);
	//	m_List.SetItemText(pNMListView->iItem,0,"1");

	} 
	else if((pNMListView->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
		&& (pNMListView->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
		) 
	{ //测试
	//	m_List.GetCheck(pNMListView->iItem);
	//	m_List.SetItemText(pNMListView->iItem,0,"2");

	} 
	
	*pResult = 0;
}

//设置摄像头名称
void CDLGSetRecord::OnCloseupComboArea() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int areai=m_area.GetCurSel();
	if(-1==areai)
	{
		//全删
		m_cam.ResetContent();
		return ;
	}
	//获取区域字符串
	
	m_area.GetLBText(areai,AreaStr);
	
	//全删
	m_cam.ResetContent();
	m_camip.Empty();
	//重新添加
	for(int i=0;i<DlgMain->DlgDeviceTree.iptotal;i++)
	{
		//==当前选择的区域名
		if(DlgMain->DlgDeviceTree.iplist[i].area == AreaStr )
			m_cam.AddString(DlgMain->DlgDeviceTree.iplist[i].name ); 
	}
	UpdateData(FALSE);
	InvalidateRect(printf_Rect, TRUE);
}

void CDLGSetRecord::OnCloseupComboCam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int cami=m_cam.GetCurSel();
	if(-1==cami)
	{
		//全删
		m_camip.Empty();
		return ;
	}
	//获取区域字符串
	
	m_cam.GetLBText(cami,CamStr);
	
	//全删
	m_camip.Empty();
	//重新添加
	for(int i=0;i<DlgMain->DlgDeviceTree.iptotal;i++)
	{
		//==当前选择的区域名
		if(DlgMain->DlgDeviceTree.iplist[i].area ==AreaStr &&
			DlgMain->DlgDeviceTree.iplist[i].name == CamStr)
		{
			m_camip=DlgMain->DlgDeviceTree.iplist[i].ip;
			camid=DlgMain->DlgDeviceTree.iplist[i].camID;
			break;
		}
	}
	
	UpdateData(FALSE);
	InvalidateRect(printf_Rect, TRUE);
}


void CDLGSetRecord::OnCheckDate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_date)
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(TRUE);

		m_week=FALSE;
		m_everyday=FALSE;
		GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	//三个都没选。则今天
	SetToday();

}

void CDLGSetRecord::OnCheckWeek() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_week)
	{
		GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(TRUE);

		m_date=FALSE;
		m_everyday=FALSE;
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	//三个都没选。则今天
	SetToday();
}

void CDLGSetRecord::OnCheckEveryday() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_everyday)
	{
		m_date=FALSE;
		m_week=FALSE;
		
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_STARTWEEK)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ENDWEEK)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	//三个都没选。则今天
	SetToday();
				
}
//当 3个都没打勾 设成今天
void CDLGSetRecord::SetToday() 
{
	if(FALSE == m_date 
		&& FALSE == m_week 
		&& FALSE == m_everyday )
	{
		CTime nowtime=CTime::GetTickCount();
		m_StartMon=nowtime;
		m_EndMon=nowtime;
		
		UpdateData(FALSE);
	}
}

//定时录制中查询是否需要进行录像
//返回true 则需要录像
//返回false 则不需要录像
bool CDLGSetRecord::NeedRecord(unsigned long int camid) 
{
	//锁住则等待 必须开另一个线程 查询此函数
	while(rwLock)
	{
		Sleep(100);
	}
	rwLock=true;

	CTime nowtime=CTime::GetTickCount();
	
	list<struct SET_RECORD_DATA_ST>::iterator beglist;
	
	for(beglist=list_record.begin();beglist!=list_record.end();beglist++)
	{
		if(camid == beglist->camid //摄像头ID匹配
			&& beglist->enable)	//启用
		{
			if(beglist->everyday)	//	每天
			{			
				CTime	StartT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
					beglist->StartHour,beglist->StartMin,beglist->StartSec);
				
				CTime	EndT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
					beglist->EndHour,beglist->EndMin,beglist->EndSec);
				
				if(nowtime >= StartT 
					&& nowtime <= EndT  )
				{
					rwLock=false;
					return true;
				}
			}
			else if(beglist->useweek)	//周计划
			{
				CTime	StartT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
					beglist->StartHour,beglist->StartMin,beglist->StartSec);
				
				CTime	EndT(nowtime.GetYear(),nowtime.GetMonth(),nowtime.GetDay(),
					beglist->EndHour,beglist->EndMin,beglist->EndSec);
				
				if(nowtime.GetDayOfWeek() >= (beglist->StartWeek+1) 
					&& nowtime.GetDayOfWeek() <= (beglist->EndWeek+1)  )
				{
					if(nowtime >= StartT 
						&& nowtime <= EndT  )
					{
						rwLock=false;
						return true;
					}
				}
			}
			else if(beglist->usedate)	//日期计划
			{
				CTime	StartT(beglist->StartYear,beglist->StartMon,beglist->StartDay,	\
					beglist->StartHour,beglist->StartMin,beglist->StartSec);
				
				CTime	EndT(beglist->EndYear,beglist->EndMon,beglist->EndDay,	\
					beglist->EndHour,beglist->EndMin,beglist->EndSec);
				
				if(nowtime >= StartT 
					&& nowtime <= EndT  )
				{
					rwLock=false;
					return true;
				}
			}
			else	//仅当日  代码同日期计划
			{
				CTime	StartT(beglist->StartYear,beglist->StartMon,beglist->StartDay,	\
					beglist->StartHour,beglist->StartMin,beglist->StartSec);
				
				CTime	EndT(beglist->EndYear,beglist->EndMon,beglist->EndDay,	\
					beglist->EndHour,beglist->EndMin,beglist->EndSec);
				
				if(nowtime >= StartT 
					&& nowtime <= EndT  )
				{
					rwLock=false;
					return true;
				}
			}
		}
	}

	rwLock=false;
	return false;
}

void CDLGSetRecord::OnPaint()
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
HBRUSH CDLGSetRecord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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