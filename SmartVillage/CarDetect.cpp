#include "stdafx.h"
#include "SmartVillage.h"

#include "CarDetect.h"
//////////////////////////


//////////////////////
#include "SmartVillageDlg.h"
extern CSmartVillageDlg *DlgMain;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

#include "YRSVMySQL.h"
extern CYRSVMySQL MySqlIO;

#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "JingAoUpload.h"


#if OPEN_LC_CARDETECT_CODE

CCarDetect::CCarDetect()
{
	CarHandle=NULL;
	State=-1;

	CarTotal=0;
	PicFlag=false;
	Jpg=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
	JpgSize=0;
	JpgSmall=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
	JpgSmallSize=0;


	int i;
	for(i=0;i<CAR_STRUCT_MAX;i++)
	{
		//	CarInfo[i].SmallJpg=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
		//	CarInfo[i].SmallJpgSize=0;
	}

	//	ElcarDetectFlag=1;
	//	CarDetectFlag=1;
	//截图车牌带红框 0=不带 1=带
	CarSet.RedRect=1;
	//车牌倾斜校正使能标志 0:不校正，1:校正
	CarSet.Deskew=1;
	//JPG图片质量 75
	CarSet.JPGquality=75;
	//车牌最小宽度 60
	CarSet.MinWidth=60;
	//车牌最大宽度 180
	CarSet.MaxWidth=180;

	CarSet.RangeRate.x0=0;
	CarSet.RangeRate.x1=100;
	CarSet.RangeRate.y0=10;
	CarSet.RangeRate.y1=90;
	//默认省份字符  
	//省份汉字：云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑
	memset(CarSet.DefaultChar,0,CAR_STR_MAX);
	memcpy(CarSet.DefaultChar,"闽",CAR_STR_MAX);
	//31个省 标志位
	//0: 开启该汉字识别 1: 屏蔽该汉字识别
	//云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑 (省份)
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1
	for(i=0;i<CAR_MASK_MAX;i++)
		CarSet.Mask[i]=1;
	CarSet.Mask[26]=0;

	//汉字置信度 75
	CarSet.Reliability=50;
	//车身颜色识别标志，0:不识别，1:识别
	CarSet.CarColor=1;

	/////////////////////////////////////
	cam_name[0]=0;
	l_ipaddr[0]=0;	

	JumpJPG=false;
	cam_Direction=0;
}

CCarDetect::~CCarDetect()
{
	if(1==State)
		lc_plate_analysis_destroy(CarHandle);

	if(Jpg)
	{
		free(Jpg);
		Jpg=NULL;

	}
	if(JpgSmall)
	{
		free(JpgSmall);
		JpgSmall=NULL;
	}

	/*
	int i;
	for(i=0;i<CAR_STRUCT_MAX;i++)
	free(CarInfo[i].SmallJpg);
	*/

}



void CCarDetect::SetRange()
{
	int i;
	//	char j[32]={0};
	if(1==State)
	{
		CarSet.Range.x0=(int16_t)(CarSet.Width*(CarSet.RangeRate.x0/100.0));
		CarSet.Range.x1=(int16_t)(CarSet.Width*(CarSet.RangeRate.x1/100.0));
		CarSet.Range.y0=(int16_t)(CarSet.Hight*(CarSet.RangeRate.y0/100.0));
		CarSet.Range.y1=(int16_t)(CarSet.Hight*(CarSet.RangeRate.y1/100.0));

		if(CarSet.Range.x0<3)
			CarSet.Range.x0=3;
		if(CarSet.Range.x1 > (CarSet.Width-3))
			CarSet.Range.x1=(CarSet.Width-3);

		if(CarSet.Range.y0<3)
			CarSet.Range.y0=3;
		if(CarSet.Range.y1 > (CarSet.Hight-3))
			CarSet.Range.y1=(CarSet.Hight-3);

		i=lc_plate_set_rect_detect(CarHandle,CarSet.Range);
		/*
		if(LC_PLATE_SUCCESS!=i)
		{
		sprintf(j,"%d",i);
		m_maindlg->SetWindowText(j);
		}
		*/
	}
}

