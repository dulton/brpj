// YiRongCarInFoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YiRongCarInFo.h"
#include "YiRongCarInFoDlg.h"
#include "HistoryDLG.h"

#include "sms.h"

#define SAVE_PIC_PATH "D:\\YiRongCarPic"
#define FTPSCAN_PIC_PATH "yirognftpscan.db"


#define PIC_WINDOW_W 562
#define PIC_WINDOW_H 360

#define RELINK_TIMER 1
#define DISPLAY_TIMER 2

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHistoryDLG historydlg;
CYiRongCarInFoDlg *YiRongCardlg;

SMS smsclass;
// CYiRongCarInFoDlg 对话框


CYiRongCarInFoDlg::CYiRongCarInFoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYiRongCarInFoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYiRongCarInFoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALARM, m_LISTAlarm);
	DDX_Control(pDX, IDC_LIST_REPORT, m_LISTReport);
}

BEGIN_MESSAGE_MAP(CYiRongCarInFoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_REPORT, &CYiRongCarInFoDlg::OnLvnItemActivateListReport)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_ALARM, &CYiRongCarInFoDlg::OnLvnItemActivateListAlarm)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY, &CYiRongCarInFoDlg::OnBnClickedButtonHistory)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_ALL, &CYiRongCarInFoDlg::OnBnClickedButtonHistoryAll)
END_MESSAGE_MAP()


// CYiRongCarInFoDlg 消息处理程序

BOOL CYiRongCarInFoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	YiRongCardlg=this;

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	m_LISTReport.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 50);
	m_LISTReport.InsertColumn(1, _T("时间") , LVCFMT_LEFT, 160);
	m_LISTReport.InsertColumn(2, _T("摄像头ip"), LVCFMT_LEFT, 130);
	m_LISTReport.InsertColumn(3, _T("车牌号"), LVCFMT_LEFT, 60);
	m_LISTReport.InsertColumn(4, _T("图片路径"), LVCFMT_LEFT, 120);
	m_LISTReport.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	
	m_LISTAlarm.InsertColumn(0, _T("序号") , LVCFMT_LEFT, 50);
	m_LISTAlarm.InsertColumn(1, _T("时间" ), LVCFMT_LEFT, 140);
	m_LISTAlarm.InsertColumn(2, _T("摄像头ip"), LVCFMT_LEFT, 110);
	m_LISTAlarm.InsertColumn(3, _T("车牌号"), LVCFMT_LEFT, 60);
	m_LISTAlarm.InsertColumn(4, _T("图片路径"), LVCFMT_LEFT, 5);
	m_LISTAlarm.InsertColumn(5, _T("被盗品牌") , LVCFMT_LEFT, 80);
	m_LISTAlarm.InsertColumn(6, _T("被盗车牌号"), LVCFMT_LEFT,90);
	m_LISTAlarm.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CvSize ImgSize;
	ImgSize.width = PIC_WINDOW_W;
	ImgSize.height = PIC_WINDOW_H;
	m_ImageREPORT = cvCreateImage( ImgSize, IPL_DEPTH_8U, 3 );

	ImgSize.width = PIC_WINDOW_W;
	ImgSize.height = PIC_WINDOW_H;
	m_ImageALARM = cvCreateImage( ImgSize, IPL_DEPTH_8U, 3 );

	FILE *fp;
#if 1
	int i=0;
	fp=fopen("ftppath.txt","r");
	if(fp)
	{
		memset(ftppath,0,sizeof(char)*260);
		fgets(ftppath,260*sizeof(char),fp);
		i=strlen(ftppath);
		if(i>0 && 10 == ftppath[i-1])
			ftppath[i-1]=0;
		fclose(fp);

		if(0==i)
			MessageBox("ftppath.txt is null");
	}
	else
		MessageBox("ftppath.txt con't read");

	CreateDirectory(SAVE_PIC_PATH, NULL);

	m_connectionState=pio.ConnectionOracleDBTXT("dbconfig.txt");
	if(!m_connectionState)
		MessageBox("oracle error");

	REPORTtotal=0;
	ALARMtotal=0;

	m_relinktimer = SetTimer(RELINK_TIMER,2000,NULL);
	m_displaytimer = SetTimer(DISPLAY_TIMER,1000,NULL);

	// TODO: 在此添加额外的初始化代码
	smsclass.smsStart();
	if(smsclass.smsInit())
		MessageBox("smsInit error");
	if(smsclass.smsLogin())
		MessageBox("smsLogin error");
