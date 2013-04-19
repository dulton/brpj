/***********************************************************************************
//ver :YiRongCarDetectAIO
//time:2013-03-29 11:39:00
***********************************************************************************/
#include "stdafx.h"
#include "IO.h"

IO::IO(void)
{
	state=false;

#if YRVM_PINGTAI_MODE
	YRVM_state=false;
#endif

}

IO::~IO(void)
{
   DisConnectionOracleDB();

#if YRVM_PINGTAI_MODE
   YRVM_DisConnectionOracleDB();
#endif
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
		_stscanf(temp,_T("User:%s"),User);

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
			//CString temp;			
			//temp.Format(_T("Error:%s"),e.ErrorMessage());  
			//AfxMessageBox(temp);  
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

bool IO::DEVICE_ReadAllOrgName(vector<struct DEVICE_AREA_ST>& DeviceAreaList)
{
	CString	 strsql = _T("select sorgname,nid from T_ZD_ORG");
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		struct DEVICE_AREA_ST DeviceArea = {0};

		var = m_pRecordsetPtr->GetCollect("sorgname");
		if(var.vt != NULL && var !=temp)
		{
			CString str = var.bstrVal;
			strcpy(DeviceArea.name, str.GetBuffer(0));
		}
		DeviceArea.nid = (int)(long)m_pRecordsetPtr->GetCollect("nid");
		DeviceAreaList.push_back(DeviceArea);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(DeviceAreaList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::DEVICE_AddNewArea(CString AreaName)
{
	CString strSql = _T("select seq_zd_org.nextval as nid from dual");			//获取机构nid
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	
	variant_t nid = m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));
	HRESULT hr = m_pRecordsetPtr->Open("select sorgname, nid from T_ZD_ORG t", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("sorgname", _variant_t(AreaName));
		m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));
		m_pRecordsetPtr->Update();
		m_pRecordsetPtr->Close(); 
		return true;   		
	}
	return false;
}

bool IO::DEVICE_DeleteArea(int areaID)
{
	CString strSql;
	strSql.Format("delete from T_ZD_ORG where nid=%d",areaID);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}


bool IO::DEVICE_AddNewCamera(CString AreaName,CString CamName,CString sip,int port,CString user,CString password)
{
	CString strSql;
	strSql.Format("select nid from T_ZD_ORG where sorgname='%s'",AreaName);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	variant_t nid = m_pRecordsetPtr->GetCollect("nid");
	
	strSql = _T("select seq_ncamera.nextval as ncamera from dual");	    //获取ncamera
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	
	long ncamera	= m_pRecordsetPtr->GetCollect("ncamera");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));
	HRESULT hr = m_pRecordsetPtr->Open("select * from tb_device", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("scameraname", _variant_t(CamName));
		m_pRecordsetPtr->PutCollect("sipserver", _variant_t(sip));
		m_pRecordsetPtr->PutCollect("sport", _variant_t((long)port));
		m_pRecordsetPtr->PutCollect("suser", _variant_t(user));
		m_pRecordsetPtr->PutCollect("spwd", _variant_t(password));
		m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));
		m_pRecordsetPtr->PutCollect("ncamera", _variant_t(ncamera));

		m_pRecordsetPtr->Update();
		m_pRecordsetPtr->Close(); 
		return true;   		
	}
	return false;
}

bool IO::DEVICE_ReadCameraInfo(char* AreaName,vector<IPLIST>& CameraList)
{
	CString strSql;
	strSql.Format("select nid from T_ZD_ORG where sorgname='%s'",AreaName);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	variant_t nid = m_pRecordsetPtr->GetCollect("nid");
	long a = nid;
	strSql.Format("select * from tb_device where nid=%d",a);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t temp; 
		temp.ChangeType(VT_NULL);

		variant_t CamName = m_pRecordsetPtr->GetCollect("scameraname");
		variant_t sip = m_pRecordsetPtr->GetCollect("sipserver");
		variant_t port = m_pRecordsetPtr->GetCollect("sport");
		variant_t user = m_pRecordsetPtr->GetCollect("suser");
		variant_t password = m_pRecordsetPtr->GetCollect("spwd");
		variant_t ncamera = m_pRecordsetPtr->GetCollect("ncamera");

		IPLIST CameraInfo;
		CameraInfo.area = AreaName;
		CameraInfo.camID = ncamera;
		if(sip.vt != NULL && sip !=temp)
		{
			CameraInfo.ip = sip.bstrVal;
		}
		if(CamName.vt != NULL && CamName !=temp)
		{
			CameraInfo.name = CamName.bstrVal;
		}
		CameraInfo.port = port;
		if(password.vt != NULL && password !=temp)
		{
			CameraInfo.psw = password.bstrVal;
		}
		if(user.vt != NULL && user !=temp)
		{
			CameraInfo.user = user.bstrVal;
		}
		//strcpy(CameraInfo.area, AreaName);
		CameraList.push_back(CameraInfo);

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(CameraList.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::DEVICE_DeleteCamera(int cameraID)
{
	CString strSql;
	strSql.Format("delete from tb_device where ncamera=%d",cameraID);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}

bool IO::DEVICE_DeleteCameraWithAreaID(int AreaID)
{
	CString strSql;
	strSql.Format("delete from tb_device where nid=%d",AreaID);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}

bool IO::DEVICE_UpdateCameraInfo(long CamID,CString AreaName,CString CamName,CString sip,int port,CString user,CString password)
{
	CString strSql;
	strSql.Format("select nid from T_ZD_ORG where sorgname='%s'",AreaName);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	variant_t a = m_pRecordsetPtr->GetCollect("nid");
	long nid = a;
	strSql.Format(_T("update tb_device set scameraname='%s',\
					sipserver='%s',sport=%d,suser='%s',spwd='%s',\
					nid=%d where ncamera=%d"), CamName,sip,port,user,password,nid,CamID);

	_variant_t RecordAffected;
	m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);	  
	if(RecordAffected.uintVal == 1)	   //判断是否更新成功
	{
		return	true;
	}
	else
	{
		return false;
	}
}

bool IO::USER_AddNewUser(struct SET_USER_DATA_ST UserInfo)
{
	CString strSql;
	strSql.Format(_T("select * from tb_user where suser='%s'"),UserInfo.user);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(!(m_pRecordsetPtr->BOF))
	{
		return false;
	}

	strSql = _T("select seq_user.nextval as nid from dual");			//获取机构nid
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	
	variant_t nid = m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));
	HRESULT hr = m_pRecordsetPtr->Open("select * from tb_user", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("suser", _variant_t(UserInfo.user));
		m_pRecordsetPtr->PutCollect("spwd", _variant_t(UserInfo.psw));
		m_pRecordsetPtr->PutCollect("ndeviceallot", _variant_t((long)(UserInfo.device)));		//设备管理权限
		m_pRecordsetPtr->PutCollect("nphoto", _variant_t((long)(UserInfo.capbmp)));			//抓拍权限
		m_pRecordsetPtr->PutCollect("nvideo", _variant_t((long)(UserInfo.record)));			//本地录像
		m_pRecordsetPtr->PutCollect("nview", _variant_t((long)(UserInfo.preview)));			//预览
		m_pRecordsetPtr->PutCollect("nalarm", _variant_t((long)(UserInfo.detect)));			//识别
		m_pRecordsetPtr->PutCollect("nyuntai", _variant_t((long)(UserInfo.ptz)));				//云台控制
		m_pRecordsetPtr->PutCollect("nparam", _variant_t((long)(UserInfo.systemset)));		//系统设置
		m_pRecordsetPtr->PutCollect("nvideoset", _variant_t((long)(UserInfo.recordset)));		//录像设置
		m_pRecordsetPtr->PutCollect("ncarnumber", _variant_t((long)(UserInfo.detectset)));	//车牌识别设置
		m_pRecordsetPtr->PutCollect("nblack", _variant_t((long)(UserInfo.blackset)));			//黑名单设置
		m_pRecordsetPtr->PutCollect("nvideoquery", _variant_t((long)(UserInfo.historyvideo)));		//历史视频查询
		m_pRecordsetPtr->PutCollect("nalarmquery", _variant_t((long)(UserInfo.historyalarm)));		//历史报警查询
		m_pRecordsetPtr->PutCollect("ndistinguishquery", _variant_t((long)(UserInfo.historydetect)));	//历史识别查询
		m_pRecordsetPtr->PutCollect("nlogquery", _variant_t((long)(UserInfo.historyreport)));			//系统日志查询
		m_pRecordsetPtr->PutCollect("nmanager", _variant_t((long)(UserInfo.admin)));					//系统管理员
		m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));								//nid

		m_pRecordsetPtr->Update();
		m_pRecordsetPtr->Close(); 
		return true;   		
	}
	return false;
}

