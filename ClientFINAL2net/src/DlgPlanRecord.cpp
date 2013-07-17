// DlgPlanRecord.cpp : implementation file
//

#include "stdafx.h"

#include "laump4test.h"
#include "LAUMp4TestDlg.h"

#include "DlgPlanRecord.h"
#include "SignalDownload.h"

#include "lauplaym4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPlanRecord dialog
extern IOoracle poracle;
//FTPconnect 连通=true; 不通为 false;
bool	FTPconnect=false;

extern  CLAUMp4TestDlg *m_MainDlgWnd;

CDlgPlanRecord::CDlgPlanRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlanRecord::IDD, pParent)
{
	CTime s(2012,1,1,0,0,0);

	//{{AFX_DATA_INIT(CDlgPlanRecord)
	m_PlanDayList = 0;
	m_PlanDayMask = FALSE;
	m_PlanWeekList = 0;
	m_PlanWeekMask = 10;
	m_PlanDayYMDS = s;
	m_PlanDayYMDE = s;
	m_PlanDayHME = s;
	m_PlanDayHMS = s;
	m_PlanWeekHME = s;
	m_PlanWeekHMS = s;
	m_Cuttime = 10;
	m_PlanDir = _T("D:\\");
	m_planmode = -1;
	//}}AFX_DATA_INIT
	int i;
	for(i=0;i<LISTMAX_NUM;i++)
	{
		dayplan[i].startyyyymmdd=20120101;
		dayplan[i].starthhmm=0;
		dayplan[i].stopyyyymmdd=20120101;
		dayplan[i].stophhmm=0;
		dayplan[i].maskday=0;
		weekplan[i].maskweek=10;
		weekplan[i].starthhmm=0;
		weekplan[i].stophhmm=0;

		Disp_PlanDayYMDS[i]=s;
		Disp_PlanDayYMDE[i]=s;
		Disp_PlanDayHME[i]=s;
		Disp_PlanDayHMS[i]=s;
		Disp_PlanWeekHME[i]=s;
		Disp_PlanWeekHMS[i]=s;

	}

	FileDir.CutTime=10;

	RecordStatus.status=RECORD_UNKOWN;
	RecordStatus.planweek=0; 
	RecordStatus.planday=0; 
	//批量模板才有
	modeflag=0;
	m_planmode=0;
	modeexec=10;

	for(i=0;i<FTPMAX_NUM;i++)
	{
		FtpRecord[i].ThreadFlag=true;
		FtpRecord[i].pthread=0;
	}
}

void CDlgPlanRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPlanRecord)
	DDX_Control(pDX, IDC_LIST_ALARM, m_listAlarm);
	DDX_CBIndex(pDX, IDC_COMBO_PlanDayList, m_PlanDayList);
	DDX_Check(pDX, IDC_CHECK_PlanDayMask, m_PlanDayMask);
	DDX_CBIndex(pDX, IDC_COMBO_PlanWeekList, m_PlanWeekList);
	DDX_CBIndex(pDX, IDC_COMBO_PlanWeekMask, m_PlanWeekMask);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PlanDayYMDS, m_PlanDayYMDS);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PlanDayYMDE, m_PlanDayYMDE);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PlanDayHME, m_PlanDayHME);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PlanDayHMS, m_PlanDayHMS);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PlanWeekHME, m_PlanWeekHME);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PlanWeekHMS, m_PlanWeekHMS);
	DDX_Text(pDX, IDC_EDIT_PlanRecordCut, m_Cuttime);
	DDV_MinMaxByte(pDX, m_Cuttime, 0, 240);
	DDX_Text(pDX, IDC_EDIT_PlanDir, m_PlanDir);
	DDV_MaxChars(pDX, m_PlanDir, 260);
	DDX_CBIndex(pDX, IDC_COMBO_Mode, m_planmode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlanRecord, CDialog)
	//{{AFX_MSG_MAP(CDlgPlanRecord)
	ON_BN_CLICKED(IDC_BUTTON_PlanDir, OnBUTTONPlanDir)
	ON_BN_CLICKED(IDC_CHECK_PlanDayMask, OnCHECKPlanDayMask)
	ON_CBN_CLOSEUP(IDC_COMBO_PlanWeekList, OnCloseupCOMBOPlanWeekList)
	ON_CBN_CLOSEUP(IDC_COMBO_PlanDayList, OnCloseupCOMBOPlanDayList)
	ON_CBN_CLOSEUP(IDC_COMBO_PlanWeekMask, OnCloseupCOMBOPlanWeekMask)
	ON_BN_CLICKED(IDC_BUTTON_saveall, OnBUTTONsaveall)
	ON_CBN_CLOSEUP(IDC_COMBO_Mode, OnCloseupCOMBOMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//上传上去
void CDlgPlanRecord::DialogDisplayDay() 
{
	m_PlanDayMask =dayplan[m_PlanDayList].maskday;
	m_PlanDayYMDS = Disp_PlanDayYMDS[m_PlanDayList];
	m_PlanDayYMDE = Disp_PlanDayYMDE[m_PlanDayList];
	m_PlanDayHMS = Disp_PlanDayHMS[m_PlanDayList];
	m_PlanDayHME = Disp_PlanDayHME[m_PlanDayList];
	UpdateData(FALSE);

	if(0==m_PlanDayMask)
	{
		((CButton*)GetDlgItem(IDC_CHECK_PlanDayMask))->SetCheck(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayYMDS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayYMDE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayHMS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayHME)->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_PlanDayMask))->SetCheck(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayYMDS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayYMDE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayHMS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanDayHME)->EnableWindow(TRUE);
	}
}

