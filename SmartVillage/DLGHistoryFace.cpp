// DLGHistoryFace.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGHistoryFace.h"

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


// CDLGHistoryFace dialog

IMPLEMENT_DYNAMIC(CDLGHistoryFace, CDialog)

CDLGHistoryFace::CDLGHistoryFace(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGHistoryFace::IDD, pParent)
{
	m_page = 1;
	
	ListTotal=0;
	ListNow=0;
}

CDLGHistoryFace::~CDLGHistoryFace()
{

}

void CDLGHistoryFace::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CDLGHistoryFace, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGHistoryFace::OnOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGHistoryFace::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
		ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDLGHistoryFace message handlers
BOOL CDLGHistoryFace::OnInitDialog()
{
	CDialog::OnInitDialog();
	DlgFaceSreachLite.Create(IDD_FACE_SREACH_LITE,this);
	DlgFaceSreachLite.ShowWindow(SW_SHOW);

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

	m_tree.SetBkColor(RGB(254,254,254));	//树列表背景色

	if( HISTORY_DETECT_FLAG_FACE ==flag)
	{
		SetWindowText("人脸抓拍查询");
	}
	else if( HISTORY_DETECT_FLAG_FACEALARM ==flag)
	{
		SetWindowText("人脸报警查询");
	}

	imagelist.Create(16, 16, ILC_COLOR8|ILC_MASK, 0, 4);    
	imagelist.Add(AfxGetApp()->LoadIcon(IDI_TREE_AREA));        // ico图标
	imagelist.Add(AfxGetApp()->LoadIcon(IDI_TREE_CAM));
	m_tree.SetImageList(&imagelist, TVSIL_NORMAL);  // 建立 imagelist 与 tree的映射关系


	BulidTree();
	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGHistoryFace::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);
	int distance=5;
	int treew=220;
	int sreachh=100;

	int buttonh=25;
	int buttonbmpw=24,buttonbmph=18,buttongap=10;

	//树
	CRect tree_Rect;
	tree_Rect.top = rc.top;
	tree_Rect.bottom = rc.bottom-distance;
	tree_Rect.left = rc.left;
	tree_Rect.right = rc.left+treew;

	m_tree.MoveWindow(tree_Rect);

	//检索条件
	CRect sreach_Rect;
	sreach_Rect.top = rc.top;
	sreach_Rect.bottom = rc.top+sreachh;
	sreach_Rect.left = tree_Rect.right+distance;
	sreach_Rect.right = rc.right-100;

	DlgFaceSreachLite.MoveWindow(sreach_Rect);

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

	b_Rect.left = tree_Rect.right+10;
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

// CDLGHistoryFace message handlers

void CDLGHistoryFace::OnOk()
{
	// TODO: Add your control notification handler code here
		//CDialog::OnOK();
}

void CDLGHistoryFace::OnCancel()
{
	// TODO: Add your control notification handler code here
	faceList.clear();
	DlgPictureFace.CleanList();
	CDialog::OnCancel();
}

void CDLGHistoryFace::OnPaint()
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
HBRUSH CDLGHistoryFace::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDLGHistoryFace::BulidTree(void)
{
	dbtotal=0;


	//if( HISTORY_DETECT_FLAG_FACE ==flag)
	{
		rootitem = m_tree.InsertItem("抓拍库",0,0,TVI_ROOT);		//在根结点上添加区域

		list<struct FACE_TEMP_DB_ST> TempDBList;
		MySqlIO.TEMP_DB_ReadAll(TempDBList);
		if(TempDBList.size()<=0)
			return ;

		
		list<struct FACE_TEMP_DB_ST>::iterator beglist;

		for(beglist=TempDBList.begin();beglist!=TempDBList.end();beglist++)
		{
			dblist[dbtotal].item = m_tree.InsertItem(beglist->name,1,1,rootitem);		//在根结点上添加区域
			dblist[dbtotal].nid=beglist->nid;
			dblist[dbtotal].RWdb_nid=beglist->RW_tempID;
			dblist[dbtotal].ncamera=beglist->ncamera;
			dbtotal++;
		}
	}
	/*
	else if( HISTORY_DETECT_FLAG_FACEALARM ==flag)
	{
		
		rootitem= m_tree.InsertItem("黑名单库",0,0,TVI_ROOT);		//在根结点上添加区域

		list<struct FACE_DB_ST> BlackDBList;
		MySqlIO.FACE_BlackDB_ReadAll(BlackDBList);
		if(BlackDBList.size()<=0)
			return ;

		list<struct FACE_DB_ST>::iterator beglist;

		for(beglist=BlackDBList.begin();beglist!=BlackDBList.end();beglist++)
		{
			dblist[dbtotal].item = m_tree.InsertItem(beglist->dbname,1,1,rootitem);		//在根结点上添加区域
			dblist[dbtotal].nid=beglist->nid;
			dblist[dbtotal].RWdb_nid=beglist->RW_dbID;
			dbtotal++;
		}
	}
	*/
	//全部展开
	m_tree.Expand(rootitem,TVE_EXPAND);
}

