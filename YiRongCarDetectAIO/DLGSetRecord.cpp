// DLGSetRecord.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGSetRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

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
	m_endweek = -1;
	m_startweek = -1;
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
	//}}AFX_DATA_MAP
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
	//}}AFX_MSG_MAP
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
	
	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 50);
	m_List.InsertColumn(4, _T("起始时间"), LVCFMT_LEFT, 90);
	m_List.InsertColumn(5, _T("结束时间"), LVCFMT_LEFT, 90);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	// TODO: Add extra initialization here
	//添加设备列表的区域
	for(int i=0;i<DlgMain->DlgDeviceTree.DlgAddDevice.AreaCount;i++)
	{
		if(DlgMain->DlgDeviceTree.DlgAddDevice.AreaList[i].name != NULL)
			m_area.AddString(DlgMain->DlgDeviceTree.DlgAddDevice.AreaList[i].name);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGSetRecord::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CDLGSetRecord::DisplayerList(void)
{
	list_record.clear();
	//查询数据库

	int nItem;

	list<struct SET_RECORD_DATA_ST>::iterator beglist;

	char str[128];

	m_List.DeleteAllItems();

	for(beglist=list_record.begin();beglist!=list_record.end();beglist++)
	{
		/*
		sprintf(str,"%07d",beglist->nid);
		nItem = m_List.InsertItem(0,str);
		
		m_List.SetItemText(nItem,1,beglist->user);
		*/
	}

	ListChoose=-1;
	Clear();
}

void CDLGSetRecord::Clear() 
{
	/*
	m_password_again = _T("");
	m_password = _T("");
	m_user = _T("");
	m_systemset = FALSE;
	m_recordset = FALSE;
	m_record = FALSE;
	m_ptz = FALSE;
	m_preview = FALSE;
	m_historyvideo = FALSE;
	m_historyreport = FALSE;
	m_historydetect = FALSE;
	m_historyalarm = FALSE;
	m_device = FALSE;
	m_blackset = FALSE;
	m_capbmp = FALSE;
	m_detect = FALSE;
	m_detectset = FALSE;
	m_all = FALSE;
	*/
	UpdateData(FALSE);
}


void CDLGSetRecord::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGSetRecord::OnButtonDeleteall() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGSetRecord::OnButtonEnable() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGSetRecord::OnButtonDisable() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGSetRecord::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGSetRecord::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGSetRecord::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGSetRecord::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
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
	CString str;
	m_area.GetLBText(areai,str);
	
	//全删
	m_cam.ResetContent();
	//重新添加
	for(int i=0;i<DlgMain->DlgDeviceTree.iptotal;i++)
	{
		//==当前选择的区域名
		if(DlgMain->DlgDeviceTree.iplist[i].area ==str.GetBuffer(0) )
			m_cam.AddString(DlgMain->DlgDeviceTree.iplist[i].name ); 
	}

}
