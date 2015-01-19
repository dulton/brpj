#include "stdafx.h"
#include "Web90server.h"

#define WEB_KAKOU_DEBUG 0

void ReadFromFile(char**data,char* pfile)
{
	FILE* fp = fopen(pfile,"rb");
	if(fp)
	{
		fseek(fp,0,SEEK_END);
		unsigned long len=ftell(fp);
		*data=(char *)calloc(len,1);
		fseek(fp,0,SEEK_SET);

		fread(*data,len,1,fp);
		fclose(fp);
	}
	else
	{
		printf("open fail--\r\n");
	}
}

void WriteToFileAB(string buffer,char* pfile)
{
	FILE* fp = fopen(pfile,"ab");
	if(fp)
	{
		fwrite(buffer.data(),1,buffer.size(),fp);
		fclose(fp);
	}
	else
	{
		printf("open fail--\r\n");
	}
}

void WriteToFileWB(string buffer,char* pfile)
{
	FILE* fp = fopen(pfile,"wb");
	if(fp)
	{
		fwrite(buffer.data(),1,buffer.size(),fp);
		fclose(fp);
	}
	else
	{
		printf("open fail--\r\n");
	}
}

/////////////////////////////////////////////////////////////////////////////

void CHAR2UTF8Lite(const char *str,char *utf8)
{
	wchar_t wbufflite[XMLRW_MAX_WSTR];
	// int buffLen = 0;  
	MultiByteToWideChar(CP_ACP, 0, str, -1, wbufflite, XMLRW_MAX_WSTR);  
	// buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);  
	//  utf8 = new char[buffLen+1];  
	WideCharToMultiByte(CP_UTF8, 0, wbufflite, -1, (LPSTR)utf8, XMLRW_MAX_STR, 0, 0);  
}

void UTF82CHARLite(const char *utf8,char *str)
{
	wchar_t wbufflite[XMLRW_MAX_WSTR];
	//	int buffLen = 0;  
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wbufflite, XMLRW_MAX_WSTR);  
	//	buffLen = WideCharToMultiByte(CP_ACP, 0, wbuff, -1, NULL, 0, 0, 0);  
	//	m_gb2312 = new char[buffLen+1];  
	WideCharToMultiByte(CP_ACP, 0, wbufflite, -1, (LPSTR)str, XMLRW_MAX_STR, 0, 0);  
}

void CHAR2UTF8(const char *str,char *utf8,unsigned long int len)
{
	wchar_t * wbuff=(wchar_t *)calloc(len*2+4/*+4*/,sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, len+2);  
	// buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);  
	//  utf8 = new char[buffLen+1];  
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)utf8, len*2+4, 0, 0); 
	free(wbuff);
	wbuff=NULL;
}

void UTF82CHAR(const char *utf8,char *str,unsigned long int len)
{
	wchar_t * wbuff=(wchar_t *)calloc(len*2+4,sizeof(wchar_t));
	//	int buffLen = 0;  
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wbuff, len+2);  
	//	buffLen = WideCharToMultiByte(CP_ACP, 0, wbuff, -1, NULL, 0, 0, 0);  
	//	m_gb2312 = new char[buffLen+1];  
	WideCharToMultiByte(CP_ACP, 0, wbuff, -1, (LPSTR)str, len*2+4, 0, 0);  
	free(wbuff);
	wbuff=NULL;
}


/*!
*  \brief 通过根节点和节点名获取节点指针。
*
*  \param pRootEle   xml文件的根节点。
*  \param strNodeName  要查询的节点名
*  \param Node      需要查询的节点指针
*  \return 是否找到。true为找到相应节点指针，false表示没有找到相应节点指针。
*/
bool ReadGetNodePointerByName(TiXmlElement* pRootEle,string strNodeName,TiXmlElement* &Node)
{
	// 假如等于根节点名，就退出
	if (strNodeName==pRootEle->Value())
	{
		Node = pRootEle;
		return true;
	}
	TiXmlElement* pEle = pRootEle;  
	for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())  
	{  
		//递归处理子节点，获取节点指针
		if(ReadGetNodePointerByName(pEle,strNodeName,Node))
			return true;
	}  

	return false;
}
/*!
*  \brief 通过节点查询。
*
*  \param XmlFile   xml文件全路径。
*  \param strNodeName  要查询的节点名 
*  \param strText      要查询的节点文本
*  \return 是否成功。true为成功，false表示失败。
//原始节点pRootEle
//找到的节点FindNode
*/
bool ReadQueryNodeText(TiXmlElement *pRootEle,TiXmlElement* &FindNode,string strNodeName,char *strText,unsigned long int len,bool utfflag)
{
	bool res=ReadGetNodePointerByName(pRootEle,strNodeName,FindNode);
	if (NULL!=FindNode && true==res)
	{
		const char* psz = FindNode->GetText(); 

		if (NULL==psz)
			strText[0] =0;
		else	//转码
		{
			if(utfflag)
				UTF82CHAR(psz,strText,len);
			else
				strcpy(strText,psz);
		}
		return true;
	}
	else
	{
		strText[0] =0;
		return false;
	}
}


