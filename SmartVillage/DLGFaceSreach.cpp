// DLGFaceSreach.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGFaceSreach.h"

#include "SmartVillageDlg.h"
#include "DLGWarnning.h"

////////////////lynn/////////////////
extern CYRSVMySQL MySqlIO;
extern CSmartVillageDlg *DlgMain;
////////////////lynn/////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

// CDLGFaceSreach dialog

IMPLEMENT_DYNAMIC(CDLGFaceSreach, CDialog)

CDLGFaceSreach::CDLGFaceSreach(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGFaceSreach::IDD, pParent)
	, m_e_name(_T(""))
	, m_c_cardtype(0)
	, m_c_jiguan(0)
	, m_e_cardnum(_T(""))
	, m_e_phone(_T(""))
	, m_e_addr(_T(""))
	, m_e_sim_start(60)

	, m_c_limitnum(FALSE)
	, m_e_limitnum(10)
	, m_c_sex(0)
	, m_CheckLimitNum(FALSE)
{
	filepath[0]=0;
	bim=NULL;

	m_CheckTime=FALSE;

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime.GetTime();
	m_StartHour=nowtime.GetTime();

	m_Birth_Start=nowtime.GetTime();
	m_Birth_End=nowtime.GetTime();

	CTime cstime(
		nowtime.GetYear(),
		nowtime.GetMonth(),
		nowtime.GetDay(),
		0,	0,	0);

	m_StartHour=cstime.GetTime();

	nowtime+=3600;
	m_EndMon=nowtime.GetTime();
	m_EndHour=nowtime.GetTime();


}

CDLGFaceSreach::~CDLGFaceSreach()
{
}

void CDLGFaceSreach::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_pic);

	DDX_Control(pDX, IDC_BUTTON_FILE, m_b_file);
	DDX_Text(pDX, IDC_EDIT_NAME, m_e_name);
	DDV_MaxChars(pDX, m_e_cardnum, 32);
	DDX_CBIndex(pDX, IDC_COMBO_CARDTYPE, m_c_cardtype);
	DDX_CBIndex(pDX, IDC_COMBO_JIGUAN, m_c_jiguan);
	DDX_Text(pDX, IDC_EDIT_CARDNUM, m_e_cardnum);
	DDV_MaxChars(pDX, m_e_cardnum, 32);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_e_phone);
	DDV_MaxChars(pDX, m_e_cardnum, 32);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_e_addr);
	DDV_MaxChars(pDX, m_e_cardnum, 256);
	DDX_Text(pDX, IDC_EDIT_SIM_START, m_e_sim_start);
	DDV_MinMaxInt(pDX, m_e_sim_start, 0, 90);
	DDX_Check(pDX, IDC_CHECK_LIMITNUM, m_c_limitnum);
	DDX_Text(pDX, IDC_EDIT_LIMITNUM, m_e_limitnum);
	DDV_MinMaxInt(pDX, m_e_limitnum, 10, 30);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_b_delete);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_BIRTH_START, m_Birth_Start);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_BIRTH_END, m_Birth_End);

	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTMON, m_StartMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTHOUR, m_StartHour);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDMON, m_EndMon);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDHOUR, m_EndHour);
	DDX_Check(pDX, IDC_CHECK_TIME, m_CheckTime);

	DDX_CBIndex(pDX, IDC_COMBO_SEX, m_c_sex);
	DDX_Check(pDX, IDC_CHECK_LIMITNUM, m_CheckLimitNum);
}


BEGIN_MESSAGE_MAP(CDLGFaceSreach, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGFaceSreach::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGFaceSreach::OnCancel)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
		ON_BN_CLICKED(IDC_BUTTON_FILE, &CDLGFaceSreach::OnBnClickedButtonFile)
		ON_WM_PAINT()
		ON_WM_CTLCOLOR()
		ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDLGFaceSreach::OnBnClickedButtonDelete)
		ON_BN_CLICKED(IDC_CHECK_LIMITNUM, &CDLGFaceSreach::OnBnClickedCheckLimitnum)
END_MESSAGE_MAP()


// CDLGFaceSreach message handlers

BOOL CDLGFaceSreach::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT_LIMITNUM)->EnableWindow(FALSE);

	m_b_file.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
	m_b_file.SizeToContent();		//自适应图片大小

	m_b_delete.LoadBitmaps(IDB_DEL_BUTTON,IDB_DEL_BUTTON_MOVE,NULL,IDB_DEL_BUTTON_DIS);
	m_b_delete.SizeToContent();		//自适应图片大小
	//放在最后
	AutoSize();

	OnCheckTime();
	OnBnClickedCheckLimitnum();


	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CDLGFaceSreach::OnCheckTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_CheckTime)
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDHOUR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER_STARTMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_STARTHOUR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDMON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_ENDHOUR)->EnableWindow(FALSE);
	}
}
void CDLGFaceSreach::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);

	Invalidate();
}

