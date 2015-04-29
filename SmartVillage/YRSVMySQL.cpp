#include "StdAfx.h"
#include "YRSVMySQL.h"

#include "SmartVillage.h"
#include "SmartVillageDlg.h"

#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

CYRSVMySQL::CYRSVMySQL(void)
{
	mysql_handle = NULL;
	m_bConnection = false;
	IOwriteLock = false;
}

CYRSVMySQL::~CYRSVMySQL(void)
{
	mysql_handle = NULL;
	IOwriteLock = false;
	m_bConnection = false;
	DisConnectionMySQL();
}


//解密
void CYRSVMySQL::DeCode(char *src,char *dst)
{
	int a,b;
	int i,len,v;

	a=(src[0]-'0')*10+(src[1]-'0');
	b=(src[2]-'0')*10+(src[3]-'0');

	len=_tcslen(src)/4-1;

	for(i=0;i<len;i++)
	{
		v=(src[(i+1)*4]-'0')*1000+
			(src[(i+1)*4+1]-'0')*100+
			(src[(i+1)*4+2]-'0')*10+
			(src[(i+1)*4+3]-'0');
		dst[i]=(v-b)/a;
	}
	dst[i]='\0';
}

//读数据库配置文件
bool CYRSVMySQL::ReadFile(TCHAR* FileName)
{
	TCHAR temp[RCT_MAX_STR_SIZE]="";
	TCHAR tempchar[RCT_MAX_STR_SIZE]="";

	FILE *fp=_tfopen(FileName,_T("r"));
	if(fp)
	{
		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Ip:%s"),Ip);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Port:%s"),Port);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
#if 1
		_stscanf(temp,_T("User:%s"),tempchar);
		DeCode(tempchar,User);
#else
		_stscanf(temp,_T("User:%s"),User);
#endif

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);

#if 1
		//解密
		_stscanf(temp,_T("Psw:%s"),tempchar);
		DeCode(tempchar,Psw);
#else
		_stscanf(temp,_T("Psw:%s"),password);
#endif
		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("DataBaseName:%s"),DataBaseName);

		fclose(fp);

		return TRUE;
	}
	else
		return FALSE;
}

//去除插入数据库时出现单引号
void CYRSVMySQL::filterstring(char *str)
{
	while(*str)   
	{  
		if( (*str) == '\'' )
		{
			(*str) = ' ';
		}
		str++;  
	} 
}

//连接数据库
int CYRSVMySQL::ConnectionMySQL(char* FileName)
{
	//读取配置文件;
	if(!ReadFile(FileName))
	{
		return ReadFile_FAIL;
	}

	//初始化数据结构
	//连接数据库
	//mysql_library_init(NULL,0,0);
	if(mysql_init(&myConnection) == NULL )
	{
		return MySQLInit_FAIL;
	}

	char nvalue = 1;
	mysql_options(&myConnection,MYSQL_OPT_RECONNECT,(char *)&nvalue);// 断线自动重连

	mysql_handle = mysql_real_connect(&myConnection, Ip, User, Psw, DataBaseName, atoi(Port), NULL, 0);
	
	if(mysql_handle == NULL)
	{
		mysql_close(&myConnection) ;
		return ContOpen_FAIL;
	}

	//mysql_options(&myConnection,MYSQL_SET_CHARSET_NAME,"utf8");
	mysql_set_character_set(&myConnection, "GBK");
	//mysql_query(&myConnection, "SET NAMES GBK");

	m_bConnection = true;
	return Connectd_DONE;
}

//断开连接
void CYRSVMySQL::DisConnectionMySQL(void)
{
	mysql_close(&myConnection);
}

//保存字段为字符串
void CYRSVMySQL::VarSaveString(char *dst,char *src)
{
	if(src != NULL)
	{
		strcpy(dst,src);
	}
	else
	{
		dst[0] =0 ;
		dst[1] =0 ;
	}
}

//保存字段为数字
long CYRSVMySQL::VarSaveNumber(char *src)
{
	long temp = 0;
	if(src != NULL)
	{
		temp = atol(src);
	}
	return temp;
}

void CYRSVMySQL::StrSaveArray(char *src,long *dst)
{
	string::size_type pos1, pos2;
	string temp = src;
	pos2 = temp.find('_');
    pos1 = 0;

	while (string::npos != pos2)
    {
		*dst = atol(temp.substr(pos1, pos2 - pos1).c_str());

		pos1 = pos2 + 1;
		pos2 = temp.find('_', pos1);
		*dst++;
	}
	*dst = atol(temp.substr(pos1).c_str());
}

void CYRSVMySQL::ArraySaveStr(long *src,long count,char *dst)
{
	string str;
	int i = count;
	while(i)
	{
		stringstream ss;
		ss<<src[count-i];
		str +=  ss.str();
		i--;
		if(i!=0)
			str += '_';
	}

	strcpy(dst,str.c_str());
}

void CYRSVMySQL::UCHARSaveCHAR(unsigned char *src,long count,char *dst)
{
	for(int i=0;i<count;i++)
	{
		dst[i] = src[i] + '0';
	}
	dst[count] = 0;
}

void CYRSVMySQL::StrReplace(char*src, char*sub, char*dst)
{
	CString temp;
	temp = src;
	temp.Replace(sub,dst);
	strcpy(src,temp.GetBuffer());
}



/**************************区域表*******************************/
bool CYRSVMySQL::DEVICE_GetAreaID(long &nid,char *AreaName)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select nid from t_zd_org where SORGNAME = '%s'"),AreaName);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			nid = VarSaveNumber(row[0]);
		}

		mysql_free_result(res);
		IOwriteLock=false;
		return true;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::DEVICE_AddNewArea(char *AreaName)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(AreaName);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into t_zd_org values(NULL,'%s')"),AreaName);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}
//删除区域
bool CYRSVMySQL::DEVICE_DeleteArea(long areaID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from t_zd_org where nid = %d"),areaID);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}
//读取所有的区域
bool CYRSVMySQL::DEVICE_ReadAllOrgName(vector<struct DEVICE_AREA_ST>& DeviceAreaList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from t_zd_org"));
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;
		//MYSQL_FIELD *fd ;

		res = mysql_store_result(&myConnection);//取得查询结果
		//int count = (int) mysql_num_rows(res);//获取结果记录数
		//for(int i=0; fd=mysql_fetch_field(res); i++)
		//{
		//	cout << fd->name << endl;			//获取所有字段名
		//}

		struct DEVICE_AREA_ST DeviceArea = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&DeviceArea,0,sizeof(DEVICE_AREA_ST));
			//int num = mysql_num_fields(res);		//获取字段数
			DeviceArea.nid = VarSaveNumber(row[0]);
			VarSaveString(DeviceArea.name,row[1]);

			DeviceAreaList.push_back(DeviceArea);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(DeviceAreaList.size()>0)
			return true;
		else
			return false;
	}
	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::DEVICE_UpdateAreaInfo(long areaID,char *AreaName)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(AreaName);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update t_zd_org set SORGNAME='%s' where nid = %d"),AreaName,areaID);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

/**************************设备表*******************************/
bool CYRSVMySQL::DEVICE_GetCameraID(long &nid,long areaID,char *name)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select ncamera from tb_device where SCAMERANAME = '%s' and norgid=%d"),name,areaID);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			nid = VarSaveNumber(row[0]);
		}

		mysql_free_result(res);
		IOwriteLock=false;
		return true;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::DEVICE_AddNewCamera(long &ncamera,IPLIST CamInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(CamInfo.name);
	filterstring(CamInfo.ip);
	filterstring(CamInfo.user);
	filterstring(CamInfo.psw);
	//filterstring(CamInfo.Rtspurl);
	StrReplace(CamInfo.Rtspurl,"\\","\\\\");
	filterstring(CamInfo.longitude);
	filterstring(CamInfo.latitude);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_device values(NULL,'%s','%s',%d,'%s','%s',%d,%d,%d,'%s',%d,%d,%d,'%s','%s',%d,%d)"),\
						CamInfo.name,CamInfo.ip,CamInfo.port,CamInfo.user,CamInfo.psw,
						CamInfo.areaID,CamInfo.venderID,CamInfo.channel,CamInfo.Rtspurl,CamInfo.RTP,
						CamInfo.DecodeTag,CamInfo.userID,CamInfo.longitude,CamInfo.latitude,CamInfo.direction,CamInfo.svmode);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		_stprintf(strsql,_T("select last_insert_id() as aa"));
		if(mysql_query( &myConnection, strsql) == 0)
		{
			MYSQL_RES * res;
			MYSQL_ROW row ;

			res = mysql_store_result(&myConnection);//取得查询结果

			while(row = mysql_fetch_row(res))
			{
				ncamera = VarSaveNumber(row[0]);
			}
			mysql_free_result(res);
		}
	}

	IOwriteLock=false;
	return true;
}

