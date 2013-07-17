//launet.h
#ifndef __LAUNET_H__
#define __LAUNET_H__

#include "VSNETStructDef.h"

#if defined(PLATFORM_WIN_X64)
    typedef __int64 INT_PFW, *PINT_PFW;
    typedef unsigned __int64 UINT_PFW, *PUINT_PFW;

#else
    typedef int INT_PFW, *PINT_PFW;
    typedef unsigned int UINT_PFW, *PUINT_PFW;

#endif // PLATFORM_WIN_X64

#ifndef CNET_APIIMPORT
#define CNET_APIIMPORT  extern "C"__declspec(dllexport)
#endif

#define LAUMSG_LINKMSG			1		//connect server link message
#define LAUMSG_VIDEOMOTION		2		//video motion alarm
#define LAUMSG_VIDEOLOST		3		//video lost alarm
#define LAUMSG_ALARM			4		//sensor alarm
#define LAUMSG_OUTPUTSTATUS		5		//alarm out status
#define LAUMSG_CURSWITCHCHAN	6		//current switch channel
#define LAUMSG_HIDEALARM		7		//video hide alarm
#define LAUMSG_CHRECORD         8       //reserved
#define LAUMSG_EACHVIDEOMOTION  9       //each video motion
#define LAUMSG_INTERNAL         10      //internal message
#define LAUMSG_SERVERRECORD     11      //server record status

#define LAUMSG_STARTCLIENTRECORD     12      //start client record
#define LAUMSG_STOPCLIENTRECORD      13      //stop client record
#define LAUMSG_CLIENTRECORDSTATUS    14      //client record status

#define LAUMSG_EXTENDALARM_TEMPERATURE 15    //temperature alarm message
#define LAUMSG_EXTENDALARM_HUMIDITY    16    //humidity alarm message
#define LAUMSG_BATTERYLESS			   17	 //battery less alarm message

#define LAUMSG_VAALARM_SINGLELINE      18    //intelligent analysis single line
#define LAUMSG_VAALARM_DOUBLELINE      19    //intellignet analysis double line
#define LAUMSG_VAALARM_REGIONENTER     20    //intellignet analysis region enter
#define LAUMSG_VAALARM_REGIONLEFT      21    //intellignet analysis region leave
#define LAUMSG_VAALARM_REGIONWANDER    22    //intellignet analysis region wander

#define LAUMSG_DISKFULL				   23	 //disk full
#define LAUMSG_DISKERROR			   24	 //disk error
#define LAUMSG_ACCESSVIOLATION		   25	 //access violation

#define LAUMSG_SERSTART				   26	 //server start
#define LAUMSG_SERSTOP				   27	 //server stop

#define LAUMSG_EXTENDALARM_VOLTAGE     28    //voltage alarm

#define LAUMSG_NODISK				   29	 //no disk
#define LAUMSG_UPDATESTREAMINFO		   30	 //update streaminfo

#define LAUMSG_SENSORPROTECTSTATUS	   31	 //sensor protect status

#define LAUMSG_CDHCPORTREFRESH		   32	 //CDHC port refresh
	
#define LAUMSG_USBSTATUS				33	 //DXHJ7304 usb
#define LAUMSG_AUDIOBESTIRSTATUS		34   //DXHJ7304 audio bestir
#define LAUMSG_DXSLARGESCWITCHCH		35	 //DX Large Screen App

#define LAUMSG_YWCODDATA               1001 //YWCOD param
#define LAUMSG_YWCODALARM_FLOW         1002 //YWCOD param
#define LAUMSG_YWCODALARM_PH           1003 //YWCOD param
#define LAUMSG_YWCODALARM_DES1         1004 //YWCOD param
#define LAUMSG_YWCODALARM_DES2         1005 //YWCOD param
#define LAUMSG_YWCODALARM_DES3         1006 //YWCOD param
#define LAUMSG_YWCODALARM_DES4         1007 //YWCOD param
#define LAUMSG_YWCODALARM_DES5         1008 //YWCOD param
#define LAUMSG_YWCODALARM_DES6         1009 //YWCOD param
#define LAUMSG_YWCODALARM_DES7         1010 //YWCOD param
#define LAUMSG_YWCODALARM_DES8         1011 //YWCOD param
#define LAUMSG_YWCODALARM_DES9         1012 //YWCOD param
#define LAUMSG_YWCODALARM_DES10        1013 //YWCOD param
#define LAUMSG_YWCODALARM_DES11        1014 //YWCOD param
#define LAUMSG_YWCODALARM_DES12        1015 //YWCOD param

#define LAUMSG_BLACKREPORT             2001 //black report

#define LAUMSG_SLOTUPDATE              8001
#define LAUMSG_SLOTDISCNT              8002
/********************behind handle**************/
#define CYN_PP_DEBLOCK_Y	0X00000001
#define CYN_PP_DEBLOCK_UV	0X00000002
#define CYN_PP_FILMEFFECT   0X00000004
#define CYN_PP_DERING_UV	0X00000008
#define CYN_PP_DERING_Y  	0X00000010
/********************behind handle**************/
typedef int (WINAPI *DevStatusCallback)(BOOL bOnLine,LONG handle,DWORD dwChannelNum,char *ip,WORD port,
                                        char *pszDeviceName,char *pserialno,unsigned char pyhmac[6],void *context);

typedef int(WINAPI *psercheckcallback)(const char *m_psername,const char *m_url,WORD m_port,WORD m_webport,const char *m_serialno,int m_sertype,int *pLoopView,BOOL bLogin,int *streamtype,LONG hDevHandle);
typedef int(WINAPI *pserchancallback)(const char *m_psername,const char *m_url,int m_ch,BOOL bLogin,BOOL *m_bPreRecord,int *m_framecount);

typedef int(WINAPI *pvsnetchancallback)(const char *sername, const char *url, WORD port,
										const char *serialno, unsigned char pyhmac[6],
										int channels, int AlarmInNum, int AlarmOutNum,
										int ch, BOOL bLogin, LONG hChHandle);
typedef void(WINAPI *pvsnetalarmcallback)(LONG hHandle,int wParam,int lParam,void *context);
typedef void(WINAPI *pRecordIFrameCallback)(unsigned int nTimeTick, unsigned int nGMTTime, unsigned int RecordOffset, BOOL bFirst, void *context);

