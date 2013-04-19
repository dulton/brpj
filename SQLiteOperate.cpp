#include "stdafx.h"
#include "SqliteOperate.h"

////////////////////////////////////////////////////
CSqliteOperate::CSqliteOperate()
{
	m_pDB = NULL;
	readProductSuccess = false;
	m_productInfo.nid = 0;
	strcpy(m_productInfo.RunningNumber,"");
	strcpy(m_productInfo.tag,"");
	strcpy(m_productInfo.MainCategory,"");
	strcpy(m_productInfo.SubCategory,"");
	strcpy(m_productInfo.MetaField,"");
	strcpy(m_productInfo.ColourDesc,"");
	strcpy(m_productInfo.Colour,"");
	strcpy(m_productInfo.Unit,"");
	strcpy(m_productInfo.FactoryItem,"");
	strcpy(m_productInfo.HmNum,"");
	strcpy(m_productInfo.Description,"");
	strcpy(m_productInfo.path1,"");
	strcpy(m_productInfo.path2,"");
	strcpy(m_productInfo.path3,"");
}

////////////////////////////////////////////////////
CSqliteOperate::~CSqliteOperate()
{
	CloseDB();
}

////////////////////////////////////////////////////
//打开数据库
int CSqliteOperate::OpenDB(char *file)
{
	return sqlite3_open_v2(file, &m_pDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}

////////////////////////////////////////////////////
//关闭数据库
void CSqliteOperate::CloseDB()
{
	if(m_pDB)
		sqlite3_close(m_pDB);
	m_pDB = 0;
}

////////////////////////////////////////////////////
//执行SQL语句
void CSqliteOperate::Sql_Execute(char *sql)
{
	sqlite3_prepare_v2(m_pDB, sql, strlen(sql) + 1, &stmt, NULL);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

////////////////////////////////////////////////////
//摄像头表
void CSqliteOperate::Camera_CreateTable(void)
{
	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_camera'");
	sqlite3_prepare_v2(m_pDB, sql.GetBuffer(0), strlen(sql.GetBuffer(0)) + 1, &stmt, NULL);

	if (SQLITE_ROW == sqlite3_step(stmt)) {
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_camera(id integer primary key,name nvarchar(24), \
												ip nvarchar(40), user nvarchar(20), \
												psw nvarchar(20), port integer)");
			Sql_Execute(sql.GetBuffer(0));
			//第一次创建四个空的摄像头
			struct CAMERA_INFO_ST cInfo;
			strcpy(cInfo.name,"");
			strcpy(cInfo.ip,"");
			strcpy(cInfo.user,"");
			strcpy(cInfo.psw,"");
			cInfo.port = 0;

			for(int i=0;i<MAX_PLAYWIN;i++)
			{
				Camera_Add(cInfo);
			}
		}
	}
}

////////////////////////////////////////////////////
void CSqliteOperate::Camera_Add(struct CAMERA_INFO_ST cInfo)
{
	CString sql;
	sql.Format("insert into tb_camera(name,ip,user,psw,port) values('%s', '%s', '%s', '%s', %d)",\
									  cInfo.name,cInfo.ip,cInfo.user,cInfo.psw,cInfo.port);
	Sql_Execute(sql.GetBuffer(0));
}

////////////////////////////////////////////////////
void CSqliteOperate::Camera_Modify(struct CAMERA_INFO_ST cInfo)
{
	CString sql;
	sql.Format("update tb_camera set name='%s',ip='%s',user='%s',psw='%s',port=%d where id=%d",\
									 cInfo.name,cInfo.ip,cInfo.user,cInfo.psw,cInfo.port,cInfo.nid);
	Sql_Execute(sql.GetBuffer(0));
}

////////////////////////////////////////////////////
void CSqliteOperate::Camera_Delete(unsigned long int nid)
{
	CString sql;
	sql.Format("delete from tb_camera where id=%d",nid);
	Sql_Execute(sql.GetBuffer(0));
}

////////////////////////////////////////////////////
static int ReadCameraCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct CAMERA_INFO_ST> *mSqlite = (list<struct CAMERA_INFO_ST> *)context; 

	struct CAMERA_INFO_ST cInfo = {0};

	sscanf(argv[0],"%d",&cInfo.nid);
	strcpy(cInfo.name,argv[1]);
	strcpy(cInfo.ip,argv[2]);
	strcpy(cInfo.user,argv[3]);
	strcpy(cInfo.psw,argv[4]);

	if(strlen(argv[5]))
		sscanf(argv[5],"%d",&cInfo.port);
	else
		cInfo.port=0;

	mSqlite->push_back(cInfo);

    return 0;
}

