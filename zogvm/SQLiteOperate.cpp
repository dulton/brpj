#include "stdafx.h"
#include "SqliteOperate.h"
#include "video.h"

////////////////////////////////////////////////////
CSqliteOperate::CSqliteOperate()
{
	m_pDB = NULL;
	IOwriteLock=false;
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
void CSqliteOperate::Sql_UpdateExecute(char *sql)
{
	if(m_pDB == 0)
		return;

	char * utf8=(char *)calloc(strlen(sql)*2+4,sizeof(char));

	CHAR2UTF8(sql,utf8,strlen(sql));
	sqlite3_prepare_v2(m_pDB, utf8, strlen(utf8) + 1, &stmt, NULL);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	free(utf8);
	utf8=NULL;
}
void CSqliteOperate::Sql_FindExecute(char *sql)
{
	if(m_pDB == 0)
		return;

	char * utf8=(char *)calloc(strlen(sql)*2+4,sizeof(char));
	CHAR2UTF8(sql,utf8,strlen(sql));
	sqlite3_prepare_v2(m_pDB, utf8, strlen(utf8) + 1, &stmt, NULL);
	free(utf8);
	utf8=NULL;
}

int CSqliteOperate::Sql_CallBackExecute(char *sql,int (*callback)(void*,int,char**,char**), void *list)
{
	if(m_pDB == 0)
		return SQLITE_ERROR;

	char *zErrMsg;
	char * utf8=(char *)calloc(strlen(sql)*2+4,sizeof(char));
	CHAR2UTF8(sql,utf8,strlen(sql));
	int res = sqlite3_exec(m_pDB, utf8, callback , list, &zErrMsg); 
	free(utf8);
	utf8=NULL;

	if( res != SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	return res;
}
//事务 插入数据加速
void CSqliteOperate::Begin(void)
{
	Sql_UpdateExecute("begin;");
}
void CSqliteOperate::Commit(void)
{
	Sql_UpdateExecute("commit;");
}

void CSqliteOperate::filterstring(char *str,char *dst)
{
	while(*str)   
	{  
		if( (*str) == '\'' )
		{
			(*dst) = '\'';
			dst++;
			(*dst) = '\'';
		}
		else
		{
			(*dst)=(*str);
		}
		dst++;
		str++;  
	} 
	(*dst)=0;
	dst++;
	(*dst)=0;
}

void CSqliteOperate::filterstringForLike(char *str,char *dst)
{
	CString A="";
	char b[256];
	char c[1024];
	int i=0;

	while(*str)   
	{  
		if( (*str) == '\'' || (*str) == ' ' || (*str) == '.'
			|| (*str) == '\"' || (*str) == '+'|| (*str) == '-'
			|| (*str) == ':')
		{
			if(i>0)
			{
				b[i]=0;
				if(!A.IsEmpty())
					A+="or ";
				else
					A+="( ";
				sprintf(c,"lower(name) like lower('%%%s%%') or lower(path) like lower('%%%s%%') ",b,b);
				A+=c;
				i=0;

			}
		}
		else
		{
			b[i]=(*str);
			i++;
		}
		str++;  
	} 
	if(i>0)
	{
		b[i]=0;
		if(!A.IsEmpty())
			A+="or ";
		else
			A+="( ";
		sprintf(c,"lower(name) like lower('%%%s%%') or lower(path) like lower('%%%s%%') ",b,b);
		A+=c;
		i=0;
	}
	if(!A.IsEmpty())
	{
		A+=") ";
	}

	strcpy(dst,A.GetBuffer(0));
}

void CSqliteOperate::filterstringNameForLike(char *str,char *dst)
{
	CString A="";
	char b[256];
	char c[1024];
	int i=0;

	while(*str)   
	{  
		if( (*str) == '\'' || (*str) == ' ' || (*str) == '.'
			|| (*str) == '\"' || (*str) == '+'|| (*str) == '-'
			|| (*str) == ':')
		{
			if(i>0)
			{
				b[i]=0;
				if(!A.IsEmpty())
					A+="or ";
				sprintf(c,"lower(name) like lower('%%%s%%') ",b);
				A+=c;
				i=0;

			}
		}
		else
		{
			b[i]=(*str);
			i++;
		}
		str++;  
	} 
	if(i>0)
	{
		b[i]=0;
		if(!A.IsEmpty())
			A+="or ";
		sprintf(c,"lower(name) like lower('%%%s%%') ",b);
		A+=c;
		i=0;
	}

	strcpy(dst,A.GetBuffer(0));
}

////////////////////////////////////////////////////



void CSqliteOperate::ZiDian_CreateTable(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_zidian'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_zidian(zidian_nid integer primary key,mainzidian integer, \
					   zidian nvarchar(16),lowzidian nvarchar(16))");
			Sql_UpdateExecute(sql.GetBuffer(0));

			//使用BEGIN 和COMMIT 事务加速
			Begin();

			ZiDian_Add(ZIDIAN_YAZHI,"WiKi");
			ZiDian_Add(ZIDIAN_YAZHI,"DoA");
			ZiDian_Add(ZIDIAN_YAZHI,"HDWTV");
			ZiDian_Add(ZIDIAN_YAZHI,"CMCT");
			ZiDian_Add(ZIDIAN_YAZHI,"DJYDXS");
			ZiDian_Add(ZIDIAN_YAZHI,"NGB");
			ZiDian_Add(ZIDIAN_YAZHI,"TTG");
			ZiDian_Add(ZIDIAN_YAZHI,"OoKU");
			ZiDian_Add(ZIDIAN_YAZHI,"TLF");
			ZiDian_Add(ZIDIAN_YAZHI,"PHD");
			ZiDian_Add(ZIDIAN_YAZHI,"BMDruCHinYaN");
			ZiDian_Add(ZIDIAN_YAZHI,"BlueBird");
			ZiDian_Add(ZIDIAN_YAZHI,"RUFGT");
			ZiDian_Add(ZIDIAN_YAZHI,"DIMENSION");
			ZiDian_Add(ZIDIAN_YAZHI,"BDGRP");
			ZiDian_Add(ZIDIAN_YAZHI,"HDAccess");
			ZiDian_Add(ZIDIAN_YAZHI,"SeeHD");
			ZiDian_Add(ZIDIAN_YAZHI,"SmY");
			ZiDian_Add(ZIDIAN_YAZHI,"SPRiNTER");
			ZiDian_Add(ZIDIAN_YAZHI,"MVGroup");
			ZiDian_Add(ZIDIAN_YAZHI,"c4tv");
			ZiDian_Add(ZIDIAN_YAZHI,"FTP");
			ZiDian_Add(ZIDIAN_YAZHI,"HDWinG");
			ZiDian_Add(ZIDIAN_YAZHI,"SADPANDA");
			ZiDian_Add(ZIDIAN_YAZHI,"RARBG");
			ZiDian_Add(ZIDIAN_YAZHI,"SPARKS");
			ZiDian_Add(ZIDIAN_YAZHI,"CHDTV");
			ZiDian_Add(ZIDIAN_YAZHI,"DON");
			ZiDian_Add(ZIDIAN_YAZHI,"HDCLUB");
			ZiDian_Add(ZIDIAN_YAZHI,"CtrlHD");
			ZiDian_Add(ZIDIAN_YAZHI,"SPLiTSViLLE");
			ZiDian_Add(ZIDIAN_YAZHI,"Smilodon");
			ZiDian_Add(ZIDIAN_YAZHI,"SHORTBREHD");
			ZiDian_Add(ZIDIAN_YAZHI,"NTb");
			ZiDian_Add(ZIDIAN_YAZHI,"SHORTBREHD");
			ZiDian_Add(ZIDIAN_YAZHI,"saMMie");

			ZiDian_Add(ZIDIAN_FENBIANLV,"720p");
			ZiDian_Add(ZIDIAN_FENBIANLV,"1080p");
			ZiDian_Add(ZIDIAN_FENBIANLV,"1080i");

			ZiDian_Add(ZIDIAN_HUAZHI,"WEB");
			ZiDian_Add(ZIDIAN_HUAZHI,"BDRip");
			ZiDian_Add(ZIDIAN_HUAZHI,"REBDRip");
			ZiDian_Add(ZIDIAN_HUAZHI,"BluRay");
			ZiDian_Add(ZIDIAN_HUAZHI,"HDTV");
			ZiDian_Add(ZIDIAN_HUAZHI,"Blu");
			ZiDian_Add(ZIDIAN_HUAZHI,"DVDRip");

			ZiDian_Add(ZIDIAN_3D,"3D");

			ZiDian_Add(ZIDIAN_YUYAN,"chs");
			ZiDian_Add(ZIDIAN_YUYAN,"cht");
			ZiDian_Add(ZIDIAN_YUYAN,"eng");

			char str[16];

			for(int i=1950;i<2050;i++)
			{
				sprintf(str,"%d",i);
				ZiDian_Add(ZIDIAN_YEAR,str);
			}

			Commit();

		}
	}

	IOwriteLock=false;
}


