#include "StdAfx.h"
#include "FaceDetect.h"
#include "base64_codec.h"
#include "YUV2RGB.h"

#include "FaceLoginOCX.h"
#include "FaceLoginOCXCtrl.h"
extern CFaceLoginOCXCtrl *DlgFaceLoginOCXCtrl;

CFaceDetect::CFaceDetect(void)
{
	m_macthThreshold = 0;
	Facedb = 0;
	HostInfo = "";
	PersonID = "";
	ID_Card = "";
}

CFaceDetect::~CFaceDetect(void)
{
}

int CFaceDetect::GetFaceID(CString strHtml,CString *FaceID)
{
	int a,b;
	if(strHtml.Find("bad personid") > 1)
	{
		CString strlog;
		strlog.Format(_T("<ERROR><ENROLL> - <Bad Personid - %s>"),PersonID);
		DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

		return ERR_BAD_PERSON;
	}
	else if(strHtml.IsEmpty())
	{
		return ERR_REG_FAIL;
	}

	a = strHtml.Find("\"face_id\"");
	b = strHtml.Find("}");
	*FaceID = strHtml.Mid(a+12,b-a-14);
	return ERR_NO_ERROR;
}

int CFaceDetect::GetFaceCount(CString strHtml)
{
	int a,b;
	a = strHtml.Find("\"list_size\"");
	b = strHtml.Find("\"dection_list\"");
	int faceCount = atoi(strHtml.Mid(a+13,b-a-15));
	return faceCount;
}

void CFaceDetect::ParseFaceRect(int faceCount,CString strHtml,CRect *Face_List,CString *Image_List)
{
	int a,b;
	for(int i=0;i<faceCount;i++)
	{
#if 0	//facecloud 0.4版本
		a = strHtml.Find("\"left\"");
		b = strHtml.Find("\"top\"");
		Face_List[i].left = atoi(strHtml.Mid(a+8,b-12-a));
		a = strHtml.Find("\"top\"");
		b = strHtml.Find("\"right\"");
		Face_List[i].top = atoi(strHtml.Mid(a+7,b-11-a));
		a = strHtml.Find("\"right\"");
		b = strHtml.Find("\"bottom\"");
		Face_List[i].right = atoi(strHtml.Mid(a+9,b-13-a));
		a = strHtml.Find("\"bottom\"");
		b = strHtml.Find("}");
		Face_List[i].bottom = atoi(strHtml.Mid(a+9,b-3-a));
		strHtml = strHtml.Right(strHtml.GetLength()-b-2);
#endif
		a = strHtml.Find("\"left\"");
		b = strHtml.Find("\"right\"");
		Face_List[i].left = atoi(strHtml.Mid(a+8,b-a-12));
		a = strHtml.Find("\"top\"");
		Face_List[i].right = atoi(strHtml.Mid(b+9,a-b-11));
		b = strHtml.Find("\"bottom\"");
		Face_List[i].top = atoi(strHtml.Mid(a+7,b-a-11));
		a = strHtml.Find("\"image_data\"");
		Face_List[i].bottom = atoi(strHtml.Mid(b+9,a-b-3));
		strHtml = strHtml.Right(strHtml.GetLength()-a);
		//获取人脸图像
		b = strHtml.Find("},");
		Image_List[i] = strHtml.Mid(0,b+4);;
	}
}

bool CFaceDetect::CheckEnableFlag(CString strHtml)
{
	int a=0;
	a = strHtml.Find("\"enableFR\"");
	CString str = strHtml.Mid(a+11,4);

	if(str == "true")
	{
		return true;
	}
	return false;
}

void CFaceDetect::GetToken(CString strHtml)
{
	int a=0;
	a = strHtml.Find("\"token\"");
	Token = strHtml.Mid(a+9,32);
}

int CFaceDetect::CheckResultFlag(CString strHtml)
{
	if(strHtml.IsEmpty())
	{
		CString strlog = _T("<ERROR><CHECK> - <FaceServer Out of service>");
		DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
		return ERR_SER_REEOR;
	}

	int a=0;
	a = strHtml.Find("\"flag\"");
	CString flag = strHtml.Mid(a+7,4);

	if(flag == "true")
	{
		return ERR_NO_ERROR;
	}

	CString strlog;
	strlog.Format(_T("<TIPS><CHECK> Unallow User"));
	DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
	return ERR_BAD_USER;
}

