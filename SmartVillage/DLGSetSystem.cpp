// DLGSetSystem.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "DLGSetSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DLGWarnning.h"
/////////////////////////////////////////////////////////////////////////////
// CDLGSetSystem dialog


CDLGSetSystem::CDLGSetSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSetSystem::IDD, pParent)
	, m_display_preview(TRUE)

	, m_path_faceblack(_T(""))
{
	//{{AFX_DATA_INIT(CDLGSetSystem)
	m_check_alarmpic = TRUE;
	m_check_alarmwav = FALSE;
	m_check_ftp = FALSE;
	m_display_preview=TRUE;
	m_recordfull = 1;
	m_path_alarmwav = _T("bj.wav");
	m_path_capbmp = _T("D:\\YRCapturePic");
	m_path_Detect = _T("D:\\YRDetectResult");
	m_path_record = _T("D:\\YRRecord");
	m_record_cuttime = 20;
	m_ftp_ip = _T("");
	m_ftp_port = _T("");
	m_ftp_psw = _T("");
	m_ftp_user = _T("");

	m_path_dahua = _T("大华播放器\\player264demo.exe");
	m_path_haikang = _T("海康播放器\\VSPlayer.exe");

	m_ftp_path = _T("/");
	m_tomcat_dir = _T("D:\\tomcatname\\webapps\\YRCapturePic");
	m_tomcat_url = _T("http://10.142.50.126:8089/YRCapturePic");

	m_path_faceblack = _T("D:\\YRFaceBlack");
	//}}AFX_DATA_INIT

	m_BgBrush.CreateSolidBrush(RGB(216,216,216));         // 背景的颜色
}


void CDLGSetSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetSystem)
	DDX_Check(pDX, IDC_CHECK_ALARM_PIC, m_check_alarmpic);
	DDX_Check(pDX, IDC_CHECK_ALARM_WAV, m_check_alarmwav);
	//DDX_Check(pDX, IDC_CHECK_FTP, m_check_ftp);
//	DDX_CBIndex(pDX, IDC_COMBO_RECORDFULL, m_recordfull);
	DDX_Text(pDX, IDC_EDIT_ALARM_WAV, m_path_alarmwav);
	DDV_MaxChars(pDX, m_path_alarmwav, 260);
	DDX_Text(pDX, IDC_EDIT_CAPBMP_DIR, m_path_capbmp);
	DDV_MaxChars(pDX, m_path_capbmp, 260);
	DDX_Text(pDX, IDC_EDIT_CARDETECT_DIR, m_path_Detect);
	DDV_MaxChars(pDX, m_path_Detect, 260);
//	DDX_Text(pDX, IDC_EDIT_RECORD_DIR, m_path_record);
//	DDV_MaxChars(pDX, m_path_record, 260);
//	DDX_Text(pDX, IDC_EDIT_TIME, m_record_cuttime);
//	DDV_MinMaxInt(pDX, m_record_cuttime, 1, 60);
//	DDX_Text(pDX, IDC_EDIT_FTP_IPADDR, m_ftp_ip);
//	DDV_MaxChars(pDX, m_ftp_ip, 32);
//	DDX_Text(pDX, IDC_EDIT_FTP_PORT, m_ftp_port);
//	DDV_MaxChars(pDX, m_ftp_port, 32);
//	DDX_Text(pDX, IDC_EDIT_FTP_PSW, m_ftp_psw);
//	DDV_MaxChars(pDX, m_ftp_psw, 32);
//	DDX_Text(pDX, IDC_EDIT_FTP_USER, m_ftp_user);
//	DDV_MaxChars(pDX, m_ftp_user, 32);

//	DDX_Text(pDX, IDC_EDIT_DAHUA_PATH, m_path_dahua);
//	DDV_MaxChars(pDX, m_path_dahua, 260);
//	DDX_Text(pDX, IDC_EDIT_HAIKANG_PATH, m_path_haikang);
//	DDV_MaxChars(pDX, m_path_haikang, 260);

