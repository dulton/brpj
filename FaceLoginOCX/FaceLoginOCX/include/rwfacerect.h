#ifndef RECONOVA_RW_FACE_RECT_H__
#define RECONOVA_RW_FACE_RECT_H__

/**
 * @brief 人脸信息
 */
typedef struct tagRwFaceRect
{
	/** 人脸的跟踪号 */
	int track_no;

	/** 人脸矩形框左上角的x坐标 */
	int left;
	/** 人脸矩形框左上角的y坐标 */
	int top;
	/** 人脸矩形框右下角的x坐标 */
	int right;
	/** 人脸矩形框右下角的y坐标 */
	int bottom;

	/** 人脸左眼的x坐标 */
	int lefteye_x;
	/** 人脸左眼的y坐标 */
	int lefteye_y;
	/** 人脸右眼的x坐标 */
	int righteye_x;
	/** 人脸右眼的y坐标 */
	int righteye_y;
	/** 人脸鼻子的x坐标 */
	int nose_x;
	/** 人脸鼻子的y坐标 */
	int nose_y;
	/** 人脸嘴巴的x坐标 */
	int centermouth_x;
	/** 人脸嘴巴的y坐标 */
	int centermouth_y;
}RwFaceRect;

#endif