#include "StdAfx.h"
#include "ReadConfigTxt.h"

#define RCT_MAX_STR_SIZE 256


BOOL ReadConfigTxt::ReadFile(TCHAR* FileName)
{
	TCHAR temp[RCT_MAX_STR_SIZE];

	FILE *fp=_tfopen(FileName,_T("r"));
	if(fp)
	{
		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Ip:%s"),Ip);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Port:%s"),Port);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("User:%s"),User);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Psw:%s"),Psw);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("DataBaseName:%s"),DataBaseName);

		fclose(fp);

		return TRUE;
	}
	else
		return FALSE;
}