#include "stdafx.h"
#include "CService.h"
#include "JingAoUpload.h"

void File2String(char *Path,string &Data)
{
	FILE* fp = fopen( Path, "rb" );
	if ( fp )
	{
		long nLen = 0;
		fseek(fp,0,SEEK_END);
		nLen = ftell(fp);
		fseek(fp,0,SEEK_SET);
		unsigned	char* pData = new unsigned char [nLen];
		memset(pData,0,nLen);
		fread(pData,1,nLen,fp);
		Data.append((char*) pData,nLen );
		delete pData;
		fclose(fp);
	}
}

int	JingAoUpload(char *I_IpPort,char *I_url,char *I_Part1,char *I_Part2,char *I_Part3,
			  char *I_smallfilename,char *I_smallfilepath,
			  char *I_filename,char *I_filepath,char *jsonstr)
{
	Service m_Service;
	int nRes = 0;
	string tempstrA,tempstrB;
tempstrA=I_IpPort;
tempstrB=I_url;
	string m_strUrl= "http://"+tempstrA+tempstrB;
	m_Service.m_resp_buffer = "";
	m_Service.m_resp_header = "";
	string postFileData;
	string strBoundary = "-------------------------acebdf13572468";
	string formMultipart = "Content-Type: multipart/form-data; boundary=" + strBoundary;

	vector<string> head;
	head.push_back( formMultipart );
/*
	char jsonstr[2048]="{id:\"123456789\",captureDeviceIP:\"127.0.0.1\",captureDeviceMAC:\"00-00-00-00-00-00\",\
						captureTime:\"2015-01-01 12:00:00\",throughDirection:\"0\",\
geographicalPosition:\"某小区南门\"}";
	*/
	wchar_t wbuff[5120];
	
	//如果需要转换UTF8 设置为0=关闭 1=开启
#if 1
	MultiByteToWideChar(CP_ACP, 0, jsonstr, -1, wbuff, 2048);  
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)jsonstr, 2048, 0, 0); 
#endif
	
	postFileData = postFileData + "--" + strBoundary + "\r\n";
	tempstrA=I_Part1;
	postFileData += "Content-Disposition: form-data; name=\""+tempstrA+"\"\r\n";
	postFileData += "\r\n";
	postFileData += jsonstr;
	postFileData += "\r\n";
	
	//第二部分
	string smallImageData;
	File2String(I_smallfilepath,smallImageData);
	
	postFileData = postFileData + "--" + strBoundary + "\r\n";
	tempstrA=I_Part2;
	tempstrB=I_smallfilename;
	postFileData += "Content-Disposition: form-data; name=\""+tempstrA+"\";filename=\""+tempstrB+"\"\r\n";
	postFileData += "Content-Type: image/jpeg\r\n";
	postFileData += "\r\n";
	postFileData += smallImageData;
	postFileData += "\r\n";
	//第3部分

	string BigImageData;
	File2String(I_filepath,BigImageData);

	postFileData = postFileData + "--" + strBoundary + "\r\n";
	tempstrA=I_Part3;
	tempstrB=I_filename;
	postFileData += "Content-Disposition: form-data; name=\""+tempstrA+"\";filename=\""+tempstrB+"\"\r\n";
	postFileData += "Content-Type: image/jpeg\r\n";
	postFileData += "\r\n";
	postFileData += BigImageData;
	postFileData += "\r\n";
	//结束
	postFileData = postFileData + "--" + strBoundary + "--\r\n";
	
	HTTPDATA httpData ;
	memset(&httpData,0,sizeof(HTTPDATA));
	httpData.data = (unsigned char*)postFileData.data();
	httpData.datalen = postFileData.size();
	
	nRes = m_Service.HttpRequest( "POST", m_strUrl, &httpData, head, &m_Service );
	if ( nRes == CURLE_OK )
	{
	//	string strHeader = m_Service.m_resp_header;
		MultiByteToWideChar(CP_UTF8 , 0, m_Service.m_resp_buffer.c_str(), -1, wbuff, 2048);  
		WideCharToMultiByte(CP_ACP, 0, wbuff, -1, (LPSTR)jsonstr, 2048, 0, 0); 
		//string strBuffer = 	jsonstr;
	}
	return nRes;
}

