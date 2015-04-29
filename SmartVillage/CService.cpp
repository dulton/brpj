#include "stdafx.h"
#include "CService.h"
//#include "TypeDefine.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Private members
////////////////////////////////////////////////////////////////////////////////

Service::Service()
{
    m_lpHttpData = NULL;
    m_pGetDownLoadProcess = NULL;
    m_pGetProcess = NULL;
    m_pFileWriteStruct = NULL;
    m_pFileAccessStruct = NULL;

	m_error_map[0] = "ok";
	m_error_map[-1] = "error";
	m_error_map[7] = "COULDNT_CONNECT";
	m_error_map[56] = "RECV_ERROR";
	m_error_map[67] = "LOGIN_DENIED";
	

 }
string Service::GetError( int nRes )
{
	map<int,string>::iterator iter;
	iter = m_error_map.find( nRes );
	if ( iter != m_error_map.end() )
	{
		return m_error_map[ nRes ];
	}
	//return "CService 未知错误:%d" + nRes;
	char sztmp[10] = {0};
	itoa( nRes, sztmp,10);
	string str;
	str = "CService 未知错误: ";
	str.append( sztmp );
	return str;
}

size_t Service::WriteCallback( char* data, size_t size, size_t nitems, void* userp )
{
    size_t  result = 0;
    if ( userp )
    {
        Service* lpService = ( Service* )userp;
        if ( lpService->m_pFileWriteStruct )
        {
            lpService->m_pFileWriteStruct->WriteBlock( lpService->m_pFileWriteStruct, data, size * nitems );
        }
        else
        {
            lpService->m_resp_buffer.append( data, size * nitems );
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
        Service* lpService = ( Service* )userp;

        lpService->m_resp_header.append( data, size * nmemb );
        result = size * nmemb;  // How much did we write?
    }
    return result;
}


size_t Service::UploadCallback( void *buffer, size_t sz, size_t nmemb, void *userp )
{
    size_t rd = 0;
    if ( userp )
    {
        Service *lpService = ( Service* )userp;

        if ( lpService->m_lpHttpData )
        {
            HTTPDATA* lpPut = ( HTTPDATA* )lpService->m_lpHttpData;
            rd = ( nmemb * sz > lpPut->datalen ) ? lpPut->datalen : ( nmemb * sz );
            memcpy( buffer, lpPut->data, rd );
            if ( lpService && lpService->m_pGetProcess && lpService->m_pGetProcess->GetProcessFunc )
            {
                lpService->m_pGetProcess->GetProcessFunc( lpService->m_pGetProcess,
                        ( ( DWORD )lpPut->postion + rd ) * 100.0 / ( lpPut->totallen != 0 ? lpPut->totallen : lpPut->datalen ) );
            }

            char szbuf[MAX_PATH] = {0};
            sprintf( szbuf, "postion=%ld,%lf%%", ( DWORD )lpPut->postion + rd, ( ( DWORD )lpPut->postion + rd ) * 100.0 / lpPut->totallen != 0 ? lpPut->totallen : lpPut->datalen );
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
        Service* lpService = ( Service* )userp;
        if( lpService->m_pGetDownLoadProcess && lpService->m_pGetDownLoadProcess->GetProcessFunc )
        {
            if ( dltotal )
            {
                lpService->m_pGetDownLoadProcess->GetProcessFunc( lpService->m_pGetDownLoadProcess, dlnow * 100.0 / dltotal );
            }
        }
    }
    return 0;
}



int Service::HttpRequest( const string http_method,
                          const string url,
                          const HTTPDATA* lpHttpData,
                          const vector<string> custom_headers,
                          Service* lpService )
{
    //  string resp_buffer;  // body of the response from the server
    char *memblock = NULL;  // file size of POST body
 //   curl_global_init( CURL_GLOBAL_ALL ); //不可放线程内
    CURL *curl = curl_easy_init();
    char errorBuffer[CURL_ERROR_SIZE] = {0};
	 int timeout=5;
    if ( curl )
    {
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.142.49.127:8088");
		// curl_easy_setopt(curl, CURLOPT_PROXY, "10.142.50.140:806");
		//邮件账号密码
	//	curl_easy_setopt( curl, CURLOPT_USERNAME, m_username.c_str() );
	//	curl_easy_setopt( curl, CURLOPT_PASSWORD, m_password.c_str() );
		//邮件账号面
		 //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "UIDL");
		//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "TOP 1 0");
        curl_easy_setopt( curl, CURLOPT_ERRORBUFFER, errorBuffer );
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str() );
        //curl_easy_setopt( curl, CURLOPT_HEADER, 1 );
        //curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1 );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, lpService );
        curl_easy_setopt( curl, CURLOPT_HEADERFUNCTION, WriteHeaderCallback ); // our static function
        curl_easy_setopt( curl, CURLOPT_WRITEHEADER, lpService ); //"headers" is a member variable referencing HttpHeaders
       // curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
       // curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );
       // curl_easy_setopt( curl, CURLOPT_VERBOSE , 1 );
		 curl_easy_setopt( curl, CURLOPT_VERBOSE ,0 );
		//curl_easy_setopt( curl, CURLOPT_NOSIGNAL, 1L );
	
		 //设置URL地址错误 重连N次后推出
		 curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
		 //设置最低速度。为了中途拔网线
		 curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 10);
		 curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, timeout);

        //下载进度条
        curl_easy_setopt( curl, CURLOPT_NOPROGRESS, FALSE );
        curl_easy_setopt( curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback );
        curl_easy_setopt( curl, CURLOPT_PROGRESSDATA, lpService );

        //curl_easy_setopt(curl,CURLOPT_SSLVERSION,3);

        struct curl_slist *headers = NULL;
        // Add standard headers
		unsigned int i;

        for (  i = 0; i < request_headers_.size(); ++i )
        {
            headers = curl_slist_append( headers, request_headers_[i].c_str() );
        }

        // Add any custom headers
        for (  i = 0; i < custom_headers.size(); ++i )
        {
            headers = curl_slist_append( headers, custom_headers[i].c_str() );
        }
        if ( http_method == "GET" )
        {
            curl_easy_setopt( curl, CURLOPT_HTTPGET, 1L );
//          headers =
//              curl_slist_append(headers, "Content-Type: application/atom+xml");

        }
        else if ( http_method == "POST" )
        {
            curl_easy_setopt( curl, CURLOPT_POST, 1 );
            if( lpHttpData != NULL && lpHttpData->data != NULL && lpHttpData->datalen >= 0 )
            {
                //  curl_easy_setopt(curl, CURLOPT_BINARYTRANSFER, 1);

				curl_easy_setopt( curl, CURLOPT_READFUNCTION, UploadCallback );
				lpService->m_lpHttpData = lpHttpData;
				curl_easy_setopt( curl, CURLOPT_READDATA, lpService );
				curl_easy_setopt( curl, CURLOPT_INFILESIZE, lpService->m_lpHttpData->datalen );

//                 curl_easy_setopt( curl, CURLOPT_POSTFIELDS, lpHttpData->data );
                 curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, lpHttpData->datalen );
            }
        }
        else if ( http_method == "PUT" )
        {
            curl_easy_setopt( curl, CURLOPT_PUT, 1L );

            if( lpHttpData != NULL && lpHttpData->data != NULL )
            {
                curl_easy_setopt( curl, CURLOPT_UPLOAD, 1L );

                /* we want to use our own read function */
                curl_easy_setopt( curl, CURLOPT_READFUNCTION, UploadCallback );
                lpService->m_lpHttpData = lpHttpData;
                /* now specify which file to upload */
                /*curl_easy_setopt(curl, CURLOPT_READDATA, lpPutData);
                curl_easy_setopt(curl, CURLOPT_INFILESIZE, lpPutData->datalen);*/
                curl_easy_setopt( curl, CURLOPT_READDATA, lpService );
                curl_easy_setopt( curl, CURLOPT_INFILESIZE, lpService->m_lpHttpData->datalen );
            }

        }
        else if ( http_method == "DELETE" )
        {
            curl_easy_setopt( curl, CURLOPT_CUSTOMREQUEST, "DELETE" );

        }
        else
        {
            //cerr << "Error: Unknown http method - " << http_method << endl;

            error_str = "Error: Unknown http method - " + http_method;
            //return AVERR_HTTP_METHOD;
            return CURLE_NOT_BUILT_IN;
        }

        // attach headers to this request
        curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );

        CURLcode curl_code = curl_easy_perform( curl );

        int http_code = 0;
        curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &http_code );

        // clean up
        curl_easy_cleanup( curl );
        curl_slist_free_all( headers );
    //    curl_global_cleanup(); //不可放线程内
        if ( memblock != NULL )
        {
            delete[] memblock;
        }

        if ( curl_code != CURLE_OK )
        {

            //cout << "\nError: [" << curl_code << "] - " << errorBuffer;
            //DebugPrintA(2, "\nError--%s\n",errorBuffer);
            error_str = errorBuffer;
            //linux modify callback error info????
            //::MessageBoxA(NULL,error_str.c_str(),"HttpReq-ER",0);
            return curl_code;

        }
        else if ( 400 <= http_code )
        {
            //DebugPrintA(3, "Http-code=%d",http_code);
            return http_code;
        }
        else
            error_str = "";
    }
    return CURLE_OK;
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

