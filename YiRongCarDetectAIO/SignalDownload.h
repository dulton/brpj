#ifndef __SIGNAL_DOWNLOAD_H
#define __SIGNAL_DOWNLOAD_H
#include "typedefine.h"
#include "tchar.h"


class SignalDownload
{
public:
	//url		URL地址 
	//filename	文件路径
	//timeout	设定下载超时时限
	//start		起始下载字节
	//输出		下载成功或失败
	//执行下载
	bool HTTPDownload(const char *url,const char *filename,TCHAR *failstr,long timeout,unsigned long long startsize);
	//当前完成字节
	double completesize;
	//文件总字节
	double totalsize;

	static void InitData();
	static void DestroyData();

private:
	CURL *curl;
	CURLcode res;
	FILE *outfile;
	char sizestr[128];
};

#endif