void CCarDetect::SetRedRect()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_position_flag(CarHandle,CarSet.RedRect))
			errorprintf("ERROR set_position");
	}
}

void CCarDetect::SetDeskew()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_deskew_flag(CarHandle,CarSet.Deskew))
			errorprintf("ERROR set_deskew_flag");
	}
}
void CCarDetect::SetMinMaxWidth()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_plate_width(CarHandle,CarSet.MinWidth,CarSet.MaxWidth))
			errorprintf("ERROR set_plate");
	}
}

void CCarDetect::SetMask()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_chinese_mask(CarHandle,CarSet.Mask))
			errorprintf("ERROR set_chinese_mask");
	}
}

void CCarDetect::SetDefaultChar()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_chinese_default(CarHandle,CarSet.DefaultChar))
			errorprintf("ERROR set_chinese_default");
	}
}

void CCarDetect::SetReliability()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_chinese_trust_thresh(CarHandle,CarSet.Reliability))
			errorprintf("ERROR set_chinese_trust");
	}
}

void CCarDetect::SetJPGquality()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_image_quality(CarHandle,CarSet.JPGquality))
			errorprintf("ERROR set_image_quality");
	}
}

void CCarDetect::SetCarColor()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_car_color_enable(CarHandle,CarSet.CarColor))
			errorprintf("ERROR set_car_color");
	}
}


void CCarDetect::SetAll()
{
	//第一次
	if(1==State)
	{
		SetRedRect();
		SetRange();
		SetDeskew();
		SetMinMaxWidth();
		SetMask();
		SetDefaultChar();
		SetReliability();
		SetJPGquality();
		SetCarColor();
	}
}
/*
pchannel->cardetect.Start(LC_VIDEO_FORMAT_I420,\
pchannel->image,pframeinfo->width,pframeinfo->height,pframeinfo->width*pframeinfo->height*3/2);

//不开线程
pchannel->cardetect.Result();
*/

void CCarDetect::errorprintf(char *str)
{
	char tempstr[260]={0};
	sprintf(tempstr,"窗口号=%d,摄像头名称=%s,IP地址=%s,%s\n",m_playhandle,cam_name,l_ipaddr,str);
	DlgMain->ShowRuntimeMessage(tempstr,0);
}

void CCarDetect::LimitMessageBox(void)
{
#if ALLTAB_DETECT_CAR_MODE

	uint32_t limit=0;
	char str[32]={0};
	if(LC_PLATE_SUCCESS == lc_plate_get_limit_thread_num(&limit))
	{
		sprintf(str,"最大车牌识别限制=%d个",limit);
		errorprintf(str);
	}

#endif 

}

