// DLGPictureLite.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGPictureLite.h"
#include "SmartVillageDlg.h"
#include "DLGWarnning.h"
#include "DLGpictureView.h"
#include "DLGHistoryFaceAlarmLite.h"

////////////////lynn/////////////////
extern CYRSVMySQL MySqlIO;
extern CSmartVillageDlg *DlgMain;
////////////////lynn/////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;


// CDLGPictureLite dialog

IMPLEMENT_DYNAMIC(CDLGPictureLite, CDialog)

CDLGPictureLite::CDLGPictureLite(CWnd* pParent /*=NULL*/)
: CDialog(CDLGPictureLite::IDD, pParent)
{
	bim=NULL;
	//不可以画
	DrawFlag=false;
	BlackFlag=0;
}

CDLGPictureLite::~CDLGPictureLite()
{
	pic.FreePicture();
	bim=NULL;
	DrawFlag=false;
}

void CDLGPictureLite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	DDX_Text(pDX, IDC_STATIC_TXT, m_info);
}


BEGIN_MESSAGE_MAP(CDLGPictureLite, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGPictureLite::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGPictureLite::OnCancel)
	ON_WM_PAINT()
	ON_STN_DBLCLK(IDC_STATIC_PIC, &CDLGPictureLite::OnStnDblclickStaticPic)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CDLGPictureLite::OnInitDialog()
{
	CDialog::OnInitDialog();


	UpdateData(FALSE);


	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CDLGPictureLite::LoadPic()
{
	if(DrawFlag)
	{
		//STR从数据库找出一个
		//加载图片
		bim=pic.LoadPicture(srcfile.GetBuffer());
	}
	UpdateData(FALSE);
	Invalidate();
};

void CDLGPictureLite::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);

	int w=100;
	int h=133;

	rc.top +=2 ;
	rc.left +=3;

	//人脸
	CRect pic_Rect;
	pic_Rect.top = rc.top ;
	pic_Rect.bottom =rc.top +h;
	pic_Rect.left = rc.left ;
	pic_Rect.right = rc.left+w;
	//必须 样式=重叠，边框=调整大小
	m_pic.MoveWindow(pic_Rect);


	CRect info_Rect;
	info_Rect.top = pic_Rect.bottom ;
	info_Rect.bottom =info_Rect.top+60;
	info_Rect.left = rc.left ;
	info_Rect.right = rc.left+w;
	//必须 样式=重叠，边框=调整大小
	GetDlgItem(IDC_STATIC_TXT)->MoveWindow(info_Rect);

	Invalidate();
}

char* CDLGPictureLite::FaceSex(int i)
{
	switch(i)
	{
	case SEX_MALE:return "男性";
	case SEX_UNKNOW:return "未知";
	case SEX_FEMALE:return "女性";
	default:return "未知";
	}
}
char* CDLGPictureLite::CardType(int i)
{
	switch(i)
	{
	case CARD_UNKNOW:return "未知";
	case CARD_GENERATION_ONE:return "一代证";
	case CARD_GENERATION_TWO:return "二代证";
	case CARD_GAS_CARDIN:return "士官证";
	case CARD_STUDENT:return "学生证";
	case CARD_DRIVER:return "驾驶证";
	case CARD_PASSPORT:return "护照";
	case CARD_OTHER:return "其他";
	default:return "未知";
	}
}




// CDLGPictureLite message handlers

void CDLGPictureLite::OnOk()
{
	// TODO: Add your control notification handler code here
	//	CDialog::OnOK();
}

void CDLGPictureLite::OnCancel()
{
	
	// TODO: Add your control notification handler code here
	//	CDialog::OnCancel();
}
void CDLGPictureLite::PicAutoWH(int rectw,int recth,int picw,int pich,int *dstw,int *dsth)
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

void CDLGPictureLite::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_PIC_FACE_BACK);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	int w,h;

	if(DrawFlag && bim)
	{
		//显示图片
		CDC *pDC=m_pic.GetDC();
		CRect rect; 
		m_pic.GetClientRect(&rect);//获得pictrue控件所在的举行区域

		PicAutoWH(rect.Width(),rect.Height(),pic._GetWidth(),pic._GetHeight(),&w,&h);
		/*
		if(aa)
		{
		//显示边框
		CPen   pen(PS_SOLID,   3,   RGB(255,0,0));  
		CPen   *pOldPen=pDC->SelectObject(&pen);  
		pDC->Rectangle(&rect);  
		pDC->SelectObject(pOldPen);   
		}
		*/
		pic.DrawPicture(*pDC, (rect.Width()-w)/2,(rect.Height()-h)/2,w,h);
		m_pic.ReleaseDC(pDC);


	}

	CDialog::OnPaint();
}

