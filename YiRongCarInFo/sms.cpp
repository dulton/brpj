#include "stdafx.h"
#include "sms.h"

void SMS::smsStart(void)
{
	//  定义函数指针pointer function name(pfn) 
	pfnSP_Startup = NULL; 
	pfnSP_Cleanup = NULL; 
	pfnSP_Login = NULL;
	pfnSP_Logout = NULL;

	pfnSubmitShortMessage = NULL;
	pfnSubmitShortMessageMulti = NULL;
	//  定义动态连接库内核对象句柄 
	hSMDLL = NULL; 

	FILE *fp;
	fp=fopen("smsclassload.log.txt","w");

	//  显示加载动态库到本进程空间中 
	hSMDLL = LoadLibrary("SMEntry.dll"); 
	//  加载失败处理 
	if(NULL == hSMDLL) 
		fprintf(fp,"%s \n","加载动态连接库失败!! ");

	//根据动态库输出函数名称获得函数地址，并赋值 
	pfnSP_Startup = (SP_Startup)GetProcAddress(hSMDLL, "SP_Startup");
	//函数地址获取失败处理 
	if(NULL == pfnSP_Startup)
		fprintf(fp,"%s \n","获取函数SP_Startup地址失败!! ");

	//根据动态库输出函数名称获得函数地址，并赋值 
	pfnSP_Cleanup = (SP_Cleanup)GetProcAddress(hSMDLL,"SP_Cleanup"); 
	//  函数地址获取失败处理 
	if(NULL == pfnSP_Cleanup) 
		fprintf(fp,"%s \n","获取函数SP_Cleanup地址失败!! "); 

	//根据动态库输出函数名称获得函数地址，并赋值
	pfnSP_Login = (SP_Login)GetProcAddress(hSMDLL, "SP_Login");
	// 函数地址获取失败处理
	if(NULL == pfnSP_Login)
		fprintf(fp,"%s \n","获取函数SP_Login地址失败!! ");

	//根据动态库输出函数名称获得函数地址，并赋值
	pfnSP_Logout = (SP_Logout)GetProcAddress(hSMDLL, "SP_Logout");
	// 函数地址获取失败处理
	if(NULL == pfnSP_Logout)
		fprintf(fp,"%s \n","获取函数SP_Logout地址失败!! ");

	//hSMDLL 为动态链接库内核对象句柄
	pfnSubmitShortMessage=(SubmitShortMessage)GetProcAddress(hSMDLL, "SubmitShortMessage") ;  
	if(NULL == pfnSubmitShortMessage)
		fprintf(fp,"获取函数SubmitShortMessage地址失败!! \n");

	 //hSMDLL 为动态链接库内核对象句柄
	pfnSubmitShortMessageMulti=(SubmitShortMessageMulti)GetProcAddress(hSMDLL, "SubmitShortMessageMulti");
	if(NULL == pfnSubmitShortMessageMulti)
		fprintf(fp,"获取函数SubmitShortMessageMulti地址失败!! \n");
	

	fprintf(fp,"ok");

	fclose(fp);
}

void SMS::smsEnd(void)
{
	//  程序结束时释放加载的动态连接库 
	if ( NULL != hSMDLL ) 
		FreeLibrary(hSMDLL); 
}
///////////////////////////////////////////////////////////////////////

int SMS::smsInit(void)
{
	//  使用函数指针调用动态库中的代码 
	return  pfnSP_Startup("10.131.1.24,1393","CustomSMS","SqlMsde@InfoxEie2000"); 
}

void SMS::smsUninit(void)
{
	//  使用函数指针调用动态库中的代码 
	pfnSP_Cleanup(); 
}

int SMS::smsLogin(void)
{
	// 使用函数指针调用动态库中的代码
	return pfnSP_Login("123","123456");
}

int SMS::smsLogout(void)
{
	// 使用函数指针调用动态库中的代码
	return pfnSP_Logout ();
}
//电话号码  信息 信息大小
int SMS::smsSendOne(char *dstPhoneNumber,char *msg,unsigned long msglen)
{
	if(0 == msglen)
		return -2;
	return pfnSubmitShortMessage("2013-01-23 10:20:20", "10657301110", dstPhoneNumber, msg,msglen,1,"EIE","01","1");
}

//电话号码路径  信息 信息大小
int SMS::smsSendMulti(char *dstPhonePath,char *msg,unsigned long msglen)
{
	if(0 == msglen)
		return -2;
	return pfnSubmitShortMessageMulti("2013-01-23 10:20:20","10657301110",dstPhonePath,msg,msglen,1,"EIE","01","1");
}

//电话号码路径  信息 信息大小
int SMS::smsSendMultiMy(char *dstPhonePath,char *msg,unsigned long msglen)
{
	if(0 == msglen)
		return -2;

	char temp[32];

	FILE *fp=fopen(dstPhonePath,"r");
	if(fp)
	{
		do
		{
			memset(temp,0,32);
			fscanf(fp,"%s",temp);
			//有值
			if(strlen(temp))
				pfnSubmitShortMessage("2013-01-23 10:20:20", "10657301110", temp, msg,msglen,1,"EIE","01","1");

		}while(!feof(fp));
		fclose(fp);
		return 0;
	}
	else
		return -1;
}
