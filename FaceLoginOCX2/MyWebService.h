#pragma once

#define CONNECT_TIME_OUT 500

class CMyWebService
{
public:
	CMyWebService(void);
	~CMyWebService(void);;

public:
	CString PostData(CString strHost,CString strRequest,CString strConnection);
	CString PutData(CString strHost,CString strRequest,CString strConnection);
	CString GetData(CString strHost,CString strConnection);
	CString DeleteData(CString strRequest,CString strConnection);
};
