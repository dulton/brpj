// VLC_MFCDlg.h : header file
//
#include <vlc/vlc.h>
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

#pragma once

#define VIDEO_WIDTH		1280
#define VIDEO_HEIGHT	720


// CVLC_MFCDlg dialog
class CVLC_MFCDlg : public CDialog
{
// Construction
public:
	CVLC_MFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VLC_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp; 
	libvlc_media_t *m;
	bool m_bPlay;
	//初始化的画面矩形框 永远不变
	CRect old_DrawRect;
	//绘制图案的矩形框。
	CRect DrawRect;
	//绘制图案的矩形框和初始化框的比例
	float DrawScale;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_Url;
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	void DrawCtrlImage(char * buffer, long int bufferSize,CRect rect,float scale);
	CRect SetDrawSize(CStatic * m_picBox,CRect old_DrawRect,int bmpw,int bmph,float *scale) ;
protected:
	virtual void OnCancel();
};
