// ScreenPannel.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "ScreenPannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenPannel class

CScreenPannel::CScreenPannel()
{
}

CScreenPannel::~CScreenPannel()
{
	// remove all pages
	while(!m_PageList.IsEmpty())
		m_PageList.RemoveHead();
}


BEGIN_MESSAGE_MAP(CScreenPannel, CWnd)
	//{{AFX_MSG_MAP(CScreenPannel)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreenPannel member functions
void CScreenPannel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     
	
	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_SCREEN_BACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	
	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	// TODO: Add your message handler code here
	UpdateWnd();



	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CScreenPannel::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(RGB(192,192,192));
	pDC->FillRect(&rt,&br);

	return TRUE;
	
	return CWnd::OnEraseBkgnd(pDC);
}

int CScreenPannel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	for(int i = 0; i < MAX_DEVICE_NUM; i++)
	{
		m_wndVideo[i].Create(
			NULL, 
			NULL, 
			WS_VISIBLE | WS_CHILD,
			CRect(0, 0, 0, 0),
			this,
			1979,
			NULL);

		m_wndVideo[i].SetWinID(i);
		
		AddPage(&m_wndVideo[i]);
	}

	SetActivePage(&m_wndVideo[0], TRUE);
	SetDrawActivePage(TRUE, RGB(248,5,182), RGB(248,5,182));

	return 0;
}

int CScreenPannel::SetShowPlayWin(int nMain, int nSub)
{
	if (nSub < 0 || nSub > MAX_DEVICE_NUM)
	{
		nSub = 0;
	}

	int nNum = 16;
	int nBegin = 0;
	switch(nMain) 
	{
	case SCREEN_1MODE:
		nNum = 1;
		nBegin = nSub;
		break;
	case SCREEN_4MODE:
		nNum = 4;
		if (nSub >= 12)
		{
			nBegin = 12;
		}
		else if (nSub >= 8)
		{
			nBegin = 8;
		}
		else if (nSub >= 4)
		{
			nBegin = 4;
		}
		else
		{
			nBegin = 0;
		}

		break;
	case SCREEN_9MODE:
		nNum = 9;
		if (nSub >= 10)
		{
			nBegin = 7;
		}
		else
		{
			nBegin = 0;
		}
		break;
	case SCREEN_16MODE:
		nNum = 16;
		nBegin = 0;
		break;
	default:
		break;
	}

	CList<CWnd*, CWnd*> templist;

	POSITION pos = m_PageList.GetHeadPosition();
	while(pos != NULL)
	{
		CWnd* pWnd = m_PageList.GetNext(pos);
		if (pWnd)
		{
			templist.AddTail(pWnd);
		}
	}

	m_PageList.RemoveAll();

	/*
	while(m_PageList.GetCount())
	{
		DelPage(GetTailPage());
	}
	*/
	
	for(int i=nBegin; i < (nBegin+nNum); i++)
	{
		AddPage(&m_wndVideo[i], TRUE);
	}

	SetActivePage(&m_wndVideo[nSub], TRUE);

	pos = templist.GetHeadPosition();
	while(pos != NULL)
	{
		CWnd* pWnd = templist.GetNext(pos);
		if (pWnd)
		{
			pWnd->ShowWindow(SW_HIDE);
		}
	}

	templist.RemoveAll();

	return m_PageList.GetCount();

	/*

	if (m_PageList.GetCount() > nNum)
	{
		int nCnt = m_PageList.GetCount();
		while (nCnt > nNum)
		{
			CWnd* pWnd = DelPage(GetTailPage());	
			if (!pWnd)
			{
				DelPage(GetTailPage());
			}

			nCnt = m_PageList.GetCount();
		}

		//UpdateWnd();
		SetActivePage(&m_wndVideo[0], TRUE);
	}
	else if (m_PageList.GetCount() < nNum)
	{
		int nCnt = m_PageList.GetCount();
		while (nCnt < nNum)
		{
			AddPage(&m_wndVideo[nCnt], FALSE);
			nCnt = m_PageList.GetCount();
		}

		//UpdateWnd();
		SetActivePage(&m_wndVideo[0], TRUE);
	}

	return m_PageList.GetCount();
	*/
}

void CScreenPannel::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	for(int i=0; i < MAX_DEVICE_NUM; i++)
	{
		m_wndVideo[i].DestroyWindow();
	}
}

CWnd * CScreenPannel::GetPage(int nIndex)
{
	if (nIndex >= 0 && nIndex < MAX_DEVICE_NUM)
	{
		return &m_wndVideo[nIndex];
	}

	return NULL;
}

//CScreenPannel::CScreenPannel(CWnd* pParent /*=NULL*/)
//	: CDialog(CScreenPannel::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CScreenPannel)
//		// NOTE: the ClassWizard will add member initialization here
//	//}}AFX_DATA_INIT
//}
//
//
//void CScreenPannel::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CScreenPannel)
//		// NOTE: the ClassWizard will add DDX and DDV calls here
//	//}}AFX_DATA_MAP
//}
//
//
//BEGIN_MESSAGE_MAP(CScreenPannel, CDialog)
//	//{{AFX_MSG_MAP(CScreenPannel)
//		// NOTE: the ClassWizard will add message map macros here
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()
//
///////////////////////////////////////////////////////////////////////////////
//// CScreenPannel message handlers
//
//BOOL CScreenPannel::PreTranslateMessage(MSG* pMsg) 
//{
//	if (WM_KEYDOWN == pMsg->message /* && VK_ESCAPE == pMsg->wParam */)
//	{
//		if (VK_ESCAPE == pMsg->wParam)
//		{
//			((CNetSDKDemoDlg *)AfxGetMainWnd())->ReturnOrignalSplit();
//		}
//
//		return TRUE;
//	}
//	return CDialog::PreTranslateMessage(pMsg);
//}
