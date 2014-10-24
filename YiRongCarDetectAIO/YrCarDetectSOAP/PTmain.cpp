// webserver.cpp : Defines the entry point for the console application.
//
//放最前
#include "afx.h"

#include "YrElecarDetect.nsmap"
//#pragma comment(lib,"wsock32.lib")

#include "pthreadinclude/pthread.h"
#pragma comment(lib, "pthreadlib/pthreadVC2.lib")




////////////////////////
#include "init.h"
#include "io.h"
#include "mylog.h"
IO OracleIO;
struct GLOBAL_CONFIG_S GlobalConfig;

////////////////////////////////

//宏与全局变量的定义

#define BACKLOG (100)
//线程数量
#define MAX_THR (10)
//最大请求
#define MAX_QUEUE (1000)


pthread_mutex_t queue_cs;//队列锁
pthread_cond_t queue_cv;//条件变量

SOAP_SOCKET queue[MAX_QUEUE];//数组队列
int head =0, tail =0;//队列头队列尾初始化

void * process_queue(void *);//线程入口函数
int enqueue(SOAP_SOCKET);//入队列函数
SOAP_SOCKET dequeue(void);//出队列函数


//线程入口函数
void * process_queue(void * soap)
{
	struct soap * tsoap = (struct soap *)soap;
	for(;;)
	{
		tsoap->socket = dequeue();

		if (!soap_valid_socket(tsoap->socket))
		{
			break;
		}

		soap_serve(tsoap);
		soap_destroy(tsoap);
		soap_end(tsoap);
	}
	return NULL;
}

//入队列操作
int enqueue(SOAP_SOCKET sock)
{
	int status = SOAP_OK;
	int next;

	pthread_mutex_lock(&queue_cs);
	next = tail +1;

	if (next >= MAX_QUEUE)
	{
		next = 0;
	}

	if (next == head)
	{
		status = SOAP_EOM;
	}
	else
	{
		queue[tail] =sock;
		tail = next;
	}

	pthread_cond_signal(&queue_cv);
	pthread_mutex_unlock(&queue_cs);

	return status;
}


//出队列操作
SOAP_SOCKET dequeue()
{

	SOAP_SOCKET sock;
	pthread_mutex_lock(&queue_cs);

	while (head == tail )
	{
		pthread_cond_wait(&queue_cv,&queue_cs);
	}

	sock = queue[head++];

	if (head >= MAX_QUEUE)
	{
		head =0;
	}

	pthread_mutex_unlock(&queue_cs);

	return sock;
}

//加此段 不然不显示add.wsdl
int http_get(struct soap *soap)
{
	FILE*fd = NULL;
	fd = fopen("YrElecarDetect.wsdl", "rb"); //open WSDL file to copy
	if (!fd)
	{
		return 404; //return HTTP not found error
	}
	soap->http_content = "text/xml"; //HTTP header with text /xml content
	soap_response(soap,SOAP_FILE);
	for(;;)
	{
		size_t r = fread(soap->tmpbuf,1, sizeof(soap->tmpbuf), fd);
		if (!r)
		{
			break;
		}
		if (soap_send_raw(soap, soap->tmpbuf, r))
		{
			break; //cannot send, but little we can do about that
		}
	}
	fclose(fd);
	soap_end_send(soap);
	return SOAP_OK;
} 

//被强退处理
void fnExit(void)
{
	OracleIO.DisConnectionOracleDB();
	Log_fileout(LOG_ERROR,"程序被退出\n");
}

