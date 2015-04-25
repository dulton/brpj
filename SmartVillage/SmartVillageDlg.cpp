// SmartVillageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "SmartVillageDlg.h"
////////////////////////////////////////


#include "DLGLogin.h"
extern CDLGLogin DlgLogin;
////////////////////////////////////////

#include "DLGHistoryVideo.h" //带容器的必须放在DEBUG上面
#include "DLGHistoryCarDetect.h"
#include "DLGSetUser.h"
#include "DLGSetBlack.h"
#include "DLGHistoryReport.h"
#include "DLGSetPTZ.h"
#include "DLGSetRecord.h"
#include "DLGAnalyseFlowrate.h"

#include "DLGhelp.h"

////////////////////////////////////////
#if	ALLTAB_DETECT_CAR_MODE
#include "DLGSetCar.h"
#else
#include "DLGSetElecar.h"
#endif

#include "DLGHistoryFace.h"
#include "DLGHistoryFaceAlarmLite.h"
#include "DLGSetBlackFaceLib.h"
////////////////////////////////////////





#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

////////////////lynn/////////////////
extern CYRSVMySQL MySqlIO;
////////////////lynn/////////////////


#include "DLGpictureView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////

//用来其他类引用的主窗口类
CSmartVillageDlg *DlgMain;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmartVillageDlg dialog

CSmartVillageDlg::CSmartVillageDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSmartVillageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmartVillageDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	DlgMain=this;


}

void CSmartVillageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmartVillageDlg)

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSmartVillageDlg, CDialog)
	//{{AFX_MSG_MAP(CSmartVillageDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON() 
	ON_WM_SIZE()
	ON_COMMAND(ID_MENUITEM_LOGIN_EXIT, OnMenuitemLoginExit)
	ON_COMMAND(ID_MENUITEM_LOGIN_IN, OnMenuitemLoginIn)
	ON_COMMAND(ID_MENUITEM_LOGIN_LOCK, OnMenuitemLoginLock)
	ON_COMMAND(ID_MENUITEM_CAPBMP, OnMenuitemCapbmp)
	ON_COMMAND(ID_MENUITEM_1MODE, OnMenuitem1mode)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM_1MODE, OnUpdateMenuitem1mode)
	ON_COMMAND(ID_MENUITEM_4MODE, OnMenuitem4mode)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM_4MODE, OnUpdateMenuitem4mode)
	ON_COMMAND(ID_MENUITEM_9MODE, OnMenuitem9mode)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM_9MODE, OnUpdateMenuitem9mode)
	ON_COMMAND(ID_MENUITEM_16MODE, OnMenuitem16mode)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM_16MODE, OnUpdateMenuitem16mode)


	ON_COMMAND(ID_MENUITEM_ABOUT, OnMenuitemAbout)
	ON_COMMAND(ID_MENUITEM_HELP, OnMenuitemHelp)
	ON_COMMAND(ID_MENUITEM_SYSTEM_REPORT, OnMenuitemSystemReport)
	ON_COMMAND(ID_MENUITEM_BLACK_IN, OnMenuitemBlackIn)
	ON_COMMAND(ID_MENUITEM_BLACK_OUT, OnMenuitemBlackOut)
	ON_COMMAND(ID_MENUITEM_SET_USER, OnMenuitemSetUser)
	ON_COMMAND(ID_MENUITEM_SET_SYSTEM, OnMenuitemSetSystem)
	ON_COMMAND(ID_MENUITEM_SET_SYSTEM_IN, OnMenuitemSetSystemIn)
	ON_COMMAND(ID_MENUITEM_SET_SYSTEM_OUT, OnMenuitemSetSystemOut)
//	ON_COMMAND(ID_MENUITEM_SET_RECORD, OnMenuitemSetRecord)

//	ON_COMMAND(ID_MENUITEM_SET_PTZ, OnMenuitemSetPtz)
	ON_COMMAND(ID_MENUITEM_CARSTREAM, OnMenuitemCarstream)
	ON_COMMAND(ID_MENUITEM_LOGIN_MODIFY, OnMenuitemLoginModify)

	ON_WM_NCACTIVATE()
	ON_WM_ACTIVATE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
