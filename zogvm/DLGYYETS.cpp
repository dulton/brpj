// DLGYYETS.cpp : implementation file
//

#include "stdafx.h"
#include "zogvm.h"
#include "DLGYYETS.h"
#include "video.h"

// CDLGYYETS dialog

IMPLEMENT_DYNAMIC(CDLGYYETS, CDialog)

CDLGYYETS::CDLGYYETS(CWnd* pParent /*=NULL*/)
: CDialog(CDLGYYETS::IDD, pParent)
{
	m_sreach=_T("");
	m_page = 1;
}

CDLGYYETS::~CDLGYYETS()
{
	YyetsDB.CloseDB();
}

void CDLGYYETS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SREACH, m_sreach);
	DDV_MaxChars(pDX, m_sreach, 100);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_first_button);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_pre_button);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_next_button);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_last_button);
	DDX_Control(pDX, IDC_BUTTON_JUMP, m_jump_button);
		DDX_Check(pDX, IDC_CHECK, m_check);
}


BEGIN_MESSAGE_MAP(CDLGYYETS, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDLGYYETS::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_MAGNET, &CDLGYYETS::OnBnClickedButtonMagnet)
	ON_BN_CLICKED(IDC_BUTTON_ED2K, &CDLGYYETS::OnBnClickedButtonEd2k)
	ON_BN_CLICKED(IDC_BUTTON_WANPAN, &CDLGYYETS::OnBnClickedButtonWanpan)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
		ON_BN_CLICKED(IDC_CHECK, OnCheck)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CDLGYYETS::OnInitDialog()
{
	CDialog::OnInitDialog();

	FILE* fp=fopen("yyets.db","rb");
	if(fp)
	{
		fclose(fp);
		YyetsDB.OpenDB("yyets.db");
	}
	else
	{
		MessageBox("yyets数据库未找到","错误");
		return FALSE;
	}

	m_list.InsertColumn(0, _T("序列号") , LVCFMT_LEFT, 70);
	m_list.InsertColumn(1, _T("片名") , LVCFMT_LEFT, 600);
	m_list.InsertColumn(2, _T("容量") , LVCFMT_LEFT, 70);
	m_list.InsertColumn(3, _T("磁力链接") , LVCFMT_LEFT, 200);
	m_list.InsertColumn(4, _T("电驴链接") , LVCFMT_LEFT, 200);
	m_list.InsertColumn(5, _T("网盘链接") , LVCFMT_LEFT, 200);

	//带复选框 LVS_EX_CHECKBOXES
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);


	//初始即最大化 //放最后
	ShowWindow(SW_MAXIMIZE);   


	return TRUE;
}
void CDLGYYETS::OnOK() 
{
	// TODO: Add extra validation here

	//	CDialog::OnOK();
}
void CDLGYYETS::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect		m_clientRect;		//程序界面区域位置
	GetClientRect(&m_clientRect);

	int height=70;

	CRect list_Rect;
	list_Rect.top = 	m_clientRect.top+height;
	list_Rect.bottom = m_clientRect.bottom-30;
	list_Rect.left = m_clientRect.left ;
	list_Rect.right = m_clientRect.right;
	//必须 样式=重叠，边框=调整大小
	m_list.MoveWindow(list_Rect);

	int buttonh=25;
	int buttonbmpw=24,buttonbmph=18,buttongap=10;

	CRect b_Rect;

	b_Rect.top = list_Rect.bottom+5 ;
	b_Rect.bottom =	b_Rect.top+buttonbmph;

	b_Rect.left = list_Rect.left+10;
	b_Rect.right = b_Rect.left+200;
	GetDlgItem(IDC_STATIC_INFO)->MoveWindow(b_Rect);

	printf_Rect=b_Rect;

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_first_button.MoveWindow(b_Rect);

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_pre_button.MoveWindow(b_Rect);

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_next_button.MoveWindow(b_Rect);

	b_Rect.left = b_Rect.right+buttongap;
	b_Rect.right = b_Rect.left+buttonbmpw;
	m_last_button.MoveWindow(b_Rect);
	//////////////

	b_Rect.top = list_Rect.bottom+8 ;
	b_Rect.left = b_Rect.right+buttongap*3;
	b_Rect.right = b_Rect.left+20;
	GetDlgItem(IDC_STATIC_DI)->MoveWindow(b_Rect);

	b_Rect.top = list_Rect.bottom+5 ;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+60;
	GetDlgItem(IDC_EDIT_PAGE)->MoveWindow(b_Rect);

	b_Rect.top = list_Rect.bottom+2 ;
	b_Rect.bottom =	b_Rect.top+24;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+20;
	m_jump_button.MoveWindow(b_Rect);

	b_Rect.top = list_Rect.bottom+8 ;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+20;
	GetDlgItem(IDC_STATIC_YE)->MoveWindow(b_Rect);

	Invalidate();
}

// CDLGYYETS message handlers

