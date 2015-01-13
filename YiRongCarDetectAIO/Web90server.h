#ifndef _WEBSERVER_90_H_
#define _WEBSERVER_90_H_

#define XMLRW_MAX_WSTR 25600
#define XMLRW_MAX_STR 51200

#include <stdio.h>
#include <stdlib.h>
#include  "CService.h"
#include "base64_codec.h"


#include <iostream>
#include <string>

#include <vector>
#include <list>
#include <map>
//#include "StdString.h"
using namespace std;


struct NAME_VALUE_S
{
	public:
	string namestr;
	string valuestr;
	void i(string a,string b)
	{namestr=a;valuestr=b;}
};

typedef 	pair<string,string> aaaa; 

void ReadFromFile(char**data,char* pfile);
void WriteToFileAB(string buffer,char* pfile);
void WriteToFileWB(string buffer,char* pfile);

void CHAR2UTF8Lite(const char *str,char *utf8);
void UTF82CHARLite(const char *utf8,char *str);
void CHAR2UTF8(const char *str,char *utf8,unsigned long int len);
void UTF82CHAR(const char *utf8,char *str,unsigned long int len);

bool ReadGetNodePointerByName(TiXmlElement* pRootEle,string strNodeName,TiXmlElement* &Node);
bool ReadQueryNodeText(TiXmlElement *pRootEle,TiXmlElement* &FindNode,string strNodeName,char *strText,unsigned long int len,bool utfflag);



bool CreateXMLDataUTF8(TiXmlElement *prow,string attr,const char *valuedata,char *strText,unsigned long int len,bool cdataflag);
bool CreateXMLData(TiXmlElement *prow,string attr,const char *valuedata,bool cdataflag);

bool CreateXmlLite(char *name,struct NAME_VALUE_S mapdata[],int maplen,
				   char *strText,unsigned long int len)  ;

bool CreateXmlLite_Cross(char *name,struct NAME_VALUE_S mapdata[],int maplen,
						   char *subname, char *sub2name,struct NAME_VALUE_S subdata[],int sublen,
						   char *strText,unsigned long int len)   ;

bool CreateXmlLite_Vehicle(char *name,bool isPicUrl,struct NAME_VALUE_S mapdata[],int maplen,
						   char *strText,unsigned long int len)  ;


bool XMLgetReturnData(char *utf8xmlin,char*utf8xmlout);

bool SendSoap_InitSystem(char *wsdlUrl,char *ip,char *username,char *psw,
						 char *sessionIdstr,
					  char *failstr);

bool SendSoap_insertCrossingInfo(char *wsdlUrl,
								 char *id,char *index_code,
								 char *crossindex,char *crossname,char *longi, char *lati,
								 char *crossIdstr,
								char *failstr);

bool SendSoap_deleteCrossingInfo(char *wsdlUrl,
								 char *id,
								 char *crossindex,
								char *failstr);


bool SendSoap_insertVehicleInfo(char *wsdlUrl,
								char *id,char *crossindex,char *passTime,char *plateInfo,bool isPicUrl,
								char *picPlateurl,unsigned char *picPlatedata,long picPlatelen,
								char *picVehicleurl,unsigned char *picVehicledata,long picVehiclelen,
								char *failstr);

bool SendPidPushSoap(char *wsdlUrl,char *targetNamespace,char *methodname,
					 char *valuename,const char *valuedata,
					 char *failstr);

#endif