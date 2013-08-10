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

////////////////////////////////////////
#if	ALLTAB_DETECT_CAR_MODE
	#include "DLGSetCar.h"
#else
	#include "DLGSetElecar.h"
#endif

////////////////////////////////////////

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

////////////////lynn/////////////////
extern IO OracleIO;
////////////////lynn/////////////////

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
	ON_COMMAND(ID_MENUITEM_HISTORY_VIDEO, OnMenuitemHistoryVideo)
	ON_COMMAND(ID_MENUITEM_HISTORY_CAR, OnMenuitemHistoryCar)
	ON_COMMAND(ID_MENUITEM_HISTORY_ALARM, OnMenuitemHistoryAlarm)
	ON_COMMAND(ID_MENUITEM_SET_BLACK, OnMenuitemSetBlack)
	ON_WM_INITMENUPOPUP() //为了菜单勾选事件增加的
	ON_WM_TIMER()
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
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	//////显示标题///////////////////
	CString modeflag;



	//结果输出到平台 模式 =1  单机版=0 
	if(YRVM_PINGTAI_MODE)
	{
		modeflag="P";

		if(0==ALLTAB_DETECT_CAR_MODE)
		{
			//电动车混合模式
			if(YRVM_PINGTAI_ELECAR_MIX_MODE)
				modeflag+="C";
			else
				modeflag+="E";
		}
	}
	else
		modeflag="S";

	CString str;
	str.Format("%s-%s-当前登陆用户:%s",MESSAGEBOX_TITLE,modeflag.GetBuffer(0),DlgLogin.CurrentUser.user);
	DlgMain->SetWindowText(str);




	// TODO: Add extra initialization here
	
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
	int button_top=130;
	int button_width=192;
	//tab高度
	int tab_top=163;

	//列表的高度
	int list_height=150;
	//快捷按钮的宽度
	int shortcut_width=66;
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
	printf_Rect.right = m_clientRect.right-distance;
	//必须 样式=重叠，边框=调整大小
	GetDlgItem(IDC_STATIC_PRINTF)->MoveWindow(printf_Rect);

	//底部减去日志条
//	m_clientRect.bottom -=printf_height+distance;
	m_clientRect.bottom -=printf_height;

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

	//列表`
	CRect list_Rect;
	list_Rect.top = m_clientRect.bottom-distance-list_height/* + 5*/;
	list_Rect.bottom = m_clientRect.bottom-distance/* - 10*/;
	list_Rect.left = tab_Rect.right+distance/* + 10*/;
	list_Rect.right = m_clientRect.right-distance-shortcut_width;
	//必须 样式=重叠，边框=调整大小
	m_ListCar.MoveWindow(list_Rect);

	//快捷按钮
	CRect shortcut_Rect;
	shortcut_Rect.top = m_clientRect.top/* + 5*/;
	shortcut_Rect.bottom = m_clientRect.bottom-distance/* - 10*/;
	shortcut_Rect.left = list_Rect.right+distance/* + 10*/;
	shortcut_Rect.right = m_clientRect.right;
	//必须 样式=重叠，边框=调整大小
	DlgShortCut.MoveWindow(shortcut_Rect);

	//分屏
	CRect screen_Rect;
	screen_Rect.top = m_clientRect.top/* + 5*/;
	screen_Rect.bottom = list_Rect.top -distance/* - 10*/;
	screen_Rect.left = tab_Rect.right +distance/* + 10*/;
	screen_Rect.right = list_Rect.right;
	//必须 样式=重叠，边框=调整大小
	DlgScreen.MoveWindow(screen_Rect);

}

void CYiRongCarDetectAIODlg::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CYiRongCarDetectAIODlg::OnCancel()
{
	// TODO: Add your control notification handler code here
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
	m_b_tree.Invalidate();
	m_b_ptz.LoadBitmaps(IDB_TAB_PTZ_CLOSE,NULL,NULL,NULL);
	m_b_ptz.Invalidate();
	m_b_normal.LoadBitmaps(IDB_TAB_NORMAL_CLOSE,NULL,NULL,NULL);
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

	m_b_tree.LoadBitmaps(IDB_TAB_TREE_CLOSE,NULL,NULL,NULL);
	m_b_tree.Invalidate();
	m_b_ptz.LoadBitmaps(IDB_TAB_PTZ_CLOSE,NULL,NULL,NULL);
	m_b_ptz.Invalidate();
	m_b_normal.LoadBitmaps(IDB_TAB_NORMAL,NULL,NULL,NULL);
	m_b_normal.Invalidate();
}

void CYiRongCarDetectAIODlg::OnButtonPtz() 
{
	// TODO: Add your control notification handler code here
	
	DlgDeviceTree.ShowWindow(SW_HIDE);
	DlgNormal.ShowWindow(SW_HIDE);
	DlgPtz.ShowWindow(SW_SHOW);

	m_b_tree.LoadBitmaps(IDB_TAB_TREE_CLOSE,NULL,NULL,NULL);
	m_b_tree.Invalidate();
	m_b_ptz.LoadBitmaps(IDB_TAB_PTZ,NULL,NULL,NULL);
	m_b_ptz.Invalidate();
	m_b_normal.LoadBitmaps(IDB_TAB_NORMAL_CLOSE,NULL,NULL,NULL);
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
	str.Format("%s--当前登陆用户:%s",MESSAGEBOX_TITLE,DlgLogin.CurrentUser.user);
	DlgMain->SetWindowText(str);

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
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

//////////////////////////////////////////////////////////////////////////
void CYiRongCarDetectAIODlg::ListMainInit(void)
{
	m_ListCar.SetBkColor(RGB(232,248,254));
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

#else

	m_ListCar.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 40);
	m_ListCar.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_ListCar.InsertColumn(2, _T("摄像头名称" ), LVCFMT_LEFT, 140);
	m_ListCar.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 100);
	m_ListCar.InsertColumn(4, _T("车牌号"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(5, _T("置信度"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(6, _T("行驶方向"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(7, _T("车牌颜色"), LVCFMT_LEFT, 70);
	m_ListCar.InsertColumn(8, _T("黑名单"), LVCFMT_LEFT, 50);
	m_ListCar.InsertColumn(9, _T("图片路径"), LVCFMT_LEFT, 0);
	m_ListCar.InsertColumn(10, _T("服务器模式"), LVCFMT_LEFT, 0);

#endif
	m_ListCar.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListCarTotal=0;
}

//双击显示大图
void CYiRongCarDetectAIODlg::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];

#if ALLTAB_DETECT_CAR_MODE
//汽车
	m_ListCar.GetItemText(pNMIA->iItem,11,str,260);
#else
//电动车
	m_ListCar.GetItemText(pNMIA->iItem,9,str,260);
#endif

	ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	*pResult = 0;
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
			lplvcd->clrTextBk=RGB(232,248,254);
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
		InvalidateRect(printf_Rect, FALSE);
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


void CYiRongCarDetectAIODlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}
