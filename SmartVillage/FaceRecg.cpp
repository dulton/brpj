#include "stdafx.h"
#include "FaceRecg.h"
#include "YUV2RGB.h"

#include "SmartVillage.h"
#include "SmartVillageDlg.h"
extern CYRSVMySQL MySqlIO;

extern RecoModuleHandle RWmodule_handle;

extern CSmartVillageDlg *DlgMain;

extern TCHAR CurrentDir[ZOG_MAX_PATH_STR];

#if OPEN_FACEDETECT_CODE

int myRecoSearchCallback(RecoFaceMngHandle handle,long long person_id,long long feature_id,void* person_data,void* condition)
{
	return 1;
}

CFaceRecg::CFaceRecg(void)
{
	faceFecgLock = false;
	common_handle = NULL;
	faceMngHandle = NULL;
	tempMngHandle = NULL;

	real_match_list=(RecoFeatMatchRes *)calloc(MAX_MATCH_LIST_SIZE,sizeof(RecoFeatMatchRes));//实时比对结果
	temp_match_list=(RecoFeatMatchRes *)calloc(MAX_MATCH_LIST_SIZE,sizeof(RecoFeatMatchRes));//图片人脸检索比对结果结果
	
}

CFaceRecg::~CFaceRecg(void)
{
	if(real_match_list)
	{
		free(real_match_list);
		real_match_list=NULL;
	}
	if(temp_match_list)
	{
		free(temp_match_list);
		temp_match_list=NULL;
	}
}

//********************************线程**********************************
//人脸比对线程
DWORD WINAPI FaceRecgThread(void *p)
{
	CFaceRecg * pFaceRecg = (CFaceRecg*)p;

	struct FACE_CAPTURE_ST tempFace = {0};
	struct UNCMP_FACE_INFO_ST cmpFace = {0};

	while(pFaceRecg->m_bThreadWork)
	{
		try
		{
			memset(&tempFace,0,sizeof(struct FACE_CAPTURE_ST));
			memset(&cmpFace,0,sizeof(struct UNCMP_FACE_INFO_ST));

			if(pFaceRecg->GetUnCmpFaceFromMySQL(cmpFace,tempFace))
			{
				pFaceRecg->StartCompareWithBlack(cmpFace,tempFace);
			}
			if(pFaceRecg->m_bThreadWork)
				Sleep(3000);
		}
		catch(...)
		{
			continue;
		}
	}

	return 0;
}

RecoImage* CFaceRecg::GetImage(HBITMAP pic)
{
	//从资源中载入位图     
	CBitmap    bitmapq;     
	bitmapq.Attach(pic);    
	BITMAP bmp;
	bitmapq.GetBitmap(&bmp);
//bitmapq.Detach();

	RecoImage *image;

	if (pic == NULL) {
		return NULL;
	}

	image = new RecoImage;
	if (image == NULL) {
		return NULL;
	}
	memset(image, 0, sizeof(RecoImage));
	image->width = bmp.bmWidth;
	image->height = bmp.bmHeight;
	image->widthStep = bmp.bmWidthBytes;
	image->image_size = bmp.bmHeight*bmp.bmWidthBytes;
	image->imagedata = new unsigned char[image->image_size];
	if (image->imagedata == NULL) {
		bitmapq.Detach();
		delete image;
		return NULL;
	}

	bitmapq.GetBitmapBits(image->image_size,image->imagedata);
	//memcpy(image->imagedata, bmp.bmBits, image->image_size);
	bitmapq.Detach();
	return image;
}

int CFaceRecg::SaveImage(const char *file, const RecoImage *image)
{
	unsigned char *jpegBuffer;
	unsigned long jpegSize;

	jpegBuffer = (unsigned char *)calloc(image->image_size,sizeof(unsigned char));
	EncodeToJPEGBuffer(image->imagedata,image->width,image->height,jpegBuffer,&jpegSize);
	FILE *fp = fopen(file,"wb");
	if(fp)
	{
		fwrite(jpegBuffer,jpegSize,1,fp);
		fclose(fp);
	}
	free(jpegBuffer);
	jpegBuffer = NULL;

	return 0;
}

