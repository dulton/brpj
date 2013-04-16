// DLGVideoList.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGVideoList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoList dialog


CDLGVideoList::CDLGVideoList(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGVideoList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGVideoList)
	m_page = 0;
	m_CheckTime=FALSE;
	m_runnum = _T("");
	m_tag = _T("");
	//}}AFX_DATA_INIT
	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime;
	m_StartHour=nowtime;
	m_EndMon=nowtime;
	m_EndHour=nowtime;

	ListTotal=0;
	ListNow=0;
	ListChoose=-1;
}


void CDLGVideoList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGVideoList)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_TIME, m_CheckTime);
	DDX_Text(pDX, IDC_EDIT_RUNNUM, m_runnum);
	DDX_Text(pDX, IDC_EDIT_TAG, m_tag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGVideoList, CDialog)
	//{{AFX_MSG_MAP(CDLGVideoList)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST,OnLvnItemActivateList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGVideoList message handlers
BOOL CDLGVideoList::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGVideoList::OnOK() 
{
	// TODO: Add extra validation here
	

}

void CDLGVideoList::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGVideoList::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDLGVideoList::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	char str[260];


//	m_list.GetItemText(pNMIA->iItem,10,str,260);


	ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);

	*pResult = 0;
}
