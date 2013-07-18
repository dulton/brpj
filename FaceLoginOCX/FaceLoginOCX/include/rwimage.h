#ifndef RECONOVA_RW_IMAGE_H__
#define RECONOVA_RW_IMAGE_H__

/**
 * @brief 图像信息
 */
typedef struct tagRwImage
{
	/** 图像数据 */
	unsigned char *imagedata;
	/** 图像数据大小 */
	int image_size;
	/** 图像宽 */
	int width;
	/** 图像高 */
	int height;
	/** 图像行大小 */
	int widthStep;
}RwImage;

#endif