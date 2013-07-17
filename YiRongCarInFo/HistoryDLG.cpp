// HistoryDLG.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarInFo.h"

#include "YiRongCarInFoDlg.h"
#include "IO.h"
#include "HistoryDLG.h"
#include "sms.h"

#define HL_PAGE_MAX_NUM 40

extern CYiRongCarInFoDlg *YiRongCardlg;
extern SMS smsclass;
// CHistoryDLG dialog

IMPLEMENT_DYNAMIC(CHistoryDLG, CDialog)

CHistoryDLG::CHistoryDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryDLG::IDD, pParent)
	, m_carnumber(_T(""))
	, m_TimeStartMon(0)
	, m_TimeStartHour(0)
	, m_TimeEndMon(0)
	, m_TimeEndHour(0)
	, m_smsphone(_T(""))
{
	CTime nowtime=CTime::GetTickCount();
	m_TimeStartMon=nowtime;
	m_TimeEndMon=nowtime;
	m_TimeStartHour=nowtime;
	m_TimeEndHour=nowtime;
}

CHistoryDLG::~CHistoryDLG()
{
}

void CHistoryDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_CARNUMBER, m_carnumber);
	DDV_MaxChars(pDX, m_carnumber, 5);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START, m_TimeStartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START2, m_TimeStartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END, m_TimeEndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END2, m_TimeEndHour);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_smsphone);
	DDV_MaxChars(pDX, m_smsphone, 11);
}


BEGIN_MESSAGE_MAP(CHistoryDLG, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_END, &CHistoryDLG::OnBnClickedButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CHistoryDLG::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CHistoryDLG::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CHistoryDLG::OnBnClickedButtonFirst)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST, &CHistoryDLG::OnLvnItemActivateList)
	ON_BN_CLICKED(IDC_BUTTON_CARNUMBER, &CHistoryDLG::OnBnClickedButtonCarnumber)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &CHistoryDLG::OnBnClickedButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_SMS, &CHistoryDLG::OnBnClickedButtonSms)
END_MESSAGE_MAP()


// CHistoryDLG message handlers
BOOL CHistoryDLG::OnInitDialog()
{
	CDialog::OnInitDialog();

	//1为全部 0为报警
	if(1 == Flag)
	{
		GetDlgItem(IDC_STATIC_LISTCHOICE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PHONE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PHONE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SMS)->ShowWindow(SW_HIDE);

		m_List.InsertColumn(0, "序号" , LVCFMT_LEFT, 60);
		m_List.InsertColumn(1, _T("时间") , LVCFMT_LEFT, 130);
		m_List.InsertColumn(2, _T("摄像头ip"), LVCFMT_LEFT, 130);
		m_List.InsertColumn(3, _T("车牌号"), LVCFMT_LEFT, 60);
		m_List.InsertColumn(4, _T("图片路径"), LVCFMT_LEFT, 300);
	}
	else
	{
		GetDlgItem(IDC_STATIC_LISTCHOICE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PHONE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PHONE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SMS)->ShowWindow(SW_SHOW);

		m_List.InsertColumn(0, "序号" , LVCFMT_LEFT, 60);
		m_List.InsertColumn(1, _T("时间") , LVCFMT_LEFT, 130);
		m_List.InsertColumn(2, _T("摄像头ip"), LVCFMT_LEFT, 130);
		m_List.InsertColumn(3, _T("车牌号"), LVCFMT_LEFT, 60);
		m_List.InsertColumn(4, _T("图片路径"), LVCFMT_LEFT, 5);
		m_List.InsertColumn(5, _T("被盗品牌") , LVCFMT_LEFT, 70);
		m_List.InsertColumn(6, _T("被盗车牌号"), LVCFMT_LEFT,100);
		m_List.InsertColumn(7, _T("被盗车架号"), LVCFMT_LEFT, 150);
		m_List.InsertColumn(8, _T("被盗电机号"), LVCFMT_LEFT, 150);
		m_List.InsertColumn(9, _T("失主姓名"), LVCFMT_LEFT, 60);
		m_List.InsertColumn(10, _T("失主电话"), LVCFMT_LEFT, 140);
	}

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	ListTotal=0;
	ListNow=0;
	listhistoryAll.clear();

	//1为全部 0为报警
	if(1 == Flag)
	{
		if(true == YiRongCardlg->m_connectionState)
			YiRongCardlg->pio.readAllMatchResultVehicle(listhistoryAll);
	}
	else
	{
		if(true == YiRongCardlg->m_connectionState)
			YiRongCardlg->pio.readAllAlarmVehicle(listhistoryAll);
	}

	ListTotal=listhistoryAll.size();
	DisplayerList();

	return TRUE;

}