void CDLGHistoryFace::DisplayerList(void)
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
	if((ListNow+HISTORY_FACE_PAGE_MAX_NUM) < ListTotal)
		ei=ListNow+HISTORY_FACE_PAGE_MAX_NUM ;
	else
		ei=ListTotal;

	
	char str[64]={0};

	switch(flag)
	{
	case HISTORY_DETECT_FLAG_FACE :
			MySqlIO.FACE_Capture_Read(SqlStr,searchFlag,si,ei,faceList);
			if(faceList.size()>0)
				DlgPictureFace.DisplayCapList(faceList,FLAG_FAC_CAP);
		break;
	case  HISTORY_DETECT_FLAG_FACEALARM:
			MySqlIO.FACE_Capture_ReadAlarmList(SqlStr,searchFlag,si,ei,faceList);
			if(faceList.size()>0)
				DlgPictureFace.DisplayCapList(faceList,FLAG_FACE_ALARM);
		break;
	default:
		MessageBox("CDLGHistoryFace::DisplayerList A error",MESSAGEBOX_TITLE);
		return ;
	}



	if(ListTotal%HISTORY_FACE_PAGE_MAX_NUM)
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/HISTORY_FACE_PAGE_MAX_NUM+1,ListTotal/HISTORY_FACE_PAGE_MAX_NUM+1,HISTORY_FACE_PAGE_MAX_NUM);
	}
	else
	{
		sprintf(str,"共%d条 %d/%d页 每页%d条",
			ListTotal,
			ListNow/HISTORY_FACE_PAGE_MAX_NUM+1,ListTotal/HISTORY_FACE_PAGE_MAX_NUM,HISTORY_FACE_PAGE_MAX_NUM);
	}
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
	InvalidateRect(printf_Rect, TRUE);

}
void CDLGHistoryFace::OnButtonFirst() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	ListNow=0;
	DisplayerList();
}

