#include "stdafx.h"
#include "video.h"

#include "rhashinclude\rhash.h"
extern CSqliteOperate SQLDB;

#if OPEN_FFMEPG
#ifdef __cplusplus
extern "C" 
{
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
}
#endif
int VideoPlay(TCHAR* filePath,long *sec,int *w,int *h) 
{
	AVFormatContext *pFormatCtx=NULL;
	unsigned int             i;
	int videoStream;
	AVCodecContext  *pCodecCtx=NULL;
	
	// Register all formats and codecs
	//av_register_all();
    // Open video file
	if(av_open_input_file(&pFormatCtx, filePath, NULL, 0, NULL)!=0)
		return -1; // Couldn't open file
	
	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0)
	{
		av_close_input_file(pFormatCtx);
		return -1; // Couldn't find stream information
	}
	
	// Dump information about file onto standard error
	dump_format(pFormatCtx, 0, filePath, 0);
    // Find the first video stream
	videoStream=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO) 
		{
			videoStream=i;
			break;
		}
	}
	
	if(videoStream==-1)
	{
		av_close_input_file(pFormatCtx);
		return -1; // Didn't find a video stream
	}
	// Get a pointer to the codec context for the video stream
	pCodecCtx=pFormatCtx->streams[videoStream]->codec;

	if(0==pCodecCtx->width || 0==pCodecCtx->height)
	{
		av_close_input_file(pFormatCtx);
		fprintf(stderr, "codec w h unkown!\n");
		return -1;
	}
	/*
	// 计算总时长
	unsigned long long tns, thh, tmm, tss,us;
	//秒
	tns  = pFormatCtx->duration / AV_TIME_BASE;
	//微秒
	us = pFormatCtx->duration % AV_TIME_BASE * 100 / AV_TIME_BASE;
	//时
	thh  = tns / 3600;
	//分
	tmm  = (tns % 3600) / 60;
	//秒
	tss  = (tns % 60);
	*/

	(*sec)=pFormatCtx->duration / AV_TIME_BASE;
	(*w)=pCodecCtx->width ;
	(*h)=pCodecCtx->height;

	// Close the video file
	av_close_input_file(pFormatCtx);
	
	pFormatCtx=NULL;
	pCodecCtx=NULL;

	return 0;
}

void VideoInit(void)
{
	av_register_all();
}
int AudioPlay(TCHAR* filePath,long *sec) 
{
	AVFormatContext *pFormatCtx=NULL;
	// Register all formats and codecs
	//av_register_all();
	// Open video file
	if(av_open_input_file(&pFormatCtx, filePath, NULL, 0, NULL)!=0)
		return -1; // Couldn't open file

	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0)
	{
		av_close_input_file(pFormatCtx);
		return -1; // Couldn't find stream information
	}

	// Dump information about file onto standard error
	dump_format(pFormatCtx, 0, filePath, 0);
	/*
	// 计算总时长
	unsigned long long tns, thh, tmm, tss,us;
	//秒
	tns  = pFormatCtx->duration / AV_TIME_BASE;
	//微秒
	us = pFormatCtx->duration % AV_TIME_BASE * 100 / AV_TIME_BASE;
	//时
	thh  = tns / 3600;
	//分
	tmm  = (tns % 3600) / 60;
	//秒
	tss  = (tns % 60);
	*/

	(*sec)=pFormatCtx->duration / AV_TIME_BASE;

	// Close the video file
	av_close_input_file(pFormatCtx);

	pFormatCtx=NULL;

	return 0;
}


#endif

long long filesize(TCHAR* filePath)
{
	FILE *fp;

	long long s=-1;
	fp=fopen(filePath,"rb");
	if(fp)
	{
		_fseeki64(fp,0,SEEK_END );
		s=_ftelli64(fp);
		fclose(fp);
	}
	return s;
}

void CHAR2UTF8(const char *str,char *utf8,unsigned long int len)
{
	wchar_t * wbuff=(wchar_t *)calloc(len*2+4/*+4*/,sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, len+2);  
	// buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);  
	//  utf8 = new char[buffLen+1];  
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)utf8, len*2+4, 0, 0); 
	free(wbuff);
	wbuff=NULL;
}

