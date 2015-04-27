// colorspaceViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "colorspaceView.h"
#include "colorspaceViewDlg.h"

#include "colorspaceConvert.h"
#include "tess.h"

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

void GLinit() ;
void GLview() ;
void GLrener() ;

void GLviewWp() ;
void GLrenerWp();
void GLviewGAMUT() ;
void GLrenerGAMUT();

CColorspaceViewDlg::CColorspaceViewDlg(CWnd* pParent /*=NULL*/)
: CDialog(CColorspaceViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorspaceViewDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_gl.outInitScene=GLinit;
	m_gl.outSceneView=GLview;
	m_gl.outRenderScene=GLrener;



	m_gl_wp.outInitScene=GLinit;
	m_gl_wp.outSceneView=GLviewWp;
	m_gl_wp.outRenderScene=GLrenerWp;

	m_gl_gamut.outInitScene=GLinit;
	m_gl_gamut.outSceneView=GLviewGAMUT;
	m_gl_gamut.outRenderScene=GLrenerGAMUT;

}

void CColorspaceViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorspaceViewDlg)
	DDX_Control(pDX, IDC_STATIC_GL, m_gl);
	DDX_Control(pDX, IDC_STATIC_GDI, m_gdi);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_WP, m_gl_wp);
	DDX_Control(pDX, IDC_STATIC_GAMUT, m_gl_gamut);

	DDX_Control(pDX, IDC_COMBO_CIE, m_c_cie);
	DDX_Control(pDX, IDC_COMBO_CP, m_c_cp);
	DDX_Control(pDX, IDC_COMBO_WP, m_c_wp);
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
	ON_BN_CLICKED(IDC_BUTTON_GAMUT, OnButtonGamut)
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
	pDc = m_gdi.GetDC();
	CRect rect;
	m_gdi.GetClientRect(rect);

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
	m_gdi.ReleaseDC(pDc);
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


		CRect gdi_Rect;
		gdi_Rect.top = 	m_clientRect.top;
		gdi_Rect.bottom = m_clientRect.bottom/2;
		gdi_Rect.left = m_clientRect.left+100 ;
		gdi_Rect.right = (m_clientRect.right-100)/2;
		//必须 样式=重叠，边框=调整大小
		GetDlgItem(IDC_STATIC_GDI)->MoveWindow(gdi_Rect);

		CRect gl_Rect;
		gl_Rect.top = 	m_clientRect.top;
		gl_Rect.bottom = m_clientRect.bottom/2;
		gl_Rect.left = m_clientRect.left +	gdi_Rect.right;
		gl_Rect.right = m_clientRect.right-100;
		//必须 样式=重叠，边框=调整大小
		GetDlgItem(IDC_STATIC_GL)->MoveWindow(gl_Rect);
		CRect glwp_Rect;
		glwp_Rect.top = 	m_clientRect.top+	gdi_Rect.bottom ;
		glwp_Rect.bottom = m_clientRect.bottom;
		glwp_Rect.left = m_clientRect.left+100 ;
		glwp_Rect.right =(m_clientRect.right-100)/2;
		//必须 样式=重叠，边框=调整大小
		GetDlgItem(IDC_STATIC_WP)->MoveWindow(glwp_Rect);

		CRect glgamut_Rect;
		glgamut_Rect.top = 	m_clientRect.top+	gdi_Rect.bottom ;
		glgamut_Rect.bottom = m_clientRect.bottom;
		glgamut_Rect.left = m_clientRect.left+	gdi_Rect.right;
		glgamut_Rect.right =m_clientRect.right-100;
		//必须 样式=重叠，边框=调整大小
		GetDlgItem(IDC_STATIC_GAMUT)->MoveWindow(glgamut_Rect);

		Invalidate();

	}
}

