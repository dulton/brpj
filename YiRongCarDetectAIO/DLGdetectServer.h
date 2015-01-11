#pragma once
#include "afxcmn.h"


// CDLGdetectServer dialog

class CDLGdetectServer : public CDialog
{
	DECLARE_DYNAMIC(CDLGdetectServer)

public:
	CDLGdetectServer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGdetectServer();

	BOOL OnInitDialog();
	UINT  m_DetectListTimer;


// Dialog Data
	enum { IDD = IDD_DETECT_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_list;
	afx_msg void OnBnClickedButtonFlush();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