void UTF82CHAR(const char *utf8,char *str,unsigned long int len)
{
	wchar_t * wbuff=(wchar_t *)calloc(len*2+4,sizeof(wchar_t));
	//	int buffLen = 0;  
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wbuff, len+2);  
	//	buffLen = WideCharToMultiByte(CP_ACP, 0, wbuff, -1, NULL, 0, 0, 0);  
	//	m_gb2312 = new char[buffLen+1];  
	WideCharToMultiByte(CP_ACP, 0, wbuff, -1, (LPSTR)str, len*2+4, 0, 0);  
	free(wbuff);
	wbuff=NULL;
}

//将字符串放到剪切板
int AppendBufferToClipboard( char *cBuffer, unsigned long nSize )
{
	HGLOBAL hClipboard = NULL;
	char *pClipBuf = NULL;

	if( OpenClipboard( NULL ) )
	{ 
		hClipboard = GlobalAlloc( GMEM_DDESHARE, nSize+1 );
		pClipBuf = (char*)GlobalLock(hClipboard);
		memset( pClipBuf, 0, nSize+1 );
		memcpy( pClipBuf, cBuffer, nSize );
		GlobalUnlock( hClipboard );
		SetClipboardData( CF_TEXT, hClipboard );
		CloseClipboard();
	}
	else
	{
		return -1;
	}

	return 0;
}
//从剪切板获取字符串
int GetBufferFromClipboard( char *cBuffer, unsigned long  nMaxSize )
{
	unsigned long nNeedSize = 0;
	HANDLE hClipboard = NULL;
	char *pClipBuf = NULL;

	if ( OpenClipboard( NULL ) )
	{
		hClipboard = GetClipboardData( CF_TEXT );
		nNeedSize = GlobalSize( hClipboard );
		if ( nNeedSize > nMaxSize+1 )
		{
			return -1;
		}

		pClipBuf = (char*)GlobalLock( hClipboard );
		memset( cBuffer, 0, nMaxSize );
		memcpy( cBuffer, pClipBuf, nNeedSize );
		GlobalUnlock( hClipboard );
		CloseClipboard();

		return nNeedSize;

	}
	else
	{
		return 0;
	}


}
//清空剪切板
int ClearClipboradBuffer(void)
{
	HGLOBAL hClipboard = NULL;

	if( OpenClipboard( NULL ) )
	{ 
		hClipboard = GetClipboardData( CF_TEXT );
		GlobalFree( hClipboard );
		EmptyClipboard();
		CloseClipboard();
		return 0;
	}
	else
	{
		return -1;
	}
}

//获取文件后缀,并变小写
CString GetFileExt(CString fileName)
{
	return (fileName.Right(fileName.GetLength()-fileName.ReverseFind('.'))).MakeLower();
}

//获取文件相对路径。去除盘符和名字
CString GetFilePathNoName(CString filepath)
{
	CString path=filepath.Right(filepath.GetLength()-filepath.Find('\\')-1);
	return path.Left(path.ReverseFind('\\')+1);

}
//比较后缀
int checkExt(list<struct FILETYPE_ST> typeList,char *fileext) 
{
	list<struct FILETYPE_ST>::iterator beglist;

	for(beglist=typeList.begin();beglist!=typeList.end();beglist++)
	{
		if(beglist->enable && 0== strcmp(beglist->type,fileext) )
		{
			return beglist->maintype ;
		}
	}
	// -1为无效
	return -1;
}

struct STR_SPLITE_S
{
	char s[256];
};

//分割字符串
void StrSplite(list<struct STR_SPLITE_S> &strList,char *src) 
{
	struct STR_SPLITE_S a={0};
	int i=0;
	strList.clear();

	while(*src)
	{
		if(' '==(*src) || '.'==(*src) || '-'==(*src) || '_'==(*src)|| '+'==(*src))
		{
			//小写
			strlwr(a.s);
			strList.push_back(a);
			memset(&a,0,sizeof(struct STR_SPLITE_S));
			i=0;
		}
		else
		{
			a.s[i]=(*src);
			i++;
		}
		src++;

	}
	strlwr(a.s);
	strList.push_back(a);
}

