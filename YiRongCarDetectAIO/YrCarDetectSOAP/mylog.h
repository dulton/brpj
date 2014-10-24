#ifndef _MY_LOG_H_
#define _MY_LOG_H_

#include "stdio.h"
#include "tchar.h"
#include "windows.h"


#define LOG_ERROR "ERROR",__FUNCTION__, __LINE__
#define LOG_INFO "INFO",__FUNCTION__, __LINE__

void initLogDir(TCHAR *path);
void Log_fileout(char *type,char *fun,unsigned long line,char *str,...);


#endif