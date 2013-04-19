#ifndef _READCONFIGTXT_H_
#define _READCONFIGTXT_H_

#define RCT_MAX_STR_SIZE 256

BOOL ReadFile(TCHAR* FileName,TCHAR *ipaddr,TCHAR *port,TCHAR *user,TCHAR *password,TCHAR *name);
BOOL WriteFile(TCHAR* FileName,TCHAR *ipaddr,TCHAR *port,TCHAR *user,TCHAR *password,TCHAR *name);

#endif