bool IO::USER_DeletetUser(unsigned long int nid)
{
	CString strSql;
	strSql.Format("delete from tb_user where nid=%d",nid);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}

bool IO::USER_UpdateUserInfo(struct SET_USER_DATA_ST UserInfo)
{
	CString strSql;
	strSql.Format(_T("update tb_user set suser='%s',spwd='%s',ndeviceallot=%d,\
										nphoto=%d,nvideo=%d,nview=%d,\
										nalarm=%d,nyuntai=%d,nparam=%d,\
										nvideoset=%d,ncarnumber=%d,nblack=%d,\
										nvideoquery=%d,nalarmquery=%d,ndistinguishquery=%d,\
										nlogquery=%d,nmanager=%d where nid=%d"),\
										UserInfo.user, UserInfo.psw, UserInfo.device,\
										UserInfo.capbmp, UserInfo.record, UserInfo.preview,\
										UserInfo.detect, UserInfo.ptz, UserInfo.systemset,\
										UserInfo.recordset, UserInfo.detectset, UserInfo.blackset,\
										UserInfo.historyvideo, UserInfo.historyalarm, UserInfo.historydetect,\
										UserInfo.historyreport, UserInfo.admin, UserInfo.nid);
	_variant_t RecordAffected;
	m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);	  
	if(RecordAffected.uintVal == 1)	   //判断是否更新成功
	{
		return	true;
	}
	else
	{
		return false;
	}
}

bool IO::USER_ReadUserInfoWithName(char *user,struct SET_USER_DATA_ST *UserInfo)
{
	CString	strsql;
	strsql.Format(_T("select * from tb_user where suser='%s'"),user);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	variant_t var;
	variant_t temp; 
	CString str;
	temp.ChangeType(VT_NULL);

	var = m_pRecordsetPtr->GetCollect("suser");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(UserInfo->user, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("spwd");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(UserInfo->psw, str.GetBuffer(0));
	}
	UserInfo->device = (int)(long)(m_pRecordsetPtr->GetCollect("ndeviceallot"));
	UserInfo->capbmp = (int)(long)(m_pRecordsetPtr->GetCollect("nphoto"));
	UserInfo->record = (int)(long)(m_pRecordsetPtr->GetCollect("nvideo"));
	UserInfo->preview = (int)(long)(m_pRecordsetPtr->GetCollect("nview"));
	UserInfo->detect = (int)(long)(m_pRecordsetPtr->GetCollect("nalarm"));
	UserInfo->ptz = (int)(long)(m_pRecordsetPtr->GetCollect("nyuntai"));
	UserInfo->systemset = (int)(long)(m_pRecordsetPtr->GetCollect("nparam"));
	UserInfo->recordset = (int)(long)(m_pRecordsetPtr->GetCollect("nvideoset"));
	UserInfo->detectset = (int)(long)(m_pRecordsetPtr->GetCollect("ncarnumber"));
	UserInfo->blackset = (int)(long)(m_pRecordsetPtr->GetCollect("nblack"));
	UserInfo->historyvideo = (int)(long)(m_pRecordsetPtr->GetCollect("nvideoquery"));
	UserInfo->historyalarm = (int)(long)(m_pRecordsetPtr->GetCollect("nalarmquery"));
	UserInfo->historydetect = (int)(long)(m_pRecordsetPtr->GetCollect("ndistinguishquery"));
	UserInfo->historyreport = (int)(long)(m_pRecordsetPtr->GetCollect("nlogquery"));
	UserInfo->admin = (int)(long)(m_pRecordsetPtr->GetCollect("nmanager"));
	UserInfo->nid = (int)(long)(m_pRecordsetPtr->GetCollect("nid"));

	m_pRecordsetPtr->Close();

	return true;
}

