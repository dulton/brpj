// DLGptz.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGptz.h"

#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

//////////////////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGptz dialog


CDLGptz::CDLGptz(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGptz::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGptz)
	m_edit_speed = 5;
	m_edit_set = 1;
	//}}AFX_DATA_INIT
}


void CDLGptz::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGptz)

	DDX_Control(pDX, IDC_AUTO, m_auto);
	DDX_Control(pDX, IDC_BUTTON_GO, m_go);
	DDX_Control(pDX, IDC_BUTTON_SET, m_set);
	DDX_Control(pDX, IDC_DOWN, m_down);
	DDX_Control(pDX, IDC_DOWNLEFT, m_downleft);
	DDX_Control(pDX, IDC_DOWNRIGHT, m_downright);
	DDX_Control(pDX, IDC_FOCUS_SUB, m_focussub);
	DDX_Control(pDX, IDC_FOCUS_ADD, m_focusadd);
	DDX_Control(pDX, IDC_IRIS_ADD, m_irisadd);
	DDX_Control(pDX, IDC_IRIS_SUB, m_irissub);
	DDX_Control(pDX, IDC_LEFT, m_left);
	DDX_Control(pDX, IDC_RIGHT, m_right);
	DDX_Control(pDX, IDC_SPEED_ADD, m_speedadd);
	DDX_Control(pDX, IDC_SPEED_SUB, m_speedsub);
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_ZOOM_SUB, m_zoomsub);
	DDX_Control(pDX, IDC_ZOOM_ADD, m_zoomadd);
	DDX_Control(pDX, IDC_UPRIGHT, m_upright);
	DDX_Control(pDX, IDC_UPLEFT, m_upleft);

	DDX_Text(pDX, IDC_EDIT_SPEED, m_edit_speed);
	DDV_MinMaxInt(pDX, m_edit_speed, 1, 8);
	DDX_Text(pDX, IDC_EDIT_SET, m_edit_set);
	DDV_MinMaxInt(pDX, m_edit_set, 1, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGptz, CDialog)
	//{{AFX_MSG_MAP(CDLGptz)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_UPLEFT, OnUpleft)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_UPRIGHT, OnUpright)
	ON_BN_CLICKED(IDC_LEFT, OnLeft)
	ON_BN_CLICKED(IDC_AUTO, OnAuto)
	ON_BN_CLICKED(IDC_RIGHT, OnRight)
	ON_BN_CLICKED(IDC_DOWNLEFT, OnDownleft)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_DOWNRIGHT, OnDownright)
	ON_BN_CLICKED(IDC_FOCUS_ADD, OnFocusAdd)
	ON_BN_CLICKED(IDC_IRIS_ADD, OnIrisAdd)
	ON_BN_CLICKED(IDC_ZOOM_ADD, OnZoomAdd)
	ON_BN_CLICKED(IDC_FOCUS_SUB, OnFocusSub)
	ON_BN_CLICKED(IDC_IRIS_SUB, OnIrisSub)
	ON_BN_CLICKED(IDC_ZOOM_SUB, OnZoomSub)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_GO, OnButtonGo)
	ON_BN_CLICKED(IDC_SPEED_SUB, OnSpeedSub)
	ON_BN_CLICKED(IDC_SPEED_ADD, OnSpeedAdd)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGptz message handlers
BOOL CDLGptz::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitPtzCommand();

	//贴图片
	ButtonBMP();


	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGptz::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);

//	GetParent()->GetClientRect(&rc);
//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
//	MoveWindow(rc);

	Invalidate();
}

