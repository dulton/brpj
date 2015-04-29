#pragma once

#include "DLGPictureFace.h"
#include "DLGFaceSreachLite.h"

#include <vector>
#include <list>
using namespace::std;
#define HISTORY_DETECT_FLAG_FACE 1
#define HISTORY_DETECT_FLAG_FACEALARM 2

#define HISTORY_FACE_PAGE_MAX_NUM 30

// CDLGHistoryFace dialog

class CDLGHistoryFace : public CDialog
{
	DECLARE_DYNAMIC(CDLGHistoryFace)

public:
	CDLGHistoryFace(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGHistoryFace();

	BOOL	OnInitDialog();
	void AutoSize();

	int flag;
	CImageList  imagelist;

	HTREEITEM rootitem;						//本指针
	DBLIST dblist[MAX_AREA_ADD_CAM];
	int dbtotal;

void CDLGHistoryFace::BulidTree(void);

list<struct FACE_CAPTURE_ST> faceList;
	void DisplayerList(void);
		
	CDLGPictureFace DlgPictureFace;
	CDLGFaceSreachLite DlgFaceSreachLite;
// Dialog Data
	enum { IDD = IDD_HISTORY_FACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonJump();
		afx_msg void OnButtonSearch();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//用来存放SQL语句的函数
	char SqlStr[1024];
	int searchFlag;

	CTreeCtrl m_tree;
	CRect printf_Rect;
	long ListTotal;
	long ListNow;
	long		m_page;
	CBitmapButton m_first_button;
	CBitmapButton m_pre_button;
	CBitmapButton m_next_button;
	CBitmapButton m_last_button;
	CBitmapButton m_jump_button;

	CBitmapButton m_search;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
