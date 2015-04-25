// DLGTabFace.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGTabFace.h"

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


// CDLGTabFace dialog

IMPLEMENT_DYNAMIC(CDLGTabFace, CDialog)

CDLGTabFace::CDLGTabFace(CWnd* pParent /*=NULL*/)
: CDialog(CDLGTabFace::IDD, pParent)
{
	m_page = 1;
		ListTotal=0;
	ListNow=0;
	iLastSearchMode = 0;
}

CDLGTabFace::~CDLGTabFace()
{

}

void CDLGTabFace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_page);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_first_button);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_pre_button);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_next_button);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_last_button);
	DDX_Control(pDX, IDC_BUTTON_JUMP, m_jump_button);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_search);

}


BEGIN_MESSAGE_MAP(CDLGTabFace, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGTabFace::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGTabFace::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_TREE, &CDLGTabFace::OnNMClickTree)
END_MESSAGE_MAP()

BOOL CDLGTabFace::OnInitDialog()
{
	CDialog::OnInitDialog();

	DlgFaceSreach.Create(IDD_FACE_SREACH,this);
	DlgFaceSreach.ShowWindow(SW_SHOW);

	DlgFaceSreach.GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_COMBO_CARDTYPE)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_EDIT_CARDNUM)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_COMBO_JIGUAN)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(FALSE);

	DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_START)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_END)->EnableWindow(FALSE);


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


	m_search.LoadBitmaps(IDB_FIND_BUTTON,IDB_FIND_BUTTON_MOVE,NULL,NULL);
	m_search.SizeToContent();		//自适应图片大小
	//放在最后
	AutoSize();

	m_tree.SetBkColor(RGB(254,254,254));	//树列表背景色

	imagelist.Create(16, 16, ILC_COLOR8|ILC_MASK, 0, 4);    
	imagelist.Add(AfxGetApp()->LoadIcon(IDI_TREE_AREA));        // ico图标
	imagelist.Add(AfxGetApp()->LoadIcon(IDI_TREE_CAM));
	m_tree.SetImageList(&imagelist, TVSIL_NORMAL);  // 建立 imagelist 与 tree的映射关系

	BulidTree();

#if OPEN_FACEDETECT_CODE
	FaceRecg.RWFaceRecg_BlackInit();
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGTabFace::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);
	int distance=5;
	int treew=220;
	int sreachh=160;

	int buttonh=25;
	int buttonbmpw=24,buttonbmph=18,buttongap=10;

	//树
	CRect tree_Rect;
	tree_Rect.top = rc.top+28;
	tree_Rect.bottom = rc.bottom-distance;
	tree_Rect.left = rc.left;
	tree_Rect.right = rc.left+treew;

	m_tree.MoveWindow(tree_Rect);

	//检索条件
	CRect sreach_Rect;
	sreach_Rect.top = rc.top;
	sreach_Rect.bottom = rc.top+sreachh;
	sreach_Rect.left = tree_Rect.right+distance;
	sreach_Rect.right = sreach_Rect.left +770;

	DlgFaceSreach.MoveWindow(sreach_Rect);

	CRect sreachB_Rect;
	sreachB_Rect.top = rc.top+sreachh/2-16;
	sreachB_Rect.bottom =sreachB_Rect.top+32;
	sreachB_Rect.left = sreach_Rect.right;
	sreachB_Rect.right =sreachB_Rect.left+94;

	GetDlgItem(IDC_BUTTON_SEARCH)->MoveWindow(sreachB_Rect);

	//人脸
	CRect face_Rect;
	face_Rect.top = sreach_Rect.bottom ;
	face_Rect.bottom =		rc.bottom-buttonh;
	face_Rect.left =tree_Rect.right+distance ;
	face_Rect.right = rc.right;
	//必须 样式=重叠，边框=调整大小
	DlgPictureFace.MoveWindow(face_Rect);
	DlgPictureFace.AutoSize();



	CRect b_Rect;

	b_Rect.top = face_Rect.bottom+5 ;
	b_Rect.bottom =	b_Rect.top+buttonbmph;

	b_Rect.left =	tree_Rect.right ;
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

