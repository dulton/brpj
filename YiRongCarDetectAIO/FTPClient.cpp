#include "stdafx.h"

#if OPEN_VS2008_POCO_FTP

#include "FTPClient.h"

#include <string>

FTPClient::FTPClient()
: _ftpClient(NULL)
{
}

FTPClient::~FTPClient()
{
	if (_ftpClient != NULL)
	{
		delete _ftpClient;
	}
}

int FTPClient::LoginServer(
	const char *ip,
	int port,
	const char *user,
	const char *password
	)
{
	_ftpClient = new FTPClientSession(ip, port);
	if (_ftpClient == NULL)
	{
		return -1;
	}
	_ftpClient->login(user, password);

	return 0;
}

int FTPClient::SetWorkingDirectory(const char *path)
{
	if (_ftpClient == NULL)
	{
		return -1;
	}

	_ftpClient->setWorkingDirectory(path);
	return 0;
}

int FTPClient::CloseConnect(void)
{
	if (_ftpClient == NULL)
	{
		return -1;
	}

	_ftpClient->close();

	return 0;
}

int FTPClient::UpLoadFile(
						  const char *file,
						  const char *buffer
						  )
{
	if (_ftpClient == NULL)
	{
		return -1;
	}

	std::ostream& os = _ftpClient->beginUpload(file);
	os<<buffer;
	_ftpClient->endUpload();

	return 0;
}


int FTPClient::DownLoadFile(
							char *file,
							char *buffer
							)
{
	if (_ftpClient == NULL)
	{
		return -1;
	}

	std::istream& is = _ftpClient->beginList();
	file[0] = '\0';
	is>>file;
	_ftpClient->endList();
	if (file[0] == '\0')
	{
		return -1;
	}

	std::istream& isbuffer = _ftpClient->beginDownload(file);
	std::string str;
	isbuffer>>str;
	_ftpClient->endDownload();

	//_ftpClient->remove(file);

	return 0;
}


#endif