//上传上去
void CDlgPlanRecord::DialogDisplayWeek() 
{
	m_PlanWeekMask=weekplan[m_PlanWeekList].maskweek;
	m_PlanWeekHMS=Disp_PlanWeekHMS[m_PlanWeekList];
	m_PlanWeekHME=Disp_PlanWeekHME[m_PlanWeekList];

	UpdateData(FALSE);

	if(10==m_PlanWeekMask)
	{
		GetDlgItem(IDC_DATETIMEPICKER_PlanWeekHMS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanWeekHME)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER_PlanWeekHMS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_PlanWeekHME)->EnableWindow(TRUE);
	}
}

// CDlgPlanRecord message handlers
BOOL CDlgPlanRecord::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listAlarm.InsertColumn(0, "状态" , LVCFMT_LEFT, 80);
	m_listAlarm.InsertColumn(1, "类型" , LVCFMT_LEFT, 70);
	m_listAlarm.InsertColumn(2, "列表号" , LVCFMT_LEFT, 80);
	m_listAlarm.InsertColumn(3, "启用标志", LVCFMT_LEFT, 120);
	m_listAlarm.InsertColumn(4, "起始时间", LVCFMT_LEFT, 130);
	m_listAlarm.InsertColumn(5, "结束时间", LVCFMT_LEFT, 130);

	m_listAlarm.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	DialogDisplayDay();
	DialogDisplayWeek();

	if(	RecordStatus.planweek)
		GetDlgItem(IDC_BUTTON_PlanWeekStart)->SetWindowText("停止周计划");
	else
		GetDlgItem(IDC_BUTTON_PlanWeekStart)->SetWindowText("开始周计划");

	if(	RecordStatus.planday)
		GetDlgItem(IDC_BUTTON_PlanDayStart)->SetWindowText("停止日计划");
	else
		GetDlgItem(IDC_BUTTON_PlanDayStart)->SetWindowText("开始日计划");

	//默认直接读取
	//PlanSetRead();
	//OnBUTTONPlanDayRead();
	//OnBUTTONPlanWeekRead();

	PlanReadALL();
	if(modeflag)
	{
		GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_COMBO_Mode)->ShowWindow(SW_NORMAL);
	
		if(10!=modeexec)
			m_planmode=-modeexec;
		else
			m_planmode=0;

		modeexec=10;
	}
	else
	{
		GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_Mode)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}

void CDlgPlanRecord::OnCancel()
{
	CDialog::OnCancel();
}

void CDlgPlanRecord::OnBUTTONPlanDir() 
{
#if 0
	// TODO: Add your control notification handler code here
	//保存文件
	char szFilter[]="*.*||";
	CFileDialog dlg(FALSE,"*.*","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		char filename[MAX_PATH];

		GetDlgItem(IDC_EDIT_PlanDir)->SetWindowText(dlg.GetPathName());
		GetDlgItem(IDC_EDIT_PlanDir)->GetWindowText(filename,sizeof(filename));
	
	}
#endif
	//保存目录
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体
	TCHAR   Buffer[512]= " ";
	TCHAR   FullPath[512]= " ";
	bi.hwndOwner   =   CDlgPlanRecord::GetSafeHwnd();               //窗口句柄
	bi.pidlRoot   =   NULL;
	bi.pszDisplayName   =   Buffer;            //返回选择的目录名的缓冲区
	bi.lpszTitle   =   "Selection ";           //弹出的窗口的文字提示
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;  //只返回目录。其他标志看MSDN
	bi.lpfn   =   NULL;               //回调函数，有时很有用
	bi.lParam   =   0;
	bi.iImage   =   0;
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要
	if(::SHGetPathFromIDList(pidl,FullPath)) //在ITEMIDLIST中得到目录名的整个路径
		GetDlgItem(IDC_EDIT_PlanDir)->SetWindowText(FullPath);

}