//开始画面
void CCarDetect::StartPIC(int format,unsigned char *image,int w,int h,int size)
{
		CarSet.Width=w;
		CarSet.Hight=h;
	//第一次
	if(-1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_analysis_create(&CarHandle,	\
			LC_PLATE_PROC_WAIT_PIC,CarSet.Width,CarSet.Hight,NULL,NULL))
		{	
			State=0;
			LimitMessageBox();
			errorprintf("当前车牌识别无法开启。线程数达到最大");
			return ;
		}
		else
			State=1;

		errorprintf("识别创建成功");

		if(LC_PLATE_SUCCESS!=lc_plate_set_video_format(CarHandle,format))
			return ;

		struct DEVICE_SET_ST CamSet={0};

		MySqlIO.DEVICE_SetTable_Read(camid,CamSet);

		//后面从数据库读取，以及保存。车牌识别CPP文件也要加读取
		CarSet.CarColor=CamSet.car.carlor;
		CarSet.Deskew=CamSet.car.deskew;
		CarSet.RedRect=CamSet.car.redrect;
		CarSet.JPGquality=CamSet.car.jpgquailty;
		CarSet.MinWidth=CamSet.car.minwidth;
		CarSet.MaxWidth=CamSet.car.maxwidth;
		CarSet.RangeRate.x0=CamSet.car.left;
		CarSet.RangeRate.x1=CamSet.car.right;
		CarSet.RangeRate.y0=CamSet.car.top;
		CarSet.RangeRate.y1=CamSet.car.bottom;
		strcpy(CarSet.DefaultChar,CamSet.car.defaultchar);
		CarSet.Reliability=CamSet.car.reliability;
		memcpy(CarSet.Mask,CamSet.car.mask,CAR_MASK_MAX);

		CarSet.Width=w;
		CarSet.Hight=h;

		SetRedRect();
		SetRange();
		SetDeskew();
		SetMinMaxWidth();
		SetMask();
		SetDefaultChar();
		SetReliability();
		SetJPGquality();
		SetCarColor();

		/*
		lc_plate_set_OSD_scale(CarHandle,1.0);
		lc_plate_set_OSD_position(CarHandle,0,0);
		lc_plate_set_OSD_flag(CarHandle,1);
		*/
		if(LC_PLATE_SUCCESS!=lc_plate_analysis(&CarHandle,image,w,h,size))
			return ;
	}
	else if(1==State)//N次以后
	{
		CarSet.Width=w;
		CarSet.Hight=h;

		if(LC_PLATE_SUCCESS!=lc_plate_analysis(&CarHandle,image,w,h,size))
			return ;
	}

	//如果创建失败就不做任何事
}
//开始画面
void CCarDetect::Start(int format,unsigned char *image,int w,int h,int size)
{
		CarSet.Width=w;
		CarSet.Hight=h;
	//第一次
	if(-1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_analysis_create(&CarHandle,	\
			LC_PLATE_PROC_WAIT_VIDEO,CarSet.Width,CarSet.Hight,NULL,NULL))
		{	
			State=0;
			LimitMessageBox();
			errorprintf("当前车牌识别无法开启。线程数达到最大");
			return ;
		}
		else
			State=1;

		errorprintf("识别创建成功");

		if(LC_PLATE_SUCCESS!=lc_plate_set_video_format(CarHandle,format))
			return ;

		struct DEVICE_SET_ST CamSet={0};

		MySqlIO.DEVICE_SetTable_Read(camid,CamSet);

		//后面从数据库读取，以及保存。车牌识别CPP文件也要加读取
		CarSet.CarColor=CamSet.car.carlor;
		CarSet.Deskew=CamSet.car.deskew;
		CarSet.RedRect=CamSet.car.redrect;
		CarSet.JPGquality=CamSet.car.jpgquailty;
		CarSet.MinWidth=CamSet.car.minwidth;
		CarSet.MaxWidth=CamSet.car.maxwidth;
		CarSet.RangeRate.x0=CamSet.car.left;
		CarSet.RangeRate.x1=CamSet.car.right;
		CarSet.RangeRate.y0=CamSet.car.top;
		CarSet.RangeRate.y1=CamSet.car.bottom;
		strcpy(CarSet.DefaultChar,CamSet.car.defaultchar);
		CarSet.Reliability=CamSet.car.reliability;
		memcpy(CarSet.Mask,CamSet.car.mask,CAR_MASK_MAX);

		CarSet.Width=w;
		CarSet.Hight=h;

		SetRedRect();
		SetRange();
		SetDeskew();
		SetMinMaxWidth();
		SetMask();
		SetDefaultChar();
		SetReliability();
		SetJPGquality();
		SetCarColor();

		/*
		lc_plate_set_OSD_scale(CarHandle,1.0);
		lc_plate_set_OSD_position(CarHandle,0,0);
		lc_plate_set_OSD_flag(CarHandle,1);
		*/
		if(LC_PLATE_SUCCESS!=lc_plate_analysis(&CarHandle,image,w,h,size))
			return ;
	}
	else if(1==State)//N次以后
	{
		CarSet.Width=w;
		CarSet.Hight=h;

		if(LC_PLATE_SUCCESS!=lc_plate_analysis(&CarHandle,image,w,h,size))
			return ;
	}

	//如果创建失败就不做任何事
}
/*
任意车牌颜色
蓝牌
黄牌
白牌
白牌
黑牌
红牌
绿牌
未知
*/
/*
char* CCarDetect::CarColor(uint8_t i)
{
switch(i)
{
case 0:return "蓝牌";
case 1:return "黄牌";
case 2:return "白底-军牌";
case 3:return "白底-警牌";
case 4:return "黑牌";
case 5:return "红牌";
case 6:return "绿牌";
case 99:return "未知";
default:return "未知";
}
}

char* CCarDetect::CarType(uint8_t i)
{
switch(i)
{
case 1:return "大型汽车号牌";
case 2:return "小型汽车号牌";
case 16:return "教练汽车号牌";
case 23:return "警用汽车号牌";
case 25:return "军用汽车号牌";
case 26:return "境外汽车号牌";
case 27:return "境外商用汽车号牌";
case 28:return "农用汽车号牌";
case 99:return "未知";
default:return "未知";
}
}
*/

