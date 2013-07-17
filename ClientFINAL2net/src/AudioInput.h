// AudioInput.h: interface for the CAudioInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOINPUT_H__2A628C2F_4C7E_43A6_918B_63554E0875E1__INCLUDED_)
#define AFX_AUDIOINPUT_H__2A628C2F_4C7E_43A6_918B_63554E0875E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define REC_NUM	4
class CAudioInput  
{
public:
	CAudioInput();
	virtual ~CAudioInput();
public:
	void (WINAPI *m_CaptureCallBack)(char *pbuff,int size,void *userdata);
	void    *userdata;
	int      m_wavestop;
	WAVEHDR  m_WaveHeadIn[REC_NUM];
    HWAVEIN  m_hWaveIn;
	char    *pWaveInBuff;
	int      m_NotifySize;
	HWND     m_hWnd;
public:
	int      StartCapture();
	int      StopCapture();
	int      FreeDirectSound();
protected:
	int      CreateCaptureBuff(WAVEFORMATEX *pwfxInput);
};

#endif // !defined(AFX_AUDIOINPUT_H__2A628C2F_4C7E_43A6_918B_63554E0875E1__INCLUDED_)
