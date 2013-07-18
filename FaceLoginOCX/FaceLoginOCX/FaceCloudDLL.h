/******************************************************************************************
V2013-02-05 10:30:00
V2013-03-26 10:30:00 修改log和照片的存储路径到Facecloud所在的路径
******************************************************************************************/
#ifndef _FACE_CLOUD_DLL_
#define _FACE_CLOUD_DLL_

#ifdef FACE_CLOUD_API
#else
#define FACE_CLOUD_API _declspec(dllimport)
#endif

#include "include/rwfacerect.h"

/***********************************************************************
//说明：初始化FaceCloud
//输入：DLLpath	---- DLL所在的位置
//输出：无
//返回：0		---- 初始化完成
        -1		---- 初始化失败
***********************************************************************/
extern "C" int FACE_CLOUD_API InitializeFaceCloud(char *DLLpath);

/***********************************************************************
//说明：反初始化FaceCloud
//输入：无
//输出：无
//返回：0  ---- 反初始化完成
        -1 ---- 反初始化失败
***********************************************************************/
extern "C" int FACE_CLOUD_API UnInitializeFaceCloud(void);

/***********************************************************************
//说明：提取特征
//输入：detectImage		---- 视频图像
		size			---- 视频图像大小
		width			---- 视频图像的宽
		height			---- 视频图像高
		list_size		---- 人脸个数
		face_rect_list	---- 人脸矩形框列表
//输出：无
//返回：0				---- 提取特征完成
        -1				---- 提取特征失败
***********************************************************************/
extern "C" int FACE_CLOUD_API YRDetectFace(unsigned char *detectImage, unsigned long int size, unsigned long int width, unsigned long int height, int &list_size, RwFaceRect *face_rect_list);


#endif