// DlgSerialParam.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgSerialParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialParam dialog


CDlgSerialParam::CDlgSerialParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSerialParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSerialParam)
	m_baud = _T("9600");
	m_data = 2;
	m_SerialPort = 0;
	m_stop = 0;
	m_very = 0;
	//}}AFX_DATA_INIT
}


void CDlgSerialParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSerialParam)
	DDX_CBString(pDX, IDC_COMBO_BAUD, m_baud);
	DDX_CBIndex(pDX, IDC_COMBO_DATA, m_data);
	DDX_CBIndex(pDX, IDC_COMBO_SERIALPORT, m_SerialPort);
	DDX_CBIndex(pDX, IDC_COMBO_STOP, m_stop);
	DDX_CBIndex(pDX, IDC_COMBO_VERY, m_very);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSerialParam, CDialog)
	//{{AFX_MSG_MAP(CDlgSerialParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialParam message handlers

void CDlgSerialParam::OnOK() 
{
	UpdateData();
	CDialog::OnOK();
}
