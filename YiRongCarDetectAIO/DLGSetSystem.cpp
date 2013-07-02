// DLGSetSystem.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
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
{
	//{{AFX_DATA_INIT(CDLGSetSystem)
	m_check_alarmpic = TRUE;
	m_check_alarmwav = FALSE;
	m_check_ftp = FALSE;
	m_recordfull = 1;
	m_path_alarmwav = _T("bj.wav");
	m_path_capbmp = _T("D:\\YRCapturePic");
	m_path_detect = _T("D:\\YRCarDetectResult");
	m_path_record = _T("D:\\YRRecord");
	m_record_cuttime = 20;
	m_ftp_ip = _T("");
	m_ftp_port = _T("");
	m_ftp_psw = _T("");
	m_ftp_user = _T("");
	m_check_savenet = FALSE;
	m_path_dahua = _T("大华播放器\\player264demo.exe");
	m_path_haikang = _T("海康播放器\\VSPlayer.exe");
	m_path_yaan = _T("亚安播放器\\RealMp4Play.exe");
	m_ftp_path = _T("/");
	//}}AFX_DATA_INIT

}


void CDLGSetSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetSystem)
	DDX_Check(pDX, IDC_CHECK_ALARM_PIC, m_check_alarmpic);
	DDX_Check(pDX, IDC_CHECK_ALARM_WAV, m_check_alarmwav);
	DDX_Check(pDX, IDC_CHECK_FTP, m_check_ftp);
	DDX_CBIndex(pDX, IDC_COMBO_RECORDFULL, m_recordfull);
	DDX_Text(pDX, IDC_EDIT_ALARM_WAV, m_path_alarmwav);
	DDV_MaxChars(pDX, m_path_alarmwav, 260);
	DDX_Text(pDX, IDC_EDIT_CAPBMP_DIR, m_path_capbmp);
	DDV_MaxChars(pDX, m_path_capbmp, 260);
	DDX_Text(pDX, IDC_EDIT_DETECT_DIR, m_path_detect);
	DDV_MaxChars(pDX, m_path_detect, 260);
	DDX_Text(pDX, IDC_EDIT_RECORD_DIR, m_path_record);
	DDV_MaxChars(pDX, m_path_record, 260);
	DDX_Text(pDX, IDC_EDIT_TIME, m_record_cuttime);
	DDV_MinMaxInt(pDX, m_record_cuttime, 1, 60);
	DDX_Text(pDX, IDC_EDIT_FTP_IPADDR, m_ftp_ip);
	DDV_MaxChars(pDX, m_ftp_ip, 32);
	DDX_Text(pDX, IDC_EDIT_FTP_PORT, m_ftp_port);
	DDV_MaxChars(pDX, m_ftp_port, 32);
	DDX_Text(pDX, IDC_EDIT_FTP_PSW, m_ftp_psw);
	DDV_MaxChars(pDX, m_ftp_psw, 32);
	DDX_Text(pDX, IDC_EDIT_FTP_USER, m_ftp_user);
	DDV_MaxChars(pDX, m_ftp_user, 32);
	DDX_Check(pDX, IDC_CHECK_SAVENET, m_check_savenet);
	DDX_Text(pDX, IDC_EDIT_DAHUA_PATH, m_path_dahua);
	DDV_MaxChars(pDX, m_path_dahua, 260);
	DDX_Text(pDX, IDC_EDIT_HAIKANG_PATH, m_path_haikang);
	DDV_MaxChars(pDX, m_path_haikang, 260);
	DDX_Text(pDX, IDC_EDIT_YAAN_PATH, m_path_yaan);
	DDV_MaxChars(pDX, m_path_yaan, 260);
	DDX_Text(pDX, IDC_EDIT_FTP_PATH, m_ftp_path);
	DDV_MaxChars(pDX, m_ftp_path, 260);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSetSystem, CDialog)
	//{{AFX_MSG_MAP(CDLGSetSystem)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_WAV, OnButtonAlarmWav)
	ON_BN_CLICKED(IDC_BUTTON_DETECT_DIR, OnButtonDetectDir)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_DIR, OnButtonRecordDir)
	ON_BN_CLICKED(IDC_BUTTON_CAPBMP_DIR, OnButtonCapbmpDir)
	ON_BN_CLICKED(IDC_BUTTON_HAIKANG_PATH, OnButtonHaikangPath)
	ON_BN_CLICKED(IDC_BUTTON_DAHUA_PATH, OnButtonDahuaPath)
	ON_BN_CLICKED(IDC_BUTTON_YAAN_PATH, OnButtonYaanPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSetSystem message handlers
BOOL CDLGSetSystem::OnInitDialog()
{
	CDialog::OnInitDialog();

	Read2Dlg();
	UpdateData(FALSE);

#if OPEN_VS2008_POCO_FTP

	GetDlgItem(IDC_STATIC_FTP)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_FTP_IP)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_FTP_PORT)->ShowWindow(TRUE);	
	GetDlgItem(IDC_STATIC_FTP_USER)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_FTP_PSW)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_FTP_IPADDR)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_FTP_PORT)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_FTP_USER)->ShowWindow(TRUE);	
	GetDlgItem(IDC_EDIT_FTP_PSW)->ShowWindow(TRUE);

	GetDlgItem(IDC_CHECK_FTP)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_FTP_PATH)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_FTP_PATH)->ShowWindow(TRUE);
