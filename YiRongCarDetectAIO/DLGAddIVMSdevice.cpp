// DLGAddIVMSdevice.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGAddIVMSdevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if IVMS_ORACLE_DEVICETREE

extern IO OracleIO;


// CDLGAddIVMSdevice dialog

IMPLEMENT_DYNAMIC(CDLGAddIVMSdevice, CDialog)

CDLGAddIVMSdevice::CDLGAddIVMSdevice(CWnd* pParent /*=NULL*/)
: CDialog(CDLGAddIVMSdevice::IDD, pParent)
, m_edit_find(_T(""))
{

}

CDLGAddIVMSdevice::~CDLGAddIVMSdevice()
{

}

void CDLGAddIVMSdevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIND, m_edit_find);
	DDX_Control(pDX, IDC_COMBO_CAMVENDER, m_camvender);
	DDX_Control(pDX, IDC_COMBO_ROOT, m_root);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_unit);
	DDX_Control(pDX, IDC_COMBO_REGION, m_region);
	DDX_Control(pDX, IDC_COMBO_CAM, m_cam);
}


BEGIN_MESSAGE_MAP(CDLGAddIVMSdevice, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGAddIVMSdevice::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGAddIVMSdevice::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDLGAddIVMSdevice::OnBnClickedButtonFind)
	ON_CBN_CLOSEUP(IDC_COMBO_ROOT, &CDLGAddIVMSdevice::OnCbnCloseupComboRoot)
	ON_CBN_CLOSEUP(IDC_COMBO_UNIT, &CDLGAddIVMSdevice::OnCbnCloseupComboUnit)
	ON_CBN_CLOSEUP(IDC_COMBO_REGION, &CDLGAddIVMSdevice::OnCbnCloseupComboRegion)
	ON_CBN_CLOSEUP(IDC_COMBO_CAM, &CDLGAddIVMSdevice::OnCbnCloseupComboCam)
END_MESSAGE_MAP()


void  CDLGAddIVMSdevice::InitALL(void)
{
	m_root.ResetContent();
	m_unit.ResetContent();
	m_region.ResetContent();
	m_cam.ResetContent();

	bool flag=true;
	long id;
	long i;

	list<struct CONTROL_UNIT_ST>::iterator beglist;

	for(beglist=Controlunit.begin(),i=0;beglist!=Controlunit.end();beglist++)
	{
		if(0 == beglist->ParentId)
		{
			m_root.AddString(beglist->name);
			beglist->comboxi=i;
			i++;
			if(flag)
			{
				id=beglist->nid;
				flag=false;
			}
		}
	}
	if(m_root.GetCount()>0)
		m_root.SetCurSel(0);

	for(beglist=Controlunit.begin(),i=0;beglist!=Controlunit.end();beglist++)
	{
		if(id == beglist->ParentId)
		{
			m_unit.AddString(beglist->name);
			beglist->comboxi=i;
			i++;
		}
		else 	if( beglist->ParentId >0) //不是根也不是当前值
		{
			//防止干扰
			beglist->comboxi=-1;
		}
	}
	if(m_unit.GetCount()>0)
		m_unit.SetCurSel(0);
	//全部显示模式
	FindMode=false;

}