bool IO::USER_ReadUserInfoWithNid(unsigned long int nid,struct SET_USER_DATA_ST *UserInfo)
{
	CString	strsql;
	strsql.Format(_T("select * from tb_user where nid=%d"),nid);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	variant_t var;
	variant_t temp; 
	CString str;
	temp.ChangeType(VT_NULL);

	var = m_pRecordsetPtr->GetCollect("suser");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(UserInfo->user, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("spwd");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(UserInfo->psw, str.GetBuffer(0));
	}
	UserInfo->device = (int)(long)(m_pRecordsetPtr->GetCollect("ndeviceallot"));
	UserInfo->capbmp = (int)(long)(m_pRecordsetPtr->GetCollect("nphoto"));
	UserInfo->record = (int)(long)(m_pRecordsetPtr->GetCollect("nvideo"));
	UserInfo->preview = (int)(long)(m_pRecordsetPtr->GetCollect("nview"));
	UserInfo->detect = (int)(long)(m_pRecordsetPtr->GetCollect("nalarm"));
	UserInfo->ptz = (int)(long)(m_pRecordsetPtr->GetCollect("nyuntai"));
	UserInfo->systemset = (int)(long)(m_pRecordsetPtr->GetCollect("nparam"));
	UserInfo->recordset = (int)(long)(m_pRecordsetPtr->GetCollect("nvideoset"));
	UserInfo->detectset = (int)(long)(m_pRecordsetPtr->GetCollect("ncarnumber"));
	UserInfo->blackset = (int)(long)(m_pRecordsetPtr->GetCollect("nblack"));
	UserInfo->historyvideo = (int)(long)(m_pRecordsetPtr->GetCollect("nvideoquery"));
	UserInfo->historyalarm = (int)(long)(m_pRecordsetPtr->GetCollect("nalarmquery"));
	UserInfo->historydetect = (int)(long)(m_pRecordsetPtr->GetCollect("ndistinguishquery"));
	UserInfo->historyreport = (int)(long)(m_pRecordsetPtr->GetCollect("nlogquery"));
	UserInfo->admin = (int)(long)(m_pRecordsetPtr->GetCollect("nmanager"));
	UserInfo->nid = (int)(long)(m_pRecordsetPtr->GetCollect("nid"));

	m_pRecordsetPtr->Close();

	return true;
}

bool IO::USER_ReadAllUserInfo(list<struct SET_USER_LIST_ST> &UserInfoList)
{
	CString	strsql = _T("select suser,nid from tb_user");
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	SET_USER_LIST_ST UserInfo;
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);

		var = m_pRecordsetPtr->GetCollect("suser");
		if(var.vt != NULL && var !=temp)
		{
			CString str = var.bstrVal;
			strcpy(UserInfo.user, str.GetBuffer(0));
		}
		UserInfo.nid = (int)(long)(m_pRecordsetPtr->GetCollect("nid"));

		UserInfoList.push_back(UserInfo);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(UserInfoList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::LOG_AddNewSystemLog(char *userName,char *logStr)
{
	CString strSql = _T("select seq_log.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	long id	= m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select * from tb_log", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
		m_pRecordsetPtr->PutCollect("susername", _variant_t(userName));
		m_pRecordsetPtr->PutCollect("smark", _variant_t(logStr));

		m_pRecordsetPtr->Update();
		m_pRecordsetPtr->Close(); 
		return true;
	}
	return false;
}

unsigned long IO::LOG_GetSystemLogNum(char *userName,char *startTime,char *endTime,int flag,char *SQLstr)
{
	CString	strSql="";

	CString tempSql="";

	strSql= _T("select count(1) from tb_log");

	if((flag&0x01) != 0)
	{	
		if((flag&0x02) != 0)
		{
			tempSql.Format(_T(" where susername like '%%%s%%' and dcreatetime between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),userName,startTime,endTime);
		}
		else
		{
			tempSql.Format(_T(" where susername like '%%%s%%'"),userName);
		}
	}
	else
	{
		if((flag&0x02) != 0)
		{
			tempSql.Format(_T(" where dcreatetime between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
		}
	}

	strSql = strSql + tempSql;

	strcpy(SQLstr,tempSql.GetBuffer(0));

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

	return num;
}

bool IO::LOG_ReadSystemLog(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_REPORT_ST> &HistoryList)
{
	CString	strSql = _T("select * from (select to_char(dcreatetime),smark,susername,rownum rn from tb_log");

	CString tempSql = SQLstr;

	CString numSql;

	if(0 != flag)
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}
	else
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}

	strSql = strSql + tempSql + numSql;

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		struct HISTORY_REPORT_ST report = {0};
		
		var = m_pRecordsetPtr->GetCollect("smark");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(report.str, str.GetBuffer(0));
		}
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreatetime)");
		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &report.year,&report.mon,&report.day,
			   &report.hour,&report.min,&report.sec);
		var = m_pRecordsetPtr->GetCollect("susername");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(report.user, str.GetBuffer(0));
		}

		HistoryList.push_back(report);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(HistoryList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool IO::CAR_MatchResult_AddNew(bool *isblack,unsigned long CAMID,char *platetype,char	*platecolor,char *direction,char *carcolor,
								char *plate,int reliability,char *path,unsigned long int picsize,unsigned char *picdata)
{
	CString strSql = _T("select seq_match_result_car.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	long id = (long)m_pRecordsetPtr->GetCollect("nid");

	strSql.Format(_T("select sipserver,scameraname from tb_device where ncamera=%d"),CAMID);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	_variant_t CamIp = m_pRecordsetPtr->GetCollect("sipserver");
	_variant_t CamName = m_pRecordsetPtr->GetCollect("scameraname");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select * from tb_match_result_car", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
		m_pRecordsetPtr->PutCollect("ncamera", _variant_t((long)CAMID));
		m_pRecordsetPtr->PutCollect("scameraname", _variant_t(CamName));
		m_pRecordsetPtr->PutCollect("sip", _variant_t(CamIp));
		m_pRecordsetPtr->PutCollect("stype", _variant_t(platetype));
		m_pRecordsetPtr->PutCollect("scolour", _variant_t(platecolor));
		m_pRecordsetPtr->PutCollect("sdirection", _variant_t(direction));
		m_pRecordsetPtr->PutCollect("scolor", _variant_t(carcolor));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(plate));
		m_pRecordsetPtr->PutCollect("ndegree", _variant_t((long)reliability));
		m_pRecordsetPtr->PutCollect("sfile", _variant_t(path));
		m_pRecordsetPtr->PutCollect("nsize", _variant_t((long)picsize));

		char *m_pbuff = NULL;

		SAFEARRAYBOUND rgs[1];	
		rgs[0].lLbound = 0;	   
		rgs[0].cElements =	picsize;		

		SAFEARRAY *psa;	   
		psa = SafeArrayCreate(VT_UI1,1,rgs); 
		SafeArrayAccessData(psa,(void **)&m_pbuff);		
		
		memcpy(m_pbuff, picdata, picsize); 

		variant_t varBOLB;
		varBOLB.vt = VT_ARRAY | VT_UI1;
		varBOLB.parray = psa;

		m_pRecordsetPtr->GetFields()->GetItem("bpicture")->AppendChunk(varBOLB);	  //picture

		m_pRecordsetPtr->Update();

		SafeArrayUnaccessData(psa);

		m_pRecordsetPtr->Close(); 

		CAR_AlarmResult_Execute(id,isblack);

		return true;
	}
	return false;
}

