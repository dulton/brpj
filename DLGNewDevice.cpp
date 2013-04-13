// DLGNewDevice.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGNewDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGNewDevice dialog


CDLGNewDevice::CDLGNewDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGNewDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGNewDevice)
	m_ip = _T("");
	m_name = _T("");
	m_port = 0;
	m_pwd = _T("");
	m_user = _T("");
	//}}AFX_DATA_INIT
}


void CDLGNewDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGNewDevice)
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDV_MaxChars(pDX, m_ip, 32);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 32);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_PWD, m_pwd);
	DDV_MaxChars(pDX, m_pwd, 32);
	DDX_Text(pDX, IDC_EDIT_USER, m_user);
	DDV_MaxChars(pDX, m_user, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGNewDevice, CDialog)
	//{{AFX_MSG_MAP(CDLGNewDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGNewDevice message handlers
BOOL CDLGNewDevice::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGNewDevice::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDLGNewDevice::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