typedef struct{
	char *m_sername;                    //server name
	char *m_username;                   //user name
	char *m_password;                   //password
	WORD m_tranType;                    //transmit type   
	WORD m_playstart;                   //is start view
	BYTE m_ch;                          //channel
	HWND m_hVideohWnd;                  //video wnd handle
	HWND m_hChMsgWnd;                   //message wnd handle
	UINT m_nChmsgid;                    //message id  
	int  m_buffnum;                     //buff num
	int  m_useoverlay;                  //is use overlay
	COLORREF nColorKey;                 //color key(reserved)
	char url[40];                       //url
	void (WINAPI *m_messagecallback)(LONG hHandle,int wParam,int lParam,void *context);
	                                    //message callback function
	void *context;                      //message callback context
}CHANNEL_CLIENTINFO;

typedef struct{
	char *m_sername;                    //server name
	char *m_username;                   //user name
	char *m_password;                   //password
	WORD m_tranType;                    //transmit type 
	HWND m_hChMsgWnd;                   //message wnd handle
	UINT m_nChmsgid;                    //message id 
	int  m_sockType;                    //sock type
	int  m_errormsg;                    //error message   //-1 server information error; -2 link error; -3 have no sock type cmd; -4 recve error; -5 error code; -6 threed start failed;
	void (WINAPI *m_messagecallback)(LONG hHandle,int wParam,int lParam,void *context);
	//message callback function
	void *context;                      //message callback context
}SERVER_UPDATEINFO;

typedef struct
{
	char m_multiip[16];                 //multicast ipaddress
	WORD m_wMultiPort;                  //multicast port
	WORD m_wLocaport;                   //local port
	int  m_videobuff;                   //video buff num
	int  m_audiobuff;                   //audio buff num
	int  m_channuser;                   //channel user count
	int  m_totaluser;                   //totola user count
	int  (WINAPI *m_UserCheckcallback)(char *m_url,char *username,char *password);
	                                    //user check callback function
	int  (WINAPI *m_UserConnectcallback)(char *m_sername,int channel,BOOL bStart,char *m_userurl,char *username,char *password,int transtype,long handle);
                                        //user login logout callback function
}VSNET_REDIRECTORINFO;

typedef struct
{
	char *m_rvsbindurl;		//RVS bind url£¬NULL if all url
	WORD m_rvsbindport;		//RVS bind port
}VSNETRVSINFO;

typedef struct
{
	UINT			m_videotag;		// video tag
	WORD			m_width;		// video width
	WORD			m_height;		// video height
	UINT			m_framerate;	// framerate
	UINT			m_bitrate;		// bitrate
}CHANNEL_VIDEOINFO;

typedef struct
{
	BOOL			m_bhaveaudio;	// is have audio
	WORD			m_audiotag;		// audio tag
	WORD			m_channels;		// audio channels
	UINT			m_samplerate;	// samplerate
	UINT			m_bitrate;		// bitrate
}CHANNEL_AUDIOINFO;

typedef struct  
{
	int             m_filesize;              //file total size
	int             m_curexportsize;         //current export size
	int             m_curleadinsize;         //current leadin size
	int             m_errorcode;             // error code -1 link error;-2 date faild -3 file I/O faild -4 send/recv faild 
	void *          m_context;              
}SERVER_PARAMFILEINFO;


/*ptz conctrl code*/
enum{
	PTZ_LEFT         =  0,
	PTZ_RIGHT	     =	1,
	PTZ_UP		     =	2,
	PTZ_DOWN	     =	3,
	PTZ_IRISADD	     =	4,
	PTZ_IRISDEC	     =  5,
	PTZ_FOCUSADD     =  6,
	PTZ_FOCUSDEC     =	7,
	PTZ_ZOOMADD	     =	8,
	PTZ_ZOOMDEC	     =	9,
	PTZ_GOTOPOINT    =	10,
	PTZ_SETPOINT	 =	11,
	PTZ_AUTO		 =	12,
	PTZ_STOP		 =	13,
	PTZ_LEFTSTOP	 =	14,
	PTZ_RIGHTSTOP	 =	15,
	PTZ_UPSTOP	     =	16,
	PTZ_DOWNSTOP     =	17,
	PTZ_IRISADDSTOP	 =	18,
	PTZ_IRISDECSTOP	 =	19,
	PTZ_FOCUSADDSTOP =	20,
	PTZ_FOCUSDECSTOP =	21,
	PTZ_ZOOMADDSTOP	 =  22,
	PTZ_ZOOMDECSTOP	 =  23,
	PTZ_LIGHT		 =	24,
	PTZ_LIGHTSTOP	 =	25,
	PTZ_RAIN		 =	26,
	PTZ_RAINSTOP	 =	27,
	PTZ_TRACK		 =	28,
	PTZ_TRACKSTOP	 =	29,
	PTZ_DEVOPEN		 =	30,
	PTZ_DECCLOSE	 =	31,
	PTZ_AUTOSTOP	 =	32,
	PTZ_CLEARPOINT	 =	33,
	PTZ_LEFTUP		 =  200,
	PTZ_LEFTUPSTOP	 =  201,
	PTZ_RIGHTUP		 =  202,
	PTZ_RIGHTUPSTOP	 =  203,
	PTZ_LEFTDOWN	 =  204,
	PTZ_LEFTDOWNSTOP =  205,
	PTZ_RIGHTDOWN	 =  206,
	PTZ_RIGHTDOWNSTOP=  207,
};

