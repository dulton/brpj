#pragma once


// CDLGAddArea dialog

class CDLGAddArea : public CDialog
{
	DECLARE_DYNAMIC(CDLGAddArea)

public:
	CDLGAddArea(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGAddArea();

// Dialog Data
	enum { IDD = IDD_ADD_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
		virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_AddArea;
	CBitmapButton m_b_ok;
	CBitmapButton m_b_cancel;
	afx_msg void OnPaint();
};
