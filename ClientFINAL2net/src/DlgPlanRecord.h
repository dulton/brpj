#if !defined(AFX_DLGPLANRECORD_H__309F611A_A249_48EF_9CD6_37E214A55BD9__INCLUDED_)
#define AFX_DLGPLANRECORD_H__309F611A_A249_48EF_9CD6_37E214A55BD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlanRecord.h : header file
//
#include "IOoracle.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPlanRecord dialog
#define LISTMAX_NUM 10
#define FTPMAX_NUM 100

//服务器接口
typedef struct 
{
//	char policeID[64];
	long int camID;
//	char sername[64];                    //server name
//	char username[32];                   //user name
//	char password[32];                   //password
//	char url[64];                       //url
//	WORD port;

}RECORD_CLIENTINFO;

/*
starthhmm：起始小时分钟；
stophhmm：结束小时分钟；
maskweek：
0：星期天；
1：星期一；
2：星期二；
3：星期三；
4：星期四；
5：星期五；
6：星期六；
7：星期一——星期五；
8：星期六——星期天；
9：每天；
10：关闭，不使用该时间段；
*/
typedef struct 
{
	int starthhmm;
	int stophhmm;
	unsigned char maskweek;
}RECORD_WEEKPLAN;

/*
startyyyymmdd：起始年月日
starthhmm：起始小时分钟；
stopyyyymmdd:结束年月日
stophhmm：结束小时分钟；
maskday:
0：关闭，不使用该时间段
1：开启，使用该时间段
*/
typedef struct 
{
	unsigned long int startyyyymmdd;
	int  starthhmm;
	unsigned long int stopyyyymmdd;
	int  stophhmm;
	BOOL maskday;
}RECORD_DAYPLAN;

// DIR 文件目录 需要包含最后的“\\”符号 比如C:TEMP
// CutTime 每次间隔多少分钟就分割一次文件 最多 60*23分钟
typedef struct 
{
	unsigned char CutTime;
	TCHAR DIR[MAX_PATH]; 
}RECORD_FILE;

//录制状态
//0 未开始录制
//1 正在录制
//2 信号中断或失败  
//3 暂停录制
//4 随时准备录制 （防止机器重启后不录制）
//unsigned char Status;
enum 
{
	RECORD_STOP=0,
	RECORD_START=1,
	RECORD_LOST=2,
	RECORD_PAUSE=3,
	RECORD_PLANSTART=4,
	RECORD_UNKOWN=100
};

typedef struct 
{
	//本地保存的文件路径
	TCHAR localpath[MAX_PATH]; 
	//文件名
	TCHAR filename[MAX_PATH]; 
	//相对保存路径
	TCHAR ftppath[MAX_PATH];
	TCHAR netpath[MAX_PATH]; 

	//本地保存的文件路径
	TCHAR localAVIpath[MAX_PATH]; 
	//文件名
	TCHAR AVIfilename[MAX_PATH]; 
	//相对保存路径
	TCHAR AVIftppath[MAX_PATH];
	TCHAR AVInetpath[MAX_PATH]; 

	//视频开始时间
	CTime stime;
	//视频结束时间
	CTime etime;
	//预计停止的录制时间
	int hour;
	int min;
	unsigned char status;
	unsigned char planweek; //启用1 关闭0
	unsigned char planday;  //启用1 关闭0

}RECORD_STATUS;

typedef struct
{
	//本地保存的绝对文件路径 D:\40\1.mp4
	TCHAR localpath[MAX_PATH]; 
	//文件名
	TCHAR filename[MAX_PATH]; 
	//FTP相对保存路径 /40/1.mp4
	TCHAR ftppath[MAX_PATH]; 
	//网络映射相对保存路径 \40\1.mp4
	TCHAR netpath[MAX_PATH]; 

	//本地保存的绝对文件路径 D:\40\1.avi
	TCHAR localAVIpath[MAX_PATH]; 
	//文件名
	TCHAR AVIfilename[MAX_PATH]; 
	//相对保存路径
	TCHAR AVIftppath[MAX_PATH];
	TCHAR AVInetpath[MAX_PATH]; 


	//视频开始时间
	CTime stime;
	//视频结束时间
	CTime etime;
	//摄像头ID
	long camid;
	//线程句柄
	HANDLE pthread;
	//线程在用=false  线程未用=ture
	bool ThreadFlag;
	//线程不退=false  线程强退=ture
//	bool forceout;

}FTP_RECORD;

