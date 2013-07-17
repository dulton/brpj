// LAUMp4TestDlg.h : header file
//#include "luserparamx.h"
//{{AFX_INCLUDES()
//#include "luserparamx.h"
//}}AFX_INCLUDES
//

#if !defined(AFX_LAUMP4TESTDLG_H__8109FF41_7950_4188_B68C_84F8410CE982__INCLUDED_)
#define AFX_LAUMP4TESTDLG_H__8109FF41_7950_4188_B68C_84F8410CE982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShowWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CLAUMp4TestDlg dialog
typedef struct
{
	BOOL		m_bLocalRecing;			//本地录像标志
	int			m_iPTZActStatus;		//云台运动标志，0静止，0x01000左转，0x00100右转，0x00010上转，0x00001下转，0x100000旋转
	BOOL		m_bViewing;				//预览标志 //可见=1 不可见=0
	BOOL		m_bRedirecting;			//转发标志
	CString		m_sServerName;			//服务器名
	CString		m_sServerUrl;			//服务器URL
	int			m_iPort;				//端口号
	CString		m_sUserName;			//用户名
	CString		m_sPsw;					//密码
	int			m_iChlSn;				//通道序号，即4个通道中的第几个
	int			m_iTransType;			//0,UDP; 1,多播; 2,TCP
	LONG		m_hLnk;					//连接句柄
	CString		m_sMultiCastURL;		//多播地址
	int			m_iMultiCastPort;		//多播端口
	int			m_iMultiCastLocalPort;	//多播本地端口
	int			m_iWndSn;				//被分配给哪个窗口
	CString pid;
	long int camid;
	BOOL link; //连接标志 1=已连通 0=未连通
	long ChanSock;


}VIDEOCHLINFO;

typedef struct
{
//	int			m_iCtrlID;				//控件ID，比如IDC_STATIC_VIDEO1
	CShowWnd*	m_pShowWnd;				//视频显示控件变量
	int			m_iChlSn;				//对应视频通道序号，即0,1,2,3,4,5...与窗口序号是1 : n (n >=0 )关系
}VIDEOWNDINFO;

typedef struct
{
	int			m_iSelWndSn;			//当前选中窗口序号
}GLOBEENVI;

typedef struct
{
	LONG hHandle;
	int wParam;
	int lParam;
	void *context;			
}MSGCALLBKPARAM;  //消息按通道发出，但没有通道类，只能在主窗口中接受，为了在处理函数中辨别发送通道，必须把连接句柄传送过去

#define ZOG_MAX 1024

class CLAUMp4TestDlg : public CDialog
{
// Construction
public:
	CLAUMp4TestDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CLAUMp4TestDlg)
	enum { IDD = IDD_LAUMP4TEST_DIALOG };
	CShowWnd	m_video14;
	CShowWnd	m_video16;
	CShowWnd	m_video15;
	CShowWnd	m_video13;
	CShowWnd	m_video12;
	CShowWnd	m_video11;
	CShowWnd	m_video10;
	CShowWnd	m_video9;
	CShowWnd	m_video8;
	CShowWnd	m_video7;
	CShowWnd	m_video6;
	CShowWnd	m_video5;
	CListCtrl	m_listalarm;
	CSliderCtrl	m_VolumeCtrl;
	CShowWnd	m_video4;
	CShowWnd	m_video3;
	CShowWnd	m_video2;
	CShowWnd	m_video1;
	CString	m_sername;
	CString	m_url;
	CString	m_username;
	CString	m_password;
	int		m_channel;
	int		m_trantype;
	int		m_port;
	CString	m_tranmultiurl;
	int		m_tranmultiport;
	int		m_tranlocalport;
	int		m_CB_iMode;
//	CLUSerParamX	m_paramctrl;
	long	m_camid;
	CString	m_policeid;
	//}}AFX_DATA

	int zog_Videototal;
	VIDEOWNDINFO	g_VideoWndInfo[MAX_VIDEOWND];	//窗口信息
