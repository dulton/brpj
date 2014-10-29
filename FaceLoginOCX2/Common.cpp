#include "StdAfx.h"
#include "Common.h"
#include "md5.h"

CCommon::CCommon(void)
{
}

CCommon::~CCommon(void)
{
}


CString CCommon::Char2CString(char * str)
{
	CString tmp;
	tmp.Append(str);
	return tmp;
}

char * CCommon::CString2Char(CString str)
{
	return str.GetBuffer();
}

CString CCommon::GetSystemFolder()
{
	CString sysPath;
	UINT size = GetSystemDirectory(NULL,0);
	CHAR * path = new CHAR[size];
	if(GetSystemDirectory(path,size) != 0)
	{
		sysPath.Append(path);
	}
	delete[] path;
	return sysPath;
}

char * CCommon::EncodeMd5(char * input)
{
	CMD5 md5;
	md5.GenerateMD5 ((unsigned char*)input,strlen(input));
	std::string md5string = md5.ToString();

	int len = md5string.length();
	char * retchar = new char[len];
	memset(retchar, 0, len);
	memcpy(retchar, md5string.c_str(),len+1);
	return (char *)retchar;
}

/************************************
* 设备初始化
*************************************/
bool CCommon::InitialDevice(CComboBox &m_cbDevice, LONG * width, LONG * height)
{
	LONG w = 0, h = 0;
	bool m_bCameraOpen = false;
	int cam_count = GetCameraCount();//仅仅获取摄像头数目
	m_cbDevice.SetDroppedWidth(cam_count);
	//获取所有摄像头的名称
	for(int i=0; i < cam_count; i++)
	{
		char camera_name[1024];  
		int retval = GetCameraName(i, camera_name, sizeof(camera_name));
		if(retval >0)
		{
			CString str,str_camera_name;
			str_camera_name = CCommon::Char2CString(camera_name);
			//防止摄像头名称太长，超出显示区域
			int spaceNum = str_camera_name.Find(" ");
			CString strTemp = str_camera_name.Mid(0,spaceNum);
			str.Format("[%d] %s",i,strTemp);
			m_cbDevice.InsertString(i,str.GetBuffer(0));
		}
	}

	if(cam_count > 0)
	{		
		CString camera = CCommon::GetReg("Camera");
		int m_iCameraIndex = atoi(CCommon::CString2Char(camera));//注册表获取摄像头id

		if(m_iCameraIndex < 0 || m_iCameraIndex > cam_count - 1) m_iCameraIndex = 0;
			m_cbDevice.SetCurSel(m_iCameraIndex);//设置默认选中第一个摄像头，现在从配置文件读取

		//打开第一个摄像头
		if(OpenCameraCB(m_iCameraIndex, false, &w,&h)) //不弹出属性选择窗口，用代码制定图像宽和高
		{
			m_bCameraOpen = TRUE;
		}

		if(m_bCameraOpen == false)
		{
			for(int k = 0; k < cam_count; k++)
			{
				//打开第一个摄像头
				if(OpenCameraCB(k, false, &w, &h)) //不弹出属性选择窗口，用代码制定图像宽和高
				{
					m_bCameraOpen = TRUE;
					break;
				}
			}
			
			if(m_bCameraOpen == false && cam_count> 0)
			{
				//AfxMessageBox("摄像头初始化失败", MB_OK, MB_ICONINFORMATION);
			
				//OnOK();
				*width = w;
				*width = h;
				return false;
			}
		}
	}
	*width = w;
	*height = h;
	return true;
}

/// <summary>
/// 获取注册表值
/// </summary>
/// <param name="lpValueKey">键名</param>
/// <returns>键值</returns>
CString CCommon::GetReg(CString lpValueKey)
{
	unsigned char buffer[255]={0};
	unsigned long length;
	unsigned long type;
	HKEY hKey;
	LPCTSTR data_Set= _T("SOFTWARE\\Microsoft");

	RegOpenKey(HKEY_LOCAL_MACHINE,data_Set,&hKey);
	RegQueryValueEx(hKey,lpValueKey,NULL,&type,buffer,&length);
	RegCloseKey(hKey);

	CString ret;
	ret = Char2CString((char *)buffer);

	int Count;
	Count = atoi((const char*)buffer);
	if(Count == 0)
	{
		RegCreateKey(HKEY_LOCAL_MACHINE,data_Set,&hKey);
		RegSetValueEx(hKey,lpValueKey,0,REG_SZ,(const unsigned char *) "0",1);
		RegCloseKey(hKey);

		return "0";
	}

	return ret;
}

