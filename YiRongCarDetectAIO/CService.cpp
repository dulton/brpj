#include "stdafx.h"
#include "CService.h"
#define CSERVER_DEBUG 1


//////////////////////////////////////////////////////////////////////////////// 
// Private members 
//////////////////////////////////////////////////////////////////////////////// 

Service::Service()
{
	m_lpPutData = NULL;
	m_pGetDownLoadProcess = NULL;
	m_pGetProcess = NULL;
	m_pFileWriteStruct = NULL;
	m_pFileAccessStruct = NULL;
}

void Service::InitData()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

void Service::DestroyData()
{
	curl_global_cleanup();
}

size_t Service::WriteCallback( char* data, size_t size, size_t nitems, void* userp )
{
	size_t	result = 0; 
	if ( userp )
	{
		Service* lpService = (Service*)userp;
		if (lpService->m_pFileWriteStruct)
		{
			lpService->m_pFileWriteStruct->WriteBlock( lpService->m_pFileWriteStruct,data,size * nitems );
		}
		else
		{
			lpService->m_resp_buffer.append(data, size * nitems); 
		}
		result = size * nitems;  // How much did we write? 
	}
	return result ;
}

size_t Service::WriteHeaderCallback( char *data, size_t size, size_t nmemb, void* userp )
{
	size_t result = 0; 
	if ( userp )
	{
		Service* lpService = (Service*)userp;

		lpService->m_resp_header.append(data, size * nmemb); 
		result = size * nmemb;  // How much did we write? 
	} 
	return result; 
}


size_t Service::UploadCallback( void *buffer, size_t sz, size_t nmemb, void *userp )
{
	size_t rd = 0;
	if ( userp )
	{
		Service *lpService = (Service*)userp;

		if ( lpService->m_lpPutData )
		{
			PutData* lpPut = (PutData*)lpService->m_lpPutData;
			rd = (nmemb*sz > lpPut->datalen)?lpPut->datalen:(nmemb*sz);
			memcpy(buffer, lpPut->data, rd);
			if (lpService->m_pGetProcess && lpService->m_pGetProcess->GetProcessFunc)
			{
				lpService->m_pGetProcess->GetProcessFunc(lpService->m_pGetProcess,
					((DWORD)lpPut->postion+rd)*100.0/(lpPut->totallen!=0 ?lpPut->totallen:lpPut->datalen));
			}

			char szbuf[MAX_PATH]={0};
			sprintf(szbuf,"postion=%ld,%lf%%",(DWORD)lpPut->postion+rd,((DWORD)lpPut->postion+rd)*100.0/lpPut->totallen!=0?lpPut->totallen:lpPut->datalen);
			//OutputDebugStringA(szbuf);
			//OutputDebugStringA("\r\n");
			lpPut->data += rd;
			lpPut->datalen -= rd;
			lpPut->postion += rd;

			return  rd;
		}
	}
	return 0;
}




int Service::ProgressCallback( const void* userp, double dltotal, double dlnow, double ultotal, double ulnow )
{
	if ( userp )
	{
		Service* lpService = (Service*)userp;
		if( lpService->m_pGetDownLoadProcess && lpService->m_pGetDownLoadProcess->GetProcessFunc )
		{
			if ( dltotal )
			{
				lpService->m_pGetDownLoadProcess->GetProcessFunc(lpService->m_pGetDownLoadProcess,dlnow*100.0/dltotal);
			}
		}
	}
	return 0;
}


void Service::SetUploadProcess( GETPROCESS* pGetProcess )
{
	m_pGetProcess = pGetProcess;
}

void Service::SetDownloaddProcess( GETPROCESS* pGetDownLoadProcess )
{
	m_pGetDownLoadProcess = pGetDownLoadProcess;
}

void Service::SetFileWrite( FILEWRITE* pFileWriteStruct )
{
	m_pFileWriteStruct = pFileWriteStruct;
}

/*
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:push="http://push.services.webService.vm.yr.com">
<soapenv:Header/>
<soapenv:Body>
<push:concentratePushSend>
<!--Optional:-->
<push:args0>eyJwdXNoc2Vu</push:args0>
</push:concentratePushSend>
</soapenv:Body>
</soapenv:Envelope>

*/

int Service::PostSoap( char *wsdlUrl,char *SoapActionStr,char *xmlbuffer,
					   char *failstr)
{

	m_resp_buffer="";
	m_resp_header="";
	//默认写入写出都是UTF-8模式 XML不用再次转换
	struct curl_slist *headers = NULL;
/*
	headers = curl_slist_append(headers, "Content-Type: text/xml;charset=UTF-8");
	//targetNamespace + 方法名  name
	//headers = curl_slist_append(headers, "SOAPAction: \"http://push.services.webService.vm.yr.com/videoReadPushSend\"");
	headers = curl_slist_append(headers, SoapActionStr);
*/
	//headers = curl_slist_append(headers, "Content-Type: application/soap+xml;charset=UTF-8;action=\"urn:initSystem\"");
	headers = curl_slist_append(headers, SoapActionStr);

	CURL *curl = curl_easy_init();
	if ( curl ) 
	{

		//WSDL路径
		//curl_easy_setopt(curl, CURLOPT_URL, "http://10.142.50.248:8086/uvss/services/infoPushSend");
		curl_easy_setopt(curl, CURLOPT_URL,wsdlUrl);

		char errorBuffer[CURL_ERROR_SIZE]={0};
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xmlbuffer);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(xmlbuffer));
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		//回报数据
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		int timeout=10;
		//设置URL地址错误 重连N次后推出
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
		//设置最低速度。为了中途拔网线
		curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 10);
		curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, timeout);

		if(CSERVER_DEBUG)
			curl_easy_setopt(curl, CURLOPT_VERBOSE , 1);

		curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.142.50.140:808");

		CURLcode curl_code = curl_easy_perform(curl);
		/* always cleanup */
		int http_code = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

		// clean up
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);

		if ( curl_code != CURLE_OK ) 
		{

			cout << "\nError: [" << curl_code << "] - " << errorBuffer;
			//DebugPrintA(2, "\nError--%s\n",errorBuffer);
			error_str = errorBuffer;
			//linux modify callback error info????
			//::MessageBoxA(NULL,error_str.c_str(),"HttpReq-ER",0);

			sprintf(failstr,"Cservice postcode %s",curl_easy_strerror(curl_code));

			return curl_code;

		}
		else if ( 400 <= http_code )
		{
			//DebugPrintA(3, "Http-code=%d",http_code);
			int start, end;
			start = m_resp_buffer.find("<internalReason>");
			end = m_resp_buffer.find("</internalReason>");
			if( start>=0 && end>=0 && end>start )
			{
				start += 16;
				error_str = m_resp_buffer.substr(start, end-start);
			}
			else
				error_str = m_resp_buffer;

			sprintf(failstr,"Cservice httpcode %d",http_code);

			return http_code;
		}
		else
			error_str = "";

		return CURLE_OK;
	}
	else	
	{
		sprintf(failstr,"Cservice postinit fail");
		return CURLE_FAILED_INIT;
	}

}
