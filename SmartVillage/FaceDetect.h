#ifndef _FACE_DETECT_H_
#define _FACE_DETECT_H_

#define VIDEO_WIDTH     1920
#define VIDEO_HEIGHT    1080

//WAV声音
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib") 

#define TRACK_FACE_MAX 5
	////////////////////////////////////////////////////////
class CFaceDetect
{
public:
	CFaceDetect();
	virtual ~CFaceDetect();

void errorprintf(char *str);


	void SetAll();

	void CleanList(void);

	void Stop();
void CFaceDetect::Start(int format,unsigned char *image,int w,int h,long widthStep,long size);

char* CFaceDetect::FaceType(int i);
char* CFaceDetect::FaceSex(int i);
char* CFaceDetect::FaceSexIn(int i);
int CFaceDetect::FaceSexInt(int i);
public:
	
	//线程句柄
	HANDLE pthread;
	//线程在用=false  线程未用=ture
	bool ThreadFlag;

	long track_list[TRACK_FACE_MAX];
	int track_p;
	RecoCommonHandle RWcommon_handle ;
	RecoVideoHandle  RWvideo_handle ;
	RecoImage RWrecoimage;
	int RWevent_count ;
	RecoAnalyseResult RWevent_list[MAX_FACE_COUNT];

	float RWscale_ratio;
	double RWsimilarity;
	bool outthread;
	int 	State;

	long long TimeStamp_ms;
	int RGBwidth,RGBheight;
	bool rgbLock;
	bool newFlag;
	unsigned char *RGBdata;
	unsigned char *RGBSrcdata;
	int RGBformat;

	unsigned char *Jpg;
	long JpgSize;
	unsigned char *JpgSmall;
	long JpgSmallSize;



	////////////////////////////////////////////////////////
	//数据库
	int     m_playhandle;  //播放句柄
	char	cam_name[256];	//摄像头名称
	char	l_ipaddr[64];	//IP地址
	unsigned long int	camid;			//摄像头NID
	long DBnid;					//临时人脸库
	long RWdb_nid;				//临时瑞为库

	long long starttime;


};


#endif