bool IO::CAR_MatchResult_GetPicture(unsigned long NID,unsigned char *data)
{
	CString strsql;

	strsql.Format(_T("select bpicture from tb_match_result_car where bpicture is not null and nid=5d"),NID);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	long lDatasize =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->ActualSize;	  //从数据库获取图片

	_variant_t varBLOB =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->GetChunk(lDatasize);

	if(varBLOB.vt == (VT_ARRAY | VT_UI1))
	{
		unsigned char * pbuffer = NULL;

		SafeArrayAccessData(varBLOB.parray,(void **)&pbuffer);
            
		memcpy(data, pbuffer, lDatasize);

		SafeArrayUnaccessData(varBLOB.parray);

		return true;
	}
	return false;
}

bool IO::CAR_AlarmResult_GetPicture(unsigned long NID,unsigned char *data)
{
	CString strsql;

	strsql.Format(_T("select bpicture from tb_alarm_car where bpicture is not null and nid=5d"),NID);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	long lDatasize =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->ActualSize;	  //从数据库获取图片

	_variant_t varBLOB =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->GetChunk(lDatasize);

	if(varBLOB.vt == (VT_ARRAY | VT_UI1))
	{
		unsigned char * pbuffer = NULL;

		SafeArrayAccessData(varBLOB.parray,(void **)&pbuffer);
            
		memcpy(data, pbuffer, lDatasize);

		SafeArrayUnaccessData(varBLOB.parray);

		return true;
	}
	return false;
}

unsigned long IO::CAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
					char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr)
{
	CString	strSql="";
	CString outSQL="";
	CString tempSql="";
	bool hadwhere=false;		//检查语句中是否已经有where
	strSql= _T("select count(1) from tb_match_result_car");

	if((flag&0x01) != 0)
	{	
		tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
		hadwhere = true;
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x02) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sip like '%%%s%%'"),sip);
		}
		else
		{
			tempSql.Format(_T(" where sip like '%%%s%%'"),sip);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x04) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scarnumber like '%%%s%%'"),plate);
		}
		else
		{
			tempSql.Format(_T(" where scarnumber like '%%%s%%'"),plate);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
		}
		else
		{
			tempSql.Format(_T(" where dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x10) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sdirection='%s'"),direction);
		}
		else
		{
			tempSql.Format(_T(" where sdirection='%s'"),direction);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x20) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scolour='%s'"),platecolor);
		}
		else
		{
			tempSql.Format(_T(" where scolour='%s'"),platecolor);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x40) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and stype='%s'"),platetype);
		}
		else
		{
			tempSql.Format(_T(" where stype='%s'"),platetype);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x80) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scolor='%s'"),carcolor);
		}
		else
		{
			tempSql.Format(_T(" where scolor='%s'"),carcolor);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}

	strcpy(SQLstr,outSQL.GetBuffer(0));

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

	return num;
}

unsigned long IO::CAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
					char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr)
{
	CString	strSql="";
	CString outSQL="";
	CString tempSql="";
	bool hadwhere=false;		//检查语句中是否已经有where
	strSql= _T("select count(1) from tb_alarm_car");

	if((flag&0x01) != 0)
	{	
		tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
		hadwhere = true;
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x02) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sip like '%%%s%%'"),sip);
		}
		else
		{
			tempSql.Format(_T(" where sip like '%%%s%%'"),sip);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x04) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scarnumber like '%%%s%%'"),plate);
		}
		else
		{
			tempSql.Format(_T(" where scarnumber like '%%%s%%'"),plate);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
		}
		else
		{
			tempSql.Format(_T(" where dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x10) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sdirection='%s'"),direction);
		}
		else
		{
			tempSql.Format(_T(" where sdirection='%s'"),direction);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x20) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scolour='%s'"),platecolor);
		}
		else
		{
			tempSql.Format(_T(" where scolour='%s'"),platecolor);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x40) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and stype='%s'"),platetype);
		}
		else
		{
			tempSql.Format(_T(" where stype='%s'"),platetype);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x80) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scolor='%s'"),carcolor);
		}
		else
		{
			tempSql.Format(_T(" where scolor='%s'"),carcolor);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}

	strcpy(SQLstr,outSQL.GetBuffer(0));

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

	return num;
}

bool IO::CAR_MatchResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{
	CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,rownum rn from tb_match_result_car");

	CString tempSql = SQLstr;

	CString numSql;

	if(0 != flag)
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}
	else
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}

	strSql = strSql + tempSql + numSql;

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		struct HISTORY_DETECT_ST result = {0};

		result.nid = (long)m_pRecordsetPtr->GetCollect("nid");
		var = m_pRecordsetPtr->GetCollect("scameraname");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.name, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sip");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.ip, str.GetBuffer(0));
		}
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");
		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &result.year,&result.mon,&result.day,
			   &result.hour,&result.min,&result.sec);
		var = m_pRecordsetPtr->GetCollect("stype");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.platetype, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scolour");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.platecolor, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sdirection");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.direction, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scolor");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.carcolor, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scarnumber");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.plate, str.GetBuffer(0));
		}
		result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
		var = m_pRecordsetPtr->GetCollect("sfile");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.path, str.GetBuffer(0));
		}
		result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");

		HistoryList.push_back(result);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(HistoryList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool IO::CAR_AlarmResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{
	CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,rownum rn from tb_alarm_car");

	CString tempSql = SQLstr;

	CString numSql;

	if(0 != flag)
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}
	else
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}

	strSql = strSql + tempSql + numSql;

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		struct HISTORY_DETECT_ST result = {0};

		result.nid = (long)m_pRecordsetPtr->GetCollect("nid");
		var = m_pRecordsetPtr->GetCollect("scameraname");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.name, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sip");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.ip, str.GetBuffer(0));
		}
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");
		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &result.year,&result.mon,&result.day,
			   &result.hour,&result.min,&result.sec);
		var = m_pRecordsetPtr->GetCollect("stype");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.platetype, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scolour");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.platecolor, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sdirection");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.direction, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scolor");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.carcolor, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scarnumber");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.plate, str.GetBuffer(0));
		}
		result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
		var = m_pRecordsetPtr->GetCollect("sfile");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.path, str.GetBuffer(0));
		}
		result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");

		HistoryList.push_back(result);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(HistoryList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool IO::CAR_AlarmResult_Execute(int nid,bool *isblack)
{
	bool iresult=false;
	char serror[256];
	bool black=false;

	_CommandPtr pCommandPtr = NULL;
	pCommandPtr.CreateInstance(_uuidof(Command));

	_ParameterPtr pParameterPtr;
	pParameterPtr = pCommandPtr->CreateParameter(_T("i_nid"), adInteger, adParamInput, sizeof(int), _variant_t((long)nid));
	pCommandPtr->Parameters->Append(pParameterPtr);	

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_cnt"), adInteger, adParamOutput, sizeof(bool), _variant_t(black));	
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_result"), adInteger, adParamOutput, sizeof(bool), _variant_t(iresult));	
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_errmessage"), adVarChar, adParamOutput, 256, _variant_t(serror));
	pCommandPtr->Parameters->Append(pParameterPtr);
	
	pCommandPtr->CommandText = "pro_auto_match_car";
	pCommandPtr->CommandType = adCmdStoredProc;	 

	pCommandPtr->ActiveConnection =	m_pConnection;

	pCommandPtr->Execute(NULL, NULL, adCmdStoredProc);	

	iresult = pCommandPtr->Parameters->GetItem("o_result")->GetValue();
	black = pCommandPtr->Parameters->GetItem("o_cnt")->GetValue();
	(*isblack)=black;

	return iresult;
}

bool IO::ELECAR_MatchResult_AddNew(bool *isblack,unsigned long CAMID,char *direction,char *plate,int reliability,
									char *path,unsigned long int picsize,unsigned char *picdata)
{
	CString strSql = _T("select seq_match_result_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	long id = (long)m_pRecordsetPtr->GetCollect("nid");

	strSql.Format(_T("select sipserver,scameraname from tb_device where ncamera=%d"),CAMID);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	_variant_t CamIp = m_pRecordsetPtr->GetCollect("sipserver");
	_variant_t CamName = m_pRecordsetPtr->GetCollect("scameraname");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select * from tb_match_result_vehicle", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
		m_pRecordsetPtr->PutCollect("ncamera", _variant_t((long)CAMID));
		m_pRecordsetPtr->PutCollect("scameraname", _variant_t(CamName));
		m_pRecordsetPtr->PutCollect("sip", _variant_t(CamIp));
		m_pRecordsetPtr->PutCollect("sdirection", _variant_t(direction));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(plate));
		m_pRecordsetPtr->PutCollect("ndegree", _variant_t((long)reliability));
		m_pRecordsetPtr->PutCollect("sfile", _variant_t(path));
		m_pRecordsetPtr->PutCollect("nsize", _variant_t((long)picsize));

		char *m_pbuff = NULL;

		SAFEARRAYBOUND rgs[1];	
		rgs[0].lLbound = 0;	   
		rgs[0].cElements =	picsize;		

		SAFEARRAY *psa;	   
		psa = SafeArrayCreate(VT_UI1,1,rgs); 
		SafeArrayAccessData(psa,(void **)&m_pbuff);		
		
		memcpy(m_pbuff, picdata, picsize); 

		variant_t varBOLB;
		varBOLB.vt = VT_ARRAY | VT_UI1;
		varBOLB.parray = psa;

		m_pRecordsetPtr->GetFields()->GetItem("bpicture")->AppendChunk(varBOLB);	  //picture

		m_pRecordsetPtr->Update();

		SafeArrayUnaccessData(psa);

		m_pRecordsetPtr->Close();
		
		ELECAR_AlarmResult_Execute(id,isblack);

		return true;
	}
	return false;
}

