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

	_RecordsetPtr m_pRecordsetPtr;					//创建数据记录集

	//写入锁。防止同时写入一张表 
	// 仅在 LOG_AddNewSystemLog 函数
	// VIDEO_AddNewVideo  函数
	//  CAR_MatchResult_AddNew  函数
	// ELECAR_MatchResult_AddNew 函数
	bool IOwriteLock;


public:
	IO(void);
	~IO(void);
	void DeCode(char *src,char *dst);				//解密
	bool ReadFile(char* FileName);					//读数据库配置文件

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