char* CCarDetect::CarColor2JingAo(uint8_t i)
{
	switch(i)
	{
	case 0:return "2";
	case 1:return "1";
	case 2:return "0";
	case 3:return "0";
	case 4:return "3";
	default:return "5";
	}
}

char* CCarDetect::CarType2JingAo(uint8_t i)
{

#if ALLTAB_DETECT_CAR_MODE
	switch(i)
	{
	case 1:return "H00";
	case 2:return "K00";
	default:return "ZZZ";
	}
#else
	return "M00";
#endif

}

char* CCarDetect::CarDirection(uint8_t i)
{
	switch(i)
	{
	case 0:return "未知方向";
	case 1:return "从上到下";
	case 2:return "从下到上";
	default:return "未知方向";
	}
}

//判断识别出来的车牌是否不能保存成文件(非法文件名)
// true为不正确车牌
bool CCarDetect::ErrorPlateName(char *name)
{

	if(NULL!=strchr(name,'\\'))
		return true;
	else if(NULL!=strchr(name,'/'))
		return true;
	else if(NULL!=strchr(name,':'))
		return true;
	else if(NULL!=strchr(name,'*'))
		return true;
	else if(NULL!=strchr(name,'?'))
		return true;
	else if(NULL!=strchr(name,'\"'))
		return true;
	else if(NULL!=strchr(name,'<'))
		return true;
	else if(NULL!=strchr(name,'>'))
		return true;
	else if(NULL!=strchr(name,'|'))
		return true;


#if (!ALLTAB_DETECT_CAR_MODE)

	char *p=&(name[strlen(name)-5]);

	//5个符号相同 不是电动车
	int j=p[0];

	if(j==p[1] && 
		j==p[2] && 
		j==p[3] && 
		j==p[4] )
		return true;

	// 4个1就退出
	j=0;
	for(int i=0;i<5;i++)
	{
		if( p[i]== '1')
			j++;
	}

	if(j>3)
		return true;

	/*
	select  t.* from TB_ALARM_VEHICLE t where length(REGEXP_REPlACE(t.scarnumber,'[0-9a-z]',''))>=2
	*/
	//字母超过1个。不是电动车
	j=0;
	for(int i=0;i<5;i++)
	{
		if( p[i]>= 'A' && p[i] <='Z')
			j++;
	}

	if(j>1)
		return true;

	/*
	select * from TB_ALARM_VEHICLE t where not REGEXP_LIKE(t.scarnumber,'^.[0-9a-z][0-9a-z][0-9a-z].$')
	*/
	//字母在中间3个。不是电动车
	j=0;
	for(int i=1;i<4;i++)
	{
		if( p[i]>='A' && p[i] <='Z')
			j++;
	}

	if(j>0)
		return true;

#endif

	return false;
}