//比较字符串
void checkStr(struct FILE_ST &data,list<struct ZIDIAN_ST> zidianList,list<struct STR_SPLITE_S> strList) 
{
	list<struct STR_SPLITE_S>::iterator beglistA;
	list<struct ZIDIAN_ST>::iterator beglistB;

	for(beglistA=strList.begin();beglistA!=strList.end();beglistA++)
	{
		if(strlen(beglistA->s) <=16)
		{
			for(beglistB=zidianList.begin();beglistB!=zidianList.end();beglistB++)
			{
				if(0==strcmp(beglistA->s,beglistB->lowzidian))
				{
					if(ZIDIAN_YAZHI == beglistB->mainzidian)
					{
						strcpy(data.zidian_yazhi,beglistB->zidian);
					}
					else if(ZIDIAN_HUAZHI == beglistB->mainzidian)
					{
						strcpy(data.zidian_huazhi,beglistB->zidian);
					}
					else if(ZIDIAN_FENBIANLV == beglistB->mainzidian)
					{
						strcpy(data.zidian_fenbianlv,beglistB->zidian);
					}
					else if(ZIDIAN_3D == beglistB->mainzidian)
					{
						strcpy(data.zidian_3d,beglistB->zidian);
					}
					else if(ZIDIAN_YEAR == beglistB->mainzidian)
					{
						strcpy(data.zidian_year,beglistB->zidian);
					}
					break;
				}
			}
		}
	}

}


