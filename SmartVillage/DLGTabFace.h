#pragma once

#include "DLGFaceSreach.h"
#include "afxcmn.h"
// CDLGTabFace dialog
#include "DLGPictureFace.h"
#include "FaceRecg.h"

#include <vector>
#include <list>
using namespace::std;

#define FLAG_CAP 1
#define FLAG_BLACK 2
#define FLAG_HISTORY_FACE 3

#define TREE_TEMP_ALL 1
#define TREE_TEMP_ONE 2
#define TREE_BLACK_ALL 3
#define TREE_BLACK_ONE 4

#define TAB_FACE_PAGE_MAX_NUM 40


class CDLGTabFace : public CDialog
{
	DECLARE_DYNAMIC(CDLGTabFace)

public:
	CDLGTabFace(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGTabFace();

	BOOL	OnInitDialog();
	void AutoSize();

	void Init();
	void Clean();

	HTREEITEM BlackRootitem;						//本指针
	HTREEITEM TempRootitem;						//本指针
	DBLIST dblist[MAX_AREA];
	int dbtotal;
	list<struct FACE_DB_ST> BlackDBList;
	list<struct FACE_TEMP_DB_ST> TempDBList;
	void BulidTree(void);


	list<struct FACE_CAPTURE_ST> faceList;
	list<struct TEMP_CMP_RESULT_ST> faceTempResList;
	list<struct BLACK_CMP_RESULT_ST> faceBlackResList;

	void DisplayerList(void);
void DisplayerPicList(void);

	CDLGPictureFace DlgPictureFace;
	CDLGFaceSreach DlgFaceSreach;

#if OPEN_FACEDETECT_CODE
	CFaceRecg FaceRecg;
#endif
	struct TEMP_CMP_CONDITION_ST tempcondition;
	struct BLACK_CMP_CONDITION_ST blackcondition;

	int ModeFlag;
	//用来存放SQL语句的函数
	char SqlStr[1024];
	int searchFlag;
// Dialog Data
	enum { IDD = IDD_TAB_FACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonJump();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonSearch();
		CImageList  imagelist;
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

	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);

	int iLastSearchMode;
};
