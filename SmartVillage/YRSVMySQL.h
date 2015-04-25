
#ifndef _YRSV_MYSQL_H_
#define _YRSV_MYSQL_H_

#include <windows.h>
#include <mysql.h>
#pragma comment(lib,"libmysql.lib")

#include <vector>
#include <list>
#include <sstream>
#include <string>

using namespace::std;


#define MAX_FACE_DB_NUM 32
#define RCT_MAX_STR_SIZE 256
//车的省份
#define CAR_MASK_MAX 31

//性别定义
enum SEX_TYPE
{
	SEX_UNKNOW=0,		//未知
	SEX_MALE,				//男性
	SEX_FEMALE,			//女性
};

//卡类型定义
enum CARD_TYPE
{
	CARD_UNKNOW=0,				//未知
	CARD_GENERATION_ONE,	//一代证
	CARD_GENERATION_TWO,	//二代证
	CARD_GAS_CARDIN,			//士官证
	CARD_STUDENT,					//学生证
	CARD_DRIVER,					//驾驶证
	CARD_PASSPORT,				//护照
	CARD_OTHER,						//其他
};

//籍贯定义
enum NATIVE_PLACE_TYPE
{
	NATIVE_Unknow=0,			//未知
	NATIVE_BeiJing,				//北京
	NATIVE_TianJin,				//天津
	NATIVE_ChongQing,			//重庆
	NATIVE_GuangDong,		//广东
	NATIVE_HeiBei,				//河北
	NATIVE_ShanXi,				//山西
	NATIVE_LiaoNing,			//辽宁
	NATIVE_JiLin,					//吉林
	NATIVE_HeiLongJiang,	//黑龙江
	NATIVE_JiangSu,				//江苏
	NATIVE_ZheJiang,			//浙江
	NATIVE_AnHui,					//安徽
	NATIVE_FuJiang,				//福建
	NATIVE_JiangXi,				//江西
	NATIVE_ShangDong,			//山东
	NATIVE_HeNan,					//河南
	NATIVE_HuBei,					//湖北
	NATIVE_HuNan,					//湖南
	NATIVE_HaiNan,				//海南
	NATIVE_SiChuan,				//四川
	NATIVE_GuiZhou,				//贵州
	NATIVE_YunNan,				//云南
	NATIVE_ShanXi2,				//陕西
	NATIVE_GanSu,					//甘肃
	NATIVE_QingHai,				//青海
	NATIVE_TaiWan,				//台湾
	NATIVE_XinJiang,			//新疆
	NATIVE_GuangXi,				//广西
	NATIVE_NingXia,				//宁夏
	NATIVE_NeiMengGu,			//内蒙古
	NATIVE_XiZang,				//西藏
};

//定时录制计划表
struct SET_RECORD_DATA_ST
{
	//数据库的NID
	long nid;
	long ncamera;		//摄像头ID 
	char area[128];	//摄像头区域
	char name[64];	//摄像头名称
	char ip[32];		//IP地址
	
	//如果下面3个都是false 则仅今天有效
	int everyday;	//使用每天 bool 
	int usedate;	//使用日期 bool	 
	int useweek;	//使用每周 bool
	// 启用状态 启用则执行录制。不启用就只是备用 bool
	int enable;
	
	int StartWeek; //0 周日 1-6 周一到周六
	int EndWeek;	//0 周日 1-6 周一到周六

	//开始时间
	int StartYear;	
	int StartMon;
	int StartDay;
	int StartHour;
	int StartMin;
	int StartSec;
	//结束时间
	int EndYear;
	int EndMon;
	int EndDay;
	int EndHour;
	int EndMin;
	int EndSec;
};

//视频录像信息表
struct HISTORY_VIDEO_ST
{
	long nid;
	long ncamera;		//摄像头ID new
	char name[260];	//摄像头名称
	char ip[32];		//IP地址
	int venderID;		//厂家
	char format[8];	//后缀MP4 AVI 格式
	long size;	//文件大小
	char path[260];

	//开始时间
	int StartYear;
	int StartMon;
	int StartDay;
	int StartHour;
	int StartMin;
	int StartSec;
	//结束时间
	int EndYear;
	int EndMon;
	int EndDay;
	int EndHour;
	int EndMin;
	int EndSec;
};