// CDLGTabFace message handlers

void CDLGTabFace::OnOk()
{
	// TODO: Add your control notification handler code here
	//	CDialog::OnOK();
}

void CDLGTabFace::Init()
{
	// TODO: Add your control notification handler code here
	faceList.clear();
	DlgPictureFace.CleanList();
	m_page = 1;
	ListTotal=0;
	ListNow=0;
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
	InvalidateRect(printf_Rect, TRUE);
	BulidTree();
	DlgFaceSreach.Clean();

	DlgFaceSreach.GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_COMBO_CARDTYPE)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_EDIT_CARDNUM)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_COMBO_JIGUAN)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_START)->EnableWindow(FALSE);
	DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_END)->EnableWindow(FALSE);
	iLastSearchMode = 0;
}

void CDLGTabFace::Clean()
{
	// TODO: Add your control notification handler code here
	faceList.clear();
	DlgPictureFace.CleanList();
	TempDBList.clear();
	BlackDBList.clear();

	faceTempResList.clear();
	faceBlackResList.clear();

#if OPEN_FACEDETECT_CODE
	FaceRecg.RWFaceRecg_UnInit();
#endif
}

void CDLGTabFace::OnCancel()
{
	// TODO: Add your control notification handler code here
	//清理不能放这
	DlgMain->OnCancel();
}

void CDLGTabFace::OnPaint()
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

	CDialog::OnPaint();
}

//静态文本控件 透明
HBRUSH CDLGTabFace::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDLGTabFace::BulidTree(void)
{
	dbtotal=0;
	m_tree.DeleteAllItems();
	TempRootitem = m_tree.InsertItem("抓拍库",0,0,TVI_ROOT);		//在根结点上添加区域
	TempDBList.clear();
	MySqlIO.TEMP_DB_ReadAll(TempDBList);
	if(TempDBList.size()>0)
	{	
		list<struct FACE_TEMP_DB_ST>::iterator beglistA;

		for(beglistA=TempDBList.begin();beglistA!=TempDBList.end();beglistA++)
		{
			dblist[dbtotal].item = m_tree.InsertItem(beglistA->name,1,1,TempRootitem);		//在根结点上添加区域
			dblist[dbtotal].nid=beglistA->nid;
			dblist[dbtotal].RWdb_nid=beglistA->RW_tempID;
			dblist[dbtotal].ncamera=beglistA->ncamera;
			dbtotal++;
		}
	}

	BlackRootitem= m_tree.InsertItem("黑名单库",0,0,TVI_ROOT);		//在根结点上添加区域
	BlackDBList.clear();
	MySqlIO.FACE_BlackDB_ReadAll(BlackDBList);
	if(BlackDBList.size()>0)
	{
		list<struct FACE_DB_ST>::iterator beglistB;

		for(beglistB=BlackDBList.begin();beglistB!=BlackDBList.end();beglistB++)
		{
			dblist[dbtotal].item = m_tree.InsertItem(beglistB->dbname,1,1,BlackRootitem);		//在根结点上添加区域
			dblist[dbtotal].ncamera=0;
			dblist[dbtotal].nid=beglistB->nid;
			dblist[dbtotal].RWdb_nid=beglistB->RW_dbID;
			dbtotal++;
		}
	}

	//全部展开
	m_tree.Expand(BlackRootitem,TVE_EXPAND);
	m_tree.Expand(TempRootitem,TVE_EXPAND);
}



