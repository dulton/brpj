// DLGAddDevice.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGAddDevice.h"
#include "YiRongCarDetectAIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGAddDevice dialog


CDLGAddDevice::CDLGAddDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGAddDevice::IDD, pParent)
	, m_CamRtspurl(_T(""))
{
	//{{AFX_DATA_INIT(CDLGAddDevice)
	m_CamIpAddr = _T("");
	m_CamName = _T("");
	m_CamPsw = _T("");
	m_CamUser = _T("");
	m_AddArea = _T("");
	m_CamArea = _T("");
	m_CamPort = 0;
	AreaCount = 0;
	AreaComboCur = 0;
	AddAreaFlag = false;
	VenderComboCur = 0;
	m_CamChannel = 0;
	m_CamRtspurl = _T("");
	RTPComboCur=0;
	DecodeTagComboCur=0;
	//}}AFX_DATA_INIT
}


void CDLGAddDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGAddDevice)
	DDX_Control(pDX, IDC_COMBO_CAMAREA, m_AreaComboCtrl);
	DDX_Control(pDX, IDC_COMBO_CAMVENDER, m_camVender);
	DDX_Text(pDX, IDC_EDIT_CAM_IPADDR, m_CamIpAddr);
	DDX_Text(pDX, IDC_EDIT_CAM_NAME, m_CamName);
	DDX_Text(pDX, IDC_EDIT_CAM_PSW, m_CamPsw);
	DDX_Text(pDX, IDC_EDIT_CAM_USER, m_CamUser);
	DDX_Text(pDX, IDC_EDIT_ADDAREA, m_AddArea);
	DDV_MaxChars(pDX, m_AddArea, 32);
	DDX_CBString(pDX, IDC_COMBO_CAMAREA, m_CamArea);
	DDX_Text(pDX, IDC_EDIT_CAM_PORT, m_CamPort);
	DDX_Text(pDX, IDC_EDIT_CAM_CHANNEL, m_CamChannel);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_RTSPURL, m_CamRtspurl);
	DDX_Control(pDX, IDC_COMBO_RTP, m_CamRTP);
	DDX_Control(pDX, IDC_COMBO_DECODETAG, m_CamDecodetag);
	DDX_Control(pDX, IDOK, m_b_ok);
	DDX_Control(pDX, IDCANCEL, m_b_cancel);
	DDX_Control(pDX, IDC_BUTTON_ADDAREA, m_b_addarea);
}


BEGIN_MESSAGE_MAP(CDLGAddDevice, CDialog)
	//{{AFX_MSG_MAP(CDLGAddDevice)
	ON_BN_CLICKED(IDC_BUTTON_ADDAREA, OnButtonAddarea)
	//}}AFX_MSG_MAP
	ON_CBN_CLOSEUP(IDC_COMBO_CAMVENDER, &CDLGAddDevice::OnCbnCloseupComboCamvender)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGAddDevice message handlers

