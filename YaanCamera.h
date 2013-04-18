// VideoChannel.h: interface for the CVideoChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOCHANNEL_H__D1736789_C3E0_4E20_88A2_304B89946F22__INCLUDED_)
#define AFX_VIDEOCHANNEL_H__D1736789_C3E0_4E20_88A2_304B89946F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYaanCamera  
{
public:
	CYaanCamera();
	virtual ~CYaanCamera();
public:
	int         m_playhandle[MAX_PLAYWIN];  //²¥·Å¾ä±ú
	BOOL        m_bplay[MAX_PLAYWIN];
public:
	void		SDKinit();
	void		StartPlay(int nCuWinID,char *name,char *sip,WORD port,char *user,char *psw,HWND hWnd);
	void        StopPlay(int nCuWinID);

	void        CapturePic(int nCuWinID,char *filename);
	int         StartRecord(int nCuWinID,LPCSTR filename);
	int         StopRecord(int nCuWinID);
};

#endif // !defined(AFX_VIDEOCHANNEL_H__D1736789_C3E0_4E20_88A2_304B89946F22__INCLUDED_)