// CDLGFaceSreach message handlers

void CDLGFaceSreach::OnOk()
{
	// TODO: Add your control notification handler code here
	//	CDialog::OnOK();
}

void CDLGFaceSreach::OnCancel()
{
	// TODO: Add your control notification handler code here
	DlgMain->OnCancel();
}

void CDLGFaceSreach::PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth)
{
	int w,h;
	if( 0== rectw || 0== recth || 0==picw || 0==pich)
	{
		(*dstw)=100;
		(*dsth)=100;
		return ;
	}
	if(picw> rectw)
	{
		w=rectw;
		h= pich*((float)rectw/(float)picw);

		if(h> recth)
		{
			w=w*((float)recth/(float)h);
			h= recth;
		}

	}
	else
	{
		if(pich> recth)
		{

			w=picw*((float)recth/(float)pich);
			h= recth;
		}
		else
		{
			w=picw;
			h=pich;
		}
	}
	(*dstw)=w;
	 (*dsth)=h;
}

void CDLGFaceSreach::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_FIND_BACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	int w,h;

	if(bim)
	{
		//显示图片
		CDC *pDC=m_pic.GetDC();
		CRect rect; 
		m_pic.GetClientRect(&rect);//获得pictrue控件所在的举行区域

		PicAutoWH(rect.Width(),rect.Height(),pic._GetWidth(),pic._GetHeight(),&w,&h);

		pic.DrawPicture(*pDC, (rect.Width()-w)/2,(rect.Height()-h)/2,w,h);
		m_pic.ReleaseDC(pDC);

	}

	CDialog::OnPaint();
}

//静态文本控件 透明
HBRUSH CDLGFaceSreach::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); // 设置透明背景
		// TODO: Change any attributes of the DC here
		pDC->SetTextColor(RGB(0, 0, 0)); // 设置文本颜色
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		hbr=(HBRUSH)GetStockObject(HOLLOW_BRUSH); // 返回透明画刷	
		// TODO: Return a different brush if the default is not desired
	}

	return hbr;
}


void CDLGFaceSreach::OnBnClickedButtonFile()
{
	// TODO: Add your control notification handler code here

	// TODO: Add your control notification handler code here
	char szFilter[]="jpg Files (*.jpg)|*.jpg|bmp Files (*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE,"jpg","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		//文件路径
		strcpy(filepath,dlg.GetPathName().GetBuffer());
		UpdateData(TRUE);
	
		//加载图片
		bim=pic.LoadPicture(filepath);
		Invalidate();


/*
		//从资源中载入位图     
		CBitmap    bitmapq;     
		bitmapq.Attach(bim);    
		BITMAP bmp;
		bitmapq.GetBitmap(&bmp);
	bitmapq.Detach();
	*/
	}
}

void CDLGFaceSreach::OnBnClickedButtonDelete()
{
	memset(filepath,0,MAX_PATH);
	//加载图片
	pic.FreePicture();
	bim=NULL;
	m_pic.SetBitmap(0);
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDLGFaceSreach::Clean()
{
	OnBnClickedButtonDelete();

	m_e_name=_T("");
	m_c_cardtype=0;
	m_c_jiguan=0;
	m_e_cardnum=_T("");
	m_e_phone=_T("");
	m_e_addr=_T("");
	m_e_sim_start=60;
	m_c_limitnum=FALSE;
	m_e_limitnum=10;
	m_c_sex=0;

	m_CheckTime=FALSE;

	CTime nowtime=CTime::GetTickCount();
	m_StartMon=nowtime.GetTime();
	m_StartHour=nowtime.GetTime();

	m_Birth_Start=nowtime.GetTime();
	m_Birth_End=nowtime.GetTime();
	CTime cstime(
		nowtime.GetYear(),
		nowtime.GetMonth(),
		nowtime.GetDay(),
		0,	0,	0);

	m_StartHour=cstime.GetTime();
	nowtime+=3600;
	m_EndMon=nowtime.GetTime();
	m_EndHour=nowtime.GetTime();

	UpdateData(FALSE);

}
void CDLGFaceSreach::OnBnClickedCheckLimitnum()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_CheckLimitNum)
	{
		GetDlgItem(IDC_EDIT_LIMITNUM)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_LIMITNUM)->EnableWindow(FALSE);
	}
}