//	DDX_Text(pDX, IDC_EDIT_FTP_PATH, m_ftp_path);
//	DDV_MaxChars(pDX, m_ftp_path, 260);
//	DDX_Text(pDX, IDC_EDIT_TOMCAT_DIR, m_tomcat_dir);
//	DDX_Text(pDX, IDC_EDIT_TOMCAT_URL, m_tomcat_url);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_DISPLAY_PREVIEW, m_display_preview);


	DDX_Control(pDX, IDOK, m_b_ok);
	DDX_Control(pDX, IDCANCEL, m_b_cancel);
	DDX_Control(pDX, IDC_BUTTON_ALARM_WAV, m_b_alarmwav);
	DDX_Control(pDX, IDC_BUTTON_CARDETECT_DIR, m_b_CarDetectdir);
//	DDX_Control(pDX, IDC_BUTTON_RECORD_DIR, m_b_recorddir);
	DDX_Control(pDX, IDC_BUTTON_CAPBMP_DIR, m_b_cambmpdir);
//	DDX_Control(pDX, IDC_BUTTON_HAIKANG_PATH, m_b_haikangpath);
//	DDX_Control(pDX, IDC_BUTTON_DAHUA_PATH, m_b_dahuapath);

//	DDX_Control(pDX, IDC_BUTTON_TOMCAT_DIR, m_b_tomcatdir);

	DDX_Text(pDX, IDC_EDIT_BLACKFACE_DIR, m_path_faceblack);
	DDV_MaxChars(pDX, m_path_faceblack, 260);
	DDX_Control(pDX, IDC_BUTTON_BLACKFACE_DIR, m_b_faceblack);
}


BEGIN_MESSAGE_MAP(CDLGSetSystem, CDialog)
	//{{AFX_MSG_MAP(CDLGSetSystem)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_WAV, OnButtonAlarmWav)
	ON_BN_CLICKED(IDC_BUTTON_CARDETECT_DIR, OnButtonCarDetectDir)
	//ON_BN_CLICKED(IDC_BUTTON_RECORD_DIR, OnButtonRecordDir)
	ON_BN_CLICKED(IDC_BUTTON_CAPBMP_DIR, OnButtonCapbmpDir)
	//ON_BN_CLICKED(IDC_BUTTON_HAIKANG_PATH, OnButtonHaikangPath)
//	ON_BN_CLICKED(IDC_BUTTON_DAHUA_PATH, OnButtonDahuaPath)

