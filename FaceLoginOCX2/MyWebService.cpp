#include "StdAfx.h"
#include "MyWebService.h"
#include <afxinet.h>

#include "FaceLoginOCX.h"
#include "FaceLoginOCXCtrl.h"
#include "CService.h"

extern CFaceLoginOCXCtrl *DlgFaceLoginOCXCtrl;

CMyWebService::CMyWebService(void)
{
}

CMyWebService::~CMyWebService(void)
{
}

#ifdef USE_CHTTP
CString CMyWebService::PostData(CString strRequest,CString strConnection)
{
	CInternetSession m_InetSession;
	CHttpConnection *pHttpConnection= NULL;
	CHttpFile *pHttpFile = NULL;

	m_InetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,CONNECT_TIME_OUT);
	pHttpConnection = m_InetSession.GetHttpConnection(DlgFaceLoginOCXCtrl->strHost);
	pHttpFile =	pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,strConnection);
	//发送数据请求
	pHttpFile->SendRequest(NULL, 0, (LPTSTR)(LPCTSTR)strRequest, strRequest.GetLength());

	DWORD dwRet;
	CString strHtml;
	pHttpFile->QueryInfoStatusCode(dwRet);
	BOOL nRead;
	if (dwRet == HTTP_STATUS_OK){
		CString strLine;
		while ((nRead = pHttpFile->ReadString(strLine))>0)
		{
			strHtml += strLine;
		}
		//AfxMessageBox(strHtml);
	}    
	delete pHttpFile;

	return strHtml;
}

CString CMyWebService::PutData(CString strRequest,CString strConnection)
{
	CInternetSession m_InetSession;
	CHttpConnection *pHttpConnection= NULL;
	CHttpFile *pHttpFile = NULL;

	m_InetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,CONNECT_TIME_OUT);
	pHttpConnection = m_InetSession.GetHttpConnection(DlgFaceLoginOCXCtrl->strHost);
	pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_PUT,strConnection);
	//发送数据请求
	pHttpFile->SendRequest(NULL, 0, (LPTSTR)(LPCTSTR)strRequest, strRequest.GetLength());

	DWORD dwRet;
	CString strHtml;
	pHttpFile->QueryInfoStatusCode(dwRet);
	BOOL nRead;
	if (dwRet == HTTP_STATUS_OK){
		CString strLine;
		while ((nRead = pHttpFile->ReadString(strLine))>0)
		{
			strHtml += strLine;
		}
		//AfxMessageBox(strHtml);
	}    
	delete pHttpFile;

	return strHtml;
}

CString CMyWebService::GetData(CString strHost,CString strConnection)
{
	CInternetSession m_InetSession;
	CHttpConnection *pHttpConnection= NULL;
	CHttpFile *pHttpFile = NULL;

	m_InetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,CONNECT_TIME_OUT);
	pHttpConnection = m_InetSession.GetHttpConnection(strHost);
	pHttpFile=	pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,strConnection);

	pHttpFile->SendRequest();

	DWORD dwRet;
	CString strHtml;
	pHttpFile->QueryInfoStatusCode(dwRet);
	BOOL nRead;
	if (dwRet == HTTP_STATUS_OK){
		CString strLine;
		while ((nRead = pHttpFile->ReadString(strLine))>0)
		{
			strHtml += strLine;
		}
		AfxMessageBox(strHtml);
	}    
	delete pHttpFile;

	return strHtml;
}


CString CMyWebService::DeleteData(CString strRequest,CString strConnection)
{
	CInternetSession m_InetSession;
	CHttpConnection *pHttpConnection= NULL;
	CHttpFile *pHttpFile = NULL;

	m_InetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,CONNECT_TIME_OUT);
	pHttpConnection = m_InetSession.GetHttpConnection(DlgFaceLoginOCXCtrl->strHost);
	pHttpFile=	pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_DELETE,strConnection);

	pHttpFile->SendRequest();

	DWORD dwRet;
	CString strHtml;
	pHttpFile->QueryInfoStatusCode(dwRet);
	BOOL nRead;
	if (dwRet == HTTP_STATUS_OK){
		CString strLine;
		while ((nRead = pHttpFile->ReadString(strLine))>0)
		{
			strHtml += strLine;
		}
		AfxMessageBox(strHtml);
	}    
	delete pHttpFile;

	return strHtml;
}

#else
CString CMyWebService::PostData(CString strHost,CString strRequest,CString strConnection)
{
	Service CService;
	CService.InitData();
	int nRes ;
	string resp_buffer;
	string resp_header;
	CString strUrl = strHost + strConnection;

	nRes = CService.HttpRequest("POST",strUrl.GetBuffer(0),strRequest.GetBuffer(0),strRequest.GetLength(),"",0,0);


	if(0!=nRes && 200 !=nRes)
	{
		CString strlog;
		strlog.Format(_T("nres==%s=%d"),strUrl.GetBuffer(0),nRes);
		DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
		return "";
	}

	resp_buffer = CService.m_resp_buffer;
	resp_header = CService.m_resp_header;

	CString strHtml;
	strHtml = resp_buffer.c_str();
	return strHtml;
}

CString CMyWebService::PutData(CString strHost,CString strRequest,CString strConnection)
{
	Service CService;
	CService.InitData();
	int nRes ;
	string resp_buffer;
	string resp_header;
	CString strUrl = strHost + strConnection;

	nRes = CService.HttpRequest("PUT",strUrl.GetBuffer(0),strRequest.GetBuffer(0),strRequest.GetLength(),"",0,0);

	if(0!=nRes && 200 !=nRes)
	{
		CString strlog;
		strlog.Format(_T("nres==%s=%d"),strUrl.GetBuffer(0),nRes);
		DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
		return "";
	}
	resp_buffer = CService.m_resp_buffer;
	resp_header = CService.m_resp_header;

	CString strHtml;
	strHtml = resp_buffer.c_str();
	return strHtml;
}

CString CMyWebService::GetData(CString strHost,CString strConnection)
{
	Service CService;
	CService.InitData();
	int nRes ;
	string resp_buffer;
	string resp_header;

	CString strSend;
	strSend = strHost + strConnection;
	nRes = CService.HttpRequest("GET",strSend.GetBuffer(0));

	if(0!=nRes && 200 !=nRes)
	{
		CString strlog;
		strlog.Format(_T("nres==%s=%d"),strSend.GetBuffer(0),nRes);
		DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
		return "";
	}

	resp_buffer = CService.m_resp_buffer;
	resp_header = CService.m_resp_header;

	CString strHtml;
	strHtml = resp_buffer.c_str();
	return strHtml;
}
#endif
