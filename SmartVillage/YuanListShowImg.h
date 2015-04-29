#ifndef __YUAN_LIST_SHOWIMG_H__
#define __YUAN_LIST_SHOWIMG_H__

/*
* 此类主要时实现往list的item中插入jpg图片
*/
class CYuanListShowImg
{
public:
	CYuanListShowImg();
	~CYuanListShowImg();
	/*
	* 传入数据
	*/
	bool Init(CListCtrl *pLctrl, int nImgWidth, int nImgHeight, CBitmap *pDefaultBmp, int nColorType = ILC_COLOR24);
	/*
	* 插入列表(也可以在外部创建)
	*/
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT,  int nWidth = -1, int nSubItem = -1);

	/*
	* 添加一张图片
	*/
	bool AddImg(char *pImgPath);
	/*
	* 设置默认图片
	*/
	BOOL SetDefaultBitmap(CBitmap *pBmp);
	/*
	* 删除指定图片(删除后，后面的会往前移，会错乱)
	*/
	BOOL RemoveImg(int nIndex);
	/*
	* 清空图片数据
	*/
	void ClearImageList(void);
	/*
	* 插入图片item(正常nItem和nImgIndex是一致的)
	*/
	int InsertItem(int nItem, int nImgIndex);
	/*
	* 插入item数据（第0列默认为图片列，不能插入数据)
	*/
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	/*
	* 交换列表框中的两列位置
	*/
	void SwapColumns(int c1, int c2);
	/*
	* 返回指定大小的CBitmap（外部释放空间)
	*/
	CBitmap *GetScaleBitmap(HBITMAP bmp, int w, int h);
	/////////////以下的可以不用///////////////
	/*
	* 让list 和imagelist关联起来
	*/
	void ListComImageList(void);
	/*
	* 设置风格
	*/
	int SetExtendedStyle(int nNewStyle);
	/*
	* 获取风格
	*/
	int GetExtendedStyle(void);
	/*
	* 风格置0
	*/
	void ReSetExtendedStyle(int nNewStyle = 0);
private:
	CListCtrl *m_plcShow; // 列表指针
	CImageList m_imList; // 图片列表指针
	int m_nImgWidth;
	int m_nImgHeight;
	CBitmap *m_pDefaultBmp;
	int m_nColorType;
};
#endif /* __YUAN_LIST_SHOWIMG_H__ */