void CHistoryDLG::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CHistoryDLG::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	if((ListNow+HL_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HL_PAGE_MAX_NUM;

	DisplayerList();
}

void CHistoryDLG::OnBnClickedButtonUp()
{
	// TODO: Add your control notification handler code here
	if((ListNow-HL_PAGE_MAX_NUM) >= 0)
		ListNow-=HL_PAGE_MAX_NUM;

	DisplayerList();
}

void CHistoryDLG::OnBnClickedButtonFirst()
{
	// TODO: Add your control notification handler code here
	ListNow=0;
	DisplayerList();
}

void CHistoryDLG::OnBnClickedButtonEnd()
{
	// TODO: Add your control notification handler code here
	while((ListNow+HL_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HL_PAGE_MAX_NUM;

	DisplayerList();
}


void CHistoryDLG::DisplayerList(void)
{
	int nItem;

	list<struct HISTORY_STRUCT>::iterator beglist;
	char timestr[128];

	int i;
	long int total=0;

	m_List.DeleteAllItems();
	beglist=listhistoryAll.begin();
	for(i=0;i<ListNow;i++)
	{
		beglist++;
	}
	for(NULL;(beglist!=listhistoryAll.end()) && (total < HL_PAGE_MAX_NUM);beglist++)
	{
		sprintf(timestr,"%07d",total+ListNow+1);
		nItem = m_List.InsertItem(0,timestr);

		sprintf(timestr,"%04d-%02d-%02d  %02d:%02d:%02d",
			beglist->year,
			beglist->mon,
			beglist->day,
			beglist->hour,
			beglist->min,
			beglist->sec);

		m_List.SetItemText(nItem,1,timestr);
		m_List.SetItemText(nItem,2,beglist->ip);
		m_List.SetItemText(nItem,3,beglist->carNumber);
		m_List.SetItemText(nItem,4,beglist->path);

		if(0 == Flag)
		{
			m_List.SetItemText(nItem,5,beglist->info.PinPai);
			m_List.SetItemText(nItem,6,beglist->info.ChePaiHao);
			m_List.SetItemText(nItem,7,beglist->info.CheJiaHao);
			m_List.SetItemText(nItem,8,beglist->info.DianJiHao);
			m_List.SetItemText(nItem,9,beglist->info.Name);
			m_List.SetItemText(nItem,10,beglist->info.Phone);
		}

		total++;
	}

	sprintf(timestr,"共%d条 第%d/%d页",ListTotal,ListNow/HL_PAGE_MAX_NUM+1,ListTotal/HL_PAGE_MAX_NUM+1);
	GetDlgItem(IDC_STATIC)->SetWindowText(timestr);
	
	listchoice=0;
	GetDlgItem(IDC_STATIC_LISTCHOICE)->SetWindowText("未选择车牌号");
}

void CHistoryDLG::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];
	char timestr[260];
	m_List.GetItemText(pNMIA->iItem,4,str,260);
	ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);
	
	m_List.GetItemText(pNMIA->iItem,3,str,260);
	sprintf(timestr,"已选择 车牌号：%s",str);
	GetDlgItem(IDC_STATIC_LISTCHOICE)->SetWindowText(timestr);
	listchoice=pNMIA->iItem;
	
	*pResult = 0;
}