////////////////////////////////////////////////////
void CSqliteOperate::ZiDian_Add(int mainzidian,char *zidian)
{
	//在内部。不能加LOCK
	CString sql;

	char zidian2str[2048]="";
	filterstring(zidian,zidian2str);

	char str[16];

	strcpy(str,zidian2str);
	sql.Format("insert into tb_zidian(mainzidian,zidian,lowzidian) values(%d,'%s','%s');",\
		mainzidian,zidian2str,strlwr(str));
	Sql_UpdateExecute(sql.GetBuffer(0));
}


////////////////////////////////////////////////////
static int ZiDianCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct ZIDIAN_ST> *mSqlite = (list<struct ZIDIAN_ST> *)context; 

	struct ZIDIAN_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.zidian_nid);
	sscanf(argv[1],"%d",&cInfo.mainzidian);

	UTF82CHAR(argv[2],cInfo.zidian,strlen(argv[2]));
	UTF82CHAR(argv[3],cInfo.lowzidian,strlen(argv[3]));

	mSqlite->push_back(cInfo);

	return 0;
}

////////////////////////////////////////////////////
bool CSqliteOperate::ZiDian_Read(list<struct ZIDIAN_ST> &List)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;


	List.clear();
	CString strSql;
	strSql.Format(_T("select * from tb_zidian"));

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),ZiDianCallBack , &List); 

	IOwriteLock=false;

	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}

}


void CSqliteOperate::Double_CreateTable(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_double'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_double(double_nid integer primary key,file_nid int8)");
			Sql_UpdateExecute(sql.GetBuffer(0));
		}
	}

	IOwriteLock=false;
}


