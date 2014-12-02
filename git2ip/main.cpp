#include "SignalDownload.h"

#define FILE_LEN_MAX 10240

int main(int a,char* b[])
{
	char fail[1024]="";
	char filestr[FILE_LEN_MAX]="";

	bool proxyflag=false;
	TCHAR proxystr[128]="";

	//开代理
	if(a>1)
	{	
		proxyflag=true;
		strcpy(proxystr,b[1]);
	}
	
	SignalDownload sd;
	sd.InitData();

	if(!sd.HTTPDownload("https://raw.githubusercontent.com/zogna/myip/master/wanip.txt",false,"",proxyflag,proxystr,fail,100,0))
	{
		sd.DestroyData();
		printf("无法连接原因:%s\n",fail);

		getchar();
		return 0;
	}
	memcpy(	filestr,sd.outstr,sd.totalstrlen);
	sd.DestroyData();

	/////////////////////////////////////////

	ShellExecute(0, "open", filestr, 0,0, SW_SHOWNORMAL );
	return 0;
}