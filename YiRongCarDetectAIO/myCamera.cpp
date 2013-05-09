
#include "stdafx.h"
#include "myCamera.h"

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
	switch(venderID)
	{
		case VENDER_TYPE_HAIKANG:
			ret = m_haikang.StartPlay(screenNo,name, sip, nPort, user, psw, hWnd, subtype);
			if(ret)
			{
				m_LoginHandle[screenNo] = m_haikang.m_LoginHandle[screenNo];
				m_RealHandle[screenNo] = m_haikang.m_RealHandle[screenNo];
			}
			break;
		case VENDER_TYPE_DAHUA:
			ret = m_dahua.StartPlay(screenNo,name, sip, nPort, user, psw, hWnd, subtype);
			if(ret)
			{
				m_LoginHandle[screenNo] = m_dahua.m_LoginHandle[screenNo];
				m_RealHandle[screenNo] = m_dahua.m_RealHandle[screenNo];
			}
			break;
		case VENDER_TYPE_YAAN:
			ret = m_yaAn.StartPlay(screenNo,name, sip, nPort, user, psw, hWnd, subtype);
			if(ret)
			{
				m_LoginHandle[screenNo] = m_yaAn.m_LoginHandle[screenNo];
				m_RealHandle[screenNo] = m_yaAn.m_RealHandle[screenNo];
			}
			break;
		default:break;
	}
	return ret;
}

void CMyCamera::StopPlay(int venderID,int screenNo)
{
	switch(venderID)
	{
		case VENDER_TYPE_HAIKANG:
			m_haikang.StopPlay(screenNo);
			break;
		case VENDER_TYPE_DAHUA:
			m_dahua.StopPlay(screenNo);
			break;
		case VENDER_TYPE_YAAN:
			m_yaAn.StopPlay(screenNo);
			break;
		default:
			break;
	}
	m_LoginHandle[screenNo] = 0;
	m_RealHandle[screenNo] = 0;
}

void CMyCamera::Capture(int venderID,int screenNo,char *filename)
{
	switch(venderID)
	{
		case VENDER_TYPE_HAIKANG:
			m_haikang.Capture(screenNo,filename);
			break;
		case VENDER_TYPE_DAHUA:
			m_dahua.Capture(screenNo,filename);
			break;
		case VENDER_TYPE_YAAN:
			m_yaAn.Capture(screenNo,filename);
			break;
		default:
			break;
	}
}


void CMyCamera::PtzControl(int venderID, int screenNo, int type, BOOL dwStop, int param)
{
	switch(venderID)
	{
		case VENDER_TYPE_HAIKANG:
			m_haikang.PtzControl(m_RealHandle[screenNo],type,dwStop,param);
			break;
		case VENDER_TYPE_DAHUA:
			m_dahua.PtzControl(m_LoginHandle[screenNo],type,dwStop,param);
			break;
		case VENDER_TYPE_YAAN:
			m_yaAn.PtzControl(m_LoginHandle[screenNo],type,dwStop,param);
			break;
		default:
			break;
	}
}

void CMyCamera::StartRecord(int venderID,int screenNo,char *filename)
{
	switch(venderID)
	{
		case VENDER_TYPE_HAIKANG:
//			m_haikang.StartRecord(screenNo,filename);
			break;
		case VENDER_TYPE_DAHUA:
//			m_dahua.StartRecord(screenNo,filename);
			break;
		case VENDER_TYPE_YAAN:
			m_yaAn.StartRecord(screenNo,filename);
			break;
		default:
			break;
	}
}
void CMyCamera::StopRecord(int venderID,int screenNo)
{
	switch(venderID)
	{
		case VENDER_TYPE_HAIKANG:
//			m_haikang.StopRecord(screenNo);
			break;
		case VENDER_TYPE_DAHUA:
//			m_dahua.StopRecord(screenNo);
			break;
		case VENDER_TYPE_YAAN:
			m_yaAn.StopRecord(screenNo);
			break;
		default:
			break;
	}
}