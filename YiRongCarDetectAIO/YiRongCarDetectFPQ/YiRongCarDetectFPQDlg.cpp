// YiRongCarDetectFPQDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YiRongCarDetectFPQ.h"
#include "YiRongCarDetectFPQDlg.h"

#include "IO.h"
IO OracleIO;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CYiRongCarDetectFPQDlg 对话框




CYiRongCarDetectFPQDlg::CYiRongCarDetectFPQDlg(CWnd* pParent /*=NULL*/)
: CDialog(CYiRongCarDetectFPQDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYiRongCarDetectFPQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYiRongCarDetectFPQDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CYiRongCarDetectFPQDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CYiRongCarDetectFPQDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//线程
DWORD WINAPI ORACLE_ThreadPROC(LPVOID lpParameter)
{
	CYiRongCarDetectFPQDlg *pdlg = (CYiRongCarDetectFPQDlg*)lpParameter;
	pdlg->ThreadFlag=TRUE;

	long long Missionid=0;

	unsigned long int camid=0;
	unsigned long int isplay=0;
	unsigned long int userid=0;
	char indate[32];

	long DetectDeviceID=0;

	while(pdlg->ThreadFlag)
	{
		Missionid=0;
		camid=0;
		isplay=0;
		userid=0;
		DetectDeviceID=0;
		indate[0]=0;

		//ORACLE查询任务 分发
		if(OracleIO.MISSION_Read(&Missionid,&camid,&isplay,&userid,indate)<0)
		{
			pdlg->ThreadFlag=FALSE;
			return 0;
		}

		if(isplay)
		{
			//开始
			OracleIO.DEVICE_SearchForStop(&DetectDeviceID,camid);

			if(DetectDeviceID>0)
			{
				// 已经开始 失效任务
				OracleIO.MISSION_Edit( Missionid,0,2);
			}
			else
			{
				OracleIO.DEVICE_SearchForStart(&DetectDeviceID);

				if(DetectDeviceID>0)
				{
					//有效
					//预占用识别服务器。
					OracleIO.DETECT_EditCamera(DetectDeviceID,Missionid,camid);
					//修改任务
					OracleIO.MISSION_Edit( Missionid,DetectDeviceID,1);
				}
				else
				{
					//满了
					OracleIO.MISSION_Edit( Missionid,0,2);
				}
			}
		}
		else
		{
			//停止
			OracleIO.DEVICE_SearchForStop(&DetectDeviceID,camid);

			if(DetectDeviceID>0)
			{
				//有效
				OracleIO.MISSION_Edit( Missionid,DetectDeviceID,1);
			}
			else
			{
				// 多次停止  失效任务
				OracleIO.MISSION_Edit( Missionid,0,2);
			}
		}

		Sleep(200);
	}

	pdlg->ThreadFlag=FALSE;
	return 0;
}
// CYiRongCarDetectFPQDlg 消息处理程序

BOOL CYiRongCarDetectFPQDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	DlgDetectDevice.Create(IDD_DETECT_DEVICE,this);
	DlgDetectDevice.ShowWindow(SW_SHOW);

	////////////////lynn/////////////////
	int OracleError = OracleIO.ConnectionOracleDBTXT("YRDataBaseConfig.db");
	if(OracleError == ReadFile_FAIL)
	{
		this->MessageBox("读数据库配置文件失败","连接数据库");

		return FALSE;
	}
	else if(OracleError == Instance_FAIL)
	{
		this->MessageBox("数据库ADO初始化失败","连接数据库");
		return FALSE;
	}
	else if(OracleError == ContOpen_FAIL)
	{
		this->MessageBox("数据库无法连接:账号密码IP异常 或文件夹带()","连接数据库");
		return FALSE;
	}
	else if(OracleError == Connectd_DONE)
	{
		//MessageBox("连接数据库成功");
	}


	ThreadFlag=FALSE;
	pthread=NULL;

	pthread=CreateThread(NULL,0,ORACLE_ThreadPROC,this,0,NULL);

	if(NULL==pthread)
	{
		MessageBox("创建线程错误");
	}
	else
		ThreadFlag=TRUE;


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CYiRongCarDetectFPQDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYiRongCarDetectFPQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CYiRongCarDetectFPQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYiRongCarDetectFPQDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CYiRongCarDetectFPQDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	ThreadFlag=FALSE;
	Sleep(1000);
	TerminateThread(pthread,0);

	OracleIO.DisConnectionOracleDB();
	OnCancel();
}

void CYiRongCarDetectFPQDlg::OnSize(UINT nType, int cx, int cy)
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

		DlgDetectDevice.AutoSize();

		Invalidate();
	}
}
void CYiRongCarDetectFPQDlg::UpdatePannelPosition()
{
	GetClientRect(&m_clientRect);
	/*
	int button_top=40;

	CRect dlg_Rect;
	dlg_Rect.top = m_clientRect.top+button_top;
	dlg_Rect.bottom = m_clientRect.bottom;
	dlg_Rect.left = 	m_clientRect.left;
	dlg_Rect.right =	m_clientRect.right;
	*/
	DlgDetectDevice.MoveWindow(m_clientRect);


}
void CYiRongCarDetectFPQDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}