////////////////////////////////////////////////////
void CSqliteOperate::Double_Add(long long file_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;

	sql.Format("insert into tb_double(file_nid) values(%I64u)",
		file_nid);
	Sql_UpdateExecute(sql.GetBuffer(0));
	IOwriteLock=false;
}


////////////////////////////////////////////////////
static int DoubleCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct DOUBLE_FILE_ST> *mSqlite = (list<struct DOUBLE_FILE_ST> *)context; 

	struct DOUBLE_FILE_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.double_nid);
	sscanf(argv[1],"%I64u",&cInfo.file_nid);

	mSqlite->push_back(cInfo);

	return 0;
}

////////////////////////////////////////////////////
bool CSqliteOperate::Double_Read(list<struct DOUBLE_FILE_ST> &List)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	List.clear();
	CString strSql;
	strSql.Format(_T("select * from tb_double"));

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),DoubleCallBack , &List); 

	IOwriteLock=false;

	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}

}

void CSqliteOperate::Double_ReadOne(long long *double_nid,long long file_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	(*double_nid)=0;

	CString strSql;
	strSql.Format(_T("select double_nid from tb_double where file_nid=%I64u"),file_nid);
	Sql_FindExecute(strSql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		(*double_nid) = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
	}
	IOwriteLock=false;
}


////////////////////////////////////////////////////


void CSqliteOperate::Type_CreateTable(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_filetype'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_filetype(type_nid integer primary key,maintype integer, \
					   type nvarchar(16), enable integer)");
			Sql_UpdateExecute(sql.GetBuffer(0));

		}
	}

	IOwriteLock=false;
}

void CSqliteOperate::Type_AddAll(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	//使用BEGIN 和COMMIT 事务加速
	Begin();

	Type_Add(MAINTYPE_OTHER,".iso");

	Type_Add(MAINTYPE_VIDEO,".mkv");
	Type_Add(MAINTYPE_VIDEO,".avi");
	Type_Add(MAINTYPE_VIDEO,".mp4");
	Type_Add(MAINTYPE_VIDEO,".rmvb");
	Type_Add(MAINTYPE_VIDEO,".rm");	
	Type_Add(MAINTYPE_VIDEO,".mpeg");
	Type_Add(MAINTYPE_VIDEO,".asf");
	Type_Add(MAINTYPE_VIDEO,".mpg");	
	//	Type_Add(MAINTYPE_VIDEO,".dat");
	Type_Add(MAINTYPE_VIDEO,".wmv");
	Type_Add(MAINTYPE_VIDEO,".tp");
	Type_Add(MAINTYPE_VIDEO,".vob");
	Type_Add(MAINTYPE_VIDEO,".mov");	
	Type_Add(MAINTYPE_VIDEO,".3gp");
	Type_Add(MAINTYPE_VIDEO,".flv");
	Type_Add(MAINTYPE_VIDEO,".ts");
	Type_Add(MAINTYPE_VIDEO,".trp");	
	Type_Add(MAINTYPE_VIDEO,".nsr");
	Type_Add(MAINTYPE_VIDEO,".m4p");
	Type_Add(MAINTYPE_VIDEO,".m2ts");
	Type_Add(MAINTYPE_VIDEO,".m2t");
	Type_Add(MAINTYPE_VIDEO,".mts");
	Type_Add(MAINTYPE_VIDEO,".evo");

	Type_Add(MAINTYPE_MUSIC,".wav");
	Type_Add(MAINTYPE_MUSIC,".wma");
	Type_Add(MAINTYPE_MUSIC,".mpa");
	Type_Add(MAINTYPE_MUSIC,".mp2");
	Type_Add(MAINTYPE_MUSIC,".mp3");
	Type_Add(MAINTYPE_MUSIC,".flac");
	Type_Add(MAINTYPE_MUSIC,".ape");
	Type_Add(MAINTYPE_MUSIC,".mpc");
	Type_Add(MAINTYPE_MUSIC,".ogg");
	Type_Add(MAINTYPE_MUSIC,".acc");
	Type_Add(MAINTYPE_MUSIC,".mka");
	Type_Add(MAINTYPE_MUSIC,".mod");
	Type_Add(MAINTYPE_MUSIC,".ac3");
	Type_Add(MAINTYPE_MUSIC,".dts");
	Type_Add(MAINTYPE_MUSIC,".wv");

	Type_Add(MAINTYPE_SUB,".smi");
	Type_Add(MAINTYPE_SUB,".srt");
	Type_Add(MAINTYPE_SUB,".ass");
	Type_Add(MAINTYPE_SUB,".ssa");
	Type_Add(MAINTYPE_SUB,".sub");
	Type_Add(MAINTYPE_SUB,".idx"); //开启  挪动的时候用SUB来挪
	Type_Add(MAINTYPE_SUB,".sup");

	Commit();

	IOwriteLock=false;
}

////////////////////////////////////////////////////
void CSqliteOperate::Type_Add(int maintype,char *type)
{

	//在内部。不能加LOCK
	CString sql;

	char type2str[2048]="";
	filterstring(type,type2str);

	sql.Format("insert into tb_filetype(maintype,type,enable) values(%d,'%s', 1)",\
		maintype,type2str
		);
	Sql_UpdateExecute(sql.GetBuffer(0));
}

