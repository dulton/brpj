#include "mylog.h"
//日志路径
TCHAR LogDir[MAX_PATH];

void initLogDir(TCHAR *path)
{
	//日志目录
	_stprintf(LogDir,_T("%s\\logDir"),path);
	CreateDirectory(LogDir, NULL);
}


//日志导出
void Log_fileout(char *type,char *fun,unsigned long line,char *str,...)
{
	va_list lVarArgs;
	va_start(lVarArgs, str);

	FILE *fp;

	SYSTEMTIME sys;
	GetLocalTime( &sys ); 

	char filepath[MAX_PATH];
	sprintf(filepath,"%s\\%04d_%02d_%02d.vslog.txt",LogDir,sys.wYear,sys.wMonth,sys.wDay);

	fp=fopen(filepath,"a+");
	if(fp)
	{
		//也打印到命令行
		vfprintf(stderr,str, lVarArgs);
		//写入日志
		fprintf(fp,"%04d-%02d-%02d %02d:%02d:%02d :[%s] [%s %d]:", 
			sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,
			type,fun,line);
		vfprintf(fp,str, lVarArgs);
		fflush(fp);
		fclose(fp);
	}
	va_end(lVarArgs);
}
