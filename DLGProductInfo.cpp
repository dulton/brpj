// DLGProductInfo.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGProductInfo.h"
#include "DLGWarnning.h"

#include "SignalDownload.h"

#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;
extern bool IsChinese;

#include "DLGSettings.h"
extern CDLGSettings DlgSettings;

////////////////////////////////////
#include "SqliteOperate.h"
extern CSqliteOperate SQLiteIO;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RECORD_TIMER 1

/////////////////////////////////////////////////////////////////////////////
// CDLGProductInfo dialog


CDLGProductInfo::CDLGProductInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGProductInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGProductInfo)
	m_barcode = _T("");
	//}}AFX_DATA_INIT
	old_barcode = _T("");
	curchoose=0;
	m_recordtimer = 0;

	//清空 产品
	memset(&temp,0,sizeof(struct PRODUCT_INFO_ST));
	for(int i=0;i<MAX_PLAYWIN;i++)
		memset(&data[i],0,sizeof(struct PRODUCT_INFO_ST));
}


void CDLGProductInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGProductInfo)
	DDX_Control(pDX, IDC_EDIT_BARCODE, m_barcodeCtrl);
	DDX_Control(pDX, IDC_STATIC_PIC3, m_pic3);
	DDX_Control(pDX, IDC_STATIC_PIC2, m_pic2);
	DDX_Control(pDX, IDC_STATIC_PIC1, m_pic1);
	DDX_Text(pDX, IDC_EDIT_BARCODE, m_barcode);
	DDV_MaxChars(pDX, m_barcode, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGProductInfo, CDialog)
	//{{AFX_MSG_MAP(CDLGProductInfo)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGProductInfo message handlers

BOOL CDLGProductInfo::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();

//	CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_BARCODE);
//	pEdit->SetFocus();
//	pEdit->SetWindowText("");
//	pEdit->SetSel(0,-1);

	//设置字体
	TextFont.CreatePointFont(350,_T("Arial Black"));
	GetDlgItem(IDC_STATIC_RECORDTIME)->SetFont(&TextFont,true);

	//定时500ms
	m_recordtimer = SetTimer(RECORD_TIMER,500,NULL);

	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGProductInfo::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
	if(nIDEvent == RECORD_TIMER)
	{
		RecordTimerEvent();
	}
}

void CDLGProductInfo::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_recordtimer) 
		KillTimer(m_recordtimer); 
	m_recordtimer = 0;
}

void CDLGProductInfo::OnCancel()
{

}

void CDLGProductInfo::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);


	Invalidate();
}

void CDLGProductInfo::OnOK() 
{
	// TODO: Add extra validation here
	OnButtonOk();

}

void CDLGProductInfo::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_barcode.IsEmpty())
	{
		CDLGWarnning dlgw;
		dlgw.m_wintxt=Language_ConvertString("Warnning");						//窗口标题
		dlgw.m_warntxt=Language_ConvertString("Please Input Running Number or Carton TAG");		//窗口内容
		dlgw.DoModal();

		return ;
	}

	//查询数据库
	memset(&temp,0,sizeof(struct PRODUCT_INFO_ST));
	if(!SQLiteIO.Product_Read(m_barcode.GetBuffer(0),temp))
	{
		CDLGWarnning dlgw;
		dlgw.m_wintxt=Language_ConvertString("Warnning");						//窗口标题
		dlgw.m_warntxt=Language_ConvertString("This Product can not be found");		//窗口内容
		dlgw.DoModal();
	}
	else
	{
		DisplayLite(temp);
		if(old_barcode != m_barcode)
		{
			pCMainDlg->DlgControl.OnButtonStop();
			pCMainDlg->DlgControl.OnButtonRecord();
			old_barcode = m_barcode;
		}
		else
		{
			pCMainDlg->DlgControl.OnButtonStop();
		}
	}
	CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_BARCODE);
	pEdit->SetFocus();
//	pEdit->SetWindowText("");
	pEdit->SetSel(0,-1);
}