void CSqliteOperate::Type_Add_Lock(int maintype,char *type)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	//在内部。不能加LOCK
	CString sql;

	char type2str[2048]="";
	filterstring(type,type2str);

	sql.Format("insert into tb_filetype(maintype,type,enable) values(%d,'%s', 1)",\
		maintype,type2str
		);
	Sql_UpdateExecute(sql.GetBuffer(0));

	IOwriteLock=false;
}

void CSqliteOperate::Type_DeleteAll(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("delete from tb_filetype ");
	Sql_UpdateExecute(sql.GetBuffer(0));


	IOwriteLock=false;
}
////////////////////////////////////////////////////
void CSqliteOperate::Type_ModifyEnable(char* nid,int enable)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("update tb_filetype set enable=%d where type_nid=%s",\
		enable,nid);
	Sql_UpdateExecute(sql.GetBuffer(0));

	IOwriteLock=false;
}

////////////////////////////////////////////////////

////////////////////////////////////////////////////
static int FileTypeCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct FILETYPE_ST> *mSqlite = (list<struct FILETYPE_ST> *)context; 

	struct FILETYPE_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.type_nid);
	sscanf(argv[1],"%d",&cInfo.maintype);

	UTF82CHAR(argv[2],cInfo.type,strlen(argv[2]));

	sscanf(argv[3],"%d",&cInfo.enable);

	mSqlite->push_back(cInfo);

	return 0;
}

////////////////////////////////////////////////////
bool CSqliteOperate::Type_Read(list<struct FILETYPE_ST> &List)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;


	List.clear();
	CString strSql;
	strSql.Format(_T("select * from tb_filetype"));

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileTypeCallBack , &List); 

	IOwriteLock=false;

	if(res == SQLITE_OK)
	{
		if(List.size())
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
void CSqliteOperate::Hdd_CreateTable(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;


	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_hdd'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			//必须给SERNO设置UNIQUE 相当于 索引。后面才能用replace （当存在时更新 不存在时插入）
			//不适合该表。因为REPLACE 需要全字段都要插入。不然会被默认值代替。
			sql.Format("create table tb_hdd(hdd_nid integer primary key,serno nvarchar(32), \
					   area nvarchar(32), mark nvarchar(128), \
					   insertflag integer,	enable integer, \
					   TotalBytes int8,FreeBytes int8)");
			Sql_UpdateExecute(sql.GetBuffer(0));

		}
	}
	IOwriteLock=false;
}
void CSqliteOperate::Hdd_SetNonsert(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;

	sql.Format("update tb_hdd set insertflag=0 where insertflag=1");
	Sql_UpdateExecute(sql.GetBuffer(0));

	IOwriteLock=false;
}

void CSqliteOperate::Hdd_Add(char *serno,char *area,char *mark,long long TotalBytes,long long FreeBytes)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;


	CString sql;
	char area2str[2048]="";
	char mark2str[2048]="";
	filterstring(area,area2str);
	filterstring(mark,mark2str);

	sql.Format("select * from tb_hdd where serno='%s' limit 1",serno);
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		sql.Format("update tb_hdd set area='%s',mark='%s',insertflag=1,TotalBytes=%I64u ,FreeBytes=%I64u  where serno='%s'",
			area2str,mark2str,TotalBytes ,FreeBytes ,serno);
		Sql_UpdateExecute(sql.GetBuffer(0));
	}
	else
	{
		sql.Format("insert into tb_hdd(serno,area,mark,insertflag,enable,TotalBytes ,FreeBytes ) \
				   values('%s','%s','%s',1, 0,%I64u,%I64u)",
				   serno,area2str,mark2str,TotalBytes ,FreeBytes );
		Sql_UpdateExecute(sql.GetBuffer(0));
	}
	/*//不能用
	sql.Format("replace into tb_hdd(serno,area,mark,insertflag,TotalBytes ,FreeBytes ) \
	values('%s','%s','%s',1, %I64u,%I64u)",
	serno,area2str,mark2str,TotalBytes ,FreeBytes );
	Sql_UpdateExecute(sql.GetBuffer(0));
	*/

	IOwriteLock=false;
}

void CSqliteOperate::Hdd_ModifyEnable(char* nid,int enable)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;

	sql.Format("update tb_hdd set enable=%d where hdd_nid=%s",
		enable,nid);
	Sql_UpdateExecute(sql.GetBuffer(0));
	IOwriteLock=false;
}

////////////////////////////////////////////////////
static int HddCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct HDD_ST> *mSqlite = (list<struct HDD_ST> *)context; 

	struct HDD_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.hdd_nid);

	UTF82CHAR(argv[1],cInfo.serno,strlen(argv[1]));
	UTF82CHAR(argv[2],cInfo.area,strlen(argv[2]));
	UTF82CHAR(argv[3],cInfo.mark,strlen(argv[3]));

	sscanf(argv[4],"%d",&cInfo.insertflag);
	sscanf(argv[5],"%d",&cInfo.enable);
	sscanf(argv[6],"%I64u",&cInfo.TotalBytes);
	sscanf(argv[7],"%I64u",&cInfo.FreeBytes);

	mSqlite->push_back(cInfo);

	return 0;
}

////////////////////////////////////////////////////
bool CSqliteOperate::Hdd_Read(list<struct HDD_ST> &List)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	List.clear();
	CString strSql;
	strSql.Format(_T("select * from tb_hdd"));

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),HddCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}



