// YiRongCarDetectAIODlg.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "YiRongCarDetectAIODlg.h"
////////////////////////////////////////


#include "DLGLogin.h"
extern CDLGLogin DlgLogin;
////////////////////////////////////////
#include "DLGOpenClose.h"
#include "DLGHistoryVideo.h" //带容器的必须放在DEBUG上面
#include "DLGHistroyDetect.h"
#include "DLGSetUser.h"
#include "DLGSetBlack.h"
#include "DLGHistoryReport.h"
#include "DLGSetPTZ.h"
#include "DLGSetRecord.h"
#include "DLGAnalyseFlowrate.h"
#include "DLGVideoDetect.h"
#include "DLGhelp.h"
////////////////////////////////////////
#if	ALLTAB_DETECT_CAR_MODE
#include "DLGSetCar.h"
#else
#include "DLGSetElecar.h"
#endif

#define DETECTLIST_TIMER 123

////////////////////////////////////////
#include "URLencode.h"
#include "SignalDownload.h"


//WAV声音
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib") 


#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

////////////////lynn/////////////////
extern IO OracleIO;
////////////////lynn/////////////////


#include "DLGpictureView.h"

#include "Web90server.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////

//用来其他类引用的主窗口类
CYiRongCarDetectAIODlg *DlgMain;

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
// CYiRongCarDetectAIODlg dialog

CYiRongCarDetectAIODlg::CYiRongCarDetectAIODlg(CWnd* pParent /*=NULL*/)
: CDialog(CYiRongCarDetectAIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYiRongCarDetectAIODlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	DlgMain=this;
	m_DetectListTimer=0;
	m_DetectListTimerFlag=0;
	for(int i=0;i<MAX_DEVICE_NUM;i++)
		oldnid[i]=-1;
}

void CYiRongCarDetectAIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYiRongCarDetectAIODlg)
	DDX_Control(pDX, IDC_BUTTON_TREE, m_b_tree);
	DDX_Control(pDX, IDC_BUTTON_PTZ, m_b_ptz);
	DDX_Control(pDX, IDC_BUTTON_NORMAL, m_b_normal);
	DDX_Control(pDX, IDC_LIST_CAR, m_ListCar);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CYiRongCarDetectAIODlg, CDialog)
	//{{AFX_MSG_MAP(CYiRongCarDetectAIODlg)
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
	ON_COMMAND(ID_MENUITEM_PREVIEW, OnMenuitemPreview)
	ON_COMMAND(ID_MENUITEM_DETECT, OnMenuitemDetect)
	ON_COMMAND(ID_MENUITEM_ALARM, OnMenuitemAlarm)
	ON_COMMAND(ID_MENUITEM_RECORD, OnMenuitemRecord)
	ON_COMMAND(ID_MENUITEM_ABOUT, OnMenuitemAbout)
	ON_COMMAND(ID_MENUITEM_HELP, OnMenuitemHelp)
	ON_COMMAND(ID_MENUITEM_SYSTEM_REPORT, OnMenuitemSystemReport)
	ON_COMMAND(ID_MENUITEM_BLACK_IN, OnMenuitemBlackIn)
	ON_COMMAND(ID_MENUITEM_BLACK_OUT, OnMenuitemBlackOut)
	ON_COMMAND(ID_MENUITEM_SET_USER, OnMenuitemSetUser)
	ON_COMMAND(ID_MENUITEM_SET_SYSTEM, OnMenuitemSetSystem)
	ON_COMMAND(ID_MENUITEM_SET_SYSTEM_IN, OnMenuitemSetSystemIn)
	ON_COMMAND(ID_MENUITEM_SET_SYSTEM_OUT, OnMenuitemSetSystemOut)
	ON_COMMAND(ID_MENUITEM_SET_RECORD, OnMenuitemSetRecord)
	ON_COMMAND(ID_MENUITEM_SET_CAR, OnMenuitemSetCar)
	ON_COMMAND(ID_MENUITEM_SET_PTZ, OnMenuitemSetPtz)
	ON_COMMAND(ID_MENUITEM_CARSTREAM, OnMenuitemCarstream)
	ON_COMMAND(ID_MENUITEM_LOGIN_MODIFY, OnMenuitemLoginModify)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_CAR,OnLvnItemActivateList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_CAR, OnNMCustomdrawList)
	ON_WM_NCACTIVATE()
	ON_WM_ACTIVATE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TREE, OnButtonTree)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, OnButtonNormal)
	ON_BN_CLICKED(IDC_BUTTON_PTZ, OnButtonPtz)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENUITEM_HISTORY_VIDEO, OnMenuitemHistoryVideo)
	ON_COMMAND(ID_MENUITEM_HISTORY_CAR, OnMenuitemHistoryCar)
	ON_COMMAND(ID_MENUITEM_HISTORY_ALARM, OnMenuitemHistoryAlarm)
	ON_COMMAND(ID_MENUITEM_SET_BLACK, OnMenuitemSetBlack)
	ON_WM_INITMENUPOPUP() //为了菜单勾选事件增加的
	ON_COMMAND(ID_MENUITEM_VIDEODETECT, OnMenuitemVideodetect)
	ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTER, OnMaxMinInfo)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYiRongCarDetectAIODlg message handlers

