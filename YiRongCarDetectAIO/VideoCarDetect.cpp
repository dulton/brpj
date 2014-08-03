#include "stdafx.h"
#include "yirongcardetectaio.h"

#include "VideoCarDetect.h"
//////////////////////////
//WAV声音
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib") 

//////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

#include "IO.h"
extern IO OracleIO;

#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGVideoDetect.h"
extern CDLGVideoDetect *DlgVDmain;


#if ALLTAB_DETECT_CAR_MODE
//汽车
#pragma comment(lib, "carSDK\\plate_dll.lib")

#else
//电动车
#pragma comment(lib, "elecarSDK\\plate_dll.lib")

#endif

#if OPEN_CARDETECT_CODE

CVideoCarDetect::CVideoCarDetect()
{
	CarHandle=NULL;
	State=-1;
	
	CarTotal=0;
	PicFlag=false;
	Jpg=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
	JpgSize=0;
	
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
	
}

CVideoCarDetect::~CVideoCarDetect()
{
	if(1==State)
		lc_plate_analysis_destroy(CarHandle);
	
	free(Jpg);
	
	/*
	int i;
	for(i=0;i<CAR_STRUCT_MAX;i++)
	free(CarInfo[i].SmallJpg);
	*/
	
}


void CVideoCarDetect::SetRange()
{
	int i;
	//	char j[32];
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

void CVideoCarDetect::SetRedRect()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_position_flag(CarHandle,CarSet.RedRect))
			DlgMain->MessageBox("ERROR set_position",MESSAGEBOX_TITLE);
	}
}

void CVideoCarDetect::SetDeskew()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_deskew_flag(CarHandle,CarSet.Deskew))
			DlgMain->MessageBox("ERROR set_deskew_flag",MESSAGEBOX_TITLE);
	}
}
void CVideoCarDetect::SetMinMaxWidth()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_plate_width(CarHandle,CarSet.MinWidth,CarSet.MaxWidth))
			DlgMain->MessageBox("ERROR set_plate",MESSAGEBOX_TITLE);
	}
}

void CVideoCarDetect::SetMask()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_chinese_mask(CarHandle,CarSet.Mask))
			DlgMain->MessageBox("ERROR set_chinese_mask",MESSAGEBOX_TITLE);
	}
}

void CVideoCarDetect::SetDefaultChar()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_chinese_default(CarHandle,CarSet.DefaultChar))
			DlgMain->MessageBox("ERROR set_chinese_default",MESSAGEBOX_TITLE);
	}
}

void CVideoCarDetect::SetReliability()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_chinese_trust_thresh(CarHandle,CarSet.Reliability))
			DlgMain->MessageBox("ERROR set_chinese_trust",MESSAGEBOX_TITLE);
	}
}

void CVideoCarDetect::SetJPGquality()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_image_quality(CarHandle,CarSet.JPGquality))
			DlgMain->MessageBox("ERROR set_image_quality",MESSAGEBOX_TITLE);
	}
}

void CVideoCarDetect::SetCarColor()
{	
	if(1==State)
	{
		if(LC_PLATE_SUCCESS!=lc_plate_set_car_color_enable(CarHandle,CarSet.CarColor))
			DlgMain->MessageBox("ERROR set_car_color",MESSAGEBOX_TITLE);
	}
}


void CVideoCarDetect::SetAll()
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



void CVideoCarDetect::LimitMessageBox(void)
{
#if ALLTAB_DETECT_CAR_MODE
	
	uint32_t limit=0;
	char str[32];
	if(LC_PLATE_SUCCESS == lc_plate_get_limit_thread_num(&limit))
	{
		sprintf(str,"最大车牌识别限制=%d个",limit);
		DlgMain->MessageBox(str,MESSAGEBOX_TITLE);
	}
	
#endif 
	
}