void CSqliteOperate::File_CreateTable(void)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_file'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_file(file_nid integer primary key,name nvarchar(256), \
					   path nvarchar(256), hdd_nid int8, \
					   maintype integer,type nvarchar(16), \
					   resolutionW integer, resolutionH integer,\
					   filesize int8,filetime integer,md5 nvarchar(64),\
					   CreationTime int8,LastWriteTime int8,\
					   video_nid int8,\
					   zidian_3d nvarchar(16),zidian_fenbianlv nvarchar(16),\
					   zidian_yazhi nvarchar(16),zidian_huazhi nvarchar(16),\
					   zidian_year nvarchar(16),\
					   newflag integer,double_nid int8,\
					   Ed2kUrl nvarchar(512))");
			Sql_UpdateExecute(sql.GetBuffer(0));

		}
	}
	IOwriteLock=false;
}


void CSqliteOperate::File_CreateView(void)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("select count(1) from sqlite_master where type='view' and name='filelist_view'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create  view filelist_view as\
					   select *\
					   from  tb_file a\
					   left join tb_hdd b\
					   on a.hdd_nid = b.hdd_nid");
			Sql_UpdateExecute(sql.GetBuffer(0));

		}
	}
	IOwriteLock=false;
}

void CSqliteOperate::File_DeleteforHdd(long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("delete from tb_file where hdd_nid=%I64u",hdd_nid);
	Sql_UpdateExecute(sql.GetBuffer(0));


	IOwriteLock=false;
}
void CSqliteOperate::Hdd_DeleteforHdd(long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("delete from tb_hdd where hdd_nid=%I64u",hdd_nid);
	Sql_UpdateExecute(sql.GetBuffer(0));


	IOwriteLock=false;
}


void CSqliteOperate::File_SetNoFlagAll(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;

	sql.Format("update tb_file set newflag=0 where newflag=1");
	Sql_UpdateExecute(sql.GetBuffer(0));

	IOwriteLock=false;
}

void CSqliteOperate::File_SetNoFlagOne(long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;

	sql.Format("update tb_file set newflag=0 where hdd_nid=%I64u",hdd_nid);
	Sql_UpdateExecute(sql.GetBuffer(0));

	IOwriteLock=false;
}


//返回真 存在文件 返回假 不存在文件
bool CSqliteOperate::File_CheckDoublePos(char *name,char *path,long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	char sql[2048];

	char name2str[2048]="";
	char path2str[2048]="";
	filterstring(name,name2str);
	filterstring(path,path2str);

	sprintf(sql,"select * from tb_file where name='%s' and path='%s' and hdd_nid=%I64u limit 1",name2str,path2str,hdd_nid);
	Sql_FindExecute(sql);
	//有值
	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		//改为1
		sprintf(sql,"update tb_file set newflag=1 where name='%s' and path='%s' and hdd_nid=%I64u",name2str,path2str,hdd_nid);
		Sql_UpdateExecute(sql);

		IOwriteLock=false;
		return true;

	}
	//无值
	IOwriteLock=false;
	return false;
}



void CSqliteOperate::File_Add(struct FILE_ST data)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	char sql[2048];

	char name2str[2048]="";
	char path2str[2048]="";
	filterstring(data.name,name2str);
	filterstring(data.path,path2str);

	sprintf(sql,"insert into tb_file(name,path,hdd_nid,maintype,type,\
				resolutionW,resolutionH,filesize,filetime,md5,\
				CreationTime,LastWriteTime,video_nid,\
				zidian_3d,zidian_fenbianlv,zidian_yazhi,zidian_huazhi,zidian_year,\
				newflag,double_nid,Ed2kUrl)\
				values('%s','%s',%I64u,%d,'%s',%d,%d,%I64u,%d,'%s',%I64u,%I64u,%I64u,\
				'%s','%s','%s','%s','%s',1,0,'%s')",
				name2str,path2str,data.hdd_nid,data.maintype,data.type,
				data.resolutionW,data.resolutionH,
				data.filesize,data.filetime,data.md5,
				data.CreationTime,data.LastWriteTime,
				data.video_nid,
				data.zidian_3d,data.zidian_fenbianlv,
				data.zidian_yazhi,data.zidian_huazhi,data.zidian_year,
				data.Ed2kUrl);
	Sql_UpdateExecute(sql);
	IOwriteLock=false;

}

void CSqliteOperate::File_Delete(long long file_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("delete from tb_file where file_nid=%I64u",file_nid);
	Sql_UpdateExecute(sql.GetBuffer(0));
	IOwriteLock=false;
}

void CSqliteOperate::File_ResetDouble2Zero(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	//找出所有DOUBLE_NID 计数为1的值 并将此数据的DOUBLE NID设为0  防止单条信息存在。
	sql.Format("update tb_file set double_nid=0 where file_nid in \
			   (select file_nid from tb_file where double_nid >0 group by double_nid having count(0)=1)");
	Sql_UpdateExecute(sql.GetBuffer(0));
	IOwriteLock=false;
}

void CSqliteOperate::File_CleanDouble2Zero(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	//清空
	sql.Format("update tb_file set double_nid=0 where double_nid > 0 ");
	Sql_UpdateExecute(sql.GetBuffer(0));
	IOwriteLock=false;
}




