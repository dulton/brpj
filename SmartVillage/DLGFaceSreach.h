#pragma once
#include "afxwin.h"

#include "Picture.h"
// CDLGFaceSreach dialog

class CDLGFaceSreach : public CDialog
{
	DECLARE_DYNAMIC(CDLGFaceSreach)

public:
	CDLGFaceSreach(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGFaceSreach();

	BOOL	OnInitDialog();
	void AutoSize();

	char filepath[MAX_PATH];

	CPicture pic;
	HBITMAP bim;
	void PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth);

	void Clean();

// Dialog Data

	enum { IDD = IDD_FACE_SREACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnCheckTime();
	CBitmapButton m_b_file;
	afx_msg void OnBnClickedButtonFile();
		CStatic m_pic;
	COleDateTime	m_StartMon;
	COleDateTime	m_StartHour;
	COleDateTime	m_EndMon;
	COleDateTime	m_EndHour;
	BOOL	m_CheckTime;

	COleDateTime	m_Birth_Start;
	COleDateTime	m_Birth_End;
		
	CBitmapButton m_b_delete;

	CString m_e_name;
	int m_c_cardtype;
	int m_c_jiguan;
	CString m_e_cardnum;
	CString m_e_phone;
	CString m_e_addr;
	int m_e_sim_start;

	BOOL m_c_limitnum;
	int m_e_limitnum;
	int m_c_sex;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCheckLimitnum();
	BOOL m_CheckLimitNum;
};