void CFaceDetect::ReadEnrollInfo(CString strHtml)
{
	int a=0;
	int b=0;
	a = strHtml.Find("\"personid\"");
	b = strHtml.Find("\"faceServerInfo\"");
	PersonID = strHtml.Mid(a+11,b-a-12);

	a = strHtml.Find("\"faceServerInfo\"");
	b = strHtml.Find("\"facedbid\"");
	HostInfo = strHtml.Mid(a+25,b-a-27);

	a = strHtml.Find("\"facedbid\"");
	b = strHtml.Find("}");
	Facedb = atoi(strHtml.Mid(a+11,b-a-11));

	CString strlog;
	strlog.Format(_T("<TIPS><ENROLL> - UserInfo<%s><%s><%s><%d>"),DlgFaceLoginOCXCtrl->EnrollUser,PersonID,HostInfo,Facedb);
	DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
}

void CFaceDetect::ReadMatchInfo(CString strHtml)
{
	int a=0;
	int b=0;
	a = strHtml.Find("\"token\"");
	b = strHtml.Find("\"enableFR\"");
	Token = strHtml.Mid(a+9,b-a-11);

	a = strHtml.Find("\"idcard\"");
	b = strHtml.Find("\"faceServerInfo\"");
	ID_Card = strHtml.Mid(a+10,b-a-12);

	a = strHtml.Find("\"faceServerInfo\"");
	b = strHtml.Find("\"facedbid\"");
	HostInfo = strHtml.Mid(a+25,b-a-27);

	a = strHtml.Find("\"facedbid\"");
	b = strHtml.Find("\"threshold\"");
	Facedb = atoi(strHtml.Mid(a+11,b-a-12));

	a = strHtml.Find("\"threshold\"");
	b = strHtml.Find("}");
	m_macthThreshold = atoi(strHtml.Mid(a+13,b-a-14));

	CString strlog;
	strlog.Format(_T("<TIPS><MATCH> - UserInfo<%s><%s><%s><%d><%f>"),DlgFaceLoginOCXCtrl->MatchUser,ID_Card,HostInfo,Facedb,m_macthThreshold);
	DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
}

int CFaceDetect::CheckRegInfo(CString strFaceServer,int sysID,CString user,CString password)
{
	CString strData;
	strData.Format(_T("{\
						\"suserid\":\"%s\",\
						\"spwd\":\"%s\",\
						\"sysID\":\"%d\",\
						}"),user,password,sysID);

	CString result;
	result = m_webService.PostData(strFaceServer,strData,"/faceuser/checkRegInfo");

	int ret = CheckResultFlag(result);
	if(ret == ERR_NO_ERROR)
	{
		ReadEnrollInfo(result);
		return ERR_NO_ERROR;
	}
	return ret;
}

int CFaceDetect::CheckMatchInfo(CString strFaceServer,int sysID,CString user,CString password)
{
	CString strData;
	strData.Format(_T("{\
						\"suserid\":\"%s\",\
						\"spwd\":\"%s\",\
						\"sysID\":\"%d\",\
						}"),user,password,sysID);

	CString result;
	result = m_webService.PostData(strFaceServer,strData,"/faceuser/checkInfo");

	int ret = CheckResultFlag(result);
	if(ret == ERR_NO_ERROR)
	{
		if(CheckEnableFlag(result))	//判断是否需要进行人脸验证
		{
			ReadMatchInfo(result);
			return ERR_NO_ERROR;
		}
		else
		{
			GetToken(result);
			return ERR_NONEED_MATCH;
		}
	}

	return ret;
}

bool CFaceDetect::GetFaceCloudState(void)
{
	CString strData;
	strData.Format(_T("%s/facedb"),HostInfo);

	CString result;
	result = m_webService.GetData(strData,"");
	if(!result.IsEmpty())
	{
		return true;
	}

	CString strlog = _T("<ERROR><CHECK> - FaceCloud <Out of service>");
	DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
	return false;
}

