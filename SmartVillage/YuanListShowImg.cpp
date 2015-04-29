#include "stdafx.h"
#include "YuanListShowImg.h"
#include "Picture.h"

CYuanListShowImg::CYuanListShowImg()
{
	m_plcShow = NULL;
	m_nImgWidth = 0;
	m_nImgHeight = 0;
	m_nColorType = ILC_COLOR24;
	m_pDefaultBmp = NULL;
}
CYuanListShowImg::~CYuanListShowImg()
{
	ClearImageList();
}
/*
* 外部只需要定义一个CListCtrl直接传就可以
*/
bool CYuanListShowImg::Init(CListCtrl *pLctrl, int nImgWidth, int nImgHeight, CBitmap *pDefaultBmp, int nColorType)
{
	m_plcShow = pLctrl;
	m_nImgWidth = nImgWidth;
	m_nImgHeight = nImgHeight;
	m_nColorType = nColorType;
	m_pDefaultBmp = pDefaultBmp;
	// 设置CListCtrl控件扩展风格(网络线、整行选中、图片)  
	DWORD dwStyle;  
	dwStyle = m_plcShow->GetExtendedStyle();  
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES;  // 要显示图片必须加LVS_EX_SUBITEMIMAGES
	m_plcShow->SetExtendedStyle(dwStyle);
	//Bitmap图片(如果图片大小不统一，这里应该设置为最小的，不然有可能图片加载不成功)  
	m_imList.Create(m_nImgWidth, m_nImgHeight, m_nColorType, 0, 4);  

	// 设置图像列表与CListCtrl控件关联，要使用LVSIL_SMALL风格  
	m_plcShow->SetImageList(&m_imList,LVSIL_SMALL ); //这里要使用，LVSIL_SMALL风格
	return true;
}

/*
* 插入列表
*/
int CYuanListShowImg::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat,  int nWidth, int nSubItem)
{
	return m_plcShow->InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}
