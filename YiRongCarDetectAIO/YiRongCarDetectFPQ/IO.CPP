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


//判断设备是否正常并修改  .不改其他。由服务端修复任务
bool IO::DETECTSERVER_HeartTest(long maxtime)
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
		strSql.Format(_T("update TB_DETECT_SERVER t set isenable=0 where (sysdate-lasttime)*24*3600>%d"),maxtime);

		_variant_t RecordAffected;
		m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);

		//两值相减小于MAXTIME 正常
		strSql.Format(_T("update TB_DETECT_SERVER t set isenable=1 where (sysdate-lasttime)*24*3600<=%d"),maxtime);

		m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DETECTSERVER_HeartTest error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;
}

//判断设备是否正常并修改
bool IO::DETECTSERVER_ADD(char *ip,long channel)
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

		HRESULT hr = m_pRecordsetPtr->Open("select * from TB_DETECT_SERVER", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		if(SUCCEEDED(hr))
		{
			m_pRecordsetPtr->AddNew();
			m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));
			m_pRecordsetPtr->PutCollect("sip", _variant_t(ip));
			m_pRecordsetPtr->PutCollect("channelnum", _variant_t(channel));

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
		in_errormessage.Format("IO::DETECTSERVER_ADD error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	IOwriteLock=false;
}

//分配服务器后修改任务
bool IO::MISSION_Edit(long long Missionid,long DETECTSERVERid,int flag)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		//分配
		strSql.Format(_T("update TB_mission set detectid=%d,flag=%d where nid=%I64u"),DETECTSERVERid,Missionid,flag);

		_variant_t RecordAffected;
		m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::MISSION_Edit error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;
}


//预先占用识别服务器
bool IO::DETECT_EditCamera(long DETECTSERVERid,long camid)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		//分配
		strSql.Format(_T("update TB_DETECT_SERVER set ncamera=%d where nid=%d"),camid,DETECTSERVERid);

		_variant_t RecordAffected;
		m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DETECT_EditCamera error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;
}

//读取任务
bool IO::MISSION_Read(long long *Missionid,unsigned long int *camid,unsigned long int *isplay,unsigned long int *userid,char* indate)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		strSql.Format(_T("select * from(select row_number() over(order by nid) r,\
						 nid,userid,isplay,ncamera,to_char(indate) from tb_mission where flag=0 ) where r = 1"));

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
			(*Missionid) =VarSaveNumber2(m_pRecordsetPtr,"nid");
			(*isplay) =  VarSaveNumber2(m_pRecordsetPtr,"isplay");
			(*camid) =  VarSaveNumber2(m_pRecordsetPtr,"ncamera");
			(*userid) =  VarSaveNumber2(m_pRecordsetPtr,"userid");
			VarSaveStr2(indate,m_pRecordsetPtr,"to_char(indate)");
		}

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::MISSION_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;
}

//在表中找摄像头 进行停止
bool IO::DEVICE_SearchForStop(long *DETECTSERVERID,long camid)
{


	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		strSql.Format(_T("select NID from TB_DETECT_SERVER where ncamera=%d"),camid);

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

		(*DETECTSERVERID)=0;
		if(!m_pRecordsetPtr->adoEOF)
		{
			(*DETECTSERVERID) =VarSaveNumber2(m_pRecordsetPtr,"NID");
		}

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_SearchForStop error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;


}


//在表中找空余设备 进行开始
bool IO::DEVICE_SearchForStart(long *DETECTSERVERID)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql;
		strSql.Format(_T("select nid from TB_DETECT_SERVER t where isenable=1 and isoccupy=0 and ncamera=0"));

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

		(*DETECTSERVERID)=0;
		if(!m_pRecordsetPtr->adoEOF)
		{
			(*DETECTSERVERID) =VarSaveNumber2(m_pRecordsetPtr,"nid");
		}

		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_SearchForStop error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;
}

//发送强制停止任务 已分配
bool IO::MISSION_AddStop(long DETECTSERVERid,unsigned long int camid)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;

	try
	{
		long long nid =  AutoAddNid("mission","nid");

		m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

		HRESULT hr = m_pRecordsetPtr->Open("select * from tb_mission", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		if(SUCCEEDED(hr))
		{
			m_pRecordsetPtr->AddNew();
			m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));
			m_pRecordsetPtr->PutCollect("userid", _variant_t(999999));
			m_pRecordsetPtr->PutCollect("ncamera", _variant_t(camid));

			m_pRecordsetPtr->PutCollect("DETECTID", _variant_t(DETECTSERVERid));
			m_pRecordsetPtr->PutCollect("isplay", _variant_t(0));
			m_pRecordsetPtr->PutCollect("flag", _variant_t(1));

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
		in_errormessage.Format("IO::MISSION_AddStop error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	IOwriteLock=false;
}
/*
create or replace view devicelist_view as
select a."NID",a."SIP",a."ISOCCUPY",a."CHANNELNUM",a."ISENABLE",a."MISSIONID",b.userid,b.isplay,to_char(b.indate) cindate,c.ncamera,c.scameraname,c.sipserver camip,c.rtspurl,c.isplay camisplay
from TB_DETECT_SERVER a
left join tb_mission b
on a.missionid = b.nid
left join tb_device c
on a.nid =c.DETECTID order by nid desc;

*/
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
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_ReadList error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;

}