////////////////////////////////////////////////////
bool CSqliteOperate::Camera_Read(int maxNum,list<struct CAMERA_INFO_ST> &cameraList)
{
	CString strSql;
	strSql.Format(_T("select * from tb_camera limit %d"),maxNum);
	char* cErrMsg;
	int res = sqlite3_exec(m_pDB, strSql.GetBuffer(0), ReadCameraCallBack , &cameraList , &cErrMsg); 
	if(res == SQLITE_OK)
	{
		if(cameraList.size())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////
//产品信息表
void CSqliteOperate::Product_CreateTable(void)
{
	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_product'");
	sqlite3_prepare_v2(m_pDB, sql.GetBuffer(0), strlen(sql.GetBuffer(0)) + 1, &stmt, NULL);

	if (SQLITE_ROW == sqlite3_step(stmt)) {
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_product(id integer primary key,RunningNumber nvarchar(256), \
												tag nvarchar(256), MainCategory nvarchar(256), \
												SubCategory nvarchar(256), MetaField nvarchar(256), \
												ColourDesc nvarchar(256), Colour nvarchar(256), \
												Unit nvarchar(256), FactoryItem nvarchar(256), \
												HmNum nvarchar(256), Description nvarchar(2560), \
												path1 nvarchar(260), path2 nvarchar(260), \
												path3 nvarchar(260))");
			Sql_Execute(sql.GetBuffer(0));
		}
	}
}

////////////////////////////////////////////////////
void CSqliteOperate::Product_Add(struct PRODUCT_INFO_ST pInfo)
{
	CString sql;
	sql.Format("insert into tb_product(RunningNumber,tag,MainCategory,SubCategory,\
										MetaField,ColourDesc,Colour,Unit,FactoryItem,\
										HmNum,Description,path1,path2,path3) \
										values('%s', '%s', '%s', '%s', '%s', '%s', '%s', \
										'%s', '%s', '%s', '%s', '%s', '%s', '%s')",\
										pInfo.RunningNumber,pInfo.tag,pInfo.MainCategory,pInfo.SubCategory,\
										pInfo.MetaField,pInfo.ColourDesc,pInfo.Colour,pInfo.Unit,pInfo.FactoryItem,\
										pInfo.HmNum,pInfo.Description,pInfo.path1,pInfo.path2,pInfo.path3);
	Sql_Execute(sql.GetBuffer(0));
}

////////////////////////////////////////////////////
void CSqliteOperate::Product_Modify(struct PRODUCT_INFO_ST pInfo)
{
#if 0
	CString sql;
	sql.Format("update tb_product set barcode='%s',name='%s',description='%s',path1='%s',path2='%s',path3='%s' where id=%d",\
										pInfo.barcode,pInfo.name,pInfo.spec,pInfo.path1,pInfo.path2,pInfo.path3,pInfo.nid);
	Sql_Execute(sql.GetBuffer(0));
#endif
}

////////////////////////////////////////////////////
void CSqliteOperate::Product_Delete(unsigned long int nid)
{
#if 0
	CString sql;
	sql.Format("delete from tb_product where id=%d",nid);
	Sql_Execute(sql.GetBuffer(0));
#endif
}

////////////////////////////////////////////////////
static int ReadProductCallBack(void *context, int argc, char **argv, char **ColName)
{
	CSqliteOperate *mSqlite = (CSqliteOperate*)context; 
	mSqlite->readProductSuccess = true;

	sscanf(argv[0],"%d",&mSqlite->m_productInfo.nid);
	strcpy(mSqlite->m_productInfo.RunningNumber,argv[1]);
	strcpy(mSqlite->m_productInfo.tag,argv[2]);
	strcpy(mSqlite->m_productInfo.MainCategory,argv[3]);
	strcpy(mSqlite->m_productInfo.SubCategory,argv[4]);
	strcpy(mSqlite->m_productInfo.MetaField,argv[5]);
	strcpy(mSqlite->m_productInfo.ColourDesc,argv[6]);
	strcpy(mSqlite->m_productInfo.Colour,argv[7]);
	strcpy(mSqlite->m_productInfo.Unit,argv[8]);
	strcpy(mSqlite->m_productInfo.FactoryItem,argv[9]);
	strcpy(mSqlite->m_productInfo.HmNum,argv[10]);
	strcpy(mSqlite->m_productInfo.Description,argv[11]);
	strcpy(mSqlite->m_productInfo.path1,argv[12]);
	strcpy(mSqlite->m_productInfo.path2,argv[13]);
	strcpy(mSqlite->m_productInfo.path3,argv[14]);
    return 0;
}

////////////////////////////////////////////////////
bool CSqliteOperate::Product_Read(char *barcode,struct PRODUCT_INFO_ST &pInfo)
{
	CString sql;
	sql.Format(_T("select * from tb_product where RunningNumber='%s' or tag = '%s'"),barcode,barcode);
	readProductSuccess = false;
	char* cErrMsg;
	int res = sqlite3_exec(m_pDB, sql.GetBuffer(0), ReadProductCallBack , this , &cErrMsg); 
	if(res == SQLITE_OK)
	{
		if(readProductSuccess)
		{
			pInfo.nid = m_productInfo.nid;
			strcpy(pInfo.RunningNumber,m_productInfo.RunningNumber);
			strcpy(pInfo.tag,m_productInfo.tag);
			strcpy(pInfo.MainCategory,m_productInfo.MainCategory);
			strcpy(pInfo.SubCategory,m_productInfo.SubCategory);
			strcpy(pInfo.MetaField,m_productInfo.MetaField);
			strcpy(pInfo.ColourDesc,m_productInfo.ColourDesc);
			strcpy(pInfo.Colour,m_productInfo.Colour);
			strcpy(pInfo.Unit,m_productInfo.Unit);
			strcpy(pInfo.FactoryItem,m_productInfo.FactoryItem);
			strcpy(pInfo.HmNum,m_productInfo.HmNum);
			strcpy(pInfo.Description,m_productInfo.Description);
			strcpy(pInfo.path1,m_productInfo.path1);
			strcpy(pInfo.path2,m_productInfo.path2);
			strcpy(pInfo.path3,m_productInfo.path3);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

////////////////////////////////////////////////////
//视频信息表
void CSqliteOperate::Video_CreateTable(void)
{
	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_video'");
	sqlite3_prepare_v2(m_pDB, sql.GetBuffer(0), strlen(sql.GetBuffer(0)) + 1, &stmt, NULL);

	if (SQLITE_ROW == sqlite3_step(stmt)) {
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_video(id integer primary key,RunningNumber nvarchar(256), \
												tag nvarchar(256), HmNum nvarchar(256), Description nvarchar(2560), \
												stime datetime, etime datetime, \
												size integer, path nvarchar(260))");
			Sql_Execute(sql.GetBuffer(0));
		}
	}
}

////////////////////////////////////////////////////
void CSqliteOperate::Video_Add(struct PRODUCT_INFO_ST pInfo,char *starttime,char *endtime,char *path,unsigned long size)
{
	CString sql;
	sql.Format("insert into tb_video(RunningNumber,tag,HmNum,Description,stime,etime,size,path) \
										values('%s', '%s', '%s', '%s', datetime('%s'), datetime('%s'), %d, '%s')",\
										pInfo.RunningNumber,pInfo.tag,pInfo.HmNum,pInfo.Description,\
										starttime,endtime,size,path);
	Sql_Execute(sql.GetBuffer(0));
}

////////////////////////////////////////////////////
void CSqliteOperate::Video_Delete(unsigned long int nid)
{
	CString sql;
	sql.Format("delete from tb_video where id=%d",nid);
	Sql_Execute(sql.GetBuffer(0));
}

////////////////////////////////////////////////////
static int ReadViedoCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct VIDEO_INFO_ST> *mSqlite = (list<struct VIDEO_INFO_ST> *)context; 

	struct VIDEO_INFO_ST vInfo = {0};
	char stime[32];
	char etime[32];
	sscanf(argv[0],"%d",&vInfo.nid);
	strcpy(vInfo.RunningNumber,argv[1]);
	strcpy(vInfo.tag,argv[2]);
	strcpy(vInfo.HmNum,argv[3]);
	strcpy(vInfo.Description,argv[4]);
	strcpy(stime,argv[5]);
	strcpy(etime,argv[6]);
	sscanf(argv[7],"%d",&vInfo.size);
	strcpy(vInfo.path,argv[8]);
	sscanf(stime,"%04d-%02d-%02d %02d:%02d:%02d",
					&vInfo.start_year,
					&vInfo.start_mon,
					&vInfo.start_day,
					&vInfo.start_hour,
					&vInfo.start_min,
					&vInfo.start_sec);
	sscanf(etime,"%04d-%02d-%02d %02d:%02d:%02d",
					&vInfo.end_year,
					&vInfo.end_mon,
					&vInfo.end_day,
					&vInfo.end_hour,
					&vInfo.end_min,
					&vInfo.end_sec);

	mSqlite->push_back(vInfo);

    return 0;
}

////////////////////////////////////////////////////
unsigned long CSqliteOperate::Video_GetCount(char *runnum,char *HmNum,char *Description,\
											 char *starttime,char *endtime,int flag,char *sql)
{
	CString strSql = _T("select count(1) from tb_video");
	CString tempSql = "";
	CString outSql = "";
	bool wflag = false;

	if((flag&0x01) != 0)
	{	
		tempSql.Format(_T(" where RunningNumber = '%s' or tag = '%s'"),runnum,runnum);
		wflag = true;
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	if((flag&0x02) != 0)
	{
		if(wflag)
		{
			tempSql.Format(_T(" and HmNum = '%s'"),HmNum);
		}
		else
		{
			tempSql.Format(_T(" where HmNum = '%s'"),HmNum);
			wflag = true;
		}
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	if((flag&0x04) != 0)
	{
		if(wflag)
		{
			tempSql.Format(_T(" and Description = '%s'"),Description);
		}
		else
		{
			tempSql.Format(_T(" where Description = '%s'"),Description);
			wflag = true;
		}
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	if((flag&0x08) != 0)
	{
		if(wflag)
		{
			tempSql.Format(_T(" and stime >= strftime('%s') and etime <= strftime('%s')"),starttime,endtime);
		}
		else
		{
			tempSql.Format(_T(" where stime >= strftime('%s') and etime <= strftime('%s')"),starttime,endtime);
			wflag = true;
		}
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	strcpy(sql,outSql.GetBuffer(0));

	unsigned long num = 0;
	sqlite3_prepare_v2(m_pDB, strSql.GetBuffer(0), strlen(strSql.GetBuffer(0)) + 1, &stmt, NULL);
	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		num = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);

	return num;
}

bool CSqliteOperate::Video_GetHistory(char *sql,int flag,int startNum,int endNum,\
									  list<struct VIDEO_INFO_ST> &videoList)
{
	CString strSql = "select id,RunningNumber,tag,HmNum,Description,strftime(stime),strftime(etime),size,path from tb_video";
	CString numSql;
	numSql.Format(_T(" limit %d offset %d"),(endNum-startNum+1),(startNum-1));
	strSql = strSql + sql + numSql;
	char* cErrMsg;
	int res = sqlite3_exec(m_pDB, strSql.GetBuffer(0), ReadViedoCallBack , &videoList , &cErrMsg); 
	if(res == SQLITE_OK)
	{
		if(videoList.size())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}