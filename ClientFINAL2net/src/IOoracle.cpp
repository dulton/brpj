#include "StdAfx.h"
#include "IOoracle.h"
#include "ReadConfigTxt.h"

IOoracle::IOoracle(void)
{
	
	AfxOleInit(); 
	CoInitialize(NULL);
}

IOoracle::~IOoracle(void)
{
	DisConnectionOracleDB();
}

//连接oracle数据库
int IOoracle::ConnectionOracleDB(TCHAR*  FileName)
{
	HRESULT hr;

	ReadConfigTxt pConfig;

	if(!pConfig.ReadFile(FileName))
	{
		return YIRONGDB_ReadFile_FAIL;
	}

	CString CstrConn;

	CstrConn.Format(_T("Provider=OraOLEDB.Oracle.1;User ID=%s;Password=%s;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));Persist Security Info=False"),\
		pConfig.User, pConfig.Psw, pConfig.Ip, pConfig.Port, pConfig.DataBaseName);

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
			return YIRONGDB_ContOpen_FAIL;
		}

		return YIRONGDB_Connectd_DONE;
	}
	else
	{
		//AfxMessageBox("Create ADODB Connection Instance Failed.");
		return YIRONGDB_Instance_FAIL;
	}
}
//断开oracle数据库连接
bool IOoracle::DisConnectionOracleDB(void)
{
	try
	{
		if(m_pConnection->GetState())
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


// 从oracle数据库中读取T_ORG
void IOoracle::ReadInfoFromOracleDB_TORG(CString str, vector<CString> &id,vector<CString> &name)
{
	variant_t RecordAffected;
	_RecordsetPtr m_pRecordsetPtr;			 //创建数据记录集
	
	CString tempstr;
/*
	tempstr.Format(_T("select sorgid,sorgname from T_ORG where sorgid LIKE '%s__'"), str);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)tempstr, &RecordAffected, adCmdText);

	if(!m_pRecordsetPtr->BOF)
	{
		m_pRecordsetPtr->MoveFirst();
	}

	variant_t var; 	
	variant_t temp; 
	temp.ChangeType(VT_NULL);

	while(!m_pRecordsetPtr->adoEOF)
	{
		var = m_pRecordsetPtr->GetCollect("sorgid");	         
		if(var.vt != NULL && var !=temp)
			id.push_back(var.bstrVal);
		else
			id.push_back("");

		var = m_pRecordsetPtr->GetCollect("sorgname");	          
		if(var.vt != NULL && var !=temp)
			name.push_back(var.bstrVal);
		else
			name.push_back("");

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
*/
	tempstr.Format(_T("select sorgno,sorgname from T_ZD_ORG where sorgno LIKE '%s__'"), str);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)tempstr, &RecordAffected, adCmdText);

	if(!m_pRecordsetPtr->BOF)
	{
		m_pRecordsetPtr->MoveFirst();
	}

	variant_t var; 	
	variant_t temp; 
	temp.ChangeType(VT_NULL);

	while(!m_pRecordsetPtr->adoEOF)
	{
		var = m_pRecordsetPtr->GetCollect("sorgno");	         
		if(var.vt != NULL && var !=temp)
			id.push_back(var.bstrVal);
		else
			id.push_back("");

		var = m_pRecordsetPtr->GetCollect("sorgname");	          
		if(var.vt != NULL && var !=temp)
			name.push_back(var.bstrVal);
		else
			name.push_back("");

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

}

// 从oracle数据库中读取TB_CAMERA_INFO
void IOoracle::ReadInfoFromOracleDB_TBCAMERAINFO(CString str,vector<int> &id, vector<CString> &live,vector<CString> &ip,
	vector<CString> &user, vector<CString> &pwd,vector<CString> &channel,vector<CString> &port)
{
	_RecordsetPtr m_pRecordsetPtr;			 //创建数据记录集

	CString tempstr;
	//tempstr.Format(_T("select objectid,slivingareasname,sip from TB_CAMERA_INFO where spolicestation='%s'"), str);
	tempstr.Format(_T("select objectid,slivingareasname,sip,suser,spwd,schannel,sport from TB_CAMERA_INFO where spolicestation='%s'"), str);

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)tempstr, NULL, adCmdText);

	if(!m_pRecordsetPtr->BOF)
	{
		m_pRecordsetPtr->MoveFirst();
	}

	variant_t var; 	
	variant_t temp; 
	temp.ChangeType(VT_NULL);

	while(!m_pRecordsetPtr->adoEOF)
	{
		var = m_pRecordsetPtr->GetCollect("objectid");	         
		if(var.vt != NULL && var !=temp)
			id.push_back(var.lVal);
		else
			id.push_back(0);
		
		var = m_pRecordsetPtr->GetCollect("slivingareasname");	          
		if(var.vt != NULL && var !=temp)
			live.push_back(var.bstrVal);
		else
			live.push_back("");

		var = m_pRecordsetPtr->GetCollect("sip");	          
		if(var.vt != NULL && var !=temp)
			ip.push_back(var.bstrVal);
		else
			ip.push_back("");

		var = m_pRecordsetPtr->GetCollect("suser");	          
		if(var.vt != NULL && var !=temp)
			user.push_back(var.bstrVal);
		else
			user.push_back("888888");
		
		var = m_pRecordsetPtr->GetCollect("spwd");	          
		if(var.vt != NULL && var !=temp )
			pwd.push_back(var.bstrVal);
		else
			pwd.push_back("888888");

		var = m_pRecordsetPtr->GetCollect("sport");	          
		if(var.vt != NULL && var !=temp)
			port.push_back(var.bstrVal);
		else
			port.push_back("3000");

		var = m_pRecordsetPtr->GetCollect("schannel");	          
		if(var.vt != NULL && var !=temp)
			channel.push_back(var.bstrVal);
		else
			channel.push_back("0");

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
}