enum{
	MESSAGE_SERVERCHS = 1,
	MESSAGE_CLEANALARM,
	MESSAGE_CONNECTIP,
	MESSAGE_CMD_RESET,
	MESSAGE_CMD_SHUTDOWN,
	MESSAGE_CMD_GETGLOBALPARAM,
	MESSAGE_CMD_SETGLOBALPARAM,
	MESSAGE_GETCHANNELPARAM,
	MESSAGE_SETCHANNELPARAM,
	MESSAGE_GETALARMPARAM,
	MESSAGE_SETALARMPARAM,
	MESSAGE_CMD_GETMOTIONPARAM,
	MESSAGE_CMD_SETMOTIONPARAM,
	MESSAGE_CMD_MOTIONTEST,
	MESSAGE_CMD_GETVIDEOMASK,
	MESSAGE_CMD_SETVIDEOMASK,
	MESSAGE_CMD_GETCHANNELOSD,
	MESSAGE_CMD_SETCHANNELOSD,
	MESSAGE_CMD_SAVEPARA,
	MESSAGE_CMD_PARAMDEFAULT,	
	MESSAGE_CMD_GETVIPARAM,
	MESSAGE_CMD_SETVIPARAM,
	MESSAGE_CMD_GETSERIAL,
	MESSAGE_CMD_SETSERIAL,
	MESSAGE_CMD_GETSYSTIME,
	MESSAGE_CMD_SETSYSTIME,
	MESSAGE_CMD_GETSERIALNO,
	MESSAGE_CMD_AFFIRMUSER,
	MESSAGE_CMD_GETSYSUSER,
	MESSAGE_CMD_SETSYSUSER,
	MESSAGE_CMD_OUTPUTCTRL,
	MESSAGE_CMD_TRANUART,
	MESSAGE_CMD_OUTPUTSTATUS,
	MESSAGE_CMD_EXDEVICESTATUS,
	MESSAGE_CMD_PTZCMDFILE,
	MESSAGE_CMD_PTZCMDNAME,
	MESSAGE_CMD_CURSWITCHCHAN,
	MESSAGE_CMD_SETVIDEOLOST,
	MESSAGE_CMD_GETVIDEOLOST,
	MESSAGE_CMD_UDPPORT,
	MESSAGE_CMD_SETTIMEDRESET,
	MESSAGE_CMD_GETTIMEDRESET,
	MESSAGE_CMD_SETHIDEALARM,
	MESSAGE_CMD_GETHIDEALARM,
	MESSAGE_CMD_GETREMOTEHOST,
	MESSAGE_CMD_SETREMOTEHOST,
	MESSAGE_CMD_GETSUBCHANNELPARAM,
	MESSAGE_CMD_SETSUBCHANNELPARAM,
	MESSAGE_CMD_STARTRECORDFILE,
	MESSAGE_CMD_STOPRECORDFILE,
	MESSAGE_CMD_CAPTUREJPEG,
	MESSAGE_CMD_GETDISKSTATE,
	MESSAGE_CMD_FORMATDISK,
	MESSAGE_CMD_ENCKEYFRAME,
	MESSAGE_CMD_GETPPPOEPARAM,
	MESSAGE_CMD_SETPPPOEPARAM,
	MESSAGE_CMD_GETRECORDPARAM,
	MESSAGE_CMD_SETRECORDPARAM,
	MESSAGE_CMD_GETVIDEOLOSTPARAM,
	MESSAGE_CMD_SETVIDEOLOSTPARAM,
	MESSAGE_CMD_GETJPEGCAPPARAM,
	MESSAGE_CMD_SETJPEGCAPPARAM,
	MESSAGE_CMD_GETRECORDSTATUS,
	MESSAGE_CMD_GETSENSORALARM,
	MESSAGE_CMD_SETSENSORALARM,
	MESSAGE_CMD_GETGLOBALPARAMEX,
	MESSAGE_CMD_SETGLOBALPARAMEX,
	MESSAGE_GETCHANNELPARAMEXNAME,
	MESSAGE_SETCHANNELPARAMEXNAME,
	MESSAGE_CMD_GETAUDIOPARAM,
	MESSAGE_CMD_SETAUDIOPARAM,
	MESSAGE_CMD_GETCDMAPARAM,
	MESSAGE_CMD_SETCDMAPARAM,
	MESSAGE_CMD_GETWIFIPARAM,
	MESSAGE_CMD_SETWIFIPARAM,
	MESSAGE_CMD_GETLINKCONFIG,
	MESSAGE_CMD_SETLINKCONFIG,
	MESSAGE_CMD_GETDDNSEXPARAM,
	MESSAGE_CMD_SETDDNSEXPARAM,
	MESSAGE_CMD_GETEXALARMPARAM,
	MESSAGE_CMD_SETEXALARMPARAM,
	MESSAGE_CMD_GETHUMITUREPRAM,
	MESSAGE_CMD_SETHUMITUREPRAM,
	MESSAGE_CMD_GETVIDEOOFFSET,
	MESSAGE_CMD_SETVIDEOOFFSET,
	MESSAGE_CMD_GETVIDEOMASKAREA,
	MESSAGE_CMD_SETVIDEOMASKAREA,
	MESSAGE_CMD_GETSERIAL232,
	MESSAGE_CMD_SETSERIAL232,
	MESSAGE_CMD_GETSNMPCONFIG,
	MESSAGE_CMD_SETSNMPCONFIG,
	MESSAGE_CMD_GETALARMSMS,
	MESSAGE_CMD_SETALARMSMS,
	MESSAGE_CMD_GETALARMTYPE,
	MESSAGE_CMD_SETALARMTYPE,
	MESSAGE_CMD_GETDHCP,
	MESSAGE_CMD_SETDHCP,
	MESSAGE_CMD_GETFTP,
	MESSAGE_CMD_SETFTP,
	MESSAGE_CMD_GETUPNPCFG,
	MESSAGE_CMD_SETUPNPCFG,
	MESSAGE_CMD_GETUPNPSTATUS,
	MESSAGE_CMD_GETMAILCFG,
	MESSAGE_CMD_SETMAILCFG,
	MESSAGE_CMD_GETCCDPARAM,
	MESSAGE_CMD_SETCCDPARAM,
	MESSAGE_CMD_GETPLATFORMINFO,
	MESSAGE_CMD_SETPLATFORMINFO,
	MESSAGE_CMD_GETVI2VO,
	MESSAGE_CMD_SETVI2VO,
	MESSAGE_CMD_GETMOTIONFTPUPLOAD,
	MESSAGE_CMD_SETMOTIONFTPUPLOAD,
	MESSAGE_CMD_GETALARMFTPUPLOAD,
	MESSAGE_CMD_SETALARMFTPUPLOAD,
    MESSAGE_CMD_GETTIMERCAP,
    MESSAGE_CMD_SETTIMERCAP,
    MESSAGE_CMD_GETMOTIONCONTACTEX,
    MESSAGE_CMD_SETMOTIONCONTACTEX,
    MESSAGE_CMD_GETTEMHUMYWCNCEN,
    MESSAGE_CMD_SETTEMHUMYWCNCEN,
	MESSAGE_CMD_GETALARMPARAMYWCNCEN,
	MESSAGE_CMD_SETALARMPARAMYWCNCEN,
    MESSAGE_CMD_RESETVIPARAM,
	MESSAGE_CMD_GETDANTEMHUMYWCNCEN,
	MESSAGE_CMD_SETDANTEMHUMYWCNCEN,
    MESSAGE_CMD_GETVASINGLELINERULES,
    MESSAGE_CMD_SETVASINGLELINERULES,
    MESSAGE_CMD_GETVADOUBLELINERULES,
    MESSAGE_CMD_SETVADOUBLELINERULES,
    MESSAGE_CMD_GETVAMULTILINERULES,
    MESSAGE_CMD_SETVAMULTILINERULES,
    MESSAGE_CMD_GETVAREGIONRULES,
    MESSAGE_CMD_SETVAREGIONRULES,
    MESSAGE_CMD_GETVABACKGROUND,
    MESSAGE_CMD_SETVABACKGROUND,
    MESSAGE_CMD_GETVAOBJECTAREA,
    MESSAGE_CMD_SETVAOBJECTAREA,
    MESSAGE_CMD_GETVAALARMSETTING,
    MESSAGE_CMD_SETVAALARMSETTING,
    MESSAGE_CMD_GETVAPARAM,
    MESSAGE_CMD_SETVAPARAM,
    MESSAGE_CMD_GETVACONTACTEX,
    MESSAGE_CMD_SETVACONTACTEX,
    MESSAGE_CMD_GETSYSSUPPORTEX,
    MESSAGE_CMD_GETDVRMOTIONALARM,
    MESSAGE_CMD_SETDVRMOTIONALARM,
    MESSAGE_CMD_GETDVRSENSORALARM,
    MESSAGE_CMD_SETDVRSENSORALARM,
    MESSAGE_CMD_GETDVRHIDEALARM,
    MESSAGE_CMD_SETDVRHIDEALARM,
    MESSAGE_CMD_GETDVRVILOSTALARM,
    MESSAGE_CMD_SETDVRVILOSTALARM,
    MESSAGE_CMD_GETDVROTHERALARM,
    MESSAGE_CMD_SETDVROTHERALARM,
    MESSAGE_CMD_SETBKDISK,
    MESSAGE_CMD_GETSMARTINFO,
    MESSAGE_CMD_GETMNTNFS,
    MESSAGE_CMD_SETMNTNFS,
    MESSAGE_CMD_GETVIEWPARAM,
    MESSAGE_CMD_SETVIEWPARAM,
    MESSAGE_CMD_GETLOOPVIEW,
    MESSAGE_CMD_SETLOOPVIEW,
    MESSAGE_CMD_GETDVRUSER,
    MESSAGE_CMD_SETDVRUSER,
    MESSAGE_CMD_GETDVRCHANNELOSD,
    MESSAGE_CMD_SETDVRCHANNELOSD,
	MESSAGE_CMD_GETVIDEOOFFSETEX,
	MESSAGE_CMD_SETVIDEOOFFSETEX,
	MESSAGE_CMD_GETDEVICEID,
	MESSAGE_CMD_SETDEVICEID,
	MESSAGE_CMD_GETKEYBOARDEX,
	MESSAGE_CMD_SETKEYBOARDEX,
	MESSAGE_CMD_GETCHOOSEPTZ,
	MESSAGE_CMD_SETCHOOSEPTZ,
	MESSAGE_CMD_GETDVRDOUBLEBITS,
	MESSAGE_CMD_SETDVRDOUBLEBITS,
	MESSAGE_CMD_GETSWITCHSCREENNAME,
	MESSAGE_CMD_GET3GPARAM,
	MESSAGE_CMD_SET3GPARAM,
	MESSAGE_CMD_GETPROTOCOLPARAM,
	MESSAGE_CMD_SETPROTOCOLPARAM,
	MESSAGE_CMD_GETENCODETYPE,
	MESSAGE_CMD_SETENCODETYPE,
	MESSAGE_CMD_GETFTPTIMEREC,
	MESSAGE_CMD_SETFTPTIMEREC,
	MESSAGE_CMD_GETSUBENCODETYPE,
	MESSAGE_CMD_SETSUBENCODETYPE,
    MESSAGE_CMD_CHOOSEVIDEOINPUT,
    MESSAGE_CMD_GETPROLOOP,
    MESSAGE_CMD_SETPROLOOP,
	MESSAGE_CMD_GETANALYSEOSD,
	MESSAGE_CMD_SETANALYSEOSD,
	MESSAGE_CMD_SETAUDIOBESTIR,
	MESSAGE_CMD_SETVIDEOENCMAP,
	MESSAGE_CMD_GETACTORS,
	MESSAGE_CMD_GETATTESTEXHIBIT,
	MESSAGE_CMD_SETATTESTEXHIBIT,
	MESSAGE_CMD_GETALARMLINERULES,
	MESSAGE_CMD_SETALARMLINERULES,
	MESSAGE_CMD_GETREGIONINRULE,
	MESSAGE_CMD_SETREGIONINRULE,
	MESSAGE_CMD_GETREGIONOUTRULE,
	MESSAGE_CMD_SETREGIONOUTRULE,
	MESSAGE_CMD_GETREGIONHOVERRULE,
	MESSAGE_CMD_SETREGIONHOVERRULE,
	MESSAGE_CMD_GETJNKYPARAM,
	MESSAGE_CMD_SETJNKYPARAM,
	MESSAGE_CMD_GETJNKYSERINFO,
	MESSAGE_CMD_SETJNKYSERINFO,
	MESSAGE_CMD_GETCRUISEPLAN,
	MESSAGE_CMD_SETCRUISEPLAN,
	MESSAGE_CMD_GETHNDXOSD,
	MESSAGE_CMD_SETHNDXOSD,
	MESSAGE_CMD_GETAUDIOAAC,
	MESSAGE_CMD_SETAUDIOAAC,
	MESSAGE_CMD_GETHQCOLOR,
	MESSAGE_CMD_SETHQCOLOR,
	MESSAGE_CMD_GETYWDCINFO,
	MESSAGE_CMD_SETYWDCINFO,
	MESSAGE_CMD_SETVASENSE,
	MESSAGE_CMD_GETVASENSE,
	MESSAGE_CMD_SETVATRACKTYPE,
	MESSAGE_CMD_GETVATRACKTYPE,
	MESSAGE_CMD_SETCDRHSTATIONINFO,
	MESSAGE_CMD_GETCDRHSTATIONINFO,
	MESSAGE_CMD_GET3GSTATE,
    MESSAGE_CMD_SETYWCODDEVPARAM,
    MESSAGE_CMD_GETYWCODDEVPARAM,
    MESSAGE_CMD_SETBLACKREGIONTARGET,
    MESSAGE_CMD_GETBLACKREGIONTARGET,
    MESSAGE_CMD_SETBLACKREGIONREFER,
    MESSAGE_CMD_GETBLACKREGIONREFER,
    MESSAGE_CMD_SETBLACKALARMRANGE,
    MESSAGE_CMD_GETBLACKALARMRANGE,
    MESSAGE_CMD_GETAUDIOSILENT,
	MESSAGE_CMD_SETAUDIOSILENT,
    MESSAGE_CMD_SETBUFFLOCK,
    MESSAGE_CMD_GETBUFFLOCK,
	MESSAGE_CMD_SETDVRUSERSTATE,
	MESSAGE_CMD_GETDVRUSERSTATE,
	MESSAGE_CMD_SETREDUNDANCERECORD,
	MESSAGE_CMD_GETREDUNDANCERECORD,
	MESSAGE_CMD_SETTHIRDENCODEPARA,
	MESSAGE_CMD_GETTHIRDENCODEPARA,
	MESSAGE_CMD_SETTHIRDENCODETYPE,
	MESSAGE_CMD_GETTHIRDENCODETYPE,
	MESSAGE_CMD_MAILTEST,
    MESSAGE_CMD_SETPTZCTRLTIME,
    MESSAGE_CMD_GETPTZCTRLTIME,
    MESSAGE_CMD_SETSYSUSERBJHT,
    MESSAGE_CMD_GETSYSUSERBJHT,
	MESSAGE_CMD_GETAUDIOBESTIR,
	MESSAGE_CMD_SETDXJBVGA,
	MESSAGE_CMD_GETDXJBCASEINFO,
	MESSAGE_CMD_SETAUDIORECORD,
	MESSAGE_CMD_GETAUDIORECORD,
	MESSAGE_CMD_SETAUDIOPARAMTYPE,
	MESSAGE_CMD_GETAUDIOPARAMTYPE,
	MESSAGE_CMD_GETBH3G,
	MESSAGE_CMD_GETHZFLGPS,
    MESSAGE_CMD_GETTIMEROSD,
	MESSAGE_CMD_SETTIMEROSD,
	MESSAGE_CMD_GETGZJIEWEIUID,
	MESSAGE_CMD_SETGZJIEWEIUID,
	MESSAGE_CMD_GETDVRSYSUSEREX,
	MESSAGE_CMD_SETDVRSYSUSEREX,
	MESSAGE_CMD_GETBJHTPTZWATCHER,
	MESSAGE_CMD_SETBJHTPTZWATCHER,
	MESSAGE_CMD_GETSLOTSTATE,
	MESSAGE_CMD_SETSLOTSTATE,
	MESSAGE_CMD_GETNASPARAM,
	MESSAGE_CMD_SETNASPARAM,
	MESSAGE_CMD_GETCARDPARAM,
	MESSAGE_CMD_SETCARDPARAM,
};

