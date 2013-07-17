#include "StdAfx.h"
#include "SignalDownload.h"

#define LOWSPEEDLIMIT 256

//扩展定义
#define SD_curl_easy_setopt_EXT(A,B,C)			\
	res=curl_easy_setopt(A,B,C);				\
	if(CURLE_OK != res)  /* we failed */		\
	{											\
		fprintf(stderr, "ERROR:%s,%d,curl told us %s\n", __FILE__,__LINE__,curl_easy_strerror(res));	\
		fclose(outfile);						\
		outfile=NULL;							\
		curl_easy_cleanup(A);				\
		A=NULL;								\
		return false;							\
	}

//实时上传回调
size_t SD_ReadFunc(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	//fprintf(stderr, "*** We read %d bytes from file\n", nmemb);
	return fread(ptr, size, nmemb, stream);
}
//ConfigFile	配置文件路径
//filename 本地文件路径
//FtpPath FTP相对文件路径
//执行上传
bool SignalDownload::FTPUpload(TCHAR* ConfigFile,TCHAR *NetPath,TCHAR *FtpPath,TCHAR *filename,long timeout=10)
{
	TCHAR temp[SD_MAX_STR_SIZE];
	unsigned int i;
	FILE *fp=_tfopen(ConfigFile,_T("r"));
	if(fp)
	{
		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		//_stscanf(temp,_T("FtpPath:%s"),url);
		memcpy(url,&temp[8],(SD_MAX_STR_SIZE-8)*sizeof(TCHAR));
		i=_tcslen(url);
		url[i-1]=0;

		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		//_stscanf(temp,_T("FtpPath:%s"),url);
		memcpy(ftplocalpath,&temp[13],(SD_MAX_STR_SIZE-8)*sizeof(TCHAR));
		i=_tcslen(ftplocalpath);
		ftplocalpath[i-1]=0;

		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("FtpPort:%d"),&port);

		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("FtpUserPsw:%s"),userpsw);

		fclose(fp);
	}
	else
	{	
		fprintf(stderr, "configure file con't open\n");	
		return false;
	}

	curl=NULL;
	/* get a curl handle */
	curl = curl_easy_init();
	if(NULL == curl)
		return false;

	/* get the file size of the local file */
	/* get a FILE * of the same file */
	outfile=NULL;
	outfile = _tfopen(filename, _T("rb"));
	if(NULL== outfile)
	{
		curl_easy_cleanup(curl);
		curl=NULL;
		return false;
	}

	fseek(outfile,0,SEEK_END);
	fsize =	ftell(outfile);
	fseek(outfile,0,SEEK_SET);

	//printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

	/* enable uploading */
	SD_curl_easy_setopt_EXT(curl, CURLOPT_UPLOAD, 1L);

	/* specify target */
	_tcscat(url,FtpPath);
	//本地路径
	//_tcscat(ftplocalpath,FtpPath);
	_tcscat(ftplocalpath,NetPath);

	SD_curl_easy_setopt_EXT(curl,CURLOPT_URL, url);
	SD_curl_easy_setopt_EXT(curl,CURLOPT_USERPWD, userpsw);
	SD_curl_easy_setopt_EXT(curl,CURLOPT_PORT, port);

	//设置下载速度=0时 N次退出 