//开始画面
void CVideoCarDetect::Start(int format,unsigned char *image,int w,int h,int size)
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
			DlgMain->MessageBox("当前车牌识别无法开启。线程数达到最大",MESSAGEBOX_TITLE);
			return ;
		}
		else
			State=1;
		
		if(LC_PLATE_SUCCESS!=lc_plate_set_video_format(CarHandle,format))
			return ;
		
		SetRedRect();
		SetRange();
		SetDeskew();
		SetMinMaxWidth();
		SetMask();
		SetDefaultChar();
		SetReliability();
		SetJPGquality();
		SetCarColor();
		
		if(LC_PLATE_SUCCESS!=lc_plate_analysis(&CarHandle,image,w,h,size))
			return ;
	}
	else if(1==State)//N次以后
	{
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
char* CVideoCarDetect::CarColor(uint8_t i)
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

  char* CVideoCarDetect::CarType(uint8_t i)
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
char* CVideoCarDetect::CarDirection(uint8_t i)
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
bool CVideoCarDetect::ErrorPlateName(char *name)
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
	
	return false;
}

//返回 0为句柄无效
//返回 1为写入数据库
//返回 2为获得车辆信息
//返回 3为查询结果，无返回结果
int CVideoCarDetect::WriteDBResult()
{
	if(1!=State)
		return 0;
	
	int i,re,ge;
	int tempCartotal=0;
	//仅用于返回
	int tempstate=3;
	char tempstr[100]={0};
	
	char str[260];
	char pathstr[ZOG_MAX_PATH_STR];
	char timestr[100];
	char dirstr[ZOG_MAX_PATH_STR];
	
	int nItem;
	
	int yrvm_first;
	long nid;
	char Timeformat[32];
	FILE *fp;
	bool isBlack=false;
	
	re=lc_plate_get_plate_number(CarHandle,&tempCartotal);
	
	if(LC_PLATE_SUCCESS==re && tempCartotal>0)
	{
		PicFlag=true;
		CarTotal=tempCartotal;
		
		for(i=0;i<CarTotal;i++)
		{
			lc_plate_get_plate_name(CarHandle,i,&CarInfo[i].Str);
			
			//lc_plate_get_plate_color_id(CarHandle,i,&CarInfo[i].ColorId);
			lc_plate_get_plate_color_name(CarHandle,i,&CarInfo[i].PlateColor);
			
			lc_plate_get_plate_reliability(CarHandle,i,&CarInfo[i].Reliability);
			
			lc_plate_get_car_direction(CarHandle,i,&CarInfo[i].Direction);
			
			lc_plate_get_plate_position(CarHandle,i,&CarInfo[i].CarRect);
			
			//	lc_plate_get_plate_type_id(CarHandle,i,&CarInfo[i].Type);
			lc_plate_get_plate_type_name(CarHandle,i,&CarInfo[i].PlateType);
			
			//车身颜色
			lc_plate_get_car_color_name(CarHandle,i,&CarInfo[i].CarColor);
			
			/*
			r=lc_plate_get_small_picture(CarHandle,CarInfo[i].SmallJpg,&CarInfo[i].SmallJpgSize,i);
			if(LC_PLATE_SUCCESS==r && JpgSize>0 && JpgSize< 3110400)
			{
			sprintf(str,"%s-small.jpg",CarInfo[i].Str);
			fp=fopen(str,"wb");
			if(fp)
			{
			fwrite(CarInfo[i].SmallJpg,CarInfo[i].SmallJpgSize,1,fp);
			fclose(fp);
			}
			}
			*/
		}
		tempstate=2;
	}
	//准备抓图
	if(PicFlag && CarTotal>0)
	{
		ge=lc_plate_get_plate_picture(CarHandle,Jpg,&JpgSize);
		
		if(LC_PLATE_SUCCESS==ge && JpgSize>0 && JpgSize< 3110400 )
		{
			//修改了时间
			CTime nowtime=starttime+totalsec;
			sprintf(timestr,"%04d-%02d-%02d-%02d-%02d-%02d",
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				nowtime.GetHour(),
				nowtime.GetMinute(),
				nowtime.GetSecond());
#if OPEN_TOMCAT_MODE
			//改存到TOMCAT地址
			sprintf(dirstr,"%s\\%04d-%02d-%02d",
				DlgSetSystem.m_tomcat_dir.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay());

			CreateDirectory(dirstr, NULL);

			sprintf(dirstr,"%s\\%04d-%02d-%02d\\%d",
				DlgSetSystem.m_tomcat_dir.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				camid);

			CreateDirectory(dirstr, NULL);
#else			
			sprintf(dirstr,"%s\\%04d-%02d-%02d",
				DlgSetSystem.m_path_detect.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay());
			
			CreateDirectory(dirstr, NULL);
			
			sprintf(dirstr,"%s\\%04d-%02d-%02d\\%d",
				DlgSetSystem.m_path_detect.GetBuffer(0),
				nowtime.GetYear(),
				nowtime.GetMonth(),
				nowtime.GetDay(),
				camid);
			
			CreateDirectory(dirstr, NULL);
#endif			
			yrvm_first=0;
			
			for(i=0;i<CarTotal;i++)
			{
				if(strlen(CarInfo[i].Str) <7)
					continue;
				//错误车牌号
				if(ErrorPlateName(CarInfo[i].Str))
					continue;
				
				sprintf(Timeformat,"%04d-%02d-%02d %02d:%02d:%02d",
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					nowtime.GetHour(),
					nowtime.GetMinute(),
					nowtime.GetSecond());
				
#if ALLTAB_DETECT_CAR_MODE
				//汽车
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
				
				bool tempadd;
#if OPEN_TOMCAT_MODE
				char tomcatpathURL[1024];

				sprintf(tomcatpathURL,"%s/%04d-%02d-%02d/%d/%s %d %s %s %d "
					//					"%d %d %d %d"
					" %s %s %s %s %d .jpg",	\
					DlgSetSystem.m_tomcat_url.GetBuffer(0),
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					camid,
					timestr,camid,l_ipaddr,
					CarInfo[i].Str,
					CarInfo[i].Reliability,	
					//					CarInfo[i].CarRect.x0,CarInfo[i].CarRect.x1,CarInfo[i].CarRect.y0,CarInfo[i].CarRect.y1,
					//CarColor(CarInfo[i].ColorId),CarDirection(CarInfo[i].Direction),CarType(CarInfo[i].Type),
					CarInfo[i].PlateColor,CarDirection(CarInfo[i].Direction),CarInfo[i].PlateType,
					CarInfo[i].CarColor,
					JpgSize);

				//TOMCAT 照片不写数据库
				tempadd=OracleIO.CAR_MatchResult_AddNewNoPicData(
					&isBlack,
					camid,
					CarInfo[i].PlateType,
					CarInfo[i].PlateColor,	
					CarDirection(CarInfo[i].Direction),
					CarInfo[i].CarColor,
					CarInfo[i].Str,
					CarInfo[i].Reliability,	
					tomcatpathURL,
					JpgSize);
#else
				//网络保存
				if(DlgSetSystem.m_check_savenet)
				{
					//写数据库
					tempadd=OracleIO.CAR_MatchResult_AddNew(
						&isBlack,
						camid,
						CarInfo[i].PlateType,
						CarInfo[i].PlateColor,	
						CarDirection(CarInfo[i].Direction),
						CarInfo[i].CarColor,
						CarInfo[i].Str,
						CarInfo[i].Reliability,	
						"null",
						JpgSize,Jpg);
				}
				else
				{
					//本地保存
					tempadd=OracleIO.CAR_MatchResult_AddNew(
						&isBlack,
						camid,
						CarInfo[i].PlateType,
						CarInfo[i].PlateColor,	
						CarDirection(CarInfo[i].Direction),
						CarInfo[i].CarColor,
						CarInfo[i].Str,
						CarInfo[i].Reliability,	
						pathstr,
						JpgSize,Jpg);
					
				}
#endif
				if(tempadd)
				{
					//插入列表
					DlgVDmain->m_ListCarTotal++;
					sprintf(str,"%d",DlgVDmain->m_ListCarTotal);
					nItem = DlgVDmain->m_list.InsertItem(0,str);
					
					DlgVDmain->m_list.SetItemText(nItem,1,Timeformat);
					DlgVDmain->m_list.SetItemText(nItem,2,CarInfo[i].Str);
					
					sprintf(str,"%d",CarInfo[i].Reliability);
					DlgVDmain->m_list.SetItemText(nItem,3,str);
					
					DlgVDmain->m_list.SetItemText(nItem,4,CarDirection(CarInfo[i].Direction));
					DlgVDmain->m_list.SetItemText(nItem,5,	CarInfo[i].PlateColor);
					DlgVDmain->m_list.SetItemText(nItem,6,	CarInfo[i].PlateType);
					DlgVDmain->m_list.SetItemText(nItem,7,	CarInfo[i].CarColor);
					if( isBlack)
						DlgVDmain->m_list.SetItemText(nItem,8,	"是");
					else
						DlgVDmain->m_list.SetItemText(nItem,8,	"否");
					
					DlgVDmain->m_list.SetItemText(nItem,9,	pathstr);
					
#if OPEN_TOMCAT_MODE
					DlgVDmain->m_list.SetItemText(nItem,10,	"否");
#else
					if(DlgSetSystem.m_check_savenet)
						DlgVDmain->m_list.SetItemText(nItem,10,	"是");
					else
						DlgVDmain->m_list.SetItemText(nItem,10,	"否");
#endif

				}
				else
				{
					DlgMain->ShowRuntimeMessage("数据库错误:识别结果插入错误",1);
				}
				
				
#if OPEN_VS2008_POCO_FTP
				
				//上传到FTP
				FtpUploadCar(timestr,
					l_ipaddr,
					CarInfo[i].Str,
					CarInfo[i].Reliability,	
					CarDirection(CarInfo[i].Direction),
					CarInfo[i].PlateType,
					CarInfo[i].PlateColor,
					CarInfo[i].CarColor,
					Jpg,
					JpgSize);
#endif
				
#else
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
#if OPEN_TOMCAT_MODE

				char tomcatpathURL[1024];

				sprintf(tomcatpathURL,"%s/%04d-%02d-%02d/%d/%s %d %s %s %d %s %d .jpg",	\
					DlgSetSystem.m_tomcat_url.GetBuffer(0),
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					camid,
					timestr,camid,l_ipaddr,
					&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],
					CarInfo[i].Reliability,
					CarDirection(CarInfo[i].Direction),
					JpgSize);

				//TOMCAT 照片不写数据库
				tempadd=OracleIO.ELECAR_MatchResult_AddNewNoPicdata(
					&isBlack,
					camid,
					"绿牌",	
					CarDirection(CarInfo[i].Direction),
					&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],
					CarInfo[i].Reliability,	
					tomcatpathURL,
					JpgSize);
#else

				//网络保存
				if(DlgSetSystem.m_check_savenet)
				{
					//写数据库
					tempadd=OracleIO.ELECAR_MatchResult_AddNew(
						&isBlack,
						camid,
						"绿牌",	
						CarDirection(CarInfo[i].Direction),
						&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],
						CarInfo[i].Reliability,	
						"null",
						JpgSize,Jpg);
				}
				else
				{
					//保存本地
					tempadd=OracleIO.ELECAR_MatchResult_AddNew(
						&isBlack,
						camid,
						"绿牌",
						CarDirection(CarInfo[i].Direction),
						&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],
						CarInfo[i].Reliability,	
						pathstr,
						JpgSize,Jpg);
				}