//读某个区域所有的设备信息
bool CYRSVMySQL::DEVICE_ReadCameraInfo(long areaID,vector<IPLIST>& CameraList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_device where NORGID = %d"),areaID);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		IPLIST CamInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&CamInfo,0,sizeof(IPLIST));
			CamInfo.ncamera = VarSaveNumber(row[0]);
			VarSaveString(CamInfo.name,row[1]);
			VarSaveString(CamInfo.ip,row[2]);
			CamInfo.port = VarSaveNumber(row[3]);
			VarSaveString(CamInfo.user,row[4]);
			VarSaveString(CamInfo.psw,row[5]);
			CamInfo.areaID = VarSaveNumber(row[6]);
			CamInfo.venderID = VarSaveNumber(row[7]);
			CamInfo.channel = VarSaveNumber(row[8]);
			VarSaveString(CamInfo.Rtspurl,row[9]);
			CamInfo.RTP = VarSaveNumber(row[10]);
			CamInfo.DecodeTag = VarSaveNumber(row[11]);
			CamInfo.userID = VarSaveNumber(row[12]);
			VarSaveString(CamInfo.longitude,row[13]);
			VarSaveString(CamInfo.latitude,row[14]);
			CamInfo.direction = VarSaveNumber(row[15]);
			CamInfo.svmode = VarSaveNumber(row[16]);

			CameraList.push_back(CamInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(CameraList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

//删除摄像机
bool CYRSVMySQL::DEVICE_DeleteCamera(long cameraID)									
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_device where ncamera = %d"),cameraID);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

//删除某个区域的所有摄像机
bool CYRSVMySQL::DEVICE_DeleteCameraWithAreaID(long AreaID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_device where NORGID = %d"),AreaID);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::DEVICE_UpdateCameraInfo(IPLIST CamInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(CamInfo.name);
	filterstring(CamInfo.ip);
	filterstring(CamInfo.user);
	filterstring(CamInfo.psw);
	//filterstring(CamInfo.Rtspurl);
	StrReplace(CamInfo.Rtspurl,"\\","\\\\");
	filterstring(CamInfo.longitude);
	filterstring(CamInfo.latitude);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_device set SCAMERANAME='%s',SIPSERVER='%s',NPORT=%d,SUSER='%s',SPWD='%s',\
											NORGID=%d,NVENDERID=%d,CHANNEL=%d,RTSPURL='%s',RTPMODE=%d,\
											DECODETAG=%d,USERID=%d,LONGITUDE='%s',LATITUDE='%s',NDIRECTION=%d,NSVMODE=%d where NCAMERA=%d"),
											CamInfo.name,CamInfo.ip,CamInfo.port,CamInfo.user,CamInfo.psw,
											CamInfo.areaID,CamInfo.venderID,CamInfo.channel,CamInfo.Rtspurl,CamInfo.RTP,
											CamInfo.DecodeTag,CamInfo.userID,CamInfo.longitude,CamInfo.latitude,CamInfo.direction,CamInfo.svmode,CamInfo.ncamera);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::DEVICE_JudgeCamera(long ncamera)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select count(1) from tb_device where ncamera=%d"),ncamera);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(count>0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

//读某个区域所有的设备信息
bool CYRSVMySQL::DEVICE_GetCameraWithAreaID(long AreaID,list<long> &ncameraList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select ncamera from tb_device where NORGID = %d"),AreaID);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			long camid = VarSaveNumber(row[0]);
			ncameraList.push_back(camid);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(ncameraList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::DEVICE_ReadCameraInfoFromCamID(long ncamera,IPLIST &CamInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_device where ncamera = %d"),ncamera);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			CamInfo.ncamera = VarSaveNumber(row[0]);
			VarSaveString(CamInfo.name,row[1]);
			VarSaveString(CamInfo.ip,row[2]);
			CamInfo.port = VarSaveNumber(row[3]);
			VarSaveString(CamInfo.user,row[4]);
			VarSaveString(CamInfo.psw,row[5]);
			CamInfo.areaID = VarSaveNumber(row[6]);
			CamInfo.venderID = VarSaveNumber(row[7]);
			CamInfo.channel = VarSaveNumber(row[8]);
			VarSaveString(CamInfo.Rtspurl,row[9]);
			CamInfo.RTP = VarSaveNumber(row[10]);
			CamInfo.DecodeTag = VarSaveNumber(row[11]);
			CamInfo.userID = VarSaveNumber(row[12]);
			VarSaveString(CamInfo.longitude,row[13]);
			VarSaveString(CamInfo.latitude,row[14]);
			CamInfo.direction = VarSaveNumber(row[15]);
			CamInfo.svmode = VarSaveNumber(row[16]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

/**************************用户表*******************************/
bool CYRSVMySQL::USER_UserNameJudge(long &nid,char *userName)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(userName);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select nid from tb_user where SUSER = '%s'"),userName);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			nid = VarSaveNumber(row[0]);
		}

		mysql_free_result(res);
		IOwriteLock=false;
		return true;
	}

	IOwriteLock=false;
	return false;
}
//新增用户信息
bool CYRSVMySQL::USER_AddNewUser(struct SET_USER_DATA_ST UserInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(UserInfo.user);
	filterstring(UserInfo.psw);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_user values(NULL,'%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"),\
							UserInfo.user,UserInfo.psw,UserInfo.admin,UserInfo.systemset,UserInfo.recordset,
							UserInfo.record,UserInfo.ptz,UserInfo.preview,UserInfo.historyvideo,UserInfo.historyalarm,
							UserInfo.historydetect,UserInfo.historyreport,UserInfo.device,UserInfo.blackset,UserInfo.capbmp,
							UserInfo.Cardetect,UserInfo.CarDetectset,UserInfo.Facedetect,UserInfo.FaceDetectset);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

//删除用户信息
bool CYRSVMySQL::USER_DeletetUser(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_user where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

//编辑用户信息
bool CYRSVMySQL::USER_UpdateUserInfo(struct SET_USER_DATA_ST UserInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(UserInfo.user);
	filterstring(UserInfo.psw);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_user set SUSER='%s',SPWD='%s',NMANAGER=%d,NSET=%d,NVIDEOSET=%d,\
							NVIDEO=%d,NPTZ=%d,NVIEW=%d,NVIDEOQUERY=%d,NALARMQUERY=%d,\
							NDISTINGUISHQUERY=%d,NLOGQUERY=%d,NDEVICEALLOT=%d,NBLACKSET=%d,NPHOTO=%d,\
							NCARNUMBER=%d,NCARSET=%d,NFACERCG=%d,NFACESET=%d where nid=%d"),
							UserInfo.user,UserInfo.psw,UserInfo.admin,UserInfo.systemset,UserInfo.recordset,
							UserInfo.record,UserInfo.ptz,UserInfo.preview,UserInfo.historyvideo,UserInfo.historyalarm,
							UserInfo.historydetect,UserInfo.historyreport,UserInfo.device,UserInfo.blackset,UserInfo.capbmp,
							UserInfo.Cardetect,UserInfo.CarDetectset,UserInfo.Facedetect,UserInfo.FaceDetectset,UserInfo.nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::USER_ReadUserInfoWithName(char *user,struct SET_USER_DATA_ST *UserInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_user where SUSER = '%s'"),user);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			UserInfo->nid = VarSaveNumber(row[0]);
			VarSaveString(UserInfo->user,row[1]);
			VarSaveString(UserInfo->psw,row[2]);
			UserInfo->admin = VarSaveNumber(row[3]);
			UserInfo->systemset = VarSaveNumber(row[4]);
			UserInfo->recordset = VarSaveNumber(row[5]);
			UserInfo->record = VarSaveNumber(row[6]);
			UserInfo->ptz = VarSaveNumber(row[7]);
			UserInfo->preview = VarSaveNumber(row[8]);
			UserInfo->historyvideo = VarSaveNumber(row[9]);
			UserInfo->historyalarm = VarSaveNumber(row[10]);
			UserInfo->historydetect = VarSaveNumber(row[11]);
			UserInfo->historyreport = VarSaveNumber(row[12]);
			UserInfo->device = VarSaveNumber(row[13]);
			UserInfo->blackset = VarSaveNumber(row[14]);
			UserInfo->capbmp = VarSaveNumber(row[15]);
			UserInfo->Cardetect = VarSaveNumber(row[16]);
			UserInfo->CarDetectset = VarSaveNumber(row[17]);
			UserInfo->Facedetect = VarSaveNumber(row[18]);
			UserInfo->FaceDetectset = VarSaveNumber(row[19]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::USER_ReadUserInfoWithNid(long nid,struct SET_USER_DATA_ST *UserInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_user where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			UserInfo->nid = VarSaveNumber(row[0]);
			VarSaveString(UserInfo->user,row[1]);
			VarSaveString(UserInfo->psw,row[2]);
			UserInfo->admin = VarSaveNumber(row[3]);
			UserInfo->systemset = VarSaveNumber(row[4]);
			UserInfo->recordset = VarSaveNumber(row[5]);
			UserInfo->record = VarSaveNumber(row[6]);
			UserInfo->ptz = VarSaveNumber(row[7]);
			UserInfo->preview = VarSaveNumber(row[8]);
			UserInfo->historyvideo = VarSaveNumber(row[9]);
			UserInfo->historyalarm = VarSaveNumber(row[10]);
			UserInfo->historydetect = VarSaveNumber(row[11]);
			UserInfo->historyreport = VarSaveNumber(row[12]);
			UserInfo->device = VarSaveNumber(row[13]);
			UserInfo->blackset = VarSaveNumber(row[14]);
			UserInfo->capbmp = VarSaveNumber(row[15]);
			UserInfo->Cardetect = VarSaveNumber(row[16]);
			UserInfo->CarDetectset = VarSaveNumber(row[17]);
			UserInfo->Facedetect = VarSaveNumber(row[18]);
			UserInfo->FaceDetectset = VarSaveNumber(row[19]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::USER_ReadAllUserInfo(list<struct SET_USER_LIST_ST> &UserList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_user"));
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		struct SET_USER_LIST_ST UserInfo={0};
		while(row = mysql_fetch_row(res))
		{
			memset(&UserInfo,0,sizeof(SET_USER_LIST_ST));
			UserInfo.nid = VarSaveNumber(row[0]);
			VarSaveString(UserInfo.user,row[1]);

			UserList.push_back(UserInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(UserList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::LOG_AddNewSystemLog(char *userName,char *logStr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_log values(NULL,NULL,'%s','%s')"),userName,logStr);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

long CYRSVMySQL::LOG_GetSystemLogNum(char *userName,char *startTime,char *endTime,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(userName);

	char strsql[1024] = {0};
	char tempsql[1024] = {0};

	if((flag&0x01) != 0)
	{	
		filterstring(userName);
		if((flag&0x02) != 0)
		{
			_stprintf(tempsql,_T(" where susername like '%%%s%%' and dcreatetime between '%s' and '%s'"),
								userName,startTime,endTime);
		}
		else
		{
			_stprintf(tempsql,_T(" where susername like '%%%s%%'"),userName);
		}
	}
	else
	{
		if((flag&0x02) != 0)
		{
			_stprintf(tempsql,_T(" where dcreatetime between '%s' and '%s'"),
								startTime,endTime);
		}
	}

	_stprintf(strsql,_T("select count(1) from tb_log %s"),tempsql);
	strcpy(SQLstr,tempsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::LOG_ReadSystemLog(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_REPORT_ST> &HistoryList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};

	if(0 != flag)
	{
		_stprintf(strsql,_T("select * from tb_log %s order by dcreatetime limit %d,%d"),SQLstr,startNum-1,endNum-(startNum-1));
	}
	else
	{
		_stprintf(strsql,_T("select * from tb_log order by dcreatetime limit %d,%d"),startNum-1,endNum-(startNum-1));
	}
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct HISTORY_REPORT_ST HistoryInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&HistoryInfo,0,sizeof(HISTORY_REPORT_ST));
			char strdate[32] = {0};
			VarSaveString(strdate,row[1]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&HistoryInfo.year,&HistoryInfo.mon,&HistoryInfo.day,
											  &HistoryInfo.hour,&HistoryInfo.min,&HistoryInfo.sec);
			VarSaveString(HistoryInfo.user,row[2]);
			VarSaveString(HistoryInfo.str,row[3]);
			HistoryList.push_back(HistoryInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(HistoryList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

/**************************汽车结果表、报警表*******************************/
bool CYRSVMySQL::CAR_MatchResult_AddNewNoPicData(bool &isblack,long &nid,long &blackID,struct HISTORY_CarDetect_ST carInfo)
{
	IPLIST CamInfo = {0};
	DEVICE_ReadCameraInfoFromCamID(carInfo.ncamera,CamInfo);//获取摄像机信息
	strcpy(carInfo.name,CamInfo.name);
	strcpy(carInfo.ip,CamInfo.ip);

	char strsql[1024] = {0};
	CAR_Black_Compare(isblack,carInfo.blackid,carInfo.plate);//判断是否为布控车辆
	int flag;
	if(isblack)
	{
		CAR_AlarmResult_AddNewNoPicData(carInfo);
		flag = 1;
	}
	else
	{
		flag = 0;
	}

	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strdate[32]={0};
	_stprintf(strdate,"%04d-%02d-%02d %02d:%02d:%02d",carInfo.year,carInfo.mon,carInfo.day,
														carInfo.hour,carInfo.min,carInfo.sec);

	_stprintf(strsql,_T("insert into tb_match_result_car values(NULL,%d,'%s','%s','%s','%s','%s',%d,'%s',%d,NULL,'%s',%d,'%s',%d,'%s','%s')"),\
						carInfo.ncamera,carInfo.plate,carInfo.platecolor,carInfo.platetype,carInfo.direction,
						carInfo.carcolor,carInfo.reliability,strdate,flag,carInfo.path,carInfo.picsize,
						carInfo.smallpath,carInfo.smallsize,carInfo.name,carInfo.ip);

	StrReplace(strsql,"\\","\\\\");

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		_stprintf(strsql,_T("select last_insert_id() as aa"));
		if(mysql_query( &myConnection, strsql) == 0)
		{
			MYSQL_RES * res;
			MYSQL_ROW row ;

			res = mysql_store_result(&myConnection);//取得查询结果

			while(row = mysql_fetch_row(res))
			{
				nid = VarSaveNumber(row[0]);
			}
			mysql_free_result(res);
		}
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::CAR_Black_Compare(bool &isblack,long &blackID,char *carNum)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select count(1),nid from tb_car_black where scarnumber = '%s'"),carNum);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
			blackID = VarSaveNumber(row[1]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(count > 0)
			isblack = true;
		else
			isblack = false;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::CAR_AlarmResult_AddNewNoPicData(struct HISTORY_CarDetect_ST carInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strdate[32]={0};
	_stprintf(strdate,"%04d-%02d-%02d %02d:%02d:%02d",carInfo.year,carInfo.mon,carInfo.day,
														carInfo.hour,carInfo.min,carInfo.sec);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_alarm_car values(NULL,%d,'%s','%s','%s','%s','%s',%d,'%s',NULL,'%s',%d,'%s',%d,'%s','%s',%d)"),\
						carInfo.ncamera,carInfo.plate,carInfo.platecolor,carInfo.platetype,carInfo.direction,
						carInfo.carcolor,carInfo.reliability,strdate,carInfo.path,carInfo.picsize,
						carInfo.smallpath,carInfo.smallsize,carInfo.name,carInfo.ip,carInfo.blackid);

	StrReplace(strsql,"\\","\\\\");

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

long CYRSVMySQL::CAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
							char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	char tempsql[1024] = {0};
	char outsql[1024] = {0};
	bool hadwhere=false;		//检查语句中是否已经有where

	if((flag&0x01) != 0)
	{	
		filterstring(CamName);
		_stprintf(tempsql,_T(" where scameraname like '%%%s%%'"),CamName);
		strcpy(outsql,tempsql);
		hadwhere = true;
	}
	if((flag&0x02) != 0)
	{
		filterstring(sip);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and sip like '%%%s%%'"),sip);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where sip like '%%%s%%'"),sip);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x04) != 0)
	{
		filterstring(plate);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and scarnumber like '%%%s%%'"),plate);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where scarnumber like '%%%s%%'"),plate);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x10) != 0)
	{

		filterstring(direction);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and sdirection='%s'"),direction);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{

			_stprintf(tempsql,_T(" where sdirection='%s'"),direction);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x20) != 0)
	{
		filterstring(platecolor);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and scolour='%s'"),platecolor);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where scolour='%s'"),platecolor);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x40) != 0)
	{

		filterstring(platetype);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and stype='%s'"),platetype);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where stype='%s'"),platetype);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x80) != 0)
	{
		filterstring(carcolor);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and scolor='%s'"),carcolor);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where scolor='%s'"),carcolor);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}

	_stprintf(strsql,_T("select count(1) from tb_match_result_car %s"),outsql);
	strcpy(SQLstr,outsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

long CYRSVMySQL::CAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
							char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	char tempsql[1024] = {0};
	char outsql[1024] = {0};
	bool hadwhere=false;		//检查语句中是否已经有where

	if((flag&0x01) != 0)
	{	
		filterstring(CamName);
		_stprintf(tempsql,_T(" where scameraname like '%%%s%%'"),CamName);
		strcpy(outsql,tempsql);
		hadwhere = true;
	}
	if((flag&0x02) != 0)
	{
		filterstring(sip);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and sip like '%%%s%%'"),sip);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where sip like '%%%s%%'"),sip);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x04) != 0)
	{
		filterstring(plate);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and scarnumber like '%%%s%%'"),plate);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where scarnumber like '%%%s%%'"),plate);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x10) != 0)
	{

		filterstring(direction);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and sdirection='%s'"),direction);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{

			_stprintf(tempsql,_T(" where sdirection='%s'"),direction);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x20) != 0)
	{
		filterstring(platecolor);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and scolour='%s'"),platecolor);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where scolour='%s'"),platecolor);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x40) != 0)
	{

		filterstring(platetype);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and stype='%s'"),platetype);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where stype='%s'"),platetype);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x80) != 0)
	{
		filterstring(carcolor);
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and scolor='%s'"),carcolor);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where scolor='%s'"),carcolor);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}

	_stprintf(strsql,_T("select count(1) from tb_alarm_car %s"),outsql);
	strcpy(SQLstr,outsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::CAR_MatchResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_CarDetect_ST> &HistoryList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};

	if(0 != flag)
	{
		_stprintf(strsql,_T("select * from tb_match_result_car %s order by dcreate limit %d,%d"),SQLstr,startNum-1,endNum-(startNum-1));
	}
	else
	{
		_stprintf(strsql,_T("select * from tb_match_result_car order by dcreate limit %d,%d"),startNum-1,endNum-(startNum-1));
	}
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct HISTORY_CarDetect_ST HistoryInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&HistoryInfo,0,sizeof(HISTORY_CarDetect_ST));
			HistoryInfo.nid = VarSaveNumber(row[0]);
			HistoryInfo.ncamera = VarSaveNumber(row[1]);
			VarSaveString(HistoryInfo.plate,row[2]);
			VarSaveString(HistoryInfo.platecolor,row[3]);
			VarSaveString(HistoryInfo.platetype,row[4]);
			VarSaveString(HistoryInfo.direction,row[5]);
			VarSaveString(HistoryInfo.carcolor,row[6]);
			HistoryInfo.reliability = VarSaveNumber(row[7]);
			HistoryInfo.nflag = VarSaveNumber(row[9]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[10]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&HistoryInfo.year,&HistoryInfo.mon,&HistoryInfo.day,
											  &HistoryInfo.hour,&HistoryInfo.min,&HistoryInfo.sec);
			VarSaveString(HistoryInfo.path,row[11]);
			HistoryInfo.picsize = VarSaveNumber(row[12]);
			VarSaveString(HistoryInfo.smallpath,row[13]);
			HistoryInfo.smallsize = VarSaveNumber(row[14]);
			VarSaveString(HistoryInfo.name,row[15]);
			VarSaveString(HistoryInfo.ip,row[16]);

			HistoryList.push_back(HistoryInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(HistoryList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::CAR_AlarmResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_CarDetect_ST> &HistoryList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};

	if(0 != flag)
	{
		_stprintf(strsql,_T("select * from tb_alarm_car %s order by dcreate limit %d,%d"),SQLstr,startNum-1,endNum-(startNum-1));
	}
	else
	{
		_stprintf(strsql,_T("select * from tb_alarm_car order by dcreate limit %d,%d"),startNum-1,endNum-(startNum-1));
	}
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct HISTORY_CarDetect_ST HistoryInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&HistoryInfo,0,sizeof(HISTORY_CarDetect_ST));
			HistoryInfo.nid = VarSaveNumber(row[0]);
			HistoryInfo.ncamera = VarSaveNumber(row[1]);
			VarSaveString(HistoryInfo.plate,row[2]);
			VarSaveString(HistoryInfo.platecolor,row[3]);
			VarSaveString(HistoryInfo.platetype,row[4]);
			VarSaveString(HistoryInfo.direction,row[5]);
			VarSaveString(HistoryInfo.carcolor,row[6]);
			HistoryInfo.reliability = VarSaveNumber(row[7]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[9]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&HistoryInfo.year,&HistoryInfo.mon,&HistoryInfo.day,
											  &HistoryInfo.hour,&HistoryInfo.min,&HistoryInfo.sec);
			VarSaveString(HistoryInfo.path,row[10]);
			HistoryInfo.picsize = VarSaveNumber(row[11]);
			VarSaveString(HistoryInfo.smallpath,row[12]);
			HistoryInfo.smallsize = VarSaveNumber(row[13]);
			VarSaveString(HistoryInfo.name,row[14]);
			VarSaveString(HistoryInfo.ip,row[15]);
			HistoryInfo.blackid = VarSaveNumber(row[16]);

			HistoryList.push_back(HistoryInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(HistoryList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

long CYRSVMySQL::CAR_MatchResult_Count(long CAMID,char *startTime,char *endTime)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select count(1) from tb_match_result_car where ncamera = %d and dcreate between '%s' and '%s'"),CAMID,startTime,endTime);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::CAR_MatchResult_ReadforListOne(long nid,struct HISTORY_CarDetect_ST &CarData)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_match_result_car where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			CarData.nid = VarSaveNumber(row[0]);
			CarData.ncamera = VarSaveNumber(row[1]);
			VarSaveString(CarData.plate,row[2]);
			VarSaveString(CarData.platecolor,row[3]);
			VarSaveString(CarData.platetype,row[4]);
			VarSaveString(CarData.direction,row[5]);
			VarSaveString(CarData.carcolor,row[6]);
			CarData.reliability = VarSaveNumber(row[7]);
			CarData.nflag = VarSaveNumber(row[9]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[10]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&CarData.year,&CarData.mon,&CarData.day,
											  &CarData.hour,&CarData.min,&CarData.sec);
			VarSaveString(CarData.path,row[11]);
			CarData.picsize = VarSaveNumber(row[12]);
			VarSaveString(CarData.smallpath,row[13]);
			CarData.smallsize = VarSaveNumber(row[14]);
			VarSaveString(CarData.name,row[15]);
			VarSaveString(CarData.ip,row[16]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::CAR_AlarmResult_ReadforListOne(long nid,struct HISTORY_CarDetect_ST &CarData)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_alarm_car where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			CarData.nid = VarSaveNumber(row[0]);
			CarData.ncamera = VarSaveNumber(row[1]);
			VarSaveString(CarData.plate,row[2]);
			VarSaveString(CarData.platecolor,row[3]);
			VarSaveString(CarData.platetype,row[4]);
			VarSaveString(CarData.direction,row[5]);
			VarSaveString(CarData.carcolor,row[6]);
			CarData.reliability = VarSaveNumber(row[7]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[9]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&CarData.year,&CarData.mon,&CarData.day,
											  &CarData.hour,&CarData.min,&CarData.sec);
			VarSaveString(CarData.path,row[10]);
			CarData.picsize = VarSaveNumber(row[11]);
			VarSaveString(CarData.smallpath,row[12]);
			CarData.smallsize = VarSaveNumber(row[13]);
			VarSaveString(CarData.name,row[14]);
			VarSaveString(CarData.ip,row[15]);
			CarData.blackid = VarSaveNumber(row[16]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

/**************************汽车黑名单表*******************************/
bool CYRSVMySQL::CAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(BlackInfo.plate);
	filterstring(BlackInfo.brand);
	filterstring(BlackInfo.name);
	filterstring(BlackInfo.Phone);
	filterstring(BlackInfo.other);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_car_black values(NULL,'%s','%s','%s','%s','%s')"),\
						BlackInfo.plate,BlackInfo.brand,BlackInfo.name,BlackInfo.Phone,BlackInfo.other);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false; 
	return true;
}

bool CYRSVMySQL::CAR_BlackTable_DeleteWithNid(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_car_black where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::CAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(BlackInfo.plate);
	filterstring(BlackInfo.brand);
	filterstring(BlackInfo.name);
	filterstring(BlackInfo.Phone);
	filterstring(BlackInfo.other);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_car_black set SCARNUMBER='%s',SBRAND='%s',SLOSTNAME='%s',SPHONE='%s',SMARK='%s' where nid = %d"),
						BlackInfo.plate,BlackInfo.brand,BlackInfo.name,BlackInfo.Phone,BlackInfo.other,BlackInfo.nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

long CYRSVMySQL::CAR_BlackTable_GetNum(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select count(1) from tb_car_black"));

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::CAR_BlackTable_Read(long startNum,long endNum,list<struct BLACK_DATA_ST> &BlackList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_car_black order by nid limit %d,%d"),startNum-1,endNum-(startNum-1));
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct BLACK_DATA_ST BlackInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&BlackInfo,0,sizeof(BLACK_DATA_ST));
			BlackInfo.nid = VarSaveNumber(row[0]);
			VarSaveString(BlackInfo.plate,row[1]);
			VarSaveString(BlackInfo.brand,row[2]);
			VarSaveString(BlackInfo.name,row[3]);
			VarSaveString(BlackInfo.Phone,row[4]);
			VarSaveString(BlackInfo.other,row[5]);
			BlackList.push_back(BlackInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(BlackList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::CAR_BlackTable_ReadOne(long Num,struct BLACK_DATA_ST &black)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_car_black order by nid limit %d,%d"),Num-1,1);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			black.nid = VarSaveNumber(row[0]);
			VarSaveString(black.plate,row[1]);
			VarSaveString(black.brand,row[2]);
			VarSaveString(black.name,row[3]);
			VarSaveString(black.Phone,row[4]);
			VarSaveString(black.other,row[5]);
			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::CAR_BlackTable_DeleteAll(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_car_black"));
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::CAR_Alarm_DeleteAll(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_alarm_car"));
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::CAR_Alarm_DeleteWithBlackNid(long blackid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_alarm_car where nid = %d"),blackid);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::CAR_BlackTable_ReadOneWithNid(long Nid,struct BLACK_DATA_ST &black)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_car_black where nid = %d"),Nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			black.nid = VarSaveNumber(row[0]);
			VarSaveString(black.plate,row[1]);
			VarSaveString(black.brand,row[2]);
			VarSaveString(black.name,row[3]);
			VarSaveString(black.Phone,row[4]);
			VarSaveString(black.other,row[5]);
			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

/**************************人脸布控库信息表*******************************/
bool CYRSVMySQL::FACE_BlackDB_AddNew(long &nid,struct FACE_DB_ST faceDB)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(faceDB.dbname);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_face_db values(NULL,'%s',%d)"),\
										faceDB.dbname,faceDB.RW_dbID);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		_stprintf(strsql,_T("select last_insert_id() as aa"));
		if(mysql_query( &myConnection, strsql) == 0)
		{
			MYSQL_RES * res;
			MYSQL_ROW row ;

			res = mysql_store_result(&myConnection);//取得查询结果

			while(row = mysql_fetch_row(res))
			{
				nid = VarSaveNumber(row[0]);
			}
			mysql_free_result(res);
		}
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackDB_ReadAll(list<struct FACE_DB_ST> &FaceDBList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_face_db order by nid"));
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		struct FACE_DB_ST DBInfo={0};
		while(row = mysql_fetch_row(res))
		{
			DBInfo.nid = VarSaveNumber(row[0]);
			VarSaveString(DBInfo.dbname,row[1]);
			DBInfo.RW_dbID = VarSaveNumber(row[2]);

			FaceDBList.push_back(DBInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(FaceDBList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_BlackDB_Edit(struct FACE_DB_ST faceDB)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(faceDB.dbname);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_face_db set SDBNAME='%s',NRWDBID=%d where nid = %d"),
										faceDB.dbname,faceDB.RW_dbID,faceDB.nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackDB_DeleteWithNid(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_face_db where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackDB_GetRWID(long nid,long &RW_dbID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select NRWDBID from tb_face_db where NID = %d"),nid);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			RW_dbID = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return true;
	}

	IOwriteLock=false;
	return false;
}

/**************************人员布控信息表*******************************/
long CYRSVMySQL::FACE_BlackPerson_GetNum(long DBID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select count(1) from tb_person_black where NDBID = %d"),DBID);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::FACE_BlackPerson_Read(long DBID,long startNum,long endNum,list<struct PERSON_BLACK_ST> &BlackList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_person_black where NDBID = %d order by nid limit %d,%d"),DBID,startNum-1,endNum-(startNum-1));
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct PERSON_BLACK_ST BlackInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&BlackInfo,0,sizeof(PERSON_BLACK_ST));
			BlackInfo.nid = VarSaveNumber(row[0]);
			BlackInfo.db_nid = VarSaveNumber(row[1]);
			BlackInfo.RW_dbID = VarSaveNumber(row[2]);
			BlackInfo.RW_personID = VarSaveNumber(row[3]);
			VarSaveString(BlackInfo.name,row[4]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[5]);
			sscanf(strdate,"%d-%d-%d",&BlackInfo.year,&BlackInfo.mon,&BlackInfo.day);
			BlackInfo.sex = VarSaveNumber(row[6]);
			VarSaveString(BlackInfo.phone,row[7]);
			BlackInfo.cardtype = VarSaveNumber(row[8]);
			VarSaveString(BlackInfo.cardNO,row[9]);
			BlackInfo.native = VarSaveNumber(row[10]);
			VarSaveString(BlackInfo.address,row[11]);

			BlackList.push_back(BlackInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(BlackList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_BlackPerson_ReadOne(long nid,struct PERSON_BLACK_ST &black)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_person_black where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			black.nid = VarSaveNumber(row[0]);
			black.db_nid = VarSaveNumber(row[1]);
			black.RW_dbID = VarSaveNumber(row[2]);
			black.RW_personID = VarSaveNumber(row[3]);
			VarSaveString(black.name,row[4]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[5]);
			sscanf(strdate,"%d-%d-%d",&black.year,&black.mon,&black.day);
			black.sex = VarSaveNumber(row[6]);
			VarSaveString(black.phone,row[7]);
			black.cardtype = VarSaveNumber(row[8]);
			VarSaveString(black.cardNO,row[9]);
			black.native = VarSaveNumber(row[10]);
			VarSaveString(black.address,row[11]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_BlackPerson_AddNew(long &personID,struct PERSON_BLACK_ST black)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(black.name);
	filterstring(black.phone);
	filterstring(black.cardNO);
	filterstring(black.address);

	char strsql[1024] = {0};
	char strdate[32] = {0};
	_stprintf(strdate,_T("%04d-%02d-%02d"),black.year,black.mon,black.day);
	_stprintf(strsql,_T("insert into tb_person_black values(NULL,%d,%d,%d,'%s','%s',%d,'%s',%d,'%s',%d,'%s')"),
						black.db_nid,black.RW_dbID,black.RW_personID,black.name,strdate,
						black.sex,black.phone,black.cardtype,black.cardNO,black.native,black.address);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		_stprintf(strsql,_T("select last_insert_id() as aa"));
		if(mysql_query( &myConnection, strsql) == 0)
		{
			MYSQL_RES * res;
			MYSQL_ROW row ;

			res = mysql_store_result(&myConnection);//取得查询结果

			while(row = mysql_fetch_row(res))
			{
				personID = VarSaveNumber(row[0]);
			}
			mysql_free_result(res);
		}
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackPerson_Edit(struct PERSON_BLACK_ST black)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(black.name);
	filterstring(black.phone);
	filterstring(black.cardNO);
	filterstring(black.address);

	char strsql[1024] = {0};
	char strdate[32] = {0};
	_stprintf(strdate,_T("%04d-%02d-%02d"),black.year,black.mon,black.day);
	_stprintf(strsql,_T("update tb_person_black set NDBID=%d,NRWDBID=%d,NRWPERSONID=%d,SNAME='%s',DAGE='%s',\
						NSEX=%d,SPHONE='%s',NCARDTYPE=%d,SCARDNO='%s',NNATIVE=%d,SADDRESS='%s' where nid = %d"),
						black.db_nid,black.RW_dbID,black.RW_personID,black.name,strdate,
						black.sex,black.phone,black.cardtype,black.cardNO,black.native,black.address,black.nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackPerson_DeleteWithNid(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_person_black where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackPerson_DeleteWithDBID(long dbID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_person_black where NDBID = %d"),dbID);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackPerson_CmpRead(struct PERSON_BLACK_ST &black,char *name,int sex,char *sage,char *eage,int cardtype,char *cardNO,
										  char *phone,int native,char *address,int flag,long db_id,long RW_DBID,long RW_PersonID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	char tempsql[1024] = {0};
	char outsql[1024] = {0};

	if((flag&0x02) != 0)
	{
		_stprintf(tempsql,_T(" and NSEX = %d"),sex);
		strcpy(outsql,tempsql);
	}
	if((flag&0x04) != 0)
	{
		_stprintf(tempsql,_T(" and DAGE between '%s' and '%s'"),sage,eage);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x20) != 0)
	{
		filterstring(name);
		_stprintf(tempsql,_T(" and SNAME = '%s'"),name);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x40) != 0)
	{
		_stprintf(tempsql,_T(" and NCARDTYPE = %d"),cardtype);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x80) != 0)
	{
		filterstring(cardNO);
		_stprintf(tempsql,_T(" and SCARDNO = '%s'"),cardNO);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x100) != 0)
	{
		filterstring(phone);
		_stprintf(tempsql,_T(" and SPHONE = '%s'"),phone);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x200) != 0)
	{
		_stprintf(tempsql,_T(" and NNATIVE = %d"),native);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x400) != 0)
	{
		filterstring(address);
		_stprintf(tempsql,_T(" and SADDRESS = '%s'"),address);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}

	_stprintf(strsql,_T("select * from tb_person_black where NDBID = %d and NRWDBID = %d and NRWPERSONID = %d %s"),
						db_id,RW_DBID,RW_PersonID,outsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			black.nid = VarSaveNumber(row[0]);
			black.db_nid = VarSaveNumber(row[1]);
			black.RW_dbID = VarSaveNumber(row[2]);
			black.RW_personID = VarSaveNumber(row[3]);
			VarSaveString(black.name,row[4]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[5]);
			sscanf(strdate,"%d-%d-%d",&black.year,&black.mon,&black.day);
			black.sex = VarSaveNumber(row[6]);
			VarSaveString(black.phone,row[7]);
			black.cardtype = VarSaveNumber(row[8]);
			VarSaveString(black.cardNO,row[9]);
			black.native = VarSaveNumber(row[10]);
			VarSaveString(black.address,row[11]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

/**************************布控人脸信息表*******************************/
bool CYRSVMySQL::FACE_BlackFace_Read(long PersonID,list<struct FACE_BLACK_ST> &FaceList )
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_face_black where NBLACKID = %d order by nid"),PersonID);
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct FACE_BLACK_ST BlackInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&BlackInfo,0,sizeof(FACE_BLACK_ST));
			BlackInfo.nid = VarSaveNumber(row[0]);
			BlackInfo.person_black_nid = VarSaveNumber(row[1]);
			BlackInfo.RW_faceID = VarSaveNumber(row[2]);
			BlackInfo.RW_personID = VarSaveNumber(row[3]);
			VarSaveString(BlackInfo.file,row[4]);
			BlackInfo.size = VarSaveNumber(row[5]);
			VarSaveString(BlackInfo.face,row[6]);
			BlackInfo.facesize = VarSaveNumber(row[7]);

			FaceList.push_back(BlackInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(FaceList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_BlackFace_AddNew(struct FACE_BLACK_ST face)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_face_black values(NULL,%d,%d,%d,'%s',%d,'%s',%d)"),
		face.person_black_nid,face.RW_faceID,face.RW_personID,face.file,face.size,face.face,face.facesize);

	StrReplace(strsql,"\\","\\\\");

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackFace_Update(struct FACE_BLACK_ST face)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_face_black set NBLACKID=%d,NRWFACEID=%d,NRWPERSONID=%d,SFILE='%s',NSIZE=%d,SFACE='%s',NFACESIZE=%d where nid = %d"),
						face.person_black_nid,face.RW_faceID,face.RW_personID,face.file,face.size,face.face,face.facesize,face.nid);

	StrReplace(strsql,"\\","\\\\");

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackFace_DeleteWithNid(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_face_black where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackFace_DeleteWithPersonID(long personID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_face_black where NBLACKID = %d"),personID);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_BlackFace_ReadWithRWfaceID(long RW_faceID,long RW_personID,struct FACE_BLACK_ST &face)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_face_black where NRWFACEID = %d and NRWPERSONID = %d"),RW_faceID,RW_personID);
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		while(row = mysql_fetch_row(res))
		{
			face.nid = VarSaveNumber(row[0]);
			face.person_black_nid = VarSaveNumber(row[1]);
			face.RW_faceID = VarSaveNumber(row[2]);
			face.RW_personID = VarSaveNumber(row[3]);
			VarSaveString(face.file,row[4]);
			face.size = VarSaveNumber(row[5]);
			VarSaveString(face.face,row[6]);
			face.facesize = VarSaveNumber(row[7]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_Black_Read_PersonAndFace(long faceID,long personID,struct PERSON_BLACK_ST &person,struct FACE_BLACK_ST &face)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("SELECT * FROM tb_person_black t,tb_face_black d where d.NBLACKID=t.NID and t.nid=%d and d.NID=%d"),personID,faceID);
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		while(row = mysql_fetch_row(res))
		{
			person.nid = VarSaveNumber(row[0]);
			person.db_nid = VarSaveNumber(row[1]);
			person.RW_dbID = VarSaveNumber(row[2]);
			person.RW_personID = VarSaveNumber(row[3]);
			VarSaveString(person.name,row[4]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[5]);
			sscanf(strdate,"%d-%d-%d",&person.year,&person.mon,&person.day);
			person.sex = VarSaveNumber(row[6]);
			VarSaveString(person.phone,row[7]);
			person.cardtype = VarSaveNumber(row[8]);
			VarSaveString(person.cardNO,row[9]);
			person.native = VarSaveNumber(row[10]);
			VarSaveString(person.address,row[11]);

			face.nid = VarSaveNumber(row[12]);
			face.person_black_nid = VarSaveNumber(row[13]);
			face.RW_faceID = VarSaveNumber(row[14]);
			face.RW_personID = VarSaveNumber(row[15]);
			VarSaveString(face.file,row[16]);
			face.size = VarSaveNumber(row[17]);
			VarSaveString(face.face,row[18]);
			face.facesize = VarSaveNumber(row[19]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_Black_Read_DetailBlack(long faceID,long personID,struct FACE_ALARM_VIEW_ST &alarmInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("SELECT * FROM tb_person_black t,tb_face_black f,tb_face_db d where f.NBLACKID=t.NID and d.NID=t.NDBID and t.nid=%d and f.NID=%d"),personID,faceID);
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		while(row = mysql_fetch_row(res))
		{
			alarmInfo.person_black_nid = VarSaveNumber(row[0]);
			alarmInfo.db_nid =  VarSaveNumber(row[1]);
			alarmInfo.RW_dbID= VarSaveNumber(row[2]);
			alarmInfo.RW_personID= VarSaveNumber(row[3]);
			VarSaveString(alarmInfo.name,row[4]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[5]);
			sscanf(strdate,"%d-%d-%d",&alarmInfo.year,&alarmInfo.mon,&alarmInfo.day);
			alarmInfo.sex = VarSaveNumber(row[6]);
			VarSaveString(alarmInfo.phone,row[7]);
			alarmInfo.cardtype = VarSaveNumber(row[8]);
			VarSaveString(alarmInfo.cardNO,row[9]);
			alarmInfo.native = VarSaveNumber(row[10]);
			VarSaveString(alarmInfo.address,row[11]);
			alarmInfo.RW_faceID= VarSaveNumber(row[14]);
			VarSaveString(alarmInfo.file,row[16]);
			alarmInfo.size = VarSaveNumber(row[17]);
			VarSaveString(alarmInfo.face,row[18]);
			alarmInfo.facesize = VarSaveNumber(row[19]);
			VarSaveString(alarmInfo.dbname,row[21]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}


/**************************摄像头分析设置表*******************************/
bool CYRSVMySQL::DEVICE_SetTable_AddNew(long ncamera)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_device_set (NCAMERA) values(%d)"),ncamera);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::DEVICE_SetTable_Read(long ncamera,struct DEVICE_SET_ST &CamSet)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_device_set where NCAMERA = %d"),ncamera);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		char tempstr[1024]={0};
		while(row = mysql_fetch_row(res))
		{
			CamSet.nid = VarSaveNumber(row[0]);
			CamSet.ncamera = VarSaveNumber(row[1]);
			CamSet.svmode = VarSaveNumber(row[2]);
			CamSet.face.minface = VarSaveNumber(row[3]);
			CamSet.face.maxface = VarSaveNumber(row[4]);

			memset(&tempstr,0,1024);
			VarSaveString(tempstr,row[5]);
			CamSet.face.frontface_ts = atof(tempstr);
			memset(&tempstr,0,1024);
			VarSaveString(tempstr,row[6]);
			CamSet.face.sideface_ts = atof(tempstr);

			CamSet.face.time_out = VarSaveNumber(row[7]);
			memset(&tempstr,0,1024);
			VarSaveString(tempstr,row[8]);
			CamSet.face.scale_ratio = atof(tempstr);

			CamSet.face.video_ts = VarSaveNumber(row[9]);
			CamSet.face.threshold = VarSaveNumber(row[10]);
			CamSet.face.maxcount = VarSaveNumber(row[11]);
			CamSet.face.dbTotal = VarSaveNumber(row[12]);

			memset(&tempstr,0,1024);
			VarSaveString(tempstr,row[13]);
			StrSaveArray(tempstr,CamSet.face.db_nid);
			CamSet.width = VarSaveNumber(row[14]);
			CamSet.hight = VarSaveNumber(row[15]);
			CamSet.car.redrect = VarSaveNumber(row[16]);
			CamSet.car.deskew = VarSaveNumber(row[17]);
			CamSet.car.jpgquailty = VarSaveNumber(row[18]);
			CamSet.car.minwidth = VarSaveNumber(row[19]);
			CamSet.car.maxwidth = VarSaveNumber(row[20]);
			CamSet.car.left = VarSaveNumber(row[21]);
			CamSet.car.right = VarSaveNumber(row[22]);
			CamSet.car.top = VarSaveNumber(row[23]);
			CamSet.car.bottom = VarSaveNumber(row[24]);
			VarSaveString(CamSet.car.defaultchar,row[25]);
			CamSet.car.reliability = VarSaveNumber(row[26]);
			CamSet.car.carlor = VarSaveNumber(row[27]);
			VarSaveString(tempstr,row[28]);
			for(int i=0;i<CAR_MASK_MAX;i++)
			{
				CamSet.car.mask[i] = tempstr[i] - '0';
			}
			CamSet.car.night = VarSaveNumber(row[29]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::DEVICE_SetTable_Update(struct DEVICE_SET_ST CamSet)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	char tempstr[1024]={0};
	ArraySaveStr(CamSet.face.db_nid,CamSet.face.dbTotal,tempstr);
	char tempMask[CAR_MASK_MAX+1]={0};
	UCHARSaveCHAR(CamSet.car.mask,CAR_MASK_MAX,tempMask);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_device_set set NCAMERA=%d,NSVMODE=%d,NMINFACE=%d,NMAXFACE=%d,NFRONTFACE=%f,NSIDEFACE=%f,\
												NTIMEOUT=%d,NSCALE=%f,NVIDEOTS=%d,NTHRESHOLD=%d,NMAXCOUNT=%d,\
												NRWDBTOTAL=%d,SRWDBSTR='%s',NWIDTH=%d,NHIGHT=%d,NREDRECT=%d,\
												NDESKEW=%d,NJPGQUALITY=%d,NMINWIDTH=%d,NMAXWIDTH=%d,NLEFT=%d,NRIGHT=%d,\
												NTOP=%d,NBOTTOM=%d,SDEFAULTCHAR='%s',NRELIABILITY=%d,NCARCOLOR=%d,NMASK='%s',NNIGHT=%d\
												 where nid = %d"),
						CamSet.ncamera,CamSet.svmode,CamSet.face.minface,CamSet.face.maxface,CamSet.face.frontface_ts,CamSet.face.sideface_ts,
						CamSet.face.time_out,CamSet.face.scale_ratio,CamSet.face.video_ts,CamSet.face.threshold,CamSet.face.maxcount,
						CamSet.face.dbTotal,tempstr,CamSet.width,CamSet.hight,CamSet.car.redrect,
						CamSet.car.deskew,CamSet.car.jpgquailty,CamSet.car.minwidth,CamSet.car.maxwidth,CamSet.car.left,CamSet.car.right,
						CamSet.car.top,CamSet.car.bottom,CamSet.car.defaultchar,CamSet.car.reliability,CamSet.car.carlor,
						tempMask,CamSet.car.night,CamSet.nid);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::DEVICE_SetTable_DeleteWithCamID(long ncamera)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_device_set where NCAMERA = %d"),ncamera);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

/**************************临时布控库信息表*******************************/
bool CYRSVMySQL::TEMP_DB_AddNew(struct FACE_TEMP_DB_ST tempDB)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(tempDB.name);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_temp_db values(NULL,%d,%d,'%s')"),tempDB.RW_tempID,tempDB.ncamera,tempDB.name);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::TEMP_DB_ReadAll(list<struct FACE_TEMP_DB_ST> &TempDBList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_temp_db order by nid"));
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		struct FACE_TEMP_DB_ST DBInfo={0};
		while(row = mysql_fetch_row(res))
		{
			DBInfo.nid = VarSaveNumber(row[0]);
			DBInfo.RW_tempID = VarSaveNumber(row[1]);
			DBInfo.ncamera = VarSaveNumber(row[2]);
			VarSaveString(DBInfo.name,row[3]);
			TempDBList.push_back(DBInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(TempDBList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::TEMP_DB_Read(long ncamera,struct FACE_TEMP_DB_ST &tempDB)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_temp_db where NCAMERA = %d"),ncamera);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			tempDB.nid = VarSaveNumber(row[0]);
			tempDB.RW_tempID = VarSaveNumber(row[1]);
			tempDB.ncamera = VarSaveNumber(row[2]);
			VarSaveString(tempDB.name,row[3]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::TEMP_DB_UpdateWithNcamera(struct FACE_TEMP_DB_ST tempDB)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(tempDB.name);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_temp_db set SNAME='%s' where NCAMERA = %d"),
												tempDB.name,tempDB.ncamera);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::TEMP_DB_DeleteWithNcamera(long ncamera)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_temp_db where NCAMERA = %d"),ncamera);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::TEMP_DB_Update(struct FACE_TEMP_DB_ST tempDB)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	filterstring(tempDB.name);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_temp_db set NRWTEMPDB=%d,NCAMERA=%d,SNAME='%s' where nid = %d"),
												tempDB.RW_tempID,tempDB.ncamera,tempDB.name,tempDB.nid);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::TEMP_DB_DeleteWithNid(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("delete from tb_temp_db where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql))
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::TEMP_DB_Get_RWtempID(long ncamera,long &RWtempID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select NRWTEMPDB from tb_temp_db where NCAMERA = %d"),ncamera);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			RWtempID = VarSaveNumber(row[0]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::TEMP_DB_Get_RWtempID_List(list<long>&IDList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select NRWTEMPDB from tb_temp_db"));
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;
		long tempID;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			tempID = VarSaveNumber(row[0]);
			IDList.push_back(tempID);
		}

		mysql_free_result(res);
		IOwriteLock=false;
		if(IDList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

/**************************人脸抓拍表*******************************/
bool CYRSVMySQL::FACE_Capture_AddNew(long &nid,struct FACE_CAPTURE_ST tempFace)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strdate[32]={0};
	_stprintf(strdate,"%04d-%02d-%02d %02d:%02d:%02d",tempFace.year,tempFace.mon,tempFace.day,
														tempFace.hour,tempFace.min,tempFace.sec);

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_face_capture values(NULL,'%s',%d,%d,%d,%d,'%s',%d,'%s',%d,0,NULL,%d,'%s','%s',0,%d,%d,%d,%d)"),
						strdate,tempFace.sex,tempFace.age,tempFace.facetype,tempFace.quality,
						tempFace.file,tempFace.size,tempFace.face,tempFace.facesize,
						tempFace.ncamera,tempFace.cameraName,tempFace.sip,
						tempFace.temp_db_nid,tempFace.RW_tempID,tempFace.RW_personID,tempFace.RW_faceID);

	StrReplace(strsql,"\\","\\\\");
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		_stprintf(strsql,_T("select last_insert_id() as aa"));
		if(mysql_query( &myConnection, strsql) == 0)
		{
			MYSQL_RES * res;
			MYSQL_ROW row ;

			res = mysql_store_result(&myConnection);//取得查询结果

			while(row = mysql_fetch_row(res))
			{
				nid = VarSaveNumber(row[0]);
			}
			mysql_free_result(res);
		}
	}

	IOwriteLock=false;
	return true;
}

long CYRSVMySQL::FACE_Capture_GetNum(long ncamera,int sex,int sage,int eage,char *startTime,char *endTime,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	char tempsql[1024] = {0};
	char outsql[1024] = {0};
	bool hadwhere=false;		//检查语句中是否已经有where

	if((flag&0x01) != 0)
	{
		_stprintf(tempsql,_T(" where NCAMERA = %d"),ncamera);
		strcpy(outsql,tempsql);
		hadwhere = true;
	}
	if((flag&0x02) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and NSEX =%d"),sex);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where NSEX =%d"),sex);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x04) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and NAGE between %d and %d"),sage,eage);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where NAGE between %d and %d"),sage,eage);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	_stprintf(strsql,_T("select count(1) from tb_face_capture %s"),outsql);
	strcpy(SQLstr,outsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::FACE_Capture_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct FACE_CAPTURE_ST> &faceList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};

	if(0 != flag)
	{
		_stprintf(strsql,_T("select * from tb_face_capture %s order by dcreate limit %d,%d"),SQLstr,startNum-1,endNum-(startNum-1));
	}
	else
	{
		_stprintf(strsql,_T("select * from tb_face_capture order by dcreate limit %d,%d"),startNum-1,endNum-(startNum-1));
	}
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct FACE_CAPTURE_ST tempFace = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&tempFace,0,sizeof(FACE_CAPTURE_ST));
			tempFace.nid = VarSaveNumber(row[0]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[1]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&tempFace.year,&tempFace.mon,&tempFace.day,
											  &tempFace.hour,&tempFace.min,&tempFace.sec);

			tempFace.sex = VarSaveNumber(row[2]);
			tempFace.age = VarSaveNumber(row[3]);
			tempFace.facetype = VarSaveNumber(row[4]);
			tempFace.quality = VarSaveNumber(row[5]);
			VarSaveString(tempFace.file,row[6]);
			tempFace.size = VarSaveNumber(row[7]);
			VarSaveString(tempFace.face,row[8]);
			tempFace.facesize = VarSaveNumber(row[9]);
			tempFace.isblack = VarSaveNumber(row[10]);
			tempFace.ncamera = VarSaveNumber(row[12]);
			VarSaveString(tempFace.cameraName,row[13]);
			VarSaveString(tempFace.sip,row[14]);
			tempFace.deal = VarSaveNumber(row[15]);
			tempFace.temp_db_nid = VarSaveNumber(row[16]);
			tempFace.RW_tempID = VarSaveNumber(row[17]);
			tempFace.RW_personID = VarSaveNumber(row[18]);
			tempFace.RW_faceID = VarSaveNumber(row[19]);

			faceList.push_back(tempFace);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(faceList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_Capture_ReadforListOne(long nid,struct FACE_CAPTURE_ST &tempFace)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_face_capture where nid = %d"),nid);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			tempFace.nid = VarSaveNumber(row[0]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[1]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&tempFace.year,&tempFace.mon,&tempFace.day,
											  &tempFace.hour,&tempFace.min,&tempFace.sec);

			tempFace.sex = VarSaveNumber(row[2]);
			tempFace.age = VarSaveNumber(row[3]);
			tempFace.facetype = VarSaveNumber(row[4]);
			tempFace.quality = VarSaveNumber(row[5]);
			VarSaveString(tempFace.file,row[6]);
			tempFace.size = VarSaveNumber(row[7]);
			VarSaveString(tempFace.face,row[8]);
			tempFace.facesize = VarSaveNumber(row[9]);
			tempFace.isblack = VarSaveNumber(row[10]);
			tempFace.ncamera = VarSaveNumber(row[12]);
			VarSaveString(tempFace.cameraName,row[13]);
			VarSaveString(tempFace.sip,row[14]);
			tempFace.deal = VarSaveNumber(row[15]);
			tempFace.temp_db_nid = VarSaveNumber(row[16]);
			tempFace.RW_tempID = VarSaveNumber(row[17]);
			tempFace.RW_personID = VarSaveNumber(row[18]);
			tempFace.RW_faceID = VarSaveNumber(row[19]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_Capture_GetUndealFace(struct FACE_CAPTURE_ST &tempFace)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from tb_face_capture where NDEAL=0 order by DCREATE limit 0,1"));
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			tempFace.nid = VarSaveNumber(row[0]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[1]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&tempFace.year,&tempFace.mon,&tempFace.day,
											  &tempFace.hour,&tempFace.min,&tempFace.sec);

			tempFace.sex = VarSaveNumber(row[2]);
			tempFace.age = VarSaveNumber(row[3]);
			tempFace.facetype = VarSaveNumber(row[4]);
			tempFace.quality = VarSaveNumber(row[5]);
			VarSaveString(tempFace.file,row[6]);
			tempFace.size = VarSaveNumber(row[7]);
			VarSaveString(tempFace.face,row[8]);
			tempFace.facesize = VarSaveNumber(row[9]);
			tempFace.isblack = VarSaveNumber(row[10]);
			tempFace.ncamera = VarSaveNumber(row[12]);
			VarSaveString(tempFace.cameraName,row[13]);
			VarSaveString(tempFace.sip,row[14]);
			tempFace.deal = VarSaveNumber(row[15]);
			tempFace.temp_db_nid = VarSaveNumber(row[16]);
			tempFace.RW_tempID = VarSaveNumber(row[17]);
			tempFace.RW_personID = VarSaveNumber(row[18]);
			tempFace.RW_faceID = VarSaveNumber(row[19]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_Capture_UpdateDealFlag(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_face_capture set NDEAL=1 where NID = %d"),nid);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

bool CYRSVMySQL::FACE_Capture_UpdateBlackFlag(long nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("update tb_face_capture set NISBLACK=1 where NID = %d"),nid);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

long CYRSVMySQL::FACE_Capture_GetAlarmNum(long ncamera,int sex,int sage,int eage,char *startTime,char *endTime,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	char tempsql[1024] = {0};
	char outsql[1024] = {0};
	//bool hadwhere=false;		//检查语句中是否已经有where

	if((flag&0x01) != 0)
	{	
		_stprintf(tempsql,_T(" and NCAMERA = %d"),ncamera);
		strcpy(outsql,tempsql);
	}
	if((flag&0x02) != 0)
	{
		_stprintf(tempsql,_T(" and NSEX =%d"),sex);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x04) != 0)
	{
		_stprintf(tempsql,_T(" and NAGE between %d and %d"),sage,eage);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x08) != 0)
	{
		_stprintf(tempsql,_T(" and dcreate between '%s' and '%s'"),startTime,endTime);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	_stprintf(strsql,_T("select count(1) from tb_face_capture where NISBLACK=1 %s"),outsql);
	strcpy(SQLstr,outsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::FACE_Capture_ReadAlarmList(char *SQLstr,int flag,long startNum,long endNum,list<struct FACE_CAPTURE_ST> &faceList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};

	if(0 != flag)
	{
		_stprintf(strsql,_T("select * from tb_face_capture where NISBLACK=1 %s order by dcreate limit %d,%d"),SQLstr,startNum-1,endNum-(startNum-1));
	}
	else
	{
		_stprintf(strsql,_T("select * from tb_face_capture where NISBLACK=1 order by dcreate limit %d,%d"),startNum-1,endNum-(startNum-1));
	}
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct FACE_CAPTURE_ST tempFace = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&tempFace,0,sizeof(FACE_CAPTURE_ST));
			tempFace.nid = VarSaveNumber(row[0]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[1]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&tempFace.year,&tempFace.mon,&tempFace.day,
											  &tempFace.hour,&tempFace.min,&tempFace.sec);

			tempFace.sex = VarSaveNumber(row[2]);
			tempFace.age = VarSaveNumber(row[3]);
			tempFace.facetype = VarSaveNumber(row[4]);
			tempFace.quality = VarSaveNumber(row[5]);
			VarSaveString(tempFace.file,row[6]);
			tempFace.size = VarSaveNumber(row[7]);
			VarSaveString(tempFace.face,row[8]);
			tempFace.facesize = VarSaveNumber(row[9]);
			tempFace.isblack = VarSaveNumber(row[10]);
			tempFace.ncamera = VarSaveNumber(row[12]);
			VarSaveString(tempFace.cameraName,row[13]);
			VarSaveString(tempFace.sip,row[14]);
			tempFace.deal = VarSaveNumber(row[15]);
			tempFace.temp_db_nid = VarSaveNumber(row[16]);
			tempFace.RW_tempID = VarSaveNumber(row[17]);
			tempFace.RW_personID = VarSaveNumber(row[18]);
			tempFace.RW_faceID = VarSaveNumber(row[19]);

			faceList.push_back(tempFace);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(faceList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::FACE_Capture_CmpRead(struct FACE_CAPTURE_ST &tempFace,long RW_tempID,long RW_personID,long RW_faceID,
									  int sex,long ncamera,int flag,char *startTime,char *endTime)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	char tempsql[1024] = {0};
	char outsql[1024] = {0};

	if((flag&0x01) != 0)
	{
		_stprintf(tempsql,_T(" and NCAMERA = %d"),ncamera);
		strcpy(outsql,tempsql);
	}
	if((flag&0x02) != 0)
	{
		_stprintf(tempsql,_T(" and NSEX = %d"),sex);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}
	if((flag&0x08) != 0)
	{
		_stprintf(tempsql,_T(" and DGETTIME between '%s' and '%s'"),startTime,endTime);
		strcpy(strsql,outsql);
		_stprintf(outsql,_T("%s%s"),strsql,tempsql);
	}

	_stprintf(strsql,_T("select * from tb_face_capture where NRWTEMPID = %d and NRWPERSONID = %d and NRWFACEID = %d %s"),
						RW_tempID,RW_personID,RW_faceID,outsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			tempFace.nid = VarSaveNumber(row[0]);
			char strdate[32] = {0};
			VarSaveString(strdate,row[1]);
			sscanf(strdate,"%d-%d-%d %d:%d:%d",&tempFace.year,&tempFace.mon,&tempFace.day,
											  &tempFace.hour,&tempFace.min,&tempFace.sec);

			tempFace.sex = VarSaveNumber(row[2]);
			tempFace.age = VarSaveNumber(row[3]);
			tempFace.facetype = VarSaveNumber(row[4]);
			tempFace.quality = VarSaveNumber(row[5]);
			VarSaveString(tempFace.file,row[6]);
			tempFace.size = VarSaveNumber(row[7]);
			VarSaveString(tempFace.face,row[8]);
			tempFace.facesize = VarSaveNumber(row[9]);
			tempFace.isblack = VarSaveNumber(row[10]);
			tempFace.ncamera = VarSaveNumber(row[12]);
			VarSaveString(tempFace.cameraName,row[13]);
			VarSaveString(tempFace.sip,row[14]);
			tempFace.deal = VarSaveNumber(row[15]);
			tempFace.temp_db_nid = VarSaveNumber(row[16]);
			tempFace.RW_tempID = VarSaveNumber(row[17]);
			tempFace.RW_personID = VarSaveNumber(row[18]);
			tempFace.RW_faceID = VarSaveNumber(row[19]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

/**************************人脸报警表*******************************/
bool CYRSVMySQL::FACE_Alarm_AddNew(struct FACE_ALARM_ST alarmFace)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_face_alarm values(NULL,%d,%d,%d,%d,%d,%d,%d,'%s','%s')"),
						alarmFace.captureID,alarmFace.db_nid,alarmFace.RW_dbID,alarmFace.RW_personID,alarmFace.RW_faceID,
						alarmFace.similarity,alarmFace.ncamera,alarmFace.cameraName,alarmFace.sip);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}

	IOwriteLock=false;
	return true;
}

/**************************人脸报警视图*******************************/
long CYRSVMySQL::VS_FACE_Alarm_GetNum(long ncamera,int sex,int sage,int eage,char *startTime,char *endTime,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	char tempsql[1024] = {0};
	char outsql[1024] = {0};
	bool hadwhere=false;		//检查语句中是否已经有where

	if((flag&0x01) != 0)
	{	
		_stprintf(tempsql,_T(" where NCAMERA = %d"),ncamera);
		strcpy(outsql,tempsql);
		hadwhere = true;
	}
	if((flag&0x02) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and NSEX =%d"),sex);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where NSEX =%d"),sex);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x04) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and NAGE between %d and %d"),sage,eage);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where NAGE between %d and %d"),sage,eage);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			_stprintf(tempsql,_T(" and dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(strsql,outsql);
			_stprintf(outsql,_T("%s%s"),strsql,tempsql);
		}
		else
		{
			_stprintf(tempsql,_T(" where dcreate between '%s' and '%s'"),startTime,endTime);
			strcpy(outsql,tempsql);
			hadwhere = true;
		}
	}
	_stprintf(strsql,_T("select count(1) from vs_face_alarm %s"),outsql);
	strcpy(SQLstr,outsql);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::VS_FACE_Alarm_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct FACE_ALARM_VIEW_ST> &alarmList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};

	if(0 != flag)
	{
		_stprintf(strsql,_T("select * from vs_face_alarm %s order by dcreate limit %d,%d"),SQLstr,startNum-1,endNum-(startNum-1));
	}
	else
	{
		_stprintf(strsql,_T("select * from vs_face_alarm order by dcreate limit %d,%d"),startNum-1,endNum-(startNum-1));
	}
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct FACE_ALARM_VIEW_ST alarmInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&alarmInfo,0,sizeof(FACE_ALARM_VIEW_ST));
			alarmInfo.captureID = VarSaveNumber(row[0]);
			alarmInfo.db_nid =  VarSaveNumber(row[1]);
			alarmInfo.RW_dbID= VarSaveNumber(row[2]);
			alarmInfo.RW_personID= VarSaveNumber(row[3]);
			alarmInfo.RW_faceID= VarSaveNumber(row[4]);
			alarmInfo.similarity = VarSaveNumber(row[5]);
			alarmInfo.ncamera = VarSaveNumber(row[6]);
			VarSaveString(alarmInfo.cameraName,row[7]);
			VarSaveString(alarmInfo.sip,row[8]);
			alarmInfo.person_black_nid = VarSaveNumber(row[9]);
			VarSaveString(alarmInfo.name,row[10]);

			char strdate[32] = {0};
			VarSaveString(strdate,row[11]);
			sscanf(strdate,"%d-%d-%d",&alarmInfo.year,&alarmInfo.mon,&alarmInfo.day);
			alarmInfo.sex = VarSaveNumber(row[12]);
			VarSaveString(alarmInfo.phone,row[13]);
			alarmInfo.cardtype = VarSaveNumber(row[14]);
			VarSaveString(alarmInfo.cardNO,row[15]);
			alarmInfo.native = VarSaveNumber(row[16]);
			VarSaveString(alarmInfo.address,row[17]);
			VarSaveString(alarmInfo.face,row[18]);
			alarmInfo.facesize = VarSaveNumber(row[19]);
			VarSaveString(alarmInfo.file,row[20]);
			alarmInfo.size = VarSaveNumber(row[21]);
			VarSaveString(alarmInfo.dbname,row[22]);

			alarmList.push_back(alarmInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(alarmList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

long CYRSVMySQL::VS_FACE_Alarm_GetNum(long captureID)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select count(1) from vs_face_alarm where NCAPTUREID = %d"),captureID);

	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		int count = 0;
		while(row = mysql_fetch_row(res))
		{
			count = VarSaveNumber(row[0]);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		return count;
	}

	IOwriteLock=false;
	return 0;
}

bool CYRSVMySQL::VS_FACE_Alarm_ReadWithCaptureID(long captureID,long startNum,long endNum,list<struct FACE_ALARM_VIEW_ST> &alarmList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from vs_face_alarm where NCAPTUREID = %d order by NSIMILARITY desc limit %d,%d"),captureID,startNum-1,endNum-(startNum-1));
	
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return 0;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果
		struct FACE_ALARM_VIEW_ST alarmInfo = {0};
		while(row = mysql_fetch_row(res))
		{
			memset(&alarmInfo,0,sizeof(FACE_ALARM_VIEW_ST));
			alarmInfo.captureID = VarSaveNumber(row[0]);
			alarmInfo.db_nid =  VarSaveNumber(row[1]);
			alarmInfo.RW_dbID= VarSaveNumber(row[2]);
			alarmInfo.RW_personID= VarSaveNumber(row[3]);
			alarmInfo.RW_faceID= VarSaveNumber(row[4]);
			alarmInfo.similarity = VarSaveNumber(row[5]);
			alarmInfo.ncamera = VarSaveNumber(row[6]);
			VarSaveString(alarmInfo.cameraName,row[7]);
			VarSaveString(alarmInfo.sip,row[8]);
			alarmInfo.person_black_nid = VarSaveNumber(row[9]);
			VarSaveString(alarmInfo.name,row[10]);

			char strdate[32] = {0};
			VarSaveString(strdate,row[11]);
			sscanf(strdate,"%d-%d-%d",&alarmInfo.year,&alarmInfo.mon,&alarmInfo.day);
			alarmInfo.sex = VarSaveNumber(row[12]);
			VarSaveString(alarmInfo.phone,row[13]);
			alarmInfo.cardtype = VarSaveNumber(row[14]);
			VarSaveString(alarmInfo.cardNO,row[15]);
			alarmInfo.native = VarSaveNumber(row[16]);
			VarSaveString(alarmInfo.address,row[17]);
			VarSaveString(alarmInfo.face,row[18]);
			alarmInfo.facesize = VarSaveNumber(row[19]);
			VarSaveString(alarmInfo.file,row[20]);
			alarmInfo.size = VarSaveNumber(row[21]);
			VarSaveString(alarmInfo.dbname,row[22]);

			alarmList.push_back(alarmInfo);
		}
		mysql_free_result(res);
		IOwriteLock=false;
		if(alarmList.size() > 0)
			return true;
		else
			return false;
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::VS_FACE_Alarm_ReadOne(long captureID,long personID,struct FACE_ALARM_VIEW_ST &alarmInfo)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	char strsql[1024] = {0};
	_stprintf(strsql,_T("select * from vs_face_alarm where NCAPTUREID = %d and NID = %d"),captureID,personID);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		LOG_AddNewMysqlLog(DlgLogin.CurrentUser.user,mysql_error(&myConnection));
		IOwriteLock=false;
		return false;
	}
	else
	{
		MYSQL_RES * res;
		MYSQL_ROW row ;

		res = mysql_store_result(&myConnection);//取得查询结果

		while(row = mysql_fetch_row(res))
		{
			alarmInfo.captureID = VarSaveNumber(row[0]);
			alarmInfo.db_nid =  VarSaveNumber(row[1]);
			alarmInfo.RW_dbID= VarSaveNumber(row[2]);
			alarmInfo.RW_personID= VarSaveNumber(row[3]);
			alarmInfo.RW_faceID= VarSaveNumber(row[4]);
			alarmInfo.similarity = VarSaveNumber(row[5]);
			alarmInfo.ncamera = VarSaveNumber(row[6]);
			VarSaveString(alarmInfo.cameraName,row[7]);
			VarSaveString(alarmInfo.sip,row[8]);
			alarmInfo.person_black_nid = VarSaveNumber(row[9]);
			VarSaveString(alarmInfo.name,row[10]);

			char strdate[32] = {0};
			VarSaveString(strdate,row[11]);
			sscanf(strdate,"%d-%d-%d",&alarmInfo.year,&alarmInfo.mon,&alarmInfo.day);
			alarmInfo.sex = VarSaveNumber(row[12]);
			VarSaveString(alarmInfo.phone,row[13]);
			alarmInfo.cardtype = VarSaveNumber(row[14]);
			VarSaveString(alarmInfo.cardNO,row[15]);
			alarmInfo.native = VarSaveNumber(row[16]);
			VarSaveString(alarmInfo.address,row[17]);
			VarSaveString(alarmInfo.face,row[18]);
			alarmInfo.facesize = VarSaveNumber(row[19]);
			VarSaveString(alarmInfo.file,row[20]);
			alarmInfo.size = VarSaveNumber(row[21]);
			VarSaveString(alarmInfo.dbname,row[22]);

			mysql_free_result(res);
			IOwriteLock=false;
			return true;
		}
	}

	IOwriteLock=false;
	return false;
}

bool CYRSVMySQL::LOG_AddNewMysqlLog(char *userName,const char *logStr)
{
	char strsql[1024] = {0};
	_stprintf(strsql,_T("insert into tb_log values(NULL,NULL,'%s','%s')"),userName,logStr);
	if(mysql_query( &myConnection, strsql) != 0)
	{
		return false;
	}
	return true;
}