bool IO::ELECAR_MatchResult_GetPicture(unsigned long NID,unsigned char *data)
{
	CString strsql;

	strsql.Format(_T("select bpicture from tb_match_result_vehicle where bpicture is not null and nid=5d"),NID);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	long lDatasize =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->ActualSize;	  //从数据库获取图片

	_variant_t varBLOB =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->GetChunk(lDatasize);

	if(varBLOB.vt == (VT_ARRAY | VT_UI1))
	{
		unsigned char * pbuffer = NULL;

		SafeArrayAccessData(varBLOB.parray,(void **)&pbuffer);
            
		memcpy(data, pbuffer, lDatasize);

		SafeArrayUnaccessData(varBLOB.parray);

		return true;
	}
	return false;
}

bool IO::ELECAR_AlarmResult_GetPicture(unsigned long NID,unsigned char *data)
{
	CString strsql;

	strsql.Format(_T("select bpicture from tb_alarm_vehicle where bpicture is not null and nid=5d"),NID);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	long lDatasize =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->ActualSize;	  //从数据库获取图片

	_variant_t varBLOB =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->GetChunk(lDatasize);

	if(varBLOB.vt == (VT_ARRAY | VT_UI1))
	{
		unsigned char * pbuffer = NULL;

		SafeArrayAccessData(varBLOB.parray,(void **)&pbuffer);
            
		memcpy(data, pbuffer, lDatasize);

		SafeArrayUnaccessData(varBLOB.parray);

		return true;
	}
	return false;
}

unsigned long IO::ELECAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
											char *direction,int flag,char *SQLstr)
{
	CString	strSql="";
	CString outSQL="";
	CString tempSql="";
	bool hadwhere=false;		//检查语句中是否已经有where
	strSql= _T("select count(1) from tb_match_result_vehicle");

	if((flag&0x01) != 0)
	{	
		tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
		hadwhere = true;
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x02) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sip like '%%%s%%'"),sip);
		}
		else
		{
			tempSql.Format(_T(" where sip like '%%%s%%'"),sip);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x04) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scarnumber like '%%%s%%'"),plate);
		}
		else
		{
			tempSql.Format(_T(" where scarnumber like '%%%s%%'"),plate);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
		}
		else
		{
			tempSql.Format(_T(" where dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x10) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sdirection='%s'"),direction);
		}
		else
		{
			tempSql.Format(_T(" where sdirection='%s'"),direction);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}

	strcpy(SQLstr,outSQL.GetBuffer(0));

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

	return num;
}

