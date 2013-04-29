// DLGSetBlack.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGSetBlack.h"

#include "DLGBlackInOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DLGWarnning.h"

extern IO OracleIO;
/////////////////////////////////////////////////////////////////////////////
// CDLGSetBlack dialog

CDLGSetBlack::CDLGSetBlack(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSetBlack::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSetBlack)
	m_Brand = _T("");
	m_Name = _T("");
	m_Phone = _T("");
	m_Plate = _T("");
	m_Other = _T("");
	m_page = 0;
	//}}AFX_DATA_INIT
	ListChoose=-1;
}


void CDLGSetBlack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetBlack)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_BLACK_BRAND, m_Brand);
	DDV_MaxChars(pDX, m_Brand, 256);
	DDX_Text(pDX, IDC_EDIT_BLACK_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name,256);
	DDX_Text(pDX, IDC_EDIT_BLACK_PHONE, m_Phone);
	DDV_MaxChars(pDX, m_Phone, 256);
	DDX_Text(pDX, IDC_EDIT_BLACK_PLATE, m_Plate);
	DDV_MaxChars(pDX, m_Plate, 256);
	DDX_Text(pDX, IDC_EDIT_OTHER, m_Other);
	DDV_MaxChars(pDX, m_Other, 1024);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSetBlack, CDialog)
	//{{AFX_MSG_MAP(CDLGSetBlack)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_BLACK_ADD, OnButtonBlackAdd)
	ON_BN_CLICKED(IDC_BUTTON_BLACK_MODIFY, OnButtonBlackModify)
	ON_BN_CLICKED(IDC_BUTTON_BLACK_DELETE, OnButtonBlackdelete)
	ON_BN_CLICKED(IDC_BUTTON_BLACK_INPUT, OnButtonBlackInput)
	ON_BN_CLICKED(IDC_BUTTON_BLACK_OUPUT, OnButtonBlackOuput)
	ON_BN_CLICKED(IDC_BUTTON_BLACK_CLEAN, OnButtonBlackClean)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSetBlack message handlers
BOOL CDLGSetBlack::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 50);
	m_List.InsertColumn(1, _T("车辆品牌" ), LVCFMT_LEFT, 100);
	m_List.InsertColumn(2, _T("车牌号" ), LVCFMT_LEFT, 100);
	m_List.InsertColumn(3, _T("失主姓名" ), LVCFMT_LEFT, 100);
	m_List.InsertColumn(4, _T("联系电话" ), LVCFMT_LEFT, 120);
	m_List.InsertColumn(5, _T("其他信息" ), LVCFMT_LEFT, 200);
	m_List.InsertColumn(6, _T("id" ), LVCFMT_LEFT, 0);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//重新载入
	reflush();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGSetBlack::OnOK()
{
	// TODO: Add your control notification handler code here
}

void CDLGSetBlack::OnButtonBlackAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_Plate.IsEmpty())
	{
		MessageBox("车牌号不为空",MESSAGEBOX_TITLE);
		return ;
	}
	//插入数据库
	M2Struct();

#if ALLTAB_DETECT_CAR_MODE
	if(!OracleIO.CAR_BlackTable_AddNew(data))
#else
	if(!OracleIO.ELECAR_BlackTable_AddNew(data))
#endif
	{
		MessageBox("数据库错误：无法增加",MESSAGEBOX_TITLE);
		return ;
	}
	else
	{
		MessageBox("添加成功",MESSAGEBOX_TITLE);
	}

	//重新载入
	reflush();
}

void CDLGSetBlack::OnButtonBlackModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_Plate.IsEmpty())
	{
		MessageBox("车牌号不为空",MESSAGEBOX_TITLE);
		return ;
	}
	//插入数据库
	M2Struct();

#if ALLTAB_DETECT_CAR_MODE
	if(!OracleIO.CAR_BlackTable_Edit(data))
#else
	if(!OracleIO.ELECAR_BlackTable_Edit(data))
#endif
	{
		MessageBox("数据库错误：无法修改",MESSAGEBOX_TITLE);
		return ;
	}
	else
	{
		MessageBox("修改成功",MESSAGEBOX_TITLE);
	}

	//重新载入
	reflush();
}

