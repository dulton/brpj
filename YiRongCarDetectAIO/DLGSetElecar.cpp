// DLGSetElecar.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGSetElecar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGSetElecar dialog


CDLGSetElecar::CDLGSetElecar(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSetElecar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSetElecar)
	m_JPGquality = 75;
	m_RectDown = 90;
	m_RectLeft = 0;
	m_RectRight = 100;
	m_RectUp = 10;
	m_MaxWidth = 60;
	m_MinWidth = 200;
	//}}AFX_DATA_INIT
}


void CDLGSetElecar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetElecar)
	DDX_Text(pDX, IDC_EDIT_JPG_QUALITY, m_JPGquality);
	DDV_MinMaxInt(pDX, m_JPGquality, 1, 100);
	DDX_Text(pDX, IDC_EDIT_DOWN, m_RectDown);
	DDV_MinMaxInt(pDX, m_RectDown, 0, 100);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_RectLeft);
	DDV_MinMaxInt(pDX, m_RectLeft, 0, 100);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_RectRight);
	DDV_MinMaxInt(pDX, m_RectRight, 0, 100);
	DDX_Text(pDX, IDC_EDIT_UP, m_RectUp);
	DDV_MinMaxInt(pDX, m_RectUp, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MAX_WIDTH, m_MaxWidth);
	DDV_MinMaxInt(pDX, m_MaxWidth, 60, 200);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH, m_MinWidth);
	DDV_MinMaxInt(pDX, m_MinWidth, 60, 200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSetElecar, CDialog)
	//{{AFX_MSG_MAP(CDLGSetElecar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSetElecar message handlers

BOOL CDLGSetElecar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_CHECK_OUTPUT_RECT))->SetCheck(CarSet.RedRect);
	((CButton*)GetDlgItem(IDC_CHECK_DESKEW))->SetCheck(CarSet.Deskew);
	((CButton*)GetDlgItem(IDC_CHECK_CAR_COLOR))->SetCheck(CarSet.CarColor);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGSetElecar::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	CarSet.RedRect = ((CButton*)GetDlgItem(IDC_CHECK_OUTPUT_RECT))->GetCheck();
	CarSet.Deskew = ((CButton*)GetDlgItem(IDC_CHECK_DESKEW))->GetCheck();
	CarSet.CarColor = ((CButton*)GetDlgItem(IDC_CHECK_CAR_COLOR))->GetCheck();
	CarSet.JPGquality = m_JPGquality;
	CarSet.MaxWidth = m_MaxWidth;
	CarSet.MinWidth = m_MinWidth;
	CarSet.RangeRate.x0 = m_RectLeft;
	CarSet.RangeRate.x1 = m_RectRight;
	CarSet.RangeRate.y0 = m_RectUp;
	CarSet.RangeRate.y1 = m_RectDown;
}

void CDLGSetElecar::ReadOldConfig(struct CarSetStruct OldCarSet)
{
	strcpy(CarSet.DefaultChar,OldCarSet.DefaultChar);
	CarSet.Reliability = OldCarSet.Reliability;
	CarSet.RedRect = OldCarSet.RedRect;
	CarSet.Deskew = OldCarSet.Deskew;
	CarSet.CarColor = OldCarSet.CarColor;
	m_JPGquality = OldCarSet.JPGquality;	
	m_MaxWidth = OldCarSet.MaxWidth;
	m_MinWidth = OldCarSet.MinWidth;
	m_RectLeft = OldCarSet.RangeRate.x0;
	m_RectRight = OldCarSet.RangeRate.x1;
	m_RectUp = OldCarSet.RangeRate.y0;
	m_RectDown = OldCarSet.RangeRate.y1;

	for (int i=0; i<CAR_MASK_MAX; i++)
	{
		CarSet.Mask[i] = OldCarSet.Mask[i];
	}
}
