//lauplaym4.h
#ifndef __LAUPLAYM4_H__
#define __LAUPLAYM4_H__

#define CPLAY_APIIMPORT  extern "C"__declspec(dllexport)

#define MAX_DISPLAY_RECT    4

enum{
	LUPLAYMSG_END = 1,
	LUPLAYMSG_START,
	LUPLAYMSG_STARTSTAY,
	LUPLAYMSG_STOPSTAY,
	LUPLAYMSG_UPDATESTREAMINFO
};

typedef struct{
	int			  m_year;		//year
	unsigned char m_month;		//month
	unsigned char m_day;		//day
	unsigned char m_hour;		//hour
	unsigned char m_minute;		//minute
	unsigned char m_second;		//second
	unsigned char Received[3];
}CPLAY_NETTIME;

typedef struct
{
	char        sername[40];    //server name,to distinguish the different devices
	int         channel;        //channel number
    int         dwRecvType;     //record type:0xff-all,0x01-manually record,0x02-timed record,0x04-video move record,0x08-sensor alarm record,
                                //0x10-video lost record,0x20-video hide record,0x40-other record
	char        sFileName[256]; //filename
	unsigned int dwFileSize;    //file size
	CPLAY_NETTIME StartTime;    //start time
	CPLAY_NETTIME StopTime;     //end time
    int         dwIsLocked;     //is file locked:0-unlocked,1-locked
    int         auxtype;        //use extra data:0-nouse,nonzero-use
    char        auxdata[64];    //extra data
}NDVRFILE_FILEINFO;

typedef struct
{
	char        sername[40];    //server name,to distinguish the different devices
	int         channel;        //channel number
    int         dwFileType;     //file type:1-MP4,2-JPEG
    int         dwRecvType;     //record type:0xff-all,0x01-manually record,0x02-timed record,0x04-video move record,0x08-sensor alarm record,
                                //0x10-video lost record,0x20-video hide record,0x40-other record
    int         dwIsLocked;     //is file locked:0-unlocked,1-locked
    BOOL        bTimelimite;    //is time limite
	CPLAY_NETTIME  StartTime;   //start time
	CPLAY_NETTIME  StopTime;    //end time
    int         auxtype;        //use extra data:0-nouse,nonzero-use
    char        auxdata[64];    //extra data
	int			disktype;		//disk type: 5(1<<0+1<<2):record disk, 6(1<<1+1<<2):redundancy disk, 7(1<<0+1<<1+1<<2):record and redundancy.
}NDVRFILE_FINDINFO;