void CFaceDetect::SendEnrollLog(CString strFaceServer,CString strResult)
{
	//http://10.142.50.125:9080/rlrz/extUser!saveRegLog
	CString result;
	result = m_webService.PostData(strFaceServer,strResult,"/faceuser/saveRegLog");
}

void CFaceDetect::SendMatchLog(CString strFaceServer,CString strResult)
{
	//http://10.142.50.125:9080/rlrz/extUser!saveRegLog
	CString result;
	result = m_webService.PostData(strFaceServer,strResult,"/faceuser/saveCplog");
}

CString CFaceDetect::RGBtoBase64(unsigned char *image,unsigned long int size,unsigned long int w,unsigned long int h)
{
	unsigned char *jpegBuffer;
	unsigned long jpegSize;

	jpegBuffer = (unsigned char *)calloc(size,sizeof(unsigned char));
	EncodeToJPEGBuffer(image,w,h,jpegBuffer,&jpegSize);
	//FILE *fp = fopen("D:\\test.jpg","wb");
	//if(fp)
	//{
	//	fwrite(jpegBuffer,jpegSize,1,fp);
	//	fclose(fp);
	//}

	CString strRequest;
	char *base64buf = (char *)calloc(OUT_ENBASE64_SIZE(jpegSize) ,sizeof(char));
	if(base64buf != NULL)
	{
		if(base64_encode(base64buf, sizeof(char) * OUT_ENBASE64_SIZE(jpegSize),
			(unsigned char *)jpegBuffer, jpegSize) != 0)
		{
			strRequest.Format(_T("{\"image_data\":{\"type\": \"jpg\",\"content\": \"%s\"}}"),base64buf);
		}
	}
	free(jpegBuffer);
	jpegBuffer = NULL;

	free(base64buf);
	base64buf = NULL;

	return strRequest;
}

void CFaceDetect::FaceDetect(CString strRequest,int *faceCount,CRect *Face_Rect_List,CString *Image_List)
{
	//SYSTEMTIME stime;
	//GetLocalTime(&stime);

	CString result;
	result = m_webService.PostData(HostInfo,strRequest,"/faceops/image_detection");

	*faceCount = GetFaceCount(result);
	if(faceCount>0)
	{
		ParseFaceRect(*faceCount,result,Face_Rect_List,Image_List);
	}
	//SYSTEMTIME end;
	//GetLocalTime(&end);

	//FILE *fp = fopen("D:\\time.txt","a+");
	//if(fp)
	//{
	//	fprintf(fp,"Face Number = %d\n",*faceCount);
	//	fprintf(fp,"Start Time : %02d:%02d:%02d:%d\nEnd Time : %02d:%02d:%02d:%d\n",
	//							stime.wHour,stime.wMinute,stime.wSecond,stime.wMilliseconds,
	//							end.wHour,end.wMinute,end.wSecond,end.wMilliseconds);
	//	fclose(fp);
	//}
}

#ifdef USE_CHTTP
CString CFaceDetect::GetMacthFace(CString strHtml)
{
	int a,b;
	a = strHtml.Find("\"face_id\"");
	b = strHtml.Find("\"sim\"");
	CString macthFace = strHtml.Mid(a+12,b-a-15);

	return macthFace;
}
#else
CString CFaceDetect::GetMacthFace(CString strHtml)
{
	int a,b;
	a = strHtml.Find("\"face_id\"");
	b = strHtml.Find("\"sim\"");
	CString macthFace = strHtml.Mid(a+12,b-a-16);
	//FILE *fp = fopen("D://log.txt","a+");
	//if(fp)
	//{
	//	fprintf(fp,"faceid = %s\n",macthFace);
	//	fprintf(fp,"result = %s\n",strHtml);
	//	fclose(fp);
	//}

	return macthFace;
}
#endif

double CFaceDetect::GetMacthScore(CString strHtml)
{
	int a,b;
	a = strHtml.Find("\"sim\"");
	b = strHtml.Find("\"blur\"");
	double macthScore = atof(strHtml.Mid(a+7,b-a-7));

	return macthScore;
}

