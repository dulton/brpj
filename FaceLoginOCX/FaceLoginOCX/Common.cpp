#include "StdAfx.h"
#include "Common.h"

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
			str.Format("[%d] %s",i,str_camera_name);
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


/************************************
* 绘图
*************************************/
void CCommon::DrawCtrlImage(CStatic * m_picBox, BITMAPINFO bmpInfo, char * buffer, int bufferSize)
{
	CRect   rect;
	
	CDC *pDC = m_picBox->GetDC();
	if(NULL == pDC)
		return;
	HDC hdc = pDC->GetSafeHdc();
	HDC hdcMem = CreateCompatibleDC(hdc); 
	m_picBox->GetClientRect(&rect);

	LPBYTE   lpBits;
	HBITMAP hBmp = CreateDIBSection(hdcMem,&bmpInfo,DIB_RGB_COLORS,(void **)&lpBits,NULL,0);  
	memcpy(lpBits, buffer, bufferSize);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hBmp); 

	// 读取图片的宽和高
	int w = bmpInfo.bmiHeader.biWidth;
	int h = bmpInfo.bmiHeader.biHeight;
	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;
	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = 0.0;
	int rw ,rh;
	rw = rect.Width();// 求出图片控件的宽和高
	rh = rect.Height();
	scale = (float)rh/h <(float) rw/w ?(float)rh/h:(float) rw/w;
	// 缩放后图片的宽和高
	int nw = (int)( w*scale );
	int nh = (int)( h*scale );
	int iw = nw;//img->width;						// 读取图片的宽和高
	int ih = nh;//img->height;
	int tx = (int)(rw - iw)/2;					// 使图片的显示位置正好在控件的正中
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	//SetStretchBltMode(hdc, HALFTONE);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchBlt(hdc, rect.left,rect.top,rect.Width(),rect.Height(), hdcMem, 0, 0, bmpInfo.bmiHeader.biWidth, bmpInfo.bmiHeader.biHeight, SRCCOPY);	

	SelectObject(hdcMem,hOldBmp);//复原兼容DC数据.
	DeleteDC(hdcMem);
	DeleteObject(hOldBmp);
	DeleteObject(hBmp);
//	free(lpBits);

	rect.left=100;
	rect.top=100;
	rect.bottom=200;
	rect.right=200;
	CBrush tempBrush(RGB(0,255,0));
	pDC->FrameRect(rect, &tempBrush);

	rect.left=150;
	rect.top=150;
	rect.bottom=250;
	rect.right=250;
	pDC->FrameRect(rect, &tempBrush);

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