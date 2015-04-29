#pragma once

#include "Picture.h"
#include "YRSVMySQL.h"
//


#define RWFACE_INIT_DONE			0		//初始化成功
#define RWRACE_PROCESS_INIT_ERROR	1		//通用库初始化错误
#define RWRACE_FEATURE_INIT_ERROR	2		//获取特征参数错误
#define RWFACE_IMAGE_INIT_ERROR		3		//获取图像参数错误	
#define RWFACE_MANAGE_INIT_ERROR	4		//人脸管理库初始化错误
#define RWFACE_DATABASE_INIT_ERROR	5		//人脸数据库初始化错误

//临时抓拍表未比对记录数据结构
struct  UNCMP_FACE_INFO_ST
{
	long capture_nid;		//抓拍表nid
	long ncamera;			//摄像机
	char path[260];			//文件路径
	long size;				//照片大小
	int threshold;			//最小相似度阀值
	int maxcount;			//最大匹配数
	int dbTotal;			//设置布控数量
	long db_nid[RW_MAX_FACE_DB_NUM];		//设置布控库ID
	char cameraName[64];	//摄像头名称
	char sip[32];			//摄像头IP
};

//本地图片与临时抓拍人员比较条件数据结构
struct TEMP_CMP_CONDITION_ST
{
	int max_match_count;
	int min_similarity;
	int sex;			//性别
	long ncamera;		//摄像头编号

	int flag;
	char stime[32];		//开始时间
	char etime[32];		//结束时间
};

//本地图片与临时抓拍人员比对结果
struct TEMP_CMP_RESULT_ST
{
	struct FACE_CAPTURE_ST tempface;
	int similarity;
};

//本地图片与布控人员比较条件数据结构
struct BLACK_CMP_CONDITION_ST
{
	int max_match_count;
	int min_similarity;

	char name[32];		//姓名
	int sex;			//性别
	char sage[32];		//开始出生日期
	char eage[32];		//结束出生日期
	int	cardtype;		//证件类型
	char cardNO[32];	//证件号
	char phone[32];		//电话
	int native;			//籍贯
	char address[512];	//地址
	long db_id;		//摄像头编号

	int flag;
};

//本地图片与布控人员比对结果
struct BLACK_CMP_RESULT_ST
{
	struct PERSON_BLACK_ST person;
	struct FACE_BLACK_ST face;
	int similarity;
};

#define MAX_MATCH_LIST_SIZE	32	//最大匹配人脸结果数


class CFaceRecg
{
public:
	CFaceRecg(void);
	~CFaceRecg(void);

public:
	int faceImg_width;
	int faceImg_height;
	int faceImg_wStep;
	int faceImg_size;
	int faceFeatrue_size;

	RecoCommonHandle common_handle;
	RecoFaceMngHandle faceMngHandle;
	RecoFaceMngHandle tempMngHandle;

	bool faceFecgLock;
	bool m_bThreadWork;
	HANDLE m_pThreadRecognize;
	
	RecoImage* GetImage(HBITMAP pic);
	int SaveImage(const char *file, const RecoImage *image);
	void ReleaseImage(RecoImage *image);

	int RWFaceRecg_BlackInit(void);
	int RWFaceRecg_UnInit(void);

//	RecoFeatMatchRes real_match_list[MAX_MATCH_LIST_SIZE];//实时比对结果
//	RecoFeatMatchRes temp_match_list[MAX_MATCH_LIST_SIZE];//图片人脸检索比对结果结果
	
	RecoFeatMatchRes *real_match_list;//实时比对结果
	RecoFeatMatchRes *temp_match_list;//图片人脸检索比对结果结果

	int StartRecgThread(void);//启动人脸比对线程
	int StopRecgThread(void);//停止人脸比对线程

	//黑名单比较
	bool RWFaceMngFaceCompare(int &match_list_size,RecoFeatMatchRes *match_res_list,long dbID,int max_match_count,double min_similarity,char *path);
	//临时抓拍库比较
	bool RWFcaeMngTempCompare(int &match_list_size,RecoFeatMatchRes *match_res_list,long dbID,int max_match_count,double min_similarity,char *path);

	//从数据库获取数据
	bool GetUnCmpFaceFromMySQL(struct UNCMP_FACE_INFO_ST &cmpFace,struct FACE_CAPTURE_ST &tempFace);//从数据库获取未比对数据信息
	void StartCompareWithBlack(struct UNCMP_FACE_INFO_ST cmpFace,struct FACE_CAPTURE_ST tempFace);//临时抓拍库与黑名单比较

	//本地图片与临时抓拍库比较
	bool ImageCmpWihtTempFace(char *path,struct TEMP_CMP_CONDITION_ST condition,list<struct TEMP_CMP_RESULT_ST> &faceList);
	//本地图片与黑名单拍库比较
	bool ImageCmpWihtBlackFace(char *path,struct BLACK_CMP_CONDITION_ST condition,list<struct BLACK_CMP_RESULT_ST> &faceList);
};