void CCarDetect::FilterPlateName(char *name)
{

#if ALLTAB_DETECT_CAR_MODE
	//汽车
	if(('0'<=name[0] && name[0] <='9' && '0'<=name[1] && name[1] <='9')
		|| ('a'<=name[0] && name[0] <='z' && 'a'<=name[1] && name[1] <='z')
		|| ('A'<=name[0] && name[0] <='Z' && 'A'<=name[1] && name[1] <='Z'))
	{
		char tempchange[32]={0};
		sprintf(tempchange,"%s%s",CarSet.DefaultChar,&name[strlen(name)-6]);
		strcpy(name,tempchange);	
	}
#endif


}

//返回 0为句柄无效
//返回 1为写入数据库
//返回 2为获得车辆信息
//返回 3为查询结果，无返回结果
int CCarDetect::Result()
{
	if(1!=State)
		return 0;

	int i,re,ge;
	int tempCartotal=0;
	//仅用于返回
	int tempstate=3;
	char tempstr[100]={0};

	char str[ZOG_MAX_PATH_STR]={0};
	char pathstr[ZOG_MAX_PATH_STR]={0};
	char timestr[64]={0};
	char dirstr[ZOG_MAX_PATH_STR]={0};

	char pathSmallstr[ZOG_MAX_PATH_STR]={0};
	char dirSmallstr[ZOG_MAX_PATH_STR]={0};

	struct HISTORY_CarDetect_ST carInfo={0};
	int nItem;

	long nid,blackid;
	char Timeformat[32]={0};
	FILE *fp;
	bool isBlack=false;

	char jsonstr[2048];
	int resoap;

	re=lc_plate_get_plate_number(CarHandle,&tempCartotal);

	if(LC_PLATE_SUCCESS==re && tempCartotal>0)
	{
		PicFlag=true;
		CarTotal=tempCartotal;

		//for(i=0;i<CarTotal;i++)
		for(i=0;i<1;i++)
		{
			lc_plate_get_plate_name(CarHandle,i,&CarInfo[i].Str);

			lc_plate_get_plate_color_id(CarHandle,i,&CarInfo[i].ColorId);

			lc_plate_get_plate_color_name(CarHandle,i,&CarInfo[i].PlateColor);

			lc_plate_get_plate_reliability(CarHandle,i,&CarInfo[i].Reliability);

			lc_plate_get_car_direction(CarHandle,i,&CarInfo[i].Direction);

			lc_plate_get_plate_position(CarHandle,i,&CarInfo[i].CarRect);

			lc_plate_get_plate_type_id(CarHandle,i,&CarInfo[i].PlateTypeID);

			lc_plate_get_plate_type_name(CarHandle,i,&CarInfo[i].PlateType);

			//车身颜色
			lc_plate_get_car_color_name(CarHandle,i,&CarInfo[i].CarColor);

			memset(JpgSmall,0,sizeof(VIDEO_WIDTH*VIDEO_HEIGHT*3));
			JpgSmallSize=VIDEO_WIDTH*VIDEO_HEIGHT;
			lc_plate_get_small_picture(CarHandle,JpgSmall,&JpgSmallSize,i);
	
		}
		tempstate=2;
	}
	//准备抓图
	if(PicFlag && CarTotal>0)
	{
		ge=lc_plate_get_plate_picture(CarHandle,Jpg,&JpgSize);

		if(LC_PLATE_SUCCESS==ge && JpgSize>0 && JpgSize< 3110400 )
		{
			CTime nowtime=CTime::GetTickCount();
			sprintf(timestr,"%04d-%02d-%02d-%02d-%02d-%02d",
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				nowtime.GetHour(),
				nowtime.GetMinute(),
				nowtime.GetSecond());


			sprintf(dirstr,"%s\\%04d-%02d-%02d",
				DlgSetSystem.path_CarDetect.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay());

			CreateDirectory(dirstr, NULL);

			sprintf(dirstr,"%s\\%04d-%02d-%02d\\%d_%s",
				DlgSetSystem.path_CarDetect.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				camid,cam_name);

			CreateDirectory(dirstr, NULL);
			////////////////////////
			sprintf(dirSmallstr,"%s\\%04d-%02d-%02d",
				DlgSetSystem.path_CarDetect_Small.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay());

			CreateDirectory(dirSmallstr, NULL);

			sprintf(dirSmallstr,"%s\\%04d-%02d-%02d\\%d_%s",
				DlgSetSystem.path_CarDetect_Small.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				camid,cam_name);

			CreateDirectory(dirSmallstr, NULL);
			/////////////////
		
			//for(i=0;i<CarTotal;i++)
			for(i=0;i<1;i++)
			{
				if(strlen(CarInfo[i].Str) <7)
					continue;
				//错误车牌号
				if(ErrorPlateName(CarInfo[i].Str))
					continue;

				FilterPlateName(CarInfo[i].Str);

				sprintf(Timeformat,"%04d-%02d-%02d %02d:%02d:%02d",
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					nowtime.GetHour(),
					nowtime.GetMinute(),
					nowtime.GetSecond());

#if ALLTAB_DETECT_CAR_MODE
			
				//输出照片
				sprintf(pathstr,"%s\\%s %d %s %s %d "
					//					"%d %d %d %d"
					" %s %s %s %s %d .jpg",	\
					dirstr,
					timestr,camid,l_ipaddr,
					CarInfo[i].Str,
					CarInfo[i].Reliability,	
					//					CarInfo[i].CarRect.x0,CarInfo[i].CarRect.x1,CarInfo[i].CarRect.y0,CarInfo[i].CarRect.y1,
					//CarColor(CarInfo[i].ColorId),CarDirection(CarInfo[i].Direction),CarType(CarInfo[i].Type),
					CarInfo[i].PlateColor,CarDirection(CarInfo[i].Direction),CarInfo[i].PlateType,
					CarInfo[i].CarColor,
					JpgSize);

				fp=fopen(pathstr,"wb");
				if(fp)
				{
					fwrite(Jpg,JpgSize,1,fp);
					fclose(fp);
				}
				
				if(JpgSmall[0]!=0)
				{
					//////////////////////
					//输出照片
					sprintf(pathSmallstr,"%s\\%s %d %s %s %d "
						//					"%d %d %d %d"
						" %s %s %s %s %d .jpg",	\
						dirSmallstr,
						timestr,camid,l_ipaddr,
						CarInfo[i].Str,
						CarInfo[i].Reliability,	
						//					CarInfo[i].CarRect.x0,CarInfo[i].CarRect.x1,CarInfo[i].CarRect.y0,CarInfo[i].CarRect.y1,
						//CarColor(CarInfo[i].ColorId),CarDirection(CarInfo[i].Direction),CarType(CarInfo[i].Type),
						CarInfo[i].PlateColor,CarDirection(CarInfo[i].Direction),CarInfo[i].PlateType,
						CarInfo[i].CarColor,
						JpgSmallSize);

					fp=fopen(pathSmallstr,"wb");
					if(fp)
					{
						fwrite(JpgSmall,JpgSmallSize,1,fp);
						fclose(fp);
					}
				}
				bool tempadd;

				memset(&carInfo,0,sizeof(struct HISTORY_CarDetect_ST ));

				carInfo.ncamera=camid;
				strcpy(carInfo.platetype,CarInfo[i].PlateType);
				strcpy(carInfo.platecolor,CarInfo[i].PlateColor);
				strcpy(carInfo.direction,CarDirection(CarInfo[i].Direction));
				strcpy(carInfo.carcolor,CarInfo[i].CarColor);
				strcpy(carInfo.plate,CarInfo[i].Str);
				carInfo.reliability=CarInfo[i].Reliability;
				strcpy(carInfo.path,pathstr);
				carInfo.picsize=JpgSize;
				if(JpgSmall[0]!=0)
				{
					strcpy(carInfo.smallpath,pathSmallstr);
					carInfo.smallsize=JpgSmallSize;
				}
				carInfo.year=nowtime.GetYear();
				carInfo.mon=nowtime.GetMonth();
				carInfo.day=nowtime.GetDay();
				carInfo.hour=nowtime.GetHour();
				carInfo.min=nowtime.GetMinute();
				carInfo.sec=nowtime.GetSecond();

				nid=0;
				blackid=0;
				//本地保存
				tempadd=MySqlIO.CAR_MatchResult_AddNewNoPicData(
					isBlack,
					nid,
					blackid,
					carInfo);

				if(tempadd)
				{
					//插入列表
					if(DlgMain->DlgTabVideo.m_ListCarTotal >=LC_MAX_CAR_LIST_NUM)
					{
						CleanList();
					}

					//插图片
					if(JpgSmall[0]!=0)
						DlgMain->DlgTabVideo.m_ListImg.AddImg(pathSmallstr);
					else
						DlgMain->DlgTabVideo.m_ListImg.AddImg("");

					nItem =DlgMain->DlgTabVideo.m_ListImg.InsertItem(
						0, DlgMain->DlgTabVideo.m_ListCarTotal);

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,1,Timeformat);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,2,cam_name);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,3,l_ipaddr);

					(DlgMain->DlgTabVideo.m_ListCarTotal)++;
					sprintf(str,"%d",DlgMain->DlgTabVideo.m_ListCarTotal);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,4,str);

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,5,CarInfo[i].Str);

					sprintf(str,"%d",CarInfo[i].Reliability);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,6,str);

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,7,CarDirection(CarInfo[i].Direction));
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,8,	CarInfo[i].PlateColor);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,9,	CarInfo[i].PlateType);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,10,	CarInfo[i].CarColor);
					if( isBlack)
					{
						DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,11,	"是");
						if(DlgSetSystem.m_check_alarmpic)
						{
							//	ShellExecute(DlgMain->m_hWnd,NULL,pathstr,NULL,NULL,SW_NORMAL);
							JumpJPG=true;
							strcpy(JumpJPGpath,pathstr);
						}
						//报警声音
						if(DlgSetSystem.m_check_alarmwav)
							PlaySound(DlgSetSystem.m_path_alarmwav,NULL,SND_FILENAME | SND_ASYNC);
					}
					else
						DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,11,	"否");
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,12,	pathstr);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,13,	"否");

					sprintf(str,"%d",nid);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,14,	str);
					sprintf(str,"%d",blackid);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,15,	str);

				}
				else
				{
					errorprintf("数据库错误:识别结果插入错误");
					DlgMain->ShowRuntimeMessage("数据库错误:识别结果插入错误",1);
				}