#endif

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
	}
}

void CDLGSetSystem::OnButtonDetectDir() 
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
		m_path_detect=FullPath;
		UpdateData(FALSE);
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
	CreateDirectory(m_path_detect, NULL);
	CreateDirectory(m_path_record, NULL);

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
		CreateDirectory(m_path_detect, NULL);
		CreateDirectory(m_path_record, NULL);

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
	int check_savenet;
	int		recordfull;
	char	path_alarmwav[ZOG_MAX_PATH_STR]="";
	char	path_capbmp[ZOG_MAX_PATH_STR]="";
	char	path_detect[ZOG_MAX_PATH_STR]="";
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

	////////////////////////////////////
	if(GetPrivateProfileStruct("Alarm", "CheckPic", &check_alarmpic, sizeof(int), path))
		m_check_alarmpic=check_alarmpic;
	if(GetPrivateProfileStruct("Alarm", "CheckWav", &check_alarmwav, sizeof(int), path))
		m_check_alarmwav=check_alarmwav;
	if(GetPrivateProfileStruct("Alarm", "CheckSaveNet", &check_savenet, sizeof(int), path))
		m_check_savenet=check_savenet;
	if(GetPrivateProfileString("Alarm", "PathAlarmWav", "", path_alarmwav, ZOG_MAX_PATH_STR, path))
		m_path_alarmwav=path_alarmwav;
	if(GetPrivateProfileString("Alarm", "PathDetect", "", path_detect, ZOG_MAX_PATH_STR, path))
		m_path_detect=path_detect;
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
	if(GetPrivateProfileString("Play", "PathYaan", "", path_yaan, ZOG_MAX_PATH_STR, path))
		m_path_yaan=path_yaan;
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
	if(GetPrivateProfileString("FTP", "path", "", ftp_path, ZOG_MAX_NAME_STR, path))
		m_ftp_path=ftp_path;

	CreateDirectory(m_path_capbmp, NULL);
	CreateDirectory(m_path_detect, NULL);
	CreateDirectory(m_path_record, NULL);
}
//写ini
void CDLGSetSystem::writeini(char *path)
{
	////////////////////////////////////
	WritePrivateProfileStruct("Alarm", "CheckPic", &m_check_alarmpic, sizeof(int), path);
	WritePrivateProfileStruct("Alarm", "CheckWav", &m_check_alarmwav, sizeof(int), path);
	WritePrivateProfileStruct("Alarm", "CheckSaveNet", &m_check_savenet, sizeof(int), path);
	
	WritePrivateProfileString("Alarm", "PathAlarmWav", m_path_alarmwav.GetBuffer(0),path);
	WritePrivateProfileString("Alarm", "PathDetect", m_path_detect.GetBuffer(0),path);
	////////////////////////////////////
	WritePrivateProfileStruct("Record", "CutTime", &m_record_cuttime, sizeof(int), path);
	WritePrivateProfileStruct("Record", "RecordFull", &m_recordfull, sizeof(int), path);
	WritePrivateProfileString("Record", "PathRecord",  m_path_record.GetBuffer(0),  path);
	WritePrivateProfileString("Record", "PathCapbmp",  m_path_capbmp.GetBuffer(0), path);
	////////////////////////////////////
	WritePrivateProfileString("Play", "PathDahua",  m_path_dahua.GetBuffer(0),  path);
	WritePrivateProfileString("Play", "PathHaikang",  m_path_haikang.GetBuffer(0), path);
	WritePrivateProfileString("Play", "PathYaan",  m_path_yaan.GetBuffer(0), path);
	////////////////////////////////////
	WritePrivateProfileStruct("FTP", "check", &m_check_ftp, sizeof(int), path);
	WritePrivateProfileString("FTP", "ip", m_ftp_ip.GetBuffer(0), path);
	WritePrivateProfileString("FTP", "port", m_ftp_port.GetBuffer(0), path);
	WritePrivateProfileString("FTP", "user",  m_ftp_user.GetBuffer(0), path);

	char	ftp_psw_ext[(ZOG_MAX_NAME_STR+1)*5]="";
	ZogEnCode(m_ftp_psw.GetBuffer(0),ftp_psw_ext);
	WritePrivateProfileString("FTP", "password",  ftp_psw_ext, path);

	WritePrivateProfileString("FTP", "path",  m_ftp_path.GetBuffer(0), path);
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
	}
}

void CDLGSetSystem::OnButtonYaanPath() 
{
	// TODO: Add your control notification handler code here
	char szFilter[]="YaAn Play Executable Files (*.exe)|*.exe|*.*|*.*||";
	CFileDialog dlg(TRUE,"exe","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_path_yaan=dlg.GetPathName();
		UpdateData(FALSE);
	}
}