#endif
				if(tempadd)
				{
					//插入列表
					DlgVDmain->m_ListCarTotal++;
					sprintf(str,"%d",DlgVDmain->m_ListCarTotal);
					nItem = DlgVDmain->m_list.InsertItem(0,str);
					
					DlgVDmain->m_list.SetItemText(nItem,1,Timeformat);
					DlgVDmain->m_list.SetItemText(nItem,2,&CarInfo[i].Str[strlen(CarInfo[i].Str)-5]);
					
					sprintf(str,"%d",CarInfo[i].Reliability);
					DlgVDmain->m_list.SetItemText(nItem,3,str);
					
					DlgVDmain->m_list.SetItemText(nItem,4,CarDirection(CarInfo[i].Direction));
					
					DlgVDmain->m_list.SetItemText(nItem,5,"绿牌");
					
					if( isBlack)
						DlgVDmain->m_list.SetItemText(nItem,6,	"是");
					else
						DlgVDmain->m_list.SetItemText(nItem,6,	"否");
					
					DlgVDmain->m_list.SetItemText(nItem,7,	pathstr);
					
#if OPEN_TOMCAT_MODE
					DlgVDmain->m_list.SetItemText(nItem,8,	"否");
#else

					if(DlgSetSystem.m_check_savenet)
						DlgVDmain->m_list.SetItemText(nItem,8,	"是");
					else
						DlgVDmain->m_list.SetItemText(nItem,8,	"否");
#endif

				}
				else
				{
					DlgMain->ShowRuntimeMessage("数据库错误:识别结果插入错误",1);
				}
