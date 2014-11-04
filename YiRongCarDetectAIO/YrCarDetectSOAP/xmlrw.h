#ifndef _ZOG_XML_UTF8_RW_
#define _ZOG_XML_UTF8_RW_

#include "tchar.h"
#include "windows.h"

#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;


#include <tinyxml.h>
#include <tinystr.h>

/*
#ifdef NDEBUG
//RELEASE
#if (_MSC_VER == 1500)
#pragma comment(lib,"tinyxmlSTLvc2008UTF8Mt.lib") 
#else
#pragma comment(lib,"tinyxmlSTLvc6UTF8Mt.lib") 
#endif // _MSC_VER > 1000

#else
//DEBUG
#if (_MSC_VER == 1500)
#pragma comment(lib,"tinyxmlSTLvc2008UTF8Mtd.lib") 
#else
#pragma comment(lib,"tinyxmlSTLvc6UTF8Mtd.lib") 
#endif // _MSC_VER > 1000

#endif
*/

#ifdef _DEBUG
#pragma comment(lib,"./tinyxmlSTLvc6-UTF8/Debug/tinyxmlSTLvc6.lib")
#endif

#ifdef NDEBUG
#pragma comment(lib,"./tinyxmlSTLvc6-UTF8/Release/tinyxmlSTLvc6.lib")
#endif



#define XMLRW_MAX_WSTR 2560
#define XMLRW_MAX_STR 5120


struct NAME_VALUE_S
{
public:
	string namestr;
	string valuestr;
	void ins(string a,string b)
	{namestr=a;valuestr=b;}
};

void ReadFromFile(char**data,char* pfile);
void WriteToFileAB(string buffer,char* pfile);
void WriteToFileWB(string buffer,char* pfile);

void CHAR2UTF8Lite(const char *str,char *utf8);
void UTF82CHARLite(const char *utf8,char *str);
void CHAR2UTF8(const char *str,char *utf8,unsigned long int len);
void UTF82CHAR(const char *utf8,char *str,unsigned long int len);

bool ReadGetNodePointerByName(TiXmlElement* pRootEle,string strNodeName,TiXmlElement* &Node);
bool ReadQueryNodeText(TiXmlElement *pRootEle,TiXmlElement* &FindNode,string strNodeName,char *strText,unsigned long int len,bool utfflag);



bool CreateXMLData(TiXmlElement *prow,string attr,const char *valuedata,char *strText,unsigned long int len,bool cdataflag,bool utf8flag);
bool CreateXMLDataForDoc(TiXmlDocument *prow,string attr,const char *valuedata,char *strText,unsigned long int len,bool cdataflag,bool utf8flag);

bool CreateXmlLite_UTF8_NoSub(char *name,struct NAME_VALUE_S mapdata[],int maplen,
							  char *strText,unsigned long int len)  ;

bool CreateXmlLite_UTF8_OneSub(char *name,struct NAME_VALUE_S mapdata[],int maplen,
							   char *subname, char *sub2name,struct NAME_VALUE_S subdata[],int sublen,
							   char *strText,unsigned long int len) ;

bool CreateXmlLite_UTF8_Sub(char *name,struct NAME_VALUE_S mapdata[],int maplen,
							char *subname,int subtotal,struct NAME_VALUE_S *subdata[],int sublen,
							char *strText,unsigned long int len)  ;

//插入CDATA数据  不带子树 不带总括名
bool CreateXmlLite_UTF8_NoSub_NoName(struct NAME_VALUE_S mapdata[],int maplen,
									 char *strText,unsigned long int len) ;


bool ReadXML_UTF8_NoSub(const char *utf8xmlin,struct NAME_VALUE_S mapdata[],int maplen);

#endif