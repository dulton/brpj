// DLGpictureView.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGpictureView.h"

// CDLGpictureView dialog

IMPLEMENT_DYNAMIC(CDLGpictureView, CDialog)

CDLGpictureView::CDLGpictureView(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGpictureView::IDD, pParent)
{

}

CDLGpictureView::~CDLGpictureView()
{
}

void CDLGpictureView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD_BMP, m_b_download);
}


BEGIN_MESSAGE_MAP(CDLGpictureView, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDLGpictureView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDLGpictureView::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CDLGpictureView::OnBnClickedButtonDownload)

	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CDLGpictureView::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(Titlestr);

	m_b_download.LoadBitmaps(IDB_DOWNLOAD_BUTTON,IDB_DOWNLOAD_BUTTON_MOVE,NULL,IDB_DOWNLOAD_BUTTON_DIS);
	m_b_download.SizeToContent();		//自适应图片大小

	//加载图片
	bim=pic.LoadPicture(srcfile.GetBuffer());


	//初始即最大化 //放最后
	ShowWindow(SW_MAXIMIZE);   


	return TRUE;
}
// CDLGpictureView message handlers

void CDLGpictureView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//释放图片
	pic.FreePicture();
	OnCancel();
}

void CDLGpictureView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
}
void CDLGpictureView::OnBnClickedButtonDownload()
{
	//保存文件
	char szFilter[]="Jpeg Files (*.jpg)|*.jpg|*.*|*.*||";
	CFileDialog dlg(FALSE,".jpg",Titlestr.GetBuffer(0),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal()==IDOK)
	{
		CopyFile(srcfile,dlg.GetPathName().GetBuffer(0),FALSE);
	}
}

void CDLGpictureView::OnPaint()
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

	// TODO: Add your message handler code here
	//放在ONINIT中无效

	//DrawFileImage(&m_pic,"c:\\36273.jpg");

	if(bim)
	{
		//显示图片
		CDC *pDC=m_pic.GetDC();
		CRect rect; 
		m_pic.GetClientRect(&rect);//获得pictrue控件所在的举行区域
		pic.DrawPicture(*pDC,0,0,rect.Width(),rect.Height());
		m_pic.ReleaseDC(pDC);
	}


	CDialog::OnPaint();
}


void CDLGpictureView::DrawImage(CStatic *m_picBox,unsigned char *image,unsigned long int size)
{
	CRect rect; 
	m_picBox->GetClientRect(&rect);//获得pictrue控件所在的举行区域
	CDC *pDC=m_picBox->GetDC();//获得pictrue控件的DC 

	IPicture *pPic;//定义显示图片的接口（可显示jpg格式图片）

	HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,size);//分配内存空间
	void *pData=GlobalLock(hGlobal);//定义图象数据指针
	memcpy(pData,image,size); 
	GlobalUnlock(hGlobal);//释放内存空间
	IStream *pStream=NULL;//定义数据流指针
	//创建数据流
	if(CreateStreamOnHGlobal(hGlobal,TRUE,&pStream)==S_OK) 
	{ 
		//装载图象文件
		if(SUCCEEDED(OleLoadPicture(pStream,size,TRUE,IID_IPicture,(LPVOID*)&pPic))) 
		{  
			OLE_XSIZE_HIMETRIC hmWidth; 
			OLE_XSIZE_HIMETRIC hmHeight; 
			pPic->get_Width(&hmWidth);//获得图象真实宽度
			pPic->get_Height(&hmHeight);//获得图象真实高度
			//在控件上显示图片
			pPic->Render(*pDC,0,0,rect.Width(),rect.Height(),0,hmHeight,hmWidth,-hmHeight,NULL); 
			pPic->Release(); 
			pStream->Release();//释放数据流
		}  
	}

	m_picBox->ReleaseDC(pDC);

	if(hGlobal)
	{
		GlobalFree(hGlobal);
		hGlobal = NULL;
	}
}
void CDLGpictureView::DrawFileImage(CStatic *m_picBox, char *filename)
{
	CRect rect; 
	m_picBox->GetClientRect(&rect);//获得pictrue控件所在的举行区域
	CDC *pDC=m_picBox->GetDC();//获得pictrue控件的DC 

	IPicture *pPic;//定义显示图片的接口（可显示jpg格式图片）

	FILE *fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp ,0, SEEK_SET);

	HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,size);//分配内存空间
	void *pData=GlobalLock(hGlobal);//定义图象数据指针

	fread(pData, sizeof(BYTE), size, fp);
	GlobalUnlock(hGlobal);//释放内存空间

	fclose(fp);

	IStream *pStream=NULL;//定义数据流指针
	//创建数据流
	if(CreateStreamOnHGlobal(hGlobal,TRUE,&pStream)==S_OK) 
	{ 
		//装载图象文件
		if(SUCCEEDED(OleLoadPicture(pStream,size,TRUE,IID_IPicture,(LPVOID*)&pPic))) 
		{  
			OLE_XSIZE_HIMETRIC hmWidth; 
			OLE_XSIZE_HIMETRIC hmHeight; 
			pPic->get_Width(&hmWidth);//获得图象真实宽度
			pPic->get_Height(&hmHeight);//获得图象真实高度
			//在控件上显示图片
			pPic->Render(*pDC,0,0,rect.Width(),rect.Height(),0,hmHeight,hmWidth,-hmHeight,NULL); 
			pPic->Release(); 
			pStream->Release();//释放数据流
		}  
	}

	m_picBox->ReleaseDC(pDC);

	if(hGlobal)
	{
		GlobalFree(hGlobal);
		hGlobal = NULL;
	}
}


//静态文本控件 透明
HBRUSH CDLGpictureView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
void CDLGpictureView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect		m_clientRect;		//程序界面区域位置
	GetClientRect(&m_clientRect);

	//打印日志条
	int download_height=24;
	int distance=5;
	int  txt_width=80;

	//图片
	CRect pic_Rect;
	pic_Rect.top=m_clientRect.top;
	pic_Rect.bottom=m_clientRect.bottom-download_height-distance*2;

	pic_Rect.left = m_clientRect.left;
	pic_Rect.right = m_clientRect.right;
	GetDlgItem(IDC_PIC)->MoveWindow(pic_Rect);

	//DOWNLOAD
	CRect download_Rect;
	download_Rect.top=pic_Rect.bottom+distance;
	download_Rect.bottom=m_clientRect.bottom-distance;

	download_Rect.left = m_clientRect.right-txt_width-distance;
	download_Rect.right = m_clientRect.right-distance;
	GetDlgItem(IDC_BUTTON_DOWNLOAD_BMP)->MoveWindow(download_Rect);

	Invalidate();
}