//	SD_curl_easy_setopt_EXT(curl, CURLOPT_TIMEOUT, timeout); 绝对不能出现。不然只连接timeout秒
	//设置URL地址错误 重连N次后推出
	SD_curl_easy_setopt_EXT(curl, CURLOPT_CONNECTTIMEOUT, timeout);
	SD_curl_easy_setopt_EXT(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
	//设置最低速度。为了中途拔网线
	SD_curl_easy_setopt_EXT(curl, CURLOPT_LOW_SPEED_LIMIT, LOWSPEEDLIMIT);
	SD_curl_easy_setopt_EXT(curl, CURLOPT_LOW_SPEED_TIME, timeout);

	/* pass in that last of FTP commands to run after the transfer */
	SD_curl_easy_setopt_EXT(curl, CURLOPT_FTPPORT, "-"); /* disable passive mode */
	SD_curl_easy_setopt_EXT(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);

//	SD_curl_easy_setopt_EXT(curl, CURLOPT_VERBOSE, 1L);

	/* we want to use our own read function */
	SD_curl_easy_setopt_EXT(curl, CURLOPT_READFUNCTION, SD_ReadFunc);

	/* now specify which file to upload */
	SD_curl_easy_setopt_EXT(curl, CURLOPT_READDATA, outfile);

	/* Set the size of the file to upload (optional).  If you give a *_LARGE
	option you MUST make sure that the type of the passed-in argument is a
	curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
	make sure that to pass in a type 'long' argument. */
	SD_curl_easy_setopt_EXT(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)fsize);

	SD_curl_easy_setopt_EXT(curl, CURLOPT_INFILESIZE,(curl_off_t)fsize);

	/* Now run off and do what you've been told! */
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(CURLE_OK != res)  /* we failed */				
	{												
		fprintf(stderr, "curl result %s\n",curl_easy_strerror(res));	
	//	sprintf(temp,"%s",curl_easy_strerror(res));
		fclose(outfile);
		outfile=NULL;
		curl_easy_cleanup(curl);
		curl=NULL;
		return false;							
	}	

	//关闭文件
	fclose(outfile);
	outfile=NULL;

	curl_easy_cleanup(curl);
	curl=NULL;

	return true;
}

//ConfigFile	配置文件路径
//filename 本地文件路径
//FtpPath FTP相对文件路径
//执行上传
bool SignalDownload::FTPtest(TCHAR* ConfigFile)
{
	int timeout =1;
	TCHAR temp[SD_MAX_STR_SIZE];
	unsigned int i;
	FILE *fp=_tfopen(ConfigFile,_T("r"));
	if(fp)
	{
		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		//_stscanf(temp,_T("FtpPath:%s"),url);
		memcpy(url,&temp[8],(SD_MAX_STR_SIZE-8)*sizeof(TCHAR));
		i=_tcslen(url);
		url[i-1]=0;

		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		//_stscanf(temp,_T("FtpPath:%s"),url);
		memcpy(ftplocalpath,&temp[13],(SD_MAX_STR_SIZE-8)*sizeof(TCHAR));
		i=_tcslen(ftplocalpath);
		ftplocalpath[i-1]=0;

		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("FtpPort:%d"),&port);

		_fgetts(temp,SD_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("FtpUserPsw:%s"),userpsw);

		fclose(fp);
	}
	else
	{	
		fprintf(stderr, "configure file con't open\n");	
		return false;
	}

	curl=NULL;
	/* get a curl handle */
	curl = curl_easy_init();
	if(NULL == curl)
		return false;

	SD_curl_easy_setopt_EXT(curl,CURLOPT_URL, url);
	SD_curl_easy_setopt_EXT(curl,CURLOPT_USERPWD, userpsw);
	SD_curl_easy_setopt_EXT(curl,CURLOPT_PORT, port);

	//设置下载速度=0时 N次退出
	SD_curl_easy_setopt_EXT(curl, CURLOPT_TIMEOUT, timeout);
	//设置URL地址错误 重连N次后推出
	SD_curl_easy_setopt_EXT(curl, CURLOPT_CONNECTTIMEOUT, timeout);
	SD_curl_easy_setopt_EXT(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
	//设置最低速度。为了中途拔网线
	SD_curl_easy_setopt_EXT(curl, CURLOPT_LOW_SPEED_LIMIT, LOWSPEEDLIMIT);
	SD_curl_easy_setopt_EXT(curl, CURLOPT_LOW_SPEED_TIME, timeout);

	/* Now run off and do what you've been told! */
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(CURLE_OK != res)  /* we failed */				
	{												
		fprintf(stderr, "curl result %s\n",curl_easy_strerror(res));	
	//	sprintf(temp,"%s",curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl=NULL;
		return false;							
	}	


	//关闭文件
	curl_easy_cleanup(curl);
	curl=NULL;

	return true;
}
