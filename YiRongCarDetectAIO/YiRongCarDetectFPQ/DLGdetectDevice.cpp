// DLGdetectDevice.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectFPQ.h"
#include "DLGdetectDevice.h"

#include "IO.h"
extern IO OracleIO;
// CDLGdetectDevice dialog

#define HEART_MAX_TIME 20

#define HEARTTEST_TIMER 122
#define DETECTLIST_TIMER 123


IMPLEMENT_DYNAMIC(CDLGdetectDevice, CDialog)

CDLGdetectDevice::CDLGdetectDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGdetectDevice::IDD, pParent)
	, m_ip(_T(""))
	, m_channel(0)
{
	chooseDetectDeviceid=0;
	chooseCamid=0;
}

CDLGdetectDevice::~CDLGdetectDevice()
{
}

void CDLGdetectDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDX_Text(pDX, IDC_EDIT_CHANNEL, m_channel);
	DDX_Control(pDX, IDC_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CDLGdetectDevice, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGdetectDevice::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGdetectDevice::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FLUSH, &CDLGdetectDevice::OnBnClickedButtonFlush)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDLGdetectDevice::OnBnClickedButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDLGdetectDevice::OnBnClickedButtonAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CDLGdetectDevice::OnNMClickList)
END_MESSAGE_MAP()

BOOL CDLGdetectDevice::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.InsertColumn(0, _T("识别ID") , LVCFMT_LEFT, 50);
	m_list.InsertColumn(1, _T("识别IP" ), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("识别通道"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(3, _T("识别故障"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(4, _T("识别占用"), LVCFMT_LEFT, 60);

	m_list.InsertColumn(5, _T("摄像ID") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(6, _T("摄像名称" ), LVCFMT_LEFT, 100);
	m_list.InsertColumn(7, _T("摄像IP"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(8, _T("摄像URL"), LVCFMT_LEFT, 20);
	m_list.InsertColumn(9, _T("是否识别"), LVCFMT_LEFT, 60);

	m_list.InsertColumn(10, _T("任务ID") , LVCFMT_LEFT, 60);
	m_list.InsertColumn(11, _T("下发用户" ), LVCFMT_LEFT, 60);
	m_list.InsertColumn(12, _T("用户名" ), LVCFMT_LEFT, 60);
	m_list.InsertColumn(13, _T("下发时间"), LVCFMT_LEFT, 130);
	m_list.InsertColumn(14, _T("下发指令"), LVCFMT_LEFT, 60);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//初始化不能加数据库

	m_DetectListTimer = SetTimer(DETECTLIST_TIMER,5000,NULL);
	m_HeartTestTimer = SetTimer(HEARTTEST_TIMER,HEART_MAX_TIME*1000,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// CDLGdetectDevice message handlers

void CDLGdetectDevice::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CDLGdetectDevice::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CDLGdetectDevice::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//需要变化在这添加
	//列表
	CRect list_Rect;
	list_Rect.top = rc.top;
	list_Rect.bottom = rc.bottom-150;
	list_Rect.left = rc.left ;
	list_Rect.right = rc.right;

	//必须 样式=重叠，边框=调整大小
	m_list.MoveWindow(list_Rect);

	Invalidate();
}

void CDLGdetectDevice::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	OracleIO.DETECTSERVER_ADD(m_ip.GetBuffer(0), m_channel);
	OnBnClickedButtonFlush();
}


void CDLGdetectDevice::OnBnClickedButtonFlush()
{
	// TODO: Add your control notification handler code here

	chooseDetectDeviceid=0;
	chooseCamid=0;

	list<DEVICE_LIST> DeviceList;

	m_list.DeleteAllItems();

	OracleIO.DEVICE_ReadList(DeviceList);

	int nItem;
	char str[128];

	if(DeviceList.size()<=0)
		return ;

	list<DEVICE_LIST>::iterator beglist;

	for(beglist=DeviceList.begin();beglist!=DeviceList.end();beglist++)
	{
		sprintf(str,"%d",beglist->detect.id);
		nItem = m_list.InsertItem(0,str);

		m_list.SetItemText(nItem,1,beglist->detect.ip);

		sprintf(str,"%d",beglist->detect.channel);
		m_list.SetItemText(nItem,2,str);

		if(beglist->detect.isenable)
			m_list.SetItemText(nItem,3,"正常");
		else
			m_list.SetItemText(nItem,3,"故障--");

		if(beglist->detect.isoccupy)
			m_list.SetItemText(nItem,4,"占用");
		else
			m_list.SetItemText(nItem,4,"未占用");

		sprintf(str,"%d",beglist->camera.camid);
		m_list.SetItemText(nItem,5,str);

		m_list.SetItemText(nItem,6,beglist->camera.name);

		m_list.SetItemText(nItem,7,beglist->camera.ip);

		m_list.SetItemText(nItem,8,beglist->camera.rtspurl);

		if(beglist->camera.isplay)
			m_list.SetItemText(nItem,9,"识别");
		else
			m_list.SetItemText(nItem,9,"未识别");

		sprintf(str,"%d",beglist->mission.missionid);
		m_list.SetItemText(nItem,10,str);

		sprintf(str,"%d",beglist->mission.userid);
		m_list.SetItemText(nItem,11,str);

		m_list.SetItemText(nItem,12,beglist->mission.username);

		m_list.SetItemText(nItem,13,beglist->mission.time);

		if(beglist->mission.isplay)
			m_list.SetItemText(nItem,14,"开启识别");
		else
			m_list.SetItemText(nItem,14,"关闭识别");
	}
	UpdateData(FALSE);
}

void CDLGdetectDevice::OnBnClickedButtonStop()
{
	UpdateData(TRUE);
	long tDetectDeviceid=chooseDetectDeviceid;
	long tCamid=chooseCamid;
	// TODO: Add your control notification handler code here
	if(	tDetectDeviceid>0 && tCamid>0)
	{
		OracleIO.MISSION_AddStop(tDetectDeviceid,tCamid);
		MessageBox("已发送停止");
	}

}

void CDLGdetectDevice::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
	UpdateData(TRUE);


	if(nIDEvent == DETECTLIST_TIMER)
	{
		OnBnClickedButtonFlush();
	}
	//心跳测试
	if(nIDEvent == HEARTTEST_TIMER)
	{
		OracleIO.DETECTSERVER_HeartTest(HEART_MAX_TIME);
	}
}


void CDLGdetectDevice::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;

	//选中的赋值
	long ListChoose= p->iItem;
	//没中
	if(-1 == ListChoose)
		return ;

	char str[260];
	m_list.GetItemText(ListChoose,0,str,260);

	if(strlen(str)>0)
		sscanf(str,"%d",&chooseDetectDeviceid);
	else
		chooseDetectDeviceid=0;

	m_list.GetItemText(ListChoose,5,str,260);

	if(strlen(str)>0)
		sscanf(str,"%d",&chooseCamid);
	else
		chooseCamid=0;

	UpdateData(FALSE);
	*pResult = 0;

}