/*
* 设置风格
*/
int CYuanListShowImg::SetExtendedStyle(int nNewStyle)
{
	return m_plcShow->SetExtendedStyle(nNewStyle);
}
/*
* 获取风格
*/
int CYuanListShowImg::GetExtendedStyle(void)
{
	return m_plcShow->GetExtendedStyle();
}
/*
* 风格置0
*/
void CYuanListShowImg::ReSetExtendedStyle(int nNewStyle)
{
	m_plcShow->SetExtendedStyle(nNewStyle);
}
/*
* 添加一张图片
*/
bool CYuanListShowImg::AddImg(char *pImgPath)
{
	{
	//	GetScaleBitmap(pImgPath, m_nImgWidth, m_nImgHeight);
	}
	int bef = m_imList.GetImageCount();
	
	CPicture pic;
	int nRet = 0;
	CBitmap * pBmp = NULL; 

	/*pBmp = new CBitmap();  
	HBITMAP bmp = NULL;
	bmp = pic.LoadPicture(pImgPath);
	nRet = pBmp->Attach(bmp);*/

	//必须放外面。不然指针会被销毁导致不显示图案
	HBITMAP bmp = NULL;
	bmp = pic.LoadPicture(pImgPath);

	pBmp = GetScaleBitmap(bmp, m_nImgWidth, m_nImgHeight);
	if(NULL == pBmp)
	{
		// 插入失败时，必须插入一张默认的图片，不然后面的图片序号会往上移动
		// 导致图片显示错乱
		nRet = m_imList.Add(m_pDefaultBmp,RGB(0,0,0));  
		return true;
	}
	
	nRet = m_imList.Add(pBmp,RGB(0,0,0));  

	if (nRet == -1)
	{
		// 插入失败时，必须插入一张默认的图片，不然后面的图片序号会往上移动
		// 导致图片显示错乱
		nRet = m_imList.Add(m_pDefaultBmp,RGB(0,0,0));  
	}
	else
	{
		int aft = m_imList.GetImageCount();
		if(bef==aft)
		{
			nRet = m_imList.Add(m_pDefaultBmp,RGB(0,0,0));
		}
	}
	delete pBmp; 

	if (nRet == -1)
		return false;
	return true;
}
/*
* 设置默认图片
*/
BOOL CYuanListShowImg::SetDefaultBitmap(CBitmap *pBmp)
{
	m_pDefaultBmp = pBmp;
	return TRUE;
}
/*
* 删除指定图片
*/
BOOL CYuanListShowImg::RemoveImg(int nIndex)
{
	if (nIndex >= m_imList.GetImageCount())
		return FALSE;
	return m_imList.Remove(nIndex);
}
/*
* 清空图片数据
*/
void CYuanListShowImg::ClearImageList(void)
{
	int nCount = m_imList.GetImageCount();
	int i = 0;
	for (i = nCount-1; i >= 0; i--)
	{
		m_imList.Remove(i);
	}
}
/*
* 插入图片item
*/
int CYuanListShowImg::InsertItem(int nItem, int nImgIndex)
{

	LVITEM lvItem={0};
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;  //文字、图片、状态  
	lvItem.iItem = nItem;       //行号(第一行)  
	lvItem.iImage = nImgIndex;  //图片索引号(第一幅图片)  
	lvItem.iSubItem = 0;    //子列号  
	//第一列为图片
	return m_plcShow->InsertItem(&lvItem);
}
/*
* 插入item数据（第0列默认为图片列，不能插入数据)
*/
BOOL CYuanListShowImg::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	return m_plcShow->SetItemText(nItem, nSubItem, lpszText); 
}
/*
* 交换列表框中的两列位置
*/
void CYuanListShowImg::SwapColumns(int c1, int c2)
{
	CHeaderCtrl *pmyHeaderCtrl = m_plcShow->GetHeaderCtrl();
	int   nCount   =   pmyHeaderCtrl->GetItemCount();   
	LPINT   pnOrder   =   (LPINT)   malloc(nCount*sizeof(int));   
	ASSERT(pnOrder   !=   NULL);   

	pmyHeaderCtrl->GetOrderArray(pnOrder,   nCount);   
	int nTemp;
	nTemp     = pnOrder[c1];
	pnOrder[c1]        =   pnOrder[c2];  
	pnOrder[c2] = nTemp;

	pmyHeaderCtrl->SetOrderArray(nCount,   pnOrder);   
	free(pnOrder);
}
/*
* 返回指定大小的CBitmap（外部释放空间)
*/
CBitmap *CYuanListShowImg::GetScaleBitmap(HBITMAP bmp,int w, int h)
{
	CPicture pic;

	if(NULL== bmp)
		return NULL;

	CBitmap *pBitmap = new CBitmap();
	pBitmap->Attach(bmp);
	BITMAP bitInfo = {0};
	pBitmap->GetBitmap(&bitInfo);


	// 大小符合就不缩放
	if (bitInfo.bmWidth == w && bitInfo.bmHeight == h)
		return pBitmap;

	CDC memdc;
	memdc.CreateCompatibleDC(NULL);     
	memdc.SelectObject(pBitmap);

	CDC m_dcCompatible;
	m_dcCompatible.CreateCompatibleDC(&memdc); // 此函数参数DC必须赋值
	CBitmap *pTemp = new CBitmap();
	pTemp->CreateCompatibleBitmap(&memdc, w, h); // 此函数参数DC必须赋值
	m_dcCompatible.SelectObject(pTemp);
		
	m_dcCompatible.SetStretchBltMode(COLORONCOLOR);
	m_dcCompatible.StretchBlt(0, 0, w, h, &memdc,0, 0, 
		bitInfo.bmWidth, bitInfo.bmHeight, SRCCOPY);
	m_dcCompatible.DeleteDC();
	memdc.DeleteDC();
	delete pBitmap;

	return pTemp;
}
/*
* 让list 和imagelist关联起来
*/
void CYuanListShowImg::ListComImageList(void)
{
	// 设置图像列表与CListCtrl控件关联，要使用LVSIL_SMALL风格  
	m_plcShow->SetImageList(&m_imList,LVSIL_SMALL ); //这里要使用，LVSIL_SMALL风格
}