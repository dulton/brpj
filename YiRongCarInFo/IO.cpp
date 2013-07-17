
#include "stdafx.h"
#include "IO.h"


IO::IO(void)
{
	////连接本地oracle数据库
	//CString connstring = _T("Provider=OraOLEDB.Oracle.1;Password=goodguy;Persist Security Info=True;User ID=sde;Data Source=orcl"); 

	//连接远程oracle数据库
//	CString connstring = _T("Provider=OraOLEDB.Oracle.1;User ID=yrvm;Password=yrvm;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.142.50.125)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=yrvm)));Persist Security Info=False"); 
//	USES_CONVERSION;
//	LPCWSTR Lconnstring = A2CW((LPCSTR)connstring);
	
//	ConnectionOracleDB(Lconnstring);
}

IO::~IO(void)
{
	DisConnectionOracleDB();
}

#define RCT_MAX_STR_SIZE 256

bool IO::ReadFile(char* FileName)
{
	TCHAR temp[RCT_MAX_STR_SIZE];

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
		_stscanf(temp,_T("Psw:%s"),Psw);

		_fgetts(temp,RCT_MAX_STR_SIZE,fp);
		_stscanf(temp,_T("DataBaseName:%s"),DataBaseName);

		fclose(fp);

		return true;
	}
	else
		return false;
}