bool CreateXMLDataUTF8(TiXmlElement *prow,string attr,const char *valuedata,char *strText,unsigned long int len,bool cdataflag)
{
	// 生成子节点：data  
	TiXmlElement *pdata = new TiXmlElement(attr);  
	if (NULL==pdata)  
	{  
		return false;  
	}  
	prow->LinkEndChild(pdata);  
	//转码
	CHAR2UTF8(valuedata,strText,len);
	//添加
	TiXmlText *pdataValue = new TiXmlText(strText);  
	pdata->LinkEndChild(pdataValue); 
	//有加CDATA标记
	pdataValue->SetCDATA(cdataflag); 
	return true;
}

bool CreateXMLData(TiXmlElement *prow,string attr,const char *valuedata,bool cdataflag)
{
	// 生成子节点：data  
	TiXmlElement *pdata = new TiXmlElement(attr);  
	if (NULL==pdata)  
	{  
		return false;  
	}  
	prow->LinkEndChild(pdata);  
	//添加
	TiXmlText *pdataValue = new TiXmlText(valuedata);  
	pdata->LinkEndChild(pdataValue); 
	pdataValue->SetCDATA(cdataflag); 
	return true;
}
/*
<?xml version="1.0" encoding="UTF-8"?>
<soap:Envelope xmlns:soap="http://www.w3.org/2003/05/soap-envelope" xmlns:ser="http://service.thirdBayonet.webservice.bms.hikvision.com">
<soap:Header/>
<soap:Body>
<ser:initSystem>
<ser:xml>

<![CDATA[

<loginParam> 
<cmsUrl>35.24.13.26</cmsUrl> 
<userName>admin</userName> 
<passwd>fzga12345</passwd> 
</loginParam>

]]>


</ser:xml>
</ser:initSystem>
</soap:Body>
</soap:Envelope>
*/
bool CreateXml(char *methodname,const char *valuedata,
			   char *strText,unsigned long int len)  
{  
	// 定义一个TiXmlDocument类指针  
	TiXmlDocument pDoc;  
/*
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","utf-8","");  
	if (NULL==pDeclaration)  
	{  
		return false;  
	}  

	pDoc.LinkEndChild(pDeclaration);  
	*/
	// 生成一个根节点：pRootEle 
	TiXmlElement *pRootEle = new TiXmlElement("soap:Envelope");  
	if (NULL==pRootEle)  
	{  
		return false;  
	}  
	pDoc.LinkEndChild(pRootEle);  
	pRootEle->SetAttribute("xmlns:soap","http://www.w3.org/2003/05/soap-envelope");
	pRootEle->SetAttribute("xmlns:ser", "http://service.thirdBayonet.webservice.bms.hikvision.com");

	// 生成子节点：pHeader
	TiXmlElement *pHeader = new TiXmlElement("soap:Header"); 
	if (NULL==pHeader)  
	{  
		return false;  
	}  
	pRootEle->LinkEndChild(pHeader);  

	// 生成子节点：pbody
	TiXmlElement *pbody = new TiXmlElement("soap:Body"); 
	if (NULL==pbody)  
	{  
		return false;  
	}  
	pRootEle->LinkEndChild(pbody);  


	// 生成子节点：row
	TiXmlElement *pmethod = new TiXmlElement(methodname);  
	if (NULL==pmethod)  
	{  
		return false;  
	}  
	pbody->LinkEndChild(pmethod);  

	//特殊 由于 传入的字符已经是UTF8 这里不用再转
	if(!CreateXMLData(pmethod,
		"ser:xml",
		valuedata,
		true))
		return false;

	//保存到字符串
	TiXmlPrinter printer;
	pDoc.Accept(&printer);
	//	strResult.Format("%s", printer.CStr());
	strcpy(strText,printer.CStr());

	if(WEB_KAKOU_DEBUG)
	{
		printf("xml=\n%s\n",strText);
		//保存文件
		pDoc.SaveFile("d:\\1.xml");
	}
	return true;  
}   

//插入CDATA数据  不带子树
bool CreateXmlLite(char *name,struct NAME_VALUE_S mapdata[],int maplen,
							  char *strText,unsigned long int len)  
{  
	// 定义一个TiXmlDocument类指针  
	TiXmlDocument pDoc;  
/*
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","utf-8","");  
	if (NULL==pDeclaration)  
	{  
		return false;  
	}  

	pDoc.LinkEndChild(pDeclaration);  
*/
	// 生成一个根节点：pRootEle 
	TiXmlElement *pRootEle = new TiXmlElement(name);  
	if (NULL==pRootEle)  
	{  
		return false;  
	}  
	pDoc.LinkEndChild(pRootEle);  

	int it;
	for(it=0;it<maplen;it++)
	{
		
		if(!CreateXMLDataUTF8(pRootEle,
			mapdata[it].namestr.c_str(),
			mapdata[it].valuestr.c_str(),
			strText,len,
			false))
			return false;
	}

	//保存文件
	//pDoc.SaveFile("d:\\1.xml");

	//保存到字符串
	TiXmlPrinter printer;
	pDoc.Accept(&printer);
	//	strResult.Format("%s", printer.CStr());
	strcpy(strText,printer.CStr());

	if(WEB_KAKOU_DEBUG)
		printf("xml=\n%s\n",strText);

	return true;  
}   

