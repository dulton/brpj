#pragma once

#include "DLGPictureFace.h"

#define HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM 20
// CDLGHistoryFaceAlarmLite dialog

class CDLGHistoryFaceAlarmLite : public CDialog
{
	DECLARE_DYNAMIC(CDLGHistoryFaceAlarmLite)

public:
	CDLGHistoryFaceAlarmLite(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGHistoryFaceAlarmLite();
	BOOL	OnInitDialog();
	void AutoSize();
	
	void DisplayerList(void);

	CDLGPictureFace DlgPictureFace;
	void PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth);
	CPicture pic;
	HBITMAP bim;
	CString srcfile;

	long long capnid;

	list<struct FACE_ALARM_VIEW_ST> alarmList;

	char* FaceSex(int i);
// Dialog Data
	enum { IDD = IDD_HISTORY_FACE_ALARM_LITE };

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

	CStatic m_pic;

	CRect printf_Rect;
	long ListTotal;
	long ListNow;
	long		m_page;
	CBitmapButton m_first_button;
	CBitmapButton m_pre_button;
	CBitmapButton m_next_button;
	CBitmapButton m_last_button;
	CBitmapButton m_jump_button;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_txt;
	afx_msg void OnStnDblclickPic();
};