//	ON_COMMAND(ID_MENUITEM_HISTORY_VIDEO, OnMenuitemHistoryVideo)
	ON_COMMAND(ID_MENUITEM_HISTORY_CAR, OnMenuitemHistoryCar)
	ON_COMMAND(ID_MENUITEM_HISTORY_CARALARM, OnMenuitemHistoryCarAlarm)
	ON_COMMAND(ID_MENUITEM_SET_BLACK, OnMenuitemSetBlack)
	ON_WM_INITMENUPOPUP() //为了菜单勾选事件增加的


	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_MENUITEM_SET_FACE_BLACK, &CSmartVillageDlg::OnMenuitemSetFaceBlack)
	ON_COMMAND(ID_MENUITEM_HISTORY_FACE, &CSmartVillageDlg::OnMenuitemHistoryFace)
	ON_COMMAND(ID_MENUITEM_HISTORY_FACEALARM, &CSmartVillageDlg::OnMenuitemHistoryFacealarm)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmartVillageDlg message handlers

BOOL CSmartVillageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu="关于 "MESSAGEBOX_TITLE"(&A)...";
		//	strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	/////////////////////////////////////////////////

	DlgTab.Create(IDD_TAB_MAIN,this);
	DlgTabVideo.Create(IDD_TAB_VIDEO,this);
	DlgTabCar.Create(IDD_TAB_CAR,this);
	DlgTabFace.Create(IDD_TAB_FACE,this);


	DlgTabVideo.ShowWindow(SW_SHOW);
	DlgTab.ShowWindow(SW_SHOW);
	DlgTabCar.ShowWindow(SW_HIDE);
	DlgTabFace.ShowWindow(SW_HIDE);


	CString str;
	str.Format("%s",MESSAGEBOX_TITLE);
	DlgMain->SetWindowText(str);

	str.Format("当前登陆用户:%s",DlgLogin.CurrentUser.user);
	DlgMain->DlgTab.GetDlgItem(IDC_STATIC_USERNAME)->SetWindowText(str);

	//屏蔽菜单
	if(DlgLogin.SilentMode)
	{
		GetMenu()->GetSubMenu(1)->EnableMenuItem(0,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//客户端可以重登陆
		GetMenu()->GetSubMenu(1)->EnableMenuItem(1,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		GetMenu()->GetSubMenu(1)->EnableMenuItem(2,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}


	//改菜单背景色要的定义 放到STDAFX.H
	//#ifndef WINVER
	//#define WINVER 0x0501 
	//#endif

	//改变菜单栏颜色
	CBrush *NewBrush; 
	NewBrush =new CBrush;
	NewBrush->CreateSolidBrush(RGB(216,216,216)); 	//改变菜单栏颜色
	MENUINFO MenuInfo = {0};
	MenuInfo.cbSize = sizeof(MenuInfo); 
	MenuInfo.hbrBack = *NewBrush; 
	// 用刷子改成您想要的背景颜色
	MenuInfo.fMask = MIM_BACKGROUND|MIM_APPLYTOSUBMENUS ; 
	MenuInfo.dwStyle = MNS_AUTODISMISS;
	CMenu* pMenu = this->GetMenu(); 
	if(IsMenu(pMenu->m_hMenu)) 
	{
		SetMenuInfo(pMenu->m_hMenu,	&MenuInfo); 
	}

	// TODO: Add extra initialization here

	//初始即最大化
	ShowWindow(SW_MAXIMIZE);   

	FaceInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSmartVillageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//为了菜单勾选事件增加的
void CSmartVillageDlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items. 

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL); 

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	} 

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it. 

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		} 

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
} 

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSmartVillageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		//贴背景图	
		CRect    rect;     
		GetClientRect(&rect);     

		//从资源中载入位图     
		CBitmap    bitmap;     
		bitmap.LoadBitmap(IDB_BACK_MAIN);    
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
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSmartVillageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSmartVillageDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	//窗口最小化与窗口大小无变化不处理
	if ((cx ==0 && cy == 0) || 
		(cx == m_clientRect.Width() && cy == m_clientRect.Height())) 
	{
		return;
	}
	else
	{
		UpdatePannelPosition();
		DlgTab.AutoSize();
		DlgTabVideo.AutoSize();
		DlgTabCar.AutoSize();
		DlgTabFace.AutoSize();

		Invalidate();
	}
}