void CFaceRecg::ReleaseImage(RecoImage *image)
{
	if (image != NULL) {
		if (image->imagedata != NULL) {
			delete []image->imagedata;
		}
		delete image;
	}
}


/************************************
* 初始化
*************************************/
int CFaceRecg::RWFaceRecg_BlackInit(void)
{
	faceImg_width = 0;
	faceImg_height = 0;
	faceImg_wStep = 0;
	faceImg_size = 0;
	faceFeatrue_size = 0;

	//SetCurrentDirectory(CurrentDir);
	common_handle = FaceProcess_Init(RWmodule_handle);
	if (common_handle == NULL)
	{
		return RWRACE_PROCESS_INIT_ERROR;
	}

	if (0 != RecoFaceProcess_GetFeatureParam(common_handle, &faceFeatrue_size))
	{
		FaceProcess_UnInit(common_handle);
		return RWRACE_FEATURE_INIT_ERROR;
	}

	if (0 != RecoFaceProcess_GetFaceImageParam(common_handle, &faceImg_width, &faceImg_height, &faceImg_wStep, &faceImg_size))
	{
		return RWFACE_IMAGE_INIT_ERROR;
	}

	if (RecoFaceMng_Init() != 0)
	{
		return RWFACE_MANAGE_INIT_ERROR;
	}
	SetCurrentDirectory(CurrentDir);
	faceMngHandle = RecoFaceMng_CreateDBHandle("RWBlack.db", common_handle);
	if (faceMngHandle == NULL)
	{
		return RWFACE_DATABASE_INIT_ERROR;
	}
	SetCurrentDirectory(CurrentDir);
	tempMngHandle = RecoFaceMng_CreateDBHandle("RWTemp.db", common_handle);
	if (tempMngHandle == NULL)
	{
		return RWFACE_DATABASE_INIT_ERROR;
	}

	return RWFACE_INIT_DONE;
}

int CFaceRecg::RWFaceRecg_UnInit(void)
{	
	if(common_handle)
	{
		FaceProcess_UnInit(common_handle);
		common_handle=0;
	}

	RecoFaceMng_DestroyDBHandle(faceMngHandle);
	RecoFaceMng_DestroyDBHandle(tempMngHandle);
	RecoFaceMng_UnInit();
	return 0;
}

/************************************
* 启动人脸比对线程
*************************************/
int CFaceRecg::StartRecgThread(void)
{
	if(RWFaceRecg_BlackInit() == RWFACE_INIT_DONE)
	{	
		m_bThreadWork = true;
		m_pThreadRecognize=NULL;
		m_pThreadRecognize=CreateThread(NULL,0,FaceRecgThread,this,0,NULL);

		return 1;
	}
	return 0;
}

/************************************
* 停止人脸比对线程
*************************************/
int CFaceRecg::StopRecgThread(void)
{
	if(m_bThreadWork)
	{
		m_bThreadWork = false;
		Sleep(1000);

		TerminateThread(m_pThreadRecognize,0);
		m_pThreadRecognize=NULL;

		Sleep(1000);

		return 1;
	}
	return 0;
}

bool CFaceRecg::RWFaceMngFaceCompare(int &match_list_size,RecoFeatMatchRes *match_res_list,long dbID,int max_match_count,double min_similarity,char *path)
{
	while(faceFecgLock)
	{
		Sleep(10);
	}
	faceFecgLock=true;

	CPicture pic;
	HBITMAP bim;
	bim = pic.LoadPicture(path);
	RecoImage *pRecoImg = GetImage(bim);
	if(pRecoImg)
	{
		if (RecoFaceMng_SearchDBPerson(faceMngHandle, dbID, *pRecoImg, max_match_count, min_similarity, 
			myRecoSearchCallback,this,&match_list_size, match_res_list) != 0)
		{
			faceFecgLock=false;
			return false;
		}
		else
		{
			if(match_list_size > 0)
			{
				faceFecgLock=false;
				return true;
			}
			else
			{
				faceFecgLock=false;
				return false;
			}
		}
	}
	faceFecgLock=false;
	return false;
}