void CDLGptz::InitPtzCommand()
{		

	m_auto.SetButtonCommand(PTZ_CONTROL_AUTO,NORMAL_PTZ_FLAG);
	m_up.SetButtonCommand(PTZ_CONTROL_UP,NORMAL_PTZ_FLAG);
	m_down.SetButtonCommand(PTZ_CONTROL_DOWN,NORMAL_PTZ_FLAG);
	m_left.SetButtonCommand(PTZ_CONTROL_LEFT,NORMAL_PTZ_FLAG);
	m_right.SetButtonCommand(PTZ_CONTROL_RIGHT,NORMAL_PTZ_FLAG);
	m_upleft.SetButtonCommand(PTZ_CONTROL_UPLEFT,NORMAL_PTZ_FLAG);
	m_upright.SetButtonCommand(PTZ_CONTROL_UPRIGHT,NORMAL_PTZ_FLAG);
	m_downleft.SetButtonCommand(PTZ_CONTROL_DOWNLEFT,NORMAL_PTZ_FLAG);
	m_downright.SetButtonCommand(PTZ_CONTROL_DOWNRIGHT,NORMAL_PTZ_FLAG);
	m_focusadd.SetButtonCommand(PTZ_CONTROL_FOCUS_ADD,NORMAL_PTZ_FLAG);
	m_focussub.SetButtonCommand(PTZ_CONTROL_FOCUS_SUB,NORMAL_PTZ_FLAG);
	m_irisadd.SetButtonCommand(PTZ_CONTROL_IRIS_ADD,NORMAL_PTZ_FLAG);
	m_irissub.SetButtonCommand(PTZ_CONTROL_IRIS_SUB,NORMAL_PTZ_FLAG);
	m_zoomadd.SetButtonCommand(PTZ_CONTROL_ZOOM_ADD,NORMAL_PTZ_FLAG);
	m_zoomsub.SetButtonCommand(PTZ_CONTROL_ZOOM_SUB,NORMAL_PTZ_FLAG);
	m_set.SetButtonCommand(PTZ_CONTROL_POINT_SET,NORMAL_PTZ_FLAG);
	m_go.SetButtonCommand(PTZ_CONTROL_POINT_MOVE,NORMAL_PTZ_FLAG);
	m_speedadd.SetButtonCommand(PTZ_CONTROL_SPEED_ADD,NORMAL_PTZ_FLAG);
	m_speedsub.SetButtonCommand(PTZ_CONTROL_SPEED_SUB,NORMAL_PTZ_FLAG);
}

void CDLGptz::ButtonBMP() 
{

	m_auto.LoadBitmaps(IDB_PTZ_AUTO_OPEN,IDB_PTZ_AUTO_MOVE,NULL,IDB_PTZ_AUTO_CLOSE);
	m_auto.SizeToContent();		//自适应图片大小

	m_go.LoadBitmaps(IDB_PTZ_GO_OPEN,IDB_PTZ_GO_MOVE,NULL,IDB_PTZ_GO_CLOSE);
	m_go.SizeToContent();		//自适应图片大小

	m_set.LoadBitmaps(IDB_PTZ_SET_OPEN,IDB_PTZ_SET_MOVE,NULL,IDB_PTZ_SET_CLOSE);
	m_set.SizeToContent();		//自适应图片大小

	m_down.LoadBitmaps(IDB_PTZ_DOWN_OPEN,IDB_PTZ_DOWN_MOVE,NULL,IDB_PTZ_DOWN_CLOSE);
	m_down.SizeToContent();		//自适应图片大小

	m_downleft.LoadBitmaps(IDB_PTZ_LEFTDOWN_OPEN,IDB_PTZ_LEFTDOWN_MOVE,NULL,IDB_PTZ_LEFTDOWN_CLOSE);
	m_downleft.SizeToContent();		//自适应图片大小

	m_downright.LoadBitmaps(IDB_PTZ_RIGHTDOWN_OPEN,IDB_PTZ_RIGHTDOWN_MOVE,NULL,IDB_PTZ_RIGHTDOWN_CLOSE);
	m_downright.SizeToContent();		//自适应图片大小

	m_focusadd.LoadBitmaps(IDB_PTZ_FOCUSADD_OPEN,IDB_PTZ_FOCUSADD_MOVE,NULL,IDB_PTZ_FOCUSADD_CLOSE);
	m_focusadd.SizeToContent();		//自适应图片大小
	
	m_focussub.LoadBitmaps(IDB_PTZ_FOCUSSUB_OPEN,IDB_PTZ_FOCUSSUB_MOVE,NULL,IDB_PTZ_FOCUSSUB_CLOSE);
	m_focussub.SizeToContent();		//自适应图片大小

	m_irisadd.LoadBitmaps(IDB_PTZ_IRISADD_OPEN,IDB_PTZ_IRISADD_MOVE,NULL,IDB_PTZ_IRISADD_CLOSE);
	m_irisadd.SizeToContent();		//自适应图片大小
	
	m_irissub.LoadBitmaps(IDB_PTZ_IRISSUB_OPEN,IDB_PTZ_IRISSUB_MOVE,NULL,IDB_PTZ_IRISSUB_CLOSE);
	m_irissub.SizeToContent();		//自适应图片大小

	m_left.LoadBitmaps(IDB_PTZ_LEFT_OPEN,IDB_PTZ_LEFT_MOVE,NULL,IDB_PTZ_LEFT_CLOSE);
	m_left.SizeToContent();		//自适应图片大小
	
	m_right.LoadBitmaps(IDB_PTZ_RIGHT_OPEN,IDB_PTZ_RIGHT_MOVE,NULL,IDB_PTZ_RIGHT_CLOSE);
	m_right.SizeToContent();		//自适应图片大小

	m_speedadd.LoadBitmaps(IDB_PTZ_SPEEDADD_OPEN,IDB_PTZ_SPEEDADD_MOVE,NULL,IDB_PTZ_SPEEDADD_CLOSE);
	m_speedadd.SizeToContent();		//自适应图片大小
	
	m_speedsub.LoadBitmaps(IDB_PTZ_SPEEDSUB_OPEN,IDB_PTZ_SPEEDSUB_MOVE,NULL,IDB_PTZ_SPEEDSUB_CLOSE);
	m_speedsub.SizeToContent();		//自适应图片大小

	m_up.LoadBitmaps(IDB_PTZ_UP_OPEN,IDB_PTZ_UP_MOVE,NULL,IDB_PTZ_UP_CLOSE);
	m_up.SizeToContent();		//自适应图片大小

	m_zoomsub.LoadBitmaps(IDB_PTZ_ZOOMSUB_OPEN,IDB_PTZ_ZOOMSUB_MOVE,NULL,IDB_PTZ_ZOOMSUB_CLOSE);
	m_zoomsub.SizeToContent();		//自适应图片大小

	m_zoomadd.LoadBitmaps(IDB_PTZ_ZOOMADD_OPEN,IDB_PTZ_ZOOMADD_MOVE,NULL,IDB_PTZ_ZOOMADD_CLOSE);
	m_zoomadd.SizeToContent();		//自适应图片大小

	m_upright.LoadBitmaps(IDB_PTZ_RIGHTUP_OPEN,IDB_PTZ_RIGHTUP_MOVE,NULL,IDB_PTZ_RIGHTUP_CLOSE);
	m_upright.SizeToContent();		//自适应图片大小

	m_upleft.LoadBitmaps(IDB_PTZ_LEFTUP_OPEN,IDB_PTZ_LEFTUP_MOVE,NULL,IDB_PTZ_LEFTUP_CLOSE);
	m_upleft.SizeToContent();		//自适应图片大小

}

