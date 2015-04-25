#pragma once
#include "ContainerWnd.h"
#include "DLGPictureLite.h"
// CDLGPictureFace dialog
#include "math.h"

#define WM_ADDFACE_MESSAGE    WM_USER + 101
#define WM_CLEANFACE_MESSAGE    WM_USER + 102


#include <vector>
#include <list>
using namespace::std;
class CDLGPictureFace : public CDialog
{
	DECLARE_DYNAMIC(CDLGPictureFace)

public:
	CDLGPictureFace(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGPictureFace();
	BOOL	OnInitDialog();
	void AutoSize();


	//可以显示图片的标记
	bool TitleDrawFlag;
	int TitleResDef;
	


	CContainerWnd m_list;
void CleanList(void) ;

	void InitList(void);
	void MoveList(CRect Rect);
	void MoveListScroll(CRect Rect);
	void MoveListWin(CRect Rect);
	

	int DlgPLiteTotal;

	int Liteheight;
	int Litewidth;
	CDLGPictureLite	**DlgPictureLite;

void DisplayCapList(list<struct FACE_CAPTURE_ST> &faceList,int AlarmFlag);
void DisplayAlarmLiteList(list<struct FACE_ALARM_VIEW_ST> &faceList,int AlarmFlag) ;

void DisplayRecgTEMPList(list<struct TEMP_CMP_RESULT_ST> &faceList,int AlarmFlag) ;
void CDLGPictureFace::DisplayRecgBLACKList(list<struct BLACK_CMP_RESULT_ST> &faceList,int AlarmFlag) ;

void AddCapList(struct FACE_CAPTURE_ST facedata,int AlarmFlag) ;
char* FaceSex(int i);

// Dialog Data
	enum { IDD = IDD_PICTURE_FACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg LRESULT OnAddMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCleanMessage(WPARAM wParam,LPARAM lParam);
};