//插入CDATA数据  带一个子树 只能加一个 为插入卡口定制
bool CreateXmlLite_Cross(char *name,struct NAME_VALUE_S mapdata[],int maplen,
						   char *subname, char *sub2name,struct NAME_VALUE_S subdata[],int sublen,
						  char *strText,unsigned long int len)  
{  
	// 定义一个TiXmlDocument类指针  
	TiXmlDocument pDoc;  
/*
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","utf-8","");  
	if (NULL==pDeclaration)  
	{  
		return false;  
	}  

	pDoc.LinkEndChild(pDeclaration);  
	*/
	// 生成一个根节点：pRootEle 
	TiXmlElement *pRootEle = new TiXmlElement(name);  
	if (NULL==pRootEle)  
	{  
		return false;  
	}  
	pDoc.LinkEndChild(pRootEle);  

	int it;
	for(it=0;it<maplen;it++)
	{
		if(!CreateXMLDataUTF8(pRootEle,
			mapdata[it].namestr.c_str(),
			mapdata[it].valuestr.c_str(),
			strText,len,
			false))
			return false;
	}

	// 生成一个根节点：pRootEle 
	TiXmlElement *pSubEle = new TiXmlElement(subname);  
	if (NULL==pSubEle)  
	{  
		return false;  
	}  
	pRootEle->LinkEndChild(pSubEle);  

	// 生成一个根节点：pRootEle 
	TiXmlElement *pSub2Ele = new TiXmlElement(sub2name);  
	if (NULL==pSub2Ele)  
	{  
		return false;  
	}  
	pSubEle->LinkEndChild(pSub2Ele);  

	for(it=0;it<sublen;it++)
	{
		if(!CreateXMLDataUTF8(pSub2Ele,
			subdata[it].namestr.c_str(),
			subdata[it].valuestr.c_str(),
			strText,len,
			false))
			return false;
	}

	//保存文件
	//pDoc.SaveFile("d:\\1.xml");

	//保存到字符串
	TiXmlPrinter printer;
	pDoc.Accept(&printer);
	//	strResult.Format("%s", printer.CStr());
	strcpy(strText,printer.CStr());

	if(WEB_KAKOU_DEBUG)
		printf("xml=\n%s\n",strText);

	return true;  
}  

