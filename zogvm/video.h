#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "stdio.h"
#include "tchar.h"

#if OPEN_FFMEPG
void VideoInit(void);
int VideoPlay(TCHAR* filePath,long *sec,int *w,int *h)  ;

int AudioPlay(TCHAR* filePath,long *sec) ;

#endif
long long filesize(TCHAR* filePath);
void CHAR2UTF8(const char *str,char *utf8,unsigned long int len);
void UTF82CHAR(const char *utf8,char *str,unsigned long int len);

int ClearClipboradBuffer(void);
int AppendBufferToClipboard( char *cBuffer, int nSize );
int GetBufferFromClipboard( char *cBuffer, int nMaxSize );

#endif