bool CFaceRecg::RWFcaeMngTempCompare(int &match_list_size,RecoFeatMatchRes *match_res_list,long dbID,int max_match_count,double min_similarity,char *path)
{
	while(faceFecgLock)
	{
		Sleep(10);
	}
	faceFecgLock=true;

	CPicture pic;
	HBITMAP bim;
	bim = pic.LoadPicture(path);
	RecoImage *pRecoImg = GetImage(bim);
	if(pRecoImg)
	{
		if (RecoFaceMng_SearchDBPerson(tempMngHandle, dbID, *pRecoImg, max_match_count, min_similarity, 
			myRecoSearchCallback,this,&match_list_size, match_res_list) != 0)
		{
			faceFecgLock=false;
			return false;
		}
		else
		{
			if(match_list_size > 0)
			{
				faceFecgLock=false;
				return true;
			}
			else
			{
				faceFecgLock=false;
				return false;
			}
		}
	}
	faceFecgLock=false;
	return false;
}

bool CFaceRecg::GetUnCmpFaceFromMySQL(struct UNCMP_FACE_INFO_ST &cmpFace,struct FACE_CAPTURE_ST &tempFace)
{
	bool ret = MySqlIO.FACE_Capture_GetUndealFace(tempFace);
	if(!ret)
	{
		return false;
	}

	struct DEVICE_SET_ST CamSet = {0};
	ret = MySqlIO.DEVICE_SetTable_Read(tempFace.ncamera,CamSet);
	if(!ret)
	{
		return false;
	}

	cmpFace.capture_nid = tempFace.nid;
	cmpFace.ncamera = tempFace.ncamera;
	cmpFace.size = tempFace.facesize;
	strcpy(cmpFace.path,tempFace.face);
	cmpFace.ncamera = tempFace.ncamera;
	strcpy(cmpFace.cameraName,tempFace.cameraName);
	strcpy(cmpFace.sip,tempFace.sip);
	cmpFace.maxcount = CamSet.face.maxcount;
	cmpFace.threshold = CamSet.face.threshold;
	cmpFace.dbTotal = CamSet.face.dbTotal;
	for(int i=0;i<cmpFace.dbTotal;i++)
	{
		cmpFace.db_nid[i] = CamSet.face.db_nid[i];
	}

	return true;;
}

void CFaceRecg::StartCompareWithBlack(struct UNCMP_FACE_INFO_ST cmpFace,struct FACE_CAPTURE_ST tempFace)
{
	int list_size;
	struct FACE_ALARM_ST alarmFace = {0};
	bool ret = false;

	for(int i=0;i<cmpFace.dbTotal;i++)
	{
		long RW_dbid;
		MySqlIO.FACE_BlackDB_GetRWID(cmpFace.db_nid[i],RW_dbid);

		list_size = 0;
		for(int k=0;k<MAX_MATCH_LIST_SIZE;k++)
		{
			memset(&real_match_list[k],0,sizeof(RecoFeatMatchRes));
		}
		
		ret = RWFaceMngFaceCompare(list_size,real_match_list,RW_dbid,cmpFace.maxcount,cmpFace.threshold,cmpFace.path);
		if(ret)
		{
			for(int j=0;j<list_size;j++)
			{
				memset(&alarmFace,0,sizeof(FACE_ALARM_ST));
				alarmFace.captureID = cmpFace.capture_nid;
				alarmFace.db_nid = cmpFace.db_nid[i];
				alarmFace.RW_dbID = RW_dbid;
				alarmFace.RW_faceID = real_match_list[j].feature_id;//特征ID与FaceID是一样的
				alarmFace.RW_personID = real_match_list[j].person_id;
				alarmFace.similarity =(int) real_match_list[j].similarity;
				alarmFace.ncamera = cmpFace.ncamera;
				strcpy(alarmFace.cameraName,cmpFace.cameraName);
				strcpy(alarmFace.sip,cmpFace.sip);
				MySqlIO.FACE_Alarm_AddNew(alarmFace);

			}
		}
	}
	//有黑名单
	if(ret)
	{
		MySqlIO.FACE_Capture_UpdateBlackFlag(cmpFace.capture_nid);

		//报警弹
		if(	DlgMain->DlgTabVideo.DlgPictureFaceAlarm.DlgPLiteTotal >=50)
			DlgMain->DlgTabVideo.DlgPictureFaceAlarm.SendMessage(WM_CLEANFACE_MESSAGE,NULL,NULL);

		int j=FLAG_FACE_ALARM;
		DlgMain->DlgTabVideo.DlgPictureFaceAlarm.SendMessage(WM_ADDFACE_MESSAGE,(WPARAM)(&j),(LPARAM)&tempFace);
	}
	MySqlIO.FACE_Capture_UpdateDealFlag(cmpFace.capture_nid);
}

