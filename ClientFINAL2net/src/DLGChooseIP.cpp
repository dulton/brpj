// DLGChooseIP.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DLGChooseIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGChooseIP dialog
CDLGChooseIP::CDLGChooseIP(CWnd* pParent /*=NULL*/)
: CDialog(CDLGChooseIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGChooseIP)
	m_view = FALSE;
	//}}AFX_DATA_INIT
	iptotal=0;
	chooseip_total=0;
	int i;
	for( i=0;i<1024;i++)
		iplist[i].view=TRUE;
	PlanModedlg.modeflag=1;
	PlanModedlg.info.camID=0;
}


void CDLGChooseIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGChooseIP)
	DDX_Control(pDX, IDC_LIST_ALARM, m_listchoose);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Check(pDX, IDC_CHECK_VIEW, m_view);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGChooseIP, CDialog)
//{{AFX_MSG_MAP(CDLGChooseIP)
ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
ON_BN_CLICKED(IDC_BUTTON_SetPlanrecordS, OnBUTTONSetPlanrecordS)
ON_NOTIFY(NM_CLICK, IDC_TREE, OnClickTree)
ON_BN_CLICKED(IDC_BUTTON_SetPlanrecordN, OnBUTTONSetPlanrecordN)
	ON_BN_CLICKED(IDC_BUTTON_PlanDayStart, OnBUTTONPlanDayStart)
	ON_BN_CLICKED(IDC_BUTTON_PlanWeekStart, OnBUTTONPlanWeekStart)
	ON_BN_CLICKED(IDC_CHECK_VIEW, OnCheckView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGChooseIP message handlers
BOOL CDLGChooseIP::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	BuildTree();
	
	m_listchoose.InsertColumn(0, "摄像头ID" , LVCFMT_LEFT, 70);
	m_listchoose.InsertColumn(1, "IP" , LVCFMT_LEFT, 110);
	m_listchoose.InsertColumn(2, "用户名" , LVCFMT_LEFT, 100);
	m_listchoose.InsertColumn(3, "密码", LVCFMT_LEFT, 100);
	m_listchoose.InsertColumn(4, "端口", LVCFMT_LEFT, 60);
	m_listchoose.InsertColumn(5, "通道", LVCFMT_LEFT, 60);
	m_listchoose.InsertColumn(6, "预览", LVCFMT_LEFT, 60);
	m_listchoose.InsertColumn(7, "预览窗", LVCFMT_LEFT, 60);

	m_listchoose.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	int i,j;
	int a;
	for( i=0;i<iptotal;i++)
	{
		if(curIP == iplist[i].ip  && 
			curCamID== iplist[i].camID &&
			curPID==iplist[i].policeID)
		{
			m_tree.SelectItem(iplist[i].item);
			m_view=iplist[i].view;
			break;
		}
	}
	UpdateData(FALSE);

	m_listchoose.DeleteAllItems();
	//重设复选框 必须
	m_tree.ModifyStyle(TVS_CHECKBOXES,0);
	m_tree.ModifyStyle(0,TVS_CHECKBOXES);
	
	for( i=0;i<testdlg->zog_total;i++)
	{
		for(j=0;j<iptotal;j++)
		{
			if(testdlg->zog_VideoChlInfo[i].camid  == iplist[j].camID)
			{
				a=m_tree.SetCheck(iplist[j].item,1);
				
				AlarmMassage(iplist[j].camID,iplist[j].ip,iplist[j].user,iplist[j].psw,
					iplist[j].channel,iplist[j].port,iplist[j].view,testdlg->zog_VideoChlInfo[i].m_iWndSn);
			}
		}
	}
	//标志
	if(chooseip_total >=1 )
	{
		if(PlanRecorddlg[0].RecordStatus.planday || PlanRecorddlg[0].RecordStatus.planweek)
			GetDlgItem(IDC_TREE)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_TREE)->EnableWindow(TRUE);

		if(	PlanRecorddlg[0].RecordStatus.planweek)
			GetDlgItem(IDC_BUTTON_PlanWeekStart)->SetWindowText("停止周计划");
		else
			GetDlgItem(IDC_BUTTON_PlanWeekStart)->SetWindowText("开始周计划");

		if(	PlanRecorddlg[0].RecordStatus.planday)
			GetDlgItem(IDC_BUTTON_PlanDayStart)->SetWindowText("停止日计划");
		else
			GetDlgItem(IDC_BUTTON_PlanDayStart)->SetWindowText("开始日计划");
	}

	return TRUE;
}

