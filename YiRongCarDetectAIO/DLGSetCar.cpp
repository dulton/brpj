// DLGSetCar.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGSetCar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGSetCar dialog


CDLGSetCar::CDLGSetCar(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSetCar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSetCar)
	m_defaultProvince = _T("Ãö");
	m_reliability = 50;
	m_JPGquality = 75;
	m_RectDown = 90;
	m_RectLeft = 0;
	m_RectRight = 100;
	m_RectUp = 10;
	m_MinWidth = 60;
	m_MaxWidth = 200;
	//}}AFX_DATA_INIT
}


void CDLGSetCar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetCar)
	DDX_CBString(pDX, IDC_DEFAULT_PROVINCE, m_defaultProvince);
	DDX_Text(pDX, IDC_EDIT_RELIABILITY, m_reliability);
	DDV_MinMaxInt(pDX, m_reliability, 0, 100);
	DDX_Text(pDX, IDC_EDIT_JPG_QUALITY, m_JPGquality);
	DDX_Text(pDX, IDC_EDIT_DOWN, m_RectDown);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_RectLeft);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_RectRight);
	DDX_Text(pDX, IDC_EDIT_UP, m_RectUp);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH, m_MinWidth);
	DDX_Text(pDX, IDC_EDIT_MAX_WIDTH, m_MaxWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSetCar, CDialog)
	//{{AFX_MSG_MAP(CDLGSetCar)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSetCar message handlers

BOOL CDLGSetCar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_CHECK_OUTPUT_RECT))->SetCheck(CarSet.RedRect);
	((CButton*)GetDlgItem(IDC_CHECK_DESKEW))->SetCheck(CarSet.Deskew);
	((CButton*)GetDlgItem(IDC_CHECK_CAR_COLOR))->SetCheck(CarSet.CarColor);
	for (int i=0; i<CAR_MASK_MAX; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK1+i))->SetCheck(CarSet.Mask[i]);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGSetCar::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	strcpy(CarSet.DefaultChar,m_defaultProvince.GetBuffer(0));
	CarSet.Reliability = m_reliability;
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
	
	for(int i=0;i<CAR_MASK_MAX;i++)
	{
		int state = ((CButton*)GetDlgItem(IDC_CHECK1+i))->GetCheck();
		CarSet.Mask[i] =  state ? 0:1;
	}	
}

void CDLGSetCar::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	int state = ((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck();

	for (int i=0; i<CAR_MASK_MAX; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK1+i))->SetCheck(state);
	}	
}

void CDLGSetCar::ReadOldConfig(struct CarSetStruct OldCarSet)
{
	strcpy(m_defaultProvince.GetBuffer(0),OldCarSet.DefaultChar);
	m_reliability = OldCarSet.Reliability;
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
		CarSet.Mask[i] = OldCarSet.Mask[i] ? 0:1;
	}
}

