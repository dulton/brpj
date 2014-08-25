
#include "stdafx.h"
#include "ReadConfigTxt.h"

////////////加密
void EnCode(char *src,char *dst)
{
	int a,b;
	int i,len,v;
	TCHAR tempchar[16]="";

	//4个字符加密
	srand(GetTickCount()%60000);
	//必须加1
	a=rand()%30+1;
	b=rand()%99;
	_stprintf(dst,"%02d%02d",a,b);

	len=_tcslen(src);

	for(i=0;i<len;i++)
	{
		//加密算法
		v=(int)(src[i])*a+b;
		//合并
		_stprintf(tempchar,"%04d",v);
		_tcscat(dst,tempchar);
	}
}
///////////////解密
void DeCode(char *src,char *dst)
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

BOOL ReadFile(TCHAR* FileName,TCHAR *ipaddr,TCHAR *port,TCHAR *user,TCHAR *password,TCHAR *name)
{
	TCHAR temp[RCT_MAX_STR_SIZE]="";
	TCHAR tempchar[RCT_MAX_STR_SIZE]="";

	FILE *fp=_tfopen(FileName,_T("r"));
	if(fp)
	{
		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Ip:%s"),ipaddr);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Port:%s"),port);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
#if 1
		_stscanf(temp,_T("User:%s"),tempchar);
		DeCode(tempchar,user);
#else
		_stscanf(temp,_T("User:%s"),user);
#endif

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);


#if 1
		//解密
		_stscanf(temp,_T("Psw:%s"),tempchar);
		DeCode(tempchar,password);
#else
		_stscanf(temp,_T("Psw:%s"),password);
#endif
		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("DataBaseName:%s"),name);

		fclose(fp);

		return TRUE;
	}
	else
		return FALSE;
}

BOOL WriteFile(TCHAR* FileName,TCHAR *ipaddr,TCHAR *port,TCHAR *user,TCHAR *password,TCHAR *name)
{
	TCHAR temp[RCT_MAX_STR_SIZE]="";
	TCHAR tempchar[RCT_MAX_STR_SIZE]="";

	FILE *fp=_tfopen(FileName,_T("w"));
	if(fp)
	{
		_ftprintf(fp,_T("Ip:%s\n"),ipaddr);

		_ftprintf(fp,_T("Port:%s\n"),port);


#if 1
		EnCode(user,temp);
		_ftprintf(fp,_T("User:%s\n"),temp);
#else	
		_ftprintf(fp,_T("User:%s\n"),user);
#endif
		Sleep(1256);
#if 1
		EnCode(password,temp);
		_ftprintf(fp,_T("Psw:%s\n"),temp);
#else	
		_ftprintf(fp,_T("Psw:%s\n"),password);
#endif
		_ftprintf(fp,_T("DataBaseName:%s\n"),name);

		fclose(fp);

		return TRUE;
	}
	else
		return FALSE;
}