void CDLGPictureLite::OnStnDblclickStaticPic()
{
	// TODO: Add your control notification handler code here
	/*
	//变大图
	aa=!aa;
	this->Invalidate();
	*/
	char str[260]={0};

	if(FLAG_FAC_CAP==BlackFlag)
	{
		struct FACE_CAPTURE_ST tempFace={0};

		if(!MySqlIO.FACE_Capture_ReadforListOne(capnid,tempFace))
			return ;

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
	else 	if(FLAG_FACE_ALARM==BlackFlag)
	{
		CDLGHistoryFaceAlarmLite DlgHistoryFaceAlarmLite;
		DlgHistoryFaceAlarmLite.capnid=capnid;
		DlgHistoryFaceAlarmLite.DoModal();
	}
	else if(FLAG_FACE_BLACK==BlackFlag || FLAG_FACE_BLACK_ONE==BlackFlag)
	{
		struct FACE_ALARM_VIEW_ST tempFace={0};

		char STR_NATIVE[32][32] = {"未知","北京","天津","重庆","广东","河北","山西","辽宁","吉林","黑龙江",
			"江苏","浙江","安徽","福建","江西","山东","河南","湖北","湖南","海南",
			"四川","贵州","云南","陕西","甘肃","青海","台湾","新疆","广西","宁夏",
			"内蒙古","西藏"};

		if(FLAG_FACE_BLACK==BlackFlag)
		{
			if(!MySqlIO.VS_FACE_Alarm_ReadOne(capnid,personnid,tempFace))
				return ;
		}
		else if(FLAG_FACE_BLACK_ONE==BlackFlag)
		{
			if(!MySqlIO.FACE_Black_Read_DetailBlack(capnid,personnid,tempFace))
				return ;
		}
		else
		{
			 return ;
		}

		CDLGpictureView dlgPicView;

		dlgPicView.m_txt="布控库名称:";
		dlgPicView.m_txt+=tempFace.dbname;

		dlgPicView.m_txt+="\n姓名:      ";
		dlgPicView.m_txt+=tempFace.name;

		sprintf(str,"%04d-%02d-%02d",
			tempFace.year,
			tempFace.mon,
			tempFace.day);

		dlgPicView.m_txt+="\n出生日期:  ";
		dlgPicView.m_txt+=str;

		dlgPicView.m_txt+="\n性别:      ";
		dlgPicView.m_txt+=FaceSex(tempFace.sex);

		dlgPicView.m_txt+="\n电话:      ";
		dlgPicView.m_txt+=tempFace.phone;

		dlgPicView.m_txt+="\n证件类型:  ";
		dlgPicView.m_txt+=CardType(tempFace.cardtype);

		dlgPicView.m_txt+="\n证件号:    ";
		dlgPicView.m_txt+=tempFace.cardNO;

		dlgPicView.m_txt+="\n籍贯:      ";
		dlgPicView.m_txt+=STR_NATIVE[tempFace.native];
		
		dlgPicView.m_txt+="\n地址:      ";
		dlgPicView.m_txt+=tempFace.address;

		sprintf(str,"%d",tempFace.similarity);

		dlgPicView.m_txt+="\n相似度:  ";
		dlgPicView.m_txt+=str;

		//标题
		dlgPicView.Titlestr="布控信息";

		//路径
		dlgPicView.srcfile=tempFace.file;
		dlgPicView.srcsmallfile=tempFace.face;

		dlgPicView.DoModal();
	}

}

HBRUSH CDLGPictureLite::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT); // 设置透明背景
		// TODO: Change any attributes of the DC here
		pDC->SetTextColor(RGB(0, 0, 0)); // 设置文本颜色
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		hbr=(HBRUSH)GetStockObject(HOLLOW_BRUSH); // 返回透明画刷	
		// TODO: Return a different brush if the default is not desired
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