void CDLGTabFace::DisplayerList(void)
{
	faceList.clear();
	DlgPictureFace.CleanList();

	if(0==ListTotal)
	{
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("共0条 1/1页");
		InvalidateRect(printf_Rect, TRUE);
		return ;
	}

	unsigned long int si,ei;

	si=ListNow+1;
	if((ListNow+TAB_FACE_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+TAB_FACE_PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	char str[64]={0};

	switch(ModeFlag)
	{
	case FLAG_HISTORY_FACE :
		MySqlIO.FACE_Capture_Read(SqlStr,searchFlag,si,ei,faceList);
		if(faceList.size()>0)
			DlgPictureFace.DisplayCapList(faceList,FLAG_FAC_CAP);
		break;
	default:
		MessageBox("CDLGTabFace::DisplayerList A error",MESSAGEBOX_TITLE);
		return ;
	}

	if(ListTotal%TAB_FACE_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/TAB_FACE_PAGE_MAX_NUM+1,ListTotal/TAB_FACE_PAGE_MAX_NUM+1,TAB_FACE_PAGE_MAX_NUM);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/TAB_FACE_PAGE_MAX_NUM+1,ListTotal/TAB_FACE_PAGE_MAX_NUM,TAB_FACE_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	InvalidateRect(printf_Rect, TRUE);

}

void CDLGTabFace::DisplayerPicList(void)
{
	faceTempResList.clear();
	faceBlackResList.clear();
	DlgPictureFace.CleanList();

	char str[64]={0};

	switch(ModeFlag)
	{
	case  FLAG_CAP:

#if OPEN_FACEDETECT_CODE
		FaceRecg.ImageCmpWihtTempFace(DlgFaceSreach.filepath,tempcondition,faceTempResList);
		if(faceTempResList.size()>0)
			DlgPictureFace.DisplayRecgTEMPList(faceTempResList,FLAG_FAC_CAP);
#endif

		ListTotal=faceTempResList.size();

		break;
	case  FLAG_BLACK:
#if OPEN_FACEDETECT_CODE
		FaceRecg.ImageCmpWihtBlackFace(DlgFaceSreach.filepath,blackcondition,faceBlackResList);
		if(faceBlackResList.size()>0)
			DlgPictureFace.DisplayRecgBLACKList(faceBlackResList,FLAG_FACE_BLACK_ONE);
	
#endif
		ListTotal=faceBlackResList.size();
		break;


	default:
		MessageBox("CDLGTabFace::DisplayerPicList A error",MESSAGEBOX_TITLE);
		return ;
	}

	sprintf(str,"共%d条 %d/%d页",ListTotal,	1,1);
	
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	InvalidateRect(printf_Rect, TRUE);

}


void CDLGTabFace::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if(FLAG_HISTORY_FACE !=ModeFlag)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGTabFace::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if(FLAG_HISTORY_FACE !=ModeFlag)
		return ;

	if((ListNow-TAB_FACE_PAGE_MAX_NUM) >= 0)
		ListNow-=TAB_FACE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGTabFace::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if(FLAG_HISTORY_FACE !=ModeFlag)
		return ;

	if((ListNow+TAB_FACE_PAGE_MAX_NUM) < ListTotal)
		ListNow+=TAB_FACE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGTabFace::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if(FLAG_HISTORY_FACE !=ModeFlag)
		return ;

	while((ListNow+TAB_FACE_PAGE_MAX_NUM) < ListTotal)
		ListNow+=TAB_FACE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGTabFace::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if(FLAG_HISTORY_FACE !=ModeFlag)
		return ;

	if((m_page>=1)
		&& (m_page <= (ListTotal/TAB_FACE_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+TAB_FACE_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+TAB_FACE_PAGE_MAX_NUM) < (TAB_FACE_PAGE_MAX_NUM*m_page))
			)
			ListNow+=TAB_FACE_PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);

}