#endif
/*
sprintf(str,"smsInit %d",ggg);
MessageBox(str);
*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CYiRongCarInFoDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == RELINK_TIMER)
	{
		MoveFile();
	}
	else if(nIDEvent == DISPLAY_TIMER)
	{
		ReadAlarm();
		ReadReport();
	}

	CDialog::OnTimer(nIDEvent);
}

#if 0

//调用命令行命令而不显示命令行窗口  
BOOL system_hide(char* CommandLine)     
{     
	SECURITY_ATTRIBUTES   sa;     
	HANDLE   hRead,hWrite;     

	sa.nLength   =   sizeof(SECURITY_ATTRIBUTES);     
	sa.lpSecurityDescriptor   =   NULL;     
	sa.bInheritHandle   =   TRUE;     
	if   (!CreatePipe(&hRead,&hWrite,&sa,0))       
	{     
		return   FALSE;     
	}       

	STARTUPINFO   si;     
	PROCESS_INFORMATION   pi;       
	si.cb   =   sizeof(STARTUPINFO);     
	GetStartupInfo(&si);       
	si.hStdError   =   hWrite;     
	si.hStdOutput   =   hWrite;     
	si.wShowWindow   =   SW_HIDE;     
	si.dwFlags   =   STARTF_USESHOWWINDOW   |   STARTF_USESTDHANDLES;     
	//关键步骤，CreateProcess函数参数意义请查阅MSDN     
	if   (!CreateProcess(NULL, CommandLine, NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))       
	{     
		return   FALSE;     
	}     
	CloseHandle(hWrite);     
/*
	char   buffer[4096]   =   {0};     
	DWORD   bytesRead;       
	while(true)       
	{     
	memset(buffer,0,strlen(buffer));  
	if(ReadFile(hRead,buffer,4095,&bytesRead,NULL)==NULL)     
	break;     
	//buffer中就是执行的结果，可以保存到文本，也可以直接输出     
	//printf(buffer);//这行注释掉就可以了     
     
	}      
*/	
	return   TRUE;     
}  
#endif


void CYiRongCarInFoDlg::MoveFile(void)
{
	char str[1024]={0};
	char tempstr[260]={0};
	char tempDst[260]={0};
	char *tempstrB;

	unsigned long int tempsize; 
	unsigned long int fsize; 
	struct HISTORY_STRUCT hs={0};
	
	char tempJPG[260];
	char *tempstrC;

	FILE *fp;
	FILE *fppic;

	int i;
	//创建文件
	sprintf(str,"cmd.exe /c dir /s /b \"%s\\*.txt\" >\"%s\\%s\"",ftppath,ftppath,FTPSCAN_PIC_PATH);
	//system_hide(str);
	WinExec(str,SW_HIDE);
	Sleep(100);

	sprintf(str,"%s\\%s",ftppath,FTPSCAN_PIC_PATH);
	fp=fopen(str,"r");
	if(fp)
	{
		do
		{
			memset(tempstr,0,sizeof(char)*260);
			fgets(tempstr,260*sizeof(char),fp);
			i=strlen(tempstr);
			if(i>0)
			{
				tempstr[i-1]=0;
				tempstrB=strrchr(tempstr,'\\');
				tempstrB++;

				tempsize=0;
				sscanf(tempstrB,"%d-%d-%d-%d-%d-%d %s %s %d .txt",	\
					&hs.year,
					&hs.mon,
					&hs.day,
					&hs.hour,
					&hs.min,
					&hs.sec,
					hs.ip,
					hs.carNumber,
					&tempsize);

				fppic=NULL;
				fppic = fopen(tempstr, "rb");
				if(NULL == fppic)
					continue;
				fsize=0;
				fseek(fppic,0,SEEK_END);
				fsize = ftell(fppic);
				fclose(fppic);

				if(tempsize != fsize)
					continue;

				//创建目录
				sprintf(str,"%s\\%04d-%02d-%02d",SAVE_PIC_PATH,hs.year,hs.mon,hs.day);
				CreateDirectory(str, NULL);

				//改名
					sprintf(tempJPG,"%s",tempstrB);
					tempstrC=strrchr(tempJPG,'.');
					*(tempstrC+1)='j';
					*(tempstrC+2)='p';
					*(tempstrC+3)='g';
					*(tempstrC+4)=0;

				//加JPG 永远保持JPG格式
				sprintf(tempDst,"%s\\%04d-%02d-%02d\\%s",	\
					SAVE_PIC_PATH,hs.year,hs.mon,hs.day,tempJPG);

				sprintf(str,"cmd.exe /c move \"%s\" \"%s\"",	\
					tempstr,tempDst);
				//system_hide(str);
				WinExec(str,SW_HIDE);
				Sleep(100);

				//写数据库
				if(true == m_connectionState)
					pio.writeMatchResultVehiclePath(hs.ip,hs.carNumber,tempDst);
			}

		}while(!feof(fp));
		fclose(fp);
	}
//	else
//		MessageBox(str);
}

