#ifndef RECONOVA_FACE_RECOGNITION_H__
#define RECONOVA_FACE_RECOGNITION_H__

#ifdef __linux
#define RWFACERECOG_API
#else

#ifdef RWFACERECOG_EXPORTS
#define RWFACERECOG_API __declspec(dllexport)
#else
#define RWFACERECOG_API __declspec(dllimport)
#endif

#endif


#include "rwinit.h"
#include "rwimage.h"
#include "rwfeature.h"
#include "rwfacerect.h"
#include "rwtrackdata.h"


/** 人脸识别句柄 */
typedef void * RwFaceRecogHandle;

/** 图片检测最大返回人脸数 */
#define MAX_DET_RESULT_COUNT 32 


/**
 * 初始化人脸识别模块
 * @param [in] module_handle 底层模块句柄
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int InitializeFaceRecog(RwModuleHandle module_handle);

/**
 * 反初始化人脸识别模块
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int UnInitializeFaceRecog(void);


/**
 * 获取人脸识别句柄
 * @param [out] face_recog_handle 人脸识别句柄
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int CreateFaceRecogHandle(
	RwFaceRecogHandle *face_recog_handle
);

/**
 * 释放人脸识别句柄
 * @param [in] face_recog_handle 人脸识别句柄
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int ReleaseFaceRecogHandle(
	RwFaceRecogHandle *face_recog_handle
	);

/**
 * 获取人脸截图参数
 * @param [in] face_recog_handle 人脸识别句柄
 * @param [out] face_width 人脸截图图像的宽
 * @param [out] face_height 人脸截图图像的高
 * @param [out] face_width_step 人脸截图图像行大小
 * @param [out] face_data_size 人脸截图图像大小
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int GetFaceImageParam(
	RwFaceRecogHandle face_recog_handle,
	int *face_width,
	int *face_height,
	int *face_width_step,
	int *face_data_size
);

/**
 * 获取人脸截图参数
 * @param [in] face_recog_handle 人脸识别句柄
 * @param [out] feature_size 特征大小
 * - 提取特征的时候需要在外部申请特征内存
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int GetFeatureParam(
	RwFaceRecogHandle face_recog_handle,
	int *feature_size
);

/**
 * 单张图片人脸检测，获取人脸在图像中的人脸信息
 * @param [in] face_recog_handle 人脸识别句柄
 * @param [in] image 图像信息
 * @param [in] scale_ratio 缩放比例，在处理的时候，先缩放后处理
 * - 1.0f 不缩放
 * @param [out] list_size 图像中人脸个数
 * @param [out] face_rect_list 图像中人脸信息数组
 * - 图片检测最大返回人脸数MAX_DET_RESULT_COUNT
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int DetectFace(
	RwFaceRecogHandle face_recog_handle,
	RwImage &image,
	float scale_ratio,
	int *list_size,
	RwFaceRect *face_rect_list
);

/**
 * 视频人脸检测，获取人脸在图像中的人脸信息
 * @param [in] face_recog_handle 人脸识别句柄
 * @param [in] image 图像信息
 * @param [in] img_time 图像时间（毫秒）
 * @param [in] scale_ratio 缩放比例，在处理的时候，先缩放后处理
 * - 1.0f 不缩放
 * @param [out] list_size 图像中人脸个数
 * @param [out] face_rect_list 图像中人脸信息数组
 * @param [out] track_data_list 图像中跟踪信息数组与人脸信息对应
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int DetectFaceV(
	RwFaceRecogHandle face_recog_handle,
	RwImage &image,
	long img_time,
	float scale_ratio,
	int *list_size,
	RwFaceRect *face_rect_list,
	RwTrackData *track_data_list
);

/**
 * 提取单张图片中的特征
 * @param [in] face_recog_handle 人脸识别句柄
 * @param [in] image 图像信息
 * @param [in] scale_ratio 缩放比例，在处理的时候，先缩放后处理
 * - 1.0f 不缩放
 * @param [in&out] facerect 图像中单个人脸信息
 * - 返回时，写入跟踪号
 * @param [out] feature 特征信息
 * @param [out] face_image 人脸图像截图信息
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int ExtractFeature(
	RwFaceRecogHandle face_recog_handle,
	RwImage &image,
	float scale_ratio,
	RwFaceRect *facerect,
	RwFeature *feature,
	RwImage *face_image
	);

/**
 * 提取视频图片中的特征
 * @param [in] face_recog_handle 人脸识别句柄
 * @param [in] image 图像信息
 * @param [in] scale_ratio 缩放比例，在处理的时候，先缩放后处理
 * - 1.0f 不缩放
 * @param [in&out] facerect 图像中单个人脸信息
 * - 返回时，写入跟踪号
 * @param [in&out] track_data 跟踪信息
 * @param [out] feature 特征信息
 * @param [out] face_image 人脸图像截图信息
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWFACERECOG_API int ExtractFeatureV(
	RwFaceRecogHandle face_recog_handle,
	RwImage &image,
	float scale_ratio,
	RwFaceRect *facerect,
	RwTrackData *track_data,
	RwFeature *feature,
	RwImage *face_image
);

#endif