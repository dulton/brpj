// YiRongCarInFoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


#include "lib\cv.h"
#include "lib\highgui.h"
#include "CvvImage.h"

#include "IO.h"
// CYiRongCarInFoDlg 对话框
class CYiRongCarInFoDlg : public CDialog
{
// 构造
public:
	CYiRongCarInFoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_YIRONGCARINFO_DIALOG };

	IO pio;
	bool m_connectionState;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	char ftppath[260];
	IplImage* m_ImageREPORT;
	IplImage* m_ImageALARM;

	unsigned int REPORTtotal;
	unsigned int ALARMtotal;

	void ShowImage(IplImage* img, UINT ID);
	void ResizeImageALARM(IplImage* img);
	void ReadImageALARM(CString imgPath);
	void ResizeImageREPORT(IplImage* img);
	void ReadImageREPORT(CString imgPath);

	UINT  m_relinktimer;
	UINT  m_displaytimer;

	void MoveFile(void);
	void ReadAlarm(void);

	void  ReadReport(void);


// 实现

	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void OnOK();
	void OnDestroy();

	CListCtrl m_LISTAlarm;
	CListCtrl m_LISTReport;
	afx_msg void OnLvnItemActivateListReport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateListAlarm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonHistory();
	afx_msg void OnBnClickedButtonHistoryAll();
};
