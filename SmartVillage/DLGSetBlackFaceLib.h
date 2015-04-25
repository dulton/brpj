#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "YRSVMySQL.h"

#define SET_FACEBLACK_PAGE_MAX_NUM 35

// CDLGSetBlackFaceLib dialog

class CDLGSetBlackFaceLib : public CDialog
{
	DECLARE_DYNAMIC(CDLGSetBlackFaceLib)

public:
	CDLGSetBlackFaceLib(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGSetBlackFaceLib();

// Dialog Data
	enum { IDD = IDD_SET_BLACK_FACELIB };


		void DisplayerList(void);
		void DisplayerPersonList(void);

			long ListChoose;

			long ListPersonChoose;
			long ListPersonTotal;
			long ListPersonNow;

			list<struct FACE_DB_ST> list_facedb;
			long facedbNum;
			list<struct PERSON_BLACK_ST> list_person;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnButtonBlackAdd();
	afx_msg void OnButtonBlackModify();
	afx_msg void OnButtonBlackdelete();

	
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickPersonList(NMHDR* pNMHDR, LRESULT* pResult);
	

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
		virtual void OnCancel();
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();

	CBitmapButton m_b_add;
	CBitmapButton m_b_edit;
	CBitmapButton m_b_delete;

public:
		CListCtrl	m_List;
	CString m_name;
	afx_msg void OnBnClickedButtonBlackPersonadd();
	afx_msg void OnBnClickedButtonBlackPersonmodify();
	afx_msg void OnBnClickedButtonBlackPersondelete();
	CBitmapButton m_b_person_add;
	CBitmapButton m_b_person_modify;
	CBitmapButton m_b_person_delete;
	CListCtrl m_List_person;


	CRect printf_Rect;
	long		m_page;
	afx_msg void OnButtonJump();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	CBitmapButton m_first_button;
	CBitmapButton m_pre_button;
	CBitmapButton m_next_button;
	CBitmapButton m_last_button;
	CBitmapButton m_jump_button;
	afx_msg void OnLvnItemActivateListPerson(NMHDR *pNMHDR, LRESULT *pResult);
};
