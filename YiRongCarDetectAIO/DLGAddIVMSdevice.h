#if !defined(AFX_IDD_ADD_IVMS_DEVICE__INCLUDED_)
#define AFX_IDD_ADD_IVMS_DEVICE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if IVMS_ORACLE_DEVICETREE


#include "IO.h"
#include <vector>
#include <list>
#include "afxwin.h"
using namespace::std;

// CDLGAddIVMSdevice dialog

class CDLGAddIVMSdevice : public CDialog
{
	DECLARE_DYNAMIC(CDLGAddIVMSdevice)

public:
	CDLGAddIVMSdevice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGAddIVMSdevice();


	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ADD_IVMS_DEVICE };


	list<struct CONTROL_UNIT_ST> Controlunit;
	list<struct REGION_INFO_ST> RegionInfo;

	list<struct STREAM_SERVER_ST> Streamserver;
	list<struct PAG_SERVER_ST> PAGserver;

	list<struct CAMERA_INFO_LITE_ST> Caminfo;
	list<struct CAMERA_INFO_LITE_ST> CaminfoFind;

	struct CAMERA_INFO_ST CamData;
	CString Rtspurl;

	bool FindMode;
	void  InitALL(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonFind();
	CString m_edit_find;
	CComboBox m_camvender;
	CComboBox m_root;
	CComboBox m_unit;
	CComboBox m_region;
	CComboBox m_cam;
	afx_msg void OnCbnCloseupComboRoot();
	afx_msg void OnCbnCloseupComboUnit();
	afx_msg void OnCbnCloseupComboRegion();
	afx_msg void OnCbnCloseupComboCam();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CBitmapButton m_b_find;
	CBitmapButton m_b_ok;
	CBitmapButton m_b_cancel;
};

#endif


#endif