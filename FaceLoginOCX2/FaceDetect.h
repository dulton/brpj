#pragma once

#include "MyWebService.h"

#define ERR_NO_ERROR		0	//OK
#define ERR_BAD_PERSON		1	//无此personID
#define ERR_REG_FAIL		2	//注册失败
#define ERR_SER_REEOR		3	//人脸平台异常
#define ERR_BAD_USER		4	//非法用户
#define ERR_NONEED_MATCH	5	//不需要进行人脸验证的标志
#define ERR_WRONG_VERSION	6	//OCX版本与服务器版本不一致


class CFaceDetect
{
public:
	CFaceDetect(void);
	~CFaceDetect(void);

	CMyWebService m_webService;
	double m_macthThreshold;		//相似阈值
	int Facedb;						//识别或者注册库
	long PersonID;					//待注册人员在facecloud的人员库ID
	CString ID_Card;				//待比对人员在facecloud的人员库中的ID_Card
	CString HostInfo;				//facecloud的服务器地址信息
	CString Token;					//验证令牌
	int matchTimes;					//平台设置的比对次数
	int matchCount;					//当前比对次数
public:
	//获取facecloud的运行状态
	bool GetFaceCloudState(void);
	//从平台校验当前注册人员，并获取facecloud信息
	//校验待注册人员信息
	int CheckRegInfo(CString strFaceServer,int sysID,CString user,CString password);
	//解析注册返回信息
	void ReadEnrollInfo(CString strHtml);
	//发送注册日志到平台
	void SendEnrollLog(CString strFaceServer,CString strResult);
	//发送识别日志到平台
	void SendMatchLog(CString strFaceServer,CString strResult);
	//校验当前请求是否合法
	int CheckResultFlag(CString strHtml);
	//判断当前人员是否需要人来验证
	bool CheckEnableFlag(CString strHtml);
	//判断当前人员是否需要人来验证
	void GetToken(CString strHtml);
	//校验当前识别人员信息
	int CheckMatchInfo(CString strFaceServer,int sysID,CString user,CString password);
	//解析识别返回信息
	void ReadMatchInfo(CString strHtml);


	//RGB转BASE64
	CString RGBtoBase64(unsigned char *image,unsigned long int size,unsigned long int w,unsigned long int h);
	//提交人脸检测
	void FaceDetect(CString strRequest,int *faceCount,CRect *Face_Rect_List,CString *Image_List);
	//人脸识别
	void FaceMacth(CString strRequest,CString *strResult,int *matchFlag);
	//人脸注册
	int FaceEnroll(unsigned char *image,unsigned long int size,CString *FaceID);
	//CString FaceEnroll(CString strRequest,CString personID);
	//解析检查结果
	int GetFaceID(CString strHtml,CString *FaceID);
	CString GetMacthFace(CString strHtml);
	double GetMacthScore(CString strHtml);
	int GetFaceCount(CString strHtml);															//获取人脸个数
	void ParseFaceRect(int faceCount,CString strHtml,CRect *Face_List,CString *Image_List);		//获取人脸框
	//人脸截图
	void Clip(int faceCount,CRect *faceRect,int imageWidth,int imageHeight,CRect *ClipRect);
	void ClipFaceImage(unsigned char *image,int w,int h,unsigned char *clipImage,unsigned long int *size,CRect ClipRect);
};
