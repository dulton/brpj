#pragma once


// CDLGFaceSreachLite dialog

class CDLGFaceSreachLite : public CDialog
{
	DECLARE_DYNAMIC(CDLGFaceSreachLite)

public:
	CDLGFaceSreachLite(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGFaceSreachLite();

	BOOL	OnInitDialog();
	void AutoSize();

// Dialog Data
	enum { IDD = IDD_FACE_SREACH_LITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int m_age_start;
	int m_age_end;
	afx_msg void OnCheckTime();
	COleDateTime	m_StartMon;
	COleDateTime	m_StartHour;
	COleDateTime	m_EndMon;
	COleDateTime	m_EndHour;
	BOOL	m_CheckTime;

	int m_c_sex;

};