#ifdef USE_CHTTP
void CFaceDetect::FaceMacth(CString strRequest,CString idCard,CString *strResult,int *matchFlag)
{
	CString strIDcard;// = _T(",\"id_card\":\"1234\"}");
	strIDcard.Format(_T("\"id_card\":\"%s\"}"),idCard);
	CString strTemp = strRequest.Left(strRequest.GetLength()-1);
	strTemp = "{"+strTemp + strIDcard;

	CString result;
	CString strCompare;
	strCompare.Format(_T("/faceops/image_compare?face_db=%d"),DlgFaceLoginOCXCtrl->FaceDB);
	result = m_webService.PostData(strTemp,strCompare);

	double macthScore = GetMacthScore(result);
	CString faceID = GetMacthFace(result);
	strTemp = strRequest.Mid(49,strRequest.GetLength()-57);
	CString temp;
	temp.Format(_T("%s,%s,%f"),strTemp,faceID,macthScore);
	*strResult = temp;

	FILE *fp = fopen("D:\\image.txt","a+");
	if(fp)
	{
		fprintf(fp,"%s\n",temp);
		fclose(fp);
	}

	if(macthScore > DlgFaceLoginOCXCtrl->macthScore)
	{
		*matchFlag = 1;
	}
}
#else
void CFaceDetect::FaceMacth(CString strRequest,CString *strResult,int *matchFlag)
{
	CString strIDcard;// = _T(",\"id_card\":\"1234\"}");
	strIDcard.Format(_T("\"id_card\":\"%s\"}"),ID_Card);
	CString strTemp = strRequest.Left(strRequest.GetLength()-1);
	strTemp = "{"+strTemp + strIDcard;

	CString result;
	CString strCompare;
	strCompare.Format(_T("/faceops/image_compare?face_db=%d"),Facedb);
	result = m_webService.PostData(HostInfo,strTemp,strCompare);

	if(*matchFlag == 1)
	{
		CString strlog = _T("<TIPS><MATCH> - Success - Other Thread Return!!");
		DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);
		return;
	}

	double macthScore = GetMacthScore(result);
	if(macthScore > m_macthThreshold)
	{
		CString faceID = GetMacthFace(result);
		strTemp = strRequest.Mid(51,strRequest.GetLength()-60);

		CString temp;
		temp.Format(_T("{\
						\"ret\":\"success\",\
						\"user\":\"%s\",\
						\"sysID\":\"%d\",\
						\"face_id\":\"%s\",\
						\"picture\":\"%s\",\
						\"similarity\":\"%f\",\
						\"flag\":\"true\"\
						}"),DlgFaceLoginOCXCtrl->MatchUser,DlgFaceLoginOCXCtrl->MatchSysID,faceID,strTemp,macthScore);
		temp.Replace("+","_");
		*strResult = temp;

		CString strlog;
		strlog.Format(_T("<TIPS><MATCH> - Success - User<%s><%s><%s><%f>"),DlgFaceLoginOCXCtrl->MatchUser,ID_Card,faceID,macthScore);
		DlgFaceLoginOCXCtrl->ShowOcxLog(strlog);

		*matchFlag = 1;
	}
	else if(macthScore != 0)
	{
		CString faceID = GetMacthFace(result);
		strTemp = strRequest.Mid(51,strRequest.GetLength()-60);

		CString temp;
		temp.Format(_T("{\
						\"ret\":\"success\",\
						\"user\":\"%s\",\
						\"sysID\":\"%d\",\
						\"face_id\":\"%s\",\
						\"picture\":\"%s\",\
						\"similarity\":\"%f\",\
						\"flag\":\"false\"\
						}"),DlgFaceLoginOCXCtrl->MatchUser,DlgFaceLoginOCXCtrl->MatchSysID,faceID,strTemp,macthScore);
		temp.Replace("+","_");
		*strResult = temp;
	}
}
#endif