void CDLGSetBlack::OnButtonBlackdelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//警告框
	CDLGWarnning dlgw;
	dlgw.m_wintxt="删除黑名单";					//窗口标题
	dlgw.m_warntxt="请再次确定是否删除";	//窗口内容
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{
		///删除
#if ALLTAB_DETECT_CAR_MODE
		if(!OracleIO.CAR_BlackTable_DeleteWithNid(data.nid))
#else
		if(!OracleIO.ELECAR_BlackTable_DeleteWithNid(data.nid))
#endif
		{
			MessageBox("数据库错误：无法删除",MESSAGEBOX_TITLE);
			return ;
		}
		else
		{
			MessageBox("删除成功",MESSAGEBOX_TITLE);
		}
	}
	else if (nResponse == IDCANCEL)
		return ;
	else
		return ;

	//重新载入
	reflush();
}

void CDLGSetBlack::OnButtonBlackInput() 
{
	// TODO: Add your control notification handler code here
	
	char szFilter[]="utf8 xml Files (*.xml)|*.xml|*.*||";
	CFileDialog dlg(TRUE,"xml","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		CDLGBlackInOut dlgBlackInout;
		dlgBlackInout.flag=BLACK_INOUT_INPUT;
		strcpy(dlgBlackInout.path,dlg.GetPathName().GetBuffer(0));
		dlgBlackInout.DoModal();
		reflush();
	}
}

void CDLGSetBlack::BlackInput() 
{
	// TODO: Add your control notification handler code here
	
	char szFilter[]="utf8 xml Files (*.xml)|*.xml|*.*||";
	CFileDialog dlg(TRUE,"xml","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		CDLGBlackInOut dlgBlackInout;
		dlgBlackInout.flag=BLACK_INOUT_INPUT;
		strcpy(dlgBlackInout.path,dlg.GetPathName().GetBuffer(0));
		dlgBlackInout.DoModal();
	}
}

void CDLGSetBlack::OnButtonBlackOuput() 
{
	// TODO: Add your control notification handler code here
	//保存文件
	char szFilter[]="utf8 xml Files (*.xml)|*.xml|*.*||";
	CFileDialog dlg(FALSE,"xml","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		CDLGBlackInOut dlgBlackInout;
		dlgBlackInout.flag=BLACK_INOUT_OUTPUT;
		strcpy(dlgBlackInout.path,dlg.GetPathName().GetBuffer(0));
		dlgBlackInout.DoModal();
		reflush();
	}
}

void CDLGSetBlack::BlackOuput() 
{
	// TODO: Add your control notification handler code here
	//保存文件
	char szFilter[]="utf8 xml Files (*.xml)|*.xml|*.*||";
	CFileDialog dlg(FALSE,"xml","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		CDLGBlackInOut dlgBlackInout;
		dlgBlackInout.flag=BLACK_INOUT_OUTPUT;
		strcpy(dlgBlackInout.path,dlg.GetPathName().GetBuffer(0));
		dlgBlackInout.DoModal();
	}
}

void CDLGSetBlack::OnButtonBlackClean() 
{
	// TODO: Add your control notification handler code here
	CDLGWarnning dlgw;
	dlgw.m_wintxt="黑名单数据清空警告";
	dlgw.m_warntxt="数据将被清空";
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{

#if ALLTAB_DETECT_CAR_MODE
		OracleIO.CAR_BlackTable_DeleteAll();
#else
		OracleIO.ELECAR_BlackTable_DeleteAll();
#endif
		//重新载入
		reflush();
	}
	else if (nResponse == IDCANCEL)
		return ;
	else
		return ;
}


void CDLGSetBlack::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
	
}

void CDLGSetBlack::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-SET_BLACK_PAGE_MAX_NUM) >= 0)
		ListNow-=SET_BLACK_PAGE_MAX_NUM;

	DisplayerList();

}

