#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "stdio.h"
#include "tchar.h"
#include "SqliteOperate.h"

#include <vector>
#include <list>
using namespace::std;

#if OPEN_FFMEPG
void VideoInit(void);
int VideoPlay(TCHAR* filePath,long *sec,int *w,int *h)  ;

int AudioPlay(TCHAR* filePath,long *sec) ;

#endif
long long filesize(TCHAR* filePath);
void CHAR2UTF8(const char *str,char *utf8,unsigned long int len);
void UTF82CHAR(const char *utf8,char *str,unsigned long int len);

int ClearClipboradBuffer(void);
int AppendBufferToClipboard( char *cBuffer, unsigned long  nSize );
int GetBufferFromClipboard( char *cBuffer, unsigned long  nMaxSize );

CString GetFileExt(CString fileName);

CString GetFilePathNoName(CString filepath);
int checkExt(list<struct FILETYPE_ST> typeList,char *fileext) ;
void StrSplite(list<struct STR_SPLITE_S> &strList,char *src) ;
void checkStr(struct FILE_ST &data,list<struct ZIDIAN_ST> zidianList,list<struct STR_SPLITE_S> strList) ;
void FindAllFile(long long hdd_nid,CString hdd_area,
				 list<struct FILETYPE_ST> typeList,list<struct ZIDIAN_ST> zidianList);

void FindAllFile_NEW(long long hdd_nid,CString hdd_area,
					 list<struct FILETYPE_ST> typeList,list<struct ZIDIAN_ST> zidianList);

void CheckAllDoubleFile(long long hdd_nid);

void CheckAllIsFile(long long hdd_nid);
void GetEd2kUrl(char* path,char *name,char *dst);


#endif