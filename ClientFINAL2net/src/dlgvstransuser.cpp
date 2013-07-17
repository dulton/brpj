// DlgVSTransUser.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgVSTransUser.h"
#include "Winsock2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CDlgVSTransUser dialog


CDlgVSTransUser::CDlgVSTransUser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVSTransUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVSTransUser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgVSTransUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVSTransUser)
	DDX_Control(pDX, IDC_LIST_VSTRANSUSER, m_List_VSTransUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVSTransUser, CDialog)
	//{{AFX_MSG_MAP(CDlgVSTransUser)
	ON_BN_CLICKED(ID_BTN_TRANSDEL, OnBtnTransdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVSTransUser message handlers

BOOL CDlgVSTransUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	str.LoadString(IDS_STRING218);
	m_List_VSTransUser.InsertColumn(0, str  , LVCFMT_LEFT, 60);
	str.LoadString(IDS_STRING219);
	m_List_VSTransUser.InsertColumn(1, str  , LVCFMT_LEFT, 110);
	str.LoadString(IDS_STRING220);
	m_List_VSTransUser.InsertColumn(2, str , LVCFMT_LEFT, 80);
	str.LoadString(IDS_STRING221);
	m_List_VSTransUser.InsertColumn(3, str  , LVCFMT_LEFT, 55);
	str.LoadString(IDS_STRING222);
	m_List_VSTransUser.InsertColumn(4, str  , LVCFMT_LEFT, 110);
	m_List_VSTransUser.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	UpdateListLoop();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgVSTransUser::UpdateListLoop(void) 
{
	int iRet,count,nItem;
	char lpstr[20];
	VSUSERINFO userinfo;
	struct   in_addr in;
	CString str;

	m_List_VSTransUser.DeleteAllItems();
	iRet = VSNET_RedirectorGetNormalUser(&userinfo,0);
	if(iRet >= 0)
	{
		count = iRet;
		TRACE("count %d\n",count);
	}
	else
	{
		AfxMessageBox(IDS_STRING223);
		return ;
	}
	if(count > 0 )
	{
		for(iRet=0;iRet<count;iRet++)
		{
			if(VSNET_RedirectorGetNormalUser(&userinfo,iRet))
			{
				sprintf(lpstr,"%d",iRet);
				nItem = m_List_VSTransUser.InsertItem(m_List_VSTransUser.GetItemCount(),lpstr);
				in.s_addr = userinfo.m_url;
				m_List_VSTransUser.SetItemText(nItem,1,inet_ntoa(in));
				m_List_VSTransUser.SetItemText(nItem,2,userinfo.m_sername);
				sprintf(lpstr,"%d",userinfo.m_ch);
				m_List_VSTransUser.SetItemText(nItem,3,lpstr);
				if(userinfo.trantype == 1)
				{
					str.LoadString(IDS_STRING224);
		        	strcpy(lpstr,str);
				}
				else if(userinfo.trantype == 2)
				{
					str.LoadString(IDS_STRING225);
					strcpy(lpstr,str);
				}
				else
				{
					str.LoadString(IDS_STRING226);
					strcpy(lpstr,str);
				}
				m_List_VSTransUser.SetItemText(nItem,4,lpstr);
			}
			else
			{
				AfxMessageBox(IDS_STRING227);
			}
		}
	}
}

void CDlgVSTransUser::OnBtnTransdel() 
{
	VSUSERINFO userinfo;
	if(m_List_VSTransUser.GetItemCount() < 1) return;

	if(m_List_VSTransUser.GetSelectedCount() == 0)   return;
	int nItem = m_List_VSTransUser.GetNextItem(-1, LVNI_SELECTED);
	if(VSNET_RedirectorGetNormalUser(&userinfo,nItem))
	{
		VSNET_RedirectorDelUser(&userinfo);
	}
	else
	{
		AfxMessageBox(IDS_STRING227);
	}
	UpdateListLoop();
}
