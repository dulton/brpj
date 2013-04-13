// DLGSettings.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeRecord.h"
#include "DLGSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGSettings dialog


CDLGSettings::CDLGSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSettings)
	m_record = _T("D:\\\\record");
	m_shotframe = _T("D:\\\\shotframe");
	m_comboRecord = 0;
	m_comboShotframe =0;
	//}}AFX_DATA_INIT
}


void CDLGSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSettings)
	DDX_Text(pDX, IDC_EDIT_RECORD, m_record);
	DDV_MaxChars(pDX, m_record, 260);
	DDX_Text(pDX, IDC_EDIT_SHOTFRAME, m_shotframe);
	DDV_MaxChars(pDX, m_shotframe, 260);
	DDX_CBIndex(pDX, IDC_COMBO_RECORD, m_comboRecord);
	DDX_CBIndex(pDX, IDC_COMBO_SHOTFRAME, m_comboShotframe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSettings, CDialog)
	//{{AFX_MSG_MAP(CDLGSettings)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_SHOTFRAME, OnButtonShotframe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSettings message handlers
BOOL CDLGSettings::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();

	CComboBox *p=(CComboBox *)GetDlgItem(IDC_COMBO_RECORD);
	p->AddString(Language_ConvertString("Date-ProductName-Size"));
	p->AddString(Language_ConvertString("Date-Barcode-Size"));
	p->AddString(Language_ConvertString("Barcode-Date-Size"));
	p->AddString(Language_ConvertString("ProductName-Date-Size"));
	p->AddString(Language_ConvertString("Date-ProductName-Barcode-Size"));
	p->AddString(Language_ConvertString("Barcode-Date-ProductName-Size"));
	
	p=(CComboBox *)GetDlgItem(IDC_COMBO_SHOTFRAME);
	p->AddString(Language_ConvertString("Date-ProductName-Size"));
	p->AddString(Language_ConvertString("Date-Barcode-Size"));
	p->AddString(Language_ConvertString("Barcode-Date-Size"));
	p->AddString(Language_ConvertString("ProductName-Date-Size"));
	p->AddString(Language_ConvertString("Date-ProductName-Barcode-Size"));
	p->AddString(Language_ConvertString("Barcode-Date-ProductName-Size"));

	Read2Dlg();
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDLGSettings::OnButtonRecord() 
{
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
		m_record=FullPath;
		UpdateData(FALSE);
	}
}

void CDLGSettings::OnButtonShotframe() 
{
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
		m_shotframe=FullPath;
		UpdateData(FALSE);
	}
}

void CDLGSettings::OnOK() 
{
	// TODO: Add extra validation here
	//写入
	UpdateData(TRUE);
	Dlg2Write();

	//创建文件夹
	CreateDirectory(m_record, NULL);
	CreateDirectory(m_shotframe, NULL);
	CDialog::OnOK();
}

void CDLGSettings::OnCancel() 
{
	// TODO: Add extra cleanup here
	//重读
	Read2Dlg();
	UpdateData(FALSE);
	CDialog::OnCancel();
}

void CDLGSettings::Read2Dlg() 
{
	//测试文件是否存在
	FILE *fp;
	if(fp=fopen(SystemIniPath,"r"))
		fclose(fp);
	else
	{
		//不存在则写入后退出
		Dlg2Write();
		CreateDirectory(m_record, NULL);
		CreateDirectory(m_shotframe, NULL);

		return;
	}

	readini(SystemIniPath);
}

void CDLGSettings::Dlg2Write() 
{
	writeini(SystemIniPath);
}

//读INI
void CDLGSettings::readini(char *path) 
{
	int	check_record;
	int	check_shotframe;

	char	path_record[ZOG_MAX_PATH_STR]="";
	char	path_shotframe[ZOG_MAX_PATH_STR]="";

	////////////////////////////////////

	if(GetPrivateProfileStruct("Record", "combo", &check_record, sizeof(int), path))
		m_comboRecord=check_record;
	if(GetPrivateProfileStruct("Shotframe", "combo", &check_shotframe, sizeof(int), path))
		m_comboShotframe=check_shotframe;

	if(GetPrivateProfileString("Record", "Path", "", path_record, ZOG_MAX_PATH_STR, path))
		m_record=path_record;
	if(GetPrivateProfileString("Shotframe", "Path", "", path_shotframe, ZOG_MAX_PATH_STR, path))
		m_shotframe=path_shotframe;

	CreateDirectory(m_record, NULL);
	CreateDirectory(m_shotframe, NULL);

}
//写ini
void CDLGSettings::writeini(char *path)
{
	WritePrivateProfileStruct("Record", "combo", &m_comboRecord, sizeof(int), path);
	WritePrivateProfileStruct("Shotframe", "combo", &m_comboShotframe, sizeof(int), path);

	WritePrivateProfileString("Record", "Path", m_record.GetBuffer(0),path);
	WritePrivateProfileString("Shotframe", "Path", m_shotframe.GetBuffer(0),path);
}