class CDlgPlanRecord : public CDialog
{
	// Construction
public:
	CDlgPlanRecord(CWnd* pParent = NULL);   // standard constructor

	void Lost(long int handle);
	void CheckTime(BOOL link,long int handle,
		 int year,int mon,int day,int hour, int min,int week);
	// Dialog Data
	//{{AFX_DATA(CDlgPlanRecord)
	enum { IDD = IDD_PLANRECORD };
	CListCtrl	m_listAlarm;
	int		m_PlanDayList;
	BOOL	m_PlanDayMask;
	int		m_PlanWeekList;
	int		m_PlanWeekMask;
	CTime	m_PlanDayYMDS;
	CTime	m_PlanDayYMDE;
	CTime	m_PlanDayHME;
	CTime	m_PlanDayHMS;
	CTime	m_PlanWeekHME;
	CTime	m_PlanWeekHMS;
	BYTE	m_Cuttime;
	CString	m_PlanDir;
	int		m_planmode;
	//}}AFX_DATA
	RECORD_WEEKPLAN weekplan[LISTMAX_NUM];
	RECORD_DAYPLAN dayplan[LISTMAX_NUM];
	RECORD_FILE FileDir;
	RECORD_CLIENTINFO info;
	RECORD_STATUS RecordStatus;
	long int m_hHandle;


	int modeflag; //0=非模板 1=模板
	int modeexec;//执行保存的info.camid 不执行保存=10

	CTime	Disp_PlanDayYMDS[LISTMAX_NUM];
	CTime	Disp_PlanDayYMDE[LISTMAX_NUM];
	CTime	Disp_PlanDayHME[LISTMAX_NUM];
	CTime	Disp_PlanDayHMS[LISTMAX_NUM];
	CTime	Disp_PlanWeekHME[LISTMAX_NUM];
	CTime	Disp_PlanWeekHMS[LISTMAX_NUM];

	// Overrides
	FTP_RECORD FtpRecord[FTPMAX_NUM];

	//用于批量读取和设置
	
	BOOL PlanReadALLC(long int id) ;
	void PlanSaveALLC();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPlanRecord)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL



	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPlanRecord)
	afx_msg void OnBUTTONPlanDaySave();
	afx_msg void OnBUTTONPlanWeekSave();
	afx_msg void OnBUTTONPlanSetSave();
	afx_msg void OnBUTTONPlanDayStart();
	afx_msg void OnBUTTONPlanWeekStart();
	afx_msg void OnBUTTONPlanDir();
	afx_msg void OnCHECKPlanDayMask();
	afx_msg void OnCloseupCOMBOPlanWeekList();
	afx_msg void OnCloseupCOMBOPlanDayList();
	afx_msg void OnCloseupCOMBOPlanWeekMask();
	afx_msg void OnBUTTONPlanDayRead();
	afx_msg void OnBUTTONPlanWeekRead();
	afx_msg void OnBUTTONsaveall();
	afx_msg void OnCloseupCOMBOMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void DialogDisplayDay();
	void DialogDisplayWeek();
	void PlanWeekSaveList(int list);
	void PlanDaySaveList(int list);

	BOOL PlanWeekRecordTimeRange(int weekplan_total,int  hour, int min,int week);

	BOOL PlanDayRecordTimeRange(int dayplan_total,int year,int mon,int day,int hour, int min);

	BOOL LocalRecordCheckTime(int  hour, int min);
	void LocalRecordStart(BOOL link);
	void LocalRecordStop();

	void AlarmMassage(int status,int type,int num,int flag,const char *stime,const char *etime);
	void AlarmMassageSet(void);

	void PlanReadALL() ;
	//void PlanSetRead();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLANRECORD_H__309F611A_A249_48EF_9CD6_37E214A55BD9__INCLUDED_)
