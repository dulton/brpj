#pragma once
#include "afxwin.h"
#include "Picture.h"

#define FLAG_FAC_CAP 1
#define FLAG_FACE_ALARM 2
#define FLAG_FACE_BLACK 3
#define FLAG_FACE_BLACK_ONE 4
// CDLGPictureLite dialog

class CDLGPictureLite : public CDialog
{
	DECLARE_DYNAMIC(CDLGPictureLite)

public:
	CDLGPictureLite(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGPictureLite();
	BOOL	OnInitDialog();
	void AutoSize();


	CString srcfile;


	long long capnid;//NID
	long long personnid;//NID

	//为了弹窗
	int BlackFlag;
char* FaceSex(int i);
char* CardType(int i);


	void PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth);
	CPicture pic;
	HBITMAP bim;
void LoadPic();

	//可以显示图片的标记
	bool DrawFlag;


	bool aa;

// Dialog Data
	enum { IDD = IDD_PICTURE_LITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
		afx_msg void OnPaint();
	CStatic m_pic;
	CString m_info;
	afx_msg void OnStnDblclickStaticPic();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
