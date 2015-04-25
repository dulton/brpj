
#ifndef _VLC_RTSP_SDK_
#define _VLC_RTSP_SDK_

#if OPEN_VLC_RTSP_SDK
#include "DCCache.h"

#include "libvlcinclude/vlc/vlc.h"
#include "libvlcinclude/vlc/libvlc.h"
#pragma comment(lib, "libvlclib/libvlc.lib")
#pragma comment(lib, "libvlclib/libvlccore.lib")

class CVlcRtspSDK
{
public:
	CVlcRtspSDK();
	virtual ~CVlcRtspSDK();

	int screenNo;

	unsigned char *RGBdata;
	CString CapturePath;

	libvlc_instance_t *m_pLibvlc_Inst;
	libvlc_media_player_t *m_pLibvlc_Mp;
	libvlc_media_t *m_pLibvlc_m;

	unsigned int Width;
	unsigned int Height;

	HWND hWnd;
	CDC *pDc ;
	HDC  hDC;

	CDCCache m_DC_Cache;
	int m_nBitCount; // ÑÕÉ«Î»Êý

	CRITICAL_SECTION lockflag;
	void Lock();
	void UnLock();

	bool StartPlay(int screenNum,char *name,HWND inhWnd,char *Rtspurl);
	void StopPlay();
	void Capture(char *filename);



};

#endif

#endif