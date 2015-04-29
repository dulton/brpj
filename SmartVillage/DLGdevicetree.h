#if !defined(AFX_DLGDEVICETREE_H__E9261DEE_ECE8_4CEF_8A5C_6F1C52B4B07B__INCLUDED_)
#define AFX_DLGDEVICETREE_H__E9261DEE_ECE8_4CEF_8A5C_6F1C52B4B07B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGdevicetree.h : header file
//
#include "DLGAddDevice.h"
#include "FaceMng.h"


#include "YRSVMySQL.h"
#include <vector>
#include <list>
using namespace::std;
/////////////////////////////////////////////////////////////////////////////
// CDLGdevicetree dialog



class CDLGdevicetree : public CDialog
{
// Construction
public:
	CDLGdevicetree(CWnd* pParent = NULL);   // standard constructor
~CDLGdevicetree();
// Dialog Data
	//{{AFX_DATA(CDLGdevicetree)
	enum { IDD = IDD_DEVICE_TREE };
	CTreeCtrl	m_DeviceTree;
	HTREEITEM	m_selectItem;
	CDLGAddDevice DlgAddDevice;

#if OPEN_FACEDETECT_CODE
	CFaceMng faceMngTempDB;
#endif

	CImageList  imagelist;

	//}}AFX_DATA
	IPLIST iplist[MAX_CAMERA];
	int iptotal;


	BOOL	OnInitDialog();
	void AutoSize();

	//ÆÁ±Î²Ù×÷
	void OnOK();
	void OnCancel();

	bool kakouADDdevice(long ncamera,char *name,char *longitude,char *latitude);
	bool kakouDeldevice(long ncamera);
	bool kakouDelArea(long areaid);
	void ChangeTreeItemIcon(long ncamera,bool isPlay);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGdevicetree)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGdevicetree)
	afx_msg void OnPaint();
	afx_msg void OnRclickTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemAdddevice();
	afx_msg void OnMenuitemUpdate();
	afx_msg void OnMenuitemEdit();
	afx_msg void OnMenuitemDeleteDevice();
	afx_msg void OnMenuitemDeletearea();
	afx_msg void OnDblclkTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuitemAddivmsdevice();
	afx_msg void OnMenuitemFlushtree();
	afx_msg void OnMenuitemSetCar();
	afx_msg void OnMenuitemSetFace();
	afx_msg void OnMenuitemAddarea();
	afx_msg void OnMenuitemModifyarea();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEVICETREE_H__E9261DEE_ECE8_4CEF_8A5C_6F1C52B4B07B__INCLUDED_)