void CSqliteOperate::File_filterUpDown(char *SQLstr,struct UPDOWN_ST updown)
{
	CString tempsql="";
	bool orflag=false;

	if(
		(updown.type_video &&
		updown.type_audio &&
		updown.type_sub &&
		updown.type_other ) ||
		(!updown.type_video &&
		!updown.type_audio &&
		!updown.type_sub &&
		!updown.type_other ) 
		)
	{
		// 全选  全不选  啥都不干 
		NULL;
	}
	else if(!(updown.type_video &&
		updown.type_audio &&
		updown.type_sub &&
		updown.type_other ) )
	{
		if(updown.issearch)
			tempsql+=" and ( ";
		else
			tempsql+=" where (";

		if(updown.type_video)
		{
			tempsql+=" maintype= 1 ";
			orflag=true;
		}
		if(updown.type_audio)
		{
			if(orflag)
				tempsql+=" or ";

			tempsql+=" maintype= 2 ";
			orflag=true;
		}
		if(updown.type_sub)
		{
			if(orflag)
				tempsql+=" or ";

			tempsql+=" maintype= 3 ";
			orflag=true;
		}
		if(updown.type_other)
		{
			if(orflag)
				tempsql+=" or ";

			tempsql+=" maintype= 0 ";
			orflag=true;
		}
		tempsql+=" ) ";
	}

	switch(updown.point)
	{
	case POINT_PATH:	
		if(updown.path)
			tempsql+=" order by hdd_nid asc,lower(path) asc ";			
		else					
			tempsql+=" order by hdd_nid desc,lower(path) desc ";	
		break;
	case	POINT_NAME:
		if(updown.name)
			tempsql+=" order by lower(name) asc ";			
		else					
			tempsql+=" order by lower(name) desc ";	
		break;
	case	POINT_CREATTIME:
		if(updown.creattime)
			tempsql+=" order by CreationTime asc ";			
		else					
			tempsql+=" order by CreationTime desc ";	
		break;
	case	POINT_LASTTIME:
		if(updown.lasttime)
			tempsql+=" order by LastWriteTime asc ";			
		else					
			tempsql+=" order by LastWriteTime desc ";	
		break;
	case	POINT_FILESIZE:
		if(updown.filesize)
			tempsql+=" order by filesize asc ";			
		else					
			tempsql+=" order by filesize desc ";	
		break;
	case	POINT_RESOLUTION:
		if(updown.resolution)
			tempsql+=" order by zidian_fenbianlv asc ";			
		else					
			tempsql+=" order by zidian_fenbianlv desc ";	
		break;

	}
	strcpy(SQLstr,tempsql.GetBuffer(0));

}

long long CSqliteOperate::File_Number(char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	sql.Format("select count(1) from filelist_view %s",SQLstr);
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}

long long CSqliteOperate::File_NumberForSearch(char *SQLstr,char *str)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	char likestr[5120];
	filterstringForLike(str,likestr);

	sql.Format("select count(1) from filelist_view where %s %s ",likestr,SQLstr);

	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}

long long CSqliteOperate::File_NumberForHDD(long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	sql.Format("select count(1) from filelist_view where hdd_nid=%I64u",hdd_nid);
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}


////////////////////////////////////////////////////
static int FileCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct FILE_VIEW_ST> *mSqlite = (list<struct FILE_VIEW_ST> *)context; 

	struct FILE_VIEW_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.file_data.file_nid);

	UTF82CHAR(argv[1],cInfo.file_data.name,strlen(argv[1]));
	UTF82CHAR(argv[2],cInfo.file_data.path,strlen(argv[2]));

	sscanf(argv[3],"%I64u",&cInfo.file_data.hdd_nid);
	sscanf(argv[4],"%d",&cInfo.file_data.maintype);

	UTF82CHAR(argv[5],cInfo.file_data.type,strlen(argv[5]));

	sscanf(argv[6],"%d",&cInfo.file_data.resolutionW);
	sscanf(argv[7],"%d",&cInfo.file_data.resolutionH);
	sscanf(argv[8],"%I64u",&cInfo.file_data.filesize);
	sscanf(argv[9],"%d",&cInfo.file_data.filetime);

	UTF82CHAR(argv[10],cInfo.file_data.md5,strlen(argv[10]));

	sscanf(argv[11],"%I64u",&cInfo.file_data.CreationTime);
	sscanf(argv[12],"%I64u",&cInfo.file_data.LastWriteTime);
	sscanf(argv[13],"%I64u",&cInfo.file_data.video_nid);

	UTF82CHAR(argv[14],cInfo.file_data.zidian_3d,strlen(argv[14]));
	UTF82CHAR(argv[15],cInfo.file_data.zidian_fenbianlv,strlen(argv[15]));
	UTF82CHAR(argv[16],cInfo.file_data.zidian_yazhi,strlen(argv[16]));
	UTF82CHAR(argv[17],cInfo.file_data.zidian_huazhi,strlen(argv[17]));
	UTF82CHAR(argv[18],cInfo.file_data.zidian_year,strlen(argv[18]));

	sscanf(argv[19],"%d",&cInfo.file_data.newflag);
	sscanf(argv[20],"%I64u",&cInfo.file_data.double_nid);

	UTF82CHAR(argv[21],cInfo.file_data.Ed2kUrl,strlen(argv[21]));

	sscanf(argv[22],"%I64u",&cInfo.hdd_data.hdd_nid);

	UTF82CHAR(argv[23],cInfo.hdd_data.serno,strlen(argv[23]));
	UTF82CHAR(argv[24],cInfo.hdd_data.area,strlen(argv[24]));
	UTF82CHAR(argv[25],cInfo.hdd_data.mark,strlen(argv[25]));

	sscanf(argv[26],"%d",&cInfo.hdd_data.insertflag);
	sscanf(argv[27],"%d",&cInfo.hdd_data.enable);

	sscanf(argv[28],"%I64u",&cInfo.hdd_data.TotalBytes);
	sscanf(argv[29],"%I64u",&cInfo.hdd_data.FreeBytes);

	mSqlite->push_back(cInfo);

	return 0;
}

