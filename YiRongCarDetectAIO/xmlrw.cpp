#include "stdafx.h"
#include "yirongcardetectaio.h"

#include "DLGBlackInOut.h"
extern CDLGBlackInOut *pDlgBlackInout;

#include "xmlrw.h"
////////////////////////////////
#include "IO.h"
extern IO OracleIO;
////////////////////////////////

#if defined( WIN32 ) && defined( TUNE )
#include <crtdbg.h>
_CrtMemState startMemState;
_CrtMemState endMemState;
#endif

XMLRW::XMLRW()
{

}

XMLRW::~XMLRW()
{

}

void XMLRW::CHAR2UTF8(const char *str,char *utf8)
{
	// int buffLen = 0;  
	MultiByteToWideChar(CP_ACP, 0, str, -1, wbuff, XMLRW_MAX_WSTR);  
	// buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);  
	//  utf8 = new char[buffLen+1];  
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)utf8, XMLRW_MAX_STR, 0, 0);  
}

void XMLRW::UTF82CHAR(const char *utf8,char *str)
{
//	int buffLen = 0;  
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wbuff, XMLRW_MAX_WSTR);  
//	buffLen = WideCharToMultiByte(CP_ACP, 0, wbuff, -1, NULL, 0, 0, 0);  
//	m_gb2312 = new char[buffLen+1];  
	WideCharToMultiByte(CP_ACP, 0, wbuff, -1, (LPSTR)str, XMLRW_MAX_STR, 0, 0);  
}

bool XMLRW::CreateData(TiXmlElement *prow,string attr,char *value)
{
	// 生成子节点：data  
	TiXmlElement *pdata = new TiXmlElement(attr);  
	if (NULL==pdata)  
	{  
        return false;  
	}  
	prow->LinkEndChild(pdata);  
	//转码
	CHAR2UTF8(value,DstStr);
	//添加
	TiXmlText *pdataValue = new TiXmlText(DstStr);  
	pdata->LinkEndChild(pdataValue); 
	return true;
}

bool XMLRW::CreateRow(TiXmlElement *pRootEle,unsigned long int i)
{
	struct BLACK_DATA_ST data={0};

	//取一条数据
#if ALLTAB_DETECT_CAR_MODE
//汽车
	if(OracleIO.CAR_BlackTable_ReadOne(i,data))
#else
//电动车
	if(OracleIO.ELECAR_BlackTable_ReadOne(i,data))
#endif
	{
		// 生成子节点：row
		TiXmlElement *prow = new TiXmlElement(_T("ROW"));  
		if (NULL==prow)  
		{  
			return false;  
		}  
		pRootEle->LinkEndChild(prow);  
		
		if(!CreateData(prow,"SCARNUMBER",data.plate))
			return false;
		if(!CreateData(prow,"SBRAND",data.brand))
			return false;
		if(!CreateData(prow,"SLOSTNAME",data.name))
			return false;
		if(!CreateData(prow,"SPHONE",data.Phone))
			return false;
		if(!CreateData(prow,"SMARK",data.other))
			return false;
	}
	return true;
}

bool XMLRW::CreateXml(char* XmlFile)  
{  
	// 定义一个TiXmlDocument类指针  
	TiXmlDocument *pDoc = new TiXmlDocument;  
	if (NULL==pDoc)  
	{  
		return false;  
	}  
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration(_T("1.0"),_T("utf-8"),_T(""));  
	if (NULL==pDeclaration)  
	{  
		return false;  
    }  

	pDoc->LinkEndChild(pDeclaration);  
	// 生成一个根节点：pRootEle 
	TiXmlElement *pRootEle = new TiXmlElement(_T("ROWDATA"));  
	if (NULL==pRootEle)  
	{  
		return false;  
	}  
	pDoc->LinkEndChild(pRootEle);  

#if ALLTAB_DETECT_CAR_MODE
//汽车
	total=OracleIO.CAR_BlackTable_GetNum();

#else
//电动车
	total=OracleIO.ELECAR_BlackTable_GetNum();

#endif
	if(0==total)
	{
		pDlgBlackInout->m_info.Format("数据库无数据");
		pDlgBlackInout->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(pDlgBlackInout->m_info);
		return true;
	}

	unsigned long int i;
	//设置进度条
	pDlgBlackInout->m_progress.SetRange32(0,total);
	pDlgBlackInout->m_progress.SetStep(1);

	//有N条数据 就有N条这个
	for(i=1; (pDlgBlackInout->ThreadFlag) && (i<=total) ;i++)
	{
		if(!CreateRow(pRootEle,i))
			break;
		//填写进度
		pDlgBlackInout->m_progress.StepIt();
		pDlgBlackInout->m_info.Format("已导出:%d / %d 条",i,total);
		pDlgBlackInout->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(pDlgBlackInout->m_info);
	}
	//保存文件
	pDoc->SaveFile(XmlFile);  
    return true;  
}   

