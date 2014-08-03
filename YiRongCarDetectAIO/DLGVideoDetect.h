#if !defined(AFX_DLGVIDEODETECT_H__F1ECA381_984B_4062_8C2A_7552B1365DB4__INCLUDED_)
#define AFX_DLGVIDEODETECT_H__F1ECA381_984B_4062_8C2A_7552B1365DB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGVideoDetect.h : header file
//

#if OPEN_CARDETECT_CODE
#include "VideoCarDetect.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CDLGVideoDetect dialog
enum 
{
	PLAY_FLAG_STOP=0,// 停止播放
	PLAY_FLAG_START=1,//开始播放
	PLAY_FLAG_PAUSE=2,// 暂停播放
	PLAY_FLAG_GOON=3, //暂停后继续播放
};


class CDLGVideoDetect : public CDialog
{
// Construction
public:
	CDLGVideoDetect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGVideoDetect)
	enum { IDD = IDD_VIDEO_DETECT };
	CComboBox	m_cam;
	CComboBox	m_area;
	CSliderCtrl	m_slider;
	CListCtrl	m_list;
	CStatic	m_playwin;
	BOOL	m_check_writedb;
	CString	m_edit_file;
	CString	m_camip;
	CString	m_static_endtime;
	CString	m_static_starttime;
	CTime	m_StartHour;
	CTime	m_StartMon;
	//}}AFX_DATA

	CString AreaStr;
	CString CamStr;
	unsigned long int camid;
	char *filename;
	
	HDC playwin_hdc;
	RECT playwin_rect;
	//线程句柄
	HANDLE pthread;
	// 0=playflag 停止播放
	// 1=playflag 开始播放
	// 2=playflag 暂停播放
	// 3=playflag 暂停后继续播放
	int playflag;
	//true=全速 false=固定时间
	bool speedflag;
	//抓一个照片
	bool cutpic; 
	//进度条跳转触发
	bool sliderSeekflag;
	unsigned long long sliderSeek;

	int fps;
	CTime StartTime;

	BOOL OnInitDialog();
	void EnablePlayState();

	void ListMainInit(void);
	void CleanList(void);
	int m_ListCarTotal;

#if OPEN_CARDETECT_CODE 
	CVideoCarDetect	VideoCarDetect;
#endif
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGVideoDetect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGVideoDetect)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonFile();
	afx_msg void OnButtonCutpic();
	afx_msg void OnButtonSpeed();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonPause();
	afx_msg void OnButtonStart();
	afx_msg void OnCloseupComboArea();
	afx_msg void OnCloseupComboCam();
	afx_msg void OnCheckWriteDb();
	afx_msg void OnCheckFilename();
	afx_msg void OnCheckCamname();
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIDEODETECT_H__F1ECA381_984B_4062_8C2A_7552B1365DB4__INCLUDED_)