//	VIDEOCHLINFO	g_VideoChlInfo[MAX_VIDEOWND];	//通道信息
	GLOBEENVI		g_GlobeEnvi;					//全局信息
	
	LONG		    m_hTalkHandle;   //对讲句柄

	int zog_total;
	VIDEOCHLINFO	zog_VideoChlInfo[ZOG_MAX];	//通道信息

	void	OnClkVideoWnd(int Num, int Id);
	void	SetWndParkMode(int SingleLineMode);
	void	GetWndRect(int CurWndSn, int SingleLineMode, CRect* m_rstRect);
	void	PlayStopSwitch();
	int		iGet_SelChlNo();
//	VIDEOCHLINFO* pGet_SelChl();
//	VIDEOCHLINFO* pHandleToChl(LONG mhHandle);
	void	PTZActSafeCtrl(int type, int val);
	void	TranslateMsg(MSGCALLBKPARAM* pMsgCallBkParam, char* chMsg);
	void	StopAllChlActs();
	
	void StopPlay();
	void StartPlay();
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLAUMp4TestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLAUMp4TestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStaticVideo1();
	afx_msg void OnStaticVideo2();
	afx_msg void OnStaticVideo3();
	afx_msg void OnStaticVideo4();
	afx_msg void OnDbStatic1();
	afx_msg void OnDbStatic2();
    afx_msg void OnDbStatic3();
    afx_msg void OnDbStatic4();
	afx_msg void PlayChl();
	//afx_msg void StopChlActs();
	afx_msg void OnDestroy();
	afx_msg void OnBtnUpdate();
	virtual void OnCancel();
	afx_msg void OnBtnConfig();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnUserip();
	afx_msg void OnBtnTransmit();
	afx_msg void TransmitStart();
	afx_msg void TransmitStop();
	afx_msg void OnBtnTalk();
	afx_msg void LocalRecStart();
	afx_msg void LocalRecStop();
	afx_msg void OnBtnLocalRec();
	afx_msg void OnBtnUarttran();
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOutofmemorySliderVoice(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnVstransuser();
	afx_msg void OnBtnSysparam();
	afx_msg void OnCheckView();
	afx_msg void OnBtnClear();
	afx_msg void OnBtnBradcast();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnBtnCapture();
	afx_msg void OnButton4();
	afx_msg void OnStaticVideo5();
	afx_msg void OnStaticVideo6();
	afx_msg void OnStaticVideo7();
	afx_msg void OnStaticVideo8();
	afx_msg void OnStaticVideo9();
	afx_msg void OnStaticVideo10();
	afx_msg void OnStaticVideo11();
	afx_msg void OnStaticVideo12();
	afx_msg void OnStaticVideo13();
	afx_msg void OnStaticVideo14();
	afx_msg void OnStaticVideo15();
	afx_msg void OnStaticVideo16();
	afx_msg void OnSelchangeCOMBOMode();
	afx_msg void OnBtnPtzleft();
	afx_msg void OnBTNPTZRight();
	afx_msg void OnBtnPtzup();
	afx_msg void OnBtnPtzdown();
	afx_msg void OnBtnPtzauto();
	afx_msg void OnBtnBmpcapture();
	afx_msg void OnBtnServerjpgcapture();
	afx_msg LRESULT OnMsgSthHappened(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnPreset();
	afx_msg void OnBtnCall();
	afx_msg void OnBtnPlanrecord();
	afx_msg void OnTimer(UINT_PTR nIDEvent); 
	afx_msg void OnButtonChooseip();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	void SetBtnStatus();


	void zogPlayChl(int i) ;
	void zogStopChlActs(int i);

	int zogHandleToChl(LONG mhHandle);
	VIDEOCHLINFO* zogGet_SelChl();
	int zogGet_SelChlNo();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUMP4TESTDLG_H__8109FF41_7950_4188_B68C_84F8410CE982__INCLUDED_)