void CDlgPlanRecord::OnCHECKPlanDayMask() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	PlanDaySaveList(m_PlanDayList);
	DialogDisplayDay();
}


void CDlgPlanRecord::OnCloseupCOMBOPlanWeekList() 
{
	// TODO: Add your control notification handler code here
	int beforeList=m_PlanWeekList;

	UpdateData(TRUE);
	PlanWeekSaveList(beforeList);
	DialogDisplayWeek();
}

void CDlgPlanRecord::OnCloseupCOMBOPlanDayList() 
{
	// TODO: Add your control notification handler code here
	int beforeList=m_PlanDayList;

	UpdateData(TRUE);
	PlanDaySaveList(beforeList);
	DialogDisplayDay();
}

void CDlgPlanRecord::PlanWeekSaveList(int list) 
{
	weekplan[list].maskweek=m_PlanWeekMask;

	weekplan[list].starthhmm=
		m_PlanWeekHMS.GetHour()*100+m_PlanWeekHMS.GetMinute();

	weekplan[list].stophhmm=
		m_PlanWeekHME.GetHour()*100+m_PlanWeekHME.GetMinute();

	Disp_PlanWeekHMS[list]=m_PlanWeekHMS;
	Disp_PlanWeekHME[list]=m_PlanWeekHME;

	char strB[MAX_PATH];
	char str[MAX_PATH];
	sprintf(str,"%04d分",weekplan[list].starthhmm);
	sprintf(strB,"%04d分",weekplan[list].stophhmm);
	AlarmMassage(0,0,list,weekplan[list].maskweek,str,strB);

}

void CDlgPlanRecord::PlanDaySaveList(int list) 
{
	dayplan[list].maskday=m_PlanDayMask;

	dayplan[list].startyyyymmdd=	
		(unsigned long int)m_PlanDayYMDS.GetYear()*10000+
		(unsigned long int)m_PlanDayYMDS.GetMonth()*100+
		(unsigned long int)m_PlanDayYMDS.GetDay();

	dayplan[list].starthhmm=
		m_PlanDayHMS.GetHour()*100+m_PlanDayHMS.GetMinute();

	dayplan[list].stopyyyymmdd=	
		(unsigned long int)m_PlanDayYMDE.GetYear()*10000+
		(unsigned long int)m_PlanDayYMDE.GetMonth()*100+
		(unsigned long int)m_PlanDayYMDE.GetDay();

	dayplan[list].stophhmm=
		m_PlanDayHME.GetHour()*100+m_PlanDayHME.GetMinute();

	Disp_PlanDayYMDS[list]=m_PlanDayYMDS;
	Disp_PlanDayYMDE[list]=m_PlanDayYMDE;

	Disp_PlanDayHMS[list]=m_PlanDayHMS;
	Disp_PlanDayHME[list]=m_PlanDayHME;

	char strB[MAX_PATH];
	char str[MAX_PATH];
	sprintf(str,"%08d日%04d分",dayplan[list].startyyyymmdd,dayplan[list].starthhmm);
	sprintf(strB,"%08d日%04d分",dayplan[list].stopyyyymmdd,dayplan[list].stophhmm);
	AlarmMassage(0,1,list,dayplan[list].maskday,str,strB);

}

void CDlgPlanRecord::OnCloseupCOMBOPlanWeekMask() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	PlanWeekSaveList(m_PlanWeekList);
	DialogDisplayWeek();
}

void CDlgPlanRecord::Lost(long int handle)
{
	m_hHandle=handle;

	if(RECORD_START == RecordStatus.status)
		LocalRecordStop();
}

void CDlgPlanRecord::CheckTime(BOOL link,long int handle,
	 int year,int mon, int day, int hour, int min,int week)
{
	m_hHandle=handle;
	//未开始录制
	if(RECORD_START != RecordStatus.status)
	{
		if(!link) return ;
		//查询是否可以开始
		if((RecordStatus.planweek && PlanWeekRecordTimeRange(LISTMAX_NUM,hour,min,week))||
			(RecordStatus.planday && PlanDayRecordTimeRange(LISTMAX_NUM,year,mon,day,hour,min) ))
			LocalRecordStart(link);
	}
	else
	{	
		//已开始录制
		if(!link) 
		{
			LocalRecordStop();
			return ;
		}
		//查询是否可以停止
		if(!((RecordStatus.planweek && PlanWeekRecordTimeRange(LISTMAX_NUM,hour,min,week))||
			(RecordStatus.planday && PlanDayRecordTimeRange(LISTMAX_NUM,year,mon,day,hour,min) )))
			LocalRecordStop();
		//不必停止则查询是否可以切割
		else	if(LocalRecordCheckTime(hour,min))
		{
			LocalRecordStop();
			if(RECORD_START != RecordStatus.status)
				LocalRecordStart(link);
		}
	}	
}