void CSmartVillageDlg::UpdatePannelPosition()
{
	GetClientRect(&m_clientRect);
	//打印日志条
	int printf_height=16;

	//窗体边距
	int distance=5;

	CRect tab_Rect;
	tab_Rect.top = 	m_clientRect.top/* + 5*/;
	tab_Rect.bottom = tab_Rect.top+36/* - 10*/;
	tab_Rect.left = m_clientRect.left /* + 10*/;
	tab_Rect.right = m_clientRect.right;
	//必须 样式=重叠，边框=调整大小
	DlgTab.MoveWindow(tab_Rect);


	//主界面
	CRect tab_win_Rect;
	tab_win_Rect.top = tab_Rect.bottom;
	tab_win_Rect.bottom = m_clientRect.bottom/* - 10*/;
	tab_win_Rect.left = m_clientRect.left/* + 10*/;
	tab_win_Rect.right = m_clientRect.right;
	//必须 样式=重叠，边框=调整大小
	DlgTabVideo.MoveWindow(tab_win_Rect);
	DlgTabCar.MoveWindow(tab_win_Rect);
	DlgTabFace.MoveWindow(tab_win_Rect);


}



void CSmartVillageDlg::FaceInit()
{
	// TODO: Add your control notification handler code here
#if OPEN_FACEDETECT_CODE
	CFaceAlarm.StartRecgThread();
	FaceMng.RWFaceMng_BlackInit();
#endif
}

void CSmartVillageDlg::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CSmartVillageDlg::OnCancel()
{
	// TODO: Add your control notification handler code here


	if(DlgLogin.SilentMode)
		DlgMain->NewLogMessage("退出");
	else
	{	
		//退出系统
		//退出框
		DlgLogin.flag=LOGIN_EXIT;
		if( IDCANCEL == DlgLogin.DoModal())
			return ;
	}
	///////////////////////////////////////


	///STOP PLAY
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		DlgTabVideo.DlgScreen.StopPlay(i);
	}

#if OPEN_FACEDETECT_CODE
	DlgTabVideo.DlgDeviceTree.faceMngTempDB.RWFaceMng_UnInit();

	CFaceAlarm.StopRecgThread();
	CFaceAlarm.RWFaceRecg_UnInit();
	FaceMng.RWFaceMng_UnInit();
#endif
	Sleep(200);
	////////////////lynn/////////////////
	MySqlIO.DisConnectionMySQL();

	DlgTabFace.Clean();


	////////////////lynn/////////////////

	DlgTabVideo.DlgPictureFaceAlarm.CleanList();
	DlgTabVideo.DlgPictureFaceCap.CleanList();

	CDialog::OnCancel();
}





//////////////菜单项///////////文件/////////////////////////////////
void CSmartVillageDlg::OnMenuitemLoginExit() 
{
	// TODO: Add your command handler code here
	//退出框
	OnCancel();
}

void CSmartVillageDlg::OnMenuitemLoginIn() 
{
	// TODO: Add your command handler code here

	//登陆框
	DlgLogin.flag=LOGIN_IN;
	DlgLogin.DoModal();

	CString str;
	str.Format("当前登陆用户:%s",DlgLogin.CurrentUser.user);
	DlgMain->DlgTab.GetDlgItem(IDC_STATIC_USERNAME)->SetWindowText(str);

}

void CSmartVillageDlg::OnMenuitemLoginLock() 
{
	// TODO: Add your command handler code here

	//锁定框
	DlgLogin.flag=LOGIN_LOCK;
	//必须登陆成功，否则锁住
	while(IDCANCEL == DlgLogin.DoModal())
		NULL;

}

void CSmartVillageDlg::OnMenuitemLoginModify() 
{
	// TODO: Add your command handler code here

	//修改密码框
	DlgLogin.flag=LOGIN_MODIFY;
	DlgLogin.DoModal();

}

