#if !defined(AFX_DLGNEWDEVICE_H__F2669FA9_9106_44E1_8A0B_4E1ACD3B1E26__INCLUDED_)
#define AFX_DLGNEWDEVICE_H__F2669FA9_9106_44E1_8A0B_4E1ACD3B1E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGNewDevice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGNewDevice dialog

class CDLGNewDevice : public CDialog
{
// Construction
public:
	CDLGNewDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGNewDevice)
	enum { IDD = IDD_NEWDEVICE };
	CString	m_ip;
	CString	m_name;
	UINT	m_port;
	CString	m_pwd;
	CString	m_user;
	//}}AFX_DATA
	BOOL OnInitDialog();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGNewDevice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGNewDevice)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWDEVICE_H__F2669FA9_9106_44E1_8A0B_4E1ACD3B1E26__INCLUDED_)
