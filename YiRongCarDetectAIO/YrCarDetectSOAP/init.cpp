#include "init.h"

//读数据库配置文件
bool ReadConfigFile(TCHAR* FileName,struct GLOBAL_CONFIG_S &dst)
{
	TCHAR temp[260]="";
	TCHAR tempchar[260]="";

	FILE *fp=_tfopen(FileName,_T("r"));
	if(fp)
	{
		_fgetts(temp,260,fp);
		_stscanf(temp,_T("Port:%d"),&dst.Port);

		_fgetts(temp,260,fp);
		_stscanf(temp,_T("UserID:%d"),&dst.UserID);

		_fgetts(temp,260,fp);
		_stscanf(temp,_T("UserLV:%d"),&dst.UserLV);

		_fgetts(temp,260,fp);
		_stscanf(temp,_T("AreaID:%d"),&dst.AreaID);

		fclose(fp);

		return true;
	}
	else
		return false;
}