//系统日志表
struct HISTORY_REPORT_ST
{
	//时间
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;

	char user[32];	//用户名
	char str[1024];	//日志信息
};

//区域表
struct DEVICE_AREA_ST
{
	long nid;
	char name[128];
};

//摄像头信息表
typedef struct
{
	HTREEITEM Parent_item;		//父指针
	HTREEITEM item;						//本指针
	long ncamera;
	long areaID;						//new
	char area[128];						//区域
	char name[64];						//摄像机名称
	char ip[32];
	char user[32];
	char psw[128];
	long port;
	long venderID;			//0---海康,1---大华
	long channel;
	char Rtspurl[512];	//rtsp流地址
	long RTP;						//流媒体传输模式0=RTP-RTSP，1=RTP-UDP
	long DecodeTag;			//流媒体解码标志0=haikang，1=ga_h264
	char longitude[32]; //经度
	char latitude[32];	//纬度
	long userID;					//USER表NID
}IPLIST;

struct  DEVICE_SET_CAR_LITE_ST
{
	int redrect;			//截图车牌带红框
	int deskew;				//车牌倾斜校正
	int jpgquailty;		//JPG图片质量
	int minwidth;			//车牌最小宽度
	int maxwidth;			//车牌最大宽度
	int left;					//检测区域-左
	int right;				//检测区域-右
	int top;					//检测区域-上
	int bottom;				//检测区域-下
	char defaultchar[32];	//默认省份
	int reliability;	//汉字置信度NRELIABILITY
	int carlor;				//车身颜色识别
	unsigned char mask[CAR_MASK_MAX];	//出现较多的省份
};

struct  DEVICE_SET_FACE_LITE_ST
{
	//FaceProcess 人脸检测时用
	int minface;			//最小人脸尺寸 SDK最小尺寸 32*32=1024
	int maxface;			//最大人脸尺寸 SDK最小尺寸 240*240=57600
	float frontface_ts;		//正面阀值  0.5  [0-1.0]
	float sideface_ts;		//侧面阀值  0.5  [0-1.0]

	//RecoVideo
	int time_out;			//去重时间 秒   30
	float scale_ratio;		//缩放比        1.0
	int video_ts;			//查重阀值		50 [1-100] 
 
	//RecoFaceMng 人脸管理库 人脸入库报警比对时 用
	int threshold;			//最小相似度阀值	50 [1-100] 
	int maxcount;			//最大匹配数		10

	int dbTotal;			//设置布控数量
	long db_nid[MAX_FACE_DB_NUM];		//设置布控库ID

};

//摄像头分析设置表
struct DEVICE_SET_ST
{
	long nid;				//序列号
	long ncamera;		//摄像头id
	int svmode;				//智能分析模式 0=车牌识别，1=人脸识别
	int width;				//视频图像宽
	int hight;				//视频图像高

	struct  DEVICE_SET_FACE_LITE_ST face;
	struct  DEVICE_SET_CAR_LITE_ST car;
};

//用户表
struct SET_USER_LIST_ST
{
	long nid;
	char user[128];
};

//用户表  用户信息
struct SET_USER_DATA_ST
{
	long nid;
	char user[128];
	char psw[128];
	
	//权限
	int admin;//管理员权限
	int	systemset;//系统设置
	int	recordset;//录制设置
	int	record;//录制
	int	ptz;//云台
	int	preview;//预览
	int	historyvideo;//历史视频查询
	int	historyreport;//系统日志查询
	int	historydetect;//历史识别查询
	int	historyalarm;//历史报警查询
	int	device;//设备管理
	int	blackset;//车牌黑名单设置
	int	capbmp;//抓拍
	int	Cardetect;//车牌识别报警
	int	CarDetectset;//车牌识别设置
	int Facedetect;//人脸识别
	int FaceDetectset;//人脸识别设置
};

//车牌识别结果表和报警表
struct HISTORY_CarDetect_ST
{
	long nid;
	long ncamera;				//摄像头编号
	char area[64];				//区域名称
	char name[32];				//摄像头名称
	char ip[32];					//IP地址
	
