/***********************************************************************************
//ver :YiRongCarDetectAIO
//time:2013-03-29 11:39:00
***********************************************************************************/

#include "IO.h"
#include "mylog.h"

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
		Log_fileout(LOG_ERROR,_T("数据库断开连接失败信息提示:%s"),e.ErrorMessage());
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
		Log_fileout(LOG_ERROR,"读数据库配置文件失败\n");	//窗口内容
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
			Log_fileout(LOG_ERROR,"数据库无法连接:账号密码IP异常 或文件夹带()  %s\n",e.ErrorMessage());
			return ContOpen_FAIL;
		}

		state=true;
		return Connectd_DONE;
	}
	else
	{
		Log_fileout(LOG_ERROR,"数据库ADO初始化失败\n");
		return Instance_FAIL;
	}
}

//注意此表不加锁
//插入表时获取自增的NID
unsigned long long IO::AutoAddNid(TCHAR *TableName)
{
	try
	{
		unsigned long long id=0;

		TCHAR	 strsql[2048];
		_stprintf(strsql, _T("select seq_%s.nextval as nid from dual"),TableName);	    //获取nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql,NULL,adCmdText);	

		id = VarSaveNumber2(m_pRecordsetPtr,"nid");

		m_pRecordsetPtr->Close(); 

		return id;
	}
	catch(_com_error e)
	{
		Log_fileout(LOG_ERROR,"IO::AutoAddNid error：%s", e.ErrorMessage());

		return 0;
	}  
}

//////////////////////////////////////////////////////////////////////////////////
//插入任务
bool IO::Mission_ADD(unsigned long int detectid,unsigned long int camid,unsigned long int userid,unsigned long int isplay)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;

	try
	{  
		long long id =  AutoAddNid("mission");

		m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

		HRESULT hr = m_pRecordsetPtr->Open("select * from tb_mission", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		if(SUCCEEDED(hr))
		{
			m_pRecordsetPtr->AddNew();
			m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
			m_pRecordsetPtr->PutCollect("DETECTID", _variant_t(detectid));
			m_pRecordsetPtr->PutCollect("ncamera", _variant_t(camid));
			m_pRecordsetPtr->PutCollect("userid", _variant_t(userid));
			m_pRecordsetPtr->PutCollect("isplay", _variant_t((long)isplay));
			m_pRecordsetPtr->PutCollect("FLAG", _variant_t(2));
			m_pRecordsetPtr->PutCollect("MESSAGE", _variant_t("第三方SOAP发送"));

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
		
		Log_fileout(LOG_ERROR,"IO::Mission_ADD error：%s", e.ErrorMessage());

		IOwriteLock=false;
		return false;
	}  
}

bool IO::DEVICE_AddNewArea(char* AreaName)
{	
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{

		long long nid =  AutoAddNid("zd_org");

		m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));
		HRESULT hr = m_pRecordsetPtr->Open("select sorgname, nid from t_zd_org t", \
			m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		if(SUCCEEDED(hr))
		{
			m_pRecordsetPtr->AddNew();
			m_pRecordsetPtr->PutCollect("sorgname", _variant_t(AreaName));
			m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));
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
		
		Log_fileout(LOG_ERROR,"IO::DEVICE_AddNewArea error：%s", e.ErrorMessage());
		
		IOwriteLock=false;
		return false;
	}  
}


bool IO::DEVICE_AddNewCamera(long long areaID,const char* CamName,const char* sip,int port,int channel,
							 const char* user,const char* password,long venderID,const char* Rtspurl,int RTP,int DecodeTag,
							 const char* longitude,const char* latitude,	
							 const char* crossIndex,long crossID,
							 long userID,long userLV,
							 long long *out_camid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		long long ncamera =  AutoAddNid("ncamera");

		m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));
		HRESULT hr = m_pRecordsetPtr->Open("select * from tb_device", \
			m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		if(SUCCEEDED(hr))
		{
			m_pRecordsetPtr->AddNew();
			m_pRecordsetPtr->PutCollect("scameraname", _variant_t(CamName));
			m_pRecordsetPtr->PutCollect("sipserver", _variant_t(sip));
			m_pRecordsetPtr->PutCollect("sport", _variant_t((long)port));
			m_pRecordsetPtr->PutCollect("channel", _variant_t((long)channel));
			m_pRecordsetPtr->PutCollect("suser", _variant_t(user));
			m_pRecordsetPtr->PutCollect("spwd", _variant_t(password));
			m_pRecordsetPtr->PutCollect("nid", _variant_t(areaID));
			m_pRecordsetPtr->PutCollect("ncamera", _variant_t(ncamera));
			m_pRecordsetPtr->PutCollect("svenderid", _variant_t(venderID));

			m_pRecordsetPtr->PutCollect("rtspurl", _variant_t(Rtspurl));
			m_pRecordsetPtr->PutCollect("rtpmode", _variant_t((long)RTP));
			m_pRecordsetPtr->PutCollect("decodetag", _variant_t((long)DecodeTag));

			m_pRecordsetPtr->PutCollect("longitude", _variant_t(longitude));
			m_pRecordsetPtr->PutCollect("latitude", _variant_t(latitude));

			m_pRecordsetPtr->PutCollect("crossindex", _variant_t(crossIndex));
			m_pRecordsetPtr->PutCollect("crossid", _variant_t((long)crossID));
			m_pRecordsetPtr->PutCollect("userid", _variant_t((long)userID));
			m_pRecordsetPtr->PutCollect("userlv", _variant_t((long)userLV));

			m_pRecordsetPtr->Update();
			m_pRecordsetPtr->Close(); 

			(*out_camid)=ncamera;
			IOwriteLock=false;
			return true;   		
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		
		Log_fileout(LOG_ERROR,"IO::DEVICE_AddNewCamera error：%s", e.ErrorMessage());
	
		IOwriteLock=false;
		return false;
	}  
}