void CColorspaceViewDlg::OnButtonLum() 
{
	// TODO: Add your control notification handler code here

	CDC *pDc = NULL;
	pDc = m_gdi.GetDC();
	CRect rect;
	m_gdi.GetClientRect(rect);

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

	int W=256,H=256;
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

#define  Gmax 256
#define  Gmin 0

	POINT p[Gmax-Gmin]={0};
	double y;

	FILE *fp;
	fp=fopen("a.txt","wb");
	if(fp)
	{

		for(int i=Gmin;i<Gmax;i++)
		{
			GammaLine(2.2,((double)i)/256.0,&y);
			p[i-Gmin].x=i;
			p[i-Gmin].y=y*Gmax;

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
	m_gdi.ReleaseDC(pDc);
}

void CColorspaceViewDlg::OnButtonGamut() 
{
	// TODO: Add your control notification handler code here

}
void GLinit() 
{
	// TODO: Add your control notification handler code here
	glDisable(GL_DEPTH_TEST);
}
void GLview() 
{
	// TODO: Add your control notification handler code here
	//
	glOrtho(0,256,0,256,-1,1 );
}
void GLrener() 
{
	// TODO: Add your control notification handler code here
	//绘背景色
	glClearColor(1,1,1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//设置场景坐标系
	glLoadIdentity();

	//设定绘制颜色
	glColor3f(1,0,0);

	//	glLineWidth(1.0f);

	float p[256*2]={0};
	double y;



	for(int i=0;i<256;i++)
	{
		GammaLine(2.2,((double)i)/256.0,&y);
		p[i*2]=i;
		p[i*2+1]=y*256.0;
	}

	glVertexPointer(2, GL_FLOAT, 0, p);

	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawArrays(GL_LINE_STRIP,0,256);

	glDisableClientState(GL_VERTEX_ARRAY);


	glRasterPos2f(50,50);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'0');

}
void GLviewWp() 
{
	// TODO: Add your control notification handler code here
	//
	glOrtho(0.26,0.48,0.26,0.48,-1,1 );
}
void GLrenerWp() 
{
	// TODO: Add your control notification handler code here
	//绘背景色
	glClearColor(1,1,1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//设置场景坐标系
	glLoadIdentity();

	//设定绘制颜色
	glColor3f(1,0,0);

	//	glLineWidth(1.0f);
#define  Kmax 10000
#define  Kmin 2800


	float p[(Kmax-Kmin)*2]={0};
	double x,y;

	for(int i=Kmin;i<Kmax;i++)
	{
		CCT_to_CIE_xy((double)i,&x,&y);
		p[(i-Kmin)*2]=x;
		p[(i-Kmin)*2+1]=y;

	}	

	glVertexPointer(2, GL_FLOAT, 0, p);

	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawArrays(GL_LINE_STRIP,0,10000-2800);

	glDisableClientState(GL_VERTEX_ARRAY);

}

void GLviewGAMUT() 
{
	// TODO: Add your control notification handler code here
	//
	glOrtho(0,1,0,1,-1,1 );
}

extern  struct ChromaticityCoordinates_Lite_ST CIE1931_X2_CC[471];
void GLrenerGAMUT() 
{
	// TODO: Add your control notification handler code here
	//绘背景色
	glClearColor(1,1,1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//设置场景坐标系
	glLoadIdentity();

	//设定绘制颜色
	glColor3f(1,0,0);

#if 0
	//	glLineWidth(1.0f);
	float p[6] ={0.1, 0.1,
		0.10, 0.50,
		0.50,0.50,
	};


	double c[9] ={1,0,0,
		0,1,0,
		0,0,1,
	};


	CIE_XYZ_1931_to_CIE_RGB(1,0,0,&c[0],&c[1],&c[2]);
	CIE_XYZ_1931_to_CIE_RGB(0,1,0,&c[3],&c[4],&c[5]);
	CIE_XYZ_1931_to_CIE_RGB(0,0,1,&c[6],&c[7],&c[8]);

	CIE_XYZ_1931_to_CIE_RGB(0.0014,0,0.0066,&c[0],&c[1],&c[2]);
	//	CIE_XYZ_1931_to_CIE_RGB(0.27368,0.71743,0.0089,&c[3],&c[4],&c[5]);
	//	CIE_XYZ_1931_to_CIE_RGB(0.16654,0.00888,0.82458,&c[6],&c[7],&c[8]);

	double x;double y;double z;

	CIE_rgb_to_CIE_xyz_1931(1,0,0,&x,&y,&z);
	CIE_rgb_to_CIE_xyz_1931(0,1,0,&x,&y,&z);
	CIE_rgb_to_CIE_xyz_1931(0,0,1,&x,&y,&z);

	glVertexPointer(2, GL_FLOAT, 0, p);
	glColorPointer(3,GL_DOUBLE, 0, c);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_TRIANGLES,0,3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//	glRasterPos2f(0.5,0.5);
	//	glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'0');

#else
	/*
	double p[3][5] ={0.1, 0.1,1,0,0,
		0.10, 0.50,0,1,0,
		0.50,0.50,0,0,1,
	};
	*/

	double **p=(double **)calloc(471,sizeof(double *));
	for(int i=0;i<471;i++)
	{
		p[i]=(double *)calloc(5,sizeof(double));
		p[i][0]=CIE1931_X2_CC[i].x;
		p[i][1]=CIE1931_X2_CC[i].y;
		p[i][2]=CIE1931_X2_CC[i].x;
		p[i][3]=CIE1931_X2_CC[i].y;
		p[i][4]=CIE1931_X2_CC[i].z;
	
	}


Tessdraw(p,471);

#endif

}
