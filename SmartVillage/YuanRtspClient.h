#ifndef __YUAN_RTSP_CLIENT_H__
#define __YUAN_RTSP_CLIENT_H__

#pragma comment(lib, "ws2_32.lib")

/*
rtsp 客户端
实现rtsp连接等功能
*/

/*
判断一个rtsp是否可连通
// 判断rtsp地址是否可以连接
// 使用url去创建一个CYuanRtspClient对象

CYuanRtspClient YuanRtsp("rtsp://35.33.141.131/cam/realmonitor?channel=1&subtype=0");
ERTSPERROR eRet = YuanRtsp.InitSocket();
if (eRet == E_RTSP_SUCCESS)
{
	eRet = YuanRtsp.JudgeCorrect();
	if (eRet == E_RTSP_SUCCESS)
		printf("url可连通\n");
	else
		printf("url不可连通\n"); // 具体错误可以对eRet的值进行判断
}
*/

#include <WinSock2.h>
#include <string>
using namespace std;

// rtsp的默认端口号
#define RTSP_PROTOCOL_PORT 554

// 连接rtsp时的一些错误类型
typedef enum tagERTSPERROR
{
	
	E_RTSP_SUCCESS = 0, // 正确
	E_RTSP_SOCKET_START_ERROR, // 启动套接字失败
	E_RTSP_SOCKET_VERSION_ERROR, // 套接字版本不匹配
	E_RTSP_IP_ERROR, // url中的ip不对
	E_RTSP_SOCKET_ERROR, //创建套接字失败
	E_RTSP_SOCKET_CONNECT_ERROR, // 连接服务器失败
	E_RTSP_SOCKET_SEND_ERROR, // socket 发送失败
	E_RTSP_SOCKET_REV_ERROR, // socket 接收失败
	E_RTSP_SERVER_UNAVAILABLE, // rtsp server不可用(可能是地址不对)
	E_RTSP_UNKNOW_ERROR // 位置错误
}ERTSPERROR;

class CYuanRtspClient
{
public:
	/*
	* strUrl rtsp服务端的网址
	* nPort socket的端口号
	* strIp 服务端的ip（如果不填，则会去strUrl中提取)
	*/
	CYuanRtspClient(const string &strUrl, int nPort = RTSP_PROTOCOL_PORT, const string &strIp = "");
	~CYuanRtspClient();
	/*
	* 初始化套接字
	*/
	ERTSPERROR InitSocket(void);
	
	/*
	* 判断是否可以连接上url
	*/
	ERTSPERROR JudgeCorrect(void);
	
private:
	///////////////////////////////////////////
	/*
	* 关闭套接字
	*/
	void CloseSocket(void);
	/*
	* 连接服务器 
	*/
	ERTSPERROR Connect(void);
	/*
	* 请求媒体流信息 
	*/
	ERTSPERROR RtstDescribe(void);
	/*
	* 从url中提取ip 
	*/
	bool GetIpFromUrl(const string &strUrl);
	/*
	* 判断rtsp的url是否合法（rtsp://10.142.50.239这样vlc内部会崩溃，因此不合法)
	* 暂时不使用此接口
	*/
	bool IsValid(char *url);
	/*
	* 判断describe返回信息是否正确
	*/
	bool IsGetDescribeCorrect(const string &str);
private:
	string m_strUrl; // rtsp服务端网址
	string m_strIp; // 服务端的ip
	int m_nPort; // socket端口号
	SOCKET m_tcpSocket; // tcp的socket
	struct sockaddr_in m_sockAddr;
	int m_nSeq; // rtsp的CSep值
	// 用户名信息等
	string m_strAdmin;
	string m_strPassword;

};

#endif /* __YUAN_RTSP_CLIENT_H__ */