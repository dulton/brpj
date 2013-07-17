#pragma once

#include "Common.h"
#include "afxwin.h"

// CFrmFaceMatch dialog

class CFrmFaceMatch : public CDialog
{
	DECLARE_DYNAMIC(CFrmFaceMatch)

public:
	CFrmFaceMatch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFrmFaceMatch();

// Dialog Data
	enum { IDD = IDD_FACEMATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void InitParameters();
	int StartThread();
	int StopThread();
	void CloseWindow();
public:
	CCommon * m_common;
	LONG m_lVideoWidth;
	LONG m_lVideoHeight;
	bool m_bThreadWork;
	bool m_bIsClose;//ÊÇ·ñ´°Ìå¹Ø±Õ
	HANDLE m_pThreadDetect;
	HANDLE m_pThreadMatch;
	char* FaceImage[10];
protected:
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
public:
	CComboBox m_cbDevice;
	afx_msg void OnClose();
};