void CDLGSetBlack::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+SET_BLACK_PAGE_MAX_NUM) < ListTotal)
		ListNow+=SET_BLACK_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGSetBlack::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+SET_BLACK_PAGE_MAX_NUM) < ListTotal)
		ListNow+=SET_BLACK_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGSetBlack::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/SET_BLACK_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+SET_BLACK_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+SET_BLACK_PAGE_MAX_NUM) < (SET_BLACK_PAGE_MAX_NUM*m_page))
			)
			ListNow+=SET_BLACK_PAGE_MAX_NUM;

		DisplayerList();
	}	
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);
}

void CDLGSetBlack::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;
	//选中的赋值
	ListChoose= p->iItem;
	//没中
	if(-1 == ListChoose)
		return ;
	//显示
	char str[ZOG_MAX_PATH_STR];
	m_List.GetItemText(ListChoose,1,data.brand,256);
	m_List.GetItemText(ListChoose,2,data.plate,256);
	m_List.GetItemText(ListChoose,3,data.name,256);
	m_List.GetItemText(ListChoose,4,data.Phone,256);
	m_List.GetItemText(ListChoose,5,data.other,1024);

	m_List.GetItemText(ListChoose,6,str,ZOG_MAX_PATH_STR);
	sscanf(str,"%d",&(data.nid));

	Struct2M();

	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_BLACK_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_BLACK_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BLACK_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BLACK_DELETE)->EnableWindow(FALSE);
	}

	//更新
	UpdateData(FALSE);

	*pResult = 0;
}

void CDLGSetBlack::reflush() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ListTotal=0;

#if ALLTAB_DETECT_CAR_MODE
	ListTotal=OracleIO.CAR_BlackTable_GetNum();
#else
	ListTotal=OracleIO.ELECAR_BlackTable_GetNum();
#endif

	ListNow=0;

	DisplayerList();
}

void CDLGSetBlack::DisplayerList(void)
{
	list_black.clear();
	m_List.DeleteAllItems();
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
	ListChoose=-1;
	Clear();

	if(0==ListTotal)
		return ;

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+SET_BLACK_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+SET_BLACK_PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	//查询数据库
#if ALLTAB_DETECT_CAR_MODE
	OracleIO.CAR_BlackTable_Read(si,ei,list_black);
#else
	OracleIO.ELECAR_BlackTable_Read(si,ei,list_black);
#endif

	int nItem;
	char str[128];
	int i=0;

	list<struct BLACK_DATA_ST>::iterator beglist;

	for(beglist=list_black.begin();beglist!=list_black.end();beglist++)
	{
		sprintf(str,"%07d",ListNow+1+i);
		nItem = m_List.InsertItem(0,str);
		i++;

		m_List.SetItemText(nItem,1,beglist->brand);
		m_List.SetItemText(nItem,2,beglist->plate);
		m_List.SetItemText(nItem,3,beglist->name);
		m_List.SetItemText(nItem,4,beglist->Phone);
		m_List.SetItemText(nItem,5,beglist->other);
		
		sprintf(str,"%d",beglist->nid);
		m_List.SetItemText(nItem,6,str);
	}

	if(ListTotal%SET_BLACK_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/SET_BLACK_PAGE_MAX_NUM+1,ListTotal/SET_BLACK_PAGE_MAX_NUM+1);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/SET_BLACK_PAGE_MAX_NUM+1,ListTotal/SET_BLACK_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);

	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_BLACK_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_BLACK_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BLACK_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BLACK_DELETE)->EnableWindow(FALSE);
	}
}

void CDLGSetBlack::Clear() 
{
	m_Brand = _T("");
	m_Name = _T("");
	m_Phone = _T("");
	m_Plate = _T("");
	m_Other = _T("");
	UpdateData(FALSE);
}

void CDLGSetBlack::Struct2M(void)
{
	m_Brand=data.brand;
	m_Name =data.name;
	m_Phone = data.Phone;
	m_Plate = data.plate;
	m_Other = data.other;
}

void CDLGSetBlack::M2Struct(void)
{
	strcpy(data.brand,m_Brand.GetBuffer(0));
	strcpy(data.name,m_Name.GetBuffer(0));
	strcpy(data.Phone,m_Phone.GetBuffer(0));
	strcpy(data.plate,m_Plate.GetBuffer(0));
	strcpy(data.other,m_Other.GetBuffer(0));
}