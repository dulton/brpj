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
	CBitmapButton m_b_download_small;

	CString Titlestr;
	CString srcfile;
	CString srcsmallfile;
	
	CPicture pic;
	HBITMAP bim;

	CPicture picsmall;
	HBITMAP bimsmall;
	void PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth);

	void DrawImage(CStatic *m_picBox,unsigned char *image,unsigned long int size);
	void DrawFileImage(CStatic *m_picBox, char *filename);

	BOOL OnInitDialog();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCancel();
	afx_msg void OnOk();

	afx_msg void OnBnClickedButtonDownload();
	CStatic m_pic;
	CStatic m_pic_small;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	
	afx_msg void OnBnClickedButtonDownloadSmall();

	CString m_txt;
};