//遍历所有文件
void FindAllFile(long long hdd_nid,CString hdd_area,
				 list<struct FILETYPE_ST> typeList,list<struct ZIDIAN_ST> zidianList)
{

	FILETIME cTime;
	FILETIME lTime;
	CString fileName;
	CString fileExt;

    ULARGE_INTEGER  uli; 

	struct FILE_ST data;
	list<STR_SPLITE_S> strList;

	CFileFind fileFinder;
	CString filePath = hdd_area + _T("//*.*");

	BOOL bFinished = fileFinder.FindFile(filePath);

	while(bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();

		if(fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
		{
			FindAllFile(hdd_nid,fileFinder.GetFilePath(),typeList,zidianList);
		}
		else  //再判断是否为txt文件
		{
			//获取文件类型
			fileName = fileFinder.GetFileName();
			fileExt=GetFileExt(fileName);

			memset(&data,0,sizeof(struct FILE_ST ));
			//有效后缀
			data.maintype=checkExt(typeList,fileExt.GetBuffer(0));

			if(data.maintype >=0)
			{
				if(SQLDB.File_CheckDoublePos(fileName.GetBuffer(0),
					GetFilePathNoName(fileFinder.GetFilePath()).GetBuffer(0),
					hdd_nid))
					continue;

				strcpy(data.name,fileName.GetBuffer(0)	);
				strcpy(data.path,GetFilePathNoName(fileFinder.GetFilePath()).GetBuffer(0));

				data.hdd_nid=hdd_nid;
				strcpy(data.type,fileExt.GetBuffer(0));
				data.filesize=fileFinder.GetLength();

				if(fileFinder.GetCreationTime(&cTime))
				{
					uli.LowPart = cTime.dwLowDateTime;  
					uli.HighPart = cTime.dwHighDateTime;  
					data.CreationTime=uli.QuadPart;
				}
				else
					data.CreationTime=0;

				if(fileFinder.GetLastWriteTime(&lTime))
				{
					uli.LowPart = lTime.dwLowDateTime;  
					uli.HighPart = lTime.dwHighDateTime;  
					data.LastWriteTime=uli.QuadPart;
				}
				else 
					data.LastWriteTime=0;

			}
			if(MAINTYPE_OTHER == data.maintype )
			{
				strList.clear();
				StrSplite(strList,fileName.GetBuffer(0));
				checkStr(data,zidianList,strList); 
				strList.clear();
			}
			else if( MAINTYPE_SUB == data.maintype)
			{
				NULL;
			}
			else  if(MAINTYPE_VIDEO == data.maintype )
			{
#if OPEN_FFMEPG
				VideoPlay(fileFinder.GetFilePath().GetBuffer(0),
					&data.filetime,&data.resolutionW,&data.resolutionH);
#endif
				strList.clear();
				StrSplite(strList,fileName.GetBuffer(0));
				checkStr(data,zidianList,strList); 
				strList.clear();
			}
			else  if(MAINTYPE_MUSIC == data.maintype )
			{
#if OPEN_FFMEPG
				AudioPlay(fileFinder.GetFilePath().GetBuffer(0),&data.filetime) ;
#endif
				NULL;
			}
			if(data.maintype >=0)
			{
				SQLDB.File_Add(data);
			}
		}
	}

	fileFinder.Close();
}

//遍历所有文件
void FindAllFile_NEW(long long hdd_nid,CString hdd_area,
				 list<struct FILETYPE_ST> typeList,list<struct ZIDIAN_ST> zidianList)
{

	FILETIME cTime;
	FILETIME lTime;
	CString fileName;
	CString fileExt;

    ULARGE_INTEGER  uli; 

	struct FILE_ST data;

	list<struct FILE_ST> Filedata;
	list<STR_SPLITE_S> strList;

	CFileFind fileFinder;
	CString filePath = hdd_area + _T("//*.*");

	BOOL bFinished = fileFinder.FindFile(filePath);

	Filedata.clear();
	while(bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();

		if(fileFinder.IsDots())
			continue;
		else if(fileFinder.IsDirectory())  //若是目录则递归调用此方法
		{
			FindAllFile(hdd_nid,fileFinder.GetFilePath(),typeList,zidianList);
		}
		else  //再判断是否为txt文件
		{
			//获取文件类型
			fileName = fileFinder.GetFileName();
			fileExt=GetFileExt(fileName);

			memset(&data,0,sizeof(struct FILE_ST ));
			//有效后缀
			data.maintype=checkExt(typeList,fileExt.GetBuffer(0));

			if(data.maintype >=0)
			{
				if(SQLDB.File_CheckDoublePos(fileName.GetBuffer(0),
					GetFilePathNoName(fileFinder.GetFilePath()).GetBuffer(0),
					hdd_nid))
					continue;

				strcpy(data.name,fileName.GetBuffer(0)	);
				strcpy(data.path,GetFilePathNoName(fileFinder.GetFilePath()).GetBuffer(0));

				data.hdd_nid=hdd_nid;
				strcpy(data.type,fileExt.GetBuffer(0));
				data.filesize=fileFinder.GetLength();

				if(fileFinder.GetCreationTime(&cTime))
				{
					uli.LowPart = cTime.dwLowDateTime;  
					uli.HighPart = cTime.dwHighDateTime;  
					data.CreationTime=uli.QuadPart;
				}
				else
					data.CreationTime=0;

				if(fileFinder.GetLastWriteTime(&lTime))
				{
					uli.LowPart = lTime.dwLowDateTime;  
					uli.HighPart = lTime.dwHighDateTime;  
					data.LastWriteTime=uli.QuadPart;
				}
				else 
					data.LastWriteTime=0;

			}
			if(MAINTYPE_OTHER == data.maintype )
			{
				strList.clear();
				StrSplite(strList,fileName.GetBuffer(0));
				checkStr(data,zidianList,strList); 
				strList.clear();
			}
			else if( MAINTYPE_SUB == data.maintype)
			{
				NULL;
			}
			else  if(MAINTYPE_VIDEO == data.maintype )
			{
#if OPEN_FFMEPG
				VideoPlay(fileFinder.GetFilePath().GetBuffer(0),
					&data.filetime,&data.resolutionW,&data.resolutionH);
#endif
				strList.clear();
				StrSplite(strList,fileName.GetBuffer(0));
				checkStr(data,zidianList,strList); 
				strList.clear();
			}
			else  if(MAINTYPE_MUSIC == data.maintype )
			{
#if OPEN_FFMEPG
				AudioPlay(fileFinder.GetFilePath().GetBuffer(0),&data.filetime) ;
#endif
				NULL;
			}
			if(data.maintype >=0)
			{
				Filedata.push_back(data);
			}
		}
	}

	if(Filedata.size()>0)
	{	
		SQLDB.Begin();
		list<struct FILE_ST>::iterator beglist;

		for(beglist=Filedata.begin();beglist!=Filedata.end();beglist++)
		{
			SQLDB.File_Add(*beglist);
		}	
		SQLDB.Commit();

	}

	Filedata.clear();

	fileFinder.Close();
}


void CheckAllDoubleFile(long long hdd_nid)
{
	long long doublenid;
	CString strid;
	char cstrid[64];

	list<struct FILE_VIEW_LITE_ST> fileviewList;
	list<struct FILE_VIEW_LITE_ST>::iterator beglist;
	list<struct FILE_VIEW_LITE_ST> fileviewDoubleList;
	list<struct FILE_VIEW_LITE_ST>::iterator Doublebeglist;
	fileviewList.clear();
	fileviewDoubleList.clear();

	if(SQLDB.File_ReadHddAllwithVideoAudio(fileviewList,hdd_nid))
	{
		for(beglist=fileviewList.begin();beglist!=fileviewList.end();beglist++)
		{
			fileviewDoubleList.clear();
			SQLDB.File_CheckDouble(beglist->file_maintype,beglist->filesize,
				beglist->file_name,beglist->file_md5,
				fileviewDoubleList);

			//查看重复 必须多于1条
			if(fileviewDoubleList.size()>1)
			{
				//发现重复的
				doublenid=0;
				//看看之前有没ID。 复用他
				for(Doublebeglist=fileviewDoubleList.begin();Doublebeglist!=fileviewDoubleList.end();Doublebeglist++)
				{
					if(Doublebeglist->double_nid>0)
					{
						doublenid=Doublebeglist->double_nid;
						break;
					}
				}
				//新增ID
				if(doublenid <=0 )
				{
					SQLDB.Double_Add(beglist->file_nid);
					//获取一个DOUBLEID 
					SQLDB.Double_ReadOne(&doublenid,beglist->file_nid);
				}
				//写到各个文件里
				if(doublenid >0)	
				{
					strid="";
					for(Doublebeglist=fileviewDoubleList.begin();Doublebeglist!=fileviewDoubleList.end();Doublebeglist++)
					{
						if(Doublebeglist!=fileviewDoubleList.begin())
							sprintf(cstrid,",%I64u",Doublebeglist->file_nid);
						else
							sprintf(cstrid,"%I64u",Doublebeglist->file_nid);
						strid+=cstrid;
					}

					SQLDB.File_SetDouble(doublenid,strid.GetBuffer(0));
				}
			}
		
		}
	}
	fileviewList.clear();
	fileviewDoubleList.clear();

}
void CheckAllIsFile(long long hdd_nid)
{
	FILE *fp=NULL;
	char str[512];

	list<struct FILE_VIEW_LITE_ST> fileviewList;
	list<struct FILE_VIEW_LITE_ST>::iterator beglist;
	
	fileviewList.clear();
	//查看文件是否存在
	if(SQLDB.File_ReadHddAllandOld(fileviewList,hdd_nid))
	{
			
		for(beglist=fileviewList.begin();beglist!=fileviewList.end();beglist++)
		{
			sprintf(str,"%s%s%s",beglist->hdd_area,beglist->file_path,beglist->file_name);
			fp=fopen(str,"rb");
			if(fp)
			{
				//文件存在
				fclose(fp);
				fp=NULL;
			}
			else
			{
				//文件不存在
				SQLDB.File_Delete(beglist->file_nid);
			}
		}
	}
	//重置全部文件为0
	SQLDB.File_SetNoFlagOne(hdd_nid);
	fileviewList.clear();

}

//生成ED2K连接。
void GetEd2kUrl(char* path,char *name,char *dst)
{
	unsigned char digest[64]="";
	char outputed2k[256]="";
	char outputaich[256]="";

	strcpy(dst,"");
	long long sizemax=1024*1024;
	sizemax*=1024*4;

	long long size=filesize(path);
	if(size >= sizemax)
	{
		//大于4G 退出
		return ;
	}
	rhash_library_init(); 

	rhash_file(RHASH_ED2K,path, digest);
	rhash_print_bytes(outputed2k, digest, rhash_get_digest_size(RHASH_ED2K),(RHPR_HEX));


	rhash_file(RHASH_AICH,path, digest);
	rhash_print_bytes(outputaich, digest, rhash_get_digest_size(RHASH_AICH),(RHPR_BASE32 ));

	sprintf(dst,"ed2k://|file|%s|%I64u|%s|h=%s",name,size,outputed2k,outputaich);

//	rhash_file(RHASH_TTH,filepath, digest);
//	rhash_print_bytes(output, digest, rhash_get_digest_size(RHASH_TTH),(RHPR_BASE32 ));

//	rhash_file(RHASH_SHA1,filepath, digest);
//	rhash_print_bytes(output, digest, rhash_get_digest_size(RHASH_SHA1 ),RHPR_HEX);

}