//测试摄像头是否存在
bool IO::DEVICE_IfCamera(unsigned long int camid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		strSql.Format(_T("select ncamera from TB_DEVICE t where ncamera =%d"),camid);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{	
			IOwriteLock=false;
			return false;
		}
		else
		{
			m_pRecordsetPtr->MoveFirst();
		}
		if(!m_pRecordsetPtr->adoEOF)
		{
			IOwriteLock=false;
			return true;
		}

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		Log_fileout(LOG_ERROR,"IO::IfCamera error：%s", e.ErrorMessage());
		
		IOwriteLock=false;

		return false;
	}  
}
//测试识别服务器是否存在
bool IO::DEVICE_IfDetectServer(unsigned long int detectid)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		strSql.Format(_T("select nid from TB_DETECT_SERVER where nid=%d"),detectid);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{	
			IOwriteLock=false;
			return false;
		}
		else
		{
			m_pRecordsetPtr->MoveFirst();
		}
		if(!m_pRecordsetPtr->adoEOF)
		{
			IOwriteLock=false;
			return true;
		}

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		Log_fileout(LOG_ERROR,"IO::DEVICE_IfDetectServer error：%s", e.ErrorMessage());

		IOwriteLock=false;

		return false;
	}  
}

//删除摄像头
bool IO::DEVICE_DeleteCamera(unsigned long int camid)
{	
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString strSql;
		strSql.Format("delete from tb_device where ncamera=%d",camid);
		m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	

		IOwriteLock=false;
		return true;
	}
	catch(_com_error e)
	{
		
		Log_fileout(LOG_ERROR,"IO::DEVICE_DeleteCamera error：%s", e.ErrorMessage());
		
		IOwriteLock=false;
		return false;
	}  
}

//视图查询
//联合读取设备树和服务器信息
bool IO::DEVICE_ReadList(list<DEVICE_LIST> &DeviceList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		strSql.Format(_T("select * from DEVICELIST_VIEW"));

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
			return false;
		}
		else
		{
			m_pRecordsetPtr->MoveFirst();
		}

		DEVICE_LIST tempdata;

		while(!m_pRecordsetPtr->adoEOF)
		{
			memset(&tempdata,0,sizeof(DEVICE_LIST));

			tempdata.detect.id =VarSaveNumber2(m_pRecordsetPtr,"nid");
			VarSaveStr2(tempdata.detect.ip ,m_pRecordsetPtr,"sip");
			tempdata.detect.channel =VarSaveNumber2(m_pRecordsetPtr,"channelnum");
			tempdata.detect.isoccupy =VarSaveNumber2(m_pRecordsetPtr,"isoccupy");
			tempdata.detect.isenable =VarSaveNumber2(m_pRecordsetPtr,"isenable");

			tempdata.mission.missionid=VarSaveNumber2(m_pRecordsetPtr,"missionid");
			tempdata.mission.userid=VarSaveNumber2(m_pRecordsetPtr,"userid");
			VarSaveStr2(tempdata.mission.username,m_pRecordsetPtr,"suser");
			tempdata.mission.isplay=VarSaveNumber2(m_pRecordsetPtr,"isplay");
			VarSaveStr2(tempdata.mission.time ,m_pRecordsetPtr,"cindate");

			tempdata.camera.camid=VarSaveNumber2(m_pRecordsetPtr,"ncamera");
			VarSaveStr2(tempdata.camera.name,m_pRecordsetPtr,"scameraname");
			VarSaveStr2(tempdata.camera.ip,m_pRecordsetPtr,"camip");
			VarSaveStr2(tempdata.camera.rtspurl,m_pRecordsetPtr,"rtspurl");
			tempdata.camera.isplay=VarSaveNumber2(m_pRecordsetPtr,"camisplay");

			DeviceList.push_back(tempdata);
			m_pRecordsetPtr->MoveNext();
		}

		m_pRecordsetPtr->Close();
		IOwriteLock=false;
		return true;
	}
	catch(_com_error e)
	{
		Log_fileout(LOG_ERROR,"IO::DEVICE_ReadList error：%s", e.ErrorMessage());
		
		IOwriteLock=false;
		return false;
	}  

}