enum
{
    EXTENDALARM_TYPE_TEMPERATURE = 0,       //temperature alarm
    EXTENDALARM_TYPE_HUMIDITY,              //humidity alarm
	EXTENDALARM_TYPE_VOLTAGE				//voltage alarm
};

enum
{
	SOCK_CMD_SERVERMSG = 1,                //server message sock
};

enum
{
	FILE_EXPORT_FINISH = 1,
	FILE_EXPORT_FAILD,
	FILE_EXPORT_UPDATE,
	FILE_EXPORT_END,
	FILE_LEADIN_FINISH,
	FILE_LEADIN_FAILD,
	FILE_LEADIN_UPDATE,
	FILE_LEADIN_END,
};

//SDK initialize
CNET_APIIMPORT void  __stdcall VSNET_ClientSetDebugFile(char *filename);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientStartup(UINT m_nMessage,HWND m_hWnd,void (WINAPI *m_messagecallback)(LONG hHandle,int wParam,INT_PTR lParam,void *context) = NULL,void *context = NULL, char *key = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientReadMessage(char *m_sername,char *m_url,WORD *m_port,int *m_ch,DWORD *wParam,DWORD *lParam);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientWaitTime(int m_waitnum = 6,int m_trynum = 3);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientCleanup();

//connect server,record
CNET_APIIMPORT LONG  __stdcall VSNET_ClientStart(char *m_url,CHANNEL_CLIENTINFO *m_pChaninfo,WORD wserport = 3000,int streamtype = 0);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientStop(LONG hHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientStartView(LONG hHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientStopView(LONG hHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetWnd(LONG hHandle,HWND hWnd);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientRefrenshWnd(LONG hHandle,RECT *rect = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientShowcallback(LONG hHandle,void(WINAPI *ShowCallBack)(BYTE *m_y,BYTE *m_u,BYTE *m_v,int stridey,int strideuv,int width,int height,void *context),void *context);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientAudioDeccallback(LONG hHandle,void(WINAPI *AudioDecCallBack)(char *pBuffer,int size,void *context),void *context);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientDrawCallBack(LONG hHandle,void(WINAPI *DrawCallBack)(HDC hDC,void *context),void *context);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientVideoQuality(LONG hHandle,int m_quality);
CNET_APIIMPORT LONG  __stdcall VSNET_ClientGetState(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStartASFFileCap(LONG hHandle,LPTSTR m_FileName,BOOL m_bbroad = FALSE);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStopCapture(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientPauseCapture(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientCaptureRestart(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientPlayAudio(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStopAudio(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientGetVideoSize(LONG hHandle,DWORD *m_pWidth,DWORD *m_pHeight);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientCapturePicture(LONG hHandle,LPTSTR m_filename);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientPTZCtrl(LONG hHandle,int type,int value,int priority = 0, char *extrabuff = NULL, int extrasize = 0);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetPTZAddr(LONG hHandle,BYTE m_ptzaddr);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientPTZDev(LONG hHandle,char m_devch,BOOL bOn);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientOutPut(LONG hHandle,char m_devch,BOOL bOn);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientViAdjust(LONG hHandle,BYTE m_bri,BYTE m_con,BYTE m_sta,BYTE m_hue);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSwitchChannel(LONG hHandle,char m_curch);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientPrerecord(LONG hHandle,BOOL m_benable,int m_buffsize,int m_framecount);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSerialSend(LONG hHandle,int m_serialport,BYTE *pData,int size);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientClearAlarmOutHandle(LONG hHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientStartViewEx(LONG hHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientStopViewEx(LONG hHandle);
CNET_APIIMPORT int   __stdcall VSNET_ClientGetSwitchChannels(LONG hHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetStreamInfo(LONG hHandle,VSTREAMINFO *m_pStreamInfo);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStartNomalCap(LONG hHandle,
					   void(WINAPI *m_nomalvideo)(char *pbuff,int headsize,int datasize,int timetick,int biskeyframe,void *context),void *pvideocontext,
					   void(WINAPI *m_nomalaudio)(char *pbuff,int headsize,int datasize,int timetick,int biskeyframe,void *context),void *paudiocontext);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStopNomalCap(LONG hHandle);
CNET_APIIMPORT LONG  __stdcall VSNET_InfoClientStart(char *m_infourl,CHANNEL_CLIENTINFO *m_pChaninfo,WORD winfoport = 8080,int streamtype = 0);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientInfoGetIPHandle(LONG hHandle,char* m_pgetip,WORD *m_wgetport);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStartMp4Capture(LONG hHandle, LPTSTR m_FileName);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStopMp4Capture(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientPauseMp4Capture(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientMp4CaptureRestart(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_CleanVideoDisplayBuffer(LONG hHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientMediaData(LONG hHandle,BOOL bStart);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientAudioVolume(LONG hHandle,WORD m_Volume);
CNET_APIIMPORT int   __stdcall VSNET_ClientGetFrameRate(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientCapturePictureEx(LONG hHandle,char *pbuff,int stride,int size);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetPriority(LONG hHandle,int priority);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSendBankInfo(LONG hHandle,NETRVS_BANKINFO *pbankinfo);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientFireExtendAlarm(LONG hHandle,int alarmType,float alarmValue);

CNET_APIIMPORT LONG  __stdcall VSNET_ServerStart(char *m_url,SERVER_UPDATEINFO *m_pSerinfo,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ServerStop(LONG hHandle);

CNET_APIIMPORT LONG  __stdcall VSNET_ServerParamExport(char *m_url,char *m_username,char *m_password,char *m_filename,WORD wserport,char *m_sername,void* context,
									  void(WINAPI *ExportCallBack)(int msgtype,int curLeadinsize,int curExportsize,int filetotalsize,int errorcode,void *context));
CNET_APIIMPORT BOOL  __stdcall VSNET_ServerParamExportStop(LONG hHandle);
CNET_APIIMPORT LONG  __stdcall VSNET_ServerParamLeadin(char *m_url,char *m_username,char *m_password,char *m_filename,WORD wserport,char *m_sername,void* context,
									  void(WINAPI *LeadinCallBack)(int msgtype,int curLeadinsize,int curExportsize,int filetotalsize,int errorcode,void *context));
CNET_APIIMPORT BOOL  __stdcall VSNET_ServerParamLeadinStop(LONG hHandle);


//talk and brodcast(auto collection)
CNET_APIIMPORT LONG  __stdcall VSNET_ClientTalkExchangeStart(char *m_url,char *m_username,char *m_password,WORD wserport = 3000,void(WINAPI *TalkCallBack)(char *pbuff,int size,void *context) = NULL,char *m_servername = NULL, int samplerate = 8000,void *context = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTalkExchangeStop(LONG hAudioHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTalkBroadcastStart();
CNET_APIIMPORT int   __stdcall VSNET_ClientTalkAddnew(char *m_url,char *m_username,char *m_password,WORD wserport = 3000,char *m_servername = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTalkDel(char *m_url,WORD wserport = 3000,char *m_servername = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTalkBroadcastStop();
//talk(user collection)
CNET_APIIMPORT int   __stdcall VSNET_ClientTalkStart(char *m_url,char *m_username,char *m_password,int m_tranType,LONG *handle,WORD wserport=3000,char *m_servername = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTalkStop(LONG hAudioHandle);
CNET_APIIMPORT int	 __stdcall VSNET_ClientTalkSendData(LONG hAudioHandle,char *pBuf,int buflen);
CNET_APIIMPORT void  __stdcall VSNET_ClientTalkFreeAll();

//update server
CNET_APIIMPORT LONG  __stdcall VSNET_ClientUpdateStart(char *m_url,char *m_username,char *m_password,char *m_filename,HANDLE m_hEndEvent,WORD wserport = 3000,char *m_sername = NULL);
CNET_APIIMPORT int   __stdcall VSNET_ClientUpdateStop(LONG hHandle);
CNET_APIIMPORT void	 __stdcall VSNET_StartUploadRom(char *m_sername,char *m_url,char *m_username,char *m_password,WORD m_wserport, char* m_szRomPath);
CNET_APIIMPORT int	 __stdcall VSNET_GetUploadRomRst();

//param function
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientClearAlarmOut(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetChannelAmount(char *m_sername,char *m_url,DWORD *m_channs,DWORD *m_alarmins,DWORD *m_alarmouts,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetConnectIP(char *m_sername,char *m_url,char *m_username,char *m_password,CONNECTIP *m_pConnecIP,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientResetServer(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientShutDownServer(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetWholePara(char *m_sername,char *m_url,char *m_username,char *m_password,WHOLEPARAM *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetWholePara(char *m_sername,char *m_url,char *m_username,char *m_password,WHOLEPARAM *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetChannelPara(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNELPARAM *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetChannelPara(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNELPARAM *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetAlarmRecord(char *m_sername,char *m_url,char *m_username,char *m_password,ALARMPARAM *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetAlarmRecord(char *m_sername,char *m_url,char *m_username,char *m_password,ALARMPARAM *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetMotionPara(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNMOTION *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetMotionPara(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNMOTION *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientMotionTest(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,BOOL m_bOn,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetVideoMask(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNVIDEOMASK *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetVideoMask(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNVIDEOMASK *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetChanOSD(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNOSD *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetChanOSD(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNOSD *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetChanOSDWithBK(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNOSD *m_pGetPara,WORD wserport = 3000, char *m_szOSDTransparent = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetChanOSDWithBK(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNOSD *m_pSetPara,WORD wserport = 3000, char *m_szOSDTransparent = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSaveServerPara(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientDefaultPara(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetViparam(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNVIPARAM *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetViparam(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNVIPARAM *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetSerial485Param(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,SERIAL485_PARAM *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetSerial485Param(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,SERIAL485_PARAM *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetSystime(char *m_sername,char *m_url,char *m_username,char *m_password,TIME_PARAM *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetSystime(char *m_sername,char *m_url,char *m_username,char *m_password,TIME_PARAM *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetSerialNo(char *m_sername,char *m_url,char *m_username,char *m_password,BYTE m_serial[50],WORD wserport = 3000);
CNET_APIIMPORT int   __stdcall VSNET_ClientAffirmUser(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetUser(char *m_sername,char *m_url,char *m_username,char *m_password,WHOLE_USER *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetUser(char *m_sername,char *m_url,char *m_username,char *m_password,WHOLE_USER *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientOutPutServer(char *m_sername,char *m_url,char *m_username,char *m_password,char m_devch,BOOL bOn,WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSerialSendToServer(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,int m_serialport,BYTE *pData,int size,WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientGetOutputStatus(char *m_sername,char *m_url,char *m_username,char *m_password,BYTE m_outputstatus[8],WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientGetExDeviceStatus(char *m_sername,char *m_url,char *m_username,char *m_password,BYTE m_outputstatus[16],WORD wserport = 3000);
CNET_APIIMPORT int 	 __stdcall VSNET_ClientSetPTZCommandFile(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,char *m_filename,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetPTZCommandName(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,char *m_ptzname,WORD wserport = 3000);
CNET_APIIMPORT int 	 __stdcall VSNET_ClientGetCurSwitchChannel(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetVideoLost(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,BOOL bEnabled,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetVideoLost(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,BOOL *pbEnabled,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetTimerReset(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,VSTIMERRESET *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetTimerReset(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,VSTIMERRESET *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetHideAlarm(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNELHIDEALARM *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetHideAlarm(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,CHANNELHIDEALARM *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientEncKeyFrame(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport=3000,int streamtype=0);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetUdpPort(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,unsigned short *videoport,unsigned short *audioport,WORD wserport=3000);
CNET_APIIMPORT int   __stdcall VSNET_ClientServerTalk(ULONG m_nurl,char *m_username,char *m_password,int m_tranType,BOOL isStart,UINT_PFW *sock,DWORD wserport = 3000,char *m_servername = NULL);
CNET_APIIMPORT int 	 __stdcall VSNET_ClientSetLicenseFile(char *m_sername,char *m_url,char *m_username,char *m_password,char *pbuff,int size,WORD wserport = 3000);
CNET_APIIMPORT int 	 __stdcall VSNET_ClientGetLicenseFile(char *m_sername,char *m_url,char *m_username,char *m_password,void(WINAPI *licdatacallback)(char *pbuff,int size,void *userdata),void *userdata = NULL,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetHostNotify(char *m_sername,char *m_url,char *m_username,char *m_password,VSNETHOSTNOTIFY *m_pSetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetHostNotify(char *m_sername,char *m_url,char *m_username,char *m_password,VSNETHOSTNOTIFY *m_pGetPara,WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetSerialNumData(char *m_sername,char *m_url,char *m_username,char *m_password,char *pSetbuff,WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientGetSerialNumData(char *m_sername,char *m_url,char *m_username,char *m_password,char *pGetbuff,WORD wserport = 3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetSerialNumPass(char *m_sername,char *m_url,char *m_username,char *m_password,char *pSetbuff,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetTempData(char *m_sername,char *m_url,char *m_username,char *m_password,char *pbuff,int size,WORD wserport = 3000);
CNET_APIIMPORT int   __stdcall VSNET_ClientGetTempData(char *m_sername,char *m_url,char *m_username,char *m_password,char *pbuff,WORD wserport = 3000);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetPTZText(char *m_sername,char *m_url,char *m_username,char *m_password,char *pbuff,int size,WORD wserport = 3000);
CNET_APIIMPORT int	 __stdcall VSNET_ClientPTZLock(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,BOOL block,int priority,WORD wserport=3000);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetPTZLockTime(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,int time,WORD wserport=3000);
CNET_APIIMPORT int	 __stdcall VSNET_ClientCheckPTZPriority(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,int priority,WORD wserport=3000);

//remote host
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetRemoteHost(char *m_sername,char *m_url,char *m_username,char *m_password,VSREMOTEHOST *m_pGetPara,WORD wserport );
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetRemoteHost(char *m_sername,char *m_url,char *m_username,char *m_password,VSREMOTEHOST *m_pSetPara,WORD wserport );

//sub channel video param
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetSubChanPara(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,VSSUBCHANPARAM *m_pGetPara,WORD wserport );
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSetSubChanPara(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,VSSUBCHANPARAM *m_pSetPara,WORD wserport );

//server record and capture
CNET_APIIMPORT BOOL	 __stdcall VSNET_StartServerRecord(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,int lRecordType,WORD wserport );
CNET_APIIMPORT BOOL	 __stdcall VSNET_StartServerRecord_RecName(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,int lRecordType,WORD wserport, VSNETSERVERRECPARAM *m_pServerRecParam );
CNET_APIIMPORT BOOL	 __stdcall VSNET_StartServerRecord_DXJB(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport, VSNETDXJBCASEINFO *m_pCaseInfo);
CNET_APIIMPORT BOOL	 __stdcall VSNET_StopServerRecord(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport,VSNETPLAY_FILE_DATA *filedata = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetRecordStatus(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ServerCapJPEG(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,int m_quality,WORD wserport );

//server disk
CNET_APIIMPORT BOOL	 __stdcall VSNET_GetDiskState(char *m_sername,char *m_url,char *m_username,char *m_password,VSDISKSTATE *m_pDiskState,WORD wserport );
CNET_APIIMPORT int 	 __stdcall VSNET_FormatDisk(char *m_sername,char *m_url,char *m_username,char *m_password,int m_diskno,WORD wserport );
//server param common
CNET_APIIMPORT LONG	 __stdcall VSNET_ClientMessageOpen(char *sername,char *url,char *username,char *password,WORD serport = 3000);
CNET_APIIMPORT int	 __stdcall VSNET_ClientMessageOpt(LONG hHandle,int opt,int ch = 0,void *param1 = NULL,void *param2 = NULL,void *param3 = NULL);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientMessageClose(LONG hHandle);
CNET_APIIMPORT int   __stdcall VSNET_UniversalParamsInterface(char *url, char *sername, DWORD serport, char *username, char *password, int ch, int cmd, int paramGetSet, int datasize, void *param1, void *param2, void *param3,  LONG& handle, int trynum = 3, int waittime = 10);
//redirector
CNET_APIIMPORT BOOL	 __stdcall VSNET_SetRedirectorInfo(VSNET_REDIRECTORINFO *m_pRedirect);
CNET_APIIMPORT int	 __stdcall VSNET_StartListenClient(char *m_localAddrIP = NULL);
CNET_APIIMPORT BOOL	 __stdcall VSNET_StopListenClient();
CNET_APIIMPORT int   __stdcall VSNET_RedirectorGetNormalUser(VSUSERINFO *userinfo,int index);
CNET_APIIMPORT int   __stdcall VSNET_RedirectorGetMultiUser(VSUSERINFO *userinfo,int index);
CNET_APIIMPORT BOOL  __stdcall VSNET_RedirectorDelUser(VSUSERINFO *userinfo);

//get server ipaddress from ddns
CNET_APIIMPORT int 	 __stdcall VSNET_ClientInfoGetServerIP(char* m_infourl,WORD m_wInfoport,char *m_sername,char* m_pgetip,WORD *m_wgetport);

//transparent serial
CNET_APIIMPORT LONG	 __stdcall VSNET_ClientSerialStart(char *m_sername,char *m_url,char *m_username,char *m_password,int iSerialPort,
												 VSSERIAL_INFO *pSerialInfo,
												 void(WINAPI *fSerialDataCallBack)(LONG hSerial,char *pRecvDataBuff,int BuffSize,void *context),
												 void *context,WORD wserport);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSerialStop(LONG hSerial);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSerialSendNew(LONG hSerial,char *pSendBuff,DWORD BuffSize);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSerialRecvPause(LONG hSerial);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSerialRecvRestart(LONG hSerial);
//JPEG capture
CNET_APIIMPORT LONG	 __stdcall VSNET_ClientJpegCapStart(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport,
											void(WINAPI *jpegdatacallback)(LONG hHandle,int m_ch,char *pBuffer,int size,void *userdata),void *userdata);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientJpegCapSingle(LONG hHandle,int m_ch,int m_quality);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientJpegCapStop(LONG hHandle);
//new
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetDelayTime(LONG hHandle,int delaytime);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientFlipVideo(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientFlipVideoH(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientGetVideoMove(LONG hHandle,VSNETVIDEOMOVE_BLOCK *block);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientSharpeVideo(LONG hHandle, int SharpeStrength);

CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStartMp4StreamCapture(LONG hHandle,WORD port);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStopMp4StreamCapture(LONG hHandle);

CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetRedirectDelayTime(LONG hHandle,int delaytime);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientSetImageShow(LONG hHandle,BOOL bShow);

//client record
CNET_APIIMPORT BOOL	 __stdcall VSNET_StartClientRecord(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport,char *pfilename);
CNET_APIIMPORT BOOL	 __stdcall VSNET_StopClientRecord(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport);
CNET_APIIMPORT BOOL  __stdcall VSNET_GetClientRecordStatus(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport);
//
CNET_APIIMPORT LONG	 __stdcall VSNET_ClientStartImageShow(HWND hWnd,int width,int height);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientStopImageShow(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientRefreshImageShow(LONG hHandle);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientShowImageShow(LONG hHandle,BYTE *m_y,BYTE *m_u,BYTE *m_v,int ystride,int uvstride);
CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientShowSetWnd(LONG hHandle,HWND hWnd);
//listen server status
CNET_APIIMPORT LONG  __stdcall VSNET_DevStatusListenStart(DevStatusCallback callback,void *context,WORD wListenPort = 9000,int timeout = 30);
CNET_APIIMPORT BOOL  __stdcall VSNET_DevStatusListenStop(LONG hHandle);
//prevent image split
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientPreventImageSplit(LONG hHandle, BOOL bsplit);
//search wifi
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientWifiSearch(char *m_sername,char *m_url,char *m_username,char *m_password,VSNETWIFISSIDLIST *m_pWifiList,WORD wserport);
//talk bothway
CNET_APIIMPORT LONG  __stdcall VSNET_ClientTalkBothwayStart(char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport,void(WINAPI *TalkCallBack)(char *pbuff,int size,void *context),char *m_servername,HWND hwnd, int samplerate = 8000,void *context = NULL);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTalkBothwayStop(LONG hAudioHandle);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTalkDeccallback(void(WINAPI *AudioDecCallBack)(char *pBuffer,int size,void *context),void *context);
//
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientTranCtrl(LONG hHandle,int ctrlCode);
//Station
CNET_APIIMPORT BOOL  __stdcall VSNET_RVSRegSerCheckCallback(pvsnetchancallback pchancheck);
CNET_APIIMPORT BOOL  __stdcall VSNET_RVSRegMsgCallback(LONG hHandle,pvsnetalarmcallback palarmcallback,void *context);
CNET_APIIMPORT BOOL  __stdcall VSNET_RVSStartServer(VSNETRVSINFO *m_pRvsInfo);
CNET_APIIMPORT BOOL  __stdcall VSNET_RVSStopServer();
CNET_APIIMPORT BOOL  __stdcall VSNET_RVSSetChanServerID(LONG hHandle, char *pSerID);
//intelligent alarm
CNET_APIIMPORT LONG  __stdcall VSNET_ClientPicAlarmStart(char *m_sername,char *m_url,char *m_username,char *m_password,WORD wserport,
                                                         void (WINAPI *m_alarmpiccallback)(LONG hHandle, int ch, char *info, int size, char *buff, void *userdata), 
                                                         void *userdata);
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientPicAlarmStop(LONG hHandle);

CNET_APIIMPORT BOOL  __stdcall VSNET_ChannelChange(char *m_sername,char *m_url,char m_ch,char *m_username,char *m_password,WORD wserport);

//get web rom version
CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetWebRomVer(char *m_sername,char *m_url,char *m_username,char *m_password,VERSIONWEBROM *m_pGetPara,WORD wserport = 3000);

//sewage
CNET_APIIMPORT BOOL  __stdcall VSNET_FlowMeterComm(char *m_sername, char *m_url, char *m_username,char *m_password,WORD wserport, char comm, int address, VSNETYWFLOWMETERCODE *filedata);
CNET_APIIMPORT BOOL  __stdcall VSNET_SZCYQComm(char *m_sername, char *m_url, char *m_username,char *m_password,WORD wserport, char comm, VSNETYWSZCYQCODE *filedata);
CNET_APIIMPORT BOOL  __stdcall VSNET_SwitchComm(char *m_sername, char *m_url, char *m_username,char *m_password,WORD wserport, char comm);

CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetMsgBuff(LONG hHandle, int type, char *msgbuff);

CNET_APIIMPORT BOOL  __stdcall VSNET_ClientGetVideoAudioInfo(LONG hHandle, CHANNEL_VIDEOINFO *pVideoInfo, CHANNEL_AUDIOINFO *pAudioInfo);
CNET_APIIMPORT int	 __stdcall VSNET_ClientGetVolumeLevel(LONG hHandle);

CNET_APIIMPORT BOOL	 __stdcall VSNET_ClientRecordIFrameCallback(LONG hHandle,pRecordIFrameCallback piframecallback, void *context);
#endif//__LAUNET_H__
