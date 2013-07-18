#ifndef RECONOVA_RW_FEATURE_LIST_EVENT_H__
#define RECONOVA_RW_FEATURE_LIST_EVENT_H__


/**
 * @brief 视频事件类型
 */
enum RwVideoEventType{
	/** 进入 */
	FL_EVENT_TYPE_ENTER,
	/** 活动 */
	FL_EVENT_TYPE_ACTIVITY,
	/** 离开 */
	FL_EVENT_TYPE_LEAVE,
};


/**
 * @brief 视频事件
 */
typedef struct tagRwVideoEvent
{
	/** 事件类型 */
	RwVideoEventType event_type;
	/** 事件时间 */
	long time;
	/** 跟踪号 */
	int track_no;
	/** 特征队列和图片队列的长度 */
	int list_size;
	/** 特征队列 */
	RwFeature **feature_list;
	/** 图片队列 */
	RwImage **image_list;
}RwVideoEvent;


#endif