unsigned long IO::ELECAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
											char *direction,int flag,char *SQLstr)
{
	CString	strSql="";
	CString outSQL="";
	CString tempSql="";
	bool hadwhere=false;		//检查语句中是否已经有where
	strSql= _T("select count(1) from tb_alarm_vehicle");

	if((flag&0x01) != 0)
	{	
		tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
		hadwhere = true;
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x02) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sip like '%%%s%%'"),sip);
		}
		else
		{
			tempSql.Format(_T(" where sip like '%%%s%%'"),sip);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x04) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and scarnumber like '%%%s%%'"),plate);
		}
		else
		{
			tempSql.Format(_T(" where scarnumber like '%%%s%%'"),plate);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x08) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
		}
		else
		{
			tempSql.Format(_T(" where dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),startTime,endTime);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}
	if((flag&0x10) != 0)
	{
		if(hadwhere)
		{
			tempSql.Format(_T(" and sdirection='%s'"),direction);
		}
		else
		{
			tempSql.Format(_T(" where sdirection='%s'"),direction);
			hadwhere = true;
		}
		outSQL = outSQL + tempSql;
		strSql = strSql + tempSql;
	}

	strcpy(SQLstr,outSQL.GetBuffer(0));

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

	return num;
}

bool IO::ELECAR_MatchResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{
	CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,rownum rn from tb_match_result_vehicle");

	CString tempSql = SQLstr;

	CString numSql;

	if(0 != flag)
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}
	else
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}

	strSql = strSql + tempSql + numSql;

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		struct HISTORY_DETECT_ST result = {0};

		result.nid = (long)m_pRecordsetPtr->GetCollect("nid");
		var = m_pRecordsetPtr->GetCollect("scameraname");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.name, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sip");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.ip, str.GetBuffer(0));
		}
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");
		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &result.year,&result.mon,&result.day,
			   &result.hour,&result.min,&result.sec);
		var = m_pRecordsetPtr->GetCollect("sdirection");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.direction, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scarnumber");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.plate, str.GetBuffer(0));
		}
		result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
		var = m_pRecordsetPtr->GetCollect("sfile");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.path, str.GetBuffer(0));
		}
		result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");

		HistoryList.push_back(result);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(HistoryList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool IO::ELECAR_AlarmResult_Read(char *SQLstr,int flag,int startNum,int endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{
	CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,rownum rn from tb_alarm_vehicle");

	CString tempSql = SQLstr;

	CString numSql;

	if(0 != flag)
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}
	else
	{
		numSql.Format(_T(") where rn >= %d and rn <= %d"),startNum,endNum);
	}

	strSql = strSql + tempSql + numSql;

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		struct HISTORY_DETECT_ST result = {0};

		result.nid = (long)m_pRecordsetPtr->GetCollect("nid");
		var = m_pRecordsetPtr->GetCollect("scameraname");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.name, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sip");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.ip, str.GetBuffer(0));
		}
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");
		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &result.year,&result.mon,&result.day,
			   &result.hour,&result.min,&result.sec);
		var = m_pRecordsetPtr->GetCollect("sdirection");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.direction, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scarnumber");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.plate, str.GetBuffer(0));
		}
		result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
		var = m_pRecordsetPtr->GetCollect("sfile");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(result.path, str.GetBuffer(0));
		}
		result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");

		HistoryList.push_back(result);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(HistoryList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool IO::ELECAR_AlarmResult_Execute(int nid,bool *isblack)
{
	bool iresult=false;
	char serror[256];
	bool black=false;

	_CommandPtr pCommandPtr = NULL;
	pCommandPtr.CreateInstance(_uuidof(Command));

	_ParameterPtr pParameterPtr;
	pParameterPtr = pCommandPtr->CreateParameter(_T("i_nid"), adInteger, adParamInput, sizeof(nid), _variant_t((long)nid));
	pCommandPtr->Parameters->Append(pParameterPtr);	

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_cnt"), adInteger, adParamOutput, sizeof(bool), _variant_t(black));	
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_result"), adInteger, adParamOutput, sizeof(bool), _variant_t(iresult));	
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_errmessage"), adVarChar, adParamOutput, 256, _variant_t(serror));
	pCommandPtr->Parameters->Append(pParameterPtr);
	
	pCommandPtr->CommandText = "pro_auto_match_vehicle";
	pCommandPtr->CommandType = adCmdStoredProc;	 

	pCommandPtr->ActiveConnection =	m_pConnection;

	pCommandPtr->Execute(NULL, NULL, adCmdStoredProc);	

	iresult = pCommandPtr->Parameters->GetItem("o_result")->GetValue();
	black = pCommandPtr->Parameters->GetItem("o_cnt")->GetValue();
	(*isblack)=black;

	return iresult;
}

bool IO::CAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo)
{
	CString strSql = _T("select seq_black_car.nextval as nid from dual");

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	long id	= m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select * from tb_car_black", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(BlackInfo.plate));
		m_pRecordsetPtr->PutCollect("sbrand", _variant_t(BlackInfo.brand));
		m_pRecordsetPtr->PutCollect("slostname", _variant_t(BlackInfo.name));
		m_pRecordsetPtr->PutCollect("sphone", _variant_t(BlackInfo.Phone));
		m_pRecordsetPtr->PutCollect("smark", _variant_t(BlackInfo.other));

		m_pRecordsetPtr->Update();
		m_pRecordsetPtr->Close(); 
		return true;
	}
	return false;
}

bool IO::CAR_BlackTable_DeleteWithNid(unsigned long int nid)
{
	CString strSql;
	strSql.Format("delete from tb_car_black where nid=%d",nid);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}

bool IO::CAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo)
{
	CString strSql;
	strSql.Format(_T("update tb_car_black set scarnumber='%s',\
					sbrand='%s',slostname='%s',sphone='%s',smark='%s' where nid=%d"),\
					BlackInfo.plate,BlackInfo.brand,BlackInfo.name,BlackInfo.Phone,BlackInfo.other,BlackInfo.nid);

	_variant_t RecordAffected;
	m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);	  
	if(RecordAffected.uintVal == 1)	   //判断是否更新成功
	{
		return	true;
	}
	else
	{
		return false;
	}
}

unsigned long IO::CAR_BlackTable_GetNum(void)
{
	CString	strSql = _T("select count(1) from tb_car_black");

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

	return num;	
}

bool IO::CAR_BlackTable_Read(int startNum,int endNum,list<struct BLACK_DATA_ST> &BlackList)
{
	CString	strSql;
	strSql.Format(_T("select * from (select nid,scarnumber,sbrand,slostname,sphone,smark,rownum rn from tb_car_black) where rn>=%d and rn<=%d"),startNum,endNum);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		struct BLACK_DATA_ST black = {0};

		black.nid = (long)m_pRecordsetPtr->GetCollect("nid");
		var = m_pRecordsetPtr->GetCollect("scarnumber");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.plate, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sbrand");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.brand, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("slostname");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.name, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sphone");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.Phone, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("smark");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.other, str.GetBuffer(0));
		}

		BlackList.push_back(black);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(BlackList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool IO::CAR_BlackTable_ReadOne(int Num,struct BLACK_DATA_ST &black)
{
	CString	strSql;
	strSql.Format(_T("select * from (select nid,scarnumber,sbrand,slostname,sphone,smark,rownum rn from tb_car_black) where rn=%d"),Num);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	variant_t var;
	variant_t temp; 
	temp.ChangeType(VT_NULL);
	CString str;

	black.nid = (long)m_pRecordsetPtr->GetCollect("nid");
	var = m_pRecordsetPtr->GetCollect("scarnumber");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.plate, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("sbrand");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.brand, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("slostname");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.name, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("sphone");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.Phone, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("smark");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.other, str.GetBuffer(0));
	}

	m_pRecordsetPtr->Close();

	return true;
}

bool IO::CAR_BlackTable_DeleteAll(void)
{
	CString strSql=_T("delete from tb_car_black");
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}

bool IO::ELECAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo)
{
	CString strSql = _T("select seq_black_vehicle.nextval as nid from dual");

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	long id	= m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select * from tb_vehicle_black", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(BlackInfo.plate));
		m_pRecordsetPtr->PutCollect("sbrand", _variant_t(BlackInfo.brand));
		m_pRecordsetPtr->PutCollect("slostname", _variant_t(BlackInfo.name));
		m_pRecordsetPtr->PutCollect("sphone", _variant_t(BlackInfo.Phone));
		m_pRecordsetPtr->PutCollect("smark", _variant_t(BlackInfo.other));

		m_pRecordsetPtr->Update();
		m_pRecordsetPtr->Close(); 
		return true;
	}
	return false;
}

