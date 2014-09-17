#pragma once
#include "afxwin.h"

#include "Picture.h"

// CDLGpictureView dialog

class CDLGpictureView : public CDialog
{
	DECLARE_DYNAMIC(CDLGpictureView)

public:
	CDLGpictureView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGpictureView();

// Dialog Data
	enum { IDD = IDD_PICVIEW };

	CBitmapButton m_b_download;

	CString Titlestr;
	CString srcfile;
	
	CPicture pic;
	HBITMAP bim;

	void DrawImage(CStatic *m_picBox,unsigned char *image,unsigned long int size);
	void DrawFileImage(CStatic *m_picBox, char *filename);

	BOOL OnInitDialog();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString m_path;
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedButtonDownload();
	CStatic m_pic;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
