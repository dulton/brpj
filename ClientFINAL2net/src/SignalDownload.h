#ifndef __SIGNAL_DOWNLOAD_H
#define __SIGNAL_DOWNLOAD_H
#include "tchar.h"
#include "stdlib.h"
#include "curl/curl.h"
#include "curl/easy.h"

#define SD_MAX_STR_SIZE 256
#define	SD_CONFIG_TXT _T("ftp_config.txt")

class SignalDownload
{
public:
	TCHAR url[SD_MAX_STR_SIZE];
	TCHAR userpsw[SD_MAX_STR_SIZE];
	long port;
	TCHAR ftplocalpath[SD_MAX_STR_SIZE];

	unsigned long fsize; 
	bool FTPUpload(TCHAR* ConfigFile,TCHAR *NetPath,TCHAR *FtpPath,TCHAR *filename,long timeout);
	bool FTPtest(TCHAR* ConfigFile);
private:
	CURL *curl;
	CURLcode res;
	FILE *outfile;


};

#endif