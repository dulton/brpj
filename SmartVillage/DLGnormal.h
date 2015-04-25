#if !defined(AFX_DLGNORMAL_H__4E5F2C18_4EF5_4408_8B67_14A78DDBC9BF__INCLUDED_)
#define AFX_DLGNORMAL_H__4E5F2C18_4EF5_4408_8B67_14A78DDBC9BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGnormal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGnormal dialog

class CDLGnormal : public CDialog
{
// Construction
public:
	CDLGnormal(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGnormal)
	enum { IDD = IDD_NORMAL };
	CBitmapButton	m_b_set_car;
	CBitmapButton	m_b_set_black;
	CBitmapButton	m_b_channel;
	CBitmapButton	m_b_record_openall;
	CBitmapButton	m_b_record_open;
	CBitmapButton	m_b_preview_openall;
	CBitmapButton	m_b_preview_open;
	CBitmapButton	m_b_CarDetect_openall;
	CBitmapButton	m_b_CarDetect_open;

	CBitmapButton	m_b_history_video;
	CBitmapButton	m_b_history_CarDetect;
	CBitmapButton	m_b_history_CarAlarm;
	CBitmapButton	m_b_record_closeall;
	CBitmapButton	m_b_preview_closeall;
	CBitmapButton	m_b_CarDetect_closeall;

	CBitmapButton	m_b_capbmp;
	int		m_stream;
	int		m_screen;
	//}}AFX_DATA

	BOOL	OnInitDialog();
	void AutoSize();

		//ÆÁ±Î²Ù×÷
	void OnOK();
	void OnCancel();

	//lynn
	void UpdateNormalWnd(void);
	void StartPlay(int screenNo);

	void ChangePreviewFontPic(bool start);
	void ChangeCarDetectFontPic(bool start);
void ChangeFaceDetectFontPic(bool start) ;
	void ChangeRecordFontPic(bool start);

	void StopPlay(int screenNo);
	bool OpenCarDetect(int screenNo);
	void CloseCarDetect(int screenNo);
bool OpenFaceDetect(int screenNo);
void CloseFaceDetect(int screenNo);

void OnButtonOpenFaceDetect() ;

	void Capture(int screenNo);

	bool OpenRecord(int screenNo);
	void CloseRecord(int screenNo);



	afx_msg void OnButtonOpenPreview();
	afx_msg void OnButtonOpenCarDetect();


	afx_msg void OnButtonCapbmp();
	afx_msg void OnButtonOpenRecord();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGnormal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGnormal)
	afx_msg void OnPaint();
	afx_msg void OnCloseupComboScreen();
	afx_msg void OnCloseupComboStream();
	afx_msg void OnButtonFullScreen();
	afx_msg void OnButtonOpenchannel();

	afx_msg void OnButtonOpenPreviewAll();
	afx_msg void OnButtonClosePreviewAll();

	afx_msg void OnButtonOpenCarDetectAll();
	afx_msg void OnButtonCloseCarDetectAll();

	afx_msg void OnButtonOpenRecordAll();
	afx_msg void OnButtonCloseRecordAll();



	afx_msg void OnButtonSetBlack();
	afx_msg void OnButtonHistoryVideo();
	afx_msg void OnButtonHistoryCarDetect();
	afx_msg void OnButtonHistoryCarAlarm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNORMAL_H__4E5F2C18_4EF5_4408_8B67_14A78DDBC9BF__INCLUDED_)
