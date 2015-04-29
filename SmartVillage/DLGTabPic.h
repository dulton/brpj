#pragma once
#include "afxwin.h"
#include "Picture.h"

// CDLGTabPic dialog

class CDLGTabPic : public CDialog
{
	DECLARE_DYNAMIC(CDLGTabPic)

public:
	CDLGTabPic(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGTabPic();
	BOOL	OnInitDialog();
	void AutoSize();

void PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth);
	CString srcfile;
	CString srcsmallfile;

	CPicture pic;
	HBITMAP bim;

	CPicture picsmall;
	HBITMAP bimsmall;

	CStatic m_pic;
	CStatic m_pic_small;

void Load();
void clear();
// Dialog Data
	enum { IDD = IDD_TAB_PIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
			
	CString m_txt;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