	char plate[32];//车牌号
	char platecolor[32];//车牌颜色
	char platetype[32];//车牌类型
	char direction[32];//车牌方向
	char carcolor[32];//车身颜色
	int reliability;//置信度
	
	//捕获时间DGETTIME
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;

	int nflag;						//标识
	char path[260];				//文件路径
	long picsize;					//图像大小
	char smallpath[260];	//车牌照片路径 new
	long smallsize;				//车牌照片大小 new
	long blackid;				//黑名单ID
};

//车辆黑名单表
struct BLACK_DATA_ST
{
	long nid;						//序列号
	char plate[32];				//车牌号
	char brand[32];				//品牌
	char name[32];				//失主姓名
	char Phone[32];				//联系电话
	char other[1024];			//其他信息
};

//人脸布控库信息表
struct FACE_DB_ST
{
	long	nid;		//序列号
	char dbname[32];	//布控库名称
	long	RW_dbID;			//布控库ID
};

//人员布控信息表
struct PERSON_BLACK_ST
{
	long	nid;					//序列号
	long	db_nid;				//库序列号
	long	RW_dbID;			//布控库ID
	long	RW_personID;	//布控人员ID
	char name[32];				//姓名

	//出生日期
	int year;
	int mon;
	int day;
	
	int sex;					//性别  0--未知  1--男性  2--女性
	char phone[32];		//电话
	int	cardtype;			//证件类型
	char cardNO[32];		//证件号
	int native;				//籍贯
	char address[512];//地址
};

//布控人脸信息表
struct FACE_BLACK_ST
{
	long	nid;								//序列号
	long	person_black_nid;		//布控人员表序列号
	long	RW_faceID;					//人脸ID
	long	RW_personID;				//布控人员ID
	char file[260];							//照片路径
	long size;									//照片大小
	char face[260];
	long facesize;
};

//临时布控库信息表
struct FACE_TEMP_DB_ST
{
	long nid;			//序列号
	char name[64];	//摄像机名称
	long RW_tempID;	//临时库ID
	long ncamera;		//摄像机ID
};

//人脸抓拍表
struct FACE_CAPTURE_ST
{
	long nid;			//序列号
	int sex;			//性别
	int	age;				//年龄
	int	facetype;		//脸类型
	int	quality;		//照片质量
	char file[260];	//现场照片路径
	int	size;				//现场照片大小
	char face[260];	//小 人脸图片路径
	int	facesize;		//小 人脸图片大小
	int	isblack;		//报警标志
	long	ncamera;	//摄像头编号
	char cameraName[64];	//摄像头名称
	char sip[32];		//摄像头IP
	int	deal;				//是否已比对处理标志	
	
	//捕获时间DGETTIME
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
	
	long temp_db_nid;	//抓拍库序列号
	long RW_tempID;		//临时库ID
	long RW_personID;	//临时人员ID
	long RW_faceID;		//人脸ID
};

//人脸报警表
struct FACE_ALARM_ST
{
	long nid;					//序列号
	long captureID;		//抓拍表ID
	long db_nid;		//tb_face_db的NID
	long RW_dbID;			//人脸库ID
	long RW_personID;	//布控人员ID
	long RW_faceID;		//人脸ID 其中一张图片
	int similarity;			//相似度
	long ncamera;
	char cameraName[64];	//摄像头名称
	char sip[32];		//摄像头IP
};

//================================视图===============================///
//人脸报警视图信息
struct FACE_ALARM_VIEW_ST
{
	long captureID;		//抓拍表ID
	long db_nid;		//tb_face_db的nid
	long	person_black_nid;	
	long RW_dbID;			//人脸库ID
	long	RW_personID;	//布控人员ID
	long RW_faceID;		//人脸ID
	
	char dbname[32];			//布控库名称
	char name[32];				//姓名
	//出生日期
	int year;
	int mon;
	int day;
	int sex;						//性别  0--未知  1--男性  2--女性
	char phone[32];			//电话
	int	cardtype;				//证件类型
	char cardNO[32];			//证件号
	int native;					//籍贯
	char address[512];	//地址	
	char file[260];		//布控原始图片
	int size;
	char face[260];	//布控照片
	int facesize;		//照片大小
	int similarity;			//相似度
	long	ncamera;	//摄像头编号
	char cameraName[64];	//摄像头名称
	char sip[32];		//摄像头IP
};