//返回值 1 在某个时间内 要处于录制状态
//返回值 0 在全部的时间外 不必录制 
BOOL CDlgPlanRecord::PlanWeekRecordTimeRange(int weekplan_total,int  hour, int min,int week)
{
	int i;
	int nowtime;

	if(0==weekplan_total)
		return 0;

	nowtime= hour*100+min;

	for(i=0;i<weekplan_total;i++)
	{
		if(10==weekplan[i].maskweek)
			continue;
		else if(9==weekplan[i].maskweek)
		{
			if(( weekplan[i].starthhmm <=nowtime) && (nowtime <= weekplan[i].stophhmm ))
				return 1;
		}
		else if(8==weekplan[i].maskweek)
		{
			if((6==week) && (0==week))
			{
				if(( weekplan[i].starthhmm <=nowtime) && (nowtime <= weekplan[i].stophhmm ))
					return 1;
			}
		}
		else if (7==weekplan[i].maskweek)
		{
			if((1 <= week) && (week <=5 ))
			{
				if(( weekplan[i].starthhmm <=nowtime) && (nowtime <= weekplan[i].stophhmm ))
					return 1;
			}
		}
		else if (weekplan[i].maskweek ==week)
		{
			if(( weekplan[i].starthhmm <=nowtime) && (nowtime <= weekplan[i].stophhmm ))
				return 1;
		}	
	}

	return 0;
}
//返回值 1 在某个时间内 要处于录制状态
//返回值 0 在全部的时间外 不必录制
BOOL CDlgPlanRecord::PlanDayRecordTimeRange(int dayplan_total,
						int year,int mon,int day,int hour, int min)
{
	int i;

	unsigned long int nowyyyymmdd;
	int nowhhmm;

	if(0==dayplan_total)
		return 0;

	nowyyyymmdd=(unsigned long int)year*10000+(unsigned long int)mon*100+(unsigned long int)day;
	nowhhmm=(int)hour*100+(int)min;

	for(i=0;i<dayplan_total;i++)
	{
		if(0==dayplan[i].maskday)
			continue ;

		if( ((dayplan[i].startyyyymmdd <= nowyyyymmdd) && (nowyyyymmdd <= dayplan[i].stopyyyymmdd)) &&
			((dayplan[i].starthhmm <= nowhhmm) && (nowhhmm <= dayplan[i].stophhmm)))
			return 1;
	}
	return 0;
}

//定时切割文件 返回1 切割 新建文件 0不切割
BOOL CDlgPlanRecord::LocalRecordCheckTime(int hour, int min)
{
	if((RECORD_START == RecordStatus.status) && (0 != FileDir.CutTime) )
	{
		if((RecordStatus.hour == hour) &&
			(RecordStatus.min <= min) )
			return 1;
	}
	return 0;
}
//命名规则在这里改
void CDlgPlanRecord::LocalRecordStart(BOOL link)
{
	TCHAR path[MAX_PATH]={0};
	TCHAR temppath[MAX_PATH]={0};

	if(!link)
	{
		RecordStatus.status=RECORD_PLANSTART;
		return ;
	}
	char timestr[64];
	int year, mon, day, hour, min,sec;

	//获取时间
	if( poracle.GetSysdateFromOracleDB(timestr,&year, &mon, &day, &hour, &min,&sec))
	{
		_stprintf(temppath,_T("%s\\%d"),FileDir.DIR,info.camID);
		CreateDirectory(temppath, NULL);
		_stprintf(temppath,_T("%s\\%d\\%04d-%02d-%02d"),
			FileDir.DIR,
			info.camID,	
			year,
			mon,
			day);
		CreateDirectory(temppath, NULL);

		_stprintf(path,_T("%s\\%d_%s.mp4"),
			temppath,
			info.camID,
			timestr);

		if(VSNET_ClientStartMp4Capture(m_hHandle,path))
		{
			RecordStatus.hour=(hour+(min+FileDir.CutTime)/60)%24;
			//当前时间大于未来中断时间 说明跨天
			if(hour > RecordStatus.hour)
			{
				RecordStatus.hour=0;
				RecordStatus.min=0;
			}
			else
				RecordStatus.min=(min+FileDir.CutTime)%60;
			//拷贝文件路径 时间
			CTime s(year, mon, day, hour, min,sec);
			RecordStatus.stime=s;

			memcpy(RecordStatus.localpath,path,MAX_PATH);

			//设置FTP路径
			_stprintf(RecordStatus.ftppath,_T("%d/%04d-%02d-%02d/%d_%s.mp4"),
				info.camID,	
				year,
				mon,
				day,	
				info.camID,
				timestr);
			//设置NET路径
			_stprintf(RecordStatus.netpath,_T("%d\\%04d-%02d-%02d\\%d_%s.mp4"),
				info.camID,	
				year,
				mon,
				day,	
				info.camID,
				timestr);

			_stprintf(RecordStatus.filename,_T("%d_%s.mp4"),info.camID,timestr);

			/////////////////////AVI//////////////////
			_stprintf(RecordStatus.localAVIpath,_T("%s\\%d\\%04d-%02d-%02d\\%d_%s.avi"),
				FileDir.DIR,
				info.camID,	
				year,
				mon,
				day,	
				info.camID,
				timestr);
			//设置FTP路径
			_stprintf(RecordStatus.AVIftppath,_T("%d/%04d-%02d-%02d/%d_%s.avi"),
				info.camID,	
				year,
				mon,
				day,	
				info.camID,
				timestr);
			//设置NET路径
			_stprintf(RecordStatus.AVInetpath,_T("%d\\%04d-%02d-%02d\\%d_%s.avi"),
				info.camID,	
				year,
				mon,
				day,	
				info.camID,
				timestr);

			_stprintf(RecordStatus.AVIfilename,_T("%d_%s.avi"),info.camID,timestr);

			RecordStatus.status=RECORD_START;
		}
		else
			RecordStatus.status=RECORD_PLANSTART;
	}
	else
		RecordStatus.status=RECORD_PLANSTART;
}

