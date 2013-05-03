// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"


// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)
CListCtrlEx::CListCtrlEx() :m_ProgressValueColumn(0),m_ProgressColumn(1),m_Style(NULL)
{

}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CListCtrlEx message handlers


void CListCtrlEx::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages


	int Top=GetTopIndex();
	int Total=GetItemCount();
	int PerPage=GetCountPerPage();
	int LastItem=((Top+PerPage)>Total)?Total:Top+PerPage;

	// if the count in the list os nut zero delete all the progress controls and them procede
	{
		int Count=(int)m_ProgressList.GetCount();
		for(int i=0;i<Count;i++)
		{
			CProgressCtrl* pControl=m_ProgressList.GetAt(0);
			pControl->DestroyWindow();
			//修复内存泄露
			delete pControl;
			m_ProgressList.RemoveAt(0);
		}
	}

	CHeaderCtrl* pHeader=GetHeaderCtrl();
	for(int i=Top;i<LastItem;i++)
	{
		CRect ColRt;
		pHeader->GetItemRect(m_ProgressColumn,&ColRt);
		// get the rect
		CRect rt;
		GetItemRect(i,&rt,LVIR_LABEL);
		rt.top+=1;
		rt.bottom-=1;
		rt.left+=ColRt.left;
		int Width=ColRt.Width();
		rt.right=rt.left+Width-4;
		/*
		rt.left=ColRt.left+1;
		rt.right=ColRt.right-1;
		*/

		// create the progress control and set their position
		CProgressCtrl* pControl=new CProgressCtrl();
		pControl->Create(m_Style,rt,this,IDC_PROGRESS_LIST+i);

		CString Data=GetItemText(i,m_ProgressValueColumn);
		int Percent=atoi(Data);

		// set the position on the control
		pControl->SetPos(Percent);
		pControl->ShowWindow(SW_SHOWNORMAL);
		// add them to the list
		m_ProgressList.Add(pControl);
	}
	CListCtrl::OnPaint();
}

void CListCtrlEx::InitProgressColumn(int ValueNum,int ColNum/*=0*/)
{
	//值在第几行
	m_ProgressValueColumn=ValueNum;
	m_ProgressColumn=ColNum;
}

void CListCtrlEx::InitProgressStyle(DWORD style)
{
	m_Style=style;
}