//CString CFaceDetect::FaceEnroll(CString strRequest,CString personID)
//{
//	CString strEnroll;
//	strEnroll.Format(_T("/facedb/%d/persons/%s/image"),DlgFaceLoginOCXCtrl->FaceDB,personID);
//
//	CString result;
//	result = m_webService.PutData(strRequest,strEnroll);
//
//	CString FaceID = GetFaceID(result);
//
//	return FaceID;
//}

int CFaceDetect::FaceEnroll(unsigned char *image,unsigned long int size,CString *FaceID)
{
	CString strRequest; //POST过去的数据

	char *base64buf = (char *)calloc(OUT_ENBASE64_SIZE(size) ,sizeof(char));
	if(base64buf != NULL)
	{
		if(base64_encode(base64buf, sizeof(char) * OUT_ENBASE64_SIZE(size),
			(unsigned char *)image, size) != 0)
		{
			strRequest.Format(_T("{\"image_data\":{\"type\": \"jpg\",\"content\": \"%s\"}}"),base64buf);
		}
	}
	free(base64buf);
	base64buf = NULL;

	CString strEnroll;
	strEnroll.Format(_T("/facedb/%d/persons/%s/image"),Facedb,PersonID);

	CString result;
	result = m_webService.PutData(HostInfo,strRequest,strEnroll);

	return GetFaceID(result,FaceID);
}

void CFaceDetect::Clip(int faceCount,CRect *faceRect,int imageWidth,int imageHeight,CRect *ClipRect)
{
	int max = 0;
	int maxScale = 0;
	for(int i=0;i<faceCount;i++)
	{
		int old = faceRect[i].right - faceRect[i].left;
		if(old > maxScale)
		{
			maxScale = old;
			max=i;
		}
	}

	int temp;
	temp = faceRect[max].left%8;
	int left = faceRect[max].left - 32 - temp;
	temp = faceRect[max].right%8;
	int right = faceRect[max].right + 64 - temp;
	int top = faceRect[max].top-80;
	int bottom = faceRect[max].bottom+80;

	ClipRect->left = (left > 0) ? left : 0;
	ClipRect->right = (right < imageWidth) ? right : imageWidth;
	ClipRect->top = (top > 0) ? top : 0;
	ClipRect->bottom = (bottom < imageHeight) ? bottom : imageHeight;

	//FILE *fp = fopen("D:\\rect.txt","a+");
	//if(fp)
	//{
	//	fprintf(fp,"left = %d\ntop = %d\nbottom = %d\nright=%d\n",faceRect.left,faceRect.top,faceRect.bottom,faceRect.right);
	//	fprintf(fp,"left = %d\ntop = %d\nbottom = %d\nright=%d\n",ClipRect->left,ClipRect->top,ClipRect->bottom,ClipRect->right);
	//	fprintf(fp,"imageWidth = %d\nimageHeight = %d\n",imageWidth,imageHeight);
	//	fprintf(fp,"**************************************************************\n");
	//	fclose(fp);
	//}
}

void CFaceDetect::ClipFaceImage(unsigned char *image,int w,int h,unsigned char *clipImage,unsigned long int *size,CRect ClipRect)
{
	int tempw = ClipRect.right-ClipRect.left;
	int temph = ClipRect.bottom-ClipRect.top;
	unsigned char *tempImage = (unsigned char *)calloc(3*tempw*temph,sizeof(unsigned char));

	for(int y = 0 ; y < temph ; y++)
	{
		memcpy(&tempImage[(temph-y-1)*tempw*3],&image[ClipRect.left*3+(h-1-(y+ClipRect.top))*3*w],tempw*3);
		//for(int x = 0 ; x < (faceWidth*3) ; x++)
		//{
		//	tempIamge[x+y*faceWidth*3] = image[x+left + (y+top)*w*3];
		//}
	}


	EncodeToJPEGBuffer(tempImage,tempw,temph,clipImage,size);
	//FILE *fp = fopen("D:\\clip.jpg","wb");
	//if(fp)
	//{
	//	fwrite(clipImage,(*size),1,fp);
	//	fclose(fp);
	//}

	free(tempImage);
	tempImage = NULL;
}