//获取系统时间
BOOL IOoracle::GetSysdateFromOracleDB(char *str,
									  int *year,int *mon,int *day,
									  int *hour,int *min,int *sec)
{
	BOOL r=0;
	_RecordsetPtr m_pRecordsetPtr;			 //创建数据记录集
	m_pRecordsetPtr =m_pConnection->Execute("select to_char(sysdate) from dual", NULL, adCmdText);

	if(!m_pRecordsetPtr->BOF)
	{
		m_pRecordsetPtr->MoveFirst();
	}

	variant_t var; 	

	while(!m_pRecordsetPtr->adoEOF)
	{
		var = m_pRecordsetPtr->GetCollect("to_char(sysdate)");	 
       	if(var.vt != NULL)
		{
			sscanf((LPCTSTR)(CString)var.bstrVal,"%d-%d-%d %d:%d:%d",
				year,mon,day,hour,min,sec);
			sprintf(str,"%04d-%02d-%02d_%02d-%02d-%02d",*year,*mon,*day,
									  *hour,*min,*sec);
			r=TRUE;
		}
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	return r;
}

//获取系统时间
BOOL IOoracle::GetSysdateFromOracleDB_DATE(int *year,int *mon,int *day,
									  int *hour,int *min,int *sec)
{
	BOOL r=0;
	_RecordsetPtr m_pRecordsetPtr;			 //创建数据记录集
	m_pRecordsetPtr =m_pConnection->Execute("select to_char(sysdate) from dual", NULL, adCmdText);

	if(!m_pRecordsetPtr->BOF)
	{
		m_pRecordsetPtr->MoveFirst();
	}

	variant_t var; 	

	while(!m_pRecordsetPtr->adoEOF)
	{
		var = m_pRecordsetPtr->GetCollect("to_char(sysdate)");	 
       	if(var.vt != NULL)
		{
			sscanf((LPCTSTR)(CString)var.bstrVal,"%d-%d-%d %d:%d:%d",
				year,mon,day,hour,min,sec);
			r=TRUE;
		}
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();

	return r;
}

//获取当前比对结果中最大比对得分插入比对结果表中的ID
long int IOoracle::GetNID_VIDEO(void)
{
	_RecordsetPtr m_pRecordsetPtr;	
	CString strSql = _T("select SEQ_VIDEO_INFO.nextval as nid from dual");

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);	

	if(m_pRecordsetPtr->adoEOF)
	{
		m_pRecordsetPtr->Close(); 
		return 0;
	}

	long int i=0;
	i=m_pRecordsetPtr->GetCollect("nid");
	m_pRecordsetPtr->Close(); 
	return  i;
}

//写入TB_VIDEO_INFO
void IOoracle::WriteInfoFromOracleDB_TBVIDEOINFO(long int camid,	\
												 const char *path,const char *filename,	\
												 CTime stime,long int size,char *type)
{
	CString str;
	long int v;

	 //获取时间 0星期天 1星期一 6星期六
	int year, mon, day, hour, min,sec;
		
	//获取时间
	if(! GetSysdateFromOracleDB_DATE(&year, &mon, &day, &hour, &min,&sec))
		return ;

	//获取TB_VIDEO_INFO 总条数
	v=GetNID_VIDEO();
	
	str.Format(_T("insert into TB_VIDEO_INFO(nid,ncamera,sfilesize,slocation,sformat,sfilename,dstarttime,dendtime) \
		values(%d,%d,'%d','%s','%s','%s',\
		to_date('%04d%02d%02d %02d%02d%02d','yyyymmdd hh24miss'),to_date('%04d%02d%02d %02d%02d%02d','yyyymmdd hh24miss'))"),
		v,camid,
		size,
		path,
		type,
		filename,
		stime.GetYear(),
		stime.GetMonth(),
		stime.GetDay(),
		stime.GetHour(),
		stime.GetMinute(),
		stime.GetSecond(),
		year, mon, day, hour, min,sec);

	m_pConnection->Execute((_bstr_t)str, NULL, adCmdText);

}

//写入TB_VIDEO_INFO
void IOoracle::WriteInfoFromOracleDB_NETVIDEOINFO(long int camid,	\
												 const char *path,const char *filename,	\
												 CTime stime,CTime etime,long int size,char *type)
{
	CString str;
	long int v;
	
	//获取TB_VIDEO_INFO 总条数
	v=GetNID_VIDEO();
	
	str.Format(_T("insert into TB_VIDEO_INFO(nid,ncamera,sfilesize,slocation,sformat,sfilename,dstarttime,dendtime) \
		values(%d,%d,'%d','%s','%s','%s',	\
		to_date('%04d%02d%02d %02d%02d%02d','yyyymmdd hh24miss'),	\
		to_date('%04d%02d%02d %02d%02d%02d','yyyymmdd hh24miss'))"),	\
		v,camid,	\
		size,	\
		path,	\
		type,
		filename,	\
		stime.GetYear(),	\
		stime.GetMonth(),	\
		stime.GetDay(),	\
		stime.GetHour(),	\
		stime.GetMinute(),	\
		stime.GetSecond(),	\
		etime.GetYear(),	\
		etime.GetMonth(),	\
		etime.GetDay(),	\
		etime.GetHour(),	\
		etime.GetMinute(),	\
		etime.GetSecond());

	m_pConnection->Execute((_bstr_t)str, NULL, adCmdText);

	//调用存储过程
	VIDEO_PRO(v);
}
//数据库存储过程
void IOoracle::VIDEO_PRO(long int nid)
{
	_CommandPtr pCommandPtr = NULL;
	pCommandPtr.CreateInstance(_uuidof(Command));

	_ParameterPtr pParameterPtr;
	long int out_temp=0;

	TCHAR temp_errorMSG[260];

	pParameterPtr = pCommandPtr->CreateParameter(_T("i_nid"), adInteger, adParamInput, sizeof(long int), _variant_t(nid));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pParameterPtr = pCommandPtr->CreateParameter(_T("o_result"), adInteger, adParamOutput, sizeof(long int), _variant_t(out_temp));	
	pCommandPtr->Parameters->Append(pParameterPtr);
 
	pParameterPtr = pCommandPtr->CreateParameter(_T("o_errmessage"), adVarChar, adParamOutput, _tcslen(temp_errorMSG), _variant_t(temp_errorMSG));
	pCommandPtr->Parameters->Append(pParameterPtr);

	pCommandPtr->CommandText = "pro_flag_videoid";
	pCommandPtr->CommandType = adCmdStoredProc;	

	pCommandPtr->ActiveConnection =	m_pConnection;

	pCommandPtr->Execute(NULL, NULL, adCmdStoredProc);	

}