//	ON_BN_CLICKED(IDC_BUTTON_TOMCAT_DIR, OnButtonTomcatDir)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_BLACKFACE_DIR, &CDLGSetSystem::OnBnClickedButtonBlackfaceDir)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSetSystem message handlers
BOOL CDLGSetSystem::OnInitDialog()
{
	CDialog::OnInitDialog();

	Read2Dlg();
	UpdateData(FALSE);







	m_b_ok.LoadBitmaps(IDB_OK_BUTTON,IDB_OK_BUTTON_MOVE,NULL,NULL);
	m_b_ok.SizeToContent();		//自适应图片大小

	m_b_cancel.LoadBitmaps(IDB_CANCEL_BUTTON,IDB_CANCEL_BUTTON_MOVE,NULL,NULL);
	m_b_cancel.SizeToContent();		//自适应图片大小

	m_b_alarmwav.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
	m_b_alarmwav.SizeToContent();		//自适应图片大小

	m_b_CarDetectdir.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
	m_b_CarDetectdir.SizeToContent();		//自适应图片大小

//	m_b_recorddir.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
//	m_b_recorddir.SizeToContent();		//自适应图片大小

	m_b_cambmpdir.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
	m_b_cambmpdir.SizeToContent();		//自适应图片大小

//	m_b_haikangpath.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
//	m_b_haikangpath.SizeToContent();		//自适应图片大小

//	m_b_dahuapath.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
//	m_b_dahuapath.SizeToContent();		//自适应图片大小



//	m_b_tomcatdir.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
//	m_b_tomcatdir.SizeToContent();		//自适应图片大小


	m_b_faceblack.LoadBitmaps(IDB_VIEW_BUTTON,IDB_VIEW_BUTTON_MOVE,NULL,NULL);
	m_b_faceblack.SizeToContent();		//自适应图片大小

	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGSetSystem::OnButtonAlarmWav() 
{
	// TODO: Add your control notification handler code here
	char szFilter[]="wav Files (*.wav)|*.wav||";
	CFileDialog dlg(TRUE,"wav","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		//文件路径
		m_path_alarmwav=dlg.GetPathName();
		UpdateData(FALSE);
		CRect t_Rect;
		GetDlgItem(IDC_EDIT_ALARM_WAV)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}

void CDLGSetSystem::OnButtonCarDetectDir() 
{
	// TODO: Add your control notification handler code here
	//保存目录
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体
	TCHAR   Buffer[512]= " ";
	TCHAR   FullPath[512]= " ";
	bi.hwndOwner   =   GetSafeHwnd();               //窗口句柄
	bi.pidlRoot   =   NULL;
	bi.pszDisplayName   =   Buffer;            //返回选择的目录名的缓冲区
	bi.lpszTitle   =   "Selection ";           //弹出的窗口的文字提示
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;  //只返回目录。其他标志看MSDN
	bi.lpfn   =   NULL;               //回调函数，有时很有用
	bi.lParam   =   0;
	bi.iImage   =   0;
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要
	if(::SHGetPathFromIDList(pidl,FullPath)) //在ITEMIDLIST中得到目录名的整个路径
	{
		m_path_Detect=FullPath;
		UpdateData(FALSE);
		CRect t_Rect;
		GetDlgItem(IDC_EDIT_CARDETECT_DIR)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}

void CDLGSetSystem::OnButtonRecordDir() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体
	TCHAR   Buffer[512]= " ";
	TCHAR   FullPath[512]= " ";
	bi.hwndOwner   =   GetSafeHwnd();               //窗口句柄
	bi.pidlRoot   =   NULL;
	bi.pszDisplayName   =   Buffer;            //返回选择的目录名的缓冲区
	bi.lpszTitle   =   "Selection ";           //弹出的窗口的文字提示
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;  //只返回目录。其他标志看MSDN
	bi.lpfn   =   NULL;               //回调函数，有时很有用
	bi.lParam   =   0;
	bi.iImage   =   0;
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要
	if(::SHGetPathFromIDList(pidl,FullPath)) //在ITEMIDLIST中得到目录名的整个路径
	{
		m_path_record=FullPath;
		UpdateData(FALSE);
		CRect t_Rect;
		GetDlgItem(IDC_EDIT_RECORD_DIR)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}

void CDLGSetSystem::OnButtonCapbmpDir() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体
	TCHAR   Buffer[512]= " ";
	TCHAR   FullPath[512]= " ";
	bi.hwndOwner   =   GetSafeHwnd();               //窗口句柄
	bi.pidlRoot   =   NULL;
	bi.pszDisplayName   =   Buffer;            //返回选择的目录名的缓冲区
	bi.lpszTitle   =   "Selection ";           //弹出的窗口的文字提示
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;  //只返回目录。其他标志看MSDN
	bi.lpfn   =   NULL;               //回调函数，有时很有用
	bi.lParam   =   0;
	bi.iImage   =   0;
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要
	if(::SHGetPathFromIDList(pidl,FullPath)) //在ITEMIDLIST中得到目录名的整个路径
	{
		m_path_capbmp=FullPath;
		UpdateData(FALSE);
		CRect t_Rect;
		GetDlgItem(IDC_EDIT_CAPBMP_DIR)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}

void CDLGSetSystem::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	if(m_record_cuttime < 1 || m_record_cuttime >60)
		return ;

	Dlg2Write();

	//创建文件夹
	CreateDirectory(m_path_capbmp, NULL);
	CreateDirectory(m_path_Detect, NULL);

	path_CarDetect.Format("%s\\Car",m_path_Detect.GetBuffer(0));
	CreateDirectory(path_CarDetect, NULL);

	path_CarDetect_Small.Format("%s\\CarSmall",m_path_Detect.GetBuffer(0));
	CreateDirectory(path_CarDetect_Small, NULL);

	path_FaceDetect.Format("%s\\Face",m_path_Detect.GetBuffer(0));
	CreateDirectory(path_FaceDetect, NULL);

	path_FaceDetect_Small.Format("%s\\FaceSmall",m_path_Detect.GetBuffer(0));
	CreateDirectory(path_FaceDetect_Small, NULL);

	CreateDirectory(m_path_record, NULL);

	CreateDirectory(m_path_faceblack, NULL);
	

	CDialog::OnOK();
}

void CDLGSetSystem::OnCancel() 
{
	// TODO: Add extra cleanup here
	//重读
	Read2Dlg();
	UpdateData(FALSE);
	CDialog::OnCancel();
}

void CDLGSetSystem::Read2Dlg() 
{
	//测试文件是否存在
	FILE *fp=NULL;
	fp=fopen(SystemIniPath,"r");
	if(fp)
		fclose(fp);
	else
	{
		//不存在则写入后退出
		Dlg2Write();
		CreateDirectory(m_path_capbmp, NULL);
		CreateDirectory(m_path_Detect, NULL);

		path_CarDetect.Format("%s\\Car",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_CarDetect, NULL);

		path_CarDetect_Small.Format("%s\\CarSmall",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_CarDetect_Small, NULL);

		path_FaceDetect.Format("%s\\Face",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_FaceDetect, NULL);

		path_FaceDetect_Small.Format("%s\\FaceSmall",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_FaceDetect_Small, NULL);


		//CreateDirectory(m_path_record, NULL);
		CreateDirectory(m_path_faceblack, NULL);

		return;
	}

	readini(SystemIniPath);
}

void CDLGSetSystem::Dlg2Write() 
{
	writeini(SystemIniPath);
}
//导入
void CDLGSetSystem::InputIni() 
{
	CDLGWarnning dlgw;
	dlgw.m_wintxt="系统设置导入警告";
	dlgw.m_warntxt="导入设置文件将使原设置被覆盖";
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{
		char szFilter[]="systeminfo Files (*.ini)|*.ini|*.*|*.*||";
		CFileDialog dlg(TRUE,"ini","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			szFilter);
		if(dlg.DoModal()==IDOK)
		{
			//覆盖文件
			CopyFile(dlg.GetPathName(),SystemIniPath,FALSE);
		}
	}
	else if (nResponse == IDCANCEL)
		return ;
	else
		return ;

}
//导出
void CDLGSetSystem::OutputIni() 
{
	//保存文件
	char szFilter[]="systeminfo Files (*.ini)|*.ini|*.*|*.*||";
	CFileDialog dlg(FALSE,"ini","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		//覆盖文件
		CopyFile(SystemIniPath,dlg.GetPathName(),FALSE);
	}
}

//读INI
void CDLGSetSystem::readini(char *path) 
{
	int	check_alarmpic;
	int	check_alarmwav;
	int	check_ftp;

	int		recordfull;
	char	path_alarmwav[ZOG_MAX_PATH_STR]="";
	char	path_capbmp[ZOG_MAX_PATH_STR]="";
	char	path_Detect[ZOG_MAX_PATH_STR]="";
	char	path_record[ZOG_MAX_PATH_STR]="";
	int		record_cuttime;
	char	ftp_ip[ZOG_MAX_NAME_STR]="";
	char	ftp_port[ZOG_MAX_NAME_STR]="";
	char	ftp_user[ZOG_MAX_NAME_STR]="";
	char	ftp_psw[ZOG_MAX_NAME_STR]="";
	char	ftp_psw_ext[(ZOG_MAX_NAME_STR+1)*5]="";
	char	ftp_path[ZOG_MAX_NAME_STR]="";

	char	path_dahua[ZOG_MAX_PATH_STR]="";
	char	path_haikang[ZOG_MAX_PATH_STR]="";
	char	path_yaan[ZOG_MAX_PATH_STR]="";

	char	tomcat_dir[ZOG_MAX_PATH_STR] = "";
	char	tomcat_url[ZOG_MAX_PATH_STR] = ""; 
	char	update_url[ZOG_MAX_PATH_STR] = ""; 

	char	kakou_url[ZOG_MAX_PATH_STR] = "";
	char	kakou_ip[ZOG_MAX_PATH_STR] = ""; 
	char	kakou_user[ZOG_MAX_PATH_STR] = ""; 
	char	kakou_psw[ZOG_MAX_PATH_STR] = ""; 
	char	kakou_psw_ext[ZOG_MAX_PATH_STR] = ""; 
	char	myip[ZOG_MAX_PATH_STR] = ""; 

	char path_faceblack[ZOG_MAX_PATH_STR]="";

	int check_displaypreview;

	////////////////////////////////////
	if(GetPrivateProfileStruct("Alarm", "CheckPic", &check_alarmpic, sizeof(int), path))
		m_check_alarmpic=check_alarmpic;
	if(GetPrivateProfileStruct("Alarm", "CheckWav", &check_alarmwav, sizeof(int), path))
		m_check_alarmwav=check_alarmwav;
	if(GetPrivateProfileString("Alarm", "PathAlarmWav", "", path_alarmwav, ZOG_MAX_PATH_STR, path))
		m_path_alarmwav=path_alarmwav;
	if(GetPrivateProfileString("Alarm", "PathCarDetect", "", path_Detect, ZOG_MAX_PATH_STR, path))
		m_path_Detect=path_Detect;
	////////////////////////////////////
	if(GetPrivateProfileStruct("Record", "CutTime", &record_cuttime, sizeof(int), path))
		m_record_cuttime=record_cuttime;
	if(GetPrivateProfileStruct("Record", "RecordFull", &recordfull, sizeof(int), path))
		m_recordfull=recordfull;
	if(GetPrivateProfileString("Record", "PathRecord", "", path_record, ZOG_MAX_PATH_STR, path))
		m_path_record=path_record;
	if(GetPrivateProfileString("Record", "PathCapbmp", "", path_capbmp, ZOG_MAX_PATH_STR, path))
		m_path_capbmp=path_capbmp;
	////////////////////////////////////
	if(GetPrivateProfileString("Play", "PathDahua", "", path_dahua, ZOG_MAX_PATH_STR, path))
		m_path_dahua=path_dahua;
	if(GetPrivateProfileString("Play", "PathHaikang", "", path_haikang, ZOG_MAX_PATH_STR, path))
		m_path_haikang=path_haikang;

	////////////////////////////////////
	if(GetPrivateProfileStruct("FTP", "check", &check_ftp, sizeof(int), path))
		m_check_ftp=check_ftp;
	if(GetPrivateProfileString("FTP", "ip", "", ftp_ip, ZOG_MAX_NAME_STR, path))
		m_ftp_ip=ftp_ip;
	if(GetPrivateProfileString("FTP", "port", "", ftp_port, ZOG_MAX_NAME_STR, path))
		m_ftp_port=ftp_port;
	if(GetPrivateProfileString("FTP", "user", "", ftp_user, ZOG_MAX_NAME_STR, path))
		m_ftp_user=ftp_user;
	if(GetPrivateProfileString("FTP", "password", "", ftp_psw_ext, (ZOG_MAX_NAME_STR+1)*5, path))
	{
		if(0!=strlen(ftp_psw_ext))
		{
			ZogDeCode(ftp_psw_ext,ftp_psw);
			m_ftp_psw=ftp_psw;
		}
	}
	if(GetPrivateProfileString("FTP", "path", "", ftp_path, ZOG_MAX_PATH_STR, path))
		m_ftp_path=ftp_path;

	if(GetPrivateProfileString("TomCat", "Dir", "", tomcat_dir, ZOG_MAX_PATH_STR, path))
		m_tomcat_dir=tomcat_dir;
	if(GetPrivateProfileString("TomCat", "Url", "", tomcat_url, ZOG_MAX_PATH_STR, path))
		m_tomcat_url=tomcat_url;


	if(GetPrivateProfileStruct("View", "CheckDisplayPreview", &check_displaypreview, sizeof(int), path))
		m_display_preview=check_displaypreview;
	

	if(GetPrivateProfileString("Black", "PathFaceBlack", "", path_faceblack, ZOG_MAX_PATH_STR, path))
		m_path_faceblack=path_faceblack;

	CreateDirectory(m_path_capbmp, NULL);
	CreateDirectory(m_path_Detect, NULL);
//	CreateDirectory(m_path_record, NULL);
	CreateDirectory(m_path_faceblack, NULL);

		path_CarDetect.Format("%s\\Car",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_CarDetect, NULL);

		path_CarDetect_Small.Format("%s\\CarSmall",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_CarDetect_Small, NULL);

		path_FaceDetect.Format("%s\\Face",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_FaceDetect, NULL);

		path_FaceDetect_Small.Format("%s\\FaceSmall",m_path_Detect.GetBuffer(0));
		CreateDirectory(path_FaceDetect_Small, NULL);


}
//写ini
void CDLGSetSystem::writeini(char *path)
{
	////////////////////////////////////
	WritePrivateProfileStruct("Alarm", "CheckPic", &m_check_alarmpic, sizeof(int), path);
	WritePrivateProfileStruct("Alarm", "CheckWav", &m_check_alarmwav, sizeof(int), path);

	
	WritePrivateProfileString("Alarm", "PathAlarmWav", m_path_alarmwav.GetBuffer(0),path);
	WritePrivateProfileString("Alarm", "PathCarDetect", m_path_Detect.GetBuffer(0),path);
	////////////////////////////////////
	WritePrivateProfileStruct("Record", "CutTime", &m_record_cuttime, sizeof(int), path);
	WritePrivateProfileStruct("Record", "RecordFull", &m_recordfull, sizeof(int), path);
	WritePrivateProfileString("Record", "PathRecord",  m_path_record.GetBuffer(0),  path);
	WritePrivateProfileString("Record", "PathCapbmp",  m_path_capbmp.GetBuffer(0), path);
	////////////////////////////////////
	WritePrivateProfileString("Play", "PathDahua",  m_path_dahua.GetBuffer(0),  path);
	WritePrivateProfileString("Play", "PathHaikang",  m_path_haikang.GetBuffer(0), path);

	////////////////////////////////////
	WritePrivateProfileStruct("FTP", "check", &m_check_ftp, sizeof(int), path);
	WritePrivateProfileString("FTP", "ip", m_ftp_ip.GetBuffer(0), path);
	WritePrivateProfileString("FTP", "port", m_ftp_port.GetBuffer(0), path);
	WritePrivateProfileString("FTP", "user",  m_ftp_user.GetBuffer(0), path);

	char	ftp_psw_ext[(ZOG_MAX_NAME_STR+1)*5]="";
	ZogEnCode(m_ftp_psw.GetBuffer(0),ftp_psw_ext);
	WritePrivateProfileString("FTP", "password",  ftp_psw_ext, path);

	WritePrivateProfileString("FTP", "path",  m_ftp_path.GetBuffer(0), path);

	WritePrivateProfileString("TomCat", "Dir",  m_tomcat_dir.GetBuffer(0), path);
	WritePrivateProfileString("TomCat", "Url",  m_tomcat_url.GetBuffer(0), path);
	WritePrivateProfileStruct("View", "CheckDisplayPreview", &m_display_preview, sizeof(int), path);

	WritePrivateProfileString("Black", "PathFaceBlack", m_path_faceblack.GetBuffer(0),path);

}

void CDLGSetSystem::OnButtonHaikangPath() 
{
	// TODO: Add your control notification handler code here
	char szFilter[]="HaiKang Play Executable Files (*.exe)|*.exe|*.*|*.*||";
	CFileDialog dlg(TRUE,"exe","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_path_haikang=dlg.GetPathName();
		UpdateData(FALSE);
		CRect t_Rect;
	//	GetDlgItem(IDC_EDIT_HAIKANG_PATH)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}

void CDLGSetSystem::OnButtonDahuaPath() 
{
	// TODO: Add your control notification handler code here
	char szFilter[]="DaHua Play Executable Files (*.exe)|*.exe|*.*|*.*||";
	CFileDialog dlg(TRUE,"exe","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_path_dahua=dlg.GetPathName();
		UpdateData(FALSE);
		CRect t_Rect;
	//	GetDlgItem(IDC_EDIT_DAHUA_PATH)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}



void CDLGSetSystem::OnButtonTomcatDir() 
{
	// TODO: Add your control notification handler code here
	//保存目录
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体
	TCHAR   Buffer[512]= " ";
	TCHAR   FullPath[512]= " ";
	bi.hwndOwner   =   GetSafeHwnd();               //窗口句柄
	bi.pidlRoot   =   NULL;
	bi.pszDisplayName   =   Buffer;            //返回选择的目录名的缓冲区
	bi.lpszTitle   =   "Selection ";           //弹出的窗口的文字提示
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;  //只返回目录。其他标志看MSDN
	bi.lpfn   =   NULL;               //回调函数，有时很有用
	bi.lParam   =   0;
	bi.iImage   =   0;
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要
	if(::SHGetPathFromIDList(pidl,FullPath)) //在ITEMIDLIST中得到目录名的整个路径
	{
		m_tomcat_dir=FullPath;
		UpdateData(FALSE);
		CRect t_Rect;
		//GetDlgItem(IDC_EDIT_TOMCAT_DIR)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}



void CDLGSetSystem::OnPaint()
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
HBRUSH CDLGSetSystem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	//组透明
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ALARM)
	{
		// 背景色透明
		pDC->SetBkMode(TRANSPARENT);

		// 返回背景色的画刷
		return m_BgBrush;
	}
	

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
void CDLGSetSystem::OnBnClickedButtonBlackfaceDir()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//保存目录
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体
	TCHAR   Buffer[512]= " ";
	TCHAR   FullPath[512]= " ";
	bi.hwndOwner   =   GetSafeHwnd();               //窗口句柄
	bi.pidlRoot   =   NULL;
	bi.pszDisplayName   =   Buffer;            //返回选择的目录名的缓冲区
	bi.lpszTitle   =   "Selection ";           //弹出的窗口的文字提示
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS   ;  //只返回目录。其他标志看MSDN
	bi.lpfn   =   NULL;               //回调函数，有时很有用
	bi.lParam   =   0;
	bi.iImage   =   0;
	ITEMIDLIST*   pidl   =   ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要
	if(::SHGetPathFromIDList(pidl,FullPath)) //在ITEMIDLIST中得到目录名的整个路径
	{
		m_path_faceblack=FullPath;
		UpdateData(FALSE);
		CRect t_Rect;
		GetDlgItem(IDC_EDIT_BLACKFACE_DIR)->GetWindowRect(t_Rect);
		ScreenToClient(t_Rect);
		InvalidateRect(t_Rect,TRUE);
	}
}