//ConnectionOracleDB 数据库连接函数 返回值 定义
#define ReadFile_FAIL -1	//配置文件打开失败
#define MySQLInit_FAIL -2	//mysql初始化错误
#define ContOpen_FAIL -3	//数据库无法连接
#define Connectd_DONE 1		//数据库连接成功	

class CYRSVMySQL
{
public:
	TCHAR Ip[64];
	TCHAR Port[64];
	TCHAR User[128];
	TCHAR Psw[128];
	TCHAR DataBaseName[128];

	MYSQL myConnection;		//MySQL 连接对象
	MYSQL *mysql_handle;
	bool m_bConnection;		//连接标志
	bool IOwriteLock;

public:
	CYRSVMySQL(void);
	~CYRSVMySQL(void);

	void DeCode(char *src,char *dst);				//解密
	bool ReadFile(char* FileName);					//读数据库配置文件
	void filterstring(char *str);					//去除插入数据库时出现单引号
	int ConnectionMySQL(char* FileName);			//连接数据库
	void DisConnectionMySQL(void);					//断开连接
	void VarSaveString(char *dst,char *src);		//保存字段为字符串
	long VarSaveNumber(char *src);		//保存字段为数字
	void StrSaveArray(char *src,long *dst);			//布控库字符串转换成数组
	void ArraySaveStr(long *src,long count,char *dst);
	void UCHARSaveCHAR(unsigned char *src,long count,char *dst);
	void StrReplace(char*src, char*sub, char*dst);

	/**************************区域表*******************************/
	bool DEVICE_GetAreaID(long &nid,char *AreaName);							//获取区域ID，可用于判断区域是否存在
	bool DEVICE_AddNewArea(char *AreaName);										//新增区域
	bool DEVICE_DeleteArea(long areaID);										//删除区域
	bool DEVICE_ReadAllOrgName(vector<struct DEVICE_AREA_ST>& DeviceAreaList);	//读取所有的区域
	bool DEVICE_UpdateAreaInfo(long areaID,char *AreaName);						//修改区域名称

	/**************************设备表*******************************/
	bool DEVICE_GetCameraID(long &nid,long areaID,char *name);					//获取设备ID，可用于判断设备是否存在
	bool DEVICE_AddNewCamera(long &ncamera,IPLIST CamInfo);						//新增设备
	bool DEVICE_ReadCameraInfo(long areaID,vector<IPLIST>& CameraList);			//读某个区域所有的设备信息
	bool DEVICE_DeleteCamera(long cameraID);									//删除摄像机
	bool DEVICE_DeleteCameraWithAreaID(long AreaID);							//删除某个区域的所有摄像机
	bool DEVICE_UpdateCameraInfo(IPLIST CamInfo);								//更新设备信息
	bool DEVICE_JudgeCamera(long ncamera);										//判断设备是否存在
	bool DEVICE_GetCameraWithAreaID(long AreaID,list<long> &ncameraList);		//获取某个区域下的所有摄像机ID
	bool DEVICE_ReadCameraInfoFromCamID(long ncamera,IPLIST &CamInfo);			//根据nid获取摄像机信息
	bool DEVICE_ReadCameraInfoFromCarDetectID(long CarDetectID,IPLIST &CameraInfo);	//不用

	/**************************用户表*******************************/
	bool USER_UserNameJudge(long &nid,char *userName);					//判断用户名是否存在
	bool USER_AddNewUser(struct SET_USER_DATA_ST UserInfo);				//新增用户信息
	bool USER_DeletetUser(long nid);									//删除用户信息
	bool USER_UpdateUserInfo(struct SET_USER_DATA_ST UserInfo);			//编辑用户信息
	bool USER_ReadUserInfoWithName(char *user,struct SET_USER_DATA_ST *UserInfo);
	bool USER_ReadUserInfoWithNid(long nid,struct SET_USER_DATA_ST *UserInfo);
	bool USER_ReadAllUserInfo(list<struct SET_USER_LIST_ST> &UserList);

