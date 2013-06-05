// CYaAnSDK.h: interface for the CVideoChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOCHANNEL_H__D1736789_C3E0_4E20_88A2_304B89946F22__INCLUDED_)
#define AFX_VIDEOCHANNEL_H__D1736789_C3E0_4E20_88A2_304B89946F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	char m_filemask[16];
	int  m_fileversion;
	int  videofoucc;
	int  videoid;
	short m_width;
	short m_height;
	short m_audiotag;
	short m_haveaudio;
	short m_auchs;
	short m_aurates;
	int   m_ausample;
	unsigned int m_totalframes;
	unsigned int m_firsttick;
	unsigned int m_losttick;
	int maskend;
}ETI_FILE_HEAD;

#define ETI_FILEMASK		"LAUNCHDIGITAL"

class CYaAnSDK  
{
public:
	CYaAnSDK();
	virtual ~CYaAnSDK();
public:
	int         m_RealHandle[MAX_DEVICE_NUM];	//播放句柄
	LONG        m_LoginHandle[MAX_DEVICE_NUM];	//connect handle

	int m_ptzRealHandle;
	LONG m_ptzLoginHandle;

	ETI_FILE_HEAD m_filehead;					//流播放文件头
	unsigned char *image[MAX_DEVICE_NUM];	//ZOGNA YUV420 BUFFER

public:
	void		SDKInit();
	bool        StartPlay(int screenNo,char *name,char *sip,WORD port,char *user,char *psw,HWND hWnd,int subtype);
	void        StopPlay(int screenNo);

	int         Capture(int screenNo,char *filename);
	int         StartRecord(int screenNo,LPCSTR filename);
	int         StopRecord(int screenNo);
	void		LinkProcess(int screenNo,int lParam);
	int			GetWndIndex(int LoginHandle);
	void		PtzControl(long lLoginID, int type, BOOL dwStop, int param);
	bool		PtzStartPlay(char *sip,int nPort,char *user,char *psw,HWND hWnd);
	void		PtzStopPlay();
};

#endif // !defined(AFX_VIDEOCHANNEL_H__D1736789_C3E0_4E20_88A2_304B89946F22__INCLUDED_)