void CHistoryDLG::OnBnClickedButtonCarnumber()
{
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	ListTotal=0;
	ListNow=0;
	listhistoryAll.clear();

	//1为全部 0为报警
	if(1 == Flag)
	{
		if(true == YiRongCardlg->m_connectionState)
			YiRongCardlg->pio.getMatchResultVehicleWithCarNum(m_carnumber.GetBuffer(),listhistoryAll);
	}
	else
	{
		if(true == YiRongCardlg->m_connectionState)
			YiRongCardlg->pio.getAlarmVehicleWithCarNum(m_carnumber.GetBuffer(),listhistoryAll);
	}

	ListTotal=listhistoryAll.size();
	DisplayerList();

}

void CHistoryDLG::OnBnClickedButtonTime()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CTime	StartT(m_TimeStartMon.GetYear(),m_TimeStartMon.GetMonth(),m_TimeStartMon.GetDay(),	\
		m_TimeStartHour.GetHour(),m_TimeStartHour.GetMinute(),m_TimeStartHour.GetSecond());

	CTime	EndT(m_TimeEndMon.GetYear(),m_TimeEndMon.GetMonth(),m_TimeEndMon.GetDay(),	\
		m_TimeEndHour.GetHour(),m_TimeEndHour.GetMinute(),m_TimeEndHour.GetSecond());

	if(StartT > EndT)
	{
		MessageBox("结束时间 不可小于 起始时间");
		return ;
	}

	char STime[32]={0};
	char ETime[32]={0};

	sprintf(STime,"%02d%02d%02d%02d%02d%02d",		
		m_TimeStartMon.GetYear(),
		m_TimeStartMon.GetMonth(),
		m_TimeStartMon.GetDay(),
		m_TimeStartHour.GetHour(),
		m_TimeStartHour.GetMinute(),
		m_TimeStartHour.GetSecond());

	sprintf(ETime,"%02d%02d%02d%02d%02d%02d",		
		m_TimeEndMon.GetYear(),
		m_TimeEndMon.GetMonth(),
		m_TimeEndMon.GetDay(),
		m_TimeEndHour.GetHour(),
		m_TimeEndHour.GetMinute(),
		m_TimeEndHour.GetSecond());

	// TODO: Add your control notification handler code here
	ListTotal=0;
	ListNow=0;
	listhistoryAll.clear();

	//1为全部 0为报警
	if(1 == Flag)
	{
		if(true == YiRongCardlg->m_connectionState)
			YiRongCardlg->pio.getMatchResultVehicleWithTime(STime,ETime,listhistoryAll);
	}
	else
	{
		if(true == YiRongCardlg->m_connectionState)
			YiRongCardlg->pio.getAlarmVehicleWithTime(STime,ETime,listhistoryAll);
	}

	ListTotal=listhistoryAll.size();
	DisplayerList();

}

void CHistoryDLG::OnBnClickedButtonSms()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//空号码退出
	if(m_smsphone.IsEmpty() || (0 == listchoice) || (m_smsphone.GetLength()!=11))
	{
		MessageBox("手机号码格式错误 或者 未选中列表项");
		return ;
	}

	char pinpai[260]={0};
	char chepaihao[260]={0};
	char chejiahao[260]={0};
	char dianjihao[260]={0};
	char name[260]={0};
	char phone[260]={0};
	char id[260]={0};

	m_List.GetItemText(listchoice,0,id,260);
	m_List.GetItemText(listchoice,5,pinpai,260);
	m_List.GetItemText(listchoice,6,chepaihao,260);
	m_List.GetItemText(listchoice,7,chejiahao,260);
	m_List.GetItemText(listchoice,8,dianjihao,260);
	m_List.GetItemText(listchoice,9,name,260);
	m_List.GetItemText(listchoice,10,phone,260);

	char tempstr[1024]={0};

	sprintf(tempstr,"[发现可疑电动车:%d]津泰路口\n品牌:%s\n车牌号:%s\n车架号:%s\n电机号:%s\n失主姓名:%s\n电话:%s\n",	\
		id,
		pinpai,
		chepaihao,
		chejiahao,
		dianjihao,
		name,
		phone);

	if(0 == smsclass.smsSendOne(m_smsphone.GetBuffer(),tempstr,strlen(tempstr)))
		MessageBox("已发送");
	else
		MessageBox("发送失败");
}