//主函数
int main(int argc,char ** argv)
{

	//========

	//初始化程序路径
	TCHAR CurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentDir);

	if(false== ReadConfigFile("yredSOAPconfig.db",GlobalConfig))
	{
		Log_fileout(LOG_ERROR,"配置文件不存在\n");
		return 0;
	}
	//连接数据库要的
	//AfxOleInit();
	CoInitialize(NULL);

	if(OracleIO.ConnectionOracleDBTXT("YRDataBaseConfig.db")<0)
		return 0;

	//初始化目录
	initLogDir(CurrentDir);
	//强退信息
	atexit(fnExit);

	//========

	struct soap ServerSoap;

	//初始话运行时环境
	soap_init(&ServerSoap);

	//加此行。不然不显示WSDL
	ServerSoap.fget = http_get;
	//设置UTF8传输 否则中文乱码
	soap_set_mode(&ServerSoap, SOAP_C_UTFSTRING);

	//如果没有参数，当作CGI程序处理
	/*
	if (argc <2)
	{
	fprintf(stderr, "没有传端口\n");
	//CGI 风格服务请求，单线程
	soap_serve(&ServerSoap);

	//清除序列化的类的实例
	soap_destroy(&ServerSoap);

	//清除序列化的数据
	soap_end(&ServerSoap);
	}
	else
	*/
	{

		struct soap * soap_thr[MAX_THR];

		pthread_t tid[MAX_THR];

		int i;

		//int port = atoi(argv[1]);
		int port =GlobalConfig.Port;

		Log_fileout(LOG_ERROR,"端口:%d\n",port);

		SOAP_SOCKET m,s;

		//锁和条件变量初始化
		pthread_mutex_init(&queue_cs,NULL);
		pthread_cond_init(&queue_cv,NULL);

		//绑定服务端口
		m = soap_bind(&ServerSoap,NULL,port,BACKLOG);

		//循环直至服务套接字合法
		while (!soap_valid_socket(m))
		{
			Log_fileout(LOG_ERROR,"Bind port error!\n");
			m = soap_bind(&ServerSoap,NULL,port,BACKLOG);
		}

		Log_fileout(LOG_ERROR,"socket connection successful %d \n",m);


		//生成服务线程
		for(i = 0; i <MAX_THR; i++)
		{
			soap_thr[i] = soap_copy(&ServerSoap);

			Log_fileout(LOG_ERROR,"Starting thread %d \n",i);

			pthread_create(&tid[i],NULL,(void*(*)(void*))process_queue,(void*)soap_thr[i]);
		}

		for(;;)
		{
			//接受客户端的连接
			s = soap_accept(&ServerSoap);

			if (!soap_valid_socket(s))
			{
				if (ServerSoap.errnum)
				{
					soap_print_fault(&ServerSoap,stderr);
					continue;
				}
				else
				{
					Log_fileout(LOG_ERROR,"Server timed out \n");
					break;
				}
			}

			//客户端的IP地址

			Log_fileout(LOG_ERROR,"Accepted connection from IP= %d.%d.%d.%d socket = %d \n",

				((ServerSoap.ip)>>24)&&0xFF,((ServerSoap.ip)>>16)&0xFF,((ServerSoap.ip)>>8)&0xFF,(ServerSoap.ip)&0xFF,(ServerSoap.socket));

			//请求的套接字进入队列，如果队列已满则循环等待
			while(enqueue(s) == SOAP_EOM)
			{
				Sleep(1000);
			}

		}

		//服务结束后的清理工作
		for(i = 0; i < MAX_THR; i++)
		{
			while (enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM)
			{
				Sleep(1000);
			}
		}

		for(i=0; i< MAX_THR; i++)
		{
			Log_fileout(LOG_ERROR,"Waiting for thread %d to terminate ..\n",i);
			pthread_join(tid[i],NULL);

			Log_fileout(LOG_ERROR,"terminated \n");

			soap_done(soap_thr[i]);
			free(soap_thr[i]);
		}

		pthread_mutex_destroy(&queue_cs);
		pthread_cond_destroy(&queue_cv);

	}

	//分离运行时的环境
	soap_done(&ServerSoap);


	Log_fileout(LOG_ERROR,"退出程序\n");	//窗口内容

	return 0;
}

