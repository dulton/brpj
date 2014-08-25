// YrCarDetectAIOUpdateDlg.h : header file
//

#pragma once


#include "SignalDownload.h"
#include "URLencode.h"

struct UPDATE_OP_S
{
	char flag;
	char srcurl[260];
	char dstpath[260];
};
#define MAX_UPDATE_FILE 1024

// CYrCarDetectAIOUpdateDlg dialog
class CYrCarDetectAIOUpdateDlg : public CDialog
{
// Construction
public:
	CYrCarDetectAIOUpdateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_YRCARDETECTAIOUPDATE_DIALOG };

	bool ReadVersionFile(TCHAR* FileName);
	bool CheckVersion();
	bool ReadUpdateList();
	void filterstr(char *str);
bool CheckError(char *filepath,char *filename);
bool DownloadUpdateList();
bool CopyUpdateList();

	char CurrentDir[MAX_PATH];

	SignalDownload sd;
	char updatedir[MAX_PATH];
	//操作结构体
	struct UPDATE_OP_S updateList[MAX_UPDATE_FILE];
	int  updateListTotal;



	bool stopflag;
	bool realstop;
//线程句柄
	HANDLE pthread;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support



// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	long m_version;
	CString m_ip;

};
