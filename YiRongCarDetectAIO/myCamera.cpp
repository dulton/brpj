
#include "stdafx.h"
#include "myCamera.H"

CMyCamera::CMyCamera()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_LoginHandle[i] = 0;
		m_RealHandle[i] = 0;
	}
}

CMyCamera::~CMyCamera()
{
}

void CMyCamera::SDKInit()
{
}

bool CMyCamera::StartPlay(int venderID,int screenNo,char *name,char *sip,int nPort,char *user,char *psw,HWND hWnd,int subtype)
{
	bool ret = false;
	if(venderID == 0)
	{
		ret = m_haikang.StartPlay(screenNo,name, sip, nPort, user, psw, hWnd, subtype);
		if(ret)
		{
			m_LoginHandle[screenNo] = m_haikang.m_LoginHandle[screenNo];
			m_RealHandle[screenNo] = m_haikang.m_RealHandle[screenNo];
		}
	}
	else
	{
		ret = m_dahua.StartPlay(screenNo,name, sip, nPort, user, psw, hWnd, subtype);
		if(ret)
		{
			m_LoginHandle[screenNo] = m_dahua.m_LoginHandle[screenNo];
			m_RealHandle[screenNo] = m_dahua.m_RealHandle[screenNo];
		}
	}
	return ret;
}

void CMyCamera::StopPlay(int venderID,int screenNo)
{
	if(venderID == 0)
	{
		m_haikang.StopPlay(screenNo);
	}
	else
	{
		m_dahua.StopPlay(screenNo);
	}
	m_LoginHandle[screenNo] = 0;
	m_RealHandle[screenNo] = 0;
}

void CMyCamera::Capture(int venderID,int screenNo,char *filename)
{
	if(venderID == 0)
	{
		m_haikang.Capture(screenNo,filename);
	}
	else
	{
		m_dahua.Capture(screenNo,filename);
	}
}