#else
/*
				//电动车

				//输出照片
				sprintf(pathstr,"%s\\%s %d %s %s %d %s %d .jpg",	\
					dirstr,
					timestr,camid,l_ipaddr,
					&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],
					CarInfo[i].Reliability,
					CarDirection(CarInfo[i].Direction),
					JpgSize);

				fp=fopen(pathstr,"wb");
				if(fp)
				{
					fwrite(Jpg,JpgSize,1,fp);
					fclose(fp);
				}

				bool tempadd;

				//保存本地
				tempadd=MySqlIO.ELECAR_MatchResult_AddNew(
					&isBlack,
					camid,
					"绿牌",
					CarDirection(CarInfo[i].Direction),
					&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],
					CarInfo[i].Reliability,	
					pathstr,
					JpgSize,Jpg);

				if(tempadd)
				{
					//插入列表
					if(DlgMain->DlgTabVideo.m_ListCarTotal >=MAX_CAR_LIST_NUM)
					{
						CleanList();
					}
					//插图片
					if(JpgSmall[0]!=0)
						DlgMain->DlgTabVideo.m_ListImg.AddImg(pathSmallstr);
					else
						DlgMain->DlgTabVideo.m_ListImg.AddImg("");

					nItem =DlgMain->DlgTabVideo.m_ListImg.InsertItem(
						0, DlgMain->DlgTabVideo.m_ListCarTotal);


					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,1,Timeformat);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,2,cam_name);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,3,l_ipaddr);

					(DlgMain->DlgTabVideo.m_ListCarTotal)++;
					sprintf(str,"%d",DlgMain->DlgTabVideo.m_ListCarTotal);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,4,str);

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,5,&CarInfo[i].Str[strlen(CarInfo[i].Str)-5]);

					sprintf(str,"%d",CarInfo[i].Reliability);
					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,6,str);

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,7,CarDirection(CarInfo[i].Direction));

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,8,"绿牌");

					if( isBlack)
					{
						DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,9,	"是");
						if(DlgSetSystem.m_check_alarmpic)
						{
							//	ShellExecute(DlgMain->DlgTabVideo.m_hWnd,NULL,pathstr,NULL,NULL,SW_NORMAL);
							JumpJPG=true;
							strcpy(JumpJPGpath,pathstr);
						}
						//报警声音
						if(DlgSetSystem.m_check_alarmwav)
							PlaySound(DlgSetSystem.m_path_alarmwav,NULL,SND_FILENAME | SND_ASYNC);
					}
					else
						DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,9,	"否");

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,10,	pathstr);

					DlgMain->DlgTabVideo.m_ListCar.SetItemText(nItem,11,	"否");

				}
				else
				{
					errorprintf("数据库错误:识别结果插入错误");
					DlgMain->ShowRuntimeMessage("数据库错误:识别结果插入错误",1);
				}
*/