////////////////////////////////////////////////////////


/*!
*  \brief 通过根节点和节点名获取节点指针。
*
*  \param pRootEle   xml文件的根节点。
*  \param strNodeName  要查询的节点名
*  \param Node      需要查询的节点指针
*  \return 是否找到。true为找到相应节点指针，false表示没有找到相应节点指针。
*/
bool XMLRW::ReadGetNodePointerByName(TiXmlElement* pRootEle,string strNodeName,TiXmlElement* &Node)
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
bool XMLRW::ReadQueryNodeText(TiXmlElement *pRootEle,TiXmlElement* &FindNode,string strNodeName,char *strText)
{
	ReadGetNodePointerByName(pRootEle,strNodeName,FindNode);
	if (NULL!=FindNode)
	{
        const char* psz = FindNode->GetText(); 
		if (NULL==psz)
			strText[0] =0;
		else	//转码
			UTF82CHAR(psz,strText);
		return true;
	}
	else
	{
		strText[0] =0;
        return false;
	}
}

bool XMLRW::ReadXml(char *XmlFile)
{
   // 定义一个TiXmlDocument类指针
    TiXmlDocument *pDoc = new TiXmlDocument(XmlFile);
    if (NULL==pDoc)
    {
        return false;
    }
    pDoc->LoadFile(TIXML_ENCODING_UTF8);
    TiXmlElement *pRootEle = pDoc->RootElement();
    if (NULL==pRootEle)
    {
        return false;
    }

	pDlgBlackInout->GetDlgItem(IDC_STATIC_INFO)->SetWindowText("已读取: 0 条");

	char temp[XMLRW_MAX_STR];
	struct BLACK_DATA_ST data={0};

	TiXmlElement *pNodeRow = NULL;
	TiXmlElement *pNodeData = NULL;

	unsigned long int i=1;

	for(pNodeRow = pRootEle; pNodeRow; pNodeRow = pNodeRow->NextSiblingElement())
	{
		memset(&data,0,sizeof(struct BLACK_DATA_ST));

		//获取信息
		if(ReadQueryNodeText(pNodeRow,pNodeRow,"ROW",temp))
		{
			pNodeData = NULL;
			ReadQueryNodeText(pNodeRow,pNodeData,"SCARNUMBER",data.plate);
		
			pNodeData = NULL;
			ReadQueryNodeText(pNodeRow,pNodeData,"SBRAND",data.brand);
		
			pNodeData = NULL;
			ReadQueryNodeText(pNodeRow,pNodeData,"SLOSTNAME",data.name);
	
			pNodeData = NULL;
			ReadQueryNodeText(pNodeRow,pNodeData,"SPHONE",data.Phone);
	
			pNodeData = NULL;
			ReadQueryNodeText(pNodeRow,pNodeData,"SMARK",data.other);

			//车牌号为空
			if(0==strlen(data.plate))
				continue ;
			
			//写入一条
#if ALLTAB_DETECT_CAR_MODE
			//汽车
			if(OracleIO.CAR_BlackTable_AddNew(data))
			
#else
			//电动车
			if(OracleIO.ELECAR_BlackTable_AddNew(data))
			
#endif
			{
				pDlgBlackInout->m_info.Format("已导入: %d 条",i);
				pDlgBlackInout->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(pDlgBlackInout->m_info);

				i++;
			}
		}
		
	}

	return true;

}

/*
void main()
{

XMLRW a;

a.CreateXml("a.xml");

a.ReadXml("oracle_out.xml");
}
*/