void CDLGChooseIP::BuildTree(void)
{	
	HTREEITEM hItemA,hItemB,hItemC;
	
	unsigned int i,j,k;
	iptotal=0;
	
	vector<CString> idA,idB,idC;
	vector<CString> nameA,nameB,nameC;
	
	unsigned int m;
	vector<int> id;
	vector<CString> live,ip,user, pwd,channel,port;
	CString pid_t;

	m_tree.DeleteAllItems();
	poracle->ReadInfoFromOracleDB_TORG("__",idA,nameA);
	
	for(i=0;i<idA.size();i++)
	{
		iplist[iptotal].Parent_item=TVI_ROOT;
		hItemA=iplist[iptotal].item=m_tree.InsertItem(nameA[i],iplist[iptotal].Parent_item);
		iplist[iptotal].policeID=idA[i];
		iptotal++;
		
		poracle->ReadInfoFromOracleDB_TORG(iplist[iptotal-1].policeID, idB,nameB);
		
		for(j=0;j<idB.size();j++)
		{
			iplist[iptotal].Parent_item=hItemA;
			hItemB=iplist[iptotal].item=m_tree.InsertItem(nameB[j],iplist[iptotal].Parent_item);
			iplist[iptotal].policeID=idB[j];
			iptotal++;
			
			poracle->ReadInfoFromOracleDB_TORG(iplist[iptotal-1].policeID, idC,nameC);
			
			for(k=0;k<idC.size();k++)
			{
				iplist[iptotal].Parent_item=hItemB;
				hItemC=iplist[iptotal].item=m_tree.InsertItem(nameC[k],iplist[iptotal].Parent_item);
				pid_t=iplist[iptotal].policeID=idC[k];
				iptotal++;
				
				poracle->ReadInfoFromOracleDB_TBCAMERAINFO(iplist[iptotal-1].policeID,id,live,ip,user,pwd,channel,port);
				
				for(m=0;m<id.size();m++)
				{
					iplist[iptotal].Parent_item=hItemC;
					iplist[iptotal].item=m_tree.InsertItem(live[m],iplist[iptotal].Parent_item);
					iplist[iptotal].policeID=pid_t;
					iplist[iptotal].sername=live[m];
					iplist[iptotal].camID=id[m];
					iplist[iptotal].ip=ip[m];
					
					iplist[iptotal].user=user[m];
					iplist[iptotal].psw=pwd[m];
					iplist[iptotal].channel=channel[m];
					iplist[iptotal].port=port[m];
					
					iptotal++;
				}
				
				id.clear();
				live.clear();
				ip.clear();
				
				user.clear();
				pwd.clear();
				channel.clear();
				port.clear();
			}
			idC.clear();
			nameC.clear();
		}
		
		idB.clear();
		nameB.clear();
	}
	idA.clear();
	nameA.clear();
}


void CDLGChooseIP::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM item_sel=m_tree.GetSelectedItem();
	
	char id[60];
	int i;
	for(i=0;i<iptotal;i++)
	{
		if(iplist[i].item == item_sel)
		{
			if(!iplist[i].ip.IsEmpty())
			{
				curCamID=iplist[i].camID;
				curPID=iplist[i].policeID;
				curIP=iplist[i].ip;
				
				if(!iplist[i].user.IsEmpty())
					curUser=iplist[i].user;
				
				if(!iplist[i].psw.IsEmpty())
					curPsw=iplist[i].psw;
				
				if(!iplist[i].channel.IsEmpty())
					sscanf((LPCTSTR)iplist[i].channel,"%d",&curChannel);
				
				if(!iplist[i].port.IsEmpty())
					sscanf((LPCTSTR)iplist[i].port,"%d",&curPort);
				
				sprintf(id,"%s+%d",curPID,curCamID);
				
				GetDlgItem(IDC_EDIT_CHOOSE)->SetWindowText(id);
				GetDlgItem(IDC_EDIT_DISPIP)->SetWindowText(curIP);
				GetDlgItem(IDC_EDIT_USER)->SetWindowText(curUser);
				GetDlgItem(IDC_EDIT_PSW)->SetWindowText(curPsw);
				
				sprintf(id,"%d",curChannel);
				GetDlgItem(IDC_EDIT_CHANNEL)->SetWindowText(id);
				sprintf(id,"%d",curPort);
				GetDlgItem(IDC_EDIT_PORT)->SetWindowText(id);
				
				m_view=iplist[i].view;
			
				UpdateData(FALSE);
			}
			else
			{
				GetDlgItem(IDC_EDIT_CHOOSE)->SetWindowText("");
				GetDlgItem(IDC_EDIT_DISPIP)->SetWindowText("");
				GetDlgItem(IDC_EDIT_USER)->SetWindowText("");
				GetDlgItem(IDC_EDIT_PSW)->SetWindowText("");
				
				GetDlgItem(IDC_EDIT_CHANNEL)->SetWindowText("");
				GetDlgItem(IDC_EDIT_PORT)->SetWindowText("");
				curCamID=-1;
			}
			break;
		}
	}
	
	*pResult = 0;
}

