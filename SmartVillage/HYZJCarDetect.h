#ifndef _HYZJ_CAR_DETECT_H_
#define _HYZJ_CAR_DETECT_H_

#define VIDEO_WIDTH     1920
#define VIDEO_HEIGHT    1080
//WAV声音
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib") 


#define HY_MAX_CAR_LIST_NUM 100

//汽车
#include "HYZJcarSDK\\LPKernelEx.h"

#if OPEN_HYZJ_CARDETECT_CODE
//汽车
#pragma comment(lib, "HYZJcarSDK\\LPKernelEx.lib")

#endif



//一张图里最多多少个汽车
#define HYZJ_CAR_STRUCT_MAX 32
//每个车牌最多多少个字符
#define HYZJ_CAR_STR_MAX 32
//车的省份
#define HYZJ_CAR_MASK_MAX 31
	////////////////////////////////////////////////////////

typedef struct
{
	int x0;	// 左
	int x1;	// 右
	int y0;	// 上
	int y1;	// 下
}Recti;

//基本设置
struct HYZJCarSetStruct
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
	Recti Range;
	//检测范围 比例尺 
	Recti RangeRate;
	//默认省份字符  
	//省份汉字：云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑
	char DefaultChar[HYZJ_CAR_STR_MAX];
	//31个省 标志位
	//云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑 (省份)
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	unsigned char Mask[HYZJ_CAR_MASK_MAX];
	//汉字置信度 75
	int Reliability;
	//车身颜色识别标志，0:不识别，1:识别
	int CarColor;
	//夜间模式
	int Night;
};

	////////////////////////////////////////////////////////
class CHYZJCarDetect
{
public:
	CHYZJCarDetect();
	virtual ~CHYZJCarDetect();


	void SetRange();
	char* CarType(int i);

char* CHYZJCarDetect::CarColor(int i);
char* CHYZJCarDetect::CarDirection(int i);

	void CleanList(void);

	void Stop();
	bool Start(int format,unsigned char *image,int w,int h,int size);
	int Result();


	////////////////////////////////////////////////////////
	void LimitMessageBox(void);
	void errorprintf(char *str);
	bool ErrorPlateName(char *name);



	char* CHYZJCarDetect::CarColor2JingAo(int i);
	char* CHYZJCarDetect::CarType2JingAo(int i);

public:
	int CarHandle;
	//句柄申请是否成功 1=成功。 0为失败 -1为未申请
	int State;

	//本帧车辆信息
	int CarTotal;
	TH_PlateResultImage CarInfo[HYZJ_CAR_STRUCT_MAX];

	TH_PlateResult PlateResultReal[HYZJ_CAR_STRUCT_MAX];



	long JpgSize;
	unsigned char *JpgSmall;
	long JpgSmallSize;


	//基本设置
	struct HYZJCarSetStruct CarSet;
	TH_RECT CarSetrcRange ;

	////////////////////////////////////////////////////////
	//数据库
	int     m_playhandle;  //播放句柄
	char	cam_name[256];	//摄像头名称
	char	l_ipaddr[64];	//IP地址
	unsigned long int	camid;			//摄像头NID
	
	int cam_Direction;


	bool JumpJPG;	//弹出图片 外部调用。防止亚安画面无法显示
	char JumpJPGpath[260];

	//启用标记
//	int ElcarDetectFlag;
//	int CarDetectFlag;

};


#endif