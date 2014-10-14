/***********************************************************************************
//ver :YiRongCarDetectAIO
//time:2013-03-29 11:39:00
***********************************************************************************/
#include "stdafx.h"
#include "IO.h"

IO::IO(void)
{
	state=false;

	IOwriteLock=false;

}

IO::~IO(void)
{
	DisConnectionOracleDB();

}

//断开与oracle数据库的连接
bool IO::DisConnectionOracleDB(void)
{
	try
	{
		if(m_pConnection->State)
		{
			m_pConnection->Close();
			state=false;
			return true;
		}
	}
	catch(_com_error e)        //捕捉异常
	{
		CString temp;
		temp.Format(_T("错误信息:%s"),e.ErrorMessage());
		//MessageBox(temp, _T("数据库断开连接失败信息提示"));
		return false;
	}	

	return false;
}

#define RCT_MAX_STR_SIZE 256
//解密
void IO::DeCode(char *src,char *dst)
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

//读数据库配置文件
bool IO::ReadFile(TCHAR* FileName)
{
	TCHAR temp[RCT_MAX_STR_SIZE]="";
	TCHAR tempchar[RCT_MAX_STR_SIZE]="";

	FILE *fp=_tfopen(FileName,_T("r"));
	if(fp)
	{
		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Ip:%s"),Ip);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("Port:%s"),Port);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
#if 1
		_stscanf(temp,_T("User:%s"),tempchar);
		DeCode(tempchar,User);
#else
		_stscanf(temp,_T("User:%s"),User);
#endif

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);

#if 1
		//解密
		_stscanf(temp,_T("Psw:%s"),tempchar);
		DeCode(tempchar,Psw);
#else
		_stscanf(temp,_T("Psw:%s"),password);
#endif
		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("DataBaseName:%s"),DataBaseName);

		fclose(fp);

		return TRUE;
	}
	else
		return FALSE;
}

void IO::VarSaveStr(TCHAR *dst,variant_t src)
{
	variant_t temp; 
	if(src.vt != VT_NULL && src !=temp)
	{
		_tcscpy(dst, (_bstr_t)src.bstrVal);
	}
	else
		_tcscpy(dst,"");
}

//STRING
void IO::VarSaveStr2(TCHAR *dst,_RecordsetPtr &p,TCHAR *name)
{
	variant_t temp; 
	variant_t src = p->GetCollect(name);
	if(src.vt != VT_NULL && src !=temp)
	{
		_tcscpy(dst, (_bstr_t)src.bstrVal);
	}
	else
		_tcscpy(dst,"");
}
//STRING
void IO::VarSaveString2(CString &dst,_RecordsetPtr &p,TCHAR *name)
{
	variant_t temp; 
	variant_t src = p->GetCollect(name);
	if(src.vt != VT_NULL && src !=temp)
		dst=(char*)(_bstr_t)src.bstrVal;
	else
		dst="";
}


long long IO::VarSaveNumber2(_RecordsetPtr &p,TCHAR *name)
{
	variant_t temp; 
	variant_t src = p->GetCollect(name);
	long long a;
	if(src.vt != VT_NULL && src !=temp)
	{
		a=(long long)src.lVal;
		a *= (src.decVal.sign == 128)? -1 : 1;

		return a;
	}
	else
		return 0;
}

double IO::VarSaveDouble2(_RecordsetPtr &p,TCHAR *name)
{
	variant_t temp; 
	variant_t src = p->GetCollect(name);
	if(src.vt != VT_NULL && src !=temp)
	{
		return (double)src.dblVal;
	}
	else
		return 0.0;
}
//去除插入数据库时出现单引号
void IO::filterstring(char *str)
{
	while(*str)   
	{  
		if( (*str) == '\'' )
		{
			(*str) = ' ';
		}
		str++;  
	} 
}

//连接数据库
int IO::ConnectionOracleDBTXT(char*  FileName)
{
	//ReadConfigTxt pConfig;
	if(!ReadFile(FileName))
	{
		return ReadFile_FAIL;
	}

	CString CstrConn;

	CstrConn.Format(_T("Provider=OraOLEDB.Oracle.1;User ID=%s;Password=%s;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));Persist Security Info=False"),\
		User, Psw, Ip, Port, DataBaseName);

	HRESULT hr;
	if(SUCCEEDED(m_pConnection.CreateInstance("ADODB.Connection")))
	{
		_bstr_t strConnect = _bstr_t(CstrConn);

		m_pConnection->ConnectionTimeout = 30;

		try
		{
			hr = m_pConnection->Open(strConnect,"","",adModeUnknown);
		}
		catch(_com_error e)
		{
			CString temp;			
			temp.Format(_T("Error:%s"),e.ErrorMessage());  
			AfxMessageBox(temp);  
			return ContOpen_FAIL;
		}

		state=true;
		return Connectd_DONE;
	}
	else
	{
		//AfxMessageBox("Create ADODB Connection Instance Failed.");
		return Instance_FAIL;
	}
}

//注意此表不加锁
//插入表时获取自增的NID
unsigned long long IO::AutoAddNid(TCHAR *TableName,TCHAR *NidName)
{
	try
	{
		unsigned long long id=0;

		TCHAR	 strsql[2048];
		_stprintf(strsql, _T("select seq_%s.nextval as %s from dual"),TableName,NidName);	    //获取nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql,NULL,adCmdText);	

		id = VarSaveNumber2(m_pRecordsetPtr,NidName);

		m_pRecordsetPtr->Close(); 

		return id;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::AutoAddNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return 0;
	}  
}


//判断设备是否正常并修改
bool IO::DETECTDEVICE_HeartTest(long maxtime)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		//两值相减大于MAXTIME 故障
		strSql.Format(_T("update TB_DETECT_DEVICE t set isenable=0 where (sysdate-lasttime)*24*3600>%d"),maxtime);

		_variant_t RecordAffected;
		m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);
	
		//两值相减小于MAXTIME 正常
		strSql.Format(_T("update TB_DETECT_DEVICE t set isenable=1 where (sysdate-lasttime)*24*3600<=%d"),maxtime);

		m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DETECTDEVICE_HeartTest error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;
}

//判断设备是否正常并修改
bool IO::DETECTDEVICE_ADD(char *ip,long channal)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;

	try
	{
		long long nid =  AutoAddNid("detect_device","nid");

		m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

		HRESULT hr = m_pRecordsetPtr->Open("select * from tb_detect_device", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		if(SUCCEEDED(hr))
		{
			m_pRecordsetPtr->AddNew();
			m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));
			m_pRecordsetPtr->PutCollect("sip", _variant_t(ip));
			m_pRecordsetPtr->PutCollect("channalnum", _variant_t(channal));

			m_pRecordsetPtr->Update();
			m_pRecordsetPtr->Close();

			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DETECTDEVICE_ADD error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	IOwriteLock=false;
}
/*
//分配服务器后修改任务
bool IO::MISSION_Edit(char *ip,long channal)
{

}

//读取任务
bool IO::MISSION_Read(char *ip,long channal)
{

}

//发送强制停止任务
bool IO::MISSION_AddStop(char *ip,long channal)
{

}
//联合读取设备树和服务器信息
bool IO::DEVICE_ReadList(char *ip,long channal)
{

}

*/