#pragma once

#define RWFACE_INIT_DONE			0		//初始化成功
#define RWRACE_PROCESS_INIT_ERROR	1		//通用库初始化错误
#define RWRACE_FEATURE_INIT_ERROR	2		//获取特征参数错误
#define RWFACE_IMAGE_INIT_ERROR		3		//获取图像参数错误	
#define RWFACE_MANAGE_INIT_ERROR	4		//人脸管理库初始化错误
#define RWFACE_DATABASE_INIT_ERROR	5		//人脸数据库初始化错误

#include "Picture.h"


class CFaceMng
{
public:
	CFaceMng(void);
	~CFaceMng(void);

public:
	int faceImg_width;
	int faceImg_height;
	int faceImg_wStep;
	int faceImg_size;
	int faceFeatrue_size;
	bool mySqliteLock;

	RecoCommonHandle common_handle;
	RecoFaceMngHandle faceMngHandle;

	RecoImage* GetImage(HBITMAP pic);
	//RecoImage* GetImage(const char *file);
	int SaveImage(const char *file, const RecoImage *image);
	void ReleaseImage(RecoImage *image);

	//初始化
	int RWFaceMng_BlackInit(void);
	int RWFaceMng_TempInit(void);
	int RWFaceMng_UnInit(void);

	//人脸库管理
	bool RWFaceMngDbAddNew(long &dbID,char *dbName);
	bool RWFaceMngDbDeleteWithID(long dbID);

	//人员管理
	bool RWFaceMngPersonAddNew(long &personID,RecoPersonInfo pInfo);
	bool RWFaceMngPersonDeleteWithID(long personID);
	bool RWFaceMngPersonUpdate(RecoPersonInfo pInfo);
	bool RWFaceMngPersonRead(long personID,RecoPersonInfo &pInfo);

	//人脸管理
	bool RWFaceMngFaceAddNew(long &faceID,long personID,HBITMAP pic);
	bool RWFaceMngFaceDeleteWithID(long faceID);
	bool RWFaceMngGetFaceImage(long faceID,long &size,char *path);

	//添加临时人员及人脸
	bool RWFaceMngRegistTempPerson(long dbID,int age,char *sex,int width,int height,int step,
									unsigned char *image,long size,long &personID,long &faceID);
};