#if OPEN_VS2008_POCO_FTP
				
				//上传到FTP
				FtpUploadElecar(timestr,
					l_ipaddr,
					&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],
					CarInfo[i].Reliability,	
					CarDirection(CarInfo[i].Direction),
					Jpg,
					JpgSize);
#endif
				
#endif
				/**********************平台BEGIN*********************************/
#if YRVM_PINGTAI_MODE
				
#if ALLTAB_DETECT_CAR_MODE
				//汽车
				if(0 == yrvm_first)
				{
					//数据库
					nid=OracleIO.YRVM_getCarOracleTempNid();
					//写入失败
					if(! OracleIO.YRVM_writeCarTempPhotoToOracleDB(l_ipaddr,Jpg,JpgSize,nid))
					{
						DlgMain->ShowRuntimeMessage("YRVM数据库错误:可能无此摄像头IP或者识别结果插入错误",1);
						break; //退出FOR循环
					}
					yrvm_first=1;
				}
				
				//改置信度
				sprintf(tempstr,"%02d",CarInfo[i].Reliability);
				
				//写字符串
				OracleIO.YRVM_writeCarTempInfoToOracleDB(CarInfo[i].Str,	\
					CarInfo[i].PlateColor,CarInfo[i].Direction,tempstr,CarInfo[i].PlateType,
					CarInfo[i].CarColor,nid);