void CDLGYYETS::OnBnClickedButtonFind()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ListTotal=0;

	if(m_sreach.IsEmpty())
		ListTotal=YyetsDB.YYETS_Number();
	else
		ListTotal=YyetsDB.YYETS_NumberForSearch(m_sreach.GetBuffer(0));

	ListNow=0;

	DisplayerList();
}

void CDLGYYETS::OnBnClickedButtonMagnet()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i;
	CString str="";
	char tempstr[32]={0};
	long long nid;

	if(yyetsList.size() <=0)
		return ;

	list<struct YYETS_ST>::iterator beglist;

	ClearClipboradBuffer();

	for( i=0;i<m_list.GetItemCount();i++)
	{
		if(m_list.GetCheck(i))
		{
			m_list.GetItemText(i,0,tempstr,32);
			sscanf(tempstr,"%I64u",&nid);

			for(beglist=yyetsList.begin();beglist!=yyetsList.end();beglist++)
			{
				if(beglist->yyets_nid == nid)
				{
					str+=beglist->magnet;
					str+"\n\t\n";
					break;
				}
			}
		}
	}
	
	AppendBufferToClipboard(str.GetBuffer(0),str.GetLength());

}

void CDLGYYETS::OnBnClickedButtonEd2k()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i;
	CString str="";
	char tempstr[32]={0};
	long long nid;

	if(yyetsList.size() <=0)
		return ;

	list<struct YYETS_ST>::iterator beglist;

	ClearClipboradBuffer();

	for( i=0;i<m_list.GetItemCount();i++)
	{
		if(m_list.GetCheck(i))
		{
			m_list.GetItemText(i,0,tempstr,32);
			sscanf(tempstr,"%I64u",&nid);

			for(beglist=yyetsList.begin();beglist!=yyetsList.end();beglist++)
			{
				if(beglist->yyets_nid == nid)
				{
					str+=beglist->ed2k;
					str+"\n\t\n";
					break;
				}
			}
		}
	}

	AppendBufferToClipboard(str.GetBuffer(0),str.GetLength());
}

void CDLGYYETS::OnBnClickedButtonWanpan()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i;
	CString str="";
	char tempstr[32]={0};
	long long nid;

	if(yyetsList.size() <=0)
		return ;

	list<struct YYETS_ST>::iterator beglist;

	ClearClipboradBuffer();

	for( i=0;i<m_list.GetItemCount();i++)
	{
		if(m_list.GetCheck(i))
		{
			m_list.GetItemText(i,0,tempstr,32);
			sscanf(tempstr,"%I64u",&nid);

			for(beglist=yyetsList.begin();beglist!=yyetsList.end();beglist++)
			{
				if(beglist->yyets_nid == nid)
				{
					str+=beglist->wanpan;
					str+"\n\t\n";
					break;
				}
			}
		}
	}

	AppendBufferToClipboard(str.GetBuffer(0),str.GetLength());
}

void CDLGYYETS::DisplayerList()
{
	m_list.DeleteAllItems();
	yyetsList.clear();

	if(0==ListTotal)
	{
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
		InvalidateRect(printf_Rect, TRUE);
		return ;
	}

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+PAGE_MAX_NUM ;
	else
		ei=ListTotal;


	if(m_sreach.IsEmpty())
		YyetsDB.YYETS_Read(yyetsList,si-1,ei-(si-1));
	else
		YyetsDB.YYETS_ReadForSearch(m_sreach.GetBuffer(0),yyetsList,si-1,ei-(si-1));

	if(yyetsList.size()<=0)
		return ;

	list<struct YYETS_ST>::iterator beglist;
	char str[128];
	int i=0;

	for(beglist=yyetsList.begin();beglist!=yyetsList.end();beglist++)
	{
		sprintf(str,"%I64u",beglist->yyets_nid);
		m_list.InsertItem( i,str);
		m_list.SetItemText(i,1,beglist->name);
		m_list.SetItemText(i,2,beglist->filesize);
		m_list.SetItemText(i,3,beglist->magnet);
		m_list.SetItemText(i,4,beglist->ed2k);
		m_list.SetItemText(i,5,beglist->wanpan);

		i++;
	}
	
	if(ListTotal%PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/PAGE_MAX_NUM+1,ListTotal/PAGE_MAX_NUM+1);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/PAGE_MAX_NUM+1,ListTotal/PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	InvalidateRect(printf_Rect, TRUE);


	UpdateData(FALSE);
}


void CDLGYYETS::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGYYETS::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-PAGE_MAX_NUM) >= 0)
		ListNow-=PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGYYETS::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+PAGE_MAX_NUM) < ListTotal)
		ListNow+=PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGYYETS::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+PAGE_MAX_NUM) < ListTotal)
		ListNow+=PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGYYETS::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+PAGE_MAX_NUM) < (PAGE_MAX_NUM*m_page))
			)
			ListNow+=PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围");

}


void CDLGYYETS::OnCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	for(int i=0;i<m_list.GetItemCount();i++)
		m_list.SetCheck(i,m_check);
}

