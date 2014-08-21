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

	IOwriteLock=false;

}

IO::~IO(void)
{
	DisConnectionOracleDB();

#if YRVM_PINGTAI_MODE
	YRVM_DisConnectionOracleDB();
#endif

#if IVMS_ORACLE_DEVICETREE
	IVMS_DisConnectionOracleDB();
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

bool IO::DEVICE_ReadAllOrgName(vector<struct DEVICE_AREA_ST>& DeviceAreaList)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_ReadAllOrgName error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::DEVICE_AddNewArea(CString AreaName)
{
	try
	{
		CString strSql = _T("select seq_zd_org.nextval as nid from dual");			//获取机构nid
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	
		variant_t nid = m_pRecordsetPtr->GetCollect("nid");

		m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));
		HRESULT hr = m_pRecordsetPtr->Open("select sorgname, nid from T_ZD_ORG t", \
			m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_AddNewArea error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::DEVICE_DeleteArea(long areaID)
{
	try
	{
		CString strSql;
		strSql.Format("delete from T_ZD_ORG where nid=%d",areaID);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_DeleteArea error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}


bool IO::DEVICE_AddNewCamera(CString AreaName,CString CamName,CString sip,int port,int channel,
							 CString user,CString password,long venderID,CString Rtspurl,int RTP,int DecodeTag)
{
	try
	{
		CString strSql;
		char tempstr[260];
		strcpy(tempstr,AreaName.GetBuffer(0));
		filterstring(tempstr);
		AreaName=tempstr;

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
			m_pRecordsetPtr->PutCollect("nid", _variant_t(nid));
			m_pRecordsetPtr->PutCollect("ncamera", _variant_t(ncamera));
			m_pRecordsetPtr->PutCollect("svenderid", _variant_t(venderID));

			m_pRecordsetPtr->PutCollect("rtspurl", _variant_t(Rtspurl));
			m_pRecordsetPtr->PutCollect("rtpmode", _variant_t((long)RTP));
			m_pRecordsetPtr->PutCollect("decodetag", _variant_t((long)DecodeTag));


			m_pRecordsetPtr->Update();
			m_pRecordsetPtr->Close(); 
			return true;   		
		}
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_AddNewCamera error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::DEVICE_ReadCameraInfo(char* AreaName,vector<IPLIST>& CameraList)
{
	try
	{
		CString strSql;
		filterstring(AreaName);
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
			variant_t venderID = m_pRecordsetPtr->GetCollect("svenderid");
			variant_t channel = m_pRecordsetPtr->GetCollect("channel");//20140718

			variant_t rtspurl = m_pRecordsetPtr->GetCollect("rtspurl");//20140718
			variant_t RTP = m_pRecordsetPtr->GetCollect("rtpmode");//20140718
			variant_t decodetag = m_pRecordsetPtr->GetCollect("decodetag");//20140718

			IPLIST CameraInfo;
			CameraInfo.area = AreaName;
			CameraInfo.camID = ncamera;
			CameraInfo.venderID = venderID;
			CameraInfo.channel = channel;//20140718
			CameraInfo.RTP = RTP;
			CameraInfo.DecodeTag = decodetag;

			if(sip.vt != NULL && sip !=temp)
			{
				CameraInfo.ip = sip.bstrVal;
			}
			else
				CameraInfo.ip="";

			if(CamName.vt != NULL && CamName !=temp)
			{
				CameraInfo.name = CamName.bstrVal;
			}
			else
				CameraInfo.name="";

			CameraInfo.port = port;

			if(password.vt != NULL && password !=temp)
			{
				CameraInfo.psw = password.bstrVal;
			}
			else
				CameraInfo.psw="";

			if(user.vt != NULL && user !=temp)
			{
				CameraInfo.user = user.bstrVal;
			}
			else
				CameraInfo.user="";

			if(rtspurl.vt != NULL && rtspurl !=temp)
			{
				CameraInfo.Rtspurl = rtspurl.bstrVal;
			}
			else
				CameraInfo.Rtspurl="";

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_ReadCameraInfo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::DEVICE_DeleteCamera(long cameraID)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_device where ncamera=%d",cameraID);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_DeleteCamera error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::DEVICE_DeleteCameraWithAreaID(long AreaID)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_device where nid=%d",AreaID);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_DeleteCameraWithAreaID error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::DEVICE_UpdateCameraInfo(long CamID,CString AreaName,CString CamName,CString sip,int port,int channel,
								 CString user,CString password,long venderID,CString Rtspurl,int RTP,int DecodeTag)
{
	try
	{
		CString strSql;

		char tempstr[260];
		strcpy(tempstr,AreaName.GetBuffer(0));
		filterstring(tempstr);
		AreaName=tempstr;

		strSql.Format("select nid from T_ZD_ORG where sorgname='%s'",AreaName);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		if(m_pRecordsetPtr->BOF)
		{
			return false;
		}
		variant_t a = m_pRecordsetPtr->GetCollect("nid");

		strcpy(tempstr,CamName.GetBuffer(0));
		filterstring(tempstr);
		CamName=tempstr;

		strcpy(tempstr,Rtspurl.GetBuffer(0));
		filterstring(tempstr);
		Rtspurl=tempstr;


		long nid = a;
		strSql.Format(_T("update tb_device set scameraname='%s',\
						 sipserver='%s',sport=%d,channel=%d,suser='%s',spwd='%s',\
						 nid=%d,svenderid = %d,rtspurl='%s',rtpmode=%d,decodetag=%d where ncamera=%d"), 
						 CamName,sip,port,channel,user,password,nid,venderID, Rtspurl, RTP,DecodeTag,CamID);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::DEVICE_UpdateCameraInfo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::USER_AddNewUser(struct SET_USER_DATA_ST UserInfo)
{
	try
	{
		CString strSql;
		filterstring(UserInfo.user);
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
		HRESULT hr = m_pRecordsetPtr->Open("select * from tb_user", \
			m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::USER_AddNewUser error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::USER_DeletetUser(unsigned long int nid)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_user where nid=%d",nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::USER_DeletetUser error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::USER_UpdateUserInfo(struct SET_USER_DATA_ST UserInfo)
{
	try
	{
		CString strSql;
		filterstring(UserInfo.user);
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::USER_UpdateUserInfo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::USER_ReadUserInfoWithName(char *user,struct SET_USER_DATA_ST *UserInfo)
{
	try
	{
		CString	strsql;
		filterstring(user);
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::USER_ReadUserInfoWithName error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::USER_ReadUserInfoWithNid(unsigned long int nid,struct SET_USER_DATA_ST *UserInfo)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::USER_ReadUserInfoWithNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::USER_ReadAllUserInfo(list<struct SET_USER_LIST_ST> &UserInfoList)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::USER_ReadAllUserInfo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}


bool IO::LOG_AddNewSystemLog(char *userName,char *logStr)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

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
			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::LOG_AddNewSystemLog error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

unsigned long IO::LOG_GetSystemLogNum(char *userName,char *startTime,char *endTime,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql="";

		CString tempSql="";

		strSql= _T("select count(1) from tb_log");

		if((flag&0x01) != 0)
		{	
			filterstring(userName);
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
		IOwriteLock=false;
		return num;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::LOG_GetSystemLogNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return 0;
	}  
	IOwriteLock=false;
}

bool IO::LOG_ReadSystemLog(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_REPORT_ST> &HistoryList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
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
			IOwriteLock=false;

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
			IOwriteLock=false;

			return true;
		}
		IOwriteLock=false;

		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::LOG_ReadSystemLog error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;
}

bool IO::CAR_MatchResult_AddNew(bool *isblack,unsigned long CAMID,char *platetype,char	*platecolor,char *direction,char *carcolor,
								char *plate,int reliability,char *path,unsigned long int picsize,unsigned char *picdata)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(100);
		}

		IOwriteLock=true;

		CString strSql = _T("select seq_match_result_car.nextval as nid from dual");	    //获取电动车图像数据nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		long id = (long)m_pRecordsetPtr->GetCollect("nid");

		strSql.Format(_T("select sipserver,scameraname from tb_device where ncamera=%d"),CAMID);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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

			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_MatchResult_AddNew error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	IOwriteLock=false;
}

bool IO::CAR_MatchResult_AddNewNoPicData(bool *isblack,unsigned long CAMID,char *platetype,char	*platecolor,char *direction,char *carcolor,
										 char *plate,int reliability,char *path,unsigned long int picsize)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		CString strSql = _T("select seq_match_result_car.nextval as nid from dual");	    //获取电动车图像数据nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		long id = (long)m_pRecordsetPtr->GetCollect("nid");

		strSql.Format(_T("select sipserver,scameraname from tb_device where ncamera=%d"),CAMID);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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

			m_pRecordsetPtr->Update();
			m_pRecordsetPtr->Close(); 

			CAR_AlarmResult_Execute(id,isblack);

			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_MatchResult_AddNewNoPicData error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	IOwriteLock=false;
}


bool IO::CAR_MatchResult_GetPicture(unsigned long NID,unsigned char *data)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString strsql;

		strsql.Format(_T("select bpicture from tb_match_result_car where bpicture is not null and nid=%d"),NID);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);

		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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

			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_MatchResult_GetPicture error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	IOwriteLock=false;
}

bool IO::CAR_AlarmResult_GetPicture(unsigned long NID,unsigned char *data)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString strsql;

		strsql.Format(_T("select bpicture from tb_alarm_car where bpicture is not null and nid=%d"),NID);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);

		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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
			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_AlarmResult_GetPicture error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	IOwriteLock=false;
}

unsigned long IO::CAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
										 char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql="";
		CString outSQL="";
		CString tempSql="";
		bool hadwhere=false;		//检查语句中是否已经有where
		strSql= _T("select count(1) from tb_match_result_car");

		if((flag&0x01) != 0)
		{	
			filterstring(CamName);
			tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
			hadwhere = true;
			outSQL = outSQL + tempSql;
			strSql = strSql + tempSql;
		}
		if((flag&0x02) != 0)
		{
			filterstring(sip);
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
			filterstring(plate);
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

			filterstring(direction);
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
			filterstring(platecolor);
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

			filterstring(platetype);
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
			filterstring(carcolor);
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

		IOwriteLock=false;

		return num;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_MatchResult_GetNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		IOwriteLock=false;

		return 0;
	}  

	IOwriteLock=false;

}

unsigned long IO::CAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
										 char *direction,char *platecolor,char *platetype,char *carcolor,int flag,char *SQLstr)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql="";
		CString outSQL="";
		CString tempSql="";
		bool hadwhere=false;		//检查语句中是否已经有where
		strSql= _T("select count(1) from tb_alarm_car");

		if((flag&0x01) != 0)
		{	
			filterstring(CamName);
			tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
			hadwhere = true;
			outSQL = outSQL + tempSql;
			strSql = strSql + tempSql;
		}
		if((flag&0x02) != 0)
		{
			filterstring(sip);
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
			filterstring(plate);
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
			filterstring(direction);
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
			filterstring(platecolor);
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

			filterstring(platetype);
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

			filterstring(carcolor);
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

		IOwriteLock=false;

		return num;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_AlarmResult_GetNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		IOwriteLock=false;

		return 0;
	}  

	IOwriteLock=false;

}

bool IO::CAR_MatchResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{

	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,NCAMERA,rownum rn from tb_match_result_car");

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
			IOwriteLock=false;

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
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");
			HistoryList.push_back(result);
			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();
		if(HistoryList.size() > 0)
		{
			IOwriteLock=false;

			return true;
		}
		IOwriteLock=false;

		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_MatchResult_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;

		return false;
	}  
	IOwriteLock=false;

}

bool IO::CAR_AlarmResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;
	try
	{
		CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,NCAMERA,BLACKID,rownum rn from tb_alarm_car");

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
			IOwriteLock=false;
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
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");
			result.blackid = (long)m_pRecordsetPtr->GetCollect("BLACKID");
			HistoryList.push_back(result);
			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();
		if(HistoryList.size() > 0)
		{
			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;

		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_AlarmResult_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

int IO::CAR_MatchResult_ReadforListAll(struct HISTORY_DETECT_ST *HistoryList)
{
	try
	{
		CString	strSql = _T("select * from (select a.*,to_char(a.dcreate) from tb_match_result_car a  order by a.nid desc)  where  rownum=1");


		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{
			return 0;
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
			variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(a.dcreate)");
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

			result.nflag = (long)m_pRecordsetPtr->GetCollect("nflag");
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");


			memcpy(HistoryList,&result,sizeof(struct HISTORY_DETECT_ST));

			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();

		return 1;
	}
	catch(_com_error e)
	{

		errormessage.Format("IO::CAR_MatchResult_ReadforListAll error：%s", e.ErrorMessage());

		return -1;
	}  
}

int IO::CAR_MatchResult_ReadforListOne(long int camid,struct HISTORY_DETECT_ST *HistoryList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;

	try
	{
		CString	strSql;
		char timestr[64];
		CTime nowtime=CTime::GetTickCount();
		sprintf(timestr,"%04d%02d%02d000000",
			nowtime.GetYear(),
			nowtime.GetMonth(),
			nowtime.GetDay());
		strSql.Format(_T("select * from (select a.*,to_char(a.dcreate) from tb_match_result_car a  where a.ncamera=%d  and a.dcreate> to_date('%s','yyyymmddhh24miss')  order by a.nid desc)  where  rownum=1"),
			camid,timestr);


		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{

			IOwriteLock=false;
			return 0;
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
			variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(a.dcreate)");
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

			result.nflag = (long)m_pRecordsetPtr->GetCollect("nflag");
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");


			memcpy(HistoryList,&result,sizeof(struct HISTORY_DETECT_ST));

			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();

		IOwriteLock=false;
		return 1;
	}
	catch(_com_error e)
	{

		errormessage.Format("IO::CAR_MatchResult_ReadforListOne error：%s", e.ErrorMessage());

		IOwriteLock=false;
		return -1;
	}  

	IOwriteLock=false;
}

bool IO::CAR_AlarmResult_Execute(long nid,bool *isblack)
{
	//不要LOCK
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_AlarmResult_Execute error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

unsigned long int IO::CAR_MatchResult_Count(unsigned long int CAMID,char *startTime,char *endTime)
{
	try
	{
		CString	strSql="";

		strSql.Format(_T("select count(1) from tb_match_result_car where ncamera = %d \
						 and dcreate between to_date(%s,'yyyymmddhh24miss') \
						 and to_date(%s,'yyyymmddhh24miss')"),CAMID,startTime,endTime);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		unsigned long int num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

		return num;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_MatchResult_Count error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return 0;
	}  
}

bool IO::ELECAR_MatchResult_AddNew(bool *isblack,unsigned long CAMID,char *platecolor,char *direction,
								   char *plate,int reliability,
								   char *path,unsigned long int picsize,unsigned char *picdata)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		CString strSql = _T("select seq_match_result_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		long id = (long)m_pRecordsetPtr->GetCollect("nid");

		strSql.Format(_T("select sipserver,scameraname from tb_device where ncamera=%d"),CAMID);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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
			m_pRecordsetPtr->PutCollect("scolour", _variant_t(platecolor));
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

			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_MatchResult_AddNew error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}
bool IO::ELECAR_MatchResult_AddNewNoPicdata(bool *isblack,unsigned long CAMID,char *platecolor,char *direction,
											char *plate,int reliability,
											char *path,unsigned long int picsize)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		CString strSql = _T("select seq_match_result_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		long id = (long)m_pRecordsetPtr->GetCollect("nid");

		strSql.Format(_T("select sipserver,scameraname from tb_device where ncamera=%d"),CAMID);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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
			m_pRecordsetPtr->PutCollect("scolour", _variant_t(platecolor));
			m_pRecordsetPtr->PutCollect("sdirection", _variant_t(direction));
			m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(plate));
			m_pRecordsetPtr->PutCollect("ndegree", _variant_t((long)reliability));
			m_pRecordsetPtr->PutCollect("sfile", _variant_t(path));
			m_pRecordsetPtr->PutCollect("nsize", _variant_t((long)picsize));

			m_pRecordsetPtr->Update();
			m_pRecordsetPtr->Close(); 

			ELECAR_AlarmResult_Execute(id,isblack);

			IOwriteLock=false;
			return true;
		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_MatchResult_AddNewNoPicdata error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}


bool IO::ELECAR_MatchResult_GetPicture(unsigned long NID,unsigned char *data)
{
	try
	{
		CString strsql;

		strsql.Format(_T("select bpicture from tb_match_result_vehicle where bpicture is not null and nid=%d"),NID);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_MatchResult_GetPicture error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::ELECAR_AlarmResult_GetPicture(unsigned long NID,unsigned char *data)
{
	try
	{
		CString strsql;

		strsql.Format(_T("select bpicture from tb_alarm_vehicle where bpicture is not null and nid=%d"),NID);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_AlarmResult_GetPicture error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

unsigned long IO::ELECAR_MatchResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
											char *direction,int flag,char *SQLstr)
{
	try
	{
		CString	strSql="";
		CString outSQL="";
		CString tempSql="";
		bool hadwhere=false;		//检查语句中是否已经有where
		strSql= _T("select count(1) from tb_match_result_vehicle");

		if((flag&0x01) != 0)
		{	
			filterstring(CamName);
			tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
			hadwhere = true;
			outSQL = outSQL + tempSql;
			strSql = strSql + tempSql;
		}
		if((flag&0x02) != 0)
		{
			filterstring(sip);
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
			filterstring(plate);
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
			filterstring(direction);
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_MatchResult_GetNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return 0;
	}  
}

unsigned long IO::ELECAR_AlarmResult_GetNum(char *CamName,char *sip,char *plate,char *startTime,char *endTime,
											char *direction,int flag,char *SQLstr)
{
	try
	{
		CString	strSql="";
		CString outSQL="";
		CString tempSql="";
		bool hadwhere=false;		//检查语句中是否已经有where
		strSql= _T("select count(1) from tb_alarm_vehicle");

		if((flag&0x01) != 0)
		{	
			filterstring(CamName);
			tempSql.Format(_T(" where scameraname like '%%%s%%'"),CamName);
			hadwhere = true;
			outSQL = outSQL + tempSql;
			strSql = strSql + tempSql;
		}
		if((flag&0x02) != 0)
		{
			filterstring(sip);
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
			filterstring(plate);
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
			filterstring(direction);
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_AlarmResult_GetNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return 0;
	}  
}

bool IO::ELECAR_MatchResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{
	try
	{
		CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,NCAMERA,rownum rn from tb_match_result_vehicle");

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
			var = m_pRecordsetPtr->GetCollect("scolour");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.platecolor, str.GetBuffer(0));
			}
			result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
			var = m_pRecordsetPtr->GetCollect("sfile");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.path, str.GetBuffer(0));
			}
			result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_MatchResult_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::ELECAR_AlarmResult_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_DETECT_ST> &HistoryList)
{
	try
	{
		CString	strSql = _T("select * from (select nid,scameraname,sip,to_char(dcreate),stype,scolour,sdirection,scolor,scarnumber,ndegree,sfile,nsize,NCAMERA,BLACKID,rownum rn from tb_alarm_vehicle");

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
			var = m_pRecordsetPtr->GetCollect("scolour");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.platecolor, str.GetBuffer(0));
			}
			result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
			var = m_pRecordsetPtr->GetCollect("sfile");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.path, str.GetBuffer(0));
			}
			result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");
			result.blackid = (long)m_pRecordsetPtr->GetCollect("BLACKID");

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_AlarmResult_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

int IO::ELECAR_MatchResult_ReadforlistAll(struct HISTORY_DETECT_ST *HistoryList)
{
	try
	{
		CString	strSql = _T("select * from (select a.*,to_char(a.dcreate) from tb_match_result_vehicle a  order by a.nid desc)  where  rownum=1");

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{
			return 0;
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
			variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(a.dcreate)");
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
			var = m_pRecordsetPtr->GetCollect("scolour");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.platecolor, str.GetBuffer(0));
			}
			result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
			var = m_pRecordsetPtr->GetCollect("sfile");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.path, str.GetBuffer(0));
			}
			result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");
			result.nflag = (long)m_pRecordsetPtr->GetCollect("nflag");
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");

			memcpy(HistoryList,&result,sizeof(struct HISTORY_DETECT_ST));

			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();

		return 1;

	}
	catch(_com_error e)
	{

		errormessage.Format("IO::ELECAR_MatchResult_ReadforlistAll error：%s", e.ErrorMessage());

		return -1;
	}  
}


int IO::ELECAR_MatchResult_ReadforlistOne(long int camid,struct HISTORY_DETECT_ST *HistoryList)
{
	while(IOwriteLock)
	{
		Sleep(10);
	}

	IOwriteLock=true;

	try
	{
		CString	strSql;
		char timestr[64];
		CTime nowtime=CTime::GetTickCount();
		sprintf(timestr,"%04d%02d%02d000000",
			nowtime.GetYear(),
			nowtime.GetMonth(),
			nowtime.GetDay());

		strSql.Format( _T("select * from (select a.*,to_char(a.dcreate) from tb_match_result_vehicle a  where a.ncamera=%d and a.dcreate> to_date('%s','yyyymmddhh24miss')  order by a.nid desc)  where  rownum=1"),
			camid,timestr);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
			return 0;
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
			variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(a.dcreate)");
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
			var = m_pRecordsetPtr->GetCollect("scolour");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.platecolor, str.GetBuffer(0));
			}
			result.reliability = (long)m_pRecordsetPtr->GetCollect("ndegree");
			var = m_pRecordsetPtr->GetCollect("sfile");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.path, str.GetBuffer(0));
			}
			result.picsize = (long)m_pRecordsetPtr->GetCollect("nsize");
			result.nflag = (long)m_pRecordsetPtr->GetCollect("nflag");
			result.ncamid = (long)m_pRecordsetPtr->GetCollect("NCAMERA");

			memcpy(HistoryList,&result,sizeof(struct HISTORY_DETECT_ST));

			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();
		IOwriteLock=false;
		return 1;

	}
	catch(_com_error e)
	{

		errormessage.Format("IO::ELECAR_MatchResult_ReadforlistOne error：%s", e.ErrorMessage());
		IOwriteLock=false;
		return -1;
	}  
	IOwriteLock=false;
}