#else
				
#if YRVM_PINGTAI_ELECAR_MIX_MODE
				//电动车写入机动车表
				if(0==yrvm_first)
				{
					//数据库
					nid=OracleIO.YRVM_getCarOracleTempNid();
					if(! OracleIO.YRVM_writeCarTempPhotoToOracleDB(l_ipaddr,Jpg,JpgSize,nid))
					{
						DlgMain->ShowRuntimeMessage("YRVM数据库错误:可能无此摄像头IP或者识别结果插入错误",1);
						break; //退出FOR循环
					}
					yrvm_first=1;
				}
				
				//改置信度
				sprintf(tempstr,"%02d",CarInfo[i].Reliability);
				
				//写字符串
				OracleIO.YRVM_writeCarTempInfoToOracleDB(&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],	\
					"绿牌",CarInfo[i].Direction,tempstr,"电动车号牌",
					CarInfo[i].CarColor,nid);
				
#else
				//电动车写入电动车表
				if(0==yrvm_first)
				{
					//数据库
					nid=OracleIO.YRVM_getElectricCarOracleTempNid();
					if(! OracleIO.YRVM_writeElectricCarTempPhotoToOracleDB(l_ipaddr,Jpg,JpgSize,nid))
					{
						DlgMain->ShowRuntimeMessage("YRVM数据库错误:可能无此摄像头IP或者识别结果插入错误",1);
						break; //退出FOR循环
					}
					yrvm_first=1;
				}
				
				//改置信度
				sprintf(tempstr,"%02d",CarInfo[i].Reliability);
				
				//写字符串
				OracleIO.YRVM_writeElectricCarTempInfoToOracleDB(&CarInfo[i].Str[strlen(CarInfo[i].Str)-5],	\
					//	CarInfo[i].PlateColor,
					"绿牌",
					CarInfo[i].Direction,tempstr,nid);
