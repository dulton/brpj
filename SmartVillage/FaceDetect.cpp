#include "stdafx.h"
#include "SmartVillage.h"

#include "FaceDetect.h"
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

#include "YUV2RGB.h"
#include "ijl.h"

#include "JingAoUpload.h"

extern TCHAR CurrentDir[ZOG_MAX_PATH_STR];


#if OPEN_FACEDETECT_CODE



extern RecoModuleHandle RWmodule_handle ;

CFaceDetect::CFaceDetect()
{
	Jpg=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
	JpgSize=0;
	JpgSmall=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
	JpgSmallSize=0;

	RGBdata=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
	RGBSrcdata=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));

	RWcommon_handle=NULL ;
	RWvideo_handle=NULL ;

	RWscale_ratio=1.0;
	RWsimilarity=80.0;
	/////////////////////////////////////
	cam_name[0]=0;
	l_ipaddr[0]=0;	
	State=-1;
	RWevent_count=0;

	ThreadFlag=FALSE;
	pthread=NULL;
	rgbLock=false;
	newFlag=false;

	cam_Direction=0;

	for(int i=0;i<TRACK_FACE_MAX;i++ )
	{
		track_list[i] =-1;
	}
	track_p=0;
	outthread=false;
	starttime=0;
	//RWrecoimage.imagedata = (unsigned char *)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
}

CFaceDetect::~CFaceDetect()
{
	if(FALSE !=	ThreadFlag)
	{
		ThreadFlag=FALSE;
		Sleep(100);
	}
	ThreadFlag=FALSE;
//	while(outthread)
	//	Sleep(100);

	if(pthread)
	{
		TerminateThread(pthread,0);
		pthread=NULL;
	}
	ThreadFlag=FALSE;

	if(RWvideo_handle)
	{
		RecoVideoAnalyse_UnInit(RWvideo_handle);
		RWvideo_handle=0;
	}

	if(RWcommon_handle)
	{
		FaceProcess_UnInit(RWcommon_handle);
		RWcommon_handle=0;
	}

	if(RGBdata)
	{
		free(RGBdata);
		RGBdata=NULL;
	}
	if(RGBSrcdata)
	{
		free(RGBSrcdata);
		RGBSrcdata=NULL;
	}
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
	


	State=-1;

	for(int i=0;i<TRACK_FACE_MAX;i++ )
	{
		track_list[i] =-1;
	}
	track_p=0;

}

void CFaceDetect::errorprintf(char *str)
{
	char tempstr[260]={0};
	sprintf(tempstr,"摄像头名称=%s,IP地址=%s,%s\n",cam_name,l_ipaddr,str);
	DlgMain->ShowRuntimeMessage(tempstr,0);
}

char* CFaceDetect::FaceType(int i)
{
	switch(i)
	{
	case RECO_FRONT:return "正脸";
	case RECO_SIDE:return "侧脸";
	default:return "未知";
	}
}
char* CFaceDetect::FaceSex(int i)
{
	switch(i)
	{
	case RECO_MALE:return "男性";
	case RECO_UNCERTAIN:return "未知";
	case RECO_FEMALE:return "女性";
	default:return "未知";
	}
}

char* CFaceDetect::FaceSexIn(int i)
{
	switch(i)
	{
	case RECO_MALE:return "男";
	case RECO_UNCERTAIN:return "无";
	case RECO_FEMALE:return "女";
	default:return "无";
	}
}

int CFaceDetect::FaceSexInt(int i)
{
	switch(i)
	{
	case RECO_MALE:return SEX_MALE;
	case RECO_UNCERTAIN:return SEX_UNKNOW;
	case RECO_FEMALE:return SEX_FEMALE;
	default:return SEX_UNKNOW;
	}
}

bool FACE_DETECT_LOCK=false;

