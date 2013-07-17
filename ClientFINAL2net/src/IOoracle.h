#ifndef _ZOG_ORACALE_IO
#define _ZOG_ORACALE_IO
#import "msado15.dll" no_namespace rename("EOF","adoEOF")       //引入ADO类型库
#include <vector>
#include <io.h>

using namespace::std;

//ConnectionOracleDB 数据库连接函数 返回值 定义
#define YIRONGDB_ReadFile_FAIL -1	//配置文件打开失败
#define YIRONGDB_Instance_FAIL -2	//数据库ADO初始化失败
#define YIRONGDB_ContOpen_FAIL -3	//数据库无法连接-账号密码IP异常
#define YIRONGDB_Connectd_DONE 1	//数据库连接成功	

#define ORACLE_CONFIG_TXT _T("oracle_config.txt")

class IOoracle
{
public:
	IOoracle(void);
	~IOoracle(void);

	_ConnectionPtr m_pConnection;             //创建oracle connection对象

	int ConnectionOracleDB(TCHAR*  FileName);	   //连接oracle数据库

	bool DisConnectionOracleDB(void);		   //断开与oracle数据库的连接

	void ReadInfoFromOracleDB_TORG(CString str, vector<CString> &id,vector<CString> &name);

	void ReadInfoFromOracleDB_TBCAMERAINFO(CString str,vector<int> &id, vector<CString> &live,vector<CString> &ip,
			vector<CString> &user, vector<CString> &pwd,vector<CString> &channel,vector<CString> &port);

	BOOL GetSysdateFromOracleDB(char *str,
								  int *year,int *mon,int *day,
									  int *hour,int *min,int *sec);
	BOOL GetSysdateFromOracleDB_DATE( int *year,int *mon,int *day,
									  int *hour,int *min,int *sec);

	void WriteInfoFromOracleDB_TBVIDEOINFO(long int camid,	\
		const char *path,const char *filename,CTime stime,long int size,char *type);

	void WriteInfoFromOracleDB_NETVIDEOINFO(long int camid,	\
												 const char *path,const char *filename,	\
												 CTime stime,CTime etime,long int size,char *type);

	long int GetNID_VIDEO();
	void VIDEO_PRO(long int nid);

};

#endif