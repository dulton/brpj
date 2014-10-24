
#include "xmlrw.h"

#if defined( WIN32 ) && defined( TUNE )
#include <crtdbg.h>
_CrtMemState startMemState;
_CrtMemState endMemState;
#endif


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
	wchar_t * wbuff=(wchar_t *)calloc(len+4,sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, len+2);  
	// buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);  
	//  utf8 = new char[buffLen+1];  
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)utf8, len*2+4, 0, 0); 
	free(wbuff);
}

void UTF82CHAR(const char *utf8,char *str,unsigned long int len)
{
	wchar_t * wbuff=(wchar_t *)calloc(len,sizeof(wchar_t));
	//	int buffLen = 0;  
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wbuff, len+2);  
	//	buffLen = WideCharToMultiByte(CP_ACP, 0, wbuff, -1, NULL, 0, 0, 0);  
	//	m_gb2312 = new char[buffLen+1];  
	WideCharToMultiByte(CP_ACP, 0, wbuff, -1, (LPSTR)str, len*2+4, 0, 0);  
	free(wbuff);
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



bool CreateXMLData(TiXmlElement *prow,string attr,const char *valuedata,char *strText,unsigned long int len,bool cdataflag,bool utf8flag)
{
	// 生成子节点：data  
	TiXmlElement *pdata = new TiXmlElement(attr);  
	if (NULL==pdata)  
	{  
		return false;  
	}  
	prow->LinkEndChild(pdata);  
	//添加
	TiXmlText *pdataValue;
	if(utf8flag)
	{
		//转码
		CHAR2UTF8(valuedata,strText,len);
		//添加
		pdataValue = new TiXmlText(strText);  
	}
	else
	{
		//添加
		pdataValue = new TiXmlText(valuedata);  
	}

	pdata->LinkEndChild(pdataValue); 
	//有加CDATA标记
	pdataValue->SetCDATA(cdataflag); 
	return true;
}

bool CreateXMLDataForDoc(TiXmlDocument *prow,string attr,const char *valuedata,char *strText,unsigned long int len,bool cdataflag,bool utf8flag)
{
	// 生成子节点：data  
	TiXmlElement *pdata = new TiXmlElement(attr);  
	if (NULL==pdata)  
	{  
		return false;  
	}  
	prow->LinkEndChild(pdata);  
	//添加
	TiXmlText *pdataValue;
	if(utf8flag)
	{
		//转码
		CHAR2UTF8(valuedata,strText,len);
		//添加
		pdataValue = new TiXmlText(strText);  
	}
	else
	{
		//添加
		pdataValue = new TiXmlText(valuedata);  
	}

	pdata->LinkEndChild(pdataValue); 
	//有加CDATA标记
	pdataValue->SetCDATA(cdataflag); 
	return true;
}



//插入CDATA数据  不带子树
bool CreateXmlLite_UTF8_NoSub(char *name,struct NAME_VALUE_S mapdata[],int maplen,
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
		if(!CreateXMLData(pRootEle,
			mapdata[it].namestr.c_str(),
			mapdata[it].valuestr.c_str(),
			strText,len,
			false,false))
			return false;
	}

	//保存文件
	//pDoc.SaveFile("d:\\1.xml");

	//保存到字符串
	TiXmlPrinter printer;
	pDoc.Accept(&printer);
	//	strResult.Format("%s", printer.CStr());
	strcpy(strText,printer.CStr());

	//printf("xml=\n%s\n",strText);

	return true;  
}   



//插入CDATA数据  带一个子树 只能加一个
bool CreateXmlLite_UTF8_OneSub(char *name,struct NAME_VALUE_S mapdata[],int maplen,
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
		if(!CreateXMLData(pRootEle,
			mapdata[it].namestr.c_str(),
			mapdata[it].valuestr.c_str(),
			strText,len,
			false,false))
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
		if(!CreateXMLData(pSub2Ele,
			subdata[it].namestr.c_str(),
			subdata[it].valuestr.c_str(),
			strText,len,
			false,false))
			return false;
	}

	//保存文件
	//pDoc.SaveFile("d:\\1.xml");

	//保存到字符串
	TiXmlPrinter printer;
	pDoc.Accept(&printer);
	//	strResult.Format("%s", printer.CStr());
	strcpy(strText,printer.CStr());

	//printf("xml=\n%s\n",strText);

	return true;  
}  


//报文解析
bool ReadXML_UTF8_NoSub(const char *utf8xmlin,struct NAME_VALUE_S mapdata[],int maplen)
{
	//放在栈里 内存不会泄露
	TiXmlDocument myDocument;
	myDocument.Parse(utf8xmlin);

	TiXmlElement *pRootEle = myDocument.RootElement();
	if (NULL==pRootEle)
	{
		printf("ReadXML_UTF8_NoSub XML出错\n");
		return false;
	}

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	char tempstr[XMLRW_MAX_WSTR];

	for(int i=0;i<maplen;i++)
	{
		for(pNodeRow = pRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
		{
			pNodeData = NULL;
			if(ReadQueryNodeText(pNodeRow,pNodeData,mapdata[i].namestr,tempstr,XMLRW_MAX_WSTR,false))
			{
				mapdata[i].valuestr=tempstr;
				break ;
			}
		}
	}
	
	return true;
}
