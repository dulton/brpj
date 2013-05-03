// DLGAnalyseFlowrate.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGAnalyseFlowrate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGAnalyseFlowrate dialog


CDLGAnalyseFlowrate::CDLGAnalyseFlowrate(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGAnalyseFlowrate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGAnalyseFlowrate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLGAnalyseFlowrate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGAnalyseFlowrate)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGAnalyseFlowrate, CDialog)
	//{{AFX_MSG_MAP(CDLGAnalyseFlowrate)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_ALL, OnButtonSearchAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_SINGLE, OnButtonSearchSingle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGAnalyseFlowrate message handlers
// CDLGBlackInOut message handlers
BOOL CDLGAnalyseFlowrate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_list.InitProgressColumn(3,4);
	m_list.InitProgressStyle(PBS_SMOOTH);
	m_list.InitProgressStyleEx(WS_EX_STATICEDGE);
	m_list.InitProgressColor(RGB(0,0,128),RGB(232,248,254));

	m_list.InsertColumn(0,"序号",LVCFMT_LEFT,100);
	m_list.InsertColumn(1,"设备名称",LVCFMT_LEFT,100);
	m_list.InsertColumn(2,"设备IP",LVCFMT_LEFT,100);
	m_list.InsertColumn(3,"统计数量",LVCFMT_LEFT,100);
	m_list.InsertColumn(4,"直方图",LVCFMT_LEFT,200);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	for(int i=0;i<10;i++)
	{
		CString temp;

			temp.Format("This %d is a a long string",i);

		m_list.InsertItem(i,temp);

		temp.Format("%d",i*10);
		m_list.SetItemText(i,3,temp);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGAnalyseFlowrate::OnOK() 
{
	// TODO: Add extra validation here
	
	
}
 

void CDLGAnalyseFlowrate::OnButtonSearchAll() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGAnalyseFlowrate::OnButtonSearchSingle() 
{
	// TODO: Add your control notification handler code here
	
}