/// <summary>
/// 设置注册表值
/// </summary>
/// <param name="lpValueKey">键名</param>
/// <param name="lpValueName">键值</param>
/// <returns></returns>
void CCommon::SetReg(CString lpValueKey, CString lpValueName)
{
	unsigned char buffer[255]={0};
	unsigned long length;
	unsigned long type;
	HKEY hKey;
	LPCTSTR data_Set= _T("SOFTWARE\\Microsoft");

	RegOpenKey(HKEY_LOCAL_MACHINE,data_Set,&hKey);
	RegQueryValueEx(hKey,lpValueKey,NULL,&type,buffer,&length);
	RegCreateKey(HKEY_LOCAL_MACHINE,data_Set,&hKey);
	RegSetValueEx(hKey,lpValueKey,0,REG_SZ,(const unsigned char *) CString2Char(lpValueName),lpValueName.GetLength());
	RegCloseKey(hKey);
}

CRect CCommon::SetDrawSize(CStatic * m_picBox,CRect old_DrawRect,int bmpw,int bmph,float *scale) 
{
	//////////////重设矩形框 否则出现未刷新BUG

	CRect rect;

	// 读取图片的宽和高
	int w = bmpw;
	int h = bmph;
	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;
	// 计算将图片缩放到TheImage区域所需的比例因子
	(*scale) = 1.0;
	int rw ,rh;
	rw = old_DrawRect.Width();// 求出图片控件的宽和高
	rh = old_DrawRect.Height();
	(*scale) = (float)rh/h <(float) rw/w ?(float)rh/h:(float) rw/w;
	// 缩放后图片的宽和高
	int nw = (int)( w* (*scale) );
	int nh = (int)( h*(*scale) );
	int iw = nw;//img->width;						// 读取图片的宽和高
	int ih = nh;//img->height;
	int tx = (int)(rw - iw)/2;					// 使图片的显示位置正好在控件的正中
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );

	CRect new_clientRect;
	new_clientRect.left	=	old_DrawRect.left+rect.left;
	new_clientRect.top	=	old_DrawRect.top+rect.top;
	new_clientRect.bottom	=	new_clientRect.top+rect.Height();
	new_clientRect.right	=	new_clientRect.left+rect.Width();

	//重置矩形
	m_picBox->MoveWindow(new_clientRect);

	//重置绘图矩形
	SetRect( rect, 0, 0, iw, ih );

	return rect;

}

