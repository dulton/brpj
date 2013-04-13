// DLGProductList.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGProductList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGProductList dialog


CDLGProductList::CDLGProductList(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGProductList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGProductList)
	m_image1 = _T("");
	m_image2 = _T("");
	m_image3 = _T("");
	m_product_name = _T("");
	m_search_name = _T("");
	m_spec = _T("");
	m_search_barcode = _T("");
	m_product_barcode = _T("");
	m_page = 0;
	//}}AFX_DATA_INIT
}


void CDLGProductList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGProductList)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_IMAGE1, m_image1);
	DDV_MaxChars(pDX, m_image1, 260);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_IMAGE2, m_image2);
	DDV_MaxChars(pDX, m_image2, 260);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_IMAGE3, m_image3);
	DDV_MaxChars(pDX, m_image3, 260);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_NAME, m_product_name);
	DDV_MaxChars(pDX, m_product_name, 256);
	DDX_Text(pDX, IDC_EDIT_SEARCH_NAME, m_search_name);
	DDV_MaxChars(pDX, m_search_name, 256);
	DDX_Text(pDX, IDC_EDIT_SPEC, m_spec);
	DDV_MaxChars(pDX, m_spec, 2048);
	DDX_Text(pDX, IDC_EDIT_SEARCH_BARCODE, m_search_barcode);
	DDV_MaxChars(pDX, m_search_barcode, 64);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_BARCODE, m_product_barcode);
	DDV_MaxChars(pDX, m_product_barcode, 64);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGProductList, CDialog)
	//{{AFX_MSG_MAP(CDLGProductList)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE1, OnButtonImage1)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE2, OnButtonImage2)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE3, OnButtonImage3)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGProductList message handlers
BOOL CDLGProductList::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDLGProductList::OnOK() 
{
	// TODO: Add extra validation here
	

}

void CDLGProductList::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGProductList::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGProductList::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGProductList::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGProductList::OnButtonImage1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGProductList::OnButtonImage2() 
{
	// TODO: Add your control notification handler code here
	
}

void CDLGProductList::OnButtonImage3() 
{
	// TODO: Add your control notification handler code here
	
}



void CDLGProductList::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
	
}

void CDLGProductList::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-SET_PRODUCT_PAGE_MAX_NUM) >= 0)
		ListNow-=SET_PRODUCT_PAGE_MAX_NUM;

	DisplayerList();

}

void CDLGProductList::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+SET_PRODUCT_PAGE_MAX_NUM) < ListTotal)
		ListNow+=SET_PRODUCT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGProductList::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+SET_PRODUCT_PAGE_MAX_NUM) < ListTotal)
		ListNow+=SET_PRODUCT_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGProductList::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/SET_PRODUCT_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+SET_PRODUCT_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+SET_PRODUCT_PAGE_MAX_NUM) < (SET_PRODUCT_PAGE_MAX_NUM*m_page))
			)
			ListNow+=SET_PRODUCT_PAGE_MAX_NUM;

		DisplayerList();
	}	
	else
		MessageBox("不在页面范围");
}

void CDLGProductList::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* p=	(NM_LISTVIEW *)pNMHDR;
	//选中的赋值
	ListChoose= p->iItem;
	//没中
	if(-1 == ListChoose)
		return ;
	/*
	//显示
	char str[260];
	m_List.GetItemText(ListChoose,1,data.brand,256);
	m_List.GetItemText(ListChoose,2,data.plate,256);
	m_List.GetItemText(ListChoose,3,data.name,256);
	m_List.GetItemText(ListChoose,4,data.Phone,256);
	m_List.GetItemText(ListChoose,5,data.other,1024);

	m_List.GetItemText(ListChoose,6,str,260);
	sscanf(str,"%d",&(data.nid));

	Struct2M();
*/
	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}

	//更新
	UpdateData(FALSE);

	*pResult = 0;
}

void CDLGProductList::reflush() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ListTotal=0;
/*
#if ALLTAB_DETECT_CAR_MODE
	ListTotal=OracleIO.CAR_BlackTable_GetNum();
#else
	ListTotal=OracleIO.ELECAR_BlackTable_GetNum();
#endif
*/
	ListNow=0;

	DisplayerList();
	
	if(-1!=ListChoose)
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}
}

void CDLGProductList::DisplayerList(void)
{
	list_product.clear();
	m_list.DeleteAllItems();
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
	ListChoose=-1;
	Clear();

	if(0==ListTotal)
		return ;

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+SET_PRODUCT_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+SET_PRODUCT_PAGE_MAX_NUM ;
	else
		ei=ListTotal;
/*
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


	if(ListTotal%SET_PRODUCT_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/SET_PRODUCT_PAGE_MAX_NUM+1,ListTotal/SET_PRODUCT_PAGE_MAX_NUM+1);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页",
			ListTotal,
			ListNow/SET_PRODUCT_PAGE_MAX_NUM+1,ListTotal/SET_PRODUCT_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
*/
}

void CDLGProductList::Clear() 
{
	/*
	m_Brand = _T("");
	m_Name = _T("");
	m_Phone = _T("");
	m_Plate = _T("");
	m_Other = _T("");
	UpdateData(FALSE);
	*/
}

void CDLGProductList::Struct2M(void)
{
	/*
	m_Brand=data.brand;
	m_Name =data.name;
	m_Phone = data.Phone;
	m_Plate = data.plate;
	m_Other = data.other;
	*/
}

void CDLGProductList::M2Struct(void)
{
	/*
	strcpy(data.brand,m_Brand.GetBuffer(0));
	strcpy(data.name,m_Name.GetBuffer(0));
	strcpy(data.Phone,m_Phone.GetBuffer(0));
	strcpy(data.plate,m_Plate.GetBuffer(0));
	strcpy(data.other,m_Other.GetBuffer(0));
	*/
}
