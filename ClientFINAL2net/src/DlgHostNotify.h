#if !defined(AFX_DLGHOSTNOTIFY_H__9B479146_1811_4F17_A356_26D53B82463E__INCLUDED_)
#define AFX_DLGHOSTNOTIFY_H__9B479146_1811_4F17_A356_26D53B82463E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHostNotify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHostNotify dialog

class CDlgHostNotify : public CDialog
{
// Construction
public:
	CDlgHostNotify(CWnd* pParent = NULL);   // standard constructor
	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
	int  m_chnums;

// Dialog Data
	//{{AFX_DATA(CDlgHostNotify)
	enum { IDD = IDD_DLG_HOSTNOTIFY };
	CString	m_data;
	int		m_intervaltime;
	CString	m_hosturl;
	int		m_hostport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHostNotify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHostNotify)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	VSNETHOSTNOTIFY m_notify;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHOSTNOTIFY_H__9B479146_1811_4F17_A356_26D53B82463E__INCLUDED_)