bool IO::ELECAR_BlackTable_DeleteWithNid(unsigned long int nid)
{
	CString strSql;
	strSql.Format("delete from tb_vehicle_black where nid=%d",nid);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}

bool IO::ELECAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo)
{
	CString strSql;
	strSql.Format(_T("update tb_vehicle_black set scarnumber='%s',\
					sbrand='%s',slostname='%s',sphone='%s',smark='%s' where nid=%d"),\
					BlackInfo.plate,BlackInfo.brand,BlackInfo.name,BlackInfo.Phone,BlackInfo.other,BlackInfo.nid);

	_variant_t RecordAffected;
	m_pConnection->Execute((_bstr_t)strSql, &RecordAffected, adCmdText);	  
	if(RecordAffected.uintVal == 1)	   //判断是否更新成功
	{
		return	true;
	}
	else
	{
		return false;
	}
}

unsigned long IO::ELECAR_BlackTable_GetNum(void)
{
	CString	strSql = _T("select count(1) from tb_vehicle_black");

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

	return num;	
}

bool IO::ELECAR_BlackTable_Read(int startNum,int endNum,list<struct BLACK_DATA_ST> &BlackList)
{
	CString	strSql;
	strSql.Format(_T("select * from (select nid,scarnumber,sbrand,slostname,sphone,smark,rownum rn from tb_vehicle_black) where rn>=%d and rn<=%d"),startNum,endNum);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		struct BLACK_DATA_ST black = {0};

		black.nid = (long)m_pRecordsetPtr->GetCollect("nid");
		var = m_pRecordsetPtr->GetCollect("scarnumber");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.plate, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sbrand");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.brand, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("slostname");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.name, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sphone");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.Phone, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("smark");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(black.other, str.GetBuffer(0));
		}

		BlackList.push_back(black);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(BlackList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

bool IO::ELECAR_BlackTable_ReadOne(int Num,struct BLACK_DATA_ST &black)
{
	CString	strSql;
	strSql.Format(_T("select * from (select nid,scarnumber,sbrand,slostname,sphone,smark,rownum rn from tb_vehicle_black) where rn=%d"),Num);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	variant_t var;
	variant_t temp; 
	temp.ChangeType(VT_NULL);
	CString str;

	black.nid = (long)m_pRecordsetPtr->GetCollect("nid");
	var = m_pRecordsetPtr->GetCollect("scarnumber");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.plate, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("sbrand");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.brand, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("slostname");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.name, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("sphone");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.Phone, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("smark");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(black.other, str.GetBuffer(0));
	}

	m_pRecordsetPtr->Close();

	return true;
}

bool IO::ELECAR_BlackTable_DeleteAll(void)
{
	CString strSql=_T("delete from tb_vehicle_black");
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	return true;
}

/*************************************************************************
//以下与平台连接
************************************************************************/
#if YRVM_PINGTAI_MODE

//断开与oracle数据库的连接
bool IO::YRVM_DisConnectionOracleDB(void)
{
	try
	{
		if(YRVM_pConnection->State)
		{
			YRVM_pConnection->Close();
			YRVM_state=false;
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

//连接数据库
int IO::YRVM_ConnectionOracleDBTXT(char*  FileName)
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
	if(SUCCEEDED(YRVM_pConnection.CreateInstance("ADODB.Connection")))
	{
		_bstr_t strConnect = _bstr_t(CstrConn);

		YRVM_pConnection->ConnectionTimeout = 30;

		try
		{
			hr = YRVM_pConnection->Open(strConnect,"","",adModeUnknown);
		}
		catch(_com_error e)
		{
			//CString temp;			
			//temp.Format(_T("Error:%s"),e.ErrorMessage());  
			//AfxMessageBox(temp);  
			return ContOpen_FAIL;
		}

		YRVM_state=true;
		return Connectd_DONE;
	}
	else
	{
		//AfxMessageBox("Create ADODB Connection Instance Failed.");
		return Instance_FAIL;
	}
}
// 获取数据库临时电动车表的nid
unsigned long int IO::YRVM_getElectricCarOracleTempNid(void)
{
	unsigned long int id=0;

	CString strSql = _T("select seq_temp_photo_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	id = (long)m_pRecordsetPtr->GetCollect("nid");

	return id;
}

// 写电动车临时照片表
bool IO::YRVM_writeElectricCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id)
{
	CString strSql;

	strSql.Format(_T("select objectid as ncameraid from tb_camera_info where sip = '%s'"), IpAddr);	 //获取当前连接监控摄像头IP所对应的摄像头编号

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	//如果没有这个摄像头
	variant_t var;
	variant_t temp; 
	temp.ChangeType(VT_NULL);
	
	var =(long)  m_pRecordsetPtr->GetCollect("ncameraid"); 
	
	unsigned long int cameraid;
	if(var.vt != NULL && var !=temp)
		cameraid=(long)var;
	else
		return false;

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select nid, ncamera, bpicture, ssize, sgettype, sip, nflag from tb_temp_photo_vehicle", YRVM_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t((long)id));
		m_pRecordsetPtr->PutCollect("ncamera", _variant_t((long)cameraid));
		m_pRecordsetPtr->PutCollect("ssize", _variant_t(ImageSize));
		m_pRecordsetPtr->PutCollect("sgettype", _variant_t((long)1));
		m_pRecordsetPtr->PutCollect("sip", _variant_t(IpAddr));
		m_pRecordsetPtr->PutCollect("nflag", _variant_t((long)0));

		char *m_pbuff = NULL;

		SAFEARRAYBOUND rgs[1];	
		rgs[0].lLbound = 0;	   
		rgs[0].cElements =	ImageSize;		

		SAFEARRAY *psa;	   
		psa = SafeArrayCreate(VT_UI1,1,rgs); 
		SafeArrayAccessData(psa,(void **)&m_pbuff);		
		
		memcpy(m_pbuff, Image, ImageSize); 

		variant_t varBOLB;
		varBOLB.vt = VT_ARRAY | VT_UI1;
		varBOLB.parray = psa;

		m_pRecordsetPtr->GetFields()->GetItem("bpicture")->AppendChunk(varBOLB);	  //picture

		m_pRecordsetPtr->Update();

		SafeArrayUnaccessData(psa);

		m_pRecordsetPtr->Close(); 

		return true;   		

	}
	return false;
}

// 写临时电动车信息表
bool IO::YRVM_writeElectricCarTempInfoToOracleDB(char *CarStr,char* Color,int Direction,char *Reliability,unsigned long int id)
{
	CString strSql = _T("select seq_temp_electric_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	unsigned long int nvehicleid	= (long)m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select nphoto, nvehicleid, scarnumber, scarnumtype, ncarnumflag, srate from tb_temp_electric_vehicle", YRVM_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nvehicleid", _variant_t((long)nvehicleid));
		m_pRecordsetPtr->PutCollect("nphoto", _variant_t((long)id));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(CarStr));
		m_pRecordsetPtr->PutCollect("scarnumtype", _variant_t(Color));
		m_pRecordsetPtr->PutCollect("ncarnumflag", _variant_t((long)Direction));
		m_pRecordsetPtr->PutCollect("srate", _variant_t(Reliability));

		m_pRecordsetPtr->Update();

		m_pRecordsetPtr->Close(); 

		return true;   		

	}
	return false;
}

