#if !defined(AFX_DLGCARDETECTCONFIG_H__65A2B477_F180_4407_A37C_05A33FC4413A__INCLUDED_)
#define AFX_DLGCARDETECTCONFIG_H__65A2B477_F180_4407_A37C_05A33FC4413A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSetCar.h : header file
//

#include "CarDetect.h"
#if 0

//每个车牌最多多少个字符
#define CAR_STR_MAX 32
//车的省份
#define CAR_MASK_MAX 31

typedef struct
{
	short x0;	// 左
	short x1;	// 右
	short y0;	// 上
	short y1;	// 下
}Rects;

	////////////////////////////////////////////////////////
//基本设置
struct CarSetStruct
{
	//截图车牌带红框 0=不带 1=带
	int RedRect;
	//车牌倾斜校正使能标志 0:不校正，1:校正
	int Deskew;
	//JPG图片质量 75
	int JPGquality;
	//视频图像宽
	int Width;
	//视频图像高
	int Hight;
	//车牌最小宽度 60
	int MinWidth;
	//车牌最大宽度 180
	int MaxWidth;
	//检测范围 比例尺 
	Rects Range;
	//检测范围 比例尺 
	Rects RangeRate;
	//默认省份字符  
	//省份汉字：云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑
	char DefaultChar[CAR_STR_MAX];
	//31个省 标志位
	//云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑 (省份)
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	unsigned char Mask[CAR_MASK_MAX];
	//汉字置信度 75
	int Reliability;
	//车身颜色识别标志，0:不识别，1:识别
	int CarColor;
};
#endif
/////////////////////////////////////////////////////////////////////////////
// CDLGSetCar dialog

class CDLGSetCar : public CDialog
{
// Construction
public:
	struct CarSetStruct CarSet;
	void ReadOldConfig(struct CarSetStruct OldCarSet);
public:
	CDLGSetCar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSetCar)
	enum { IDD = IDD_SET_CAR };
	CString	m_defaultProvince;
	int		m_reliability;
	int		m_JPGquality;
	int		m_RectDown;
	int		m_RectLeft;
	int		m_RectRight;
	int		m_RectUp;
	int		m_MinWidth;
	int		m_MaxWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSetCar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSetCar)
	virtual void OnOK();
	afx_msg void OnCheckAll();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCARDETECTCONFIG_H__65A2B477_F180_4407_A37C_05A33FC4413A__INCLUDED_)