bool CFaceRecg::ImageCmpWihtTempFace(char *path,struct TEMP_CMP_CONDITION_ST condition,list<struct TEMP_CMP_RESULT_ST> &faceList)
{
	int list_size = 0;
	struct FACE_CAPTURE_ST tempFace = {0};
	struct TEMP_CMP_RESULT_ST tempResult = {0};
	
	if((condition.flag&0x01) != 0)//指定摄像头检索比对
	{
		long RW_tempDB;
		MySqlIO.TEMP_DB_Get_RWtempID(condition.ncamera,RW_tempDB);

		list_size = 0;
		for(int i=0;i<MAX_MATCH_LIST_SIZE;i++)
		{
			memset(&temp_match_list[i],0,sizeof(RecoFeatMatchRes));
		}

		bool ret = RWFcaeMngTempCompare(list_size,temp_match_list,RW_tempDB,condition.max_match_count,condition.min_similarity,path);
		if(ret)
		{
			for(int j=0;j<list_size;j++)
			{
				memset(&tempFace,0,sizeof(FACE_CAPTURE_ST));
				memset(&tempResult,0,sizeof(TEMP_CMP_RESULT_ST));
				if(MySqlIO.FACE_Capture_CmpRead(tempFace,RW_tempDB,temp_match_list[j].person_id,temp_match_list[j].feature_id,
												condition.sex,condition.ncamera,condition.flag,condition.stime,condition.etime))
				{
					memcpy(&tempResult.tempface,&tempFace,sizeof(FACE_CAPTURE_ST));
					tempResult.similarity = (int)temp_match_list[j].similarity;
					faceList.push_back(tempResult);
				}
			}
			if(faceList.size() > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
			return false;
	}
	else
	{
		list<struct FACE_TEMP_DB_ST>TempDBList;
		TempDBList.clear();
		MySqlIO.TEMP_DB_ReadAll(TempDBList);

		int average_count = 0;
		if(TempDBList.size()>0)
			average_count = (condition.max_match_count/TempDBList.size() == 0) ? 1 : condition.max_match_count/TempDBList.size();
		else
			return false;
		list<struct FACE_TEMP_DB_ST>::iterator beglist;
		for(beglist=TempDBList.begin();beglist!=TempDBList.end();beglist++)
		{
			list_size = 0;
			for(int i=0;i<MAX_MATCH_LIST_SIZE;i++)
			{
				memset(&temp_match_list[i],0,sizeof(RecoFeatMatchRes));
			}

			bool ret = RWFcaeMngTempCompare(list_size,temp_match_list,beglist->RW_tempID,average_count,condition.min_similarity,path);
			if(ret)
			{
				for(int j=0;j<list_size;j++)
				{
					memset(&tempFace,0,sizeof(FACE_CAPTURE_ST));
					memset(&tempResult,0,sizeof(TEMP_CMP_RESULT_ST));
					if(MySqlIO.FACE_Capture_CmpRead(tempFace,beglist->RW_tempID,temp_match_list[j].person_id,temp_match_list[j].feature_id,
						condition.sex,beglist->ncamera,condition.flag,condition.stime,condition.etime))
					{
						memcpy(&tempResult.tempface,&tempFace,sizeof(FACE_CAPTURE_ST));
						tempResult.similarity = (int)temp_match_list[j].similarity;
						faceList.push_back(tempResult);

						if(faceList.size() == condition.max_match_count)
							return true;
					}
				}
			}
		}
		if(faceList.size() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool CFaceRecg::ImageCmpWihtBlackFace(char *path,struct BLACK_CMP_CONDITION_ST condition,list<struct BLACK_CMP_RESULT_ST> &faceList)
{
	int list_size = 0;
	struct PERSON_BLACK_ST person = {0};
	struct FACE_BLACK_ST face = {0};
	struct BLACK_CMP_RESULT_ST tempResult = {0};

	if((condition.flag&0x10) != 0)//指定布控库进行比对
	{
		long RW_tempDB;
		MySqlIO.FACE_BlackDB_GetRWID(condition.db_id,RW_tempDB);

		list_size = 0;
		for(int i=0;i<MAX_MATCH_LIST_SIZE;i++)
		{
			memset(&temp_match_list[i],0,sizeof(RecoFeatMatchRes));
		}

		bool ret = RWFaceMngFaceCompare(list_size,temp_match_list,RW_tempDB,condition.max_match_count,condition.min_similarity,path);
		if(ret)
		{
			for(int j=0;j<list_size;j++)
			{
				memset(&person,0,sizeof(PERSON_BLACK_ST));
				memset(&face,0,sizeof(FACE_BLACK_ST));
				memset(&tempResult,0,sizeof(BLACK_CMP_RESULT_ST));
				if(MySqlIO.FACE_BlackPerson_CmpRead(person,condition.name,condition.sex,condition.sage,condition.eage,condition.cardtype,condition.cardNO,
					condition.phone,condition.native,condition.address,condition.flag,condition.db_id,RW_tempDB,temp_match_list[j].person_id))
				{
					MySqlIO.FACE_BlackFace_ReadWithRWfaceID(temp_match_list[j].feature_id,temp_match_list[j].person_id,face);
					memcpy(&tempResult.person,&person,sizeof(PERSON_BLACK_ST));
					memcpy(&tempResult.face,&face,sizeof(FACE_BLACK_ST));

					tempResult.similarity = (int)temp_match_list[j].similarity;
					faceList.push_back(tempResult);
				}
			}
			if(faceList.size() > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		list<struct FACE_DB_ST> BlaceDBList;
		BlaceDBList.clear();
		MySqlIO.FACE_BlackDB_ReadAll(BlaceDBList);

		int average_count = 0;
		if(BlaceDBList.size() > 0)
			average_count = (condition.max_match_count/BlaceDBList.size() == 0) ? 1 : condition.max_match_count/BlaceDBList.size();
		else
			return false;
		list<struct FACE_DB_ST>::iterator beglist;
		for(beglist=BlaceDBList.begin();beglist!=BlaceDBList.end();beglist++)
		{
			list_size = 0;
			for(int i=0;i<MAX_MATCH_LIST_SIZE;i++)
			{
				memset(&temp_match_list[i],0,sizeof(RecoFeatMatchRes));
			}
			bool ret = RWFaceMngFaceCompare(list_size,temp_match_list,beglist->RW_dbID,average_count,condition.min_similarity,path);
			if(ret)
			{
				for(int j=0;j<list_size;j++)
				{
					memset(&person,0,sizeof(PERSON_BLACK_ST));
					memset(&face,0,sizeof(FACE_BLACK_ST));
					memset(&tempResult,0,sizeof(BLACK_CMP_RESULT_ST));

					if(MySqlIO.FACE_BlackPerson_CmpRead(person,condition.name,condition.sex,condition.sage,condition.eage,condition.cardtype,condition.cardNO,
						condition.phone,condition.native,condition.address,condition.flag,beglist->nid,beglist->RW_dbID,temp_match_list[j].person_id))
					{
						MySqlIO.FACE_BlackFace_ReadWithRWfaceID(temp_match_list[j].feature_id,temp_match_list[j].person_id,face);
						memcpy(&tempResult.person,&person,sizeof(PERSON_BLACK_ST));
						memcpy(&tempResult.face,&face,sizeof(FACE_BLACK_ST));

						tempResult.similarity =(int) temp_match_list[j].similarity;
						faceList.push_back(tempResult);
						if(faceList.size() == condition.max_match_count)
							return true;
					}
				}
			}
		}
		if(faceList.size() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

#endif