// 获取数据库临时机动车表的nid
unsigned long int IO::YRVM_getCarOracleTempNid(void)
{
	unsigned long int id;

	CString strSql = _T("select seq_temp_photo_car.nextval as nid from dual");	    //获取临时车辆图像数据nid

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	id	=(long) m_pRecordsetPtr->GetCollect("nid");

	return id;
}

// 写机动车临时照片表
bool IO::YRVM_writeCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id)
{
	CString strSql;

	strSql.Format(_T("select objectid as ncameraid from tb_camera_info where sip = '%s'"), IpAddr);	 //获取当前连接监控摄像头IP所对应的摄像头编号

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	variant_t var;
	variant_t temp; 
	temp.ChangeType(VT_NULL);
	
	var =(long)  m_pRecordsetPtr->GetCollect("ncameraid"); 
	
	unsigned long int cameraid;
	if(var.vt != NULL && var !=temp)
		cameraid=(long)var;
	else
		return false;

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select nid, ncamera, bpicture, ssize, sgettype, sip, nflag from tb_temp_photo_car", YRVM_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t((long) id));
		m_pRecordsetPtr->PutCollect("ncamera", _variant_t((long) cameraid));
		m_pRecordsetPtr->PutCollect("ssize", _variant_t(ImageSize));
		m_pRecordsetPtr->PutCollect("sgettype", _variant_t((long) 1));
		m_pRecordsetPtr->PutCollect("sip", _variant_t(IpAddr));
		m_pRecordsetPtr->PutCollect("nflag", _variant_t((long) 0));

		char *m_pbuff = NULL;

		SAFEARRAYBOUND rgs[1];	
		rgs[0].lLbound = 0;	   
		rgs[0].cElements =	ImageSize;		

		SAFEARRAY *psa;	   
		psa = SafeArrayCreate(VT_UI1,1,rgs); 
		SafeArrayAccessData(psa,(void **)&m_pbuff);		
		
		memcpy(m_pbuff, Image, ImageSize); 

		variant_t varBOLB;
		varBOLB.vt = VT_ARRAY | VT_UI1;
		varBOLB.parray = psa;

		m_pRecordsetPtr->GetFields()->GetItem("bpicture")->AppendChunk(varBOLB);	  //picture

		m_pRecordsetPtr->Update();

		SafeArrayUnaccessData(psa);

		m_pRecordsetPtr->Close(); 

		return true;   		

	}
	return false;
}

// 写临时机动车信息表
bool IO::YRVM_writeCarTempInfoToOracleDB(char *CarStr,char* PlateColor,int Direction,char *Reliability,char* Type,char *CarColor,unsigned long int id)
{
	CString strSql = _T("select seq_temp_car.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	unsigned long int ncarid	=(long) m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select nphoto, ncarid, scarnumber, stype, scategory, scarnumtype, ncarnumflag, srate,scolor from tb_temp_car", YRVM_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("ncarid", _variant_t((long) ncarid));
		m_pRecordsetPtr->PutCollect("nphoto", _variant_t((long) id));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(CarStr));
		m_pRecordsetPtr->PutCollect("stype", _variant_t(Type));
		m_pRecordsetPtr->PutCollect("scategory", _variant_t(Type));
		m_pRecordsetPtr->PutCollect("scarnumtype", _variant_t(PlateColor));
		m_pRecordsetPtr->PutCollect("ncarnumflag", _variant_t((long) Direction));
		m_pRecordsetPtr->PutCollect("srate", _variant_t(Reliability));
		m_pRecordsetPtr->PutCollect("scolor", _variant_t(CarColor));
		
		m_pRecordsetPtr->Update();

		m_pRecordsetPtr->Close(); 

		return true;   		

	}
	return false;
}

//车牌存储过程
void IO::YRVM_ExecuteInsertMatchCarResultProcedure(unsigned long int id)
{
	_CommandPtr pCommandPtr = NULL;
	pCommandPtr.CreateInstance(_uuidof(Command));

	_ParameterPtr pParameterPtr;
	pParameterPtr = pCommandPtr->CreateParameter(_T("i_taskid"), adInteger, adParamInput, sizeof(long), _variant_t((long)NULL));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("i_nid"), adInteger, adParamInput, sizeof(long), _variant_t((long)id));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_match_succes_num"), adInteger, adParamOutput, sizeof(long), _variant_t((long)NULL));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_result"), adInteger, adParamOutput, sizeof(long), _variant_t((long)NULL));	
	pCommandPtr->Parameters->Append(pParameterPtr);

	pCommandPtr->CommandText = "pro_auto_match_car";
	pCommandPtr->CommandType = adCmdStoredProc;	 

	pCommandPtr->ActiveConnection =	YRVM_pConnection;

	pCommandPtr->Execute(NULL, NULL, adCmdStoredProc);	
}

//电动车车牌存储过程
void IO::YRVM_ExecuteInsertMatchVehicleResultProcedure(unsigned long int id)
{
	_CommandPtr pCommandPtr = NULL;
	pCommandPtr.CreateInstance(_uuidof(Command));

	_ParameterPtr pParameterPtr;
	pParameterPtr = pCommandPtr->CreateParameter(_T("i_taskid"), adInteger, adParamInput, sizeof(long), _variant_t((long)NULL));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("i_nid"), adInteger, adParamInput, sizeof(long), _variant_t((long)id));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_match_succes_num"), adInteger, adParamOutput, sizeof(long), _variant_t((long)NULL));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_result"), adInteger, adParamOutput, sizeof(long), _variant_t((long)NULL));	
	pCommandPtr->Parameters->Append(pParameterPtr);

	
	pCommandPtr->CommandText = "pro_auto_match_vehicle";
	pCommandPtr->CommandType = adCmdStoredProc;	 

	pCommandPtr->ActiveConnection =	YRVM_pConnection;

	
	pCommandPtr->Execute(NULL, NULL, adCmdStoredProc);	

}

#endif