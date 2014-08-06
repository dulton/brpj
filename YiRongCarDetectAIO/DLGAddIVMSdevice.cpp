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
	}
	if(m_unit.GetCount()>0)
		m_unit.SetCurSel(0);
	//全部显示模式
	FindMode=false;

	UpdateData(FALSE);
}


BOOL CDLGAddIVMSdevice::OnInitDialog() 
{
	CDialog::OnInitDialog();

	OracleIO.IVMS_ReadControlunit(Controlunit);
	OracleIO.IVMS_ReadRegionInfo(RegionInfo);
	OracleIO.IVMS_ReadStreamserver(Streamserver);
	OracleIO.IVMS_ReadPAGserver(PAGserver);

	InitALL();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
// CDLGAddIVMSdevice message handlers

void CDLGAddIVMSdevice::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();

		UpdateData(TRUE);
		if((Streamserver.size()<1 || PAGserver.size() <1) &&
			m_camvender.GetCurSel() == 1)
		{
				MessageBox("流媒体服务器或PAG地址不存在,不支持该协议");
				return ;
		
		}

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
		MessageBox("未找到信息");
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
}

void CDLGAddIVMSdevice::OnCbnCloseupComboUnit()
{
	// TODO: Add your control notification handler code here
}

void CDLGAddIVMSdevice::OnCbnCloseupComboRegion()
{
	// TODO: Add your control notification handler code here
}

void CDLGAddIVMSdevice::OnCbnCloseupComboCam()
{
	// TODO: Add your control notification handler code here
}

#endif