bool IO::ELECAR_AlarmResult_Execute(long nid,bool *isblack)
{
	//不要LOCK
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_AlarmResult_Execute error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

unsigned long int IO::ELECAR_MatchResult_Count(unsigned long int CAMID,char *startTime,char *endTime)
{
	try
	{
		CString	strSql="";

		strSql.Format(_T("select count(1) from tb_match_result_vehicle where ncamera = %d \
						 and dcreate between to_date(%s,'yyyymmddhh24miss') \
						 and to_date(%s,'yyyymmddhh24miss')"),CAMID,startTime,endTime);

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		unsigned long int num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

		return num;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_MatchResult_Count error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return 0;
	}  
}

bool IO::CAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_AddNew error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::CAR_BlackTable_DeleteWithNid(unsigned long int nid)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_car_black where nid=%d",nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_DeleteWithNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}
bool IO::CAR_Alarm_DeleteWithBlackNid(unsigned long int blackid)
{
	try
	{
		CString strSql;
		strSql.Format("delete from TB_ALARM_CAR where blackid=%d",blackid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_Alarm_DeleteWithBlackNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::CAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo)
{
	try
	{
		CString strSql;
		filterstring(BlackInfo.plate);
		filterstring(BlackInfo.brand);
		filterstring(BlackInfo.name);
		filterstring(BlackInfo.Phone);
		filterstring(BlackInfo.other);
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_Edit error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

unsigned long IO::CAR_BlackTable_GetNum(void)
{
	try
	{
		CString	strSql = _T("select count(1) from tb_car_black");

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

		return num;	
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_GetNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return 0;
	}  
}

bool IO::CAR_BlackTable_Read(long startNum,long endNum,list<struct BLACK_DATA_ST> &BlackList)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::CAR_BlackTable_ReadOne(long Num,struct BLACK_DATA_ST &black)
{
	try
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
	
		memset(&black,0,sizeof(struct BLACK_DATA_ST));

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_ReadOne error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}


bool IO::CAR_BlackTable_ReadOneWithNid(long Nid,struct BLACK_DATA_ST &black)
{
	try
	{
		CString	strSql;
		strSql.Format(_T("select nid,scarnumber,sbrand,slostname,sphone,smark from tb_car_black where nid=%d"),Nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{
			return false;
		}

		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
	
		memset(&black,0,sizeof(struct BLACK_DATA_ST));

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_ReadOneWithNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}
bool IO::CAR_BlackTable_DeleteAll(void)
{
	try
	{
		CString strSql=_T("delete from tb_car_black");
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_BlackTable_DeleteAll error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}
bool IO::CAR_Alarm_DeleteAll(void)
{
	try
	{
		CString strSql=_T("delete from TB_ALARM_CAR");
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::CAR_Alarm_DeleteAll error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::ELECAR_BlackTable_AddNew(struct BLACK_DATA_ST BlackInfo)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_AddNew error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::ELECAR_BlackTable_DeleteWithNid(unsigned long int nid)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_vehicle_black where nid=%d",nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_DeleteWithNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}
bool IO::ELECAR_Alarm_DeleteWithBlackNid(unsigned long int blackid)
{
	try
	{
		CString strSql;
		strSql.Format("delete from TB_ALARM_VEHICLE  where BLACKID=%d",blackid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_Alarm_DeleteWithBlackNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}


bool IO::ELECAR_BlackTable_Edit(struct BLACK_DATA_ST BlackInfo)
{
	try
	{
		CString strSql;
		filterstring(BlackInfo.plate);
		filterstring(BlackInfo.brand);
		filterstring(BlackInfo.name);
		filterstring(BlackInfo.Phone);
		filterstring(BlackInfo.other);
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_Edit error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

unsigned long IO::ELECAR_BlackTable_GetNum(void)
{
	try
	{
		CString	strSql = _T("select count(1) from tb_vehicle_black");

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("count(1)");

		return num;	
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_GetNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return 0;
	}  
}

bool IO::ELECAR_BlackTable_Read(long startNum,long endNum,list<struct BLACK_DATA_ST> &BlackList)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::ELECAR_BlackTable_ReadOne(long Num,struct BLACK_DATA_ST &black)
{
	try
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
		memset(&black,0,sizeof(struct BLACK_DATA_ST));

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_ReadOne error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::ELECAR_BlackTable_ReadOneWithNid(long Nid,struct BLACK_DATA_ST &black)
{
	try
	{
		CString	strSql;
		strSql.Format(_T("select nid,scarnumber,sbrand,slostname,sphone,smark from tb_vehicle_black where nid=%d"),Nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{
			return false;
		}

		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;
		memset(&black,0,sizeof(struct BLACK_DATA_ST));

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_ReadOneWithNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::ELECAR_BlackTable_DeleteAll(void)
{
	try
	{
		CString strSql=_T("delete from tb_vehicle_black");
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_BlackTable_DeleteAll error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}
//删除数据库黑名单 也要删这个
bool IO::ELECAR_Alarm_DeleteAll(void)
{
	try
	{
		CString strSql=_T("delete from TB_ALARM_VEHICLE");
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::ELECAR_ALarm_DeleteAll error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}


bool IO::VIDEO_AddNewVideo(char *camName,char *camIp,unsigned long int camVender,char *type,\
						   unsigned long int size,char *stime,char *etime,char *path)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		CString strSql = _T("select seq_video.nextval as nid from dual");	    //获取电动车图像数据nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		long id = (long)m_pRecordsetPtr->GetCollect("nid");
		filterstring(camName);
		filterstring(camIp);
		filterstring(path);

		strSql.Format(_T("insert into TB_VIDEO(nid,scameraname,sip,svenderid,sformat,sfilesize,dstarttime,dendtime,spath) \
						 values(%d,'%s','%s',%d,'%s',%d, \
						 to_date('%s','yyyy-mm-dd hh24:mi:ss'), \
						 to_date('%s','yyyy-mm-dd hh24:mi:ss'), '%s')"), \
						 id,	\
						 camName, \
						 camIp, \
						 camVender, \
						 type, \
						 size, \
						 stime, \
						 etime, \
						 path);

		m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

		IOwriteLock=false;
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::VIDEO_AddNewVideo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
	/*
	CString strSql = _T("select seq_video.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	long id = (long)m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select nid,scameraname,sip,svenderid,sformat,sfilesize,to_char(dstarttime,'yyyy-mm-dd hh24:mi:ss'),to_char(dendtime,'yyyy-mm-dd hh24:mi:ss'),spath from tb_video", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
	m_pRecordsetPtr->AddNew();
	m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
	m_pRecordsetPtr->PutCollect("scameraname", _variant_t(camName));
	m_pRecordsetPtr->PutCollect("sip", _variant_t(camIp));
	m_pRecordsetPtr->PutCollect("svenderid", _variant_t((long)camVender));
	m_pRecordsetPtr->PutCollect("sformat", _variant_t(type));
	m_pRecordsetPtr->PutCollect("sfilesize", _variant_t((long)size));
	m_pRecordsetPtr->PutCollect("to_char(dstarttime,'yyyy-mm-dd hh24:mi:ss')", _variant_t(stime));
	m_pRecordsetPtr->PutCollect("to_char(dstarttime,'yyyy-mm-dd hh24:mi:ss')", _variant_t(etime));
	m_pRecordsetPtr->PutCollect("spath", _variant_t(path));

	m_pRecordsetPtr->Update();
	m_pRecordsetPtr->Close(); 
	return true;
	}
	return false;
	*/
}


bool IO::VIDEO_DeleteVideo(unsigned long int nid)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_video where nid=%d",nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::VIDEO_DeleteVideo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

unsigned long IO::VIDEO_GetNum(char *camName,char *camIp,char *stime,char *etime,int flag,char *SQLstr)
{
	try
	{
		CString	strSql="";
		CString outSQL="";
		CString tempSql="";
		bool hadwhere=false;		//检查语句中是否已经有where
		strSql= _T("select count(1) from tb_video");

		if((flag&0x01) != 0)
		{	
			filterstring(camName);
			tempSql.Format(_T(" where scameraname like '%%%s%%'"),camName);
			hadwhere = true;
			outSQL = outSQL + tempSql;
			strSql = strSql + tempSql;
		}
		if((flag&0x02) != 0)
		{
			filterstring(camIp);
			if(hadwhere)
			{
				tempSql.Format(_T(" and sip like '%%%s%%'"),camIp);
			}
			else
			{
				tempSql.Format(_T(" where sip like '%%%s%%'"),camIp);
				hadwhere = true;
			}
			outSQL = outSQL + tempSql;
			strSql = strSql + tempSql;
		}
		if((flag&0x04) != 0)
		{
			if(hadwhere)
			{
				tempSql.Format(_T(" and dgettime between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),stime,etime);
			}
			else
			{
				tempSql.Format(_T(" where dgettime between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')"),stime,etime);
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::VIDEO_GetNum error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return 0;
	}  
}

bool IO::VIDEO_Read(char *SQLstr,int flag,long startNum,long endNum,list<struct HISTORY_VIDEO_ST> &Videolist)
{
	try
	{
		CString	strSql = _T("select * from (select nid,scameraname,sip,svenderid,sformat,sfilesize,to_char(dstarttime),to_char(dendtime),spath,rownum rn from tb_video");

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
			struct HISTORY_VIDEO_ST result = {0};

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

			result.venderID = (long)m_pRecordsetPtr->GetCollect("svenderid");

			var = m_pRecordsetPtr->GetCollect("sformat");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.format, str.GetBuffer(0));
			}

			result.size = (long)m_pRecordsetPtr->GetCollect("sfilesize");

			variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dstarttime)");
			sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
				&result.StartYear,&result.StartMon,&result.StartDay,
				&result.StartHour,&result.StartMin,&result.StartSec);

			vargetTime = m_pRecordsetPtr->GetCollect("to_char(dendtime)");
			sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
				&result.EndYear,&result.EndMon,&result.EndDay,
				&result.EndHour,&result.EndMin,&result.EndSec);

			var = m_pRecordsetPtr->GetCollect("spath");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(result.path, str.GetBuffer(0));
			}

			Videolist.push_back(result);
			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();
		if(Videolist.size() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::VIDEO_Read error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::RECORD_PlanTable_AddNew(struct SET_RECORD_DATA_ST rPlan)
{
	char sTime[32]={0};
	char eTime[32]={0};
	sprintf(sTime,"%04d-%02d-%02d %02d:%02d:%02d",
		rPlan.StartYear,
		rPlan.StartMon,
		rPlan.StartDay,
		rPlan.StartHour,
		rPlan.StartMin,
		rPlan.StartSec);
	sprintf(eTime,"%04d-%02d-%02d %02d:%02d:%02d",
		rPlan.EndYear,
		rPlan.EndMon,
		rPlan.EndDay,
		rPlan.EndHour,
		rPlan.EndMin,
		rPlan.EndSec);

	try
	{
		CString strSql = _T("select seq_record_plan.nextval as nid from dual");	    //获取电动车图像数据nid

		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

		long id = (long)m_pRecordsetPtr->GetCollect("nid");

		strSql.Format(_T("insert into TB_RECORD_PLAN(nid,ncamera,scameraarea,scameraname,sip,dailyflag,dateflag,weekflag, \
						 enableflag,startweek,endweek,starttime,endtime) \
						 values(%d,%d,'%s','%s','%s',%d,%d,%d,%d,%d,%d, \
						 to_date('%s','yyyy-mm-dd hh24:mi:ss'), \
						 to_date('%s','yyyy-mm-dd hh24:mi:ss'))"), \
						 id,rPlan.camid,rPlan.area,rPlan.name,rPlan.ip, \
						 rPlan.everyday,rPlan.usedate,rPlan.useweek,rPlan.enable,
						 rPlan.StartWeek,rPlan.EndWeek,sTime,eTime);

		m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_AddNew error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}
bool IO::RECORD_PlanTable_DeleteWithNid(unsigned long int nid)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_record_plan where nid=%d",nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_DeleteWithNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::RECORD_PlanTable_DeleteWithCamID(unsigned long int camID)
{
	try
	{
		CString strSql;
		strSql.Format("delete from tb_record_plan where ncamera=%d",camID);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_DeleteWithNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::RECORD_PlanTable_Edit(struct SET_RECORD_DATA_ST rPlan)
{
	char sTime[32]={0};
	char eTime[32]={0};
	sprintf(sTime,"%04d-%02d-%02d %02d:%02d:%02d",
		rPlan.StartYear,
		rPlan.StartMon,
		rPlan.StartDay,
		rPlan.StartHour,
		rPlan.StartMin,
		rPlan.StartSec);
	sprintf(eTime,"%04d-%02d-%02d %02d:%02d:%02d",
		rPlan.EndYear,
		rPlan.EndMon,
		rPlan.EndDay,
		rPlan.EndHour,
		rPlan.EndMin,
		rPlan.EndSec);
	try
	{
		CString strSql;
		strSql.Format(_T("update tb_record_plan set ncamera=%d,scameraarea='%s',scameraname='%s',sip='%s',\
						 dailyflag=%d,dateflag=%d,weekflag=%d,enableflag=%d,startweek=%d,endweek=%d,\
						 starttime=to_date('%s','yyyy-mm-dd hh24:mi:ss'),\
						 endtime=to_date('%s','yyyy-mm-dd hh24:mi:ss') where nid=%d"),\
						 rPlan.camid, rPlan.area, rPlan.name, rPlan.ip, \
						 rPlan.everyday, rPlan.usedate, rPlan.useweek, rPlan.enable, \
						 rPlan.StartWeek, rPlan.EndWeek, sTime, eTime, rPlan.nid);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_Edit error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::RECORD_PlanTable_ReadOne(unsigned long int nid,struct SET_RECORD_DATA_ST *rPlan)
{
	try
	{
		CString	strSql;
		strSql.Format(_T("select a.*,to_char(starttime),to_char(endtime) from tb_record_plan a where nid=%d"),nid);
		m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
		if(m_pRecordsetPtr->BOF)
		{
			return false;
		}

		variant_t var;
		variant_t temp; 
		temp.ChangeType(VT_NULL);
		CString str;

		rPlan->nid = nid;
		rPlan->camid = (long)m_pRecordsetPtr->GetCollect("ncamera");
		rPlan->everyday = (long)m_pRecordsetPtr->GetCollect("dailyflag");
		rPlan->usedate = (long)m_pRecordsetPtr->GetCollect("dateflag");
		rPlan->useweek = (long)m_pRecordsetPtr->GetCollect("weekflag");
		rPlan->enable = (long)m_pRecordsetPtr->GetCollect("enableflag");
		rPlan->StartWeek = (long)m_pRecordsetPtr->GetCollect("startweek");
		rPlan->EndWeek = (long)m_pRecordsetPtr->GetCollect("endweek");

		var = m_pRecordsetPtr->GetCollect("scameraarea");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(rPlan->area, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("scameraname");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(rPlan->name, str.GetBuffer(0));
		}
		var = m_pRecordsetPtr->GetCollect("sip");
		if(var.vt != NULL && var !=temp)
		{
			str = var.bstrVal;
			strcpy(rPlan->ip, str.GetBuffer(0));
		}

		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(starttime)");
		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			&rPlan->StartYear,&rPlan->StartMon,&rPlan->StartDay,
			&rPlan->StartHour,&rPlan->StartMin,&rPlan->StartSec);

		vargetTime = m_pRecordsetPtr->GetCollect("to_char(endtime)");
		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			&rPlan->EndYear,&rPlan->EndMon,&rPlan->EndDay,
			&rPlan->EndHour,&rPlan->EndMin,&rPlan->EndSec);

		m_pRecordsetPtr->Close();

		return true;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_ReadOne error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::RECORD_PlanTable_ReadAll(list<struct SET_RECORD_DATA_ST> &rPlanList)
{
	try
	{
		CString	strSql = _T("select a.*,to_char(starttime),to_char(endtime) from tb_record_plan a");

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
			struct SET_RECORD_DATA_ST rPlan = {0};

			rPlan.nid = (long)m_pRecordsetPtr->GetCollect("nid");
			rPlan.camid = (long)m_pRecordsetPtr->GetCollect("ncamera");
			rPlan.everyday = (long)m_pRecordsetPtr->GetCollect("dailyflag");
			rPlan.usedate = (long)m_pRecordsetPtr->GetCollect("dateflag");
			rPlan.useweek = (long)m_pRecordsetPtr->GetCollect("weekflag");
			rPlan.enable = (long)m_pRecordsetPtr->GetCollect("enableflag");
			rPlan.StartWeek = (long)m_pRecordsetPtr->GetCollect("startweek");
			rPlan.EndWeek = (long)m_pRecordsetPtr->GetCollect("endweek");

			var = m_pRecordsetPtr->GetCollect("scameraarea");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(rPlan.area, str.GetBuffer(0));
			}
			var = m_pRecordsetPtr->GetCollect("scameraname");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(rPlan.name, str.GetBuffer(0));
			}
			var = m_pRecordsetPtr->GetCollect("sip");
			if(var.vt != NULL && var !=temp)
			{
				str = var.bstrVal;
				strcpy(rPlan.ip, str.GetBuffer(0));
			}

			variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(starttime)");
			sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
				&rPlan.StartYear,&rPlan.StartMon,&rPlan.StartDay,
				&rPlan.StartHour,&rPlan.StartMin,&rPlan.StartSec);

			vargetTime = m_pRecordsetPtr->GetCollect("to_char(endtime)");
			sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
				&rPlan.EndYear,&rPlan.EndMon,&rPlan.EndDay,
				&rPlan.EndHour,&rPlan.EndMin,&rPlan.EndSec);

			rPlanList.push_back(rPlan);
			m_pRecordsetPtr->MoveNext();
		}
		m_pRecordsetPtr->Close();
		if(rPlanList.size() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_ReadAll error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::RECORD_PlanTable_SetRecordState(unsigned long nid,int enable)
{
	try
	{
		CString strSql;
		strSql.Format(_T("update tb_record_plan set enableflag=%d where nid=%d"),enable,nid);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_SetRecordState error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
}

bool IO::RECORD_PlanTable_UpdateCamInfo(unsigned long int camid,char *area,char *camname,char *ip)
{

	try
	{
		CString strSql;
		filterstring(area);
		filterstring(camname);
		filterstring(ip);

		strSql.Format(_T("update tb_record_plan set scameraarea='%s',scameraname='%s',sip='%s' where ncamera=%d"),
			area,camname,ip,camid);

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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::RECORD_PlanTable_UpdateCamInfo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		return false;
	}  
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
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		unsigned long int id=0;

		CString strSql = _T("select seq_temp_photo_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

		m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

		id = (long)m_pRecordsetPtr->GetCollect("nid");

		IOwriteLock=false;
		return id;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_getElectricCarOracleTempNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

// 写电动车临时照片表
bool IO::YRVM_writeElectricCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		CString strSql;

		strSql.Format(_T("select objectid as ncameraid from tb_camera_info where sip = '%s'"), IpAddr);	 //获取当前连接监控摄像头IP所对应的摄像头编号

		m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 
		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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
		{
			IOwriteLock=false;
			return false;
		}

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

			IOwriteLock=false;
			return true;   		

		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_writeElectricCarTempPhotoToOracleDB error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

// 写临时电动车信息表
bool IO::YRVM_writeElectricCarTempInfoToOracleDB(char *CarStr,char* Color,int Direction,char *Reliability,unsigned long int id)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

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

			IOwriteLock=false;
			return true;   		

		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_writeElectricCarTempInfoToOracleDB error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

// 获取数据库临时机动车表的nid
unsigned long int IO::YRVM_getCarOracleTempNid(void)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		unsigned long int id;

		CString strSql = _T("select seq_temp_photo_car.nextval as nid from dual");	    //获取临时车辆图像数据nid

		m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

		id	=(long) m_pRecordsetPtr->GetCollect("nid");

		IOwriteLock=false;
		return id;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_getCarOracleTempNid error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

// 写机动车临时照片表
bool IO::YRVM_writeCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		CString strSql;

		strSql.Format(_T("select objectid as ncameraid from tb_camera_info where sip = '%s'"), IpAddr);	 //获取当前连接监控摄像头IP所对应的摄像头编号

		m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	
		if(m_pRecordsetPtr->BOF)
		{
			IOwriteLock=false;
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
		{
			IOwriteLock=false;
			return false;
		}

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

			IOwriteLock=false;
			return true;   		

		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_writeCarTempPhotoToOracleDB error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

// 写临时机动车信息表
bool IO::YRVM_writeCarTempInfoToOracleDB(char *CarStr,char* PlateColor,int Direction,char *Reliability,char* Type,char *CarColor,unsigned long int id)
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

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

			IOwriteLock=false;
			return true;   		

		}
		IOwriteLock=false;
		return false;
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_writeCarTempInfoToOracleDB error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}

//车牌存储过程
void IO::YRVM_ExecuteInsertMatchCarResultProcedure(unsigned long int id)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_ExecuteInsertMatchCarResultProcedure error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
	}  
}

//电动车车牌存储过程
void IO::YRVM_ExecuteInsertMatchVehicleResultProcedure(unsigned long int id)
{
	try
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
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_ExecuteInsertMatchVehicleResultProcedure error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
	}  

}

//将摄像头抓拍人脸数据写入oracle数据库临时人员表中      //new add by ethink.wang  at 2012-8-8 17:12
bool IO::YRVM_writeTempFaceDataToOracleDB(unsigned char *Image,long int ImageSize, char* sip)    //10.142.50.249     
{
	try
	{
		while(IOwriteLock)
		{
			Sleep(10);
		}

		IOwriteLock=true;

		CString strSql = _T("select SEQ_TEMP_ID.nextval as nid from dual");			          //获取临时人员人脸图像数据nid

		m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);	

		unsigned long int id	= (long)m_pRecordsetPtr->GetCollect("nid");

		strSql.Format(_T("select objectid as ncameraid from tb_camera_info where sip = '%s'"), sip);	    //获取当前连接监控摄像头IP所对应的摄像头编号

		m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 

		unsigned long int cameraid = (long)m_pRecordsetPtr->GetCollect("ncameraid"); 

		m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

		HRESULT hr = m_pRecordsetPtr->Open("select nid, ncamera, bpicture from TB_TEMP_PHOTO", YRVM_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);		    //待定	  yrvm.TB_TEMP_PHOTO

		if(SUCCEEDED(hr))
		{
			m_pRecordsetPtr->AddNew();	 
			m_pRecordsetPtr->PutCollect("nid", _variant_t((long)id));                  //id    
			m_pRecordsetPtr->PutCollect("ncamera", _variant_t((long)cameraid));				 //device ip

			char *m_pbuff = NULL;

			SAFEARRAYBOUND rgs[1];	
			rgs[0].lLbound = 0;	   
			rgs[0].cElements =	ImageSize;		

			SAFEARRAY *psa;	   
			psa = SafeArrayCreate(VT_UI1,1,rgs); 
			SafeArrayAccessData(psa,(void **)&m_pbuff);		

			memcpy(m_pbuff, Image, ImageSize); 

			variant_t varBOLB;
			varBOLB.vt = VT_ARRAY | VT_UI1;		   //Indicates a SAFEARRAY pointer |  Indicates a byte
			varBOLB.parray = psa;

			m_pRecordsetPtr->GetFields()->GetItem("bpicture")->AppendChunk(varBOLB);	  //picture

			m_pRecordsetPtr->Update();

			SafeArrayUnaccessData(psa);

			m_pRecordsetPtr->Close(); 
			//m_pRecordsetPtr->Release();

			CString insertsql;

			insertsql.Format(_T("update TB_TEMP_PHOTO set nflag = 0 where nid = %d"), id);

			_variant_t RecordAffected;

			YRVM_pConnection->Execute((_bstr_t)insertsql, &RecordAffected, adCmdText);	 

			IOwriteLock=false;
			return true;
		}
		else
		{	
			IOwriteLock=false;
			return false;
		}
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::YRVM_writeTempFaceDataToOracleDB error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);
		IOwriteLock=false;
		return false;
	}  
}


#endif


#if IVMS_ORACLE_DEVICETREE

//断开与oracle数据库的连接
bool IO::IVMS_DisConnectionOracleDB(void)
{
	try
	{
		if(IVMS_pConnection->State)
		{
			IVMS_pConnection->Close();
			IVMS_state=false;
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
int IO::IVMS_ConnectionOracleDBTXT(char*  FileName)
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
	if(SUCCEEDED(IVMS_pConnection.CreateInstance("ADODB.Connection")))
	{
		_bstr_t strConnect = _bstr_t(CstrConn);

		IVMS_pConnection->ConnectionTimeout = 30;

		try
		{
			hr = IVMS_pConnection->Open(strConnect,"","",adModeUnknown);
		}
		catch(_com_error e)
		{
			//CString temp;			
			//temp.Format(_T("Error:%s"),e.ErrorMessage());  
			//AfxMessageBox(temp);  
			return ContOpen_FAIL;
		}

		IVMS_state=true;
		return Connectd_DONE;
	}
	else
	{
		//AfxMessageBox("Create ADODB Connection Instance Failed.");
		return Instance_FAIL;
	}
}

//获取根节点   全读
int IO::IVMS_ReadControlunit(list<struct CONTROL_UNIT_ST> &Controlunit)
{

	TCHAR	 strsql[5120];
	try
	{
		Controlunit.clear();
		_stprintf(strsql,
			_T("select * from CONTROL_UNIT"));

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return false;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}

		while(!IVMS_pRecordsetPtr->adoEOF)
		{
			struct CONTROL_UNIT_ST tempdata= {0};
			//填充数据

			tempdata.nid = VarSaveNumber2(IVMS_pRecordsetPtr,"CONTROL_UNIT_ID");
			tempdata.ParentId=VarSaveNumber2(IVMS_pRecordsetPtr,"PARENT_ID");
			VarSaveStr2(tempdata.name,IVMS_pRecordsetPtr,_T("NAME"));
			tempdata.comboxi=-1;
			Controlunit.push_back(tempdata);
			IVMS_pRecordsetPtr->MoveNext();
		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadControlunit error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return false;
	}  
	return true;

}


//获取区域 节点 全读
int IO::IVMS_ReadRegionInfo(list<struct REGION_INFO_ST> &RegionInfo)
{

	TCHAR	 strsql[5120];
	try
	{
		RegionInfo.clear();
		_stprintf(strsql,
			_T("select * from REGION_INFO") );

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return false;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}

		while(!IVMS_pRecordsetPtr->adoEOF)
		{
			struct REGION_INFO_ST tempdata= {0};
			//填充数据

			tempdata.nid = VarSaveNumber2(IVMS_pRecordsetPtr,"REGION_ID");
			tempdata.ControlUnitId=VarSaveNumber2(IVMS_pRecordsetPtr,"CONTROL_UNIT_ID");
			VarSaveStr2(tempdata.name,IVMS_pRecordsetPtr,_T("NAME"));
			tempdata.comboxi=-1;

			RegionInfo.push_back(tempdata);
			IVMS_pRecordsetPtr->MoveNext();
		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadRegionInfo error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return false;
	}  
	return true;

}


//获取流媒体服务器 全读
int IO::IVMS_ReadStreamserver(list<struct STREAM_SERVER_ST> &Streamserver)
{

	TCHAR	 strsql[5120];
	try
	{
		Streamserver.clear();
		_stprintf(strsql,
			_T("select * from STREAM_SERVER") );

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return false;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}

		while(!IVMS_pRecordsetPtr->adoEOF)
		{
			struct STREAM_SERVER_ST tempdata= {0};
			//填充数据

			tempdata.nid = VarSaveNumber2(IVMS_pRecordsetPtr,"STREAM_SV_ID");
			tempdata.ControlUnitId=VarSaveNumber2(IVMS_pRecordsetPtr,"CONTROL_UNIT_ID");
			tempdata.NetZoneId=VarSaveNumber2(IVMS_pRecordsetPtr,"NET_ZONE_ID");
			tempdata.RtspPort=VarSaveNumber2(IVMS_pRecordsetPtr,"RTSP_PORT");
			VarSaveStr2(tempdata.ip,IVMS_pRecordsetPtr,_T("IP_ADDR"));

			Streamserver.push_back(tempdata);
			IVMS_pRecordsetPtr->MoveNext();
		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadStreamserver error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return false;
	}  
	return true;

}
//获取PAG服务器 全读
int IO::IVMS_ReadPAGserver(list<struct PAG_SERVER_ST> &PAGserver)
{

	TCHAR	 strsql[5120];
	try
	{
		PAGserver.clear();
		_stprintf(strsql,
			_T("select * from PAG_SERVER") );

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return false;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}

		while(!IVMS_pRecordsetPtr->adoEOF)
		{
			struct PAG_SERVER_ST tempdata= {0};
			//填充数据

			tempdata.nid = VarSaveNumber2(IVMS_pRecordsetPtr,"PAG_SERVER_ID");
			tempdata.ControlUnitId=VarSaveNumber2(IVMS_pRecordsetPtr,"CONTROL_UNIT_ID");
			tempdata.NetZoneId=VarSaveNumber2(IVMS_pRecordsetPtr,"NET_ZONE_ID");
			tempdata.DataPort=VarSaveNumber2(IVMS_pRecordsetPtr,"DATA_PORT");
			VarSaveStr2(tempdata.ip,IVMS_pRecordsetPtr,_T("IP_ADDR"));

			PAGserver.push_back(tempdata);
			IVMS_pRecordsetPtr->MoveNext();
		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadPAGserver error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return false;
	}  
	return true;

}

//获取设备信息  全读
int IO::IVMS_ReadCaminfoALL(list<struct CAMERA_INFO_LITE_ST> &Caminfo,long regionid)
{

	TCHAR	 strsql[5120];
	try
	{
		Caminfo.clear();
		_stprintf(strsql,
			_T("select t.camera_id,t.region_id,t.name from CAMERA_INFO t where t.region_id=%d"),regionid );

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return false;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}
		struct CAMERA_INFO_LITE_ST tempdata= {0};

		while(!IVMS_pRecordsetPtr->adoEOF)
		{
			//填充数据

			tempdata.nid = VarSaveNumber2(IVMS_pRecordsetPtr,"CAMERA_ID");
			tempdata.RegionId=VarSaveNumber2(IVMS_pRecordsetPtr,"REGION_ID");
			VarSaveStr2(tempdata.name,IVMS_pRecordsetPtr,_T("NAME"));
			tempdata.comboxi=-1;

			Caminfo.push_back(tempdata);
			IVMS_pRecordsetPtr->MoveNext();
		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadCaminfoALL error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return false;
	}  
	return true;

}

//获取设备信息  全读
int IO::IVMS_ReadCaminfoFind(list<struct CAMERA_INFO_LITE_ST> &Caminfo,char *keyword)
{

	TCHAR	 strsql[5120];
	try
	{
		Caminfo.clear();
		filterstring(keyword);
		_stprintf(strsql,
			_T("select t.camera_id,t.region_id,t.name from CAMERA_INFO t where t.name like '%%%s%%'"),keyword);

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return 0;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}
		struct CAMERA_INFO_LITE_ST tempdata= {0};

		while(!IVMS_pRecordsetPtr->adoEOF)
		{
			//填充数据

			tempdata.nid = VarSaveNumber2(IVMS_pRecordsetPtr,"CAMERA_ID");
			tempdata.RegionId=VarSaveNumber2(IVMS_pRecordsetPtr,"REGION_ID");
			VarSaveStr2(tempdata.name,IVMS_pRecordsetPtr,_T("NAME"));
			tempdata.comboxi=-1;
			Caminfo.push_back(tempdata);
			IVMS_pRecordsetPtr->MoveNext();
		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadCaminfoFind error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return -1;
	}  
	return 1;

}


//获取设备信息  全读
int IO::IVMS_ReadCaminfoOne(struct CAMERA_INFO_ST *Caminfo,long camid,int connectflag)
{

	TCHAR	 strsql[5120];
	try
	{
		memset(Caminfo,0,sizeof(struct CAMERA_INFO_ST));
		_stprintf(strsql,
			_T("select * from CAMERA_INFO t where t.camera_id=%d"),camid );

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return false;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}

		char tempstr[64];
		if(!IVMS_pRecordsetPtr->adoEOF)
		{
			//填充数据

			Caminfo->nid = VarSaveNumber2(IVMS_pRecordsetPtr,"CAMERA_ID");
			Caminfo->RegionId = VarSaveNumber2(IVMS_pRecordsetPtr,"REGION_ID");
			Caminfo->DeviceId = VarSaveNumber2(IVMS_pRecordsetPtr,"DEVICE_ID");

			VarSaveStr2(Caminfo->IndexCode,IVMS_pRecordsetPtr,_T("INDEX_CODE"));
			VarSaveStr2(Caminfo->name,IVMS_pRecordsetPtr,_T("NAME"));

			Caminfo->channel = VarSaveNumber2(IVMS_pRecordsetPtr,"CHAN_NUM");

			Caminfo->Type = VarSaveNumber2(IVMS_pRecordsetPtr,"CAMERA_TYPE");
			Caminfo->RTP = VarSaveNumber2(IVMS_pRecordsetPtr,"CONNECT_TYPE");
			if(Caminfo->RTP >2)
				Caminfo->RTP =0;
			Caminfo->STREAM_TYPE = VarSaveNumber2(IVMS_pRecordsetPtr,"STREAM_TYPE");
			Caminfo->STREAM_SVR_ID = VarSaveNumber2(IVMS_pRecordsetPtr,"STREAM_SVR_ID");	
			VarSaveStr2(tempstr,IVMS_pRecordsetPtr,_T("DECODE_TAG"));
			
			if(connectflag)
			{
				//流媒体
				if(0==strcmp(tempstr,"hikvision"))
				{
					Caminfo->DecodeTag=DECODETAG_PS;
				}
				else  if(0==strcmp(tempstr,"ga_h264"))
				{
					Caminfo->DecodeTag=DECODETAG_SRC;
				}
				else
				{
					Caminfo->DecodeTag=DECODETAG_SRC;
				}

				Caminfo->venderID=VENDER_TYPE_STREAM;
			}
			else
			{
				//直连
				if(0==strcmp(tempstr,"hikvision"))
				{
					Caminfo->DecodeTag=DECODETAG_PS;
					Caminfo->venderID=VENDER_TYPE_HAIKANG;
				}
				else  if(0==strcmp(tempstr,"ga_h264"))
				{
					Caminfo->DecodeTag=DECODETAG_SRC;
					Caminfo->venderID=VENDER_TYPE_DAHUA;
				}
				else
				{
					Caminfo->DecodeTag=DECODETAG_PS;
					Caminfo->venderID=VENDER_TYPE_HAIKANG;
				}
			}
		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadCaminfoOne CAMERA error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return false;
	}  

	try
	{
		_stprintf(strsql,
			_T("select * from DEVICE_INFO t where t.device_id=%d"),Caminfo->DeviceId );

		IVMS_pRecordsetPtr = IVMS_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
		if(IVMS_pRecordsetPtr->BOF)
		{
			return false;
		}
		else
		{
			IVMS_pRecordsetPtr->MoveFirst();
		}

		if(!IVMS_pRecordsetPtr->adoEOF)
		{
			//填充数据
			Caminfo->ControlUnitId = VarSaveNumber2(IVMS_pRecordsetPtr,"CONTROL_UNIT_ID");
			Caminfo->PAG_SERVER_ID = VarSaveNumber2(IVMS_pRecordsetPtr,"PAG_SERVER_ID");

			VarSaveStr2(Caminfo->user,IVMS_pRecordsetPtr,_T("USER_NAME"));
			VarSaveStr2(Caminfo->psw,IVMS_pRecordsetPtr,_T("USER_PWD"));

			VarSaveStr2(Caminfo->ip,IVMS_pRecordsetPtr,_T("NETWORK_ADDR"));
			Caminfo->port = VarSaveNumber2(IVMS_pRecordsetPtr,"NETWORK_PORT");	

			//写死
			if(	Caminfo->DecodeTag ==DECODETAG_PS)
			{
				strcpy(Caminfo->user,"admin");
				strcpy(Caminfo->psw,"12345");
				Caminfo->port=8000;
			}
			else if(	Caminfo->DecodeTag ==DECODETAG_SRC)
			{
				strcpy(Caminfo->user,"admin");
				strcpy(Caminfo->psw,"admin");
				Caminfo->port=37777;
			}


		}

		IVMS_pRecordsetPtr->Close();
	}
	catch(_com_error e)
	{
		CString in_errormessage;
		in_errormessage.Format("IO::IVMS_ReadCaminfoOne DEVICE error：%s", e.ErrorMessage());
		AfxMessageBox(in_errormessage);

		return false;
	}  
	return true;

}



#endif