#pragma once


// CDLGdetectDevice dialog

class CDLGdetectDevice : public CDialog
{
	DECLARE_DYNAMIC(CDLGdetectDevice)

public:
	CDLGdetectDevice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGdetectDevice();

	void AutoSize();

// Dialog Data
	enum { IDD = IDD_DETECT_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
