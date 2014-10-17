// DLGdetectServer.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGdetectServer.h"


#include "IO.h"
extern IO OracleIO;

#define DETECTLIST_TIMER 123
// CDLGdetectServer dialog

IMPLEMENT_DYNAMIC(CDLGdetectServer, CDialog)

CDLGdetectServer::CDLGdetectServer(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGdetectServer::IDD, pParent)
{

}

CDLGdetectServer::~CDLGdetectServer()
{
}

void CDLGdetectServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDLGdetectServer, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGdetectServer::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGdetectServer::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FLUSH, &CDLGdetectServer::OnBnClickedButtonFlush)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CDLGdetectServer::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// CDLGdetectServer message handlers

void CDLGdetectServer::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();


}

void CDLGdetectServer::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDLGdetectServer::OnBnClickedButtonFlush()
{
	// TODO: Add your control notification handler code here
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
}

void CDLGdetectServer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);


	if(nIDEvent == DETECTLIST_TIMER)
	{
		OnBnClickedButtonFlush();
	}
}