#endif
#endif
				
#endif
				/**********************平台END*********************************/
			}//FOR END
			/**********************平台BEGIN*********************************/
#if YRVM_PINGTAI_MODE
			//只执行一次
			if(yrvm_first)
#if ALLTAB_DETECT_CAR_MODE
				//汽车
				OracleIO.YRVM_ExecuteInsertMatchCarResultProcedure(nid);
#else
			
#if YRVM_PINGTAI_ELECAR_MIX_MODE
			//电动车写入机动车表
			OracleIO.YRVM_ExecuteInsertMatchCarResultProcedure(nid);
#else
			//电动车写入电动车表
			OracleIO.YRVM_ExecuteInsertMatchVehicleResultProcedure(nid);
#endif
			
#endif
			
#endif
			/**********************平台END*********************************/
			PicFlag=false;
			CarTotal=0;
			tempstate=1;
		}
	}
	return tempstate;
}

//停止画面
void CVideoCarDetect::Stop()
{
	if(1==State)
	{
		Sleep(100);
		lc_plate_analysis_destroy(CarHandle);
		
		CarHandle=NULL;
	}
	
	State=-1;
	
	CarTotal=0;
	PicFlag=false;
	JpgSize=0;
}

#if OPEN_VS2008_POCO_FTP

#include "FTPClient.h"
bool CVideoCarDetect::FtpUploadElecar(char *TimeStr,
									  char *IpAddr,
									  char *Plate,
									  unsigned int Reliability,	
									  char *Direction,
									  unsigned char *FileBuf,
									  unsigned long int FileSize)
{
	//未开启上传
	if(FALSE == DlgSetSystem.m_check_ftp)
		return true;
	
	FTPClient g_ftpClient;
	char temp[512];
	
	if(0 != g_ftpClient.LoginServer(
		DlgSetSystem.m_ftp_ip.GetBuffer(0),
		atoi(DlgSetSystem.m_ftp_port.GetBuffer(0)),
		DlgSetSystem.m_ftp_user.GetBuffer(0),
		DlgSetSystem.m_ftp_psw.GetBuffer(0))
		)
	{
		errorprintf("FTP用户登录失败");
		return false;
	}
	
	if(0 != g_ftpClient.SetWorkingDirectory(DlgSetSystem.m_ftp_path.GetBuffer(0)))
	{
		errorprintf("FTP设置目录失败");
		g_ftpClient.CloseConnect();
		return false;
	}
	
	//格式 年-月-日-时-分-秒(空格)IP地址(空格)车牌号(空格)置信度(空格)行驶方向(空格)文件字节长度(空格).txt
	sprintf(temp,"%s %s %s %d %s %d.txt",
		TimeStr,IpAddr,Plate,Reliability,Direction,FileSize);
	
	if(0 != g_ftpClient.UpLoadFile(temp,(const char*)FileBuf))
	{
		errorprintf("FTP上传失败");
		g_ftpClient.CloseConnect();
		return false;
	}
	
	g_ftpClient.CloseConnect();
	return true;
}