bool zogMP4toAVI(char *src,char *dst)
{
	long int conv_h=LCPLAYM4_StartAVIConvert(src,dst);

	if(conv_h >= 0)
	{
		while(LCPLAYM4_GetAVIConvertPos(conv_h) >= 0)
			Sleep(2000);

		LCPLAYM4_StopAVIConvert(conv_h);
		return true;
	}
	return false;
}

bool zogMP4toASF(char *src,char *dst)
{
	long int conv_h=LCPLAYM4_StartASFConvert(src,dst);

	if(conv_h >= 0)
	{
		while(LCPLAYM4_GetASFConvertPos(conv_h) >= 0)
			Sleep(2000);

		LCPLAYM4_StopASFConvert(conv_h);
		return true;
	}
	return false;
}

//FTP线程
DWORD WINAPI FtpThreadPROC(LPVOID lpParameter)
{
	FTP_RECORD *pFtpRecord = (FTP_RECORD*)lpParameter;
	SignalDownload sd;

	pFtpRecord->ThreadFlag=false;

	int nItem;
	nItem = m_MainDlgWnd->m_listalarm.InsertItem(0,"开始上传MP4");
	m_MainDlgWnd->m_listalarm.SetItemText(nItem,4,pFtpRecord->localpath);

	while(TRUE)
	{
		if(sd.FTPUpload(SD_CONFIG_TXT,pFtpRecord->netpath,pFtpRecord->ftppath,pFtpRecord->localpath,3))
		{
			FTPconnect=true;
			poracle.WriteInfoFromOracleDB_NETVIDEOINFO(pFtpRecord->camid,	\
				sd.ftplocalpath,	pFtpRecord->filename,	\
				pFtpRecord->stime,pFtpRecord->etime,sd.fsize,"mp4");
			break;
		}
		else
		{
			FTPconnect=false;
			Sleep(5000);
		}
	}

	nItem = m_MainDlgWnd->m_listalarm.InsertItem(0,"完成上传MP4");
	m_MainDlgWnd->m_listalarm.SetItemText(nItem,4,pFtpRecord->localpath);

#if 1

	SignalDownload xd;

	nItem = m_MainDlgWnd->m_listalarm.InsertItem(0,"压缩AVI");
	m_MainDlgWnd->m_listalarm.SetItemText(nItem,4,pFtpRecord->localAVIpath);

	if(zogMP4toAVI(pFtpRecord->localpath,pFtpRecord->localAVIpath))
	{
		nItem = m_MainDlgWnd->m_listalarm.InsertItem(0,"开始上传AVI");
		m_MainDlgWnd->m_listalarm.SetItemText(nItem,4,pFtpRecord->localAVIpath);

		while(TRUE)
		{
			///AVI//
			if(xd.FTPUpload(SD_CONFIG_TXT,pFtpRecord->AVInetpath,pFtpRecord->AVIftppath,pFtpRecord->localAVIpath,3))
			{
				FTPconnect=true;
				poracle.WriteInfoFromOracleDB_NETVIDEOINFO(pFtpRecord->camid,	\
					xd.ftplocalpath,	pFtpRecord->AVIfilename,	\
					pFtpRecord->stime,pFtpRecord->etime,xd.fsize,"avi");
				break;
			}
			else
			{
				FTPconnect=false;
				Sleep(5000);
			}
		}

		nItem = m_MainDlgWnd->m_listalarm.InsertItem(0,"完成上传AVI");
		m_MainDlgWnd->m_listalarm.SetItemText(nItem,4,pFtpRecord->localAVIpath);
	}
#endif

	pFtpRecord->ThreadFlag=true;
	return 0;
}
//取代VSNET_ClientStopMp4Capture
void CDlgPlanRecord::LocalRecordStop()
{
	 //获取时间 0星期天 1星期一 6星期六
	int year, mon, day, hour, min,sec;
	int i;

	if(VSNET_ClientStopMp4Capture(m_hHandle))
	{	
		//随时可以停止 但只有这个才写入数据库
		if(RECORD_START == RecordStatus.status )
		{
			if(poracle.GetSysdateFromOracleDB_DATE(&year, &mon, &day, &hour, &min,&sec))
			{
				//拷贝文件路径 时间
				CTime s(year, mon, day, hour, min,sec);
				RecordStatus.etime=s;
			}
			/*
			//	MessageBox(_T("文件尺寸成功"), _T("文件尺寸成功"));
			poracle->WriteInfoFromOracleDB_NETVIDEOINFO(info.camID,	\
			RecordStatus.localpath,RecordStatus.filename,	\
			RecordStatus.stime,RecordStatus.etime,size);
			*/
			for(i=0;i<FTPMAX_NUM;i++)
			{
				if(FtpRecord[i].ThreadFlag)
				{
					if(NULL!=FtpRecord[i].pthread)
						TerminateThread(FtpRecord[i].pthread,0);
				
					FtpRecord[i].camid=info.camID;
					memcpy(FtpRecord[i].localpath,RecordStatus.localpath,sizeof(TCHAR)*MAX_PATH);
					memcpy(FtpRecord[i].filename,RecordStatus.filename,sizeof(TCHAR)*MAX_PATH);
					memcpy(FtpRecord[i].ftppath,RecordStatus.ftppath,sizeof(TCHAR)*MAX_PATH);
					memcpy(FtpRecord[i].netpath,RecordStatus.netpath,sizeof(TCHAR)*MAX_PATH);

					memcpy(FtpRecord[i].localAVIpath,RecordStatus.localAVIpath,sizeof(TCHAR)*MAX_PATH);
					memcpy(FtpRecord[i].AVIfilename,RecordStatus.AVIfilename,sizeof(TCHAR)*MAX_PATH);
					memcpy(FtpRecord[i].AVIftppath,RecordStatus.AVIftppath,sizeof(TCHAR)*MAX_PATH);
					memcpy(FtpRecord[i].AVInetpath,RecordStatus.AVInetpath,sizeof(TCHAR)*MAX_PATH);
		
					FtpRecord[i].stime=RecordStatus.stime;
					FtpRecord[i].etime=RecordStatus.etime;
				
					FtpRecord[i].pthread=NULL;
					FtpRecord[i].pthread=CreateThread(NULL,0,FtpThreadPROC,&FtpRecord[i],0,NULL);
				
					//线程满了
					if(NULL==FtpRecord[i].pthread)
						break;
					break;
				}
			}
			//线程满了。不做任何事
		}
		
		RecordStatus.status=RECORD_STOP;
	}
}