void CDLGptz::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     
	
	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_BACK_PTZ);    
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

//云台控制操作
void CDLGptz::SendPtzControl(int type, BOOL dwStop)
{
	UpdateData(TRUE);
	BOOL b = FALSE;
	int nParam = 0;

	if(dwStop == FALSE)
	{
		if(type == PTZ_CONTROL_SPEED_ADD)
		{
			if(m_edit_speed < 8)
			{
				m_edit_speed ++;
				UpdateData(FALSE);
			}
		}
		
		if(type == PTZ_CONTROL_SPEED_SUB)
		{
			if(m_edit_speed > 1)
			{
				m_edit_speed --;
				UpdateData(FALSE);
			}
		}
	}
	if(type == PTZ_CONTROL_POINT_MOVE || type == PTZ_CONTROL_POINT_SET)
	{
	//	nParam = GetDlgItemInt(IDC_EDIT_SET, &b, FALSE);
	
		nParam =m_edit_set;
	}
	else
	{
	//	nParam = GetDlgItemInt(IDC_EDIT_SPEED, &b, FALSE);
		nParam= m_edit_speed;
	}
//	if (b)
	{
		DlgMain->DlgScreen.PtzControl(type,dwStop,nParam);
	}
}



//按键事件响应///////////////////////////////
void CDLGptz::OnUpleft() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnUp() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnUpright() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnLeft() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnAuto() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnRight() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnDownleft() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnDown() 
{
	// TODO: Add your control notification handler code here

}

void CDLGptz::OnDownright() 
{
	// TODO: Add your control notification handler code here
}

void CDLGptz::OnFocusAdd() 
{

}

void CDLGptz::OnIrisAdd() 
{

}

void CDLGptz::OnZoomAdd() 
{

}

void CDLGptz::OnFocusSub() 
{

}

void CDLGptz::OnIrisSub() 
{

}

void CDLGptz::OnZoomSub() 
{

}

void CDLGptz::OnButtonSet() 
{

}

void CDLGptz::OnButtonGo() 
{
}

void CDLGptz::OnSpeedSub() 
{
	// TODO: Add your control notification handler code here
}

void CDLGptz::OnSpeedAdd() 
{
	// TODO: Add your control notification handler code here
}


HBRUSH CDLGptz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
