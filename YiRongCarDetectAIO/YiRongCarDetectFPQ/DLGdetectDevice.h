#pragma once
#include "afxcmn.h"


// CDLGdetectDevice dialog

class CDLGdetectDevice : public CDialog
{
	DECLARE_DYNAMIC(CDLGdetectDevice)

public:
	CDLGdetectDevice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGdetectDevice();

	void AutoSize();
	BOOL OnInitDialog();
	UINT  m_DetectListTimer;
	UINT  m_HeartTestTimer;


	long chooseDetectDeviceid;
	unsigned long int chooseCamid;

// Dialog Data

	enum { IDD = IDD_DETECT_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonFlush();
	afx_msg void OnBnClickedButtonStop();
	CString m_ip;
	long m_channel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAdd();
	CListCtrl m_list;
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
