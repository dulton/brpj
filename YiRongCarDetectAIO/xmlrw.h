#ifndef _ZOG_XML_UTF8_RW_
#define _ZOG_XML_UTF8_RW_

#include "tchar.h"
#include "windows.h"

#ifdef TIXML_USE_STL
#include <iostream>
#include <sstream>
using namespace std;
#else
#include <stdio.h>
#endif

#include "tinyxml.h"

#define XMLRW_MAX_WSTR 2560
#define XMLRW_MAX_STR 5120

class XMLRW 
{
public:
	XMLRW();
	virtual ~XMLRW();

	void CHAR2UTF8(const char *str,char *utf8);
	void UTF82CHAR(const char *utf8,char *str);
	
	//Ð´
	bool CreateData(TiXmlElement *prow,string attr,char *value);
	bool CreateRow(TiXmlElement *pRootEle,unsigned long int i);
	bool CreateXml(char* XmlFile);

	//¶Á
	bool ReadGetNodePointerByName(TiXmlElement* pRootEle,string strNodeName,TiXmlElement* &Node);
	bool ReadQueryNodeText(TiXmlElement *pRootEle,TiXmlElement* &FindNode,string strNodeName,char *strText);
	bool ReadXml(char *XmlFile);
/*
	bool *ThreadFlag;
	CProgressCtrl	*m_progress;
	CString	*m_info;
*/
	unsigned long int total;
private:
   char DstStr[XMLRW_MAX_STR];  
   wchar_t wbuff[XMLRW_MAX_WSTR];

};

#endif