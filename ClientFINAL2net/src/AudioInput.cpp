// AudioInput.cpp: implementation of the CAudioInput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AudioInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define NUM_REC_NOTIFICATIONS 10
#define SAFE_DELETE(p)    { if(p) { delete(p); (p) = NULL; } }

static void CALLBACK s_waveInProc(HWAVEIN hwi,UINT uMsg,
            DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAudioInput::CAudioInput()
{
	pWaveInBuff = NULL;
	m_NotifySize	= 0;

	m_wavestop   = TRUE;
	m_hWaveIn    = NULL;
	for (int i = 0 ; i < REC_NUM ; i++)
	{
		memset(&m_WaveHeadIn[i],0,sizeof(WAVEHDR));
	}
	m_CaptureCallBack = NULL;
	userdata = NULL;
	m_hWnd   = NULL;
}

CAudioInput::~CAudioInput()
{
	FreeDirectSound();
}
/******************************** Code ***************************/
int CAudioInput::FreeDirectSound()
{
	if(m_hWaveIn)
	{
		m_wavestop = TRUE;
		waveInReset(m_hWaveIn);
		for (int i = 0 ; i < REC_NUM ; i++)
		{
			waveInUnprepareHeader(m_hWaveIn,&m_WaveHeadIn[i],sizeof(WAVEHDR));
		}
		waveInStop(m_hWaveIn);
		waveInClose(m_hWaveIn);
		m_hWaveIn = NULL;
	}
	SAFE_DELETE(pWaveInBuff);
	m_NotifySize = 0;
	return 0;
}
/******************************** Code ***************************/
int CAudioInput::StopCapture()
{
	m_wavestop = TRUE;
	return TRUE;
}
/******************************** Code ***************************/
int CAudioInput::CreateCaptureBuff(WAVEFORMATEX *pwfxInput)
{
	int i = 0;
	WAVEINCAPS  WaveInDevCaps;
	MMRESULT	result;

	m_NotifySize = pwfxInput->nAvgBytesPerSec / NUM_REC_NOTIFICATIONS;
	SAFE_DELETE(pWaveInBuff);             
	pWaveInBuff = new char[m_NotifySize*REC_NUM];
	if(pWaveInBuff == NULL)
	{
		return -1;
	}
	m_wavestop = FALSE;

	result = waveInGetDevCaps(0,&WaveInDevCaps,sizeof(WAVEINCAPS));
	if(result != MMSYSERR_NOERROR) 
	{
		TRACE("waveInGetDevCaps err : %d\n",result);
		goto err_exit;
	}
	result = waveInOpen(&m_hWaveIn,WAVE_MAPPER,(WAVEFORMATEX*)pwfxInput,(DWORD)s_waveInProc,(DWORD)this,CALLBACK_FUNCTION);
	if(result != MMSYSERR_NOERROR)
	{
		TRACE("waveInOpen error : %d\n",result);
		goto err_exit;
	}
	for (i = 0 ; i < REC_NUM ; i++)
	{
		m_WaveHeadIn[i].lpData         = (LPSTR)pWaveInBuff+(i*m_NotifySize);
		m_WaveHeadIn[i].dwBufferLength = m_NotifySize;
		m_WaveHeadIn[i].dwFlags        = 0L;
		m_WaveHeadIn[i].dwLoops        = 0L;
		result = waveInPrepareHeader(m_hWaveIn,&m_WaveHeadIn[i],sizeof(WAVEHDR));
		if(result != MMSYSERR_NOERROR)
		{
			TRACE("waveInPrepareHeader error : %d\n",result);
			goto err_exit;
		}
		waveInAddBuffer(m_hWaveIn,&m_WaveHeadIn[i],sizeof(WAVEHDR));
	}
	waveInStart(m_hWaveIn);
	return 0;

err_exit:
	SAFE_DELETE(pWaveInBuff);             
	return -1;
}
/******************************** Code ***************************/
int CAudioInput::StartCapture()
{
	WAVEFORMATEX wfxInput;

	if(m_hWaveIn != NULL) return -1;

	FreeDirectSound();

	memset(&wfxInput,0,sizeof(WAVEFORMATEX));
	wfxInput.wFormatTag      = WAVE_FORMAT_PCM;
	wfxInput.nSamplesPerSec  = 8000;
	wfxInput.wBitsPerSample  = 16;
	wfxInput.nChannels		 = 1;
	wfxInput.nBlockAlign     = wfxInput.nChannels * ( wfxInput.wBitsPerSample / 8 );
	wfxInput.nAvgBytesPerSec = wfxInput.nBlockAlign * wfxInput.nSamplesPerSec;
	if(CreateCaptureBuff(&wfxInput) != 0) 
	{
		FreeDirectSound();
		return -1;
	}
	return 0;
}
/******************************** Code ***************************/
static void CALLBACK s_waveInProc(HWAVEIN hwi,UINT uMsg,
            DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	CAudioInput *param = (CAudioInput*)dwInstance;
	if(uMsg == WIM_DATA)
	{
		if(param->m_wavestop)
		{
			PostMessage(param->m_hWnd,WM_MSG_AUDIOCAPEND,0,0);
		}
		else
		{
			if(param->m_CaptureCallBack)
			{
				param->m_CaptureCallBack(((PWAVEHDR)dwParam1)->lpData,((PWAVEHDR)dwParam1)->dwBytesRecorded,param->userdata);
			}
			waveInAddBuffer(param->m_hWaveIn,/*&param->m_WaveHeadIn*/(LPWAVEHDR)dwParam1,sizeof(WAVEHDR));
		}
	}
}
/******************************** Code ***************************/