//线程
DWORD WINAPI FaceDetect_ThreadPROC(LPVOID lpParameter)
{

	char pathstr[ZOG_MAX_PATH_STR]={0};
	char timestr[64]={0};
	char dirstr[ZOG_MAX_PATH_STR]={0};

	char pathSmallstr[ZOG_MAX_PATH_STR]={0};
	char dirSmallstr[ZOG_MAX_PATH_STR]={0};
	FILE *fp;
	unsigned char smallRGBtemp[320*320*4]={0};
	int smallRGBw,smallRGBh,smallRGBc;
	long smallRGBsize,smallRGBwstep;
	long personID;
	long faceID;
	bool regflag;
	struct FACE_CAPTURE_ST tempFace={0};
	int i,j;
	long capnid;
	unsigned char *RGBdataP=NULL;

	char jsonstr[2048];
	int resoap;

	CFaceDetect *p = (CFaceDetect*)lpParameter;
	p->ThreadFlag=TRUE;
	p->outthread=true;
	while(p->ThreadFlag && 1==p->State)
	{
		//有没数据的标志
		while(false == p->newFlag )
		{
			if(!p->ThreadFlag)
			{
				p->rgbLock=false;
				p->outthread=false;
				p->ThreadFlag=FALSE;
				return 0;
			}
			Sleep(20);

		}
		//读写锁
		while(p->rgbLock)
		{
			if(!p->ThreadFlag)
			{
				p->rgbLock=false;
				p->outthread=false;
				p->ThreadFlag=FALSE;
				return 0;
			}
			Sleep(20);
		}
		p->rgbLock=true;

		//新数据设为旧了
		p->newFlag=false;

		switch(p->RGBformat)
		{
		case VIDEO_FORMAT_YV12:
			YV122RGB(p->RGBSrcdata,p->RGBdata,p->RGBwidth,p->RGBheight);
			RGBdataP=p->RGBdata;
			break;
		case VIDEO_FORMAT_I420:
			YUV4202RGB(p->RGBSrcdata,p->RGBdata,p->RGBwidth,p->RGBheight);
			RGBdataP=p->RGBdata;
			break;
		case VIDEO_FORMAT_RGB888:
			RGBdataP=p->RGBSrcdata;
			break;
		case VIDEO_FORMAT_BGR888:
			RGBdataP=p->RGBSrcdata;
			break;
		default:break;
		}

	//	memcpy(	p->RWrecoimage.imagedata,RGBdataP,p->RGBwidth*p->RGBheight*3);
		p->RWrecoimage.imagedata=RGBdataP;
			p->RWrecoimage.image_size = p->RGBwidth*p->RGBheight*3;
			p->RWrecoimage.widthStep = p->RGBwidth*3;
			p->RWrecoimage.height = p->RGBheight;
			p->RWrecoimage.width =p->RGBwidth;

		p->RWevent_count=0;
		if(RecoVideoAnalyse_ProcessFrame(p->RWvideo_handle,(long)p->TimeStamp_ms, p->RWscale_ratio, p->RWsimilarity, 	p->RWrecoimage, p->RWevent_count) != 0) 
		{	
			p->errorprintf("RecoVideoAnalyse_ProcessFrame ERROR");
			p->rgbLock=false;
			continue;
		}
		

		if (p->RWevent_count > 0) 
		{
			for (i = 0; i < MAX_FACE_COUNT; i++)
				memset(&(p->RWevent_list[i]),0,sizeof(RecoAnalyseResult));

			if (RecoVideoAnalyse_GetDetectResult(p->RWvideo_handle, p->RWevent_list) != 0) 
			{
				p->rgbLock=false;
				continue;
			}

			for (i = 0; i < p->RWevent_count; i++)
			{
#if TEST_DEBUG			
				fp=fopen("d:\\22222222.txt","ab");
				if(fp)
				{
					fprintf(fp,"image_time=%d,camid=%d,l_ipaddr=%s %s age=%d track_no=%d,event_type=%d face_image_size=%d image_size=%d\r\n",
						p->RWevent_list[i].image_time,p->camid,p->l_ipaddr,
						p->FaceType(p->RWevent_list[i].face_type),p->RWevent_list[i].age,p->RWevent_list[i].track_no,
						p->RWevent_list[i].event_type,
						p->RWevent_list[i].face_image_size,	p->RWevent_list[i].image_size);

					fclose(fp);
				}
#endif
				//侧脸也退
				if(p->RWevent_list[i].face_image_size <1024 || p->RWevent_list[i].face_image_size >16000 ||
					p->RWevent_list[i].image_size <1024 || p->RWevent_list[i].image_size >768000 || 
					RECO_SIDE == p->RWevent_list[i].face_type)
				{
					continue;
				}

				if(p->RWevent_list[i].age >100 || p->RWevent_list[i].age <1 )
				{
					p->RWevent_list[i].age=30;
				}


				if(p->RWevent_list[i].event_type == RECO_EVENT_TYPE_ACTIVITY)
				{
					//激活态只插一张
					for(j=0;j<TRACK_FACE_MAX;j++ )
					{
						if(p->track_list[j] == p->RWevent_list[i].track_no)
							break;
					}
					if(j!=TRACK_FACE_MAX)
						continue;

					p->track_list[p->track_p] = p->RWevent_list[i].track_no;

					p->track_p=(p->track_p+1)%TRACK_FACE_MAX;
				}
#if TEST_DEBUG					
				fp=fopen("d:\\111111111.txt","ab");
				if(fp)
				{
					fprintf(fp,"%d,%d,%s %s %d %d,%d=\n",p->RWevent_list[i].image_time,p->camid,p->l_ipaddr,
						p->FaceType(p->RWevent_list[i].face_type),p->RWevent_list[i].age,p->RWevent_list[i].track_no,
						p->RWevent_list[i].event_type );
					fclose(fp);
				}
#endif				

				//其他态都直接获取

				CTime nowtime=p->starttime+p->RWevent_list[i].image_time/40/25;

				sprintf(timestr,"%04d-%02d-%02d-%02d-%02d-%02d",
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					nowtime.GetHour(),
					nowtime.GetMinute(),
					nowtime.GetSecond());

				sprintf(dirstr,"%s\\%04d-%02d-%02d",
					DlgSetSystem.path_FaceDetect.GetBuffer(0),
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay());

				CreateDirectory(dirstr, NULL);

				sprintf(dirstr,"%s\\%04d-%02d-%02d\\%d_%s",
					DlgSetSystem.path_FaceDetect.GetBuffer(0),
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					p->camid,p->cam_name);
				CreateDirectory(dirstr, NULL);
				////////////////////////
				sprintf(dirSmallstr,"%s\\%04d-%02d-%02d",
					DlgSetSystem.path_FaceDetect_Small.GetBuffer(0),
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay());

				CreateDirectory(dirSmallstr, NULL);

				sprintf(dirSmallstr,"%s\\%04d-%02d-%02d\\%d_%s",
					DlgSetSystem.path_FaceDetect_Small.GetBuffer(0),
					nowtime.GetYear(),
					nowtime.GetMonth(),
					nowtime.GetDay(),
					p->camid,p->cam_name);

				CreateDirectory(dirSmallstr, NULL);

				//输出照片
				sprintf(pathstr,"%s\\%s %d %s %s %d %s %d .jpg",	
					dirstr,
					timestr,p->camid,p->l_ipaddr,
					p->FaceType(p->RWevent_list[i].face_type),
					p->RWevent_list[i].age,
					p->FaceSex(p->RWevent_list[i].gender),
					p->RWevent_list[i].image_size);

				fp=fopen(pathstr,"wb");
				if(fp)
				{
					fwrite(p->RWevent_list[i].image,p->RWevent_list[i].image_size,1,fp);
					fclose(fp);
				}

				sprintf(pathSmallstr,"%s\\%s %d %s %s %d %s %d .jpg",	
					dirSmallstr,
					timestr,p->camid,p->l_ipaddr,
					p->FaceType(p->RWevent_list[i].face_type),
					p->RWevent_list[i].age,
					p->FaceSex(p->RWevent_list[i].gender),
					p->RWevent_list[i].face_image_size);

				fp=fopen(pathSmallstr,"wb");
				if(fp)
				{
					fwrite(p->RWevent_list[i].face_image,p->RWevent_list[i].face_image_size,1,fp);
					fclose(fp);
				}

				regflag=false;
#if 1
				if(JPEGData2BMP(p->RWevent_list[i].face_image,p->RWevent_list[i].face_image_size,
					smallRGBtemp, &smallRGBw,&smallRGBh,&smallRGBc,&smallRGBwstep,&smallRGBsize))
				{		

					regflag=DlgMain->DlgTabVideo.DlgDeviceTree.faceMngTempDB.RWFaceMngRegistTempPerson(
						p->RWdb_nid,
						p->RWevent_list[i].age,p->FaceSexIn(p->RWevent_list[i].gender),
						smallRGBw,smallRGBh,smallRGBwstep,
						smallRGBtemp, smallRGBsize,
						personID,faceID);
#if TEST_DEBUG	
					ZOGDramBMP("d:\\111111.bmp",smallRGBtemp,smallRGBw,smallRGBh);

					fp=fopen("d:\\111111.txt","ab");
					if(fp)
					{
						fprintf(fp,"%d,%d,%d= %d=%d %d %d %d\n",regflag,personID,faceID,	
							p->RWdb_nid,smallRGBw,smallRGBh,smallRGBwstep,smallRGBsize);
						fclose(fp);
					}
#endif
				}

				memset(&tempFace,0,sizeof(struct FACE_CAPTURE_ST ));

				tempFace.year=nowtime.GetYear();
				tempFace.mon= nowtime.GetMonth();
				tempFace.day=nowtime.GetDay();
				tempFace.hour=nowtime.GetHour();
				tempFace.min=nowtime.GetMinute();
				tempFace.sec=nowtime.GetSecond();

				tempFace.age=p->RWevent_list[i].age;
				tempFace.sex=p->FaceSexInt(p->RWevent_list[i].gender);
				tempFace.facetype= p->RWevent_list[i].face_type;
				tempFace.quality=p->RWevent_list[i].quality;

				strcpy(tempFace.face,pathSmallstr);
				tempFace.facesize=p->RWevent_list[i].face_image_size;
				strcpy(tempFace.file,pathstr);
				tempFace.size=p->RWevent_list[i].image_size;

				tempFace.ncamera=p->camid;
				strcpy(tempFace.cameraName,p->cam_name);
				strcpy(tempFace.sip,p->l_ipaddr);

				tempFace.temp_db_nid=p->DBnid;
				tempFace.RW_tempID=p->RWdb_nid;
				if(regflag)
				{
					tempFace.RW_personID=personID;
					tempFace.RW_faceID=faceID;
				}

				MySqlIO.FACE_Capture_AddNew(capnid,tempFace);
				tempFace.nid=capnid;

				if(	DlgMain->DlgTabVideo.DlgPictureFaceCap.DlgPLiteTotal >=50)
					DlgMain->DlgTabVideo.DlgPictureFaceCap.SendMessage(WM_CLEANFACE_MESSAGE,NULL,NULL);

				j=FLAG_FAC_CAP;
				DlgMain->DlgTabVideo.DlgPictureFaceCap.SendMessage(WM_ADDFACE_MESSAGE,(WPARAM)(&j),(LPARAM)&tempFace);
				//DlgMain->DlgTabVideo.DlgPictureFaceCap.AddCapList(tempFace,FLAG_FAC_CAP);

#if JING_AO_UPLOAD

				if(DlgSetSystem.m_c_jingao)
				{
					//小照片
					sprintf(pathSmallstr,"%d %s %d %s %d.jpg",	
						tempFace.nid,
						timestr,p->camid,p->l_ipaddr,
						tempFace.facesize);

					//大照片
					sprintf(pathstr,"%d %s %d %s %d.jpg",	
						tempFace.nid,
						timestr,p->camid,p->l_ipaddr,
						tempFace.size);
					//顺序不能提前
					sprintf(timestr,"%04d-%02d-%02d %02d:%02d:%02d",
						nowtime.GetYear(),
						nowtime.GetMonth(),
						nowtime.GetDay(),
						nowtime.GetHour(),
						nowtime.GetMinute(),
						nowtime.GetSecond());

					sprintf(jsonstr,"{id:\"%d\",captureDeviceIP:\"%s\",captureDeviceMAC:\"00-00-00-00-00-00\",\
									captureTime:\"%s\",throughDirection:\"%d\",\
									geographicalPosition:\"%s\"}",
									tempFace.nid,tempFace.sip,timestr,p->cam_Direction,tempFace.cameraName);

					resoap=JingAoUpload(DlgSetSystem.m_e_jingao_ipport.GetBuffer(0),
						"/capture-data-interface/face",
						"baseInfo","facePicture","panoramicPicture",
						pathSmallstr,tempFace.face,
						pathstr,tempFace.file,
						jsonstr);

					if(0!=resoap)
					{
						sprintf(jsonstr,"FACE JingAoUpload error %d",resoap);
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

#endif
			}

		}

		p->rgbLock=false;

	}
	p->rgbLock=false;
	p->outthread=false;
	p->ThreadFlag=FALSE;
	return 0;
}

//开始画面
void CFaceDetect::Start(int format,unsigned char *image,int w,int h,long widthStep,long size)
{

	if( (-1 == State) && (NULL == RWcommon_handle   || NULL == RWvideo_handle ))
	{
#if 1
		//SetCurrentDirectory(CurrentDir);
		//第一次
		RWcommon_handle = FaceProcess_Init(RWmodule_handle);
		if(NULL==RWcommon_handle)
		{
			State=0;
			errorprintf("RWcommon_handle=NULL");
			return ;
		}

		RWvideo_handle = RecoVideoAnalyse_Init(RWcommon_handle);
		if(NULL==RWvideo_handle)
		{
			State=0;
			errorprintf("RWvideo_handle=NULL");
			return ;
		}
#endif
		errorprintf("成功创建人脸识别");
		State=1;

		struct DEVICE_SET_ST CamSet={0};
		char str[32]={0};

		MySqlIO.DEVICE_SetTable_Read(camid,CamSet);
		sprintf(str,"%d",CamSet.face.time_out);
		RWsimilarity=CamSet.face.video_ts;
		RWscale_ratio=CamSet.face.scale_ratio;
#if 1
		//RecoVideoAnalyse_SetParameter(RWvideo_handle,"time_out",str);
	//	RecoVideoAnalyse_SetParameter(RWvideo_handle,"quality","1");
		RecoVideoAnalyse_SetParameter(RWvideo_handle,"age_gender","1");

	//	FaceProcess_SetParameter(RWcommon_handle,"-max_size_face",(char *)&CamSet.face.maxface);
	//	FaceProcess_SetParameter(RWcommon_handle,"-min_size_face",(char *)&CamSet.face.minface);

		sprintf(str,"%f",CamSet.face.frontface_ts);
	//	FaceProcess_SetParameter(RWcommon_handle,"-min_prob_face",str);

		sprintf(str,"%f",CamSet.face.sideface_ts);
	//	FaceProcess_SetParameter(RWcommon_handle,"-min_prob_sideface",str);
#endif
		//设置临时库ID
		struct FACE_TEMP_DB_ST tempDB={0};
		MySqlIO.TEMP_DB_Read(camid,tempDB);
		DBnid=tempDB.nid;
		RWdb_nid=tempDB.RW_tempID;		
		
		if(FALSE !=	ThreadFlag)
		{
			ThreadFlag=FALSE;
			Sleep(100);
			if(pthread)
			{
				TerminateThread(pthread,0);
				pthread=NULL;
			}
		}
		ThreadFlag=FALSE;
		pthread=NULL;

		for(int i=0;i<TRACK_FACE_MAX;i++ )
		{
			track_list[i] =-1;
		}
		track_p=0;

		
		CTime nowtime=CTime::GetTickCount();
		starttime=nowtime.GetTime();
		TimeStamp_ms=0;

		rgbLock=false;
		newFlag=false;
		pthread=CreateThread(NULL,0,FaceDetect_ThreadPROC,this,0,NULL);

		if(NULL==pthread)
		{
			errorprintf("RWvideo_handle=NULL");
		}
		else
			ThreadFlag=TRUE;

	}
	else if( 1 == State)
	{
		TimeStamp_ms+=40*FACE_JUMP_NUM;
		if(TimeStamp_ms >= (3600*1000))
		{

			CTime nowtime=CTime::GetTickCount();
			starttime=nowtime.GetTime();
			TimeStamp_ms=0;
		}


		//数据拷贝 加锁
		if(rgbLock)
			return ;

		rgbLock=true;

		RGBwidth=w;
		RGBheight=h;
		
		RGBformat=format;

		memcpy(RGBSrcdata,image,size);

		rgbLock=false;
		newFlag=true;

	}

	//如果创建失败就不做任何事
}




//停止画面
void CFaceDetect::Stop()
{
	if(FALSE !=	ThreadFlag)
	{
		ThreadFlag=FALSE;
		Sleep(100);

	}
	ThreadFlag=FALSE;
	int a=0;
	while(outthread)
	{
		Sleep(100);
		a++;
		if(a>50)
			break;
	}
	if(pthread)
	{
		TerminateThread(pthread,0);
		pthread=NULL;
	}
	ThreadFlag=FALSE;
	
#if 1
	if(RWvideo_handle)
	{
		RecoVideoAnalyse_UnInit(RWvideo_handle);
		RWvideo_handle=0;
	}

	if(RWcommon_handle)
	{
		FaceProcess_UnInit(RWcommon_handle);
		RWcommon_handle=0;
		errorprintf("人脸识别停止成功");
	}
#endif
outthread=false;
	State=-1;
	JpgSize=0;
	for(int i=0;i<TRACK_FACE_MAX;i++ )
	{
		track_list[i] =-1;
	}
	track_p=0;
		
	starttime=0;
		TimeStamp_ms=0;
}


#endif