#ifndef RECONOVA_RW_TRACK_DATA_H__
#define RECONOVA_RW_TRACK_DATA_H__


/**
 * @brief 跟踪信息
 */
typedef struct tagRwTrackData
{
	/** 跟踪信息1 */
	int data1;
	/** 跟踪信息2 */
	int data2;
	/** 跟踪信息3 */
	int data3;
	/** 跟踪信息4 */
	int data4;
	/** 预留信息大小 */
	int buff_size;
	/** 预留信息指针 */
	void* pri_buff;
}RwTrackData;

#endif