void CDLGChooseIP::AlarmMassage(long int camID,CString ip,CString user,CString psw,
								CString channel,CString port,BOOL view,int winno)
{
	char str[10];
	int nItem;
	
	sprintf(str,"%d",camID);
	nItem=m_listchoose.InsertItem(0,str);
	m_listchoose.SetItemText(nItem,1,ip);
	m_listchoose.SetItemText(nItem,2,user);
	m_listchoose.SetItemText(nItem,3,psw);
	m_listchoose.SetItemText(nItem,4,port);
	m_listchoose.SetItemText(nItem,5,channel);
	if(view)
	{
		m_listchoose.SetItemText(nItem,6,"开启");
	
		sprintf(str,"%d",winno);
		m_listchoose.SetItemText(nItem,7,str);
	}
	else
		m_listchoose.SetItemText(nItem,6,"关闭");

}

void CDLGChooseIP::OnBUTTONSetPlanrecordS() 
{
	// TODO: Add your control notification handler code here
	int i;
	for(i=0;i<chooseip_total;i++)
	{
		if(curCamID==PlanRecorddlg[i].info.camID)
		{
			PlanRecorddlg[i].DoModal();
			break;
		}
	}
}

void CDLGChooseIP::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int i;
	
	int num=0;
	testdlg->zog_Videototal=0;

	m_listchoose.DeleteAllItems();
	
	for( i=0;i<iptotal;i++)
	{
		if(m_tree.GetCheck(iplist[i].item) && !iplist[i].ip.IsEmpty())
		{
			PlanRecorddlg[num].info.camID=iplist[i].camID;
			PlanRecorddlg[num].m_hHandle=-1;

			if(testdlg->zog_Videototal<16 && iplist[i].view)
			{
				testdlg->zog_VideoChlInfo[num].m_bViewing=TRUE;
				testdlg->zog_VideoChlInfo[num].m_iWndSn=testdlg->zog_Videototal;
				testdlg->g_VideoWndInfo[testdlg->zog_Videototal].m_iChlSn=num;
				testdlg->zog_Videototal++;
			}
			else
			{
				testdlg->zog_VideoChlInfo[num].m_bViewing=FALSE;
				iplist[i].view=FALSE;
				testdlg->zog_VideoChlInfo[num].m_iWndSn=0;
			}

			testdlg->zog_VideoChlInfo[num].m_iTransType=3;

			testdlg->zog_VideoChlInfo[num].m_sServerName=iplist[i].sername;
			testdlg->zog_VideoChlInfo[num].m_sServerUrl=iplist[i].ip;
			testdlg->zog_VideoChlInfo[num].m_sUserName=iplist[i].user;
			testdlg->zog_VideoChlInfo[num].m_sPsw=iplist[i].psw;
			testdlg->zog_VideoChlInfo[num].pid=iplist[i].policeID;
			testdlg->zog_VideoChlInfo[num].camid=iplist[i].camID;

			sscanf(iplist[i].channel,"%d",&testdlg->zog_VideoChlInfo[num].m_iChlSn);
			sscanf(iplist[i].port,"%d",&testdlg->zog_VideoChlInfo[num].m_iPort);

			AlarmMassage(testdlg->zog_VideoChlInfo[num].camid,
				testdlg->zog_VideoChlInfo[num].m_sServerUrl,
				testdlg->zog_VideoChlInfo[num].m_sUserName,
				testdlg->zog_VideoChlInfo[num].m_sPsw,
				iplist[i].channel,
				iplist[i].port,
				testdlg->zog_VideoChlInfo[num].m_bViewing,
				testdlg->zog_VideoChlInfo[num].m_iWndSn);

			num++;
		}
	}

	chooseip_total=num;
	testdlg->zog_total=num;