void  CYiRongCarInFoDlg::ReadReport(void)
{
	struct HISTORY_STRUCT hs={0};

	char tempstr[260]={0};

	int nItem;

	//读数据库
	if(true == m_connectionState)
	{
		if(pio.readMatchResultVehiclePath(hs))
		{
			//插一列
			if(REPORTtotal>3000)
			{
				m_LISTReport.DeleteAllItems();
				REPORTtotal=0;
			}

			sprintf(tempstr,"%d",REPORTtotal+1);
			nItem = m_LISTReport.InsertItem(0,tempstr);

			sprintf(tempstr,"%04d-%02d-%02d  %02d:%02d:%02d",	\
				hs.year,
				hs.mon,
				hs.day,
				hs.hour,
				hs.min,
				hs.sec);

			m_LISTReport.SetItemText(nItem,1,tempstr);
			m_LISTReport.SetItemText(nItem,2,hs.ip);
			m_LISTReport.SetItemText(nItem,3,hs.carNumber);
			m_LISTReport.SetItemText(nItem,4,hs.path);
			//显示图像
			if(strlen(hs.path)>0)
				ReadImageREPORT(hs.path);

			REPORTtotal++;
		}
	}

}

void CYiRongCarInFoDlg::ReadAlarm(void)
{
	struct HISTORY_STRUCT hs={0};

	char tempstr[512]={0};

	int nItem;

	//读数据库
	if(true == m_connectionState)
	{
		if(pio.readAlarmVehicle(hs))
		{
			if(ALARMtotal>3000)
			{
				m_LISTAlarm.DeleteAllItems();
				ALARMtotal=0;
			}

			sprintf(tempstr,"%d",ALARMtotal+1);
			nItem = m_LISTAlarm.InsertItem(0,tempstr);

			sprintf(tempstr,"%04d-%02d-%02d  %02d:%02d:%02d",	\
				hs.year,
				hs.mon,
				hs.day,
				hs.hour,
				hs.min,
				hs.sec);

			m_LISTAlarm.SetItemText(nItem,1,tempstr);
			m_LISTAlarm.SetItemText(nItem,2,hs.ip);
			m_LISTAlarm.SetItemText(nItem,3,hs.carNumber);
			m_LISTAlarm.SetItemText(nItem,4,hs.path);
			m_LISTAlarm.SetItemText(nItem,5,hs.info.PinPai);
			m_LISTAlarm.SetItemText(nItem,6,hs.info.ChePaiHao);
			//显示图像
			if(strlen(hs.path)>0)
				ReadImageALARM(hs.path);

			ALARMtotal++;

			sprintf(tempstr,"[发现可疑电动车:%d]津泰路口\n品牌:%s\n车牌号:%s\n车架号:%s\n电机号:%s\n失主姓名:%s\n电话:%s\n",	\
				ALARMtotal,
				hs.info.PinPai,
				hs.info.ChePaiHao,
				hs.info.CheJiaHao,
				hs.info.DianJiHao,
				hs.info.Name,
				hs.info.Phone);

			smsclass.smsSendMultiMy("dstphone.txt",tempstr,strlen(tempstr));
		}
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYiRongCarInFoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CYiRongCarInFoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYiRongCarInFoDlg::OnOK()
{
	// TODO: Add your control notification handler code here

}

void CYiRongCarInFoDlg::OnDestroy()
{
	smsclass.smsLogout();
	smsclass.smsUninit();

	// TODO: Add your control notification handler code here
	CDialog::OnDestroy();
}

void CYiRongCarInFoDlg::OnLvnItemActivateListReport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	char str[260]={0};
	m_LISTReport.GetItemText(pNMIA->iItem,4,str,260);
	//ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);
	if(strlen(str)>0)
		ReadImageREPORT(str);

	*pResult = 0;
}

void CYiRongCarInFoDlg::OnLvnItemActivateListAlarm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	char str[260]={0};
	m_LISTAlarm.GetItemText(pNMIA->iItem,4,str,260);
	//ShellExecute(this->m_hWnd,NULL,str,NULL,NULL,SW_NORMAL);
	if(strlen(str)>0)
		ReadImageALARM(str);

	*pResult = 0;
}


void CYiRongCarInFoDlg::ReadImageREPORT(CString imgPath)
{
	// 读取图片、缓存到一个局部变量 ipl 中
	//IplImage* ipl = cvLoadImage((const char *)(LPCTSTR)imgPath, 1);

	IplImage* ipl = cvLoadImage(imgPath, 1);

	// 判断图片是否读取成功
	if(!ipl)
	  return;

	// 清除上一幅显示的图片数据
	if(this->m_ImageREPORT)
	  cvZero(this->m_ImageREPORT);

	// 对读入的图片进行缩放，使其宽或高最高值刚好等于256，再复制到 m_Image 中
	ResizeImageREPORT(ipl);

	// 调用显示图片函数
	ShowImage(this->m_ImageREPORT, IDC_STATIC_REPORT);

	//释放 ipl 占用的内存
	cvReleaseImage(&ipl); 
}