bool CSqliteOperate::File_ReadHddPage(list<struct FILE_VIEW_ST> &List,long long hdd_nid,bool up,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();
	CString strSql;

	strSql.Format(_T("select * from filelist_view where hdd_nid=%I64u order by hdd_nid asc,lower(path) asc limit %I64u,%d"),hdd_nid,start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;
}

////////////////////////////////////////////////////
static int FileLiteCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct FILE_VIEW_LITE_ST> *mSqlite = (list<struct FILE_VIEW_LITE_ST> *)context; 

	struct FILE_VIEW_LITE_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.file_nid);

	UTF82CHAR(argv[1],cInfo.file_name,strlen(argv[1]));
	UTF82CHAR(argv[2],cInfo.file_path,strlen(argv[2]));

	sscanf(argv[4],"%d",&cInfo.file_maintype);
	sscanf(argv[8],"%I64u",&cInfo.filesize);

	UTF82CHAR(argv[10],cInfo.file_md5,strlen(argv[10]));

	sscanf(argv[20],"%I64u",&cInfo.double_nid);

	UTF82CHAR(argv[24],cInfo.hdd_area,strlen(argv[24]));


	mSqlite->push_back(cInfo);

	return 0;
}
////////////////////////////////////////////////////
//
bool CSqliteOperate::File_ReadHddAllwithVideoAudio(list<struct FILE_VIEW_LITE_ST> &List,long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();
	CString strSql;

	strSql.Format(_T("select * from filelist_view where maintype!=%d and hdd_nid=%I64u "),MAINTYPE_SUB,hdd_nid);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileLiteCallBack , &List); 


	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;
}

//旧文件
bool CSqliteOperate::File_ReadHddAllandOld(list<struct FILE_VIEW_LITE_ST> &List,long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();
	CString strSql;

	strSql.Format(_T("select * from filelist_view where newflag=0 and  hdd_nid=%I64u "),hdd_nid);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileLiteCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;
}

//查看是否重复文件
bool CSqliteOperate::File_CheckDouble(int maintype,long long size,char *name,char *md5,
									  list<struct FILE_VIEW_LITE_ST> &List)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	List.clear();
	CString strSql;

	char name2str[2048]="";
	//	filterstring(name,name2str);
	//strSql.Format(_T("select * from filelist_view where maintype=%d and (filesize=%I64u or lower(name) like lower('%%%s%%'))"),maintype,size,name2str);

	strSql.Format(_T("select * from filelist_view where maintype=%d and filesize=%I64u"),maintype,size);
	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileLiteCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;
}


void CSqliteOperate::File_SetDouble(long long double_nid,char *file_nid_str)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;

	sql.Format("update tb_file set double_nid=%I64u where file_nid in (%s)",double_nid,file_nid_str);
	Sql_UpdateExecute(sql.GetBuffer(0));

	IOwriteLock=false;
}

long long CSqliteOperate::File_DoubleNumber(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	sql.Format("select count(1) from filelist_view where double_nid >0");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}

bool CSqliteOperate::File_ReadDouble(list<struct FILE_VIEW_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	CString strSql;

	strSql.Format(_T("select * from filelist_view where double_nid >0 order by double_nid asc limit %I64u,%d"),start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileCallBack , &List); 


	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}

long long CSqliteOperate::File_TrashNumber(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	sql.Format("select count(1) from filelist_view where filesize=0 or path like '%%Sample\'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}

bool CSqliteOperate::File_ReadTrash(list<struct FILE_VIEW_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	CString strSql;

	strSql.Format(_T("select * from filelist_view where filesize=0 or path like '%%Sample\'order by double_nid asc limit %I64u,%d"),start,num);
	
	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileCallBack , &List); 


	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}

void CSqliteOperate::Double_DeleteAll(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("delete from tb_double");
	Sql_UpdateExecute(sql.GetBuffer(0));
	IOwriteLock=false;
}


////////////////////////////////////////////////////
bool CSqliteOperate::File_Read(char *SQLstr,list<struct FILE_VIEW_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	CString strSql;

	strSql.Format(_T("select * from filelist_view %s limit %I64u,%d"),SQLstr,start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileCallBack , &List); 


	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}

////////////////////////////////////////////////////
bool CSqliteOperate::File_ReadForSearch(char *SQLstr,char *str,list<struct FILE_VIEW_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	char likestr[5120];
	filterstringForLike(str,likestr);

	CString strSql;

	strSql.Format(_T("select * from filelist_view where %s %s limit %I64u,%d"),likestr,SQLstr,start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),FileCallBack , &List); 


	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}



