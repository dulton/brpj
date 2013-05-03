#if !defined(AFX_EXLIST_H__BF46D1BA_5CA1_42D4_BD5E_82CB1B0BA598__INCLUDED_)
#define AFX_EXLIST_H__BF46D1BA_5CA1_42D4_BD5E_82CB1B0BA598__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CListCtrlEx
#define IDC_PROGRESS_LIST WM_USER

class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

	// the array of the Progress Controls in the list control
	CArray<CProgressCtrl*,CProgressCtrl*> m_ProgressList;
	// the column which should contain the progress bars
	int m_ProgressColumn;
	int m_ProgressValueColumn;
	DWORD m_Style;
	DWORD	m_StyleEx;
	DWORD	m_BKcolor;
	DWORD	m_color;
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();
	// initialize the column containing the bars
	void InitProgressColumn(int ValueNum=0,int ColNum=1);
	void InitProgressStyle(DWORD style);
	void InitProgressStyleEx(DWORD styleEx);
	void InitProgressColor(DWORD color,DWORD BKcolor);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


#endif