bool IO::ConnectionOracleDBTXT(char*  FileName)
{
	//ReadConfigTxt pConfig;

	if(!ReadFile(FileName))
	{
		return false;
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
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//连接oracle数据库
bool IO::ConnectionOracleDB(char* strConn)
{
	//CString CstrConn(strConn);
	HRESULT hr;
	try
	{
		//创建Connection对象
		hr = m_pConnection.CreateInstance("ADODB.Connection");    

		if(SUCCEEDED(hr))
		{
			_bstr_t strConnect = _bstr_t(strConn);

			//设置超时时间为30秒
			m_pConnection->ConnectionTimeout=30;    
		
			//连接数据库
			hr = m_pConnection->Open(strConnect,"","",adModeUnknown);
			
			return true;
		}
	}
	catch(_com_error e)            //捕捉异常
	{
		CString temp;
		temp.Format(_T("错误信息:%s"),e.ErrorMessage());
		TRACE("%s",temp);
		//MessageBox(temp, _T("数据库连接失败信息提示"));
		return false;
	}	

	return false;
}

//断开oracle数据库连接
bool IO::DisConnectionOracleDB(void)
{
	try
	{
		if(m_pConnection->State)
		{
			m_pConnection->Close();
			
			return true;
		}
	}
	catch(_com_error e)        //捕捉异常
	{
		CString temp;
		temp.Format(_T("错误信息:%s"),e.ErrorMessage());
		//MessageBox(temp, _T("数据库断开连接失败信息提示"));
		return FALSE;
	}	

	return false;
}

bool IO::writeMatchResultVehiclePicture(char *IpAddr,char *CarStr,unsigned char *Image,long int ImageSize)
{
	CString strSql = _T("select seq_match_result_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	int id	= m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select nid, bpicture, scarnumber, sip, nflag from tb_match_result_vehicle", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(CarStr));
		m_pRecordsetPtr->PutCollect("sip", _variant_t(IpAddr));
		m_pRecordsetPtr->PutCollect("nflag", _variant_t(0));
		
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

bool IO::writeMatchResultVehiclePath(char *IpAddr,char *CarStr,char *path)
{
	CString strSql = _T("select seq_match_result_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	int id	= m_pRecordsetPtr->GetCollect("nid");

	m_pRecordsetPtr.CreateInstance(_uuidof(Recordset));

	HRESULT hr = m_pRecordsetPtr->Open("select nid, scarnumber, sip, nflag, sfile from tb_match_result_vehicle", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	if(SUCCEEDED(hr))
	{
		m_pRecordsetPtr->AddNew();
		m_pRecordsetPtr->PutCollect("nid", _variant_t(id));
		m_pRecordsetPtr->PutCollect("scarnumber", _variant_t(CarStr));
		m_pRecordsetPtr->PutCollect("sip", _variant_t(IpAddr));
		m_pRecordsetPtr->PutCollect("nflag", _variant_t(0));
		m_pRecordsetPtr->PutCollect("sfile", _variant_t(path));

		m_pRecordsetPtr->Update();

		m_pRecordsetPtr->Close(); 

		//int result[1];
		int result;
		char error[256];
		ExecuteInsertAlarmVehicleProcedure(id,&result,error);

		return true;
	}
	return false;
}

bool IO::readMatchResultVehiclePath(HISTORY_STRUCT &CarInfo)
{
	CString	 strSql = _T("select nid, sfile, sip, to_char(dcreate),scarnumber from tb_match_result_vehicle where sfile is not null and nflag=0 and rownum<2");
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	int id	= m_pRecordsetPtr->GetCollect("nid");

	CString ipaddr = m_pRecordsetPtr->GetCollect("sip");

	CString carNum = m_pRecordsetPtr->GetCollect("scarnumber");	

	CString filename = m_pRecordsetPtr->GetCollect("sfile");

	//SYSTEMTIME createTime;

	variant_t varcreateTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");

	sscanf(LPCTSTR((CString)varcreateTime.bstrVal),"%d-%d-%d %d:%d:%d",
				   &CarInfo.year,&CarInfo.mon,&CarInfo.day,
				   &CarInfo.hour,&CarInfo.min,&CarInfo.sec);

	sprintf(CarInfo.carNumber,"%s",carNum.GetBuffer());
	sprintf(CarInfo.ip,"%s",ipaddr.GetBuffer());
	sprintf(CarInfo.path,"%s",filename.GetBuffer());

	m_pRecordsetPtr->Close();

	UpdateDataState("tb_match_result_vehicle","nflag",id);

	return true;
}
bool IO::readMatchResultVehicle(CString path,HISTORY_STRUCT &CarInfo)
{
	CString	 strSql = _T("select nid, bpicture, sip, to_char(dcreate),scarnumber from tb_match_result_vehicle where bpicture is not null and nflag=0 and rownum<2");
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	int id	= m_pRecordsetPtr->GetCollect("nid");

	CString ipaddr = m_pRecordsetPtr->GetCollect("sip");

	CString carNum = m_pRecordsetPtr->GetCollect("scarnumber");	

	long lDatasize =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->ActualSize;	  //从数据库获取图片

	_variant_t varBLOB =  m_pRecordsetPtr->GetFields()->GetItem("bpicture")->GetChunk(lDatasize);

	//SYSTEMTIME createTime;

	variant_t varcreateTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");

	sscanf(LPCTSTR((CString)varcreateTime.bstrVal),"%d-%d-%d %d:%d:%d",
				   &CarInfo.year,&CarInfo.mon,&CarInfo.day,
				   &CarInfo.hour,&CarInfo.min,&CarInfo.sec);

	sprintf(CarInfo.carNumber,"%s",carNum.GetBuffer());
	sprintf(CarInfo.ip,"%s",ipaddr.GetBuffer());
	//sscanf(LPCTSTR((CString)varcreateTime.bstrVal),"%d-%d-%d %d:%d:%d",
	//	   &createTime.wYear,&createTime.wMonth,&createTime.wDay,
	//	   &createTime.wHour,&createTime.wMinute,&createTime.wSecond);

	//CTime cTime(createTime);

	//CString str=cTime.Format("%Y-%m-%d %H:%M:%S");

	//str=cTime.Format("%Y-%m-%d-%H-%M-%S");

	CString filename = path;

	filename.AppendFormat(_T("%d-%d-%d-%d-%d-%d %s %s %d "),CarInfo.year,CarInfo.mon,CarInfo.day,CarInfo.hour,CarInfo.min,CarInfo.sec,ipaddr,carNum,lDatasize);

	filename.Append(_T(".jpg"));

	sprintf(CarInfo.path,"%s",filename.GetBuffer());

	CFile outputFile(filename, CFile::modeWrite | CFile::modeCreate);

	if(varBLOB.vt == (VT_ARRAY | VT_UI1))
	{
		char * pbuffer = NULL;
		SafeArrayAccessData(varBLOB.parray,(void **)&pbuffer);
            
		outputFile.Write(pbuffer, lDatasize);

		SafeArrayUnaccessData(varBLOB.parray);
	}

	m_pRecordsetPtr->Close();

	UpdateDataState("tb_match_result_vehicle","nflag",id);

	return true;
}

bool IO::readAlarmVehicle(HISTORY_STRUCT &AlarmInfo)
{
	CString	 strSql = _T("select nid, nvehicleid, scarnumber, to_char(dgettime), sip, sfile from tb_alarm_vehicle where sdealflag=0 and rownum<2 and sfile is not null");
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}

	CString carNum = m_pRecordsetPtr->GetCollect("scarnumber");

	CString ipaddr = m_pRecordsetPtr->GetCollect("sip");

	CString path = m_pRecordsetPtr->GetCollect("sfile");	

	int id	= m_pRecordsetPtr->GetCollect("nid");
	
	int ddcid = m_pRecordsetPtr->GetCollect("nvehicleid");

	variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dgettime)");

	sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
		   &AlarmInfo.year,&AlarmInfo.mon,&AlarmInfo.day,
		   &AlarmInfo.hour,&AlarmInfo.min,&AlarmInfo.sec);

	sprintf(AlarmInfo.carNumber,"%s",carNum.GetBuffer());
	sprintf(AlarmInfo.ip,"%s",ipaddr.GetBuffer());
	sprintf(AlarmInfo.path,"%s",path.GetBuffer());

	//读黑名单信息
	readVehicleInfo(AlarmInfo,ddcid);

	m_pRecordsetPtr->Close();

	UpdateDataState("tb_alarm_vehicle","sdealflag",id);

	return true;
}

bool IO::readAllAlarmVehicle(list<HISTORY_STRUCT>& AlarmList)
{
	CString	 strSql = _T("select nvehicleid, scarnumber, to_char(dgettime), sip, sfile from tb_alarm_vehicle where sdealflag=1 and sfile is not null");
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	CString carNum;
	CString ipaddr;
	CString path;
	int ddcid;

	HISTORY_STRUCT AlarmInfo={0};

	while(!m_pRecordsetPtr->adoEOF)
	{

		carNum = m_pRecordsetPtr->GetCollect("scarnumber");
		ipaddr = m_pRecordsetPtr->GetCollect("sip");
		//CString path = _T("D:\\TEMP");
		path = m_pRecordsetPtr->GetCollect("sfile");	
		ddcid	= m_pRecordsetPtr->GetCollect("nvehicleid");
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dgettime)");

		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &AlarmInfo.year,&AlarmInfo.mon,&AlarmInfo.day,
			   &AlarmInfo.hour,&AlarmInfo.min,&AlarmInfo.sec);
		sprintf(AlarmInfo.carNumber,"%s",carNum.GetBuffer());
		sprintf(AlarmInfo.ip,"%s",ipaddr.GetBuffer());
		sprintf(AlarmInfo.path,"%s",path.GetBuffer());
		//读黑名单信息
		if(!readVehicleInfo(AlarmInfo,ddcid))
		{
			m_pRecordsetPtr->MoveNext();
			continue;
		}

		AlarmList.push_back((HISTORY_STRUCT)AlarmInfo);

		//UpdateDataState("tb_alarm_vehicle","sdealflag",id);//待修改

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	if(AlarmList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::readVehicleInfo(HISTORY_STRUCT &AlarmInfo,int ddcid)
{
	variant_t var; 	
	variant_t temp; 
	temp.ChangeType(VT_NULL);

	//读黑名单信息
	CString	 strSqlInfo;
	strSqlInfo.Format("select pinpai, chejiahao, dianjihao, shizhushengming, shizhulianjidianhua, chepaihao from tb_ddc where id=%d",ddcid);

	m_pRecordsetPtrInfo = m_pConnection->Execute((_bstr_t)strSqlInfo, NULL, adCmdText);

	if(m_pRecordsetPtrInfo->BOF)
	{
		m_pRecordsetPtrInfo->Close();
		return false;
	}

	//车牌号
	var = m_pRecordsetPtrInfo->GetCollect("chepaihao");
	if(var.vt != NULL && var !=temp)
		AlarmInfo.info.ChePaiHao = var.bstrVal;
	else
		AlarmInfo.info.ChePaiHao.Empty();
	//车架号
	var = m_pRecordsetPtrInfo->GetCollect("chejiahao");
	if(var.vt != NULL && var !=temp)
		AlarmInfo.info.CheJiaHao = var.bstrVal;
	else
		AlarmInfo.info.CheJiaHao.Empty();
	//电机号
	var = m_pRecordsetPtrInfo->GetCollect("dianjihao");
	if(var.vt != NULL && var !=temp)
		AlarmInfo.info.DianJiHao = var.bstrVal;
	else
		AlarmInfo.info.DianJiHao.Empty();
	//失主
	var = m_pRecordsetPtrInfo->GetCollect("shizhushengming");
	if(var.vt != NULL && var !=temp)
		AlarmInfo.info.Name = var.bstrVal;
	else
		AlarmInfo.info.Name.Empty();
	//电话
	var = m_pRecordsetPtrInfo->GetCollect("shizhulianjidianhua");
	if(var.vt != NULL && var !=temp)
		AlarmInfo.info.Phone = var.bstrVal;
	else
		AlarmInfo.info.Phone.Empty();
	//品牌
	var = m_pRecordsetPtrInfo->GetCollect("pinpai");
	if(var.vt != NULL && var !=temp)
		AlarmInfo.info.PinPai = var.bstrVal;
	else
		AlarmInfo.info.PinPai.Empty();

	m_pRecordsetPtrInfo->Close();

	return true;
}

bool IO::readAllMatchResultVehicle(list<HISTORY_STRUCT>& MatchResultList)
{
	CString	 strSql = _T("select nid, sip, to_char(dcreate), scarnumber, sfile from tb_match_result_vehicle where nflag=1 and sfile is not null");
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	CString carNum;
	CString ipaddr;
	CString path;
	int id;
	HISTORY_STRUCT AlarmInfo={0};
	while(!m_pRecordsetPtr->adoEOF)
	{
		carNum = m_pRecordsetPtr->GetCollect("scarnumber");
		ipaddr = m_pRecordsetPtr->GetCollect("sip");
		path = m_pRecordsetPtr->GetCollect("sfile");	
		id	= m_pRecordsetPtr->GetCollect("nid");
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");

		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &AlarmInfo.year,&AlarmInfo.mon,&AlarmInfo.day,
			   &AlarmInfo.hour,&AlarmInfo.min,&AlarmInfo.sec);
		sprintf(AlarmInfo.carNumber,"%s",carNum.GetBuffer());
		sprintf(AlarmInfo.ip,"%s",ipaddr.GetBuffer());
		sprintf(AlarmInfo.path,"%s",path.GetBuffer());

		MatchResultList.push_back((HISTORY_STRUCT)AlarmInfo);

		UpdateDataState("tb_match_result_vehicle","nflag",id);

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	if(MatchResultList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::getAlarmVehicleWithTime(char *startTime,char *endTime,list<HISTORY_STRUCT>& AlarmList)
{
	CString strSql;
	strSql.Format("select nvehicleid, scarnumber, to_char(dgettime), sip, sfile from tb_alarm_vehicle where dtime between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss') and sdealflag=1 and sfile is not null",startTime,endTime);
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	CString carNum;
	CString ipaddr;
	CString path;
	int ddcid;
	HISTORY_STRUCT AlarmInfo={0};

	while(!m_pRecordsetPtr->adoEOF)
	{
		carNum = m_pRecordsetPtr->GetCollect("scarnumber");
		ipaddr = m_pRecordsetPtr->GetCollect("sip");
		//CString path = _T("D:\\TEMP");
		path = m_pRecordsetPtr->GetCollect("sfile");	
		ddcid	= m_pRecordsetPtr->GetCollect("nvehicleid");
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dgettime)");

		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &AlarmInfo.year,&AlarmInfo.mon,&AlarmInfo.day,
			   &AlarmInfo.hour,&AlarmInfo.min,&AlarmInfo.sec);
		sprintf(AlarmInfo.carNumber,"%s",carNum.GetBuffer());
		sprintf(AlarmInfo.ip,"%s",ipaddr.GetBuffer());
		sprintf(AlarmInfo.path,"%s",path.GetBuffer());
		//读黑名单信息
		if(!readVehicleInfo(AlarmInfo,ddcid))
		{
			m_pRecordsetPtr->MoveNext();
			continue;
		}

		AlarmList.push_back((HISTORY_STRUCT)AlarmInfo);

		//UpdateDataState("tb_alarm_vehicle","sdealflag",id);//待修改

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	if(AlarmList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::getAlarmVehicleWithCarNum(char *CarNum,list<HISTORY_STRUCT>& AlarmList)
{
	CString strSql;
	strSql.Format("select nvehicleid, scarnumber, to_char(dgettime), sip, sfile from tb_alarm_vehicle where scarnumber like '%%%s%%' and sdealflag=1 and sfile is not null",CarNum);
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	CString carNum;
	CString ipaddr;
	CString path;
	int ddcid;
	HISTORY_STRUCT AlarmInfo={0};

	while(!m_pRecordsetPtr->adoEOF)
	{
		carNum = m_pRecordsetPtr->GetCollect("scarnumber");
		ipaddr = m_pRecordsetPtr->GetCollect("sip");
		//CString path = _T("D:\\TEMP");
		path = m_pRecordsetPtr->GetCollect("sfile");	
		ddcid	= m_pRecordsetPtr->GetCollect("nvehicleid");
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dgettime)");

		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &AlarmInfo.year,&AlarmInfo.mon,&AlarmInfo.day,
			   &AlarmInfo.hour,&AlarmInfo.min,&AlarmInfo.sec);
		sprintf(AlarmInfo.carNumber,"%s",carNum.GetBuffer());
		sprintf(AlarmInfo.ip,"%s",ipaddr.GetBuffer());
		sprintf(AlarmInfo.path,"%s",path.GetBuffer());
		//读黑名单信息
		if(!readVehicleInfo(AlarmInfo,ddcid))
		{
			m_pRecordsetPtr->MoveNext();
			continue;
		}

		AlarmList.push_back((HISTORY_STRUCT)AlarmInfo);

		//UpdateDataState("tb_alarm_vehicle","sdealflag",id);//待修改

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	if(AlarmList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::getMatchResultVehicleWithTime(char *startTime,char *endTime,list<HISTORY_STRUCT>& MatchResultList)
{
	CString strSql;
	strSql.Format("select nid, scarnumber, to_char(dcreate), sip, sfile from tb_match_result_vehicle where dcreate between to_date(%s,'yyyymmddhh24miss') and to_date(%s,'yyyymmddhh24miss')  and nflag=1 and sfile is not null",startTime,endTime);
	
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	CString carNum;
	CString ipaddr;
	CString path;
	int id;
	HISTORY_STRUCT AlarmInfo={0};
	while(!m_pRecordsetPtr->adoEOF)
	{
		carNum = m_pRecordsetPtr->GetCollect("scarnumber");
		ipaddr = m_pRecordsetPtr->GetCollect("sip");
		path = m_pRecordsetPtr->GetCollect("sfile");	
		id	= m_pRecordsetPtr->GetCollect("nid");
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");

		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &AlarmInfo.year,&AlarmInfo.mon,&AlarmInfo.day,
			   &AlarmInfo.hour,&AlarmInfo.min,&AlarmInfo.sec);
		sprintf(AlarmInfo.carNumber,"%s",carNum.GetBuffer());
		sprintf(AlarmInfo.ip,"%s",ipaddr.GetBuffer());
		sprintf(AlarmInfo.path,"%s",path.GetBuffer());

		MatchResultList.push_back((HISTORY_STRUCT)AlarmInfo);

		UpdateDataState("tb_match_result_vehicle","nflag",id);

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	if(MatchResultList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::getMatchResultVehicleWithCarNum(char *CarNum,list<HISTORY_STRUCT>& MatchResultList)
{
	CString strSql;
	strSql.Format("select nid, scarnumber, to_char(dcreate), sip, sfile from tb_match_result_vehicle where scarnumber like '%%%s%%' and nflag=1 and sfile is not null",CarNum);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);		 

	if(m_pRecordsetPtr->BOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	CString carNum;
	CString ipaddr;
	CString path;
	long int id;
	HISTORY_STRUCT AlarmInfo={0};
	while(!m_pRecordsetPtr->adoEOF)
	{
		carNum = m_pRecordsetPtr->GetCollect("scarnumber");
		ipaddr = m_pRecordsetPtr->GetCollect("sip");
		path = m_pRecordsetPtr->GetCollect("sfile");	
		id	= m_pRecordsetPtr->GetCollect("nid");
		variant_t vargetTime = m_pRecordsetPtr->GetCollect("to_char(dcreate)");

		sscanf(LPCTSTR((CString)vargetTime.bstrVal),"%d-%d-%d %d:%d:%d",
			   &AlarmInfo.year,&AlarmInfo.mon,&AlarmInfo.day,
			   &AlarmInfo.hour,&AlarmInfo.min,&AlarmInfo.sec);
		sprintf(AlarmInfo.carNumber,"%s",carNum.GetBuffer());
		sprintf(AlarmInfo.ip,"%s",ipaddr.GetBuffer());
		sprintf(AlarmInfo.path,"%s",path.GetBuffer());

		MatchResultList.push_back((HISTORY_STRUCT)AlarmInfo);

		UpdateDataState("tb_match_result_vehicle","nflag",id);

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	if(MatchResultList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::ExecuteInsertAlarmVehicleProcedure(int nid, int *iresult, char* serror)
{
	_CommandPtr pCommandPtr = NULL;
	pCommandPtr.CreateInstance(_uuidof(Command));

	_ParameterPtr pParameterPtr;
	pParameterPtr = pCommandPtr->CreateParameter(_T("i_nid"), adInteger, adParamInput, sizeof(nid), _variant_t(nid));
	pCommandPtr->Parameters->Append(pParameterPtr);	

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_result"), adInteger, adParamOutput, sizeof(iresult), _variant_t(iresult));	
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_errmessage"), adVarChar, adParamOutput, 256, _variant_t(serror));
	pCommandPtr->Parameters->Append(pParameterPtr);
	
	pCommandPtr->CommandText = "pro_auto_match_vehicle";
	pCommandPtr->CommandType = adCmdStoredProc;	 

	pCommandPtr->ActiveConnection =	m_pConnection;

	pCommandPtr->Execute(NULL, NULL, adCmdStoredProc);	

	*iresult = pCommandPtr->Parameters->GetItem("o_result")->GetValue();
	if(*iresult == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// 更新数据记录状态
bool IO::UpdateDataState(char *table,char *str,int ID)
{
	CString insertsql;

	//insertsql.Format(_T("update tb_temp_electric_vehicle set nflag = 1 where nid = %d"), ID);
	//2013-01-06市局电动车演示
	insertsql.Format(_T("update %s set %s = 1 where nid = %d"), table, str, ID);

	_variant_t RecordAffected;

	m_pConnection->Execute((_bstr_t)insertsql, &RecordAffected, adCmdText);	  
	
	if(RecordAffected.uintVal == 1)	   //判断是否更新成功
	{
		return	true;
	}
	else
	{
		return false;
	}
}
/*
bool IO::updateDataState(CString stablename,int nflag,int nid)
{
	CString insertsql;

	insertsql.Format(_T("update %s set nflag = %d where nid = %d"),stablename,nflag,nid);

	_variant_t RecordAffected;

	m_pConnection->Execute((_bstr_t)insertsql, &RecordAffected, adCmdText);	  
	
	if(RecordAffected.uintVal == 1)	   //判断是否更新成功
	{
		return	true;
	}
	else
	{
		return false;
	}
}
*/