BOOL CDLGAddIVMSdevice::OnInitDialog() 
{
	CDialog::OnInitDialog();

	OracleIO.IVMS_ReadControlunit(Controlunit);
	OracleIO.IVMS_ReadRegionInfo(RegionInfo);
	OracleIO.IVMS_ReadStreamserver(Streamserver);
	OracleIO.IVMS_ReadPAGserver(PAGserver);

	m_camvender.SetCurSel(0);
	InitALL();

	Rtspurl="";

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// CDLGAddIVMSdevice message handlers

void CDLGAddIVMSdevice::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	/*
	STREAM_SERVER.IP_ADDR   :554/pag://  PAG_SERVER.IP_ADDR  :7302:  CAMERA_INFO.INDEX_CODE:  CAMERA_INFO.CHAN_NUM :MAIN:TCP&streamform=gb28181 
	rtsp://35.24.252.104:554/pag://35.24.252.104:7302:35010200001310012505:0:MAIN:TCP&streamform=gb28181 
	rtsp://35.24.252.104:554/pag://35.24.252.104:7302:35010201001310013268:3:MAIN:TCP&streamform=gb28181
	*/

	UpdateData(TRUE);
	if((Streamserver.size()<1 || PAGserver.size() <1) &&
		m_camvender.GetCurSel() == 1)
	{
		MessageBox("流媒体服务器或PAG地址不存在,不支持该协议",MESSAGEBOX_TITLE);
		return ;
	}

	long id=m_cam.GetCurSel();
	if(id<0)
	{
		MessageBox("未选择监控点，点 取消 关闭窗口",MESSAGEBOX_TITLE);
		return ;
	}

	bool flag=true;
	char tempstr[260];
	list<struct CAMERA_INFO_LITE_ST>::iterator beglist;
	if(FindMode)
	{
		for(beglist=CaminfoFind.begin();beglist!=CaminfoFind.end();beglist++)
		{
			if(id == beglist->comboxi)
			{
				id=beglist->nid;
				flag=false;
				break;
			}
		}
	}
	else
	{
		for(beglist=Caminfo.begin();beglist!=Caminfo.end();beglist++)
		{
			if(id == beglist->comboxi)
			{
				id=beglist->nid;
				flag=false;
				break;
			}
		}
	}
	if(flag)
	{
		MessageBox("内部错误:未找到监控点",MESSAGEBOX_TITLE);
		return ;
	}

	OracleIO.IVMS_ReadCaminfoOne(&CamData,id,m_camvender.GetCurSel());

	if(1==m_camvender.GetCurSel())
	{
		flag=true;

		list<struct STREAM_SERVER_ST>::iterator sbeglist;

		for(sbeglist=Streamserver.begin();sbeglist!=Streamserver.end();sbeglist++)
		{
			if(CamData.STREAM_SVR_ID == sbeglist->nid)
			{
				sprintf(tempstr,"rtsp://%s:%d/",sbeglist->ip,sbeglist->RtspPort);
				flag=false;
				break;
			}
		}
		if(flag)
		{
			MessageBox("该摄像头的流媒体服务器未找到,不支持该协议",MESSAGEBOX_TITLE);
			return ;
		}

		Rtspurl+=tempstr;

		flag=true;
		list<struct PAG_SERVER_ST>::iterator pbeglist;

		for(pbeglist=PAGserver.begin();pbeglist!=PAGserver.end();pbeglist++)
		{
			if(CamData.PAG_SERVER_ID == pbeglist->nid)
			{
				sprintf(tempstr,"pag://%s:%d:",pbeglist->ip,pbeglist->DataPort);
				flag=false;
				break;
			}
		}
		if(flag)
		{
			MessageBox("该摄像头的PAG服务器未找到,不支持该协议",MESSAGEBOX_TITLE);
			return ;
		}
		Rtspurl+=tempstr;

		sprintf(tempstr,"%s:%d:MAIN:",CamData.IndexCode,CamData.channel);

		Rtspurl+=tempstr;

		if(1==CamData.RTP)
		{
			sprintf(tempstr,"UDP&streamform=gb28181");
		}
		else
			sprintf(tempstr,"TCP&streamform=gb28181");

		Rtspurl+=tempstr;
	}	

	UpdateData(FALSE);

	OnOK();
}

void CDLGAddIVMSdevice::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	Caminfo.clear();
	m_region.ResetContent();
	m_cam.ResetContent();
	OnCancel();
}

void CDLGAddIVMSdevice::OnBnClickedButtonFind()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_edit_find.GetLength()<1)
	{
		InitALL();
		return ;
	}

	int re=OracleIO.IVMS_ReadCaminfoFind(CaminfoFind,m_edit_find.GetBuffer(0));

	if(0==CaminfoFind.size() || 1!=re)
	{
		MessageBox("未找到信息",MESSAGEBOX_TITLE);
		return ;
	}

	FindMode=true;
	//清空
	m_root.ResetContent();
	m_unit.ResetContent();
	m_region.ResetContent();
	m_cam.ResetContent();
	long i;

	list<struct CAMERA_INFO_LITE_ST>::iterator beglist;
	for(beglist=CaminfoFind.begin(),i=0;beglist!=CaminfoFind.end();beglist++)
	{
		m_cam.AddString(beglist->name);
		beglist->comboxi=i;
		i++;
	}
	if(m_cam.GetCount()>0)
		m_cam.SetCurSel(0);

	UpdateData(FALSE);

}