void CDLGTabFace::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DlgFaceSreach.UpdateData(TRUE);

	HTREEITEM treeid=m_tree.GetSelectedItem();

	memset(SqlStr,0,1024);

	char STime[32]={0};
	char ETime[32]={0};


	if(	DlgFaceSreach.m_StartMon.GetYear() <1900  || DlgFaceSreach.m_StartMon.GetYear() >2100 ||
		DlgFaceSreach.m_EndMon.GetYear()<1900  || DlgFaceSreach.m_EndMon.GetYear() >2100)  
	{
		MessageBox("时间范围不能小于1900年，不大于2100年",MESSAGEBOX_TITLE);
		return ;
	}

	if(	DlgFaceSreach.m_Birth_Start.GetYear() <1900  || DlgFaceSreach.m_Birth_Start.GetYear() >2100 ||
		DlgFaceSreach.m_Birth_End.GetYear()<1900  || DlgFaceSreach.m_Birth_End.GetYear() >2100) 
	{
		MessageBox("时间范围不能小于1900年，不大于2100年",MESSAGEBOX_TITLE);
		return ;
	}

	sprintf(STime,"%04d%02d%02d%02d%02d%02d",		
		DlgFaceSreach.m_StartMon.GetYear(),
		DlgFaceSreach.m_StartMon.GetMonth(),
		DlgFaceSreach.m_StartMon.GetDay(),
		DlgFaceSreach.m_StartHour.GetHour(),
		DlgFaceSreach.m_StartHour.GetMinute(),
		DlgFaceSreach.m_StartHour.GetSecond());

	sprintf(ETime,"%04d%02d%02d%02d%02d%02d",		
		DlgFaceSreach.m_EndMon.GetYear(),
		DlgFaceSreach.m_EndMon.GetMonth(),
		DlgFaceSreach.m_EndMon.GetDay(),
		DlgFaceSreach.m_EndHour.GetHour(),
		DlgFaceSreach.m_EndHour.GetMinute(),
		DlgFaceSreach.m_EndHour.GetSecond());

	/////////获取
	searchFlag = 0;
