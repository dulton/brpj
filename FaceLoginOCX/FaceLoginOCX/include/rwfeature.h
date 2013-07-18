#ifndef RECONOVA_RW_FEATURE_H__
#define RECONOVA_RW_FEATURE_H__


/**
 * @brief 人脸特征信息
 */
typedef struct tagRwFeature
{
	/** 人脸特征指针 */
	void *feature;
	/** 特征大小,通过ReleaseFaceRecogInstance接口返回 */
	int feature_size;
}RwFeature;

#endif