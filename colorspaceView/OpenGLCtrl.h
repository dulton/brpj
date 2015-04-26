#pragma once

#if 1
#include <GL/glut.h>
#else 
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif
#define GL_CTRL_TEST	0
//////////////////////////////////////////////////////////////////////
//-	宏定义

#define		CTRL_CLASS_NAME		_T("COpenGLCtrl")

// COpenGLCtrl

class COpenGLCtrl : public CWnd
{
	DECLARE_DYNAMIC(COpenGLCtrl)

	CDC*		m_pDC;
	HGLRC		m_hRC;

	GLfloat		m_glfViewPosX, m_glfViewPosY, m_glfViewPosZ,				//视点坐标
				m_glfTargetPosX, m_glfTargetPosY, m_glfTargetPosZ,			//观察目标点坐标
				m_glfCamDirectX, m_glfCamDirectY, m_glfCamDirectZ,			//相机朝向
				m_glfGridUnit;
	COLORREF	m_clrBackgroud, m_clrGridAxse, m_clrGridLine;				//背景网格的颜色
	int			m_nLines;
	bool flag3D;
public:

	enum { MS_LBBTN = 0, MS_MBBTN, MS_RBBTN };

public:
	COpenGLCtrl();
	virtual ~COpenGLCtrl();

	void		RegisterCtrlClass();
	void		SetSceneSize(GLsizei glsWidth, GLsizei glsHeight);
	void		InitOpenGL();
	void		RenderScene();
	COLORREF	GetBkgColor(){return m_clrBackgroud;}
	void		SetBkgColor(COLORREF clrBkg){m_clrBackgroud = clrBkg;}

	//外部使用函数指针 要在类构造的时候用
	 void (*outInitScene)(void);  
	 void (*outSceneView)(void);  
	 void (*outRenderScene)(void);  
	
protected:
	virtual void PreSubclassWindow();
 
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};