bool CVideoCarDetect::FtpUploadCar(char *TimeStr,
								   char *IpAddr,
								   char *Plate,
								   unsigned int Reliability,	
								   char *Direction,
								   char *PlateType,
								   char *PlateColor,
								   char *CarColor,
								   unsigned char *FileBuf,
								   unsigned long int FileSize)
{
	//未开启上传
	if(FALSE == DlgSetSystem.m_check_ftp)
		return true;
	
	FTPClient g_ftpClient;
	char temp[512];
	
	if(0 != g_ftpClient.LoginServer(
		DlgSetSystem.m_ftp_ip.GetBuffer(0),
		atoi(DlgSetSystem.m_ftp_port.GetBuffer(0)),
		DlgSetSystem.m_ftp_user.GetBuffer(0),
		DlgSetSystem.m_ftp_psw.GetBuffer(0))
		)
	{
		errorprintf("FTP用户登录失败");
		return false;
	}
	
	if(0 != g_ftpClient.SetWorkingDirectory(DlgSetSystem.m_ftp_path.GetBuffer(0)))
	{
		errorprintf("FTP设置目录失败");
		g_ftpClient.CloseConnect();
		return false;
	}
	
	//格式 年-月-日-时-分-秒(空格)IP地址(空格)车牌号(空格)置信度(空格)行驶方向(空格)车牌类型(空格)车牌颜色(空格)车身颜色(空格)文件字节长度(空格).txt
	sprintf(temp,"%s %s %s %d %s %s %s %s %d.txt",
		TimeStr,IpAddr,Plate,Reliability,Direction,PlateType,PlateColor,CarColor,FileSize);
	
	if(0 != g_ftpClient.UpLoadFile(temp,(const char*)FileBuf))
	{
		errorprintf("FTP上传失败");
		g_ftpClient.CloseConnect();
		return false;
	}
	
	g_ftpClient.CloseConnect();
	return true;
}

#endif




