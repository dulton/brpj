// colorspaceViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "colorspaceView.h"
#include "colorspaceViewDlg.h"

#include "colorspaceConvert.h"

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
// CColorspaceViewDlg dialog

CColorspaceViewDlg::CColorspaceViewDlg(CWnd* pParent /*=NULL*/)
: CDialog(CColorspaceViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorspaceViewDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CColorspaceViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorspaceViewDlg)
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorspaceViewDlg, CDialog)
//{{AFX_MSG_MAP(CColorspaceViewDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_WHITEPOINT, OnButtonWhitepoint)
ON_WM_SIZE()
ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_LUM, OnButtonLum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorspaceViewDlg message handlers

BOOL CColorspaceViewDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	//必须设置。不然静态控件在放大缩小的时候不显示
	ModifyStyle(0, WS_CLIPCHILDREN); 
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColorspaceViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CColorspaceViewDlg::OnPaint() 
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
		
	
		//或者开窗口直接显示
		POINT p[4] ={{10, 10},
		{20, 20},
		{40, 40},
		{50, 70},
		};
		CPaintDC dc(this); // device context for painting
		dc.Polyline( p,4);
		
		CDialog::OnPaint();
	}
	
	
}
BOOL CColorspaceViewDlg::OnEraseBkgnd(CDC* pDC ) 
{
	return CDialog::OnEraseBkgnd(pDC);  
	//return FALSE;
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CColorspaceViewDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CColorspaceViewDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CColorspaceViewDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CColorspaceViewDlg::OnButtonWhitepoint() 
{
	// TODO: Add your control notification handler code here

	CDC *pDc = NULL;
	pDc = m_pic.GetDC();
	CRect rect;
	m_pic.GetClientRect(rect);
	
	int RW=rect.Width();
	int RH=rect.Height();
	
	pDc->SetMapMode(MM_ANISOTROPIC);//自定义方式 X向右增加 y向上增加
	//	pDc->SetWindowExt(RW,RH);
	//	pDc->SetViewportExt(RW,-RH);
	//	pDc->SetWindowOrg(0,RH);
	// 
	pDc->SetWindowOrg(0, 0);  
	pDc->SetWindowExt(RW,RH);
	pDc->SetViewportOrg(0, RH);
	pDc->SetViewportExt(RW,-RH);
	
	int W=4600,H=4600;
	int SX=2600,SY=2600;
	
	
	m_Cache.CreateCache(W-SX,H-SY,24);
	//定义画笔
	/*	m_Cache.m_MemDC.SetMapMode(MM_ANISOTROPIC);  */
	//	m_Cache.m_MemDC.SetWindowExt(W,H);
	//	m_Cache.m_MemDC.SetWindowOrg(0,H);
	//	m_Cache.m_MemDC.SetViewportExt(W,-H);
	//	m_Cache.m_MemDC.SetViewportOrg(0,0);
	// 
	// 	m_Cache.m_MemDC.SetWindowOrg(0, 0);  
	// 	m_Cache.m_MemDC.SetWindowExt(W,H);
	// 	m_Cache.m_MemDC.SetViewportOrg(0, H);
	// 	m_Cache.m_MemDC.SetViewportExt(W,-H);
	
	CPen pen(PS_SOLID, ((W-SX)/RW)*3, RGB(255, 0, 0));
	
	HGDIOBJ oldObject = m_Cache.m_MemDC.SelectObject(pen.GetSafeHandle()); 
	
#if 0
	POINT p[4] ={{10, 10},
	{20, 20},
	{40, 40},
	{50, 70},
	};
	
	m_Cache.m_MemDC.Polyline( p,4);
#else
	
#define  Kmax 10000
#define  Kmin 2800
	
	POINT p[Kmax-Kmin]={0};
	double x,y;
	
	FILE *fp;
	fp=fopen("a.txt","wb");
	if(fp)
	{
		
		for(int i=Kmin;i<Kmax;i++)
		{
			CCT_to_CIE_xy((double)i,&x,&y);
			p[i-Kmin].x=x*10000-SX;
			p[i-Kmin].y=y*10000-SY;
			
			fprintf(fp,"%d=%d %d \n",i,p[i-Kmin].x,p[i-Kmin].y);
		}	
		fclose(fp);
	}
	
	m_Cache.m_MemDC.Polyline( p,Kmax-Kmin);
#endif
	
	//	m_Cache.m_MemDC.FillSolidRect(0,0,W,H,RGB(255,255,255));//清屏
	
	//	pDc->BitBlt(0, 0, x, y, m_Cache.GetCacheDC(), 0, 0, SRCCOPY);
	
	pDc->StretchBlt(0, 0,RW,RH, &(m_Cache.m_MemDC), 0, 0,W-SX,H-SY,SRCCOPY);
	
	m_Cache.FreeCache();
	m_pic.ReleaseDC(pDc);
}

void CColorspaceViewDlg::OnSize(UINT nType, int cx, int cy) 
{
	
	// TODO: Add your message handler code here
	CDialog::OnSize(nType, cx, cy);
	
	//窗口最小化与窗口大小无变化不处理
	if ((cx ==0 && cy == 0) || 
		(cx == m_clientRect.Width() && cy == m_clientRect.Height())) 
	{
		
		return;
	}
	else
	{
		GetClientRect(&m_clientRect);
		
		
		CRect tab_Rect;
		tab_Rect.top = 	m_clientRect.top;
		tab_Rect.bottom = m_clientRect.bottom;
		tab_Rect.left = m_clientRect.left ;
		tab_Rect.right = m_clientRect.right-100;
		//必须 样式=重叠，边框=调整大小
		GetDlgItem(IDC_STATIC_PIC)->MoveWindow(tab_Rect);
		
		Invalidate();
		
	}
}

void CColorspaceViewDlg::OnButtonLum() 
{
	// TODO: Add your control notification handler code here
	
	CDC *pDc = NULL;
	pDc = m_pic.GetDC();
	CRect rect;
	m_pic.GetClientRect(rect);
	
	int RW=rect.Width();
	int RH=rect.Height();
	
	pDc->SetMapMode(MM_ANISOTROPIC);//自定义方式 X向右增加 y向上增加
	//	pDc->SetWindowExt(RW,RH);
	//	pDc->SetViewportExt(RW,-RH);
	//	pDc->SetWindowOrg(0,RH);
	// 
	pDc->SetWindowOrg(0, 0);  
	pDc->SetWindowExt(RW,RH);
	pDc->SetViewportOrg(0, RH);
	pDc->SetViewportExt(RW,-RH);
	
	int W=1000,H=1000;
	int SX=0,SY=0;
	
	
	m_Cache.CreateCache(W-SX,H-SY,24);
	//定义画笔
	/*	m_Cache.m_MemDC.SetMapMode(MM_ANISOTROPIC);  */
	//	m_Cache.m_MemDC.SetWindowExt(W,H);
	//	m_Cache.m_MemDC.SetWindowOrg(0,H);
	//	m_Cache.m_MemDC.SetViewportExt(W,-H);
	//	m_Cache.m_MemDC.SetViewportOrg(0,0);
	// 
	// 	m_Cache.m_MemDC.SetWindowOrg(0, 0);  
	// 	m_Cache.m_MemDC.SetWindowExt(W,H);
	// 	m_Cache.m_MemDC.SetViewportOrg(0, H);
	// 	m_Cache.m_MemDC.SetViewportExt(W,-H);
	
	CPen pen(PS_SOLID, ((W-SX)/RW)*3+1, RGB(255, 0, 0));
	
	HGDIOBJ oldObject = m_Cache.m_MemDC.SelectObject(pen.GetSafeHandle()); 
	
#if 0
	POINT p[4] ={{10, 10},
	{20, 20},
	{40, 40},
	{50, 70},
	};
	
	m_Cache.m_MemDC.Polyline( p,4);
#else
	
#define  Gmax 1000
#define  Gmin 0
	
	POINT p[Gmax-Gmin]={0};
	double y;
	
	FILE *fp;
	fp=fopen("a.txt","wb");
	if(fp)
	{
		
		for(int i=Gmin;i<Gmax;i++)
		{
			GammaLine(2.2,((double)i)/1000.0,&y);
			p[i-Gmin].x=i;
			p[i-Gmin].y=y*1000;
			
			fprintf(fp,"%d=%d %d \n",i,p[i-Gmin].x,	p[i-Gmin].y);
		}	
		fclose(fp);
	}
	
	m_Cache.m_MemDC.Polyline( p,Gmax-Gmin);
#endif
	
	//	m_Cache.m_MemDC.FillSolidRect(0,0,W,H,RGB(255,255,255));//清屏
	
	//	pDc->BitBlt(0, 0, x, y, m_Cache.GetCacheDC(), 0, 0, SRCCOPY);
	
	pDc->StretchBlt(0, 0,RW,RH, &(m_Cache.m_MemDC), 0, 0,W-SX,H-SY,SRCCOPY);
	
	m_Cache.FreeCache();
	m_pic.ReleaseDC(pDc);
}
