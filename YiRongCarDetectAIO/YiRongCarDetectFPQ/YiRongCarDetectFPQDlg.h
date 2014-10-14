// YiRongCarDetectFPQDlg.h : 头文件
//

#pragma once

#include "DLGdetectDevice.h"

// CYiRongCarDetectFPQDlg 对话框
class CYiRongCarDetectFPQDlg : public CDialog
{
// 构造
public:
	CYiRongCarDetectFPQDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_YIRONGCARDETECTFPQ_DIALOG };

	HANDLE pthread;
	//线程在用=false  线程未用=ture
	bool ThreadFlag;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	//服务器
	CDLGdetectDevice	DlgDetectDevice;

	CRect		m_clientRect;		//程序界面区域位置


	//线程句柄


	//切换位置
	void UpdatePannelPosition();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCam();
	afx_msg void OnBnClickedButtonDetect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