void CDLGHistoryFace::OnButtonPrevious() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow-HISTORY_FACE_PAGE_MAX_NUM) >= 0)
		ListNow-=HISTORY_FACE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryFace::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	if((ListNow+HISTORY_FACE_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_FACE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryFace::OnButtonLast() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	while((ListNow+HISTORY_FACE_PAGE_MAX_NUM) < ListTotal)
		ListNow+=HISTORY_FACE_PAGE_MAX_NUM;

	DisplayerList();
}

void CDLGHistoryFace::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	if(0==ListTotal)
		return ;

	UpdateData(TRUE);

	if((m_page>=1)
		&& (m_page <= (ListTotal/HISTORY_FACE_PAGE_MAX_NUM+1))
		)
	{
		ListNow=0;
		while(((ListNow+HISTORY_FACE_PAGE_MAX_NUM) < ListTotal)
			&& ((ListNow+HISTORY_FACE_PAGE_MAX_NUM) < (HISTORY_FACE_PAGE_MAX_NUM*m_page))
			)
			ListNow+=HISTORY_FACE_PAGE_MAX_NUM;

		DisplayerList();
	}
	else
		MessageBox("不在页面范围",MESSAGEBOX_TITLE);

}
void CDLGHistoryFace::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	AutoSize();
}
void CDLGHistoryFace::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DlgFaceSreachLite.UpdateData(TRUE);

	HTREEITEM treeid=m_tree.GetSelectedItem();

	memset(SqlStr,0,1024);

	char STime[32]={0};
	char ETime[32]={0};

	if(	DlgFaceSreachLite.m_StartMon.GetYear() <1900  || DlgFaceSreachLite.m_StartMon.GetYear() >2100 ||
		DlgFaceSreachLite.m_EndMon.GetYear()<1900  || DlgFaceSreachLite.m_EndMon.GetYear() >2100) 
	{
		MessageBox("时间范围不能小于1900年，不大于2100年",MESSAGEBOX_TITLE);
		return ;
	}

	sprintf(STime,"%04d%02d%02d%02d%02d%02d",		
		DlgFaceSreachLite.m_StartMon.GetYear(),
		DlgFaceSreachLite.m_StartMon.GetMonth(),
		DlgFaceSreachLite.m_StartMon.GetDay(),
		DlgFaceSreachLite.m_StartHour.GetHour(),
		DlgFaceSreachLite.m_StartHour.GetMinute(),
		DlgFaceSreachLite.m_StartHour.GetSecond());

	sprintf(ETime,"%04d%02d%02d%02d%02d%02d",		
		DlgFaceSreachLite.m_EndMon.GetYear(),
		DlgFaceSreachLite.m_EndMon.GetMonth(),
		DlgFaceSreachLite.m_EndMon.GetDay(),
		DlgFaceSreachLite.m_EndHour.GetHour(),
		DlgFaceSreachLite.m_EndHour.GetMinute(),
		DlgFaceSreachLite.m_EndHour.GetSecond());

	/////////获取
	searchFlag = 0;

	if(DlgFaceSreachLite.m_age_start >DlgFaceSreachLite.m_age_end)
	{
		MessageBox("起始年龄 不得大于 结束年龄");
		return ;
	}

	long ncam=0;
	bool ncamflag=false;

	if(treeid == rootitem)
	{
		ncamflag=false;
	}
	else
	{
		for(int i=0;i<dbtotal;i++)
		{
			if(treeid == dblist[i].item)
			{
				ncam=dblist[i].ncamera;
				ncamflag=true;
				break;
			}
		}
	}
	if(ncamflag)
	{		
		searchFlag |= 0x01;
	}
	if(DlgFaceSreachLite.m_c_sex!=0)
	{
		searchFlag |= 0x02;
	}
	searchFlag |= 0x04;
	if(DlgFaceSreachLite.m_CheckTime)
	{
		searchFlag |= 0x08;

		COleDateTime cstime(
			DlgFaceSreachLite.m_StartMon.GetYear(),
			DlgFaceSreachLite.m_StartMon.GetMonth(),
			DlgFaceSreachLite.m_StartMon.GetDay(),
			DlgFaceSreachLite.m_StartHour.GetHour(),
			DlgFaceSreachLite.m_StartHour.GetMinute(),
			DlgFaceSreachLite.m_StartHour.GetSecond());
		
		COleDateTime cetime(
			DlgFaceSreachLite.m_EndMon.GetYear(),
			DlgFaceSreachLite.m_EndMon.GetMonth(),
			DlgFaceSreachLite.m_EndMon.GetDay(),
			DlgFaceSreachLite.m_EndHour.GetHour(),
			DlgFaceSreachLite.m_EndHour.GetMinute(),
			DlgFaceSreachLite.m_EndHour.GetSecond());
		
		if(cstime>cetime)
		{
			MessageBox("起始时间 不得大于 结束时间");
			return ;
		}
	}

	ListTotal=0;

	switch(flag)
	{
	case HISTORY_DETECT_FLAG_FACE :
		ListTotal=MySqlIO.FACE_Capture_GetNum(
			ncam,
			DlgFaceSreachLite.m_c_sex,
			DlgFaceSreachLite.m_age_start,
			DlgFaceSreachLite.m_age_end,
			STime,
			ETime,
			searchFlag,
			SqlStr);
		break;
	case  HISTORY_DETECT_FLAG_FACEALARM:
		ListTotal=MySqlIO.FACE_Capture_GetAlarmNum(
			ncam,
			DlgFaceSreachLite.m_c_sex,
			DlgFaceSreachLite.m_age_start,
			DlgFaceSreachLite.m_age_end,
			STime,
			ETime,
			searchFlag,
			SqlStr);
		break;
	default:
		MessageBox("CDLGHistoryFace::OnButtonSearch A error",MESSAGEBOX_TITLE);
		return ;
	}

	ListNow=0;

	DisplayerList();
}