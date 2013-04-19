/***********************************************************************************
//ver :YiRongCarDetectAIO
//time:2013-03-29 11:39:00
***********************************************************************************/

#ifndef _IO_H_
#define _IO_H_

#include <afx.h>
#include <vector>
#include <list>

#import "msado15.dll" no_namespace rename("EOF","adoEOF")       //引入ADO类型库
using namespace::std;

//命名太长会出错
struct HISTORY_REPORT_ST
{
	//起始时间
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;

	char user[260];	//用户名
	char str[1024];	//系统信息
};

struct DEVICE_AREA_ST
{
	//数据库的NID
	unsigned long int nid;
	char name[32];
};

//设备信息
typedef struct 
{
	HTREEITEM Parent_item;		//父指针
	HTREEITEM item;				//本指针
	long int camID;
	CString area;				//区域
	CString name;				//摄像机名称
	CString ip;
	CString user;
	CString psw;
	long port;
}IPLIST;


struct SET_USER_LIST_ST
{
	//数据库的NID
	unsigned long int nid;
	char user[32];
};

//用户信息
struct SET_USER_DATA_ST
{
	//数据库的NID
	unsigned long int nid;
	char user[32];
	char psw[260];
	//管理员权限
	int admin;
	//权限
	//系统设置
	int	systemset;
	//录制设置
	int	recordset;
	//录制
	int	record;
	//云台
	int	ptz;
	//预览
	int	preview;
	//历史视频查询
	int	historyvideo;
	//系统日志查询
	int	historyreport;
	//历史识别查询
	int	historydetect;
	//历史报警查询
	int	historyalarm;
	//设备管理
	int	device;
	//黑名单设置
	int	blackset;
	//抓拍
	int	capbmp;
	//识别
	int	detect;
	//识别设置
	int	detectset;
};

//命名太长会出错
struct HISTORY_DETECT_ST
{
	//nid
	unsigned long int nid;
	char area[260];	//区域名称
	char name[260];	//摄像头名称
	char ip[32];	//IP地址

	//时间
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;

	//车牌类型
	char	platetype[32];
	//车牌颜色
	char	platecolor[32];
	//车牌方向
	char	direction[32];
	//车身颜色
	char	carcolor[32];
	//车牌号
	char	plate[32];
	//置信度
	int reliability;
	
	//文件路径
	char path[260];
	//图像大小
	unsigned long int picsize;
};

struct BLACK_DATA_ST
{
	unsigned long int nid;
	char brand[256];	//品牌
	char name[256];	//失主姓名
	char Phone[256];	//失主电话
	char plate[256];	//车牌号
	char other[1024];	//其他信息
};

//ConnectionOracleDB 数据库连接函数 返回值 定义
#define ReadFile_FAIL -1	//配置文件打开失败
#define Instance_FAIL -2	//数据库ADO初始化失败
#define ContOpen_FAIL -3	//数据库无法连接-账号密码IP异常
#define Connectd_DONE 1		//数据库连接成功	

class IO
{
public:
	TCHAR Ip[256];
	TCHAR Port[256];
	TCHAR User[256];
	TCHAR Psw[256];
	TCHAR DataBaseName[256];
	_ConnectionPtr m_pConnection;					//创建oracle connection对象
	_RecordsetPtr m_pRecordsetPtr;					//创建数据记录集
	bool state;

public:
	IO(void);
	~IO(void);
	bool DisConnectionOracleDB(void);				//断开与oracle数据库的连接
	void DeCode(char *src,char *dst);				//解密
	bool ReadFile(char* FileName);					//读数据库配置文件
	int ConnectionOracleDBTXT(char* FileName);		//连接数据库
	/**************************设备表*******************************/
	bool DEVICE_ReadAllOrgName(vector<struct DEVICE_AREA_ST>& DeviceAreaList);	//读取所有的区域
	bool DEVICE_AddNewArea(CString AreaName);					//新增区域
	bool DEVICE_DeleteArea(int areaID);							//删除区域
	bool DEVICE_AddNewCamera(CString AreaName,CString CamName,CString sip,int port,CString user,CString password);
	bool DEVICE_ReadCameraInfo(char* AreaName,vector<IPLIST>& CameraList);			//读某个区域所有的摄像机信息
	bool DEVICE_DeleteCamera(int cameraID);											//删除摄像机
	bool DEVICE_DeleteCameraWithAreaID(int AreaID);									//删除某个区域的所有摄像机
	bool DEVICE_UpdateCameraInfo(long CamID,CString AreaName,\
								CString CamName,CString sip,int port,CString user,CString password);

