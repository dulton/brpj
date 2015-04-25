#pragma once
#include "Picture.h"

#include "YRSVMySQL.h"

#define MAX_PERSON_FACE 4
// CDLGSetBlackFaceEdit dialog

class CDLGSetBlackFaceEdit : public CDialog
{
	DECLARE_DYNAMIC(CDLGSetBlackFaceEdit)

public:
	CDLGSetBlackFaceEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGSetBlackFaceEdit();

	char srcpath[MAX_PERSON_FACE][MAX_PATH];//已注册人脸原始照片路径
	char facepath[MAX_PERSON_FACE][MAX_PATH];//已注册人脸照片路径

	char filepath[MAX_PERSON_FACE][MAX_PATH];//当前控件照片路径
	long filesize[MAX_PERSON_FACE];
	//char filepath1[MAX_PATH];
	CPicture pic1;
	HBITMAP bim1;
	//char filepath2[MAX_PATH];
	CPicture pic2;
	HBITMAP bim2;
	//char filepath3[MAX_PATH];
	CPicture pic3;
	HBITMAP bim3;
	//char filepath4[MAX_PATH];
	CPicture pic4;
	HBITMAP bim4;

void PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth);
	// Dialog Data
	enum { IDD = IDD_SET_BLACK_FACEEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CBitmapButton m_b_ok;
	CBitmapButton m_b_cancel;


	CBitmapButton m_b_file1;
	CStatic m_pic1;
	CBitmapButton m_b_file2;
	CStatic m_pic2;
	CBitmapButton m_b_file3;
	CStatic m_pic3;
	CBitmapButton m_b_file4;
	CStatic m_pic4;
	CBitmapButton m_b_delete1;
		CBitmapButton m_b_delete2;
			CBitmapButton m_b_delete3;
				CBitmapButton m_b_delete4;
	
public:
	bool RegistFlag;//人脸注册成功标志
	bool RegistFace(HBITMAP pic,char *path,struct PERSON_BLACK_ST black);
	void ModifyFace(HBITMAP pic,char *path,struct PERSON_BLACK_ST black,int num);

	void AddNerPerson(void);
	void EditModeInit(void);
	void EditPersonInfo(void);

	afx_msg void OnBnClickedButtonFile1();
	afx_msg void OnBnClickedButtonFile2();
	afx_msg void OnBnClickedButtonFile3();
	afx_msg void OnBnClickedButtonFile4();

	COleDateTime	m_BirthDay;
	CString m_e_name;
	int m_c_cardtype;
	int m_c_jiguan;
	CString m_e_cardnum;
	CString m_e_phone;
	CString m_e_addr;
	int m_c_sex;
	afx_msg void OnBnClickedButtonDelete1();
	afx_msg void OnBnClickedButtonDelete2();
	afx_msg void OnBnClickedButtonDelete3();
	afx_msg void OnBnClickedButtonDelete4();
	long db_id;
	long RWdb_id;
	bool bModifyFlag;
	long person_id;
	struct PERSON_BLACK_ST curBlackInfo;
	bool bFaceChange1;
	bool bFaceChange2;
	bool bFaceChange3;
	bool bFaceChange4;
	bool bFaceDelete1;
	bool bFaceDelete2;
	bool bFaceDelete3;
	bool bFaceDelete4;

	long faceID[4];
	long RWfaceID[4];
	int curFaceNum;//当前人脸数
};