//外部选中窗口后调用此
void CDLGProductInfo::Display(int i)
{
	DisplayLite(data[i]);
}

//清空函数
void CDLGProductInfo::Clean(int i)
{
	old_barcode = _T("");
	memset(&data[i],0,sizeof(struct PRODUCT_INFO_ST));
}

//TEMP拷贝到DATA
void CDLGProductInfo::temp2data(int i)
{
	memcpy(&data[i],&temp,sizeof(struct PRODUCT_INFO_ST));
}

//清空temp
void CDLGProductInfo::CleanTemp()
{
	memset(&temp,0,sizeof(struct PRODUCT_INFO_ST));
}

//显示temp
void CDLGProductInfo::DisplayTemp()
{
	DisplayLite(temp);
}

void CDLGProductInfo::DisplayLite(struct PRODUCT_INFO_ST &input)
{
	SignalDownload a;

	if(input.nid >0)
	{
		GetDlgItem(IDC_STATIC_RUNNUMBER)->SetWindowText(input.RunningNumber);     
		GetDlgItem(IDC_STATIC_TAG)->SetWindowText(input.tag);           
		GetDlgItem(IDC_STATIC_MAINC)->SetWindowText(input.MainCategory);         
		GetDlgItem(IDC_STATIC_SUBC)->SetWindowText(input.SubCategory);          
		GetDlgItem(IDC_STATIC_METAFIELD)->SetWindowText(input.MetaField);     
		GetDlgItem(IDC_STATIC_COLORDESC)->SetWindowText(input.ColourDesc);     
		GetDlgItem(IDC_STATIC_COLOR)->SetWindowText(input.Colour);         
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText(input.Unit);          
		GetDlgItem(IDC_STATIC_FACTORYITEM)->SetWindowText(input.FactoryItem);                   
		GetDlgItem(IDC_STATIC_HMNUM)->SetWindowText(input.HmNum);         
		GetDlgItem(IDC_STATIC_DESC)->SetWindowText(input.Description);   

		if(strlen(input.path1))
		{
			a.HTTPDownload(input.path1,"pi1.jpg",5,0);
			bi=pic1.LoadPicture("pi1.jpg");
			m_pic1.SetBitmap(bi);
		}
		else
			m_pic1.SetBitmap(0);

		if(strlen(input.path2))
		{
			a.HTTPDownload(input.path2,"pi2.jpg",5,0);
			bi=pic2.LoadPicture("pi2.jpg");
			m_pic2.SetBitmap(bi);
		}
		else
			m_pic2.SetBitmap(0);

		if(strlen(input.path3))
		{
			a.HTTPDownload(input.path3,"pi3.jpg",5,0);
			bi=pic3.LoadPicture("pi3.jpg");
			m_pic3.SetBitmap(bi);
		}
		else
			m_pic3.SetBitmap(0);
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
		m_pic1.SetBitmap(0);
		m_pic2.SetBitmap(0);
		m_pic3.SetBitmap(0);
	}
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDLGProductInfo::RecordTimerEvent()
{
	char rtime[16];
	if(pCMainDlg->DlgPlaywin.GetCurWndRecordState() == TRUE)
	{
		pCMainDlg->DlgPlaywin.GetCurWndRecordTime(rtime);		
	}
	else
	{
		strcpy(rtime,"00:00:00");
	}
	GetDlgItem(IDC_STATIC_RECORDTIME)->SetWindowText(rtime);
	int rhour,rmin,rsec;
	sscanf(rtime,"%2d:%2d:%2d",&rhour,&rmin,&rsec);
	if(rmin == DlgSettings.m_time)
	{
		pCMainDlg->DlgControl.OnButtonStop();
	}
}

HBRUSH CDLGProductInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_RECORDTIME:
		if(pCMainDlg->DlgPlaywin.GetCurWndRecordState())
			pDC->SetTextColor(RGB(255,0,0));
		else
			pDC->SetTextColor(RGB(0,127,0));
			break;
	default:break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}