//	*pResult = 0;
}

void CDLGChooseIP::OnBUTTONSetPlanrecordN() 
{
	// TODO: Add your control notification handler code here
	if(!(PlanRecorddlg[0].RecordStatus.planday || PlanRecorddlg[0].RecordStatus.planweek))
		OnClickTree(NULL,NULL);

	//对第0个进行设置
	if(chooseip_total >=1)
		PlanModedlg.DoModal();
	
	if(10!=PlanModedlg.modeexec)
	{
		int i;
		for(i=0;i<chooseip_total;i++)
		{
			PlanRecorddlg[i].PlanReadALLC(PlanModedlg.modeexec);
			PlanRecorddlg[i].PlanSaveALLC(); 
		}
	}
}

void CDLGChooseIP::OnBUTTONPlanDayStart() 
{
	// TODO: Add your control notification handler code here
	int i;
	
	if(!(PlanRecorddlg[0].RecordStatus.planday || PlanRecorddlg[0].RecordStatus.planweek))
		OnClickTree(NULL,NULL);

	if(chooseip_total <1)
		return ;

	if(PlanRecorddlg[0].RecordStatus.planday)
	{

		GetDlgItem(IDC_BUTTON_PlanDayStart)->SetWindowText("开始日计划");
		for(i=0;i<chooseip_total;i++)
			PlanRecorddlg[i].RecordStatus.planday=0;
		//现在全部都停
		if(0==PlanRecorddlg[0].RecordStatus.planweek )
		{
			testdlg->StopPlay();
			GetDlgItem(IDC_TREE)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PlanDayStart)->SetWindowText("停止日计划");
		for(i=0;i<chooseip_total;i++)
		{
			PlanRecorddlg[i].PlanReadALLC(PlanRecorddlg[i].info.camID);
			PlanRecorddlg[i].RecordStatus.planday=1;
		}

		//要有一个开始
		if(0==PlanRecorddlg[0].RecordStatus.planweek )
		{
			testdlg->StartPlay();
			GetDlgItem(IDC_TREE)->EnableWindow(FALSE);
		}
	}
}

void CDLGChooseIP::OnBUTTONPlanWeekStart() 
{
	int i;

	if(!(PlanRecorddlg[0].RecordStatus.planday || PlanRecorddlg[0].RecordStatus.planweek))
		OnClickTree(NULL,NULL);

	if(chooseip_total <1)
		return ;
	// TODO: Add your control notification handler code here
	if(PlanRecorddlg[0].RecordStatus.planweek)
	{
		GetDlgItem(IDC_BUTTON_PlanWeekStart)->SetWindowText("开始周计划");
		for(i=0;i<chooseip_total;i++)
			PlanRecorddlg[i].RecordStatus.planweek=0;
		//现在全部都停
		if(0==PlanRecorddlg[0].RecordStatus.planday )
		{
			testdlg->StopPlay();
			GetDlgItem(IDC_TREE)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PlanWeekStart)->SetWindowText("停止周计划");
		for(i=0;i<chooseip_total;i++)
		{
			PlanRecorddlg[i].PlanReadALLC(PlanRecorddlg[i].info.camID);
			PlanRecorddlg[i].RecordStatus.planweek=1;
		}
		//要有一个开始
		if(0==PlanRecorddlg[0].RecordStatus.planday )
		{
			testdlg->StartPlay();
			GetDlgItem(IDC_TREE)->EnableWindow(FALSE);
		}
	}
}

void CDLGChooseIP::OnCheckView() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i;	
	for(i=0;i<iptotal;i++)
	{
		if(curCamID==iplist[i].camID)
		{
			iplist[i].view=m_view;
			break;
		}
	}
	//未选中 则还原选项
	if(i==chooseip_total)
	{
		if(m_view)
			m_view=0;
		else
			m_view=1;
		UpdateData(FALSE);
	}
	if(!(PlanRecorddlg[0].RecordStatus.planday || PlanRecorddlg[0].RecordStatus.planweek))
		OnClickTree(NULL,NULL);
}
void CDLGChooseIP::OnCancel() 
{	
	if(!(PlanRecorddlg[0].RecordStatus.planday || PlanRecorddlg[0].RecordStatus.planweek))
		OnClickTree(NULL,NULL);
	CDialog::OnCancel();
}