//////////////菜单项///////////操作/////////////////////////////////
//抓屏
void CSmartVillageDlg::OnMenuitemCapbmp() 
{
	// TODO: Add your command handler code here
	DlgTabVideo.DlgNormal.OnButtonCapbmp();
}
////////////////////模式切换///////////
//1画面
void CSmartVillageDlg::OnMenuitem1mode() 
{
	// TODO: Add your command handler code here
	DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode1();
}

void CSmartVillageDlg::OnUpdateMenuitem1mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_1MODE==DlgTabVideo.DlgScreen.DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//4画面
void CSmartVillageDlg::OnMenuitem4mode() 
{
	// TODO: Add your command handler code here
	DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode4();
}

void CSmartVillageDlg::OnUpdateMenuitem4mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_4MODE==DlgTabVideo.DlgScreen.DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//9画面
void CSmartVillageDlg::OnMenuitem9mode() 
{
	// TODO: Add your command handler code here
	DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode9();
}

void CSmartVillageDlg::OnUpdateMenuitem9mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_9MODE==DlgTabVideo.DlgScreen.DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//16画面
void CSmartVillageDlg::OnMenuitem16mode() 
{
	// TODO: Add your command handler code here
	DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode16();
}

void CSmartVillageDlg::OnUpdateMenuitem16mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_16MODE==DlgTabVideo.DlgScreen.DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//系统日志
void CSmartVillageDlg::OnMenuitemSystemReport() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historyreport)
	{
		MessageBox("无 系统日志查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGHistoryReport DlgHistoryReport;
	DlgHistoryReport.DoModal();
}

//历史视频
void CSmartVillageDlg::OnMenuitemHistoryVideo() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historyvideo)
	{
		MessageBox("无 历史视频查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
#if OPEN_RECORD
	CDLGHistoryVideo DlgHistoryVideo;
	DlgHistoryVideo.DoModal();
#endif
}
//历史识别
void CSmartVillageDlg::OnMenuitemHistoryCar() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historydetect)
	{
		MessageBox("无 历史识别查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGHistoryCarDetect DlgHistoryCarDetect;
	DlgHistoryCarDetect.flag=HISTORY_DETECT_FLAG_CAR;
	DlgHistoryCarDetect.DoModal();

}
//历史报警
void CSmartVillageDlg::OnMenuitemHistoryCarAlarm() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historyalarm)
	{
		MessageBox("无 历史报警查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGHistoryCarDetect DlgHistoryCarDetect;
	DlgHistoryCarDetect.flag=HISTORY_DETECT_FLAG_CARALARM;
	DlgHistoryCarDetect.DoModal();
}
//黑名单导入
void CSmartVillageDlg::OnMenuitemBlackIn() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.blackset)
	{
		MessageBox("无 黑名单设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGSetBlack DlgSetBlack;
	DlgSetBlack.BlackInput();
}
//黑名单导出
void CSmartVillageDlg::OnMenuitemBlackOut() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.blackset)
	{
		MessageBox("无 黑名单设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGSetBlack DlgSetBlack;
	DlgSetBlack.BlackOuput();
}



//////////////菜单项///////////设置/////////////////////////////////
//用户设置
void CSmartVillageDlg::OnMenuitemSetUser() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.admin)
	{
		MessageBox("无 用户设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGSetUser DlgSetUser;
	DlgSetUser.DoModal();
}
//系统设置
void CSmartVillageDlg::OnMenuitemSetSystem() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.systemset)
	{
		MessageBox("无 系统设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	DlgSetSystem.DoModal();



}
//系统参数导入
void CSmartVillageDlg::OnMenuitemSetSystemIn() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.systemset)
	{
		MessageBox("无 系统设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	DlgSetSystem.InputIni();
}
//系统参数导出
void CSmartVillageDlg::OnMenuitemSetSystemOut() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.systemset)
	{
		MessageBox("无 系统设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	DlgSetSystem.OutputIni();
}
//定时录制设置
void CSmartVillageDlg::OnMenuitemSetRecord() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.recordset)
	{
		MessageBox("无 定时录像设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
#if OPEN_RECORD
	DlgTabVideo.DlgSetRecord.DoModal();
#endif
}

void CSmartVillageDlg::OnMenuitemSetrecord() 
{
	// TODO: Add your command handler code here

}

//黑名单设置
void CSmartVillageDlg::OnMenuitemSetBlack() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.blackset)
	{
		MessageBox("无 黑名单设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGSetBlack DlgSetBlack;
	DlgSetBlack.DoModal();
}
//云台设置
void CSmartVillageDlg::OnMenuitemSetPtz() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.ptz)
	{
		MessageBox("无 云台设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	int screenNo = DlgTabVideo.DlgScreen.GetCurWindId();

	CDLGSetPTZ DlgSetPtz;
	DlgTabVideo.DlgScreen.GetCurWindCamInfo(screenNo,DlgSetPtz.curCamInfo);
	DlgSetPtz.DoModal();
}
//////////////菜单项///////////分析/////////////////////////////////
//车流量
void CSmartVillageDlg::OnMenuitemCarstream() 
{
	// TODO: Add your command handler code here
	CDLGAnalyseFlowrate DlgAnalyseFlowrate;
	DlgAnalyseFlowrate.DoModal();
}
//////////////菜单项///////////关于//////////////////////////////////////
//关于
void CSmartVillageDlg::OnMenuitemAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
//帮助
void CSmartVillageDlg::OnMenuitemHelp() 
{
	// TODO: Add your command handler code here
	CDLGhelp DlgHelp;
	DlgHelp.DoModal();
}


/**********************************显示错误消息 BEGIN***********************/
void CSmartVillageDlg::NewLogMessage(char *msgStr)
{
	MySqlIO.LOG_AddNewSystemLog(DlgLogin.CurrentUser.user,msgStr);
}

//windowflag =1 弹窗警告
//windowflag =0 打印消息
void CSmartVillageDlg::ShowRuntimeMessage(char *msgStr,int windowflag)
{
	NewLogMessage(msgStr);

	if(windowflag)
		MessageBox(msgStr,MESSAGEBOX_TITLE);
	else
	{
		DlgTab.GetDlgItem(IDC_STATIC_PRINTF)->SetWindowText(msgStr);
		//增加刷新。否则叠加
		DlgTab.InvalidateRect(DlgTab.printf_Rect, TRUE);
	}
}

//摄像机消息提示
//windowflag =1 弹窗警告
//windowflag =0 打印消息
void CSmartVillageDlg::ShowCameraMessage(char *camName,char *msgStr,int windowflag)
{
	CString msg;
	msg.Format(_T("%s:  %s"),camName,msgStr);

	ShowRuntimeMessage(msg.GetBuffer(0),windowflag);
}

/**********************************显示错误消息 END***********************/

//为了窗口切换 重绘
BOOL CSmartVillageDlg::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default
	int a=CDialog::OnNcActivate(bActive);
	Invalidate();
	return a;
}
//为了窗口切换 重绘
void CSmartVillageDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	Invalidate();
	// TODO: Add your message handler code here
}

//静态文本控件 透明
HBRUSH CSmartVillageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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


void CSmartVillageDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
	UpdateData(TRUE);


}


void CSmartVillageDlg::OnMenuitemSetFaceBlack()
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.blackset)
	{
		MessageBox("无 黑名单设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGSetBlackFaceLib  DlgSetBlackFaceLib;
	DlgSetBlackFaceLib.DoModal();
}

void CSmartVillageDlg::OnMenuitemHistoryFace()
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historydetect)
	{
		MessageBox("无 识别结果查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}


	CDLGHistoryFace DlgHistoryFace;
	DlgHistoryFace.flag=HISTORY_DETECT_FLAG_FACE;

	DlgHistoryFace.DoModal();
}

void CSmartVillageDlg::OnMenuitemHistoryFacealarm()
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historyalarm)
	{
		MessageBox("无 报警结果查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGHistoryFace DlgHistoryFace;
	DlgHistoryFace.flag=HISTORY_DETECT_FLAG_FACEALARM;
	DlgHistoryFace.DoModal();
}
