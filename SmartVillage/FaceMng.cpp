#include "stdafx.h"
#include "FaceMng.h"
#include "YUV2RGB.h"

extern TCHAR CurrentDir[ZOG_MAX_PATH_STR];

#if OPEN_FACEDETECT_CODE



extern RecoModuleHandle RWmodule_handle;

CFaceMng::CFaceMng(void)
{
	common_handle = NULL;
	faceMngHandle = NULL;
	mySqliteLock = false;
}

CFaceMng::~CFaceMng(void)
{
	//RWFaceMng_UnInit();
}

RecoImage* CFaceMng::GetImage(HBITMAP pic)
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

int CFaceMng::SaveImage(const char *file, const RecoImage *image)
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

void CFaceMng::ReleaseImage(RecoImage *image)
{
	if (image != NULL) {
		if (image->imagedata != NULL) {
			delete []image->imagedata;
		}
		delete image;
	}
}

int CFaceMng::RWFaceMng_BlackInit(void)
{
	RecoCommonHandle common_handle = NULL;
	faceImg_width = 0;
	faceImg_height = 0;
	faceImg_wStep = 0;
	faceImg_size = 0;
	faceFeatrue_size = 0;

//	SetCurrentDirectory(CurrentDir);
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

	faceMngHandle = RecoFaceMng_CreateDBHandle("RWBlack.db", common_handle);
	if (faceMngHandle == NULL)
	{
		return RWFACE_DATABASE_INIT_ERROR;
	}

	return RWFACE_INIT_DONE;
}

int CFaceMng::RWFaceMng_TempInit(void)
{
RecoCommonHandle common_handle = NULL;
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
	faceMngHandle = RecoFaceMng_CreateDBHandle("RWTemp.db", common_handle);
	if (faceMngHandle == NULL)
	{
		return RWFACE_DATABASE_INIT_ERROR;
	}

	return 0;
}

int CFaceMng::RWFaceMng_UnInit(void)
{	
	
	if(common_handle)
{
	FaceProcess_UnInit(common_handle);
	common_handle=0;
}

	RecoFaceMng_DestroyDBHandle(faceMngHandle);
	RecoFaceMng_UnInit();
	return 0;
}

bool CFaceMng::RWFaceMngDbAddNew(long &dbID,char *dbName)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	long long id;
	if(RecoFaceMng_CreateDB(faceMngHandle, dbName, (long long)id) != 0)
	{
		dbID = 0;
		mySqliteLock = false;
		return false;
	}
	else
	{
		dbID = id;
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngDbDeleteWithID(long dbID)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	if (RecoFaceMng_DeleteDB(faceMngHandle, dbID) != 0)
	{
		mySqliteLock = false;
		return false;
	}
	else
	{
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngPersonAddNew(long &personID,RecoPersonInfo pInfo)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	if (RecoFaceMng_AddDBPerson(faceMngHandle, pInfo) != 0)
	{
		personID = 0;
		mySqliteLock = false;
		return false;
	}
	else
	{
		personID = pInfo.person_id;
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngPersonDeleteWithID(long personID)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	if (RecoFaceMng_DeleteDBPerson(faceMngHandle, personID) != 0)
	{
		mySqliteLock = false;
		return false;
	}
	else
	{
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngPersonUpdate(RecoPersonInfo pInfo)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	if (RecoFaceMng_UpdateDBPerson(faceMngHandle, pInfo) != 0)
	{
		mySqliteLock = false;
		return false;
	}
	else
	{
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngPersonRead(long personID,RecoPersonInfo &pInfo)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	if (RecoFaceMng_GetDBPersonInfo(faceMngHandle, personID, pInfo) != 0)
	{
		mySqliteLock = false;
		return false;
	}
	else
	{
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngFaceAddNew(long &faceID,long personID,HBITMAP pic)
{
	if(pic == NULL)
		return false;
	RecoImage *pRecoImg = GetImage(pic);

	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;
	
	//RecoImage *pRecoImg = GetImage(path);
	if (pRecoImg)
	{
		RecoFaceInfo info;
		info.face_img.width = pRecoImg->width;
		info.face_img.height = pRecoImg->height;
		info.face_img.widthStep = pRecoImg->widthStep;
		info.face_img.image_size = pRecoImg->image_size;
		info.face_img.imagedata = pRecoImg->imagedata;
		if (RecoFaceMng_AddDBFace(faceMngHandle, personID, info) != 0 )
		{
			faceID = 0;
			ReleaseImage(pRecoImg);
			mySqliteLock = false;
			return false;
		}
		else
		{
			faceID = info.face_id;
			ReleaseImage(pRecoImg);
			mySqliteLock = false;
			return true;
		}
	}
	mySqliteLock = false;
	return false;
}

bool CFaceMng::RWFaceMngFaceDeleteWithID(long faceID)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	if (RecoFaceMng_DeleteDBFace(faceMngHandle, faceID) != 0)
	{
		mySqliteLock = false;
		return false;
	}
	else
	{
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngGetFaceImage(long faceID,long &size,char *path)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	RecoFaceInfo info;
	info.face_img.imagedata = new unsigned char[faceImg_size];
	info.face_img.width = faceImg_width;
	info.face_img.height = faceImg_height;
	info.face_img.widthStep = faceImg_wStep;
	info.face_img.image_size = faceImg_size;
	if (0 != RecoFaceMng_GetDBFaceInfo(faceMngHandle, faceID, info))
	{
		delete[] info.face_img.imagedata;
		mySqliteLock = false;
		return false;
	}
	else
	{
		SaveImage(path, &info.face_img);
		size = info.face_img.image_size;
		delete[] info.face_img.imagedata;
		mySqliteLock = false;
		return true;
	}
}

bool CFaceMng::RWFaceMngRegistTempPerson(long dbID,int age,char *sex,int width,int height,int step,
										 unsigned char *image,long size,long &personID,long &faceID)
{
	while(mySqliteLock)
	{
		Sleep(10);
	}
	mySqliteLock=true;

	RecoPersonInfo pInfo = {0};
	strcpy(pInfo.gender,sex);
	pInfo.birthday = age;
	pInfo.db_id = dbID;
	
	if (RecoFaceMng_AddDBPerson(faceMngHandle, pInfo) != 0)
	{
		mySqliteLock = false;
		return false;
	}
	else
	{
		RecoFaceInfo info;
		info.face_img.width = width;
		info.face_img.height = height;
		info.face_img.widthStep = step;
		info.face_img.image_size = size;
		info.face_img.imagedata = image;
		if (RecoFaceMng_AddDBFace(faceMngHandle, pInfo.person_id, info) != 0 )
		{
			mySqliteLock = false;
			return false;
		}
		else
		{
			personID = pInfo.person_id;
			faceID = info.face_id;
			mySqliteLock = false;
			return true;
		}
	}
}




#endif