	/**************************系统日志表*******************************/
	bool LOG_AddNewSystemLog(char *userName,char *logStr);				//新增系统日志
	long LOG_GetSystemLogNum(char *userName,char *startTime,char *endTime,int flag,char *SQLstr);
	bool LOG_ReadSystemLog(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_REPORT_ST> &HistoryList);

	/**************************汽车结果表、报警表*******************************/
	bool CAR_MatchResult_AddNewNoPicData(bool &isblack,long &nid,long &blackID,struct HISTORY_CarDetect_ST carInfo);
	bool CAR_Black_Compare(bool &isblack,long &blackID,char *carNum);//内部调用
	bool CAR_AlarmResult_AddNewNoPicData(struct HISTORY_CarDetect_ST carInfo);
	long CAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
		char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr);
	long CAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
		char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr);
	bool CAR_MatchResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_CarDetect_ST> &HistoryList);
	bool CAR_AlarmResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_CarDetect_ST> &HistoryList);
	long CAR_MatchResult_Count(long CAMID,char *startTime,char *endTime);//统计分析
	bool CAR_MatchResult_ReadforListOne(long nid,struct HISTORY_CarDetect_ST &CarData);
	bool CAR_AlarmResult_ReadforListOne(long nid,struct HISTORY_CarDetect_ST &CarData);

	/**************************汽车黑名单表*******************************/
	bool CAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo);
	bool CAR_BlackTable_DeleteWithNid(long nid);
	bool CAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo);
	long CAR_BlackTable_GetNum(void);
	bool CAR_BlackTable_Read(long startNum,long endNum,list<struct BLACK_DATA_ST> &BlackList);
	bool CAR_BlackTable_ReadOne(long Num,struct BLACK_DATA_ST &black);
	bool CAR_BlackTable_DeleteAll(void);
	bool CAR_Alarm_DeleteAll(void);
	bool CAR_Alarm_DeleteWithBlackNid(long blackid);
	bool CAR_BlackTable_ReadOneWithNid(long Nid,struct BLACK_DATA_ST &black);

	/**************************人脸布控库信息表*******************************/
	bool FACE_BlackDB_AddNew(long &nid,struct FACE_DB_ST faceDB);
	bool FACE_BlackDB_ReadAll(list<struct FACE_DB_ST> &FaceDBList);
	bool FACE_BlackDB_Edit(struct FACE_DB_ST faceDB);
	bool FACE_BlackDB_DeleteWithNid(long nid);
	bool FACE_BlackDB_GetRWID(long nid,long &RW_dbID);//获取瑞为库ID
	
	/**************************人员布控信息表*******************************/
	long FACE_BlackPerson_GetNum(long DBID);
	bool FACE_BlackPerson_Read(long DBID,long startNum,long endNum,list<struct PERSON_BLACK_ST> &BlackList);//获取某个布控库下的所有人员
	bool FACE_BlackPerson_ReadOne(long nid,struct PERSON_BLACK_ST &black);
	bool FACE_BlackPerson_AddNew(long &personID,struct PERSON_BLACK_ST black);
	bool FACE_BlackPerson_Edit(struct PERSON_BLACK_ST black);
	bool FACE_BlackPerson_DeleteWithNid(long nid);
	bool FACE_BlackPerson_DeleteWithDBID(long dbID);
	bool FACE_BlackPerson_CmpRead(struct PERSON_BLACK_ST &black,char *name,int sex,char *sage,char *eage,int cardtype,char *cardNO,
										  char *phone,int native,char *address,int flag,long db_id,long RW_DBID,long RW_PersonID);
	
	/**************************布控人脸信息表*******************************/
	bool FACE_BlackFace_Read(long PersonID,list<struct FACE_BLACK_ST> &FaceList);//根据人员ID获取，该人员的所有人脸
	bool FACE_BlackFace_AddNew(struct FACE_BLACK_ST face);
	bool FACE_BlackFace_Update(struct FACE_BLACK_ST face);
	bool FACE_BlackFace_DeleteWithNid(long nid);
	bool FACE_BlackFace_DeleteWithPersonID(long personID);
	bool FACE_BlackFace_ReadWithRWfaceID(long RW_faceID,long RW_personID,struct FACE_BLACK_ST &face);
	bool FACE_Black_Read_PersonAndFace(long faceID,long personID,struct PERSON_BLACK_ST &person,struct FACE_BLACK_ST &face);
	bool FACE_Black_Read_DetailBlack(long faceID,long personID,struct FACE_ALARM_VIEW_ST &alarmInfo);

	/**************************摄像头分析设置表*******************************/
	bool DEVICE_SetTable_AddNew(long ncamera);//添加摄像头分析设置，在添加摄像头的时候调用此函数添加默认设置
	bool DEVICE_SetTable_Read(long ncamera,struct DEVICE_SET_ST &CamSet);
	bool DEVICE_SetTable_Update(struct DEVICE_SET_ST CamSet);
	bool DEVICE_SetTable_DeleteWithCamID(long ncamera);

	/**************************临时布控库信息表*******************************/
	bool TEMP_DB_AddNew(struct FACE_TEMP_DB_ST tempDB);//添加摄像头临时布控库，在添加摄像头的时候调用此函数添加默认设置
	bool TEMP_DB_ReadAll(list<struct FACE_TEMP_DB_ST> &TempDBList);
	bool TEMP_DB_Read(long ncamera,struct FACE_TEMP_DB_ST &tempDB);
	bool TEMP_DB_UpdateWithNcamera(struct FACE_TEMP_DB_ST tempDB);
	bool TEMP_DB_DeleteWithNcamera(long ncamera);
	bool TEMP_DB_Update(struct FACE_TEMP_DB_ST tempDB);
	bool TEMP_DB_DeleteWithNid(long nid);
	bool TEMP_DB_Get_RWtempID(long ncamera,long &RWtempID);
	bool TEMP_DB_Get_RWtempID_List(list<long>&IDList);

	/**************************人脸抓拍表*******************************/
	bool FACE_Capture_AddNew(long &nid,struct FACE_CAPTURE_ST tempFace);
	long FACE_Capture_GetNum(long ncamera,int sex,int sage,int eage,char *startTime,char *endTime,int flag,char *SQLstr);
	bool FACE_Capture_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct FACE_CAPTURE_ST> &faceList);
	bool FACE_Capture_ReadforListOne(long nid,struct FACE_CAPTURE_ST &tempFace);
	bool FACE_Capture_GetUndealFace(struct FACE_CAPTURE_ST &tempFace);//获取未比对的抓拍记录
	bool FACE_Capture_UpdateDealFlag(long nid);//更新处理标志
	bool FACE_Capture_UpdateBlackFlag(long nid);//更新是否为黑名单标志
	long FACE_Capture_GetAlarmNum(long ncamera,int sex,int sage,int eage,char *startTime,char *endTime,int flag,char *SQLstr);
	bool FACE_Capture_ReadAlarmList(char *SQLstr,int flag,long startNum,long endNum,list<struct FACE_CAPTURE_ST> &faceList);
	bool FACE_Capture_CmpRead(struct FACE_CAPTURE_ST &tempFace,long RW_tempID,long RW_personID,long RW_faceID,
									  int sex,long ncamera,int flag,char *startTime,char *endTime);

	/**************************人脸报警表*******************************/
	bool FACE_Alarm_AddNew(struct FACE_ALARM_ST alarmFace);
	/**************************人脸报警视图*******************************/
	long VS_FACE_Alarm_GetNum(long ncamera,int sex,int sage,int eage,char *startTime,char *endTime,int flag,char *SQLstr);
	bool VS_FACE_Alarm_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct FACE_ALARM_VIEW_ST> &alarmList);
	long VS_FACE_Alarm_GetNum(long captureID);
	bool VS_FACE_Alarm_ReadWithCaptureID(long captureID,long startNum,long endNum,list<struct FACE_ALARM_VIEW_ST> &alarmList);
	bool VS_FACE_Alarm_ReadOne(long captureID,long personID,struct FACE_ALARM_VIEW_ST &alarmInfo);

	bool LOG_AddNewMysqlLog(char *userName,const char *logStr);//内部调用
};



#endif