void CDlgPlanRecord::AlarmMassage(int status,int type,int num,int flag,const char *stime,const char *etime)
{
	char str[4];

	int nItem;
	switch(status)
	{
	case 0:nItem =m_listAlarm.InsertItem(0,"保存");break;
	case 1:nItem =m_listAlarm.InsertItem(0,"读取");break;
	case 2:nItem =m_listAlarm.InsertItem(0,"保存配置表");break;
	case 3:nItem =m_listAlarm.InsertItem(0,"读取配置表");break;
	default:	return;
	}

	sprintf(str,"%d",num);
	m_listAlarm.SetItemText(nItem,2,str);

	if(type)
	{
		m_listAlarm.SetItemText(nItem,1,"日计划");
		if(flag)
			m_listAlarm.SetItemText(nItem,3,"启用");
		else
			m_listAlarm.SetItemText(nItem,3,"关闭");
	}
	else
	{
		m_listAlarm.SetItemText(nItem,1,"周计划");
		switch(flag)
		{
		case 0:m_listAlarm.SetItemText(nItem,3,"每周天");break;
		case 1:m_listAlarm.SetItemText(nItem,3,"每周一");break;
		case 2:m_listAlarm.SetItemText(nItem,3,"每周二");break;
		case 3:m_listAlarm.SetItemText(nItem,3,"每周三");break;
		case 4:m_listAlarm.SetItemText(nItem,3,"每周四");break;
		case 5:m_listAlarm.SetItemText(nItem,3,"每周五");break;
		case 6:m_listAlarm.SetItemText(nItem,3,"每周六");break;
		case 7:m_listAlarm.SetItemText(nItem,3,"每周一 -- 周五");break;
		case 8:m_listAlarm.SetItemText(nItem,3,"每周六 -- 周天");break;
		case 9:m_listAlarm.SetItemText(nItem,3,"每天");break;
		case 10:m_listAlarm.SetItemText(nItem,3,"关闭");break;
		default:break;
		}
	}

	m_listAlarm.SetItemText(nItem,4,stime);
	m_listAlarm.SetItemText(nItem,5,etime);
	//溢出删除
	if(m_listAlarm.GetItemCount() >= 22)
		m_listAlarm.DeleteItem(21);
}

