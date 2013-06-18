// DataBaseConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataBaseConfig.h"
#include "DataBaseConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ReadConfigTxt.h"

/////////////////////////////////////////////////////////////////////////////
// CDataBaseConfigDlg dialog

CDataBaseConfigDlg::CDataBaseConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataBaseConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataBaseConfigDlg)
	m_ipaddr = _T("");
	m_name = _T("");
	m_port = _T("");
	m_psw = _T("");
	m_user = _T("");
	m_pswagain = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataBaseConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataBaseConfigDlg)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_IPADDR, m_ipaddr);
	DDV_MaxChars(pDX, m_ipaddr, 20);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 128);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDV_MaxChars(pDX, m_port, 10);
	DDX_Text(pDX, IDC_PSW, m_psw);
	DDV_MaxChars(pDX, m_psw, 32);
	DDX_Text(pDX, IDC_USER, m_user);
	DDV_MaxChars(pDX, m_user, 32);
	DDX_Text(pDX, IDC_PSW_AGAIN, m_pswagain);
	DDV_MaxChars(pDX, m_pswagain, 32);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataBaseConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CDataBaseConfigDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataBaseConfigDlg message handlers

BOOL CDataBaseConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//设按钮图片
	m_ok.LoadBitmaps(IDB_OK_OUT,IDB_OK_IN,NULL,NULL);
	m_ok.SizeToContent();		//自适应图片大小

	m_cancel.LoadBitmaps(IDB_CANCEL_OUT,IDB_CANCEL_IN,NULL,NULL);
	m_cancel.SizeToContent();		//自适应图片大小

	//设标题
	SetWindowText("智能视频车牌分析服务软件 2.0版-数据库连接设置");

	//先读取
	TCHAR ipaddr[RCT_MAX_STR_SIZE]="";
	TCHAR port[RCT_MAX_STR_SIZE]="";
	TCHAR user[RCT_MAX_STR_SIZE]="";
	TCHAR password[RCT_MAX_STR_SIZE]="";
	TCHAR name[RCT_MAX_STR_SIZE]="";

	if(ReadFile("DataBaseConfig.txt",ipaddr,port,user,password,name))
	{
		m_ipaddr = ipaddr;
		m_name = name;
		m_port = port;
		m_psw = password;
		m_pswagain = password;
		m_user = user;
		
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataBaseConfigDlg::OnPaint() 
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
		bitmap.LoadBitmap(IDB_BK);    
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
HCURSOR CDataBaseConfigDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDataBaseConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_pswagain != m_psw)
	{
		MessageBox("两次输入的密码不一致",MESSAGEBOX_TITLE);
		return ;
	}

	if(m_ipaddr.IsEmpty() ||
		m_name.IsEmpty() ||
		m_port.IsEmpty() ||
		m_psw .IsEmpty() ||
		m_pswagain.IsEmpty() ||
		m_user.IsEmpty()	)
	{
		MessageBox("有选项未填写",MESSAGEBOX_TITLE);
		return ;
	}

	if(-1 != m_ipaddr.Find(' '))
	{
		MessageBox("IP地址 输入字符勿含空格",MESSAGEBOX_TITLE);
		return ;
	}
	if(-1 != m_name.Find(' '))
	{
		MessageBox("数据库名称 输入字符勿含空格",MESSAGEBOX_TITLE);
		return ;
	}
	if(-1 != m_port.Find(' '))
	{
		MessageBox("端口 输入字符勿含空格",MESSAGEBOX_TITLE);
		return ;
	}
	if(-1 != m_psw.Find(' '))
	{
		MessageBox("密码 输入字符勿含空格",MESSAGEBOX_TITLE);
		return ;
	}
	if(-1 != m_pswagain.Find(' '))
	{
		MessageBox("再次确认密码 输入字符勿含空格",MESSAGEBOX_TITLE);
		return ;
	}
	if(-1 != m_user.Find(' '))
	{
		MessageBox("用户名 输入字符勿含空格",MESSAGEBOX_TITLE);
		return ;
	}

	//写入
	WriteFile("DataBaseConfig.txt",	\
		m_ipaddr.GetBuffer(0),m_port.GetBuffer(0),m_user.GetBuffer(0),m_psw.GetBuffer(0),m_name.GetBuffer(0));

	CDialog::OnOK();
}

void CDataBaseConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

HBRUSH CDataBaseConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
