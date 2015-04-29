// DLGHistoryFaceAlarmLite.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGHistoryFaceAlarmLite.h"

#include "SmartVillageDlg.h"
#include "DLGWarnning.h"
#include "DLGpictureView.h"
////////////////lynn/////////////////
extern CYRSVMySQL MySqlIO;
extern CSmartVillageDlg *DlgMain;
////////////////lynn/////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;


// CDLGHistoryFaceAlarmLite dialog

IMPLEMENT_DYNAMIC(CDLGHistoryFaceAlarmLite, CDialog)

CDLGHistoryFaceAlarmLite::CDLGHistoryFaceAlarmLite(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGHistoryFaceAlarmLite::IDD, pParent)
	, m_txt(_T(""))
{
	m_page = 1;
	bim=NULL;
		ListTotal=0;
	ListNow=0;

}

CDLGHistoryFaceAlarmLite::~CDLGHistoryFaceAlarmLite()
{
}

void CDLGHistoryFaceAlarmLite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_first_button);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_pre_button);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_next_button);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_last_button);
	DDX_Control(pDX, IDC_BUTTON_JUMP, m_jump_button);
	DDX_Text(pDX, IDC_STATIC_TXT, m_txt);
}


BEGIN_MESSAGE_MAP(CDLGHistoryFaceAlarmLite, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGHistoryFaceAlarmLite::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGHistoryFaceAlarmLite::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_STN_DBLCLK(IDC_PIC, &CDLGHistoryFaceAlarmLite::OnStnDblclickPic)
END_MESSAGE_MAP()


char* CDLGHistoryFaceAlarmLite::FaceSex(int i)
{
	switch(i)
	{
	case SEX_MALE:return "男性";
	case SEX_UNKNOW:return "未知";
	case SEX_FEMALE:return "女性";
	default:return "未知";
	}
}
// CDLGHistoryFaceAlarmLite message handlers

BOOL CDLGHistoryFaceAlarmLite::OnInitDialog()
{
	CDialog::OnInitDialog();

	DlgPictureFace.TitleDrawFlag=false;
	DlgPictureFace.Create(IDD_PICTURE_FACE,this);
	DlgPictureFace.ShowWindow(SW_SHOW);

	m_first_button.LoadBitmaps(IDB_FIRST_BUTTON,IDB_FIRST_BUTTON_MOVE,NULL,NULL);
	m_first_button.SizeToContent();		//自适应图片大小

	m_pre_button.LoadBitmaps(IDB_PRE_BUTTON,IDB_PRE_BUTTON_MOVE,NULL,NULL);
	m_pre_button.SizeToContent();		//自适应图片大小

	m_next_button.LoadBitmaps(IDB_NEXT_BUTTON,IDB_NEXT_BUTTON_MOVE,NULL,NULL);
	m_next_button.SizeToContent();		//自适应图片大小

	m_last_button.LoadBitmaps(IDB_LAST_BUTTON,IDB_LAST_BUTTON_MOVE,NULL,NULL);
	m_last_button.SizeToContent();		//自适应图片大小

	m_jump_button.LoadBitmaps(IDB_JUMP_BUTTON,IDB_JUMP_BUTTON_MOVE,NULL,NULL);
	m_jump_button.SizeToContent();		//自适应图片大小


	struct FACE_CAPTURE_ST tempFace={0};

	if(!MySqlIO.FACE_Capture_ReadforListOne(capnid,tempFace))
		return FALSE;

	char str[260]={0};
	m_txt="抓拍信息:\n时间:  ";

	sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",
		tempFace.year,
		tempFace.mon,
		tempFace.day,
		tempFace.hour,
		tempFace.min,
		tempFace.sec);

	m_txt+=str;
	m_txt+="\n设备名:";
	m_txt+=tempFace.cameraName;
	m_txt+="\n年龄:  ";
	sprintf(str,"%d",tempFace.age);
	m_txt+=str;
	m_txt+="\n性别:  ";
	m_txt+=FaceSex(tempFace.sex);

	//路径
	srcfile=tempFace.face;

	UpdateData(FALSE);

	bim=pic.LoadPicture(srcfile.GetBuffer());

	//放在最后
	AutoSize();


	ListTotal=0;

	ListTotal=MySqlIO.VS_FACE_Alarm_GetNum(capnid);

	ListNow=0;

	DisplayerList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGHistoryFaceAlarmLite::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);
	int distance=5;

	int buttonh=25;
	int buttonbmpw=24,buttonbmph=18,buttongap=10;


	int bmp_width=150;
	//图片
	CRect pic_Rect;
	pic_Rect.top=rc.top;
	pic_Rect.bottom=pic_Rect.top+bmp_width;

	pic_Rect.left = rc.left;
	pic_Rect.right = 	pic_Rect.left+bmp_width;
	GetDlgItem(IDC_PIC)->MoveWindow(pic_Rect);

	//TXT	
	CRect txt_Rect;
	txt_Rect.top=pic_Rect.top;
	txt_Rect.bottom=pic_Rect.bottom;

	txt_Rect.left = pic_Rect.right+10;
	txt_Rect.right = rc.right;
	GetDlgItem(IDC_STATIC_TXT)->MoveWindow(txt_Rect);


	//人脸
	CRect face_Rect;
	//face_Rect.top = sreach_Rect.bottom ;
	face_Rect.top =  	pic_Rect.bottom;
	face_Rect.bottom =	rc.bottom-buttonh;
	face_Rect.left =rc.left+distance ;
	face_Rect.right = rc.right;
	//必须 样式=重叠，边框=调整大小
	DlgPictureFace.MoveWindow(face_Rect);
	DlgPictureFace.AutoSize();


	CRect b_Rect;

	b_Rect.top = face_Rect.bottom+5 ;
	b_Rect.bottom =	b_Rect.top+buttonbmph;

	b_Rect.left = rc.left;
	b_Rect.right = rc.left+200;
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

	b_Rect.top = face_Rect.bottom+8 ;
	b_Rect.left = b_Rect.right+buttongap*3;
	b_Rect.right = b_Rect.left+20;
	GetDlgItem(IDC_STATIC_DI)->MoveWindow(b_Rect);

	b_Rect.top = face_Rect.bottom+5 ;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+60;
	GetDlgItem(IDC_EDIT_PAGE)->MoveWindow(b_Rect);

	b_Rect.top = face_Rect.bottom+2 ;
	b_Rect.bottom =	b_Rect.top+24;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+20;
	m_jump_button.MoveWindow(b_Rect);

	b_Rect.top = face_Rect.bottom+8 ;
	b_Rect.left = b_Rect.right;
	b_Rect.right = b_Rect.left+20;
	GetDlgItem(IDC_STATIC_YE)->MoveWindow(b_Rect);

	Invalidate();
}