//插入CDATA数据  不带子树
bool CreateXmlLite_Vehicle(char *name,bool isPicUrl,struct NAME_VALUE_S mapdata[],int maplen,
				   char *strText,unsigned long int len)  
{  
	// 定义一个TiXmlDocument类指针  
	TiXmlDocument pDoc;  
/*
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","utf-8","");  
	if (NULL==pDeclaration)  
	{  
		return false;  
	}  
	pDoc.LinkEndChild(pDeclaration);  
	*/
	// 生成一个根节点：pRootEle 
	TiXmlElement *pRootEle = new TiXmlElement(name);  
	if (NULL==pRootEle)  
	{  
		return false;  
	}  
	pDoc.LinkEndChild(pRootEle);  


	/*
	//标准写法
	if(isPicUrl)
		pRootEle->SetAttribute("isPicUrl",1);
	else
		pRootEle->SetAttribute("isPicUrl",0);
*/

//规避写法
//tinyxml.CPP和.H添加
/*
void TiXmlElement::SetAttribute( const char * name )
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
}
#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute( const std::string& name)
{	
	TiXmlAttribute* attrib = attributeSet.FindOrCreate( name );
}
#endif

*/
	if(isPicUrl)
		pRootEle->SetAttribute("isPicUrl=1");
	else
		pRootEle->SetAttribute("isPicUrl=0");


	int it;
	for(it=0;it<maplen;it++)
	{
		if(!CreateXMLDataUTF8(pRootEle,
			mapdata[it].namestr.c_str(),
			mapdata[it].valuestr.c_str(),
			strText,len,
			false))
			return false;
	}

	//保存文件
	//pDoc.SaveFile("d:\\1.xml");

	//保存到字符串
	TiXmlPrinter printer;
	pDoc.Accept(&printer);
	//	strResult.Format("%s", printer.CStr());
	strcpy(strText,printer.CStr());

	if(WEB_KAKOU_DEBUG)
		printf("xml=\n%s\n",strText);

	return true;  
}   
//报文解析
bool XMLgetReturnData(const char *utf8xmlin,char*utf8xmlout)
{
	//放在栈里 内存不会泄露
	TiXmlDocument myDocument;
	myDocument.Parse(utf8xmlin);

	TiXmlElement *pRootEle = myDocument.RootElement();
	if (NULL==pRootEle)
	{
		printf("XMLgetReturnData XML出错\n");
		return false;
	}

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	for(pNodeRow = pRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
	{
		pNodeData = NULL;
		if(ReadQueryNodeText(pNodeRow,pNodeData,"bms1:return",utf8xmlout,XMLRW_MAX_STR,false))
		{
			//printf("bms1:return=%s\n",returnstr);
			break ;
		}
	}
	return true;
}


bool SendSoap_InitSystem(char *wsdlUrl,char *ip,char *username,char *psw,
						 char *sessionIdstr,
					  char *failstr)
{
	Service CService;

	long xmlstrlen=5120;

	char xmlstr[5120];
	char xmllitestr[5120];

	char soapactionstr[512]="Content-Type: application/soap+xml;charset=UTF-8;action=\"urn:initSystem\"";
/*
	if(false==CreateXmlLite_initSystem(ip,username,psw,
		xmllitestr,51200))
	{
		strcpy(failstr,"SendPidPushSoap 无法创建XML lite文件");
		printf("SendPidPushSoap 无法创建XML lite文件\n");

		return false;
	}
*/
	/*
	map<string ,string> mapdata;
	mapdata.insert(pair<string,string>("cmsUrl",ip));
	mapdata.insert(pair<string,string>("userName",username));
	mapdata.insert(pair<string,string>("passwd",psw));
*/

 

	struct NAME_VALUE_S mapdata[3];
	mapdata[0].i("cmsUrl",ip);
	mapdata[1].i("userName",username);
	mapdata[2].i("passwd",psw);

	if(false==CreateXmlLite("loginParam",mapdata,3,
		xmllitestr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_InitSystem 无法创建XML lite文件");
		//printf("SendSoap_InitSystem 无法创建XML lite文件\n");

		return false;
	}

	if(false==CreateXml("ser:initSystem",
		xmllitestr,
		xmlstr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_InitSystem 无法创建XML文件");
		//printf("SendSoap_InitSystem 无法创建XML文件\n");

		return false;
	}

	CService.InitData();

	int nRes = CService.PostSoap(wsdlUrl,soapactionstr,xmlstr,failstr);

	if(0!=nRes)
	{
		strcat(failstr," SendInitSystemSoap POST出错 卡口服务未启动");
	//	printf("SendInitSystemSoap POST出错\n");
		CService.DestroyData();
		return false;
	}
	
	if(WEB_KAKOU_DEBUG)
		printf("\nSendInitSystemSoap resp_buffer=%s\n",CService.m_resp_buffer.c_str());

/*
	if(NULL!=strstr(CService.m_resp_buffer.c_str(),"<ns:return>"))
	{
		strcat(failstr," SendPidPushSoap 返回报文不为0");
		CService.DestroyData();
		return false;
	}
	*/
/*
	//出来默认UTF-8了
	char* charstr=(char *)calloc(CService.m_resp_buffer.length()*2+4,sizeof(char));

	UTF82CHAR(CService.m_resp_buffer.c_str(),charstr,CService.m_resp_buffer.length()*2);

	//防止报文错误 找左起第一个<符合传下去进行解析
	const char *presp=strchr(charstr,'<');
	if(NULL== presp)
	{
		strcpy(failstr,"SendInitSystemSoap XML出错 找不到<");
		printf("SendInitSystemSoap XML出错 找不到<\n");
		free(charstr);
		CService.DestroyData();
		return false;
	}

	char* utf8=(char *)calloc(strlen(presp)*2+4,sizeof(char));
	
	CHAR2UTF8(presp,utf8,strlen(presp)*2);
*/
	char returnstr[XMLRW_MAX_STR]="";

	if(false == XMLgetReturnData(CService.m_resp_buffer.c_str(),returnstr))
	{
		strcat(failstr," SendInitSystemSoap XMLgetReturnDataT出错");
		CService.DestroyData();
		return false;
	}
	// 可以清空了
	CService.DestroyData();

	//放在栈里 内存不会泄露
	TiXmlDocument returnDocument;
	returnDocument.Parse(returnstr);

	TiXmlElement *preturnRootEle = returnDocument.RootElement();
	if (NULL==preturnRootEle)
	{
		strcpy(failstr,"SendInitSystemSoap XMLreturn出错");
		//printf("SendInitSystemSoap XMLreturn出错\n");
		return false;
	}

	char codestr[32]="";
	char messagestr[256]="";
	char sessionstr[128]="";

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	for(pNodeRow = preturnRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
	{
		pNodeData = NULL;
		if(ReadQueryNodeText(pNodeRow,pNodeData,"code",codestr,XMLRW_MAX_STR,true))
		{
			//printf("code=%s\n",codestr);
			NULL;
		}
		if(ReadQueryNodeText(pNodeRow,pNodeData,"message",messagestr,XMLRW_MAX_STR,true))
		{
		//	printf("message=%s\n",messagestr);
			NULL;
		}
		if(ReadQueryNodeText(pNodeRow,pNodeData,"sessionId",sessionstr,XMLRW_MAX_STR,true))
		{
			//printf("sessionId=%s\n",sessionstr);
			if(strlen(sessionstr)>0)
			{
				strcpy(sessionIdstr,sessionstr);
			}
		}
	}

	if (0!=atol(codestr))
	{
		sprintf(failstr,"卡口系统SendInitSystemSoap XMLmessage出错 %s",messagestr);
		return false;
	}

	return true;

}




bool SendSoap_insertCrossingInfo(char *wsdlUrl,
								 char *id,char *index_code,
								 char *crossindex,char *crossname,char *longi, char *lati,
								 char *crossIdstr,
								char *failstr)
{
	Service CService;


	long xmlstrlen=5120;

	char xmlstr[5120];
	char xmllitestr[5120];

	char soapactionstr[512]="Content-Type: application/soap+xml;charset=UTF-8;action=\"urn:insertCrossingInfo\"";
	/*
	map<string ,string> mapdata;

	mapdata.insert(pair<string,string>("sessionId",id));
	mapdata.insert(pair<string,string>("crossingIndex",crossindex));
	mapdata.insert(pair<string,string>("crossingName",crossname));
	mapdata.insert(pair<string,string>("longitude",longi));
	mapdata.insert(pair<string,string>("latitude",lati));
	mapdata.insert(pair<string,string>("drivewayNum","0"));
	mapdata.insert(pair<string,string>("controlindexCode","0"));
	*/

	struct NAME_VALUE_S mapdata[7];

	mapdata[0].i("sessionId",id);
	mapdata[1].i("crossingIndex",crossindex);
	mapdata[2].i("crossingName",crossname);
	mapdata[3].i("longitude",longi);
	mapdata[4].i("latitude",lati);
	mapdata[5].i("drivewayNum","1");
	mapdata[6].i("controlindexCode",index_code);

	struct NAME_VALUE_S subdata[3];

	subdata[0].i("laneNum","1");
	subdata[1].i("laneName","车道1");
	subdata[2].i("dirName","");

	if(false==CreateXmlLite_Cross(
		"crossingInfo",mapdata,7,
		"laneInfos","laneInfo",subdata,3,
		xmllitestr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_insertCrossingInfo 无法创建XML lite文件");
		printf("SendSoap_insertCrossingInfo 无法创建XML lite文件\n");

		return false;
	}

	if(false==CreateXml("ser:insertCrossingInfo",
		xmllitestr,
		xmlstr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_insertCrossingInfo 无法创建XML文件");
		//printf("SendSoap_insertCrossingInfo 无法创建XML文件\n");

		return false;
	}

	CService.InitData();

	int nRes = CService.PostSoap(wsdlUrl,soapactionstr,xmlstr,failstr);

	if(0!=nRes)
	{
		strcat(failstr," SendSoap_insertCrossingInfo POST出错");
		//printf("SendSoap_insertCrossingInfo POST出错\n");
		CService.DestroyData();
		return false;
	}

	if(WEB_KAKOU_DEBUG)
		printf("\nSendSoap_insertCrossingInfo resp_buffer=%s\n",CService.m_resp_buffer.c_str());
/*
	if(NULL!=strstr(CService.m_resp_buffer.c_str(),"<ns:return>"))
	{
		strcat(failstr," SendPidPushSoap 返回报文不为0");
		CService.DestroyData();
		return false;
	}
	*/

	char returnstr[XMLRW_MAX_STR]="";

	if(false == XMLgetReturnData(CService.m_resp_buffer.c_str(),returnstr))
	{
		strcat(failstr," SendSoap_insertCrossingInfo XMLgetReturnDataT出错");
		CService.DestroyData();
		return false;
	}
	// 可以清空了
	CService.DestroyData();

	//放在栈里 内存不会泄露
	TiXmlDocument returnDocument;
	returnDocument.Parse(returnstr);

	TiXmlElement *preturnRootEle = returnDocument.RootElement();
	if (NULL==preturnRootEle)
	{
		strcpy(failstr,"SendSoap_insertCrossingInfo XMLreturn出错");
		//printf("SendSoap_insertCrossingInfo XMLreturn出错\n");
		return false;
	}

	char codestr[32]="";
	char messagestr[256]="";

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	for(pNodeRow = preturnRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
	{
		pNodeData = NULL;
		if(ReadQueryNodeText(pNodeRow,pNodeData,"code",codestr,XMLRW_MAX_STR,true))
		{
		//	printf("code=%s\n",codestr);
			NULL;
		}
		if(ReadQueryNodeText(pNodeRow,pNodeData,"message",messagestr,XMLRW_MAX_STR,true))
		{
			//printf("message=%s\n",messagestr);
			NULL;
		}
		if(ReadQueryNodeText(pNodeRow,pNodeData,"crossId",crossIdstr,XMLRW_MAX_STR,true))
		{
			//printf("crossId=%s\n",crossIdstr);
			NULL;
		}
	}

	if (0!=atol(codestr))
	{
		sprintf(failstr,"SendSoap_insertCrossingInfo XMLmessage出错 %s",messagestr);
		return false;
	}


	return true;

}



bool SendSoap_deleteCrossingInfo(char *wsdlUrl,
								 char *id,
								 char *crossindex,
								char *failstr)
{
	Service CService;

	long xmlstrlen=5120;

	char xmlstr[5120];
	char xmllitestr[5120];

	char soapactionstr[512]="Content-Type: application/soap+xml;charset=UTF-8;action=\"urn:deleteCrossingInfo\"";

	struct NAME_VALUE_S mapdata[2];
	mapdata[0].i("sessionId",id);
	mapdata[1].i("crossingIndex",crossindex);


	if(false==CreateXmlLite("crossingInfo",mapdata,2,
		xmllitestr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_deleteCrossingInfo 无法创建XML lite文件");
		//printf("SendSoap_deleteCrossingInfo 无法创建XML lite文件\n");

		return false;
	}

	if(false==CreateXml("ser:deleteCrossingInfo",
		xmllitestr,
		xmlstr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_deleteCrossingInfo 无法创建XML文件");
		//printf("SendSoap_deleteCrossingInfo 无法创建XML文件\n");

		return false;
	}

	CService.InitData();

	int nRes = CService.PostSoap(wsdlUrl,soapactionstr,xmlstr,failstr);

	if(0!=nRes)
	{
		strcat(failstr," SendSoap_deleteCrossingInfo POST出错 卡口服务未启动");
	//	printf("SendSoap_deleteCrossingInfo POST出错\n");
		CService.DestroyData();
		return false;
	}
	
	if(WEB_KAKOU_DEBUG)
		printf("SendSoap_deleteCrossingInfo resp_buffer=%s\n",CService.m_resp_buffer.c_str());

/*
	if(NULL!=strstr(CService.m_resp_buffer.c_str(),"<ns:return>"))
	{
		strcat(failstr," SendPidPushSoap 返回报文不为0");
		CService.DestroyData();
		return false;
	}
	*/
/*
	//出来默认UTF-8了
	char* charstr=(char *)calloc(CService.m_resp_buffer.length()*2+4,sizeof(char));

	UTF82CHAR(CService.m_resp_buffer.c_str(),charstr,CService.m_resp_buffer.length()*2);

	//防止报文错误 找左起第一个<符合传下去进行解析
	const char *presp=strchr(charstr,'<');
	if(NULL== presp)
	{
		strcpy(failstr,"SendInitSystemSoap XML出错 找不到<");
		printf("SendInitSystemSoap XML出错 找不到<\n");
		free(charstr);
		CService.DestroyData();
		return false;
	}

	char* utf8=(char *)calloc(strlen(presp)*2+4,sizeof(char));
	
	CHAR2UTF8(presp,utf8,strlen(presp)*2);
*/
	char returnstr[XMLRW_MAX_STR]="";

	if(false == XMLgetReturnData(CService.m_resp_buffer.c_str(),returnstr))
	{
		strcat(failstr," SendSoap_deleteCrossingInfo XMLgetReturnDataT出错");
		CService.DestroyData();
		return false;
	}
	// 可以清空了
	CService.DestroyData();

# if 0 //不管返回值

	//放在栈里 内存不会泄露
	TiXmlDocument returnDocument;
	returnDocument.Parse(returnstr);


	TiXmlElement *preturnRootEle = returnDocument.RootElement();
	if (NULL==preturnRootEle)
	{
		strcpy(failstr,"SendSoap_deleteCrossingInfo XMLreturn出错");
		printf("SendSoap_deleteCrossingInfo XMLreturn出错\n");
		return false;
	}

	char codestr[32]="";
	char messagestr[1024]="";
	char sessionstr[128]="";

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	for(pNodeRow = preturnRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
	{
		pNodeData = NULL;
		if(ReadQueryNodeText(pNodeRow,pNodeData,"code",codestr,XMLRW_MAX_STR,true))
		{
			printf("code=%s\n",codestr);
		}
		if(ReadQueryNodeText(pNodeRow,pNodeData,"message",messagestr,XMLRW_MAX_STR,true))
		{
			printf("message=%s\n",messagestr);
		}
		
	}

	if (0!=atol(codestr))
	{
		sprintf(failstr,"卡口系统SendSoap_deleteCrossingInfo XMLmessage出错 %s",messagestr);
		return false;
	}
#endif

	return true;

}



bool SendSoap_insertVehicleInfo(char *wsdlUrl,
								 char *id,char *crossindex,char *passTime,char *plateInfo,bool isPicUrl,
								char *picPlateurl,unsigned char *picPlatedata,long picPlatelen,
								char *picVehicleurl,unsigned char *picVehicledata,long picVehiclelen,
								char *failstr)
{
	Service CService;

	long xmlstrlen=728000;
	//这里的值特别大
	char *xmlstr=(char *)calloc(xmlstrlen,sizeof(char));
	char *xmllitestr=(char *)calloc(xmlstrlen,sizeof(char));

	char soapactionstr[512]="Content-Type: application/soap+xml;charset=UTF-8;action=\"urn:insertVehicleInfo\"";


	struct NAME_VALUE_S mapdata[17];

	mapdata[0].i("sessionId",id);
	mapdata[1].i("crossIndexcode",crossindex);
	mapdata[2].i("laneId","1"); //车道号
	mapdata[3].i("plateInfo",plateInfo); //车牌号
	mapdata[4].i("plateColor","6"); //车牌颜色  绿色
	mapdata[5].i("plateType","16"); //车牌类型  摩托车

	char Timeformat[64];
	sprintf(Timeformat,"%s.000",passTime);
	mapdata[6].i("passTime",Timeformat); //时间

	mapdata[7].i("vehicleSpeed","0"); //车速
	mapdata[8].i("vehicleType","7"); //车辆类型 电动车
	mapdata[9].i("vehColorDepth","1"); //车辆颜色深浅
	mapdata[10].i("vehicleColor","1"); //车身颜色 其他

	char *Platebase64str=NULL;
	char *Vehiclebase64str=NULL;

	if(isPicUrl)
	{
		mapdata[11].i("picPlate",picPlateurl); //车牌图
		mapdata[12].i("picVehicle",picVehicleurl); //过车图
	}
	else
	{
		if(0==picPlatelen)
		{
			mapdata[11].i("picPlate",""); //车牌图
		}
		else
		{
			//转成BASE64传入
			Platebase64str=(char *)calloc(OUT_ENBASE64_SIZE(picPlatelen)+10,sizeof(char));
			base64_encode(Platebase64str,(OUT_ENBASE64_SIZE(picPlatelen)+10)*sizeof(char),picPlatedata,picPlatelen);
			mapdata[11].i("picPlate",Platebase64str); //车牌图
		}

		Vehiclebase64str=(char *)calloc(OUT_ENBASE64_SIZE(picVehiclelen)+10,sizeof(char));
		base64_encode(Vehiclebase64str,(OUT_ENBASE64_SIZE(picVehiclelen)+10)*sizeof(char),picVehicledata,picVehiclelen);

		//转BASE64
		mapdata[12].i("picVehicle",Vehiclebase64str); //过车图
	}

	mapdata[13].i("alarmAction","0"); //违法代码 正常
	mapdata[14].i("vehicleState","1"); //行车状态  正常
	mapdata[15].i("reserved1","");
	mapdata[16].i("reserved2","");

	if(false==CreateXmlLite_Vehicle("vehicleInfo",isPicUrl,mapdata,17,
		xmllitestr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_insertVehicleInfo 无法创建XML lite文件");
		printf("SendSoap_insertVehicleInfo 无法创建XML lite文件\n");
	
		//释放
		if(Platebase64str)
		{
			free(Platebase64str);
			Platebase64str=NULL;
		}
		if(Vehiclebase64str)
		{
			free(Vehiclebase64str);
			Vehiclebase64str=NULL;
		}

		if(xmlstr)
		{
			free(xmlstr);
			xmlstr=NULL;
		}
		if(xmllitestr)
		{
			free(xmllitestr);
			xmllitestr=NULL;
		}
		return false;
	}
	//释放
	if(Platebase64str)
	{
		free(Platebase64str);
		Platebase64str=NULL;
	}
	if(Vehiclebase64str)
	{
		free(Vehiclebase64str);
		Vehiclebase64str=NULL;
	}

	if(false==CreateXml("ser:insertVehicleInfo",
		xmllitestr,
		xmlstr,xmlstrlen))
	{
		strcpy(failstr,"SendSoap_insertVehicleInfo 无法创建XML文件");
		printf("SendSoap_insertVehicleInfo 无法创建XML文件\n");

		if(xmlstr)
		{
			free(xmlstr);
			xmlstr=NULL;
		}
		if(xmllitestr)
		{
			free(xmllitestr);
			xmllitestr=NULL;
		}
		return false;
	}

	CService.InitData();

	int nRes = CService.PostSoap(wsdlUrl,soapactionstr,xmlstr,failstr);

	if(0!=nRes)
	{
		strcat(failstr," SendSoap_insertVehicleInfo POST出错");
		printf("SendSoap_insertVehicleInfo POST出错\n");
		CService.DestroyData();
		
		if(xmlstr)
		{
			free(xmlstr);
			xmlstr=NULL;
		}
		if(xmllitestr)
		{
			free(xmllitestr);
			xmllitestr=NULL;
		}

		return false;
	}
	//释放
	if(xmlstr)
	{
		free(xmlstr);
		xmlstr=NULL;
	}
	if(xmllitestr)
	{
		free(xmllitestr);
		xmllitestr=NULL;
	}

	if(WEB_KAKOU_DEBUG)
		printf("\nSendSoap_insertVehicleInfo resp_buffer=%s\n",CService.m_resp_buffer.c_str());
/*
	if(NULL!=strstr(CService.m_resp_buffer.c_str(),"<ns:return>"))
	{
		strcat(failstr," SendPidPushSoap 返回报文不为0");
		CService.DestroyData();
		return false;
	}
	*/

	char returnstr[XMLRW_MAX_STR]="";

	if(false == XMLgetReturnData(CService.m_resp_buffer.c_str(),returnstr))
		{
		strcat(failstr," SendSoap_insertVehicleInfo XMLgetReturnDataT出错");
		CService.DestroyData();
		return false;
	}
	// 可以清空了
	CService.DestroyData();

	//放在栈里 内存不会泄露
	TiXmlDocument returnDocument;
	returnDocument.Parse(returnstr);


	TiXmlElement *preturnRootEle = returnDocument.RootElement();
	if (NULL==preturnRootEle)
	{
		strcpy(failstr,"SendSoap_insertVehicleInfo XMLreturn出错");
		printf("SendSoap_insertVehicleInfo XMLreturn出错\n");
		return false;
	}

	char codestr[32]="";
	char messagestr[1024]="";

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	for(pNodeRow = preturnRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
	{
		pNodeData = NULL;
		if(ReadQueryNodeText(pNodeRow,pNodeData,"code",codestr,XMLRW_MAX_STR,true))
		{
			printf("code=%s\n",codestr);
		}
		if(ReadQueryNodeText(pNodeRow,pNodeData,"message",messagestr,XMLRW_MAX_STR,true))
		{
			printf("message=%s\n",messagestr);
		}
	}

	if (0!=atol(codestr))
	{
		sprintf(failstr,"SendSoap_insertVehicleInfo XML出错 %s",messagestr);
		return false;
	}


	return true;

}


#if 0
bool SendPidPushSoap(char *wsdlUrl,char *targetNamespace,char *methodname,
					 char *valuename,const char *valuedata,
					  char *failstr)
{
	Service CService;

	CService.InitData();

	char xmlstr[51200];
	//
	char methodnametemp[256];
	char valuenametemp[256];

	//char targetNamespace[256]="http://push.services.webService.vm.yr.com";

	char soapactionstr[256];

	//sprintf(methodnametemp,"push:%s","concentratePushSend");
	//sprintf(valuenametemp,"push:%s","args0");

	sprintf(methodnametemp,"push:%s",methodname);
	sprintf(valuenametemp,"push:%s",valuename);

	sprintf(soapactionstr,"SOAPAction: \"%s/%s\"",targetNamespace,methodname);

	//转成BASE64传入
	char *base64str=(char *)calloc(OUT_ENBASE64_SIZE(strlen(valuedata))+10,sizeof(char));
	base64_encode(base64str,(OUT_ENBASE64_SIZE(strlen(valuedata))+10)*sizeof(char),(unsigned char *)valuedata,strlen(valuedata));

	if(false==CreateXml(targetNamespace,
		methodnametemp,
		valuenametemp,
		//valuedata,
		base64str,
		xmlstr,51200))
	{
		strcpy(failstr,"SendPidPushSoap 无法创建XML文件");
		printf("SendPidPushSoap 无法创建XML文件\n");

		free(base64str);
		return false;
	}

	free(base64str);

//	int nRes = CService.PostSoap("http://10.142.50.20:8080/uvss/services/infoPushSend",
	//	"SOAPAction: \"http://push.services.webService.vm.yr.com/concentratePushSend\"",
	//	xmlstr);


	int nRes = CService.PostSoap(wsdlUrl,soapactionstr,xmlstr,failstr);

	if(0!=nRes)
	{
		strcat(failstr," SendPidPushSoap POST出错");
		printf("SendPidPushSoap POST出错\n");
		CService.DestroyData();
		return false;
	}

	if(WEB_KAKOU_DEBUG)
		printf("\nSendPidPushSoap resp_buffer=%s\n",CService.m_resp_buffer.c_str());
/*
	if(NULL!=strstr(CService.m_resp_buffer.c_str(),"<ns:return>"))
	{
		strcat(failstr," SendPidPushSoap 返回报文不为0");
		CService.DestroyData();
		return false;
	}
	*/
	
	//出来默认UTF-8了
	char* charstr=(char *)calloc(CService.m_resp_buffer.length()*2+4,sizeof(char));

	UTF82CHAR(CService.m_resp_buffer.c_str(),charstr,CService.m_resp_buffer.length()*2);

	//防止报文错误 找左起第一个<符合传下去进行解析
	const char *presp=strchr(charstr,'<');
	if(NULL== presp)
	{
		strcpy(failstr,"SendPidPushSoap XML出错 找不到<");
		printf("SendPidPushSoap XML出错 找不到<\n");
		free(charstr);
		CService.DestroyData();
		return false;
	}
	
	char* utf8=(char *)calloc(strlen(presp)*2+4,sizeof(char));
	
	CHAR2UTF8(presp,utf8,strlen(presp)*2);

	//放在栈里 内存不会泄露
	TiXmlDocument myDocument;
	myDocument.Parse(utf8);

	free(utf8);
	free(charstr);

	TiXmlElement *pRootEle = myDocument.RootElement();
	if (NULL==pRootEle)
	{
		strcpy(failstr,"SendPidPushSoap XML出错");
		printf("SendPidPushSoap XML出错\n");
		CService.DestroyData();
		return false;
	}

	char temp[XMLRW_MAX_STR]="";

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	for(pNodeRow = pRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
	{
		pNodeData = NULL;
		if(ReadQueryNodeText(pNodeRow,pNodeData,"ns:return",temp,XMLRW_MAX_STR))
		{
			printf("ns:return=%s\n",temp);
		}
	}

	if(0!=strcmp(temp,"0000"))
	{
		sprintf(failstr,"返回出错:%s",temp);
		printf("%s\n",failstr);
		CService.DestroyData();
		return false;
	}

	CService.DestroyData();

	return true;

}

#endif