long long CSqliteOperate::YYETS_Number(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	sql.Format("select count(1) from tb_yyets");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}

long long CSqliteOperate::YYETS_NumberForSearch(char *str)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	char likestr[5120];
	filterstringNameForLike(str,likestr);
	sql.Format("select count(1) from tb_yyets where %s",likestr);
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}
static int YYETSCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct YYETS_ST> *mSqlite = (list<struct YYETS_ST> *)context; 

	struct YYETS_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.yyets_nid);

	UTF82CHAR(argv[1],cInfo.name,strlen(argv[1]));
	UTF82CHAR(argv[2],cInfo.filesize,strlen(argv[2]));
	UTF82CHAR(argv[3],cInfo.magnet,strlen(argv[3]));

	if(strlen(argv[4]) < 500)
		UTF82CHAR(argv[4],cInfo.ed2k,strlen(argv[4]));

	UTF82CHAR(argv[5],cInfo.wanpan,strlen(argv[5]));

	mSqlite->push_back(cInfo);

	return 0;
}

bool CSqliteOperate::YYETS_Read(list<struct YYETS_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	CString strSql;

	strSql.Format(_T("select * from tb_yyets limit %I64u,%d"),start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),YYETSCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}

////////////////////////////////////////////////////
bool CSqliteOperate::YYETS_ReadForSearch(char *str,list<struct YYETS_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	char likestr[5120];
	filterstringNameForLike(str,likestr);

	CString strSql;

	strSql.Format(_T("select * from tb_yyets where %s order by lower(name) asc limit %I64u,%d"),likestr,start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),YYETSCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}


void CSqliteOperate::ExternED2K_CreateTable(void)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;
	sql.Format("select count(1) from sqlite_master where type='table' and name='tb_ed2k'");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		int row = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if(0 == row)
		{
			sql.Format("create table tb_ed2k(nid integer primary key,\
					   name nvarchar(260) unique,\
					   filesize int8,\
					   ed2k nvarchar(512));");
			Sql_UpdateExecute(sql.GetBuffer(0));

		}
	}
	IOwriteLock=false;
}

void CSqliteOperate::ExternED2K_Add(struct EXTERN_ED2K_ST data)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	char sql[2048];

	char name2str[2048]="";
	char ed2k2str[2048]="";
	filterstring(data.name,name2str);
	filterstring(data.ed2k,ed2k2str);

	sprintf(sql,"replace into tb_ed2k(name,filesize,ed2k)\
				values('%s',%I64u,'%s')",
				name2str,data.filesize,ed2k2str);
	Sql_UpdateExecute(sql);
	IOwriteLock=false;

}



long long CSqliteOperate::ExternED2K_Number(void)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	sql.Format("select count(1) from tb_ed2k");
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}

long long CSqliteOperate::ExternED2K_NumberForSearch(char *str)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	CString sql;
	char likestr[5120];
	filterstringNameForLike(str,likestr);
	sql.Format("select count(1) from tb_ed2k where %s",likestr);
	Sql_FindExecute(sql.GetBuffer(0));

	if (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		long long row = sqlite3_column_int64(stmt, 0);
		sqlite3_finalize(stmt);
		IOwriteLock=false;
		return row;
	}
	IOwriteLock=false;
	return 0;
}
static int ExternED2KCallBack(void *context, int argc, char **argv, char **ColName)
{
	list<struct EXTERN_ED2K_ST> *mSqlite = (list<struct EXTERN_ED2K_ST> *)context; 

	struct EXTERN_ED2K_ST cInfo = {0};

	sscanf(argv[0],"%I64u",&cInfo.nid);

	UTF82CHAR(argv[1],cInfo.name,strlen(argv[1]));
	sscanf(argv[2],"%I64u",&cInfo.filesize);
	UTF82CHAR(argv[3],cInfo.ed2k,strlen(argv[3]));

	mSqlite->push_back(cInfo);

	return 0;
}

bool CSqliteOperate::ExternED2K_Read(list<struct EXTERN_ED2K_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	CString strSql;

	strSql.Format(_T("select * from tb_ed2k limit %I64u,%d"),start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),ExternED2KCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}

////////////////////////////////////////////////////
bool CSqliteOperate::ExternED2K_ReadForSearch(char *str,list<struct EXTERN_ED2K_ST> &List,long long start,int num)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;
	List.clear();

	char likestr[5120];
	filterstringNameForLike(str,likestr);

	CString strSql;

	strSql.Format(_T("select * from tb_ed2k where %s order by lower(name) asc limit %I64u,%d"),likestr,start,num);

	int res = Sql_CallBackExecute(strSql.GetBuffer(0),ExternED2KCallBack , &List); 

	IOwriteLock=false;
	if(res == SQLITE_OK)
	{
		if(List.size())
			return true;
	}
	return false;

}



void CSqliteOperate::File_CleanIDX(long long hdd_nid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}
	IOwriteLock=true;

	CString sql;

	sql.Format("delete from tb_file where type='.idx' and hdd_nid=%I64u and name \
				not in \
				(select  replace (name,'.sub','.idx') from tb_file where type='.sub' and hdd_nid=%I64u)",
				hdd_nid,hdd_nid);
	Sql_UpdateExecute(sql.GetBuffer(0));

	IOwriteLock=false;
}