BOOL CDLGAddDevice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox* comboctrl;
	for(int i=0;i<AreaCount;i++)
	{
		if(AreaList[i].name != NULL)
		{
			m_AreaComboCtrl.AddString(AreaList[i].name); 
		}
	}
		
	m_AreaComboCtrl.SetCurSel(AreaComboCur);

	comboctrl=(CComboBox*)GetDlgItem(IDC_COMBO_CAMVENDER);
	comboctrl->SetCurSel(VenderComboCur);

	comboctrl=(CComboBox*)GetDlgItem(IDC_COMBO_RTP);
	comboctrl->SetCurSel(RTPComboCur);

	comboctrl=(CComboBox*)GetDlgItem(IDC_COMBO_DECODETAG);
	comboctrl->SetCurSel(DecodeTagComboCur);

	OnCbnCloseupComboCamvender();

	m_b_ok.LoadBitmaps(IDB_OK_BUTTON,IDB_OK_BUTTON_MOVE,NULL,NULL);
	m_b_ok.SizeToContent();		//自适应图片大小

	m_b_cancel.LoadBitmaps(IDB_CANCEL_BUTTON,IDB_CANCEL_BUTTON_MOVE,NULL,NULL);
	m_b_cancel.SizeToContent();		//自适应图片大小

	m_b_addarea.LoadBitmaps(IDB_ADDAREA_BUTTON,IDB_ADDAREA_BUTTON_MOVE,NULL,NULL);
	m_b_addarea.SizeToContent();		//自适应图片大小

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGAddDevice::OnOK() 
{
	UpdateData(TRUE);
	// TODO: Add extra validation here
	AreaComboCur = m_AreaComboCtrl.GetCurSel();
	VenderComboCur = m_camVender.GetCurSel();
	RTPComboCur = m_CamRTP.GetCurSel();
	DecodeTagComboCur=m_CamDecodetag.GetCurSel();

	if(AddAreaFlag == false)
	{
		if(m_CamName.IsEmpty())
		{
			MessageBox("摄像机名称不能为空",MESSAGEBOX_TITLE);
			return;
		}
		if((VenderComboCur != VENDER_TYPE_STREAM) && m_CamIpAddr.IsEmpty())
		{
			MessageBox("IP地址不能为空",MESSAGEBOX_TITLE);
			return;
		}
		if((VenderComboCur == VENDER_TYPE_STREAM) && m_CamRtspurl.IsEmpty())
		{
			MessageBox("流媒体地址不能为空",MESSAGEBOX_TITLE);
			return;
		}
		if(m_CamUser.IsEmpty())
		{
			MessageBox("用户名不能为空",MESSAGEBOX_TITLE);
			return;
		}
		if(m_CamPsw.IsEmpty())
		{
			MessageBox("密码不能为空",MESSAGEBOX_TITLE);
			return;
		}

#if	(!OPEN_YAAN_NEW_SDK) 
		if(VenderComboCur ==VENDER_TYPE_YAAN_NEW )
		{
			MessageBox("版本不支持 亚安新版 摄像头 ",MESSAGEBOX_TITLE);
			return;
		}
#endif
#if	(!OPEN_YAAN_SDK) 
		if(VenderComboCur ==VENDER_TYPE_YAAN )
		{
			MessageBox("版本不支持 亚安 摄像头",MESSAGEBOX_TITLE);
			return;
		}
#endif
#if	(!OPEN_DAHUA_SDK) 
		if(VenderComboCur ==VENDER_TYPE_DAHUA )
		{
			MessageBox("版本不支持 大华 摄像头",MESSAGEBOX_TITLE);
			return;
		}
#endif
#if	(!OPEN_HAIKANG_SDK) 
		if(VenderComboCur ==VENDER_TYPE_HAIKANG )
		{
			MessageBox("版本不支持 海康 摄像头",MESSAGEBOX_TITLE);
			return;
		}
#endif

	
#if	(!OPEN_STREAM_CLIENT_SDK) 
		if(VenderComboCur ==VENDER_TYPE_STREAM )
		{
			MessageBox("版本不支持 流媒体",MESSAGEBOX_TITLE);
			return;
		}
#endif



	}
	CDialog::OnOK();
}

void CDLGAddDevice::OnButtonAddarea() 
{
	// TODO: Add your control notification handler code here
	if(AreaCount == MAX_AREA)
	{
		MessageBox("超过最大的区域限制",MESSAGEBOX_TITLE);
	}
	else
	{
		this->UpdateData(TRUE);
		if(m_AddArea.IsEmpty())
		{
			MessageBox("区域名称不能为空",MESSAGEBOX_TITLE);
		}
		else
		{
			this->GetDlgItem(IDC_COMBO_CAMAREA)->EnableWindow(0);
			this->GetDlgItem(IDC_EDIT_CAM_NAME)->EnableWindow(0);
			this->GetDlgItem(IDC_EDIT_CAM_IPADDR)->EnableWindow(0);
			this->GetDlgItem(IDC_EDIT_CAM_PORT)->EnableWindow(0);
			this->GetDlgItem(IDC_EDIT_CAM_CHANNEL)->EnableWindow(0);
			this->GetDlgItem(IDC_EDIT_CAM_USER)->EnableWindow(0);
			this->GetDlgItem(IDC_EDIT_CAM_PSW)->EnableWindow(0);
			this->GetDlgItem(IDC_COMBO_CAMVENDER)->EnableWindow(0);
			this->GetDlgItem(IDC_COMBO_RTP)->EnableWindow(0);
			this->GetDlgItem(IDC_EDIT_RTSPURL)->EnableWindow(0);
			this->GetDlgItem(IDC_COMBO_DECODETAG)->EnableWindow(0);
		
			AddAreaFlag = true;
			OnOK();
		}
	}
}


void CDLGAddDevice::OnCbnCloseupComboCamvender()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	
	if( m_camVender.GetCurSel() ==VENDER_TYPE_STREAM)
	{
		GetDlgItem(IDC_COMBO_RTP)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_RTSPURL)->EnableWindow(1);
		GetDlgItem(IDC_COMBO_DECODETAG)->EnableWindow(1);

		GetDlgItem(IDC_EDIT_CAM_IPADDR)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_CAM_PORT)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_CAM_CHANNEL)->EnableWindow(0);
	}
	else
	{
		GetDlgItem(IDC_COMBO_RTP)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_RTSPURL)->EnableWindow(0);
		GetDlgItem(IDC_COMBO_DECODETAG)->EnableWindow(0);

		GetDlgItem(IDC_EDIT_CAM_IPADDR)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_CAM_PORT)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_CAM_CHANNEL)->EnableWindow(1);
	}

}

void CDLGAddDevice::OnPaint()
{

	CPaintDC dc(this); // device context for painting
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
HBRUSH CDLGAddDevice::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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