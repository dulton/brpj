#pragma once


// CDLGAddIVMSdevice dialog

class CDLGAddIVMSdevice : public CDialog
{
	DECLARE_DYNAMIC(CDLGAddIVMSdevice)

public:
	CDLGAddIVMSdevice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGAddIVMSdevice();

// Dialog Data
	enum { IDD = IDD_ADD_IVMS_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
