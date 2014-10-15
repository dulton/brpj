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

struct  S_DETECT
{
	long id;
	char ip[32];
	long isoccupy;
	long channel;
	long isenable;

};

struct  S_CAMERA
{
	long camid;
	char name[128];
	char ip[128];
	char rtspurl[1024];
	long isplay;

};

struct  S_MISSION
{
	long missionid;
	long userid;
	char time[32];
	long isplay;
};


typedef struct S_DEVICE_LIST
{
	struct S_MISSION mission;
	struct S_CAMERA camera;
	struct S_DETECT detect;

}DEVICE_LIST;


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
	//写入锁。防止同时写入一张表 
	// 仅在 LOG_AddNewSystemLog 函数
	// VIDEO_AddNewVideo  函数
	//  CAR_MatchResult_AddNew  函数
	// ELECAR_MatchResult_AddNew 函数
	bool IOwriteLock;

	CString errormessage;
public:
	IO(void);
	~IO(void);

	void DeCode(char *src,char *dst);				//解密
	bool ReadFile(char* FileName);					//读数据库配置文件
	void VarSaveStr(TCHAR *dst,variant_t src);		//保存成字符串
	void VarSaveStr2(TCHAR *dst,_RecordsetPtr &p,TCHAR *name);	//保存成字符串
	void IO::VarSaveString2(CString &dst,_RecordsetPtr &p,TCHAR *name);
	long long VarSaveNumber2(_RecordsetPtr &p,TCHAR *name);
	double VarSaveDouble2(_RecordsetPtr &p,TCHAR *name);
	void filterstring(char *str);

	bool DisConnectionOracleDB(void);				//断开与oracle数据库的连接
	int ConnectionOracleDBTXT(char* FileName);		//连接数据库
	unsigned long long IO::AutoAddNid(TCHAR *TableName,TCHAR *NidName);

	bool IO::DETECTDEVICE_HeartTest(long maxtime);
	bool IO::DETECTDEVICE_ADD(char *ip,long channel);


	bool IO::MISSION_Edit(long long Missionid,long DetectDeviceid);
	bool IO::MISSION_Read(long long *Missionid,unsigned long int *camid,unsigned long int *isplay,unsigned long int *userid,char* indate);
	bool IO::MISSION_AddStop(long DetectDeviceid,unsigned long int camid);


	bool IO::DEVICE_ReadList(list<DEVICE_LIST> &DeviceList);

};
#endif