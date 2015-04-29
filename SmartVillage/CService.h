#ifndef _CSERVICE_H_
#define _CSERVICE_H_
#include "typedefine.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>
using namespace std;


//#include "curl/curl.h"
//#include "TypeDefine.h"
using namespace std;

typedef struct _HttpData_
{
    unsigned char* data;
    long datalen;
    long postion;
    long totallen;
} HTTPDATA,*LPHTTPDATA;


typedef struct _GETPROCESS_
{
    int version;

    void ( *GetProcessFunc )( struct _GETPROCESS_* pThis, int nProcess );

} GETPROCESS,*LPGEPROCESS;

typedef struct _FILEWRITE_
{
    /**
    * @brief  Version number of the interface. Currently must be 1.
    */
    int version;

    /**
    * Output a block of data in your custom way.
    * @param[out] pThis	- Pointer to the structure itself
    * @param[out] pData	- Pointer to a buffer to output
    * @param[in]  size	- The size of the buffer.
    * @retval Should be non-zero if successful, zero for error.
    */
    int ( *WriteBlock )( _FILEWRITE_* pThis, const void* pData, unsigned long size );

} FILEWRITE,*LPFILEWRITE;


typedef struct _FILEACCESS_
{
    int m_FileMode;
    // File length, in bytes.
    unsigned long	m_FileLen;

    int ( *GetFileSize )( _FILEACCESS_* pThis, unsigned long* size );

    int ( *GetBlock )( _FILEACCESS_* pThis, unsigned long position, unsigned char** pBuf, unsigned long size );

    BOOL ( *GetFilePath )( _FILEACCESS_* pThis, wchar_t** pFilePath );

} FILEACCESS, *LPFILEACCESS;




class Service
{
public:
    Service();

    ~Service() {}

	static void GlobalInit(void)	{ curl_global_init( CURL_GLOBAL_ALL );}
	static void GlobalCleanup(void)	{ curl_global_cleanup();}

    int					HttpRequest( const string http_method,const string url,  const HTTPDATA* lpHttpData, const vector<string> custom_headers, Service* lpService );

    void				SetUploadProcess( GETPROCESS* pGetProcess );

    void				SetDownloaddProcess( GETPROCESS* pGetDownLoadProcess );

    void				SetFileWrite( FILEWRITE* pFileWriteStruct );

	void				SetAccount( string username, string password ){ m_username = username; m_password = password;}
	map<int,string>     m_error_map;
	string				GetError( int nRes );
public:
    string				m_resp_buffer;

    string				m_resp_header;

    GETPROCESS*			m_pGetProcess;

    GETPROCESS*			m_pGetDownLoadProcess;

    FILEWRITE*			m_pFileWriteStruct;

    FILEACCESS*			m_pFileAccessStruct;
protected:
    vector<string>		request_headers_;

    const HTTPDATA*		m_lpHttpData;
    
	string				error_str;

	string				m_username;

	string				m_password;
	
private:

    const vector<string>& request_headers() const {
        return request_headers_;
    }

    void set_request_headers( const vector<string>& headers )
    {
        request_headers_ = headers;
    }

    static int SeekCallback( void *userp, curl_off_t offset, int whence );

    static size_t WriteCallback( char* data, size_t size, size_t nitems, void* userp );

    static size_t WriteHeaderCallback( char *data, size_t size, size_t nmemb, void* userp );

    static int ProgressCallback( const void* userp, double dltotal, double dlnow, double ultotal, double ulnow );

    static size_t UploadCallback( void *buffer, size_t sz, size_t nmemb, void *userp );

};
#endif

