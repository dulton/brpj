#pragma once

#include "Common.h"
#include "afxwin.h"

#include "FaceCloudDLL.h"
// CFrmFaceMatch dialog

class CFrmFaceMatch : public CDialog
{
	DECLARE_DYNAMIC(CFrmFaceMatch)

public:
	CFrmFaceMatch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFrmFaceMatch();

// Dialog Data
	enum { IDD = IDD_FACEMATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void InitParameters();
	int StartThread();
	int StopThread();
	void CloseWindow();
public:
	CCommon * m_common;
	LONG m_lVideoWidth;
	LONG m_lVideoHeight;
	bool m_bThreadWork;
	bool m_bIsClose;//是否窗体关闭
	HANDLE m_pThreadDetect;


	char* FaceImage[10];

	int Face_count;
	RECT Face_range[32];
	unsigned char *tempRGB;
	int list_size;
	RwFaceRect face_rect_list[32];
	//初始化的画面矩形框 永远不变
	CRect old_DrawRect;
	//绘制图案的矩形框。
	CRect DrawRect;
	//绘制图案的矩形框和初始化框的比例
	float DrawScale;

protected:
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
public:
	CComboBox m_cbDevice;
	afx_msg void OnClose();
};