//返回 0为句柄无效
//返回 1为写入数据库
//返回 2为获得车辆信息
//返回 3为查询结果，无返回结果
int CVideoCarDetect::NoWriteDBResult()
{
	if(1!=State)
		return 0;
	
	int i,re,ge;
	int tempCartotal=0;
	//仅用于返回
	int tempstate=3;
	char tempstr[100]={0};
	
	char str[260];
	char pathstr[ZOG_MAX_PATH_STR];
	char timestr[100];
	char dirstr[ZOG_MAX_PATH_STR];
	
	int nItem;
	
	int yrvm_first;
	
	char Timeformat[32];
	FILE *fp;
	bool isBlack=false;
	
	re=lc_plate_get_plate_number(CarHandle,&tempCartotal);
	
	if(LC_PLATE_SUCCESS==re && tempCartotal>0)
	{
		PicFlag=true;
		CarTotal=tempCartotal;
		
		for(i=0;i<CarTotal;i++)
		{
			lc_plate_get_plate_name(CarHandle,i,&CarInfo[i].Str);
			
			//lc_plate_get_plate_color_id(CarHandle,i,&CarInfo[i].ColorId);
			lc_plate_get_plate_color_name(CarHandle,i,&CarInfo[i].PlateColor);
			
			lc_plate_get_plate_reliability(CarHandle,i,&CarInfo[i].Reliability);
			
			lc_plate_get_car_direction(CarHandle,i,&CarInfo[i].Direction);
			
			lc_plate_get_plate_position(CarHandle,i,&CarInfo[i].CarRect);
			
			//	lc_plate_get_plate_type_id(CarHandle,i,&CarInfo[i].Type);
			lc_plate_get_plate_type_name(CarHandle,i,&CarInfo[i].PlateType);
			
			//车身颜色
			lc_plate_get_car_color_name(CarHandle,i,&CarInfo[i].CarColor);
			
			/*
			r=lc_plate_get_small_picture(CarHandle,CarInfo[i].SmallJpg,&CarInfo[i].SmallJpgSize,i);
			if(LC_PLATE_SUCCESS==r && JpgSize>0 && JpgSize< 3110400)
			{
			sprintf(str,"%s-small.jpg",CarInfo[i].Str);
			fp=fopen(str,"wb");
			if(fp)
			{
			fwrite(CarInfo[i].SmallJpg,CarInfo[i].SmallJpgSize,1,fp);
			fclose(fp);
			}
			}
			*/
		}
		tempstate=2;
	}
	//准备抓图
	if(PicFlag && CarTotal>0)
	{
		ge=lc_plate_get_plate_picture(CarHandle,Jpg,&JpgSize);
		
		if(LC_PLATE_SUCCESS==ge && JpgSize>0 && JpgSize< 3110400 )
		{
			//修改了时间
			
			sprintf(timestr,"%d-%02d-%02d",
				totalsec/3600,(totalsec % 3600)/60,totalsec%60);
			
			sprintf(dirstr,"%s\\%s",
				DlgSetSystem.m_path_detect.GetBuffer(0),
				cam_name);
			
			CreateDirectory(dirstr, NULL);
			
			yrvm_first=0;
			
			for(i=0;i<CarTotal;i++)
			{
				if(strlen(CarInfo[i].Str) <7)
					continue;
				//错误车牌号
				if(ErrorPlateName(CarInfo[i].Str))
					continue;
				
				sprintf(Timeformat,"%d:%02d:%02d",
					totalsec/3600,(totalsec % 3600)/60,totalsec%60);
				
#if ALLTAB_DETECT_CAR_MODE
				//汽车
				//输出照片
				sprintf(pathstr,"%s\\%s %s %d "
					//					"%d %d %d %d"
					" %s %s %s %s %d .jpg",	\
					dirstr,
					timestr,
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
				
				
				//插入列表
				DlgVDmain->m_ListCarTotal++;
				sprintf(str,"%d",DlgVDmain->m_ListCarTotal);
				nItem = DlgVDmain->m_list.InsertItem(0,str);
				
				DlgVDmain->m_list.SetItemText(nItem,1,Timeformat);
				DlgVDmain->m_list.SetItemText(nItem,2,CarInfo[i].Str);
				
				sprintf(str,"%d",CarInfo[i].Reliability);
				DlgVDmain->m_list.SetItemText(nItem,3,str);
				
				DlgVDmain->m_list.SetItemText(nItem,4,CarDirection(CarInfo[i].Direction));
				DlgVDmain->m_list.SetItemText(nItem,5,	CarInfo[i].PlateColor);
				DlgVDmain->m_list.SetItemText(nItem,6,	CarInfo[i].PlateType);
				DlgVDmain->m_list.SetItemText(nItem,7,	CarInfo[i].CarColor);
				
				DlgVDmain->m_list.SetItemText(nItem,8,	"否");
				DlgVDmain->m_list.SetItemText(nItem,9,	pathstr);
				DlgVDmain->m_list.SetItemText(nItem,10,	"否");
				
#else
				//电动车
				//输出照片
				sprintf(pathstr,"%s\\%s %s %d %s %d .jpg",	\
					dirstr,
					timestr,
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
				
				//插入列表
				DlgVDmain->m_ListCarTotal++;
				sprintf(str,"%d",DlgVDmain->m_ListCarTotal);
				nItem = DlgVDmain->m_list.InsertItem(0,str);
				
				DlgVDmain->m_list.SetItemText(nItem,1,Timeformat);
				DlgVDmain->m_list.SetItemText(nItem,2,&CarInfo[i].Str[strlen(CarInfo[i].Str)-5]);
				
				sprintf(str,"%d",CarInfo[i].Reliability);
				DlgVDmain->m_list.SetItemText(nItem,3,str);
				DlgVDmain->m_list.SetItemText(nItem,4,CarDirection(CarInfo[i].Direction));
				DlgVDmain->m_list.SetItemText(nItem,5,"绿牌");
				
				DlgVDmain->m_list.SetItemText(nItem,6,	"否");
				DlgVDmain->m_list.SetItemText(nItem,7,	pathstr);
				DlgVDmain->m_list.SetItemText(nItem,8,	"否");
#endif
			}//FOR END
			
			PicFlag=false;
			CarTotal=0;
			tempstate=1;
		}
	}
	return tempstate;
}

#endif