typedef void(WINAPI *cDVRsearchcallback)(NDVRFILE_FILEINFO *pfileinfo,void *userdata);
typedef void(WINAPI *cplaysearchcallback)(char *m_sername,int  m_channel,char *sFileName,CPLAY_NETTIME *pStartTime,CPLAY_NETTIME *pStopTime,DWORD dwFileSize,void *userdata);
typedef void(WINAPI *cplaydownloadcallback)(LONG lFileHandle,char *pbuff,int size,void *context);
//init and free
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_InitPlayer(char *key = NULL);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_FreePlayer();
//play and control
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_OpenFile(LPSTR sFileName,BOOL m_createindex);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_RegEventMsgWnd(LONG hHandle,HWND hWnd,UINT msgid);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_Play(LONG hHandle, HWND hWnd);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_Stop(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_Pause(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_Fast(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_Slow(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_OneByOne(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_OneByOneBack(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_FlipVideo(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_SetPlayPos(LONG hHandle,float fRelativePos);
CPLAY_APIIMPORT float  __stdcall LCPLAYM4_GetPlayPos(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_GetPictureSize(LONG hHandle,int *pWidth,int *pHeight);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_HaveAudio(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_SetVolume(LONG hHandle,WORD nVolume);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_PlaySound(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_StopSound(LONG hHandle);
CPLAY_APIIMPORT DWORD  __stdcall LCPLAYM4_GetFileTime(LONG hHandle,DWORD *firstGMTTime = NULL);
CPLAY_APIIMPORT DWORD  __stdcall LCPLAYM4_GetPlayedTime(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_SetPlayedTime(LONG hHandle,DWORD nTime);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_SetPlayedGMTTime(LONG hHandle,DWORD nGMTTime);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_PlaySetWnd(LONG hHandle,HWND hWnd);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_RefreshDisplay(LONG hHandle,RECT *rect = NULL);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_CaptureBmp(LONG hHandle,LPSTR sFileName);
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_StartASFConvert(LPSTR mp4FileName,LPSTR asfFileName);
CPLAY_APIIMPORT int    __stdcall LCPLAYM4_GetASFConvertPos(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_StopASFConvert(LONG hHandle);
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_StartAVIConvert(LPSTR mp4FileName,LPSTR aviFileName);
CPLAY_APIIMPORT int    __stdcall LCPLAYM4_GetAVIConvertPos(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_StopAVIConvert(LONG hHandle);
CPLAY_APIIMPORT DWORD  __stdcall LCPLAYM4_GetPlayedGMTTime(LONG hHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_SetDisplayRect(LONG hHandle,int num,RECT *rect,HWND hWnd,BOOL bEnable);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_GetVideoSize(LONG hHandle,DWORD *Width,DWORD *Height);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_PreventImageSplit(LONG hHandle, BOOL bsplit);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_DrawCallback(LONG hHandle,void(WINAPI *DrawCallBack)(HDC hDC,void *context),void *context);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_SharpeVideo(LONG hHandle, int SharpeStrength);
//file cut
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_SetFileCutStart(LONG hHandle,LPSTR strFileName);
CPLAY_APIIMPORT int    __stdcall LCPLAYM4_SetFileCutEnd(LONG hHandle);
//remote search,play,download
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_nFileFindFile(char *m_url,char *m_sername,int m_channel,char *m_username,char *m_password,DWORD dwFileType,DWORD dwRecvType,BOOL bTimelimite,CPLAY_NETTIME *lpStartTime,CPLAY_NETTIME *lpStopTime,cplaysearchcallback callback,void *userdata,WORD wserport);
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_nDVRFileFindFile(char *m_url,char *m_username,char *m_password,NDVRFILE_FINDINFO *pfindinfo,cDVRsearchcallback callback,void *userdata,WORD wserport);
CPLAY_APIIMPORT int    __stdcall LCPLAYM4_nFileFindGetState(LONG lFindHandle);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_nFileFindClose(LONG lFindHandle);
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_nFileDownLoadFileByName(char *m_url,char *m_username,char *m_password,LPSTR sRemoveFile,LPSTR sSavedFileName,WORD wserport,int ratelimit=0,cplaydownloadcallback callback = NULL,void *context = NULL,char *sername = NULL);
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_nFileDownLoadFileByTime(char *m_url,char *m_username,char *m_password,int channel,CPLAY_NETTIME *StartTime,CPLAY_NETTIME *StopTime,LPSTR sSavedFileName,WORD wserport,int ratelimit=0,cplaydownloadcallback callback = NULL,void *context = NULL,char *sername = NULL);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_nFileStopDownloadFile(LONG lFileHandle);
CPLAY_APIIMPORT int    __stdcall LCPLAYM4_nFileGetDownloadPos(LONG lFileHandle);
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_nFilePlayBackByName(char *m_url,char *m_username,char *m_password,LPSTR sRemoveFile, HWND hWnd,WORD wserport,char *sername = NULL);
CPLAY_APIIMPORT LONG   __stdcall LCPLAYM4_nFilePlayBackByTime(char *m_url,char *m_username,char *m_password,int channel,CPLAY_NETTIME *StartTime,CPLAY_NETTIME *StopTime,HWND hWnd,WORD wserport,char *sername = NULL);
CPLAY_APIIMPORT BOOL   __stdcall LCPLAYM4_nFilePlayBackStayTime(LONG hHandle,DWORD dwStayTime);
CPLAY_APIIMPORT int    __stdcall LCPLAYM4_nFilePlayBackGetStayPos(LONG hHandle);
CPLAY_APIIMPORT int    __stdcall LCPLAYM4_nFileRemoteBackup(char *m_sername, char *m_url,char *m_username,char *m_password, WORD m_serport, int m_bkType, LPSTR m_remotefile, int m_iFilesize = 0);


#endif//__LAUPLAYM4_H__