/************************************
* 绘图
*************************************/
void CCommon::DrawCtrlImage(CStatic * m_picBox, BITMAPINFO bmpInfo,
							char * buffer, int bufferSize,
							int face_Count, CRect *face_Rect_List, 
							CRect rect,float scale)
{
	CRect tempRect;
	CDC *pDC = m_picBox->GetDC();
	if(NULL == pDC)
		return;
	HDC hdc = pDC->GetSafeHdc();
	HDC hdcMem = CreateCompatibleDC(hdc); 

	LPBYTE   lpBits;
	HBITMAP hBmp = CreateDIBSection(hdcMem,&bmpInfo,DIB_RGB_COLORS,(void **)&lpBits,NULL,0);  
	memcpy(lpBits, buffer, bufferSize);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hBmp); 

	
	CBrush tempBrush(RGB(0,255,0));

	//	free(lpBits);
	for(int i=0;i<face_Count;i++)
	{
		tempRect.left = face_Rect_List[i].left-1;
		tempRect.top  = face_Rect_List[i].top-1;
		tempRect.bottom = face_Rect_List[i].bottom+1;
		tempRect.right  = face_Rect_List[i].right+1;


		//FILE *fp = fopen("D:\\rect.txt","a+");
		//if(fp)
		//{
		//	fprintf(fp,"************************scale = %f*********************************\n",scale);
		//	fprintf(fp,"left = %d\ntop = %d\nbottom = %d\nright=%d\n",face_Rect_List[i].left,face_Rect_List[i].top,face_Rect_List[i].bottom,face_Rect_List[i].right);
		//	fprintf(fp,"left = %d\ntop = %d\nbottom = %d\nright=%d\n",rect.left,rect.top,rect.bottom,rect.right);
		//	fprintf(fp,"left = %d\ntop = %d\nbottom = %d\nright=%d\n",tempRect.left,tempRect.top,tempRect.bottom,tempRect.right);
		//	fprintf(fp,"**************************************************************\n");
		//	fclose(fp);
		//}
		//pDC->FrameRect(rect, &pen);
		FrameRect(hdcMem, face_Rect_List[i], (HBRUSH)tempBrush);
		FrameRect(hdcMem, tempRect, (HBRUSH)tempBrush);

		tempRect.left = face_Rect_List[i].left-2;
		tempRect.top  = face_Rect_List[i].top-2;
		tempRect.bottom = face_Rect_List[i].bottom+2;
		tempRect.right  = face_Rect_List[i].right+2;

		FrameRect(hdcMem, tempRect, (HBRUSH)tempBrush);

		tempRect.left = face_Rect_List[i].left-3;
		tempRect.top  = face_Rect_List[i].top-3;
		tempRect.bottom = face_Rect_List[i].bottom+3;
		tempRect.right  = face_Rect_List[i].right+3;

		FrameRect(hdcMem, tempRect, (HBRUSH)tempBrush);
	}


	char str[128]="";
	if(face_Count > 0)
	{
		strcpy(str,"人脸捕获中，请保持正视");
		SetTextColor(hdcMem,RGB(0,0,255));
	}
	else
	{
		strcpy(str,"未检测到人脸，请正视摄像头");
		SetTextColor(hdcMem,RGB(255,0,0));
	}
	CFont font;
	VERIFY(font.CreateFont(
		28/scale,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_HEAVY,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename

	// Do something with the font just created...
	SetBkMode(hdcMem,TRANSPARENT);

	HGDIOBJ oldfont=SelectObject(hdcMem,font);

	DrawText(hdcMem,str,-1,CRect(0,-bmpInfo.bmiHeader.biHeight+20/scale,bmpInfo.bmiHeader.biWidth,bmpInfo.bmiHeader.biHeight),DT_SINGLELINE|DT_CENTER|DT_VCENTER); 

	//SetStretchBltMode(hdc, HALFTONE);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchBlt(hdc, rect.left,rect.top,rect.Width(),rect.Height(), hdcMem, 0, 0, bmpInfo.bmiHeader.biWidth, bmpInfo.bmiHeader.biHeight, SRCCOPY);	

	SelectObject(hdcMem,oldfont);

	SelectObject(hdcMem,hOldBmp);//复原兼容DC数据.
	DeleteDC(hdcMem);
	DeleteObject(hOldBmp);
	DeleteObject(hBmp);


	if(NULL != pDC)
	{
		m_picBox-> ReleaseDC(pDC);
	}
}

int CCommon::GetCamNum()
{
	int cam_count = GetCameraCount();//仅仅获取摄像头数目
	return cam_count;
}

void CCommon::DrawFaceImageMin(CStatic *m_picBox,unsigned char *image,unsigned long int size)
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
			pPic->Render(*pDC,4,4,rect.Width()-8,rect.Height()-8,0,hmHeight,hmWidth,-hmHeight,NULL); 
			pPic->Release(); 
			pStream->Release();//释放数据流
		
		}  
	}

	if(hGlobal)
	{
		GlobalFree(hGlobal);
		hGlobal = NULL;
	}
	m_picBox-> ReleaseDC(pDC);
}

void CCommon::DrawFaceImage(CStatic *m_picBox,CBitmap    *cbitmap,    BITMAP *bitbmp)
{
	CRect rect; 
	m_picBox->GetClientRect(&rect);//获得pictrue控件所在的举行区域
	CDC *pDC=m_picBox->GetDC();//获得pictrue控件的DC 

	CDC    memdc;     
	memdc.CreateCompatibleDC(pDC);     
	memdc.SelectObject(cbitmap); 
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bitbmp->bmWidth,bitbmp->bmHeight,SRCCOPY);
	memdc.DeleteDC();

	m_picBox-> ReleaseDC(pDC);
}