void CYiRongCarInFoDlg::ResizeImageREPORT(IplImage* img)
{
	// 获取图片的宽和高
	int width = img->width;
	int height = img->height;

	// 找出宽和高的较大值
	int max = (width > height) ? width : height;

	// 计算将图片缩放到Image区域所需的比例因子
	float scale = (float) ((float) max / 256.0f);

	// 缩放后图片的宽和高
	int nwidth = PIC_WINDOW_W;//(int) (width / scale);
	int nheight = PIC_WINDOW_H;//(int) (height / scale);

	// 计算图片在Image左上角的期望坐标值
	int tlx = 0;//(nwidth > nheight) ? 0 : (int) (256 - nwidth) / 2;
	int tly = 0;//(nwidth > nheight) ? (int) (256 - nwidth) / 2 : 0;

	// 设置Image的ROI区域，用于存入图片Img
	cvSetImageROI(this->m_ImageREPORT, cvRect( tlx, tly, nwidth, nheight) );

	// 对图片img进行缩放，并存入m_Image中
	cvResize( img, this->m_ImageREPORT);

	// 重置m_Image的ROI准备读入下一幅图像
	cvResetImageROI(this->m_ImageREPORT);	  
}


void CYiRongCarInFoDlg::ReadImageALARM(CString imgPath)
{
	// 读取图片、缓存到一个局部变量 ipl 中
	//IplImage* ipl = cvLoadImage((const char *)(LPCTSTR)imgPath, 1);

	IplImage* ipl = cvLoadImage(imgPath, 1);

	// 判断图片是否读取成功
	if(!ipl)
	  return;

	// 清除上一幅显示的图片数据
	if(this->m_ImageALARM)
	  cvZero(this->m_ImageALARM);

	// 对读入的图片进行缩放，使其宽或高最高值刚好等于256，再复制到 m_Image 中
	ResizeImageALARM(ipl);

	// 调用显示图片函数
	ShowImage(this->m_ImageALARM, IDC_STATIC_ALARM);

	//释放 ipl 占用的内存
	cvReleaseImage(&ipl); 
}

void CYiRongCarInFoDlg::ResizeImageALARM(IplImage* img)
{
	// 获取图片的宽和高
	int width = img->width;
	int height = img->height;

	// 找出宽和高的较大值
	int max = (width > height) ? width : height;

	// 计算将图片缩放到Image区域所需的比例因子
	float scale = (float) ((float) max / 256.0f);

	// 缩放后图片的宽和高
	int nwidth = PIC_WINDOW_W;//(int) (width / scale);
	int nheight = PIC_WINDOW_H;//(int) (height / scale);

	// 计算图片在Image左上角的期望坐标值
	int tlx = 0;//(nwidth > nheight) ? 0 : (int) (256 - nwidth) / 2;
	int tly = 0;//(nwidth > nheight) ? (int) (256 - nwidth) / 2 : 0;

	// 设置Image的ROI区域，用于存入图片Img
	cvSetImageROI(this->m_ImageALARM, cvRect( tlx, tly, nwidth, nheight) );

	// 对图片img进行缩放，并存入m_Image中
	cvResize( img, this->m_ImageALARM);

	// 重置m_Image的ROI准备读入下一幅图像
	cvResetImageROI(this->m_ImageALARM);	  
}


void CYiRongCarInFoDlg::ShowImage(IplImage* img, UINT ID)
{
	// 获得显示控件的 DC
	CDC* pCDC = GetDlgItem(ID)->GetDC();

	// 获取HDC（设备句柄）来进行绘图操作
	HDC pHDC = pCDC->GetSafeHdc();

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	// 求出图片控件的宽和高
	int rw = rect.right - rect.left;
	int rh = rect.bottom - rect.top;

	// 读取图片的宽和高
	int iw = img->width;
	int ih = img->height;

	// 使图片的显示位置正好在控件的正中
	int tx = (int) (rw - iw) / 2;
	int ty = (int) (rh - ih) / 2;

	SetRect( rect, tx, ty, tx+iw, ty+ih);

	// 复制图片
	CvvImage cimg;
	cimg.CopyOf(img);

	// 将图片绘制到显示控件的制定区域内
	cimg.DrawToHDC(	pHDC, &rect );

	ReleaseDC( pCDC);
}

void CYiRongCarInFoDlg::OnBnClickedButtonHistory()
{
	// TODO: Add your control notification handler code here
	historydlg.Flag=0;
	historydlg.DoModal();
}

void CYiRongCarInFoDlg::OnBnClickedButtonHistoryAll()
{
	// TODO: Add your control notification handler code here
	historydlg.Flag=1;
	historydlg.DoModal();
}
