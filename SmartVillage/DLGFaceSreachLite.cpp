// DLGFaceSreachLite.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGFaceSreachLite.h"


#include "SmartVillageDlg.h"
#include "DLGWarnning.h"

////////////////lynn/////////////////
extern CYRSVMySQL MySqlIO;
extern CSmartVillageDlg *DlgMain;
////////////////lynn/////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

// CDLGFaceSreachLite dialog

IMPLEMENT_DYNAMIC(CDLGFaceSreachLite, CDialog)

CDLGFaceSreachLite::CDLGFaceSreachLite(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGFaceSreachLite::IDD, pParent)
	, m_age_start(1)
	, m_age_end(99)
	, m_c_sex(0)
{
	m_CheckTime=FALSE;

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime.GetTime();
	m_StartHour=nowtime.GetTime();
	CTime cstime(
		nowtime.GetYear(),
		nowtime.GetMonth(),
		nowtime.GetDay(),
		0,	0,	0);

	m_StartHour=cstime.GetTime();

	nowtime+=3600;
	m_EndMon=nowtime.GetTime();
	m_EndHour=nowtime.GetTime();
}

CDLGFaceSreachLite::~CDLGFaceSreachLite()
{
}

void CDLGFaceSreachLite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AGE_START, m_age_start);
	DDV_MinMaxInt(pDX, m_age_start, 0, 100);
	DDX_Text(pDX, IDC_EDIT_AGE_END, m_age_end);
	DDV_MinMaxInt(pDX, m_age_end, 0, 100);

	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_TIME, m_CheckTime);
	DDX_CBIndex(pDX, IDC_COMBO_SEX, m_c_sex);

}


BEGIN_MESSAGE_MAP(CDLGFaceSreachLite, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGFaceSreachLite::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGFaceSreachLite::OnCancel)
			ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CDLGFaceSreachLite::OnInitDialog()
{
	CDialog::OnInitDialog();


	OnCheckTime();
	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGFaceSreachLite::OnCheckTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_CheckTime)
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDHOUR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDHOUR)->EnableWindow(FALSE);
	}
}
void CDLGFaceSreachLite::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);

	Invalidate();
}

// CDLGFaceSreachLite message handlers

void CDLGFaceSreachLite::OnOk()
{
	// TODO: Add your control notification handler code here
	//	CDialog::OnOK();
}

void CDLGFaceSreachLite::OnCancel()
{
	// TODO: Add your control notification handler code here
	DlgMain->OnCancel();
}

void CDLGFaceSreachLite::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_FIND_BACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	CDialog::OnPaint();
}

//静态文本控件 透明
HBRUSH CDLGFaceSreachLite::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); // 设置透明背景
		// TODO: Change any attributes of the DC here
		pDC->SetTextColor(RGB(0, 0, 0)); // 设置文本颜色
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		hbr=(HBRUSH)GetStockObject(HOLLOW_BRUSH); // 返回透明画刷	
		// TODO: Return a different brush if the default is not desired
	}

	return hbr;
}