	/**************************用户表*******************************/
	bool USER_AddNewUser(struct SET_USER_DATA_ST UserInfo);				//新增用户信息
	bool USER_DeletetUser(unsigned long int nid);						//删除用户信息
	bool USER_UpdateUserInfo(struct SET_USER_DATA_ST UserInfo);			//编辑用户信息
	bool USER_ReadUserInfoWithName(char *user,struct SET_USER_DATA_ST *UserInfo);
	bool USER_ReadUserInfoWithNid(unsigned long int nid,struct SET_USER_DATA_ST *UserInfo);
	bool USER_ReadAllUserInfo(list<struct SET_USER_LIST_ST> &UserInfo);

	/**************************系统日志表*******************************/
	bool LOG_AddNewSystemLog(char *userName,char *logStr);				//新增系统日志
	unsigned long LOG_GetSystemLogNum(char *userName,char *startTime,char *endTime,int flag,char *SQLstr);
	bool LOG_ReadSystemLog(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_REPORT_ST> &HistoryList);
	
	/**************************汽车结果表、报警表*******************************/
	bool CAR_MatchResult_AddNew(bool *isblack,unsigned long CAMID,char *platetype,char *platecolor,char *direction,char *carcolor,
								char *plate,int reliability,char *path,unsigned long int picsize,unsigned char *picdata);
	bool CAR_MatchResult_GetPicture(unsigned long NID,unsigned char *data);	//获取比对结果表图片数据
	bool CAR_AlarmResult_GetPicture(unsigned long NID,unsigned char *data);	//获取报警表图片数据
	unsigned long CAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
					char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr);
	unsigned long CAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
					char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr);
	bool CAR_MatchResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList);
	bool CAR_AlarmResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList);
	bool CAR_AlarmResult_Execute(int nid,bool *isblack);

	/**************************电动车结果表、报警表*******************************/
	bool ELECAR_MatchResult_AddNew(bool *isblack,unsigned long CAMID,char *direction,char *plate,int reliability,
									char *path,unsigned long int picsize,unsigned char *picdata);
	bool ELECAR_MatchResult_GetPicture(unsigned long NID,unsigned char *data);	//获取电动车比对结果表图片数据
	bool ELECAR_AlarmResult_GetPicture(unsigned long NID,unsigned char *data);	//获取报警表图片数据
	unsigned long ELECAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
											char *direction,int flag,char *SQLstr);
	unsigned long ELECAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
											char *direction,int flag,char *SQLstr);
	bool ELECAR_MatchResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList);
	bool ELECAR_AlarmResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList);
	bool ELECAR_AlarmResult_Execute(int nid,bool *isblack);

	/**************************汽车黑名单表*******************************/
	bool CAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo);
	bool CAR_BlackTable_DeleteWithNid(unsigned long int nid);
	bool CAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo);
	unsigned long CAR_BlackTable_GetNum(void);
	bool CAR_BlackTable_Read(int startNum,int endNum,list<struct BLACK_DATA_ST> &BlackList);
	bool CAR_BlackTable_ReadOne(int Num,struct BLACK_DATA_ST &black);
	bool CAR_BlackTable_DeleteAll(void);

	/**************************电动车黑名单表*******************************/
	bool ELECAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo);
	bool ELECAR_BlackTable_DeleteWithNid(unsigned long int nid);
	bool ELECAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo);
	unsigned long ELECAR_BlackTable_GetNum(void);
	bool ELECAR_BlackTable_Read(int startNum,int endNum,list<struct BLACK_DATA_ST> &BlackList);
	bool ELECAR_BlackTable_ReadOne(int Num,struct BLACK_DATA_ST &black);
	bool ELECAR_BlackTable_DeleteAll(void);


	/*********************与平台连接的IO****************************/
#if YRVM_PINGTAI_MODE

	bool YRVM_state;
	_ConnectionPtr YRVM_pConnection;					//创建oracle connection对象

	bool YRVM_DisConnectionOracleDB(void);				//断开与oracle数据库的连接
	int YRVM_ConnectionOracleDBTXT(char* FileName);		//连接数据库

	// 获取数据库临时电动车表的nid
	unsigned long int YRVM_getElectricCarOracleTempNid(void);
	//写电动车临时照片表
	bool YRVM_writeElectricCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id);
	//写临时电动车信息表
	bool YRVM_writeElectricCarTempInfoToOracleDB(char *CarStr,char* Color,int Direction,char *Reliability,unsigned long int id);

	// 获取数据库临时机动车表的nid
	unsigned long int YRVM_getCarOracleTempNid(void);
	// 写机动车临时照片表
	bool YRVM_writeCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id);
	// 写临时机动车信息表
	bool YRVM_writeCarTempInfoToOracleDB(char *CarStr,char* PlateColor,int Direction,
		char *Reliability,char* Type,char *CarColor,unsigned long int id);

	//电动车车牌存储过程
	void YRVM_ExecuteInsertMatchVehicleResultProcedure(unsigned long int id);
	//车牌存储过程
	void YRVM_ExecuteInsertMatchCarResultProcedure(unsigned long int id);

#endif


};



#endif