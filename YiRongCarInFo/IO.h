#ifndef _IO_
#define _IO_

 #define _AFXDLL
#include <afx.h>
#include <list>

#import "msado15.dll" no_namespace rename("EOF","adoEOF")       //引入ADO类型库

using namespace::std;


struct BLACK_STRUCT
{
	CString PinPai;
	CString ChePaiHao;
	CString CheJiaHao;
	CString DianJiHao;

	CString Name;
	CString Phone;
};

struct HISTORY_STRUCT
{
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
	char ip[64];
	char carNumber[32];
	char path[260];

	struct BLACK_STRUCT info;
};

//struct RESULT_STRUCT
//{
//	char ip[64];
//	char carNumber[32];
//	char path[260];
//}

class IO
{
public:
	IO(void);
	~IO(void);

	_ConnectionPtr m_pConnection;					//创建oracle connection对象

	_RecordsetPtr m_pRecordsetPtr;					//创建数据记录集
	_RecordsetPtr m_pRecordsetPtrInfo;					//读取电动车信息列表

	TCHAR Ip[256];
	TCHAR Port[256];
	TCHAR User[256];
	TCHAR Psw[256];
	TCHAR DataBaseName[256];
	bool ReadFile(char* FileName);

	bool ConnectionOracleDBTXT(char*  FileName);

	bool ConnectionOracleDB(char* strConn);			//连接oracle数据库

	bool DisConnectionOracleDB(void);				//断开与oracle数据库的连接
	//guan
	bool writeMatchResultVehiclePath(char *IpAddr,char *CarStr,char *path);
	bool readMatchResultVehiclePath(HISTORY_STRUCT &CarInfo);
	bool readAlarmVehicle(HISTORY_STRUCT &AlarmInfo);
	bool readAllAlarmVehicle(list<HISTORY_STRUCT>& AlarmList);
	bool readVehicleInfo(HISTORY_STRUCT &AlarmInfo,int ddcid);
	bool readAllMatchResultVehicle(list<HISTORY_STRUCT>& MatchResultList);

	bool getAlarmVehicleWithTime(char *startTime,char *endTime,list<HISTORY_STRUCT>& AlarmList);
	bool getAlarmVehicleWithCarNum(char *CarNum,list<HISTORY_STRUCT>& AlarmList);
	bool getMatchResultVehicleWithTime(char *startTime,char *endTime,list<HISTORY_STRUCT>& MatchResultList);
	bool getMatchResultVehicleWithCarNum(char *CarNum,list<HISTORY_STRUCT>& MatchResultList);
	//lynn
	bool writeMatchResultVehiclePicture(char *IpAddr,char *CarStr,unsigned char *Image,long int ImageSize);
	bool readMatchResultVehicle(CString path,HISTORY_STRUCT &CarInfo);//路径格式(D:\\TEMP\\)
	bool ExecuteInsertAlarmVehicleProcedure(int nid, int *iresult, char* serror);
	bool UpdateDataState(char *table,char *str,int ID);
	//bool updateDataState(CString stablename,int nflag,int nid);
};



#endif