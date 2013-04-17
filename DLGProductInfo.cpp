// DLGProductInfo.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGProductInfo.h"

#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGProductInfo dialog


CDLGProductInfo::CDLGProductInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGProductInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGProductInfo)
	m_barcode = _T("");
	//}}AFX_DATA_INIT

	curchoose=0;

	//清空 产品
	for(int i=0;i<MAX_PLAYWIN;i++)
		memset(&data[i],0,sizeof(struct PRODUCT_INFO_ST));
}


void CDLGProductInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGProductInfo)
	DDX_Text(pDX, IDC_EDIT_BARCODE, m_barcode);
	DDV_MaxChars(pDX, m_barcode, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGProductInfo, CDialog)
	//{{AFX_MSG_MAP(CDLGProductInfo)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGProductInfo message handlers

BOOL CDLGProductInfo::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();
	
	AutoSize();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGProductInfo::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);


	Invalidate();
}

void CDLGProductInfo::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_barcode.IsEmpty())
	{
		MessageBox("Please Input Running Number or Carton TAG");
		return ;
	}

	//查询数据库
	if(!pCMainDlg->SQLiteIO.Product_Read(m_barcode.GetBuffer(0),data[curchoose]))
		MessageBox("This Product can not be found");
	else
	{
		Display(curchoose);
	}
}

void CDLGProductInfo::Display(int i)
{
	if(data[i].nid >1)
	{
		GetDlgItem(IDC_STATIC_RUNNUMBER)->SetWindowText(data[i].RunningNumber);     
		GetDlgItem(IDC_STATIC_TAG)->SetWindowText(data[i].tag);           
		GetDlgItem(IDC_STATIC_MAINC)->SetWindowText(data[i].MainCategory);         
		GetDlgItem(IDC_STATIC_SUBC)->SetWindowText(data[i].SubCategory);          
		GetDlgItem(IDC_STATIC_METAFIELD)->SetWindowText(data[i].MetaField);     
		GetDlgItem(IDC_STATIC_COLORDESC)->SetWindowText(data[i].ColourDesc);     
		GetDlgItem(IDC_STATIC_COLOR)->SetWindowText(data[i].Colour);         
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText(data[i].Unit);          
		GetDlgItem(IDC_STATIC_FACTORYITEM)->SetWindowText(data[i].FactoryItem);                   
		GetDlgItem(IDC_STATIC_HMNUM)->SetWindowText(data[i].HmNum);         
		GetDlgItem(IDC_STATIC_DESC)->SetWindowText(data[i].Description);   
	}
	else
	{
		GetDlgItem(IDC_STATIC_RUNNUMBER)->SetWindowText("");     
		GetDlgItem(IDC_STATIC_TAG)->SetWindowText("");           
		GetDlgItem(IDC_STATIC_MAINC)->SetWindowText("");         
		GetDlgItem(IDC_STATIC_SUBC)->SetWindowText("");          
		GetDlgItem(IDC_STATIC_METAFIELD)->SetWindowText("");     
		GetDlgItem(IDC_STATIC_COLORDESC)->SetWindowText("");     
		GetDlgItem(IDC_STATIC_COLOR)->SetWindowText("");         
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText("");          
		GetDlgItem(IDC_STATIC_FACTORYITEM)->SetWindowText("");                   
		GetDlgItem(IDC_STATIC_HMNUM)->SetWindowText("");         
		GetDlgItem(IDC_STATIC_DESC)->SetWindowText("");   
	}
}