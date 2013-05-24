/***********************************************************************************
//ver :YiRongCarDetectAIO
//time:2013-03-29 11:39:00
***********************************************************************************/
#include "stdafx.h"
#include "IO.h"

IO::IO(void)
{
#if YRVM_PINGTAI_MODE
	YRVM_state=false;
#endif

	IOwriteLock=false;

}

IO::~IO(void)
{

#if YRVM_PINGTAI_MODE
   YRVM_DisConnectionOracleDB();
#endif
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
	while(IOwriteLock)
	{
		Sleep(100);
	}

	IOwriteLock=true;

	unsigned long int id=0;

	CString strSql = _T("select seq_temp_photo_vehicle.nextval as nid from dual");	    //获取电动车图像数据nid

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	id = (long)m_pRecordsetPtr->GetCollect("nid");

	IOwriteLock=false;
	return id;
}

// 写电动车临时照片表
bool IO::YRVM_writeElectricCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id)
{
	while(IOwriteLock)
	{
		Sleep(100);
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

// 写临时电动车信息表
bool IO::YRVM_writeElectricCarTempInfoToOracleDB(char *CarStr,char* Color,int Direction,char *Reliability,unsigned long int id)
{
	while(IOwriteLock)
	{
		Sleep(100);
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

// 获取数据库临时机动车表的nid
unsigned long int IO::YRVM_getCarOracleTempNid(void)
{
	while(IOwriteLock)
	{
		Sleep(100);
	}

	IOwriteLock=true;

	unsigned long int id;

	CString strSql = _T("select seq_temp_photo_car.nextval as nid from dual");	    //获取临时车辆图像数据nid

	m_pRecordsetPtr = YRVM_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	id	=(long) m_pRecordsetPtr->GetCollect("nid");

	IOwriteLock=false;
	return id;
}

// 写机动车临时照片表
bool IO::YRVM_writeCarTempPhotoToOracleDB(char *IpAddr,unsigned char *Image,long int ImageSize,unsigned long int id)
{
	while(IOwriteLock)
	{
		Sleep(100);
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

// 写临时机动车信息表
bool IO::YRVM_writeCarTempInfoToOracleDB(char *CarStr,char* PlateColor,int Direction,char *Reliability,char* Type,char *CarColor,unsigned long int id)
{
	while(IOwriteLock)
	{
		Sleep(100);
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