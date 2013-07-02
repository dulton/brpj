#if OPEN_VS2008_POCO_FTP

#ifndef POCO__FTP_CLIENT_H__
#define POCO__FTP_CLIENT_H__

#pragma comment(lib,"pocolib/PocoFoundationmtd.lib") 
#pragma comment(lib,"pocolib/PocoNetmtd.lib") 

#include "Poco/Net/FTPClientSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"


using Poco::Net::FTPClientSession;
using Poco::Net::SocketAddress;
using Poco::Net::FTPException;

class FTPClient
{
public:
    FTPClient();
	~FTPClient();

	int LoginServer(
		const char *ip,
		int port,
		const char *user,
		const char *password
		);

	int CloseConnect(void);

	int SetWorkingDirectory(const char *path);

	int UpLoadFile(
					const char *file,
					const char *buffer
					);

	int DownLoadFile(
		char *file,
		char *buffer
		);

private:
	FTPClientSession *_ftpClient;
};

#endif

#endif