void CDLGAddIVMSdevice::OnCbnCloseupComboRoot()
{
	// TODO: Add your control notification handler code here
	if(FindMode)
		return ;
	UpdateData(TRUE);

	long i;
	long id=m_root.GetCurSel();
	if(id<0)
		return ;
	list<struct CONTROL_UNIT_ST>::iterator beglist;

	for(beglist=Controlunit.begin();beglist!=Controlunit.end();beglist++)
	{
		if(id == beglist->comboxi && 0 == beglist->ParentId)
		{
			id=beglist->nid;
			break;
		}
	}

	m_unit.ResetContent();
	for(beglist=Controlunit.begin(),i=0;beglist!=Controlunit.end();beglist++)
	{
		if(id == beglist->ParentId)
		{
			m_unit.AddString(beglist->name);
			beglist->comboxi=i;
			i++;
		}
		else 	if( beglist->ParentId >0) //不是根也不是当前值
		{
			//防止干扰
			beglist->comboxi=-1;
		}

	}

	if(m_unit.GetCount()>0)
		m_unit.SetCurSel(0);

	UpdateData(FALSE);
}

void CDLGAddIVMSdevice::OnCbnCloseupComboUnit()
{
	// TODO: Add your control notification handler code here
	if(FindMode)
		return ;
	UpdateData(TRUE);

	long i;
	long id=m_unit.GetCurSel();
	if(id<0)
		return ;
	list<struct CONTROL_UNIT_ST>::iterator beglist;

	for(beglist=Controlunit.begin();beglist!=Controlunit.end();beglist++)
	{
		if(id == beglist->comboxi && 0 != beglist->ParentId)
		{
			id=beglist->nid;
			break;
		}
	}

	m_region.ResetContent();
	list<struct REGION_INFO_ST>::iterator rbeglist;

	for(rbeglist=RegionInfo.begin(),i=0;rbeglist!=RegionInfo.end();rbeglist++)
	{
		if(id == rbeglist->ControlUnitId)
		{
			m_region.AddString(rbeglist->name);
			rbeglist->comboxi=i;
			i++;
		}
		else 	
		{
			//防止干扰
			rbeglist->comboxi=-1;
		}
	}
	if(m_region.GetCount()>0)
		m_region.SetCurSel(0);

	UpdateData(FALSE);
}

void CDLGAddIVMSdevice::OnCbnCloseupComboRegion()
{
	// TODO: Add your control notification handler code here
	if(FindMode)
		return ;
	UpdateData(TRUE);

	long i;
	long id=m_region.GetCurSel();
	if(id<0)
		return ;
	list<struct REGION_INFO_ST>::iterator beglist;

	for(beglist=RegionInfo.begin();beglist!=RegionInfo.end();beglist++)
	{
		if(id == beglist->comboxi)
		{
			id=beglist->nid;
			break;
		}
	}

	OracleIO.IVMS_ReadCaminfoALL(Caminfo,id);

	m_cam.ResetContent();
	list<struct CAMERA_INFO_LITE_ST>::iterator rbeglist;

	for(rbeglist=Caminfo.begin(),i=0;rbeglist!=Caminfo.end();rbeglist++)
	{
		if(id == rbeglist->RegionId)
		{
			m_cam.AddString(rbeglist->name);
			rbeglist->comboxi=i;
			i++;
		}
		else
		{
			//防止干扰
			rbeglist->comboxi=-1;
		}
	}
	if(m_cam.GetCount()>0)
		m_cam.SetCurSel(0);

	UpdateData(FALSE);
}

void CDLGAddIVMSdevice::OnCbnCloseupComboCam()
{
	// TODO: Add your control notification handler code here
}

#endif