void CDlgPlanRecord::AlarmMassageSet(void)
{
	char str[16];

	int nItem;
	nItem =m_listAlarm.InsertItem(0,"保存");

	m_listAlarm.SetItemText(nItem,1,"设置");

	m_listAlarm.SetItemText(nItem,2,"分割时间:");
	sprintf(str,"每%d分钟",FileDir.CutTime);
	m_listAlarm.SetItemText(nItem,3,str);

	m_listAlarm.SetItemText(nItem,4,"保存路径:");
	m_listAlarm.SetItemText(nItem,5,FileDir.DIR);

	//溢出删除
	if(m_listAlarm.GetItemCount() >= 22)
		m_listAlarm.DeleteItem(21);

}

void CDlgPlanRecord::PlanReadALL() 
{
	// TODO: Add your control notification handler code here
	//屏蔽目录 由数据库指定
	
	if(!PlanReadALLC(info.camID))
	{
		CTime s(2012,1,1,0,0,0);
		
		m_PlanDayList = 0;
		m_PlanDayMask = FALSE;
		m_PlanWeekList = 0;
		m_PlanWeekMask = 10;
		m_PlanDayYMDS = s;
		m_PlanDayYMDE = s;
		m_PlanDayHME = s;
		m_PlanDayHMS = s;
		m_PlanWeekHME = s;
		m_PlanWeekHMS = s;
		m_Cuttime = 10;
		m_PlanDir = _T("D:\\");
		
		UpdateData(FALSE);
		return ;
	}
	
	char strB[MAX_PATH];
	char str[MAX_PATH];
	int i=0;
	
	m_listAlarm.DeleteAllItems();
	for(i=0;i<LISTMAX_NUM;i++)
	{
		sprintf(str,"%08d日%04d分",dayplan[i].startyyyymmdd,dayplan[i].starthhmm);
		sprintf(strB,"%08d日%04d分",dayplan[i].stopyyyymmdd,dayplan[i].stophhmm);
		AlarmMassage(3,1,i,dayplan[i].maskday,str,strB);
	}
	DialogDisplayDay();

	for(i=0;i<LISTMAX_NUM;i++)
	{
		sprintf(str,"%04d分",weekplan[i].starthhmm);
		sprintf(strB,"%04d分",weekplan[i].stophhmm);
		AlarmMassage(3,0,i,weekplan[i].maskweek,str,strB);
	}
	DialogDisplayWeek();

	AlarmMassageSet();
	
	m_PlanDir.Format("%s",FileDir.DIR);
	m_Cuttime=FileDir.CutTime;
	
	UpdateData(FALSE);
}

void CDlgPlanRecord::OnBUTTONsaveall() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	PlanDaySaveList(m_PlanDayList);
	DialogDisplayDay();
	
	PlanWeekSaveList(m_PlanWeekList);
	DialogDisplayWeek();
	
	memcpy(FileDir.DIR,	m_PlanDir.GetBuffer(0),MAX_PATH);
	FileDir.CutTime=m_Cuttime;


	if(modeflag)
	{
		modeexec=info.camID;
	}

	int i;
	char strB[MAX_PATH];
	char str[MAX_PATH];

	PlanSaveALLC();
	
	m_listAlarm.DeleteAllItems();

	for(i=0;i<LISTMAX_NUM;i++)
	{
		sprintf(str,"%08d日%04d分",dayplan[i].startyyyymmdd,dayplan[i].starthhmm);
		sprintf(strB,"%08d日%04d分",dayplan[i].stopyyyymmdd,dayplan[i].stophhmm);
		AlarmMassage(2,1,i,dayplan[i].maskday,str,strB);
	}

	for(i=0;i<LISTMAX_NUM;i++)
	{
		sprintf(str,"%04d分",weekplan[i].starthhmm);
		sprintf(strB,"%04d分",weekplan[i].stophhmm);
		AlarmMassage(2,0,i,weekplan[i].maskweek,str,strB);
	}

	AlarmMassageSet();
}

