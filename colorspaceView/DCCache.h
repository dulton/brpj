#ifndef __YUAN_CACHE_H__
#define __YUAN_CACHE_H__

#define YF_RGBA_8(r, g, b, a) (unsigned int)(((unsigned int)((unsigned char)(b))) | ((unsigned int)(((unsigned char)(g))<<8)) | ((unsigned int)(((unsigned char)(r))<<16)) | ((unsigned int)(((unsigned char)(a))<<24)))

// 定义颜色结构体
typedef struct tagRGBDATATYPE
{
	DWORD dwRed; // 红
	DWORD dwGreen; // 绿
	DWORD dwBlue; // 蓝
}RGBDATATYPE;

//bmp格式类型，BI_BITFIELDS类型需要RGBDATATYPE
typedef struct tagBMPINFOTYPE
{
	BITMAPINFOHEADER bmiHeader;
	RGBDATATYPE bmiColors[1];
}BMPINFOTYPE, *PBMPINFOTYPE;

/*
此文件用来创建双缓存类
*/

class CDCCache
{
public:
	CDCCache();
	~CDCCache();
	// 创建缓存DC
	bool CreateCache(int nWidth, int nHeight, int nBitCount);
	// 释放缓存DC
	void FreeCache(void);
	// 获取缓存指针
	unsigned char *GetCacheBuffer(void);
	// 获取缓存DC
	CDC *GetCacheDC(void);
	// 在缓存DC上画一个框
	void DrawRectangle(int x, int y, int w, int h, int nBold, unsigned int nColor = YF_RGBA_8(255, 0, 0, 255));
	
	CDC m_MemDC; //定义一个内存设备描述表对象（即后备缓冲区）

private:
	int m_nBitCount; // 颜色位数
	int m_nWidth; // 内存块宽
	int m_nHeight; // 内存块高
	unsigned char *m_pBuffer; // 内存指针
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	 CBitmap m_pBitmap;
	bool m_bCreate; // 是否需要创建cdc
};

#endif /* __YUAN_CACHE_H__ */