#endif

#if JING_AO_UPLOAD

				if(DlgSetSystem.m_c_jingao)
				{
					//小照片
					sprintf(pathSmallstr,"%d %s %d %s %d.jpg",	
						nid,
						timestr,camid,l_ipaddr,
						JpgSmallSize);

					//大照片
					sprintf(pathstr,"%d %s %d %s %d.jpg",	
						nid,
						timestr,camid,l_ipaddr,
						JpgSize);

					sprintf(jsonstr,"{id:\"%d\",captureDeviceIP:\"%s\",\
									captureDeviceMAC:\"00-00-00-00-00-00\",captureTime:\"%s\",\
									licensePlateNumber:\"%s\",licensePlateColor:\"%s\",\
									vehicleType:\"%s\",throughDirection:\"%d\",geographicalPosition:\"%s\"}",
									nid,l_ipaddr,Timeformat,carInfo.plate,
									CarColor2JingAo(CarInfo[i].ColorId),CarType2JingAo(CarInfo[i].PlateTypeID),
									cam_Direction,cam_name);

					resoap=JingAoUpload(DlgSetSystem.m_e_jingao_ipport.GetBuffer(0),
						"/capture-data-interface/vehicle",
						"baseInfo","licensePlatePicture","panoramicPicture",
						pathSmallstr,carInfo.smallpath,
						pathstr,carInfo.path,
						jsonstr);

					if(0!=resoap)
					{
						sprintf(jsonstr,"CAR JingAoUpload error %d",resoap);
						MySqlIO.LOG_AddNewSystemLog(DlgLogin.CurrentUser.user,jsonstr);
					}
					else
					{
						char *pjsonstr=strstr(jsonstr,"result:\"0\"");
						if(NULL==pjsonstr)
						{
							MySqlIO.LOG_AddNewSystemLog(DlgLogin.CurrentUser.user,jsonstr);
						}
					}
				}
#endif




				/**********************平台END*********************************/
			}//FOR END


			PicFlag=false;
			CarTotal=0;
			tempstate=1;
		}
	}
	return tempstate;
}

void CCarDetect::CleanList(void)
{
	DlgMain->DlgTabVideo.m_ListImg.ClearImageList();
	DlgMain->DlgTabVideo.m_ListCar.DeleteAllItems();
	DlgMain->DlgTabVideo.m_ListCarTotal=0;

}


//停止画面
void CCarDetect::Stop()
{
	if(1==State)
	{
		State=0;
		Sleep(100);
		lc_plate_analysis_destroy(CarHandle);
		//当为 ~()释放函数时。加此会出错
		errorprintf("识别停止成功");
		CarHandle=NULL;
	}

	State=-1;

	CarTotal=0;
	PicFlag=false;
	JpgSize=0;
}


#endif