BOOL CDlgPlanRecord::PlanReadALLC(long int id) 
{	
	int i;
	char str[MAX_PATH];
	sprintf(str,"%d PlanList.txt",id);
	FILE* fp=fopen(str,"r");
	if(fp)
	{
		for(i=0;i<LISTMAX_NUM;i++)
		{	
			memset(str,0,MAX_PATH);
			fgets(str,MAX_PATH,fp);

			if(0==str[0] || 13==str[0] || 10==str[0]) continue ;

			sscanf(str,"%d %d %d %d %d",
				&dayplan[i].startyyyymmdd,
				&dayplan[i].starthhmm,
				&dayplan[i].stopyyyymmdd,
				&dayplan[i].stophhmm,
				&dayplan[i].maskday);

			if(dayplan[i].startyyyymmdd || dayplan[i].starthhmm)
			{
				CTime sy(dayplan[i].startyyyymmdd/10000,
					(dayplan[i].startyyyymmdd%10000)/100,
					dayplan[i].startyyyymmdd%100,
					20,20,20);

				Disp_PlanDayYMDS[i]=sy;

				CTime sh(2012,1,1,
					dayplan[i].starthhmm/100,
					dayplan[i].starthhmm%100,0);
				Disp_PlanDayHMS[i]=sh;
			}

			if(dayplan[i].stopyyyymmdd || dayplan[i].stophhmm)
			{
				CTime ey(dayplan[i].stopyyyymmdd/10000,
					(dayplan[i].stopyyyymmdd%10000)/100,
					dayplan[i].stopyyyymmdd%100,
					20,20,20);

				Disp_PlanDayYMDE[i]=ey;

				CTime eh(2012,1,1,
					dayplan[i].stophhmm/100,
					dayplan[i].stophhmm%100,0);

				Disp_PlanDayHME[i]=eh;
			}
		}

		for(i=0;i<LISTMAX_NUM;i++)
		{	
			memset(str,0,260);
			fgets(str,260,fp);

			if(0==str[0] || 13==str[0] || 10==str[0]) continue ;

			sscanf(str,"%d %d %d",
				&weekplan[i].starthhmm,
				&weekplan[i].stophhmm,
				&weekplan[i].maskweek);

			CTime s(2012,1,1,weekplan[i].starthhmm/100,weekplan[i].starthhmm%100,0);
			Disp_PlanWeekHMS[i]=s;

			CTime e(2012,1,1,weekplan[i].stophhmm/100,weekplan[i].stophhmm%100,0);
			Disp_PlanWeekHME[i]=e;
		}
		
		memset(str,0,260);
		fgets(str,260,fp);
		sscanf(str,"%d",&FileDir.CutTime);

		memset(str,0,260);
		fgets(str,260,fp);
		sscanf(str,"%s",FileDir.DIR);

		fclose(fp);
		return TRUE;
	}
	else
		return FALSE;
}

void CDlgPlanRecord::PlanSaveALLC() 
{
	int i;
	char str[260];
	sprintf(str,"%d PlanList.txt",info.camID);
	FILE* fp=fopen(str,"w");
	if(fp)
	{
		for(i=0;i<LISTMAX_NUM;i++)
		{
			fprintf(fp,"%d %d %d %d %d\n",
				dayplan[i].startyyyymmdd,
				dayplan[i].starthhmm,
				dayplan[i].stopyyyymmdd,
				dayplan[i].stophhmm,
				dayplan[i].maskday);
		}

		for(i=0;i<LISTMAX_NUM;i++)
		{
			fprintf(fp,"%d %d %d\n",
				weekplan[i].starthhmm,
				weekplan[i].stophhmm,
				weekplan[i].maskweek);
		}

		fprintf(fp,"%d\n",FileDir.CutTime);
		fprintf(fp,"%s",FileDir.DIR);
		fclose(fp);

	}
	
}

void CDlgPlanRecord::OnCloseupCOMBOMode() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	OnBUTTONsaveall();
	info.camID= -m_planmode;
	PlanReadALL();
}
