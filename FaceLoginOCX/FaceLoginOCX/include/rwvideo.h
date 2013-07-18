#ifndef RECONOVA_VIDEO_H__
#define RECONOVA_VIDEO_H__

#ifdef __linux
#define RWVIDEO_API
#else

#ifdef RWVIDEO_EXPORTS
#define RWVIDEO_API __declspec(dllexport)
#else
#define RWVIDEO_API __declspec(dllimport)
#endif

#endif


#include "rwinit.h"
#include "rwimage.h"
#include "rwfeature.h"
#include "rwtrackdata.h"
#include "rwvideoevent.h"


/** 视频接口返回列表最大长度 */
#define MAX_FEATURE_COUNT 20


/** 视频句柄 */
typedef void * RwVideoHandle;


/**
 * 获取视频句柄
 * @param [in] video_handle 视频句柄
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWVIDEO_API int CreateVideoHandle(
	RwModuleHandle module_handle,
	RwVideoHandle *video_handle
);

/**
 * 释放视频句柄
 * @param [in] video_handle 视频句柄
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWVIDEO_API int ReleaseVideoHandle(RwVideoHandle *video_handle);

/**
 * 重置视频句柄
 * 在新的一段视频识别或者注册前需要重置视频句柄
 * @param [in] video_handle 视频句柄
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWVIDEO_API int ResetVideoHandle(RwVideoHandle video_handle);

/**
 * 如果是视频识别，需要把特征加入到历史队列中
 * @param [in] video_handle 视频句柄
 * @param [in] cur_time 视频时间
 * @param [in] similarity 相对相似度
 * - 一个人的两个特征必须大于相对相似度
 * @param [in] list_size 列表大小
 * @param [in] feature_list 特征列表
 * @param [in] image_list 人脸截图列表
 * @param [in] trackdata_list 人脸跟踪信息列表
 * @param [out] event_count 事件数量
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWVIDEO_API int AddToDetect(
	RwVideoHandle video_handle,
	long cur_time,
	double similarity,
	int list_size,
	RwFeature *feature_list,
	RwImage *image_list,
	RwTrackData *trackdata_list,
	int *event_count
);

/**
 * 获取视频队列的结果
 * @param [in] video_handle 视频句柄
 * @param [out] feature_event_list 视频事件列表
 * - 最大长度MAX_FEATURE_COUNT
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWVIDEO_API int GetDetectResult(
	RwVideoHandle video_handle,
	RwVideoEvent *det_event_list
);


/**
 * 如果是视频注册，需要把特征加入到历史队列中，能够直接返回结果
 * @param [in] video_handle 视频句柄
 * @param [in] similarity 相对相似度
 * - 注册的两个特征必须小于相对相似度
 * @param [in] feature 特征信息
 * @param [in] image 人脸图像信息
 * @param [in] trackdata 人脸跟踪信息
 * @param [out] list_size 返回列表长度
 * - 最大长度MAX_FEATURE_COUNT
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWVIDEO_API int AddToRegister(
	RwVideoHandle video_handle,
	double similarity,
	RwFeature feature,
	RwImage image,
	RwTrackData trackdata,
	int *list_size
);

/**
 * 获取注册的结果
 * @param [in] video_handle 视频句柄
 * @param [out] feature_event_list 注册事件
 * - 最大长度MAX_FEATURE_COUNT
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWVIDEO_API int GetRegisterResult(
	RwVideoHandle video_handle,
	RwVideoEvent *reg_event
	);

#endif