// OpenGLCtrl.cpp : 实现文件
//

#include "stdafx.h"

#include "OpenGLCtrl.h"
#include "math.h"


// COpenGLCtrl

IMPLEMENT_DYNAMIC(COpenGLCtrl, CWnd)

COpenGLCtrl::COpenGLCtrl()
{
	//注册控件类
	RegisterCtrlClass();

	//初始变量
	m_pDC					= NULL;
	m_hRC					= NULL;
	m_nLines				= 20;
	m_glfGridUnit			= 1;

	//初始颜色表
	m_clrBackgroud			= RGB(160, 160, 160);
	m_clrGridAxse			= RGB(0, 0, 0);
	m_clrGridLine			= RGB(128, 128, 128);
	//3D视角
	flag3D=true;

	//设置视点
	m_glfViewPosX	= 0;
	m_glfViewPosY	= 5;
	m_glfViewPosZ	= 20;

	//设置观察点
	m_glfTargetPosX	= 0;
	m_glfTargetPosY	= 0;
	m_glfTargetPosZ	= 0;

	//设置相机指向
	m_glfCamDirectX	= 0;
	m_glfCamDirectY	= 1;
	m_glfCamDirectZ	= 0;
}

COpenGLCtrl::~COpenGLCtrl()
{
}

void COpenGLCtrl::RegisterCtrlClass()
{
	HINSTANCE hInstance = AfxGetInstanceHandle();

	WNDCLASS	wndclsCtrl;
	ZeroMemory(&wndclsCtrl, sizeof(WNDCLASS));

	if(::GetClassInfo(hInstance, CTRL_CLASS_NAME, &wndclsCtrl))
		return;

	//设置控件类信息
	wndclsCtrl.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclsCtrl.lpfnWndProc		= ::DefWindowProc;
	wndclsCtrl.cbClsExtra		= 0;
	wndclsCtrl.cbWndExtra		= 0;
	wndclsCtrl.hInstance		= hInstance;
	wndclsCtrl.hIcon			= NULL;
	wndclsCtrl.hCursor			= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndclsCtrl.hbrBackground	= NULL;
	wndclsCtrl.lpszMenuName		= NULL;
	wndclsCtrl.lpszClassName	= CTRL_CLASS_NAME;

	//注册控件类
	AfxRegisterClass(&wndclsCtrl);
}


