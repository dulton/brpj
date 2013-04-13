// BarcodeRecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "BarcodeRecordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CBarcodeRecordDlg dialog

CBarcodeRecordDlg::CBarcodeRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarcodeRecordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeRecordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBarcodeRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeRecordDlg)
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBarcodeRecordDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeRecordDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarcodeRecordDlg message handlers

BOOL CBarcodeRecordDlg::OnInitDialog()
{
	Language_SetWndStaticText(this);
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
	
//////////////////////////////////////////
	DlgControl.Create(IDD_CONTROL,this);
	DlgControl.ShowWindow(SW_SHOW);

	DlgPlaywin.Create(IDD_PLAYWIN,this);
	DlgPlaywin.ShowWindow(SW_SHOW);

	DlgProductInfo.Create(IDD_PRODUCT_INFO,this);
	DlgProductInfo.ShowWindow(SW_SHOW);


	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBarcodeRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBarcodeRecordDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBarcodeRecordDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBarcodeRecordDlg::OnOK() 
{
	// TODO: Add extra validation here
	

}

void CBarcodeRecordDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CBarcodeRecordDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
		//窗口最小化与窗口大小无变化不处理
	if ((cx ==0 && cy == 0) || 
		(cx == m_clientRect.Width() && cy == m_clientRect.Height())) 
	{
		return;
	}
	else
	{
		UpdatePannelPosition();

		DlgControl.AutoSize();
		DlgPlaywin.AutoSize();
		DlgProductInfo.AutoSize();

		Invalidate();
	}
}

void CBarcodeRecordDlg::UpdatePannelPosition()
{
	GetClientRect(&m_clientRect);

	//切换栏宽度=LOGO图片的宽度
	int tree_width=258;
	//LOGO图片的高度
	int tree_height=200;

	//窗体边距
	int distance=10;

	//树的标题
	CRect treetitle_Rect;
	treetitle_Rect.top = 	m_clientRect.top+distance;
	treetitle_Rect.bottom = 	treetitle_Rect.top +15;
	treetitle_Rect.left = m_clientRect.right-distance-tree_width;
	treetitle_Rect.right = m_clientRect.right-distance;
	//必须 样式=重叠，边框=调整大小
	GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(treetitle_Rect);

	//树
	CRect tree_Rect;
	tree_Rect.top = treetitle_Rect.bottom+distance;
	tree_Rect.bottom = tree_Rect.top + tree_height;
	tree_Rect.left = treetitle_Rect.left;
	tree_Rect.right =treetitle_Rect.right;
	//必须 样式=重叠，边框=调整大小
	m_tree.MoveWindow(tree_Rect);

	//产品信息
	CRect product_Rect;
	product_Rect.top = tree_Rect.bottom+distance;
	product_Rect.bottom = m_clientRect.bottom;
	product_Rect.left = treetitle_Rect.left;
	product_Rect.right =treetitle_Rect.right;

	//必须 样式=重叠，边框=调整大小
	DlgProductInfo.MoveWindow(product_Rect);

	//控制
	CRect control_Rect;
	control_Rect.top = m_clientRect.bottom-50;
	control_Rect.bottom = m_clientRect.bottom;
	control_Rect.left = m_clientRect.left;
	control_Rect.right =treetitle_Rect.left-distance;

	//必须 样式=重叠，边框=调整大小
	DlgControl.MoveWindow(control_Rect);

	//播放
	CRect playwin_Rect;
	playwin_Rect.top = m_clientRect.top+distance;
	playwin_Rect.bottom = control_Rect.top - distance;
	playwin_Rect.left = m_clientRect.left+distance;
	playwin_Rect.right =treetitle_Rect.left-distance;

	//必须 样式=重叠，边框=调整大小
	DlgPlaywin.MoveWindow(playwin_Rect);
}