/*
	if(DlgFaceSreach.m_age_start >DlgFaceSreach.m_age_end)
	{
		MessageBox("起始年龄 不得大于 结束年龄");
		return ;
	}
	*/
	long ncam=0;
	int ncamflag=0;

	if(treeid == TempRootitem)
	{
		ncamflag=TREE_TEMP_ALL;
	}
	else 	if(treeid == BlackRootitem)
	{
		ncamflag=TREE_BLACK_ALL;
	}
	else
	{
		ncamflag=TREE_TEMP_ALL;

		for(int i=0;i<dbtotal;i++)
		{
			if(treeid == dblist[i].item)
			{
				if(0==dblist[i].ncamera)
				{
					ncam=dblist[i].nid;
					ncamflag=TREE_BLACK_ONE;
				}	
				else
				{
					ncam=dblist[i].ncamera;
					ncamflag=TREE_TEMP_ONE;
				}
				break;
			}
		}
	}

	bool picflag=false;
	if(0!=strlen(DlgFaceSreach.filepath))
	{
		picflag=true;
	}

	if(TREE_TEMP_ALL == ncamflag || TREE_TEMP_ONE == ncamflag)
	{
		ModeFlag=FLAG_CAP;

		if(false==picflag)
			ModeFlag=FLAG_HISTORY_FACE;
	}
	else
	{
		ModeFlag=FLAG_BLACK;
		if(false==picflag)
		{
			MessageBox("黑名单 必须使用图片比对");
			return ;
		}
	}

	if(TREE_TEMP_ONE == ncamflag  )
	{		
		searchFlag |= 0x01;
	}
	if(DlgFaceSreach.m_c_sex!=0)
	{
		searchFlag |= 0x02;
	}
	if(DlgFaceSreach.m_Birth_Start !=DlgFaceSreach.m_Birth_End)
	{
		searchFlag |= 0x04;
	}
	//这里历史不管年龄
	if(DlgFaceSreach.m_CheckTime)
	{
		searchFlag |= 0x08;
		COleDateTime cstime(
			DlgFaceSreach.m_StartMon.GetYear(),
			DlgFaceSreach.m_StartMon.GetMonth(),
			DlgFaceSreach.m_StartMon.GetDay(),
			DlgFaceSreach.m_StartHour.GetHour(),
			DlgFaceSreach.m_StartHour.GetMinute(),
			DlgFaceSreach.m_StartHour.GetSecond());

		COleDateTime cetime(
			DlgFaceSreach.m_EndMon.GetYear(),
			DlgFaceSreach.m_EndMon.GetMonth(),
			DlgFaceSreach.m_EndMon.GetDay(),
			DlgFaceSreach.m_EndHour.GetHour(),
			DlgFaceSreach.m_EndHour.GetMinute(),
			DlgFaceSreach.m_EndHour.GetSecond());

		if(cstime>cetime)
		{
			MessageBox("起始时间 不得大于 结束时间");
			return ;
		}
	

	}
	if(TREE_BLACK_ONE ==ncamflag)
	{
		searchFlag |= 0x10;
	}
	if(!DlgFaceSreach.m_e_name.IsEmpty())
	{
		searchFlag |= 0x20;
	}
	if(!DlgFaceSreach.m_c_cardtype!=0)
	{
		searchFlag |= 0x40;
	}
	if(!DlgFaceSreach.m_e_cardnum.IsEmpty())
	{
		searchFlag |= 0x80;
	}
	if(!DlgFaceSreach.m_e_phone.IsEmpty())
	{
		searchFlag |= 0x100;
	}
	if(!DlgFaceSreach.m_c_jiguan!=0)
	{
		searchFlag |= 0x200;
	}
	if(!DlgFaceSreach.m_e_addr.IsEmpty())
	{
		searchFlag |= 0x400;
	}

	COleDateTime bstime(
		DlgFaceSreach.m_Birth_Start.GetYear(),
		DlgFaceSreach.m_Birth_Start.GetMonth(),
		DlgFaceSreach.m_Birth_Start.GetDay(),
		DlgFaceSreach.m_Birth_Start.GetHour(),
		DlgFaceSreach.m_Birth_Start.GetMinute(),
		DlgFaceSreach.m_Birth_Start.GetSecond());

	COleDateTime betime(
		DlgFaceSreach.m_Birth_End.GetYear(),
		DlgFaceSreach.m_Birth_End.GetMonth(),
		DlgFaceSreach.m_Birth_End.GetDay(),
		DlgFaceSreach.m_Birth_End.GetHour(),
		DlgFaceSreach.m_Birth_End.GetMinute(),
		DlgFaceSreach.m_Birth_End.GetSecond());

	if(bstime>betime)
	{
		MessageBox("起始生日 不得大于 结束生日");
		return ;
	}

	if(FLAG_HISTORY_FACE!= ModeFlag )
	{
		if(DlgFaceSreach.m_c_limitnum && DlgFaceSreach.m_e_limitnum>30)
		{
			return ;
		}
	}

	ListTotal=0;

	switch(ModeFlag)
	{
	case FLAG_HISTORY_FACE :
		ListTotal=MySqlIO.FACE_Capture_GetNum(
			ncam,
			DlgFaceSreach.m_c_sex,
			1,
			100,
			STime,
			ETime,
			searchFlag,
			SqlStr);

		ListNow=0;
		DisplayerList();

		break;
	case  FLAG_CAP:
		memset(&tempcondition,0,sizeof(struct TEMP_CMP_CONDITION_ST));
		if(DlgFaceSreach.m_c_limitnum)
			tempcondition.max_match_count=DlgFaceSreach.m_e_limitnum;
		else
			tempcondition.max_match_count=32;

		tempcondition.min_similarity=DlgFaceSreach.m_e_sim_start;
		tempcondition.ncamera=ncam;
		tempcondition.sex=DlgFaceSreach.m_c_sex;

		strcpy(tempcondition.stime,STime);
		strcpy(tempcondition.etime,ETime);
		tempcondition.flag=searchFlag;

		ListNow=0;
		DisplayerPicList();
		break;

	case  FLAG_BLACK:
		memset(&blackcondition,0,sizeof(struct BLACK_CMP_CONDITION_ST));
		if(DlgFaceSreach.m_c_limitnum)
			blackcondition.max_match_count=DlgFaceSreach.m_e_limitnum;
		else
			blackcondition.max_match_count=32;
		blackcondition.min_similarity=DlgFaceSreach.m_e_sim_start;
		blackcondition.flag=searchFlag;

		blackcondition.sex=DlgFaceSreach.m_c_sex;
		blackcondition.db_id=ncam;

		strcpy(blackcondition.name,DlgFaceSreach.m_e_name.GetBuffer(0));

		blackcondition.cardtype=DlgFaceSreach.m_c_cardtype;

		strcpy(blackcondition.cardNO,DlgFaceSreach.m_e_cardnum.GetBuffer(0));
		strcpy(blackcondition.phone,DlgFaceSreach.m_e_phone.GetBuffer(0));

		blackcondition.native=DlgFaceSreach.m_c_jiguan;
		strcpy(blackcondition.address,DlgFaceSreach.m_e_addr.GetBuffer(0));

		sprintf(STime,"%04d%02d%02d%02d%02d%02d",		
			DlgFaceSreach.m_Birth_Start.GetYear(),
			DlgFaceSreach.m_Birth_Start.GetMonth(),
			DlgFaceSreach.m_Birth_Start.GetDay(),
			DlgFaceSreach.m_Birth_Start.GetHour(),
			DlgFaceSreach.m_Birth_Start.GetMinute(),
			DlgFaceSreach.m_Birth_Start.GetSecond());

		sprintf(ETime,"%04d%02d%02d%02d%02d%02d",		
			DlgFaceSreach.m_Birth_End.GetYear(),
			DlgFaceSreach.m_Birth_End.GetMonth(),
			DlgFaceSreach.m_Birth_End.GetDay(),
			DlgFaceSreach.m_Birth_End.GetHour(),
			DlgFaceSreach.m_Birth_End.GetMinute(),
			DlgFaceSreach.m_Birth_End.GetSecond());

		strcpy(blackcondition.sage,STime);
		strcpy(blackcondition.eage,ETime);

		ListNow=0;
		DisplayerPicList();
		break;
		
	default:
		MessageBox("CDLGTabFace::OnButtonSearch A error",MESSAGEBOX_TITLE);
		return ;
	}


}
void CDLGTabFace::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//HTREEITEM treeid=m_tree.GetSelectedItem();
	CPoint point;
	GetCursorPos(&point);
	CPoint PointInTree = point;
	m_tree.ScreenToClient( &PointInTree);
	UINT nFlag = TVHT_ONITEM;
	HTREEITEM treeid = m_tree.HitTest( PointInTree, &nFlag );
	int iCurSeachMode = 0;

	if(treeid == TempRootitem || m_tree.GetParentItem(treeid) == TempRootitem)
	{
		DlgFaceSreach.GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
		DlgFaceSreach.GetDlgItem(IDC_COMBO_CARDTYPE)->EnableWindow(FALSE);
		DlgFaceSreach.GetDlgItem(IDC_EDIT_CARDNUM)->EnableWindow(FALSE);
		DlgFaceSreach.GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(FALSE);
		DlgFaceSreach.GetDlgItem(IDC_COMBO_JIGUAN)->EnableWindow(FALSE);
		DlgFaceSreach.GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(FALSE);
		DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_START)->EnableWindow(FALSE);
		DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_END)->EnableWindow(FALSE);
		iCurSeachMode = 0;
	}
	else if(treeid == BlackRootitem || m_tree.GetParentItem(treeid) == BlackRootitem)
	{
		DlgFaceSreach.GetDlgItem(IDC_EDIT_NAME)->EnableWindow(TRUE);
		DlgFaceSreach.GetDlgItem(IDC_COMBO_CARDTYPE)->EnableWindow(TRUE);
		DlgFaceSreach.GetDlgItem(IDC_EDIT_CARDNUM)->EnableWindow(TRUE);
		DlgFaceSreach.GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(TRUE);
		DlgFaceSreach.GetDlgItem(IDC_COMBO_JIGUAN)->EnableWindow(TRUE);
		DlgFaceSreach.GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(TRUE);
		DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_START)->EnableWindow(TRUE);
		DlgFaceSreach.GetDlgItem(IDC_DATETIMEPICKER_BIRTH_END)->EnableWindow(TRUE);

		iCurSeachMode = 1;
	}

	if(iLastSearchMode != iCurSeachMode)
	{
		iLastSearchMode = iCurSeachMode;
		DlgFaceSreach.Invalidate();
	}
	*pResult = 0;
}