BOOL CYiRongCarDetectAIODlg::OnInitDialog()
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
	//
	DlgShortCut.Create(IDD_SHORTCUT,this);
	DlgShortCut.ShowWindow(SW_SHOW);

	DlgScreen.Create(IDD_SCREEN,this);
	DlgScreen.ShowWindow(SW_SHOW);

	TabMainInit();

	//列表
	ListMainInit();
	//初始化定时录制信息
	DlgSetRecord.InitList();

	////////////////////////////
	//分隔条
	initSplitter();

	CString str;
	str.Format("%s",MESSAGEBOX_TITLE);
	DlgMain->SetWindowText(str);

	str.Format("当前登陆用户:%s",DlgLogin.CurrentUser.user);
	DlgMain->GetDlgItem(IDC_STATIC_USERNAME)->SetWindowText(str);

	//屏蔽菜单
	if(DlgLogin.SilentMode)
	{
		GetMenu()->GetSubMenu(1)->EnableMenuItem(0,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		GetMenu()->GetSubMenu(1)->EnableMenuItem(1,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		GetMenu()->GetSubMenu(1)->EnableMenuItem(2,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}

#if ALLTAB_CLIENT_MODE

	GetMenu()->GetSubMenu(2)->EnableMenuItem(3,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(4,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(5,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(7,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(12,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);

	GetMenu()->GetSubMenu(3)->EnableMenuItem(0,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	GetMenu()->GetSubMenu(3)->EnableMenuItem(4,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	GetMenu()->GetSubMenu(3)->EnableMenuItem(7,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);

	//定时1s
	m_DetectListTimer = SetTimer(DETECTLIST_TIMER,500,NULL);
	m_DetectListTimerFlag=1;

#endif



	//改菜单背景色要的定义 放到STDAFX.H
	//#ifndef WINVER
	//#define WINVER 0x0501 
	//#endif

	//改变菜单栏颜色
	CBrush* NewBrush; 
	NewBrush = new CBrush;
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
/*
	char failstr[256]="";
	char sessionIdstr[128]="c3bf3b4f-84d5-4866-ba3e-6035ccfa9015";
	if(0)
	{
		SendSoap_InitSystem(DlgSetSystem.m_kakou_url.GetBuffer(),DlgSetSystem.m_kakou_ip.GetBuffer(),
			DlgSetSystem.m_kakou_user.GetBuffer(),DlgSetSystem.m_kakou_psw.GetBuffer(),
			sessionIdstr,failstr);
	}

	if(1)
	{
		char index_code[64]="";

		OracleIO.IVMS_ReadControlunitForSOAP(index_code);

	char crossIdstr[128]="";
	SendSoap_insertCrossingInfo(DlgSetSystem.m_kakou_url.GetBuffer(),
							sessionIdstr,
							index_code,
							"11114","路口 6电动车","101","102",
							crossIdstr,
							failstr);
	}

	if(0)
	{

	unsigned char tempdata[32]="aa";
	SendSoap_insertVehicleInfo(DlgSetSystem.m_kakou_url.GetBuffer(),
		sessionIdstr,
		"11114",
		"2014-09-22 15:06:07","22345",true,
		"",tempdata,2,
		"http://35.24.13.36:8089/picture/2014-09-22/244/2014-09-22-15-20-59 244 35.33.140.12 E0163 92 从上到下 187566 .jpg",tempdata,2,
		failstr);
	}
*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CYiRongCarDetectAIODlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CYiRongCarDetectAIODlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
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

void CYiRongCarDetectAIODlg::OnPaint() 
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
HCURSOR CYiRongCarDetectAIODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CYiRongCarDetectAIODlg::OnSize(UINT nType, int cx, int cy) 
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
		DlgDeviceTree.AutoSize();
		DlgNormal.AutoSize();
		DlgPtz.AutoSize();
		DlgScreen.AutoSize();
		DlgShortCut.AutoSize();

		Invalidate();
	}
}

void CYiRongCarDetectAIODlg::UpdatePannelPosition()
{
	GetClientRect(&m_clientRect);

	//切换栏宽度=LOGO图片的宽度
	int tab_width=220;
	//LOGO图片的高度
	//int button_top=130;
	//int button_width=192;
	//tab高度
	//int tab_top=163;

	//LOGO图片的高度
	int button_top=0;
	int button_width=192;
	//tab高度
	int tab_top=26;

	//列表的高度
	int list_height=150;
	//快捷按钮的宽度
	int shortcut_width=60;
	//打印日志条
	int printf_height=16;

	//窗体边距
	int distance=5;

	//打印日志条
	//	CRect printf_Rect;	//全局
	//	printf_Rect.top = 	m_clientRect.bottom-printf_height-distance/* + 5*/;
	//	printf_Rect.bottom = 	m_clientRect.bottom -distance/* - 10*/;
	printf_Rect.top = 	m_clientRect.bottom-printf_height/* + 5*/;
	printf_Rect.bottom = 	m_clientRect.bottom /* - 10*/;
	printf_Rect.left = m_clientRect.left +distance/* + 10*/;
	printf_Rect.right = m_clientRect.right-distance-200;
	//必须 样式=重叠，边框=调整大小
	GetDlgItem(IDC_STATIC_PRINTF)->MoveWindow(printf_Rect);


	//用户栏
	CRect user_Rect;
	user_Rect.top=printf_Rect.top;
	user_Rect.bottom=printf_Rect.bottom;
	user_Rect.left = printf_Rect.right+distance/* + 10*/;
	user_Rect.right = m_clientRect.right-distance;
	GetDlgItem(IDC_STATIC_USERNAME)->MoveWindow(user_Rect);

	//底部减去日志条
	//	m_clientRect.bottom -=printf_height+distance;
	m_clientRect.bottom -=printf_height;
/*
	//TAB按钮
	CRect button1_Rect;
	button1_Rect.top = m_clientRect.top+distance+button_top;
	button1_Rect.bottom = m_clientRect.top+tab_top;
	button1_Rect.left = m_clientRect.left+distance;
	button1_Rect.right = m_clientRect.left+button_width/3+distance;
	GetDlgItem(IDC_BUTTON_TREE)->MoveWindow(button1_Rect);

	CRect button2_Rect;
	button2_Rect.top = m_clientRect.top+distance+button_top;
	button2_Rect.bottom = m_clientRect.top+tab_top;
	button2_Rect.left = 	button1_Rect.right;
	button2_Rect.right = m_clientRect.left+button_width/3*2+distance;
	GetDlgItem(IDC_BUTTON_NORMAL)->MoveWindow(button2_Rect);

	CRect button3_Rect;
	button3_Rect.top = m_clientRect.top+distance+button_top;
	button3_Rect.bottom = m_clientRect.top+tab_top;
	button3_Rect.left = 	button2_Rect.right;
	button3_Rect.right = m_clientRect.left+button_width+distance;
	GetDlgItem(IDC_BUTTON_PTZ)->MoveWindow(button3_Rect);
*/
	//TAB按钮
	CRect button1_Rect;
	button1_Rect.top = m_clientRect.top+button_top;
	button1_Rect.bottom = m_clientRect.top+tab_top;
	button1_Rect.left = m_clientRect.left+distance;
	button1_Rect.right = button1_Rect.left +76;
	GetDlgItem(IDC_BUTTON_TREE)->MoveWindow(button1_Rect);

	CRect button2_Rect;
	button2_Rect.top = m_clientRect.top+button_top;
	button2_Rect.bottom = m_clientRect.top+tab_top;
	button2_Rect.left = 	button1_Rect.right;
	button2_Rect.right =	button2_Rect.left+69;
	GetDlgItem(IDC_BUTTON_NORMAL)->MoveWindow(button2_Rect);

	CRect button3_Rect;
	button3_Rect.top = m_clientRect.top+button_top;
	button3_Rect.bottom = m_clientRect.top+tab_top;
	button3_Rect.left = 	button2_Rect.right;
	button3_Rect.right =button3_Rect.left+76;
	GetDlgItem(IDC_BUTTON_PTZ)->MoveWindow(button3_Rect);

	//切换栏
	CRect tab_Rect;
	tab_Rect.top = m_clientRect.top+tab_top;
	tab_Rect.bottom = m_clientRect.bottom-distance;
	tab_Rect.left = m_clientRect.left+distance;
	tab_Rect.right = m_clientRect.left+tab_width+distance;
	//必须 样式=重叠，边框=调整大小
	//	m_TabMain.MoveWindow(tab_Rect);

	DlgDeviceTree.MoveWindow(tab_Rect);
	DlgNormal.MoveWindow(tab_Rect);
	DlgPtz.MoveWindow(tab_Rect);

	//列表
	CRect list_Rect;
	list_Rect.top = m_clientRect.bottom-distance-list_height/* + 5*/;
	list_Rect.bottom = m_clientRect.bottom-distance/* - 10*/;
	list_Rect.left = tab_Rect.right+distance/* + 10*/;
	list_Rect.right = m_clientRect.right-distance-shortcut_width;
	//必须 样式=重叠，边框=调整大小
	m_ListCar.MoveWindow(list_Rect);

	//分屏
	CRect screen_Rect;
	screen_Rect.top = m_clientRect.top/* + 5*/;
	screen_Rect.bottom = list_Rect.top -distance/* - 10*/;
	screen_Rect.left = tab_Rect.right +distance/* + 10*/;
	screen_Rect.right = list_Rect.right;
	//必须 样式=重叠，边框=调整大小
	DlgScreen.MoveWindow(screen_Rect);

	//分隔条
	CRect rcSplit;

	rcSplit.top = screen_Rect.bottom;
	rcSplit.bottom = list_Rect.top;
	rcSplit.left = list_Rect.left;
	rcSplit.right =  list_Rect.right;
	//GetDlgItem(IDC_SPLITTER)->MoveWindow(rcSplit);
    m_wndSplitter.MoveWindow(rcSplit);

	//快捷按钮
	CRect shortcut_Rect;
	shortcut_Rect.top = m_clientRect.top/* + 5*/;
	shortcut_Rect.bottom = m_clientRect.bottom-distance/* - 10*/;
	shortcut_Rect.left = list_Rect.right+distance/* + 10*/;
	shortcut_Rect.right = m_clientRect.right;
	//必须 样式=重叠，边框=调整大小
	DlgShortCut.MoveWindow(shortcut_Rect);



}

void CYiRongCarDetectAIODlg::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CYiRongCarDetectAIODlg::OnCancel()
{
	// TODO: Add your control notification handler code here
	if(m_DetectListTimer) 
		KillTimer(m_DetectListTimer); 
	m_DetectListTimer = 0;
	m_DetectListTimerFlag=0;
	/*
	#if ALLTAB_LOGIN_WIN_MODE
	//退出系统
	//退出框
	DlgLogin.flag=LOGIN_EXIT;
	if( IDCANCEL == DlgLogin.DoModal())
	return ;
	#endif
	*/
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

	///STOP PLAY
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		DlgScreen.StopPlay(i);
	}
	Sleep(200);
	////////////////lynn/////////////////
	OracleIO.DisConnectionOracleDB();

#if YRVM_PINGTAI_MODE
	OracleIO.YRVM_DisConnectionOracleDB();
#endif

#if IVMS_ORACLE_DEVICETREE
	OracleIO.IVMS_DisConnectionOracleDB();
#endif


	Sleep(200);
	////////////////lynn/////////////////




	CDialog::OnCancel();
}


//初始化切换栏
void CYiRongCarDetectAIODlg::TabMainInit(void)
{
	DlgDeviceTree.Create(IDD_DEVICE_TREE,this);
	DlgNormal.Create(IDD_NORMAL,this);
	DlgPtz.Create(IDD_PTZ,this);

	m_b_tree.LoadBitmaps(IDB_TAB_TREE,NULL,NULL,NULL);
	m_b_tree.SizeToContent();		//自适应图片大小

	m_b_ptz.LoadBitmaps(IDB_TAB_PTZ,NULL,NULL,NULL);
	m_b_ptz.SizeToContent();		//自适应图片大小

	m_b_normal.LoadBitmaps(IDB_TAB_NORMAL,NULL,NULL,NULL);
	m_b_normal.SizeToContent();		//自适应图片大小

	OnButtonTree();	
}

void CYiRongCarDetectAIODlg::OnButtonTree() 
{
	// TODO: Add your control notification handler code here
	DlgDeviceTree.ShowWindow(SW_SHOW);
	DlgNormal.ShowWindow(SW_HIDE);
	DlgPtz.ShowWindow(SW_HIDE);

	m_b_tree.LoadBitmaps(IDB_TAB_TREE,NULL,NULL,NULL);
	m_b_tree.SizeToContent();		//自适应图片大小
	m_b_tree.Invalidate();
	m_b_ptz.LoadBitmaps(IDB_TAB_PTZ_DISA,NULL,NULL,NULL);
	m_b_ptz.SizeToContent();		//自适应图片大小
	m_b_ptz.Invalidate();
	m_b_normal.LoadBitmaps(IDB_TAB_NORMAL_DISA,NULL,NULL,NULL);
	m_b_normal.SizeToContent();		//自适应图片大小
	m_b_normal.Invalidate();
}

void CYiRongCarDetectAIODlg::OnButtonNormal() 
{
	// TODO: Add your control notification handler code here
	DlgDeviceTree.ShowWindow(SW_HIDE);
	DlgNormal.ShowWindow(SW_SHOW);
	//lynn
	DlgNormal.UpdateNormalWnd();

	DlgPtz.ShowWindow(SW_HIDE);

	m_b_tree.LoadBitmaps(IDB_TAB_TREE_DISA,NULL,NULL,NULL);
		m_b_tree.SizeToContent();		//自适应图片大小
	m_b_tree.Invalidate();
	m_b_ptz.LoadBitmaps(IDB_TAB_PTZ_DISB,NULL,NULL,NULL);
		m_b_ptz.SizeToContent();		//自适应图片大小
	m_b_ptz.Invalidate();
	m_b_normal.LoadBitmaps(IDB_TAB_NORMAL,NULL,NULL,NULL);
		m_b_normal.SizeToContent();		//自适应图片大小
	m_b_normal.Invalidate();
}

void CYiRongCarDetectAIODlg::OnButtonPtz() 
{
	// TODO: Add your control notification handler code here

	DlgDeviceTree.ShowWindow(SW_HIDE);
	DlgNormal.ShowWindow(SW_HIDE);
	DlgPtz.ShowWindow(SW_SHOW);

	m_b_tree.LoadBitmaps(IDB_TAB_TREE_DISB,NULL,NULL,NULL);
			m_b_tree.SizeToContent();		//自适应图片大小
	m_b_tree.Invalidate();
	m_b_ptz.LoadBitmaps(IDB_TAB_PTZ,NULL,NULL,NULL);
			m_b_ptz.SizeToContent();		//自适应图片大小
	m_b_ptz.Invalidate();
	m_b_normal.LoadBitmaps(IDB_TAB_NORMAL_DISB,NULL,NULL,NULL);
			m_b_normal.SizeToContent();		//自适应图片大小
	m_b_normal.Invalidate();
}
/*

//初始化切换栏
void CYiRongCarDetectAIODlg::TabMainInit(void)
{
m_TabMain.InsertItem(0,"设备");
m_TabMain.InsertItem(1,"常规");
m_TabMain.InsertItem(2,"云台");

DlgDeviceTree.Create(IDD_DEVICE_TREE,&m_TabMain);
DlgNormal.Create(IDD_NORMAL,&m_TabMain);
DlgPtz.Create(IDD_PTZ,&m_TabMain);
OnSelchangeTabMain(NULL,NULL);
}

//切换 切换栏
void CYiRongCarDetectAIODlg::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
UpdateData(TRUE);
// TODO: Add your control notification handler code here
int i=m_TabMain.GetCurSel();

switch(i)
{
case 0:
DlgDeviceTree.ShowWindow(SW_SHOW);
DlgNormal.ShowWindow(SW_HIDE);
DlgPtz.ShowWindow(SW_HIDE);
break;
case 1:
DlgDeviceTree.ShowWindow(SW_HIDE);
DlgNormal.ShowWindow(SW_SHOW);
//lynn
DlgNormal.UpdateNormalWnd();

DlgPtz.ShowWindow(SW_HIDE);
break;
case 2:
DlgDeviceTree.ShowWindow(SW_HIDE);
DlgNormal.ShowWindow(SW_HIDE);
DlgPtz.ShowWindow(SW_SHOW);
break;
default:
break;
}
}
*/

//////////////菜单项///////////文件/////////////////////////////////
void CYiRongCarDetectAIODlg::OnMenuitemLoginExit() 
{
	// TODO: Add your command handler code here
	//退出框
	OnCancel();
}

void CYiRongCarDetectAIODlg::OnMenuitemLoginIn() 
{
	// TODO: Add your command handler code here
#if ALLTAB_LOGIN_WIN_MODE
	//登陆框
	DlgLogin.flag=LOGIN_IN;
	DlgLogin.DoModal();

	CString str;
	str.Format("当前登陆用户:%s",DlgLogin.CurrentUser.user);
	DlgMain->GetDlgItem(IDC_STATIC_USERNAME)->SetWindowText(str);
#endif
}

void CYiRongCarDetectAIODlg::OnMenuitemLoginLock() 
{
	// TODO: Add your command handler code here
#if ALLTAB_LOGIN_WIN_MODE
	//锁定框
	DlgLogin.flag=LOGIN_LOCK;
	//必须登陆成功，否则锁住
	while(IDCANCEL == DlgLogin.DoModal())
		NULL;
#endif
}

void CYiRongCarDetectAIODlg::OnMenuitemLoginModify() 
{
	// TODO: Add your command handler code here
#if ALLTAB_LOGIN_WIN_MODE
	//修改密码框
	DlgLogin.flag=LOGIN_MODIFY;
	DlgLogin.DoModal();
#endif
}

//////////////菜单项///////////操作/////////////////////////////////
//抓屏
void CYiRongCarDetectAIODlg::OnMenuitemCapbmp() 
{
	// TODO: Add your command handler code here
	DlgNormal.OnButtonCapbmp();
}
////////////////////模式切换///////////
//1画面
void CYiRongCarDetectAIODlg::OnMenuitem1mode() 
{
	// TODO: Add your command handler code here
	DlgShortCut.OnButtonMode1();
}

void CYiRongCarDetectAIODlg::OnUpdateMenuitem1mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_1MODE==DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//4画面
void CYiRongCarDetectAIODlg::OnMenuitem4mode() 
{
	// TODO: Add your command handler code here
	DlgShortCut.OnButtonMode4();
}

void CYiRongCarDetectAIODlg::OnUpdateMenuitem4mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_4MODE==DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//9画面
void CYiRongCarDetectAIODlg::OnMenuitem9mode() 
{
	// TODO: Add your command handler code here
	DlgShortCut.OnButtonMode9();
}

void CYiRongCarDetectAIODlg::OnUpdateMenuitem9mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_9MODE==DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//16画面
void CYiRongCarDetectAIODlg::OnMenuitem16mode() 
{
	// TODO: Add your command handler code here
	DlgShortCut.OnButtonMode16();
}

void CYiRongCarDetectAIODlg::OnUpdateMenuitem16mode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(SCREEN_16MODE==DlgShortCut.screenmode)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//开启关闭预览
void CYiRongCarDetectAIODlg::OnMenuitemPreview() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.preview)
	{
		MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGOpenClose DlgOpenClose;
	DlgOpenClose.flag=OPENCLOSE_PREVIEW;
	DlgOpenClose.DoModal();
}
//开启关闭识别
void CYiRongCarDetectAIODlg::OnMenuitemDetect() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGOpenClose DlgOpenClose;
	DlgOpenClose.flag=OPENCLOSE_DETECT;
	DlgOpenClose.DoModal();
}
//开启关闭报警
void CYiRongCarDetectAIODlg::OnMenuitemAlarm() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.detect)
	{
		MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGOpenClose DlgOpenClose;
	DlgOpenClose.flag=OPENCLOSE_ALARM;
	DlgOpenClose.DoModal();
}
//开启关闭录制
void CYiRongCarDetectAIODlg::OnMenuitemRecord() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.record)
	{
		MessageBox("无 录像 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGOpenClose DlgOpenClose;
	DlgOpenClose.flag=OPENCLOSE_RECORD;
	DlgOpenClose.DoModal();
}
//系统日志
void CYiRongCarDetectAIODlg::OnMenuitemSystemReport() 
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
void CYiRongCarDetectAIODlg::OnMenuitemHistoryVideo() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historyvideo)
	{
		MessageBox("无 历史视频查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGHistoryVideo DlgHistoryVideo;
	DlgHistoryVideo.DoModal();
}
//历史识别
void CYiRongCarDetectAIODlg::OnMenuitemHistoryCar() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historydetect)
	{
		MessageBox("无 历史识别查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGHistroyDetect DlgHistroyDetect;
	DlgHistroyDetect.flag=HISTORY_DETECT_FLAG_CAR;
	DlgHistroyDetect.DoModal();

}
//历史报警
void CYiRongCarDetectAIODlg::OnMenuitemHistoryAlarm() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.historyalarm)
	{
		MessageBox("无 历史报警查询 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CDLGHistroyDetect DlgHistroyDetect;
	DlgHistroyDetect.flag=HISTORY_DETECT_FLAG_ALARM;
	DlgHistroyDetect.DoModal();
}
//黑名单导入
void CYiRongCarDetectAIODlg::OnMenuitemBlackIn() 
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
void CYiRongCarDetectAIODlg::OnMenuitemBlackOut() 
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

void CYiRongCarDetectAIODlg::OnMenuitemVideodetect() 
{
	// TODO: Add your command handler code here
	CDLGVideoDetect DlgVideoDetect;
	DlgVideoDetect.DoModal();
}

//////////////菜单项///////////设置/////////////////////////////////
//用户设置
void CYiRongCarDetectAIODlg::OnMenuitemSetUser() 
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
void CYiRongCarDetectAIODlg::OnMenuitemSetSystem() 
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
void CYiRongCarDetectAIODlg::OnMenuitemSetSystemIn() 
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
void CYiRongCarDetectAIODlg::OnMenuitemSetSystemOut() 
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
void CYiRongCarDetectAIODlg::OnMenuitemSetRecord() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.recordset)
	{
		MessageBox("无 定时录像设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	DlgSetRecord.DoModal();
}

void CYiRongCarDetectAIODlg::OnMenuitemSetrecord() 
{
	// TODO: Add your command handler code here

}
//车牌识别设置
void CYiRongCarDetectAIODlg::OnMenuitemSetCar() 
{
	// TODO: Add your command handler code here
	DlgNormal.OnButtonSetCar();
}

//黑名单设置
void CYiRongCarDetectAIODlg::OnMenuitemSetBlack() 
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
void CYiRongCarDetectAIODlg::OnMenuitemSetPtz() 
{
	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.ptz)
	{
		MessageBox("无 云台设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	int screenNo = DlgScreen.GetCurWindId();

	CDLGSetPTZ DlgSetPtz;
	DlgScreen.GetCurWindCamInfo(screenNo,DlgSetPtz.curCamInfo);
	DlgSetPtz.DoModal();
}
//////////////菜单项///////////分析/////////////////////////////////
//车流量
void CYiRongCarDetectAIODlg::OnMenuitemCarstream() 
{
	// TODO: Add your command handler code here
	CDLGAnalyseFlowrate DlgAnalyseFlowrate;
	DlgAnalyseFlowrate.DoModal();
}
//////////////菜单项///////////关于//////////////////////////////////////
//关于
void CYiRongCarDetectAIODlg::OnMenuitemAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
//帮助
void CYiRongCarDetectAIODlg::OnMenuitemHelp() 
{
	// TODO: Add your command handler code here
	CDLGhelp DlgHelp;
	DlgHelp.DoModal();
}

//////////////////////////////////////////////////////////////////////////
void CYiRongCarDetectAIODlg::ListMainInit(void)
{
	m_ListCar.SetBkColor(RGB(248,248,248));
#if ALLTAB_DETECT_CAR_MODE

	m_ListCar.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 40);
	m_ListCar.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_ListCar.InsertColumn(2, _T("摄像头名称" ), LVCFMT_LEFT, 140);
	m_ListCar.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 100);
	m_ListCar.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(7, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(8, _T("车牌类型"), LVCFMT_LEFT, 100);
	m_ListCar.InsertColumn(9, _T("车身颜色"), LVCFMT_LEFT, 60);
	m_ListCar.InsertColumn(10, _T("黑名单"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(11, _T("图片路径"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(12, _T("服务器模式"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(13, _T("NID"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(14, _T("图片大小"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(15, _T("CAMID"), LVCFMT_LEFT, 0);
#else

	m_ListCar.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 40);
	m_ListCar.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_ListCar.InsertColumn(2, _T("摄像头名称" ), LVCFMT_LEFT, 160);
	m_ListCar.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 100);
	m_ListCar.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 60);
	m_ListCar.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(7, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(8, _T("黑名单"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(9, _T("图片路径"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(10, _T("服务器模式"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(11, _T("NID"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(12, _T("图片大小"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(13, _T("CAMID"), LVCFMT_LEFT, 0);

#endif
	m_ListCar.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListCarTotal=0;
}

//双击显示大图
void CYiRongCarDetectAIODlg::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[ZOG_MAX_PATH_STR];

#if ALLTAB_CLIENT_MODE

#if OPEN_TOMCAT_MODE
	DisplayTomcatPic(pNMIA->iItem);
#else
	//仅客户端+数据库
	if(DlgSetSystem.m_check_savenet)
		DisplayNetPic(pNMIA->iItem);
#endif


#else
	//服务器通用

#if ALLTAB_DETECT_CAR_MODE
	//汽车
	m_ListCar.GetItemText(pNMIA->iItem,11,str,ZOG_MAX_PATH_STR);
#else
	//电动车
	m_ListCar.GetItemText(pNMIA->iItem,9,str,ZOG_MAX_PATH_STR);
#endif

	//ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	char str2[ZOG_MAX_PATH_STR];

	CDLGpictureView dlgPicView;
	//摄像头名称
	m_ListCar.GetItemText(pNMIA->iItem,2,str2,ZOG_MAX_PATH_STR);

	dlgPicView.Titlestr=str2;

	char *p=strrchr(str,'\\');
	if(p!=NULL)
		p++;
	else
		return ;

	dlgPicView.Titlestr+=p;

	dlgPicView.srcfile=str;
	dlgPicView.DoModal();

#endif

	*pResult = 0;
}
//需要NET列表 为NID
void CYiRongCarDetectAIODlg::DisplayNetPic(int iItem)
{
	char str[ZOG_MAX_PATH_STR];

	unsigned long int nid;
	unsigned long int size;
	bool tempget;

	unsigned char *data=NULL;

#if ALLTAB_DETECT_CAR_MODE
	//汽车
	m_ListCar.GetItemText(iItem,13,str,ZOG_MAX_PATH_STR);
#else
	//电动车
	m_ListCar.GetItemText(iItem,11,str,ZOG_MAX_PATH_STR);
#endif

	sscanf(str,"%d",&nid);

#if ALLTAB_DETECT_CAR_MODE
	//汽车
	m_ListCar.GetItemText(iItem,14,str,ZOG_MAX_PATH_STR);
#else
	//电动车
	m_ListCar.GetItemText(iItem,12,str,ZOG_MAX_PATH_STR);
#endif

	sscanf(str,"%d",&size);
	//获取数据 保存到临时文件
	data=(unsigned char *)calloc(size,sizeof(unsigned char));

#if ALLTAB_DETECT_CAR_MODE
	//汽车
	tempget=OracleIO.CAR_MatchResult_GetPicture(nid,data);
#else
	//电动车
	tempget=OracleIO.ELECAR_MatchResult_GetPicture(nid,data);
#endif

	sprintf(str,"%s\\yrcdtempResultListpic.jpg",DlgSetSystem.m_path_detect);

	if(tempget)
	{
		FILE *fp=NULL;

		fp=fopen(str,"wb");
		if(fp)
		{
			fwrite(data,size,1,fp);
			fclose(fp);
			Sleep(100);
			//保存后打开
			ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

		}
	}

	free(data);
	data=NULL;

}

//需要NET列表 为NID
void CYiRongCarDetectAIODlg::DisplayTomcatPic(int iItem)
{
	char url[1024];
	char url2[1024];
	char str[ZOG_MAX_PATH_STR];
	char fail[1024]="";

#if ALLTAB_DETECT_CAR_MODE
	//汽车
	m_ListCar.GetItemText(iItem,11,url,ZOG_MAX_PATH_STR);
#else
	//电动车
	m_ListCar.GetItemText(iItem,9,url,ZOG_MAX_PATH_STR);
#endif

	sprintf(str,"%s\\yrcdtempResultListpic.jpg",DlgSetSystem.m_path_detect);

	SignalDownload sd;
	sd.InitData();
	EncodeURI(url,url2,1024);

	if(false == sd.HTTPDownload(url2,str,fail,10,0))
	{
		sd.DestroyData();
		return ;
	}

	sd.DestroyData();
	Sleep(100);
	//保存后打开
	//ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	CDLGpictureView dlgPicView;
	//摄像头名称
	m_ListCar.GetItemText(iItem,2,url2,ZOG_MAX_PATH_STR);

	dlgPicView.Titlestr=url2;

	char *p=strrchr(url,'/');
	if(p!=NULL)
		p++;
	else
		return ;

	dlgPicView.Titlestr+=p;

	dlgPicView.srcfile=str;
	dlgPicView.DoModal();


}

//NMTVCUSTOMDRAW
//高亮
void CYiRongCarDetectAIODlg::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)pNMHDR;
	*pResult=CDRF_DODEFAULT;

	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			*pResult=CDRF_NOTIFYITEMDRAW;
			return;
		}
	case CDDS_ITEMPREPAINT:
		{
			*pResult=CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		{
			char str[260];
			COLORREF clrNewTextColor,clrNewBkColor;
			int nItem=static_cast<int>(lplvcd->nmcd.dwItemSpec);

#if ALLTAB_DETECT_CAR_MODE
			//汽车
			m_ListCar.GetItemText(nItem,10,str,260);
#else
			//电动车
			m_ListCar.GetItemText(nItem,8,str,260);
#endif
			lplvcd->clrTextBk=RGB(248,248,248);
			if(NULL!=strstr(str,"是"))
			{
				clrNewBkColor=RGB(255,0,0);//红底白字
				lplvcd->clrTextBk=clrNewBkColor;
				clrNewTextColor=RGB(255,255,255);	
				lplvcd->clrText=clrNewTextColor;
			}
			*pResult=CDRF_DODEFAULT;
			return;
		}
	}
}

/**********************************显示错误消息 BEGIN***********************/
void CYiRongCarDetectAIODlg::NewLogMessage(char *msgStr)
{
	OracleIO.LOG_AddNewSystemLog(DlgLogin.CurrentUser.user,msgStr);
}

//windowflag =1 弹窗警告
//windowflag =0 打印消息
void CYiRongCarDetectAIODlg::ShowRuntimeMessage(char *msgStr,int windowflag)
{
	NewLogMessage(msgStr);

	if(windowflag)
		MessageBox(msgStr,MESSAGEBOX_TITLE);
	else
	{
		GetDlgItem(IDC_STATIC_PRINTF)->SetWindowText(msgStr);
		//增加刷新。否则叠加
		InvalidateRect(printf_Rect, TRUE);
	}
}

//摄像机消息提示
//windowflag =1 弹窗警告
//windowflag =0 打印消息
void CYiRongCarDetectAIODlg::ShowCameraMessage(char *camName,char *msgStr,int windowflag)
{
	CString msg;
	msg.Format(_T("%s:  %s"),camName,msgStr);

	ShowRuntimeMessage(msg.GetBuffer(0),windowflag);
}

/**********************************显示错误消息 END***********************/

//为了窗口切换 重绘
BOOL CYiRongCarDetectAIODlg::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default
	int a=CDialog::OnNcActivate(bActive);
	Invalidate();
	return a;
}
//为了窗口切换 重绘
void CYiRongCarDetectAIODlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	Invalidate();
	// TODO: Add your message handler code here
}

//静态文本控件 透明
HBRUSH CYiRongCarDetectAIODlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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


void CYiRongCarDetectAIODlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	char fail[256];
	CDialog::OnTimer(nIDEvent);
	UpdateData(TRUE);

#if ALLTAB_CLIENT_MODE

	if(nIDEvent == DETECTLIST_TIMER && m_DetectListTimerFlag)
	{
		int res;
		//只有正在预览的才推送
		for(int j=0;j<MAX_DEVICE_NUM;j++)
		{
			if(DlgScreen.m_videoInfo[j].isplay && DlgScreen.m_videoInfo[j].camID >=0)
			{

#if ALLTAB_DETECT_CAR_MODE
				res=OracleIO.CAR_MatchResult_ReadforListOne(DlgScreen.m_videoInfo[j].camID ,&HistoryClientDATA);
				//(OracleIO.CAR_MatchResult_ReadforListAll(&HistoryClientDATA))

				if(1==res)
				{
					if(oldnid[j] !=HistoryClientDATA.nid)
					{
						oldnid[j] =HistoryClientDATA.nid;

						char str[260];
						int nItem;

						//插入列表
						if(m_ListCarTotal >=200)
						{
							m_ListCar.DeleteAllItems();
							m_ListCarTotal=0;
						}

						m_ListCarTotal++;
						sprintf(str,"%d",m_ListCarTotal);
						nItem = m_ListCar.InsertItem(0,str);

						sprintf(HistoryClientTimeformat,"%04d-%02d-%02d %02d:%02d:%02d",
							HistoryClientDATA.year,
							HistoryClientDATA.mon,
							HistoryClientDATA.day,
							HistoryClientDATA.hour,
							HistoryClientDATA.min,
							HistoryClientDATA.sec);

						m_ListCar.SetItemText(nItem,1,HistoryClientTimeformat);
						m_ListCar.SetItemText(nItem,2,HistoryClientDATA.name);
						m_ListCar.SetItemText(nItem,3,HistoryClientDATA.ip);
						m_ListCar.SetItemText(nItem,4,HistoryClientDATA.plate);

						sprintf(str,"%d",HistoryClientDATA.reliability);
						m_ListCar.SetItemText(nItem,5,str);

						m_ListCar.SetItemText(nItem,6,HistoryClientDATA.direction);
						m_ListCar.SetItemText(nItem,7,	HistoryClientDATA.platecolor);
						m_ListCar.SetItemText(nItem,8,	HistoryClientDATA.platetype);
						m_ListCar.SetItemText(nItem,9,	HistoryClientDATA.carcolor);
						if(HistoryClientDATA.nflag)
						{	
							m_ListCar.SetItemText(nItem,10,	"是");
								if(DlgSetSystem.m_check_alarmpic)
							{
								sprintf(str,"%s\\yrcdtempResultListpic.jpg",DlgSetSystem.m_path_detect);

								SignalDownload sd;
								sd.InitData();
								char url2[1024];
								EncodeURI(HistoryClientDATA.path,url2,1024);
								sd.HTTPDownload(url2,str,fail,10,0);
								sd.DestroyData();
								Sleep(100);
								//保存后打开
								ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);
							}
							//报警声音
							if(DlgSetSystem.m_check_alarmwav)
								PlaySound(DlgSetSystem.m_path_alarmwav,NULL,SND_FILENAME | SND_ASYNC);
						}
						else
							m_ListCar.SetItemText(nItem,10,	"否");

						m_ListCar.SetItemText(nItem,11,	HistoryClientDATA.path);

						m_ListCar.SetItemText(nItem,12,	"是");

						sprintf(str,"%d",HistoryClientDATA.nid);
						m_ListCar.SetItemText(nItem,13,	str);

						sprintf(str,"%d",HistoryClientDATA.picsize);
						m_ListCar.SetItemText(nItem,14,	str);

						sprintf(str,"%d",HistoryClientDATA.ncamid);
						m_ListCar.SetItemText(nItem,15,	str);
					}
				}
				else if(-1==res)
				{
					m_DetectListTimerFlag=0;
					break;
				}
#else
				res=OracleIO.ELECAR_MatchResult_ReadforlistOne(DlgScreen.m_videoInfo[j].camID ,&HistoryClientDATA);
				//res=OracleIO.ELECAR_MatchResult_ReadforlistAll(&HistoryClientDATA);
				if(1==res)
				{
					if(oldnid[j] !=HistoryClientDATA.nid)
					{
						oldnid[j] =HistoryClientDATA.nid;

						char str[260];
						int nItem;

						//插入列表
						if(m_ListCarTotal >=200)
						{
							m_ListCar.DeleteAllItems();
							m_ListCarTotal=0;
						}

						m_ListCarTotal++;
						sprintf(str,"%d",m_ListCarTotal);
						nItem = m_ListCar.InsertItem(0,str);

						sprintf(HistoryClientTimeformat,"%04d-%02d-%02d %02d:%02d:%02d",
							HistoryClientDATA.year,
							HistoryClientDATA.mon,
							HistoryClientDATA.day,
							HistoryClientDATA.hour,
							HistoryClientDATA.min,
							HistoryClientDATA.sec);

						m_ListCar.SetItemText(nItem,1,HistoryClientTimeformat);
						m_ListCar.SetItemText(nItem,2,HistoryClientDATA.name);
						m_ListCar.SetItemText(nItem,3,HistoryClientDATA.ip);
						m_ListCar.SetItemText(nItem,4,HistoryClientDATA.plate);

						sprintf(str,"%d",HistoryClientDATA.reliability);
						m_ListCar.SetItemText(nItem,5,str);

						m_ListCar.SetItemText(nItem,6,HistoryClientDATA.direction);
						m_ListCar.SetItemText(nItem,7,	HistoryClientDATA.platecolor);

						if(HistoryClientDATA.nflag)
						{
							m_ListCar.SetItemText(nItem,8,	"是");
					
							if(DlgSetSystem.m_check_alarmpic)
							{
								sprintf(str,"%s\\yrcdtempResultListpic.jpg",DlgSetSystem.m_path_detect);

								SignalDownload sd;
								sd.InitData();
								char url2[1024];
								EncodeURI(HistoryClientDATA.path,url2,1024);
								sd.HTTPDownload(url2,str,fail,10,0);
								sd.DestroyData();
								Sleep(100);
			
								//保存后打开 //报警不用窗口来开
								ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);
							}
							//报警声音
							if(DlgSetSystem.m_check_alarmwav)
								PlaySound(DlgSetSystem.m_path_alarmwav,NULL,SND_FILENAME | SND_ASYNC);
						}
						else
							m_ListCar.SetItemText(nItem,8,	"否");

						m_ListCar.SetItemText(nItem,9,	HistoryClientDATA.path);

						m_ListCar.SetItemText(nItem,10,	"是");

						sprintf(str,"%d",HistoryClientDATA.nid);
						m_ListCar.SetItemText(nItem,11,	str);

						sprintf(str,"%d",HistoryClientDATA.picsize);
						m_ListCar.SetItemText(nItem,12,	str);

						sprintf(str,"%d",HistoryClientDATA.ncamid);
						m_ListCar.SetItemText(nItem,13,	str);
					}
				}
				else if(-1==res)
				{
					m_DetectListTimerFlag=0;
					break;
				}
#endif
			}
		}
		UpdateData(FALSE);
		if(	0== m_DetectListTimerFlag)
		{
			for(int i=0;i<MAX_DEVICE_NUM;i++)
			{
				DlgScreen.StopPlay(i);
			}
			AfxMessageBox(OracleIO.errormessage);
			CDialog::OnCancel();
			return ;
		}
	}
#endif
}

void CYiRongCarDetectAIODlg::initSplitter(void)
{
	CRect rc;
	CWnd* pWnd;

	pWnd = GetDlgItem(IDC_SPLITTER);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	BOOL bRet = m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER, SPS_HORIZONTAL, RGB(0, 0, 255));
	if (FALSE == bRet)
	{
		AfxMessageBox("m_wndSplitter create failed");
	}

	m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_UP,   &(DlgScreen));
	m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_DOWN,  GetDlgItem(IDC_LIST_CAR));

	//  relayout the splotter to make them good look
	m_wndSplitter.Relayout();

}

void CYiRongCarDetectAIODlg::OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	//  Get current pos of the child controls
	CRect rcScreen;
	CRect rcList;

	DlgScreen.GetWindowRect(rcScreen);
	m_ListCar.GetWindowRect(rcList);

	this->ScreenToClient(rcScreen);
	this->ScreenToClient(rcList);

	//  return the pos limit
	SPC_NM_MAXMINPOS* pNewMaxMinPos = (SPC_NM_MAXMINPOS*)pNMHDR;
	if (IDC_SPLITTER == pNMHDR->idFrom)
	{
		pNewMaxMinPos->lMin = rcScreen.top + 300;
		pNewMaxMinPos->lMax = rcList.bottom - 155;
	}
}