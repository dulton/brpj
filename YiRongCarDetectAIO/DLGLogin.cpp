// DLGLogin.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGLogin.h"

#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

extern IO OracleIO;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGLogin dialog


CDLGLogin::CDLGLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGLogin)
	m_user = _T("");
	m_password = _T("");
	m_newpassword = _T("");
	m_newpsw_again = _T("");
	//}}AFX_DATA_INIT
	flag=LOGIN_IN;
	memset(CurrentUser.user,0,32);

}


void CDLGLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGLogin)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_EDIT_USER, m_user);
	DDV_MaxChars(pDX, m_user, 32);
	DDX_Text(pDX, IDC_EDIT_PSW, m_password);
	DDV_MaxChars(pDX, m_password, 32);
	DDX_Text(pDX, IDC_EDIT_NEWPSW, m_newpassword);
	DDV_MaxChars(pDX, m_newpassword, 32);
	DDX_Text(pDX, IDC_EDIT_NEWPSW_AGAIN, m_newpsw_again);
	DDV_MaxChars(pDX, m_newpsw_again, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGLogin, CDialog)
	//{{AFX_MSG_MAP(CDLGLogin)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGLogin message handlers
BOOL CDLGLogin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	//设按钮图片
	m_ok.LoadBitmaps(IDB_LOGIN_OKOUT,IDB_LOGIN_OKIN,NULL,NULL);
	m_ok.SizeToContent();		//自适应图片大小

	m_cancel.LoadBitmaps(IDB_LOGIN_CANCELOUT,IDB_LOGIN_CANCELIN,NULL,NULL);
	m_cancel.SizeToContent();		//自适应图片大小

	//设标题
	SetWindowText("用户");

	//密码永远要重新输入
	m_user =CurrentUser.user;
	m_password = _T("");
	m_newpassword = _T("");
	m_newpsw_again = _T("");

	switch(flag)
	{
	case LOGIN_IN:
	case LOGIN_LOCK:
	case LOGIN_EXIT:
		GetDlgItem(IDC_EDIT_NEWPSW)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_NEWPSW_AGAIN)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_NEWPSW)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_NEWPSW_AGAIN)->ShowWindow(FALSE);
		break;
	case LOGIN_MODIFY:
		GetDlgItem(IDC_EDIT_NEWPSW)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_NEWPSW_AGAIN)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_NEWPSW)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_NEWPSW_AGAIN)->ShowWindow(TRUE);
		break;
	default:
		MessageBox("BOOL CDLGLogin::OnInitDialog error",MESSAGEBOX_TITLE);
		break;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGLogin::OnOK() 
{
	CString tempstr;
	// TODO: Add extra validation here
	UpdateData(TRUE);
	//登陆操作
	switch(flag)
	{
	case LOGIN_IN:	
		if(m_user.IsEmpty() ||
			m_password.IsEmpty()	)
		{
			MessageBox("用户名和密码不为空",MESSAGEBOX_TITLE);
			return ;
		}

		tempstr=CurrentUser.user;
		//查询数据库
		if(!OracleIO.USER_ReadUserInfoWithName(m_user.GetBuffer(0),&CurrentUser))
		{
			MessageBox("无此用户",MESSAGEBOX_TITLE);
			return ;
		}
		//解密
		ZogDeCode(CurrentUser.psw,CurrentPsw);
		if(0!=strcmp(m_password.GetBuffer(0),CurrentPsw))
		{
			MessageBox("密码错误",MESSAGEBOX_TITLE);
			//还原
			OracleIO.USER_ReadUserInfoWithName(tempstr.GetBuffer(0),&CurrentUser);
			return ;
		}
		DlgMain->NewLogMessage("登陆");
		break;
	case LOGIN_LOCK:
	case LOGIN_EXIT:
		if(m_user.IsEmpty() ||
			m_password.IsEmpty())
		{
			MessageBox("用户名和密码不为空",MESSAGEBOX_TITLE);
			return ;
		}
		if(0!=strcmp(m_user.GetBuffer(0),CurrentUser.user))
		{
			MessageBox("并非原来的用户",MESSAGEBOX_TITLE);
			return ;
		}
		if(0!=strcmp(m_password.GetBuffer(0),CurrentPsw))
		{
			MessageBox("密码错误",MESSAGEBOX_TITLE);
			return ;
		}
		DlgMain->NewLogMessage("退出");
		break;
	case LOGIN_MODIFY:
		if(m_user.IsEmpty() ||
			m_password.IsEmpty() ||
			m_newpassword.IsEmpty() ||
			m_newpsw_again.IsEmpty() )
		{
			MessageBox("用户名和密码和新密码不为空",MESSAGEBOX_TITLE);
			return ;
		}
		if(0!=strcmp(m_user.GetBuffer(0),CurrentUser.user))
		{
			MessageBox("并非原来的用户",MESSAGEBOX_TITLE);
			return ;
		}
		if(0!=strcmp(m_password.GetBuffer(0),CurrentPsw))
		{
			MessageBox("原密码错误",MESSAGEBOX_TITLE);
			return ;
		}
		if(	m_newpassword!=	m_newpsw_again)
		{
			MessageBox("两次新密码不一致",MESSAGEBOX_TITLE);
			return ;
		}

		ZogEnCode(m_newpassword.GetBuffer(0),CurrentUser.psw);
		if(!OracleIO.USER_UpdateUserInfo(CurrentUser))
		{
			MessageBox("修改密码 数据库错误 未成功",MESSAGEBOX_TITLE);
			return ;
		}

		//修改密码
		strcpy(CurrentPsw,m_newpassword.GetBuffer(0));

		DlgMain->NewLogMessage("修改密码");
		break;
	default:
		MessageBox("BOOL CDLGLogin::OnOK error",MESSAGEBOX_TITLE);
		break;
	}
	//成功
	//如果是LOCK和EXIT模式。必须原用户登陆才可以正确退出

	//必须成功才执行下一句
	CDialog::OnOK();
}

void CDLGLogin::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDLGLogin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
		//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     
	
	//从资源中载入位图     
	CBitmap    bitmap;     

	if(LOGIN_IN == flag)
		bitmap.LoadBitmap(IDB_LOGIN_IN);    
	else 	if(LOGIN_LOCK == flag)
		bitmap.LoadBitmap(IDB_LOGIN_LOCK);    
	else 	if(LOGIN_EXIT == flag)
		bitmap.LoadBitmap(IDB_LOGIN_EXIT);    
	else	if(LOGIN_MODIFY == flag)
		bitmap.LoadBitmap(IDB_LOGIN_MODIFY);  
	else	
		MessageBox("CDLGLogin::OnPaint error",MESSAGEBOX_TITLE);

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

HBRUSH CDLGLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