// CDLGHistoryFaceAlarmLite message handlers

void CDLGHistoryFaceAlarmLite::OnOk()
{
	// TODO: Add your control notification handler code here
	//CDialog::OnOK();
}

void CDLGHistoryFaceAlarmLite::OnCancel()
{
	// TODO: Add your control notification handler code here
	pic.FreePicture();
	bim=NULL;
	DlgPictureFace.CleanList();
	alarmList.clear();

	CDialog::OnCancel();
}
void CDLGHistoryFaceAlarmLite::PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth)
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


void CDLGHistoryFaceAlarmLite::OnPaint()
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
HBRUSH CDLGHistoryFaceAlarmLite::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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



void CDLGHistoryFaceAlarmLite::DisplayerList(void)
{
	alarmList.clear();
	DlgPictureFace.CleanList();

	if(0==ListTotal)
	{
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
		InvalidateRect(printf_Rect, TRUE);
		return ;
	}
	char str[260]={0};

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	MySqlIO.VS_FACE_Alarm_ReadWithCaptureID(capnid,si,ei,alarmList);
	if(alarmList.size()>0)
		DlgPictureFace.DisplayAlarmLiteList(alarmList,FLAG_FACE_BLACK);

	if(ListTotal%HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM+1,ListTotal/HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM+1,HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM+1,ListTotal/HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM,HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	InvalidateRect(printf_Rect, TRUE);

}
void CDLGHistoryFaceAlarmLite::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGHistoryFaceAlarmLite::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM) >= 0)
		ListNow-=HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryFaceAlarmLite::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryFaceAlarmLite::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryFaceAlarmLite::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM) < (HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM*m_page))
			)
			ListNow+=HISTORY_FACE_ALARMLITE_PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);

}
void CDLGHistoryFaceAlarmLite::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AutoSize();
}

void CDLGHistoryFaceAlarmLite::OnStnDblclickPic()
{
	// TODO: Add your control notification handler code here
	struct FACE_CAPTURE_ST tempFace={0};

	if(!MySqlIO.FACE_Capture_ReadforListOne(capnid,tempFace))
		return ;

	char str[260]={0};
	CDLGpictureView dlgPicView;
	dlgPicView.m_txt="时间:  ";

	sprintf(str,"%04d-%02d-%02d %02d:%02d:%02d",
		tempFace.year,
		tempFace.mon,
		tempFace.day,
		tempFace.hour,
		tempFace.min,
		tempFace.sec);

	dlgPicView.m_txt+=str;
	dlgPicView.m_txt+="\n设备名:";
	dlgPicView.m_txt+=tempFace.cameraName;
	dlgPicView.m_txt+="\n年龄:  ";
	sprintf(str,"%d",tempFace.age);
	dlgPicView.m_txt+=str;
	dlgPicView.m_txt+="\n性别:  ";
	dlgPicView.m_txt+=FaceSex(tempFace.sex);

	//标题
	dlgPicView.Titlestr=tempFace.cameraName;

	char *p=strrchr(tempFace.file,'\\');
	if(p!=NULL)
		p++;
	else
		return ;

	dlgPicView.Titlestr+=p;

	//路径
	dlgPicView.srcfile=tempFace.file;
	dlgPicView.srcsmallfile=tempFace.face;

	dlgPicView.DoModal();
}
