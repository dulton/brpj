// DLGTabPic.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGTabPic.h"


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
// CDLGTabPic dialog

IMPLEMENT_DYNAMIC(CDLGTabPic, CDialog)

CDLGTabPic::CDLGTabPic(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGTabPic::IDD, pParent)
{
	bim=NULL;
	bimsmall=NULL;
}

CDLGTabPic::~CDLGTabPic()
{
}

void CDLGTabPic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Control(pDX, IDC_PIC_SMALL, m_pic_small);
}


BEGIN_MESSAGE_MAP(CDLGTabPic, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGTabPic::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGTabPic::OnCancel)

	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CDLGTabPic::OnInitDialog()
{
	CDialog::OnInitDialog();



	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGTabPic::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);


	//小图片
	CRect pic_small_Rect;
	pic_small_Rect.top=rc.top;
	pic_small_Rect.bottom=rc.top+200;
	pic_small_Rect.left = rc.left;
	pic_small_Rect.right = pic_small_Rect.left+200;
	GetDlgItem(IDC_PIC_SMALL)->MoveWindow(pic_small_Rect);

	//图片
	CRect pic_Rect;
	pic_Rect.top=pic_small_Rect.bottom+20;
	pic_Rect.bottom=rc.bottom;

	pic_Rect.left = rc.left;
	pic_Rect.right = rc.right;
	GetDlgItem(IDC_PIC)->MoveWindow(pic_Rect);



	Invalidate();
}

// CDLGTabPic message handlers

void CDLGTabPic::OnOk()
{
	// TODO: Add your control notification handler code here
	//	CDialog::OnOK();
}

void CDLGTabPic::OnCancel()
{
	// TODO: Add your control notification handler code here
	clear();

	DlgMain->OnCancel();
}
void CDLGTabPic::clear()
{
	//释放图片
	pic.FreePicture();
	picsmall.FreePicture();
	bim=NULL;
	bimsmall=NULL;
}

void CDLGTabPic::Load()
{
	clear();
	//加载图片
	bim=pic.LoadPicture(srcfile.GetBuffer());
	bimsmall=picsmall.LoadPicture(srcsmallfile.GetBuffer());
}

void CDLGTabPic::PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth)
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


void CDLGTabPic::OnPaint()
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

	if(bimsmall)
	{
		//显示图片
		CDC *pDCsmall=m_pic_small.GetDC();
		CRect rectsmall; 
		m_pic_small.GetClientRect(&rectsmall);//获得pictrue控件所在的举行区域

		PicAutoWH(rectsmall.Width(),rectsmall.Height(),picsmall._GetWidth(),picsmall._GetHeight(),&w,&h);

		picsmall.DrawPicture(*pDCsmall,(rectsmall.Width()-w)/2,(rectsmall.Height()-h)/2,w,h);
		m_pic_small.ReleaseDC(pDCsmall);
	}


	CDialog::OnPaint();
}