void COpenGLCtrl::SetSceneSize(GLsizei glsWidth, GLsizei glsHeight)
{
	if(glsHeight==0||glsWidth==0)
		return;

	float	fRatioWH;

	fRatioWH	= float(glsWidth)/glsHeight;

	glViewport(0, 0, glsWidth, glsHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

#if GL_CTRL_TEST	
	if(flag3D)
		gluPerspective(35.0f, fRatioWH, 0.01f, 1000.0f);
	else
		glOrtho(0,glsWidth, glsHeight,0,-1,1 );

#else
	outSceneView();
#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void COpenGLCtrl::InitOpenGL()
{
	GLuint	glnPixelFormat;

	static PIXELFORMATDESCRIPTOR	pfd	 = {
												sizeof(PIXELFORMATDESCRIPTOR),
												1,
												PFD_DRAW_TO_WINDOW |
												PFD_SUPPORT_OPENGL |
												PFD_DOUBLEBUFFER,
												PFD_TYPE_RGBA,
												16,
												0, 0, 0, 0, 0, 0,
												0,
												0,
												0,
												0, 0, 0, 0,
												16,
												0,
												0,
												PFD_MAIN_PLANE,
												0,
												0, 0, 0
											};

	//设置设备环境
	m_pDC			= GetDC();
	glnPixelFormat	= ChoosePixelFormat(m_pDC->m_hDC, &pfd);
	SetPixelFormat(m_pDC->m_hDC, glnPixelFormat, &pfd);
	m_hRC			= wglCreateContext(m_pDC->m_hDC);
	wglMakeCurrent(m_pDC->m_hDC, m_hRC);

	//设置渲染环境
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0,1.0,1.0,1.0);
#if GL_CTRL_TEST	
	if(flag3D)
	{
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}
	else
		glDisable(GL_DEPTH_TEST);
#else
	outInitScene();
#endif

}

void COpenGLCtrl::RenderScene()
{
	//绘背景色
	glClearColor(float(GetRValue(m_clrBackgroud))/255, float(GetGValue(m_clrBackgroud))/255, float(GetBValue(m_clrBackgroud))/255, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//设置场景坐标系
	glLoadIdentity();
	//设置观察点
	gluLookAt(m_glfViewPosX, m_glfViewPosY, m_glfViewPosZ, m_glfTargetPosX, m_glfTargetPosY, m_glfTargetPosZ, m_glfCamDirectX, m_glfCamDirectY, m_glfCamDirectZ);

	//绘制轴线
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(float(GetRValue(m_clrGridAxse))/255, float(GetGValue(m_clrGridAxse))/255, float(GetBValue(m_clrGridAxse))/255);
	//横线
	glVertex3f(-m_glfGridUnit*m_nLines, 0.0f, 0);
	glVertex3f( m_glfGridUnit*m_nLines, 0.0f, 0);
	//纵线
	glVertex3f( 0, 0.0f, -m_glfGridUnit*m_nLines);
	glVertex3f( 0, 0.0f,  m_glfGridUnit*m_nLines);
	glEnd();

	glLineWidth(0.1f);
	glBegin(GL_LINES);
	//设定绘制颜色
	glColor3f(float(GetRValue(m_clrGridLine))/255, float(GetGValue(m_clrGridLine))/255, float(GetBValue(m_clrGridLine))/255);
	//绘制网格线
	for(int i=1; i<=m_nLines; i++)
	{
		//横线
		glVertex3f(-m_glfGridUnit*m_nLines, 0.0f, m_glfGridUnit*i);
		glVertex3f( m_glfGridUnit*m_nLines, 0.0f, m_glfGridUnit*i);
		glVertex3f(-m_glfGridUnit*m_nLines, 0.0f, -m_glfGridUnit*i);
		glVertex3f( m_glfGridUnit*m_nLines, 0.0f, -m_glfGridUnit*i);
		//纵线
		glVertex3f( m_glfGridUnit*i, 0.0f, -m_glfGridUnit*m_nLines);
		glVertex3f( m_glfGridUnit*i, 0.0f,  m_glfGridUnit*m_nLines);
		glVertex3f(-m_glfGridUnit*i, 0.0f, -m_glfGridUnit*m_nLines);
		glVertex3f(-m_glfGridUnit*i, 0.0f,  m_glfGridUnit*m_nLines);
	}
	glEnd();
}

BEGIN_MESSAGE_MAP(COpenGLCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// COpenGLCtrl 消息处理程序

void COpenGLCtrl::PreSubclassWindow() 
{
	DWORD	dwStyle, dwStyleEx;
	CRect	rectCtrl;

	dwStyle		 = ::GetWindowLong(m_hWnd, GWL_STYLE);
	dwStyleEx	 = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	dwStyle		|= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	//dwStyleEx	|= WS_EX_CLIENTEDGE;

	//重设窗口风格
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwStyleEx);
	//初始OpenGL环境
	InitOpenGL();
	//设置场景大小
	GetClientRect(rectCtrl);
	SetSceneSize(rectCtrl.Width(), rectCtrl.Height());

	CWnd::PreSubclassWindow();
}

void COpenGLCtrl::OnPaint() 
{
	CRect		rectClient;
	CPaintDC	dc(this);

	//获取视图区域
	GetClientRect(rectClient);
	//关联DC与RC
	wglMakeCurrent(m_pDC->m_hDC, m_hRC);
	//绘制场景
#if GL_CTRL_TEST
	RenderScene();
#else
	outRenderScene();
#endif
	//翻页
	SwapBuffers(m_pDC->m_hDC);
	//释放绘图设备
	wglMakeCurrent(m_pDC->m_hDC, NULL);
}

void COpenGLCtrl::OnDestroy() 
{
	//清理绘图设备
	wglMakeCurrent(m_pDC->m_hDC, NULL);
	wglDeleteContext(m_hRC);

	CWnd::OnDestroy();
}

void COpenGLCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	wglMakeCurrent(m_pDC->m_hDC, m_hRC);
	SetSceneSize(cx, cy);
	wglMakeCurrent(m_pDC->m_hDC, NULL);
}