#if !defined(AFX_DLGCHANNELCFG_H__E10EB0EE_38A4_4002_BD7B_EDA3B069A480__INCLUDED_)
#define AFX_DLGCHANNELCFG_H__E10EB0EE_38A4_4002_BD7B_EDA3B069A480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChannelCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelCfg dialog

class CDlgChannelCfg : public CDialog
{
// Construction
public:
	int  m_curch;
	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
	int m_chnums;
public:
	CDlgChannelCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChannelCfg)
	enum { IDD = IDD_DLG_CHANNEL };
	CString	m_chname;
	int		m_bitstype;
	BOOL	m_bitsosd;
	int		m_delay;
	int		m_framerate;
	int		m_ikeyinterval;
	int		m_imageformat;
	int		m_maxqueue;
	int		m_maxbits;
	int		m_minqueue;
	BOOL	m_motion;
	BOOL	m_record;
	int		m_samplerate;
	int		m_streamtype;
	int		m_timeosd;
	CString	m_titalosd;
	BOOL	m_videomask;
	int		m_audiobits;
	int		m_bitosd_x;
	int		m_bitosd_y;
	int		m_timeosd_x;
	int		m_timeosd_y;
	int		m_titalosd_x;
	int		m_titalosd_y;
	CString	m_titalosd1;
	CString	m_titalosd2;
	CString	m_titalosd3;
	CString	m_titalosd4;
	int		m_titalosd1_x;
	int		m_titalosd2_x;
	int		m_titalosd3_x;
	int		m_titalosd4_x;
	int		m_titalosd1_y;
	int		m_titalosd2_y;
	int		m_titalosd3_y;
	int		m_titalosd4_y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChannelCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChannelCfg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnMotionarea();
	afx_msg void OnBtnRecord();
	afx_msg void OnBtnVideomask();
	afx_msg void OnBtnMotiontime();
	afx_msg void OnRecord();
	afx_msg void OnVideomask();
	afx_msg void OnMotion();
	afx_msg void OnBtnViparam();
	afx_msg void OnSelchangeBitratetype();
	afx_msg void OnSelchangeComboChsel();
	afx_msg void OnBtnSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CHANNVIPARAM s_viparam[MAX_CHANNEL];
	CHANNMOTION s_channmotion[MAX_CHANNEL];
	CHANNOSD    s_channosd[MAX_CHANNEL];
	CHANNVIDEOMASK s_channvmask[MAX_CHANNEL];
	CHANNELPARAM   s_chanpara[MAX_CHANNEL];
	BOOL        s_chvideolost[MAX_CHANNEL];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHANNELCFG_H__E10EB0EE_38A4_4002_BD7B_EDA3B069A480__INCLUDED_)
