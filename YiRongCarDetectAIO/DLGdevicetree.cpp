// DLGdevicetree.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGdevicetree.h"
#include "YiRongCarDetectAIODlg.h"

#include "DLGWarnning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////lynn/////////////////
extern IO OracleIO;
extern CYiRongCarDetectAIODlg *DlgMain;
////////////////lynn/////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;



/////////////////////////////////////////////////////////////////////////////
// CDLGdevicetree dialog


CDLGdevicetree::CDLGdevicetree(CWnd* pParent /*=NULL*/)
: CDialog(CDLGdevicetree::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGdevicetree)
	// NOTE: the ClassWizard will add member initialization here
	m_selectItem = NULL;
	iptotal = 0;
	//}}AFX_DATA_INIT
}


void CDLGdevicetree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGdevicetree)
	DDX_Control(pDX, IDC_TREE_DEVICE, m_DeviceTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGdevicetree, CDialog)
	//{{AFX_MSG_MAP(CDLGdevicetree)
	ON_WM_PAINT()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_DEVICE, OnRclickTreeDevice)
	ON_COMMAND(ID_MENUITEM_ADDDEVICE, OnMenuitemAdddevice)
	ON_COMMAND(ID_MENUITEM_EDITDEVICE, OnMenuitemEdit)
	ON_COMMAND(ID_MENUITEM_DELETEDEVICE, OnMenuitemDeleteDevice)
	ON_COMMAND(ID_MENUITEM_DELETEAREA, OnMenuitemDeletearea)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEVICE, OnDblclkTreeDevice)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENUITEM_ADDIVMSDEVICE, &CDLGdevicetree::OnMenuitemAddivmsdevice)
	ON_COMMAND(ID_MENUITEM_FLUSHTREE, &CDLGdevicetree::OnMenuitemFlushtree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGdevicetree message handlers
BOOL CDLGdevicetree::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_DeviceTree.SetBkColor(RGB(232,248,254));	//树列表背景色

	OnMenuitemUpdate();

	//树的图标
	imagelist.Create(16, 16, ILC_COLOR8|ILC_MASK, 0, 4);    
	imagelist.Add(AfxGetApp()->LoadIcon(IDI_TREE_AREA));        // ico图标
	imagelist.Add(AfxGetApp()->LoadIcon(IDI_TREE_CAM));
	m_DeviceTree.SetImageList(&imagelist, TVSIL_NORMAL);  // 建立 imagelist 与 tree的映射关系


	//放在最后
	AutoSize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLGdevicetree::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//	GetParent()->GetClientRect(&rc);
	//	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	//	MoveWindow(rc);
/*
	//树
	CRect tree_Rect;
	tree_Rect.top = rc.top+5;
	tree_Rect.bottom = rc.bottom -5;
	tree_Rect.left = rc.left+5;
	tree_Rect.right = rc.right-5;
*/
	m_DeviceTree.MoveWindow(rc);

	Invalidate();
}


void CDLGdevicetree::OnCancel()
{
	DlgMain->OnCancel();
}
void CDLGdevicetree::OnOK()
{

}

void CDLGdevicetree::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
	//贴背景图	
	CRect    rect;     
	GetClientRect(&rect);     

	//从资源中载入位图     
	CBitmap    bitmap;     
	bitmap.LoadBitmap(IDB_BACK_TREE);    
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC    memdc;     
	memdc.CreateCompatibleDC(&dc);     
	memdc.SelectObject(bitmap); 
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	memdc.DeleteDC();

	CDialog::OnPaint();
}

void CDLGdevicetree::OnRclickTreeDevice(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint point;
	CMenu menu;
	CMenu *ptrMenu;

	GetCursorPos(&point);
	CPoint PointInTree = point;
	m_DeviceTree.ScreenToClient( &PointInTree);
	UINT nFlag = TVHT_ONITEM;// 当在 m_DeviceTree 上右击时
	m_selectItem = m_DeviceTree.HitTest( PointInTree, &nFlag );

	// lynn 20130419 屏蔽修复 空树时无法右击的BUG
	//	if(m_selectItem != NULL)
	{
		if(m_DeviceTree.GetParentItem(m_selectItem) == NULL)
		{
			menu.LoadMenu(IDR_MENU_RCTREE);
			ptrMenu=menu.GetSubMenu(0);
			ptrMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this,NULL);
		}
		else
		{
			menu.LoadMenu(IDR_MENU_RCTREE1);
			ptrMenu=menu.GetSubMenu(0);
			ptrMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this,NULL);	
		}
	}
	*pResult = 0;
}

void CDLGdevicetree::OnMenuitemAdddevice() 
{
	if(!DlgLogin.CurrentUser.device)
	{
		MessageBox("无 设备管理 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	// TODO: Add your command handler code here
	HTREEITEM Item = m_DeviceTree.GetRootItem();
	int count = 0;
	bool flag=true;
	while(Item != NULL)
	{
		if(Item == m_selectItem)
		{
			flag=false;
			break;
		}
		Item = m_DeviceTree.GetNextItem(Item,TVGN_NEXT);
		count++;
	}
	DlgAddDevice.AreaComboCur = count;
	if(DlgAddDevice.DoModal() == IDOK)
	{
		//HTREEITEM Item;
		//Item = m_DeviceTree.GetRootItem();
		//for(int i=0;i<DlgAddDevice.AreaComboCur;i++)
		//{
		//	Item = m_DeviceTree.GetNextItem(Item,TVGN_NEXT);
		//}
		//m_DeviceTree.InsertItem(DlgAddDevice.m_CamName,Item);
		if(DlgAddDevice.AddAreaFlag)
		{
			m_DeviceTree.InsertItem(DlgAddDevice.m_AddArea,0,0,TVI_ROOT);
			OracleIO.DEVICE_AddNewArea(DlgAddDevice.m_AddArea);
			DlgAddDevice.AddAreaFlag = false;
		}
		else
		{
			if(flag)
			{
				MessageBox("请在添加设备 中 添加区域",MESSAGEBOX_TITLE);
				return ;
			}

		TCHAR crossIndex[64]="";	//卡口编号
		long crossID=0;	//卡口ID

			OracleIO.DEVICE_AddNewCamera(DlgAddDevice.m_CamArea,\
				DlgAddDevice.m_CamName,\
				DlgAddDevice.m_CamIpAddr,\
				DlgAddDevice.m_CamPort,\
				DlgAddDevice.m_CamChannel,\
				DlgAddDevice.m_CamUser,\
				DlgAddDevice.m_CamPsw,
				DlgAddDevice.VenderComboCur,
				DlgAddDevice.m_CamRtspurl,
				DlgAddDevice.RTPComboCur,
				DlgAddDevice.DecodeTagComboCur,
				DlgAddDevice.m_longitude,
				DlgAddDevice.m_latitude,
				crossIndex,
				crossID,
				DlgLogin.CurrentUser.nid,
				DlgAddDevice.m_level);

			DlgMain->ShowCameraMessage(	DlgAddDevice.m_CamName.GetBuffer(0),"添加设备成功",0);
		}
		OnMenuitemUpdate();
	}
}

void CDLGdevicetree::OnMenuitemUpdate() 
{
	// TODO: Add your command handler code here
	vector<struct DEVICE_AREA_ST> OrgName;
	vector<IPLIST> CameraList;
	//读数据库所有的区域
	OracleIO.DEVICE_ReadAllOrgName(OrgName);
	HTREEITEM hItem;
	HTREEITEM childItem;
	DlgAddDevice.AreaCount = OrgName.size();
	iptotal = 0;

	//删除所有的树
	m_DeviceTree.DeleteAllItems();
	for(int i=0;i<OrgName.size();i++)
	{
		if(i == MAX_AREA)
		{
			MessageBox("超过最大区域限制，超过不部分将不显示",MESSAGEBOX_TITLE);
			break;
		}
		strcpy(DlgAddDevice.AreaList[i].name, OrgName[i].name);
		DlgAddDevice.AreaList[i].nid = OrgName[i].nid;
		hItem = m_DeviceTree.InsertItem(OrgName[i].name,0,0,TVI_ROOT);		//在根结点上添加区域
		CameraList.clear();
		OracleIO.DEVICE_ReadCameraInfo(OrgName[i].name,CameraList);
		for(int j=0;j<CameraList.size();j++)
		{
			//等级查看
			if( CameraList[j].userLV < 	DlgLogin.CurrentUser.level)
				continue;

			iplist[iptotal].area = CameraList[j].area;
			iplist[iptotal].camID = CameraList[j].camID;
			iplist[iptotal].ip = CameraList[j].ip;
			iplist[iptotal].name = CameraList[j].name;
			iplist[iptotal].port = CameraList[j].port;
			iplist[iptotal].channel = CameraList[j].channel;
			iplist[iptotal].psw = CameraList[j].psw;
			iplist[iptotal].user = CameraList[j].user;
			iplist[iptotal].Parent_item = hItem;
			iplist[iptotal].venderID = CameraList[j].venderID;
			iplist[iptotal].Rtspurl= CameraList[j].Rtspurl;
			iplist[iptotal].RTP= CameraList[j].RTP;
			iplist[iptotal].DecodeTag= CameraList[j].DecodeTag;
			strcpy(iplist[iptotal].longitude, CameraList[j].longitude);
			strcpy(iplist[iptotal].latitude,CameraList[j].latitude);

			strcpy(iplist[iptotal].crossIndex, CameraList[j].crossIndex);
			iplist[iptotal].crossID= CameraList[j].crossID;
			iplist[iptotal].userID= CameraList[j].userID;
			iplist[iptotal].userLV= CameraList[j].userLV;

			childItem = m_DeviceTree.InsertItem(iplist[iptotal].name,1,1,hItem);		//添加设备节点
			iplist[iptotal].item = childItem;
			iptotal++;
		}
	}
}


void CDLGdevicetree::OnMenuitemEdit() 
{
	if(!DlgLogin.CurrentUser.device)
	{
		MessageBox("无 设备管理 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	// TODO: Add your command handler code here
	if(m_DeviceTree.GetParentItem(m_selectItem) == NULL)		//编辑区域预留
	{
		HTREEITEM Item;
		Item = m_DeviceTree.GetRootItem();
		while(Item != NULL)
		{
			if(m_selectItem == Item)
			{
				//编辑区域
				break;
			}
			Item = m_DeviceTree.GetNextItem(Item,TVGN_NEXT);
		}
	}
	else														//编辑设备
	{
		for(int i=0;i<MAX_AREA;i++)
		{
			if(iplist[i].item == m_selectItem)
			{
				HTREEITEM Item;
				Item = m_DeviceTree.GetRootItem();
				int count=0;
				while(Item != NULL)
				{
					if(iplist[i].Parent_item == Item)			//获取到是第几个父节点
						break;
					count++;
					Item = m_DeviceTree.GetNextItem(Item,TVGN_NEXT);
				}
				DlgAddDevice.AreaComboCur = count;
				DlgAddDevice.m_CamPort = iplist[i].port;
				DlgAddDevice.m_CamChannel = iplist[i].channel;
				DlgAddDevice.m_CamIpAddr = iplist[i].ip;
				DlgAddDevice.m_CamName = iplist[i].name;
				DlgAddDevice.m_CamPsw = iplist[i].psw;
				DlgAddDevice.m_CamUser = iplist[i].user;
				DlgAddDevice.VenderComboCur = iplist[i].venderID;
				DlgAddDevice.m_CamRtspurl= iplist[i].Rtspurl;
				DlgAddDevice.RTPComboCur= iplist[i].RTP;
				DlgAddDevice.DecodeTagComboCur= iplist[i].DecodeTag;
				DlgAddDevice.m_longitude= iplist[i].longitude;
				DlgAddDevice.m_latitude= iplist[i].latitude;
				DlgAddDevice.m_level= iplist[i].userLV;

				//判断设备是否存在 如果不存在 强制更新
				if(false==OracleIO.DEVICE_JudgeCamera(iplist[i].camID))
				{
					DlgMain->ShowCameraMessage(iplist[i].name.GetBuffer(0),"摄像头被其他用户删除，强制刷新设备树",0);
					OnMenuitemUpdate();
					return ;
				}

				if(DlgAddDevice.m_level<DlgLogin.CurrentUser.level)
				{
					MessageBox("不能修改高级别的设备",MESSAGEBOX_TITLE);
					return;
				}

				if(DlgAddDevice.DoModal() == IDOK)
				{
					//修改定时录制计划
					OracleIO.RECORD_PlanTable_UpdateCamInfo(
						iplist[i].camID,
						DlgAddDevice.m_CamArea.GetBuffer(0),\
						DlgAddDevice.m_CamName.GetBuffer(0),\
						DlgAddDevice.m_CamIpAddr.GetBuffer(0));
					//做其他事
					OracleIO.DEVICE_UpdateCameraInfo(iplist[i].camID,\
						DlgAddDevice.m_CamArea,\
						DlgAddDevice.m_CamName,\
						DlgAddDevice.m_CamIpAddr,\
						DlgAddDevice.m_CamPort,\
						DlgAddDevice.m_CamChannel,\
						DlgAddDevice.m_CamUser,\
						DlgAddDevice.m_CamPsw,\
						DlgAddDevice.VenderComboCur,
						DlgAddDevice.m_CamRtspurl,
						DlgAddDevice.RTPComboCur,
						DlgAddDevice.DecodeTagComboCur,
						DlgAddDevice.m_longitude,
						DlgAddDevice.m_latitude,
						 iplist[i].crossIndex,
						  iplist[i].crossID,
						  iplist[i].userID,
						  DlgAddDevice.m_level);

					DlgMain->ShowCameraMessage(	DlgAddDevice.m_CamName.GetBuffer(0),"编辑设备成功",0);
					OnMenuitemUpdate();

					//必须放到	OnMenuitemUpdate后面 否则IPLIST还未更新
					for(int j=0;j<MAX_DEVICE_NUM;j++)
					{
						if(DlgMain->DlgScreen.m_videoInfo[j].camID == 	iplist[i].camID)
						{
							//如果正在播放。则
							if(DlgMain->DlgScreen.GetCurWindPlayState(j))
							{
								bool beforedetect=DlgMain->DlgScreen.GetDetectState(j);

								bool beforealarm=DlgMain->DlgScreen.GetAlarmState(j);

								bool beforerecord=DlgMain->DlgScreen.GetRecordState(j);

								//重新播放
								DlgMain->DlgScreen.StartPlay(
									iplist[i].camID,
									iplist[i].area.GetBuffer(0),
									iplist[i].name.GetBuffer(0),
									iplist[i].ip.GetBuffer(0),
									iplist[i].port,
									iplist[i].channel,//20140718
									iplist[i].user.GetBuffer(0),
									iplist[i].psw.GetBuffer(0),
									j,
									0,
									iplist[i].venderID,
									iplist[i].Rtspurl.GetBuffer(0),
									iplist[i].RTP,
									iplist[i].DecodeTag);

								//恢复状态
								if(beforedetect)
									DlgMain->DlgNormal.OpenDetect(j);

								if(beforealarm)
								{
									DlgMain->DlgNormal.OpenDetect(j);
									DlgMain->DlgNormal.OpenAlarm(j);
								}
								if(beforerecord)
									DlgMain->DlgNormal.OpenRecord(j);
							}
							break;
						}
					}
				}
				break;
			}
		}

	}	
}

void CDLGdevicetree::OnMenuitemDeleteDevice() 
{
	if(!DlgLogin.CurrentUser.device)
	{
		MessageBox("无 设备管理 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	// TODO: Add your command handler code here
	int i;
	for(i=0;i<MAX_AREA;i++)
	{
		if(iplist[i].item == m_selectItem)
		{
			break;
		}
	}

	if(i==MAX_AREA)
	{
		OnMenuitemUpdate();
		return ;
	}

	CDLGWarnning dlgw;
	dlgw.m_wintxt=iplist[i].name;
	dlgw.m_warntxt="摄像头将被清空，不可恢复";
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{
		DlgMain->DlgScreen.DeleteDevice(iplist[i].camID);

#if OPEN_CS_MODE	
		OracleIO.Mission_ADD(iplist[i].camID,DlgLogin.CurrentUser.nid,0);
		Sleep(1000);
#endif

		OracleIO.DEVICE_DeleteCamera(iplist[i].camID);
		//删除定时录制计划
		OracleIO.RECORD_PlanTable_DeleteWithCamID(iplist[i].camID);

		DlgMain->ShowCameraMessage(iplist[i].name.GetBuffer(0),"删除设备成功",0);

		OnMenuitemUpdate();
	}
	else
		return ;

}

void CDLGdevicetree::OnMenuitemDeletearea() 
{
	if(!DlgLogin.CurrentUser.device)
	{
		MessageBox("无 设备管理 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	// TODO: Add your command handler code here
	HTREEITEM Item;
	int ItemCount=0;
	int tempflag=0;
	Item = m_DeviceTree.GetRootItem();
	while(Item != NULL)
	{
		if(m_selectItem == Item)
		{
			tempflag=1;
			break;
		}
		Item = m_DeviceTree.GetNextItem(Item,TVGN_NEXT);
		ItemCount++;
	}	

	if(0==tempflag)
	{
		OnMenuitemUpdate();
		return ;
	}
	CDLGWarnning dlgw;
	dlgw.m_wintxt=DlgAddDevice.AreaList[ItemCount].name;
	dlgw.m_warntxt="区域内的摄像头都将被清空，不可恢复";
	int nResponse=dlgw.DoModal();
	if (nResponse == IDOK)
	{
		list<long> ncameraList;
		ncameraList.clear();
		OracleIO.DEVICE_GetCameraWithAreaID(DlgAddDevice.AreaList[ItemCount].nid,ncameraList);
		if(ncameraList.size()>0)
		{
			list<long>::iterator beglist;
			for(beglist=ncameraList.begin();beglist!=ncameraList.end();beglist++)
			{		
				DlgMain->DlgScreen.DeleteDevice((*beglist));
#if OPEN_CS_MODE	
				OracleIO.Mission_ADD((*beglist),DlgLogin.CurrentUser.nid,0);
				Sleep(200);
#endif
			}
		}
		m_DeviceTree.DeleteItem(m_selectItem);

		OracleIO.DEVICE_DeleteCameraWithAreaID(DlgAddDevice.AreaList[ItemCount].nid);
		OracleIO.DEVICE_DeleteArea(DlgAddDevice.AreaList[ItemCount].nid);

		DlgMain->ShowCameraMessage(DlgAddDevice.AreaList[ItemCount].name,"删除区域成功",0);
		OnMenuitemUpdate();
	}
	else
		return ;
}

////LYNN///////////
void CDLGdevicetree::OnDblclkTreeDevice(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!DlgLogin.CurrentUser.preview)
	{
		MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}
#if OPEN_CS_MODE && !ALLTAB_CLIENT_MODE
	//CS模式的服务端 需要屏蔽手动按钮
	return ;
#endif
	// TODO: Add your control notification handler code here
	CPoint point;


	GetCursorPos(&point);
	CPoint PointInTree = point;
	m_DeviceTree.ScreenToClient( &PointInTree);
	UINT nFlag = TVHT_ONITEM;// 当在 m_DeviceTree 上右击时
	m_selectItem = m_DeviceTree.HitTest( PointInTree, &nFlag );

	if(m_selectItem != NULL)
	{
		if(m_DeviceTree.GetParentItem(m_selectItem) != NULL)
		{
			int ItemCount = 0;
			for(int i=0;i<MAX_AREA;i++)
			{
				if(iplist[i].item == m_selectItem)
				{
					break;
				}
				ItemCount++;
			}
			//判断设备是否存在 如果不存在 强制更新
			if(false==OracleIO.DEVICE_JudgeCamera(iplist[ItemCount].camID))
			{
				DlgMain->ShowCameraMessage(iplist[ItemCount].name.GetBuffer(0),"摄像头被其他用户删除，强制刷新设备树",0);
				OnMenuitemUpdate();
				return ;
			}

			
			int screenNo = DlgMain->DlgScreen.GetCurWindId();
			if( DlgMain->DlgScreen.GetCurWindPlayState(screenNo))
			{
				//自动选择 未在播放的 跳转
				for(int i=0;i<MAX_DEVICE_NUM;i++)
				{
					if(false==DlgMain->DlgScreen.GetCurWindPlayState(i))
					{
						screenNo=i;
						//跳转 选择框
						DlgMain->DlgScreen.m_screenPannel.SetActivePage(&(DlgMain->DlgScreen.m_screenPannel.m_wndVideo[i]), TRUE);
						DlgMain->DlgScreen.SetCurWindId(i);
						DlgMain->DlgNormal.UpdateNormalWnd();
						break;
					}
				}
			}

			DlgMain->DlgScreen.StartPlay(
				iplist[ItemCount].camID,
				iplist[ItemCount].area.GetBuffer(0),
				iplist[ItemCount].name.GetBuffer(0),
				iplist[ItemCount].ip.GetBuffer(0),
				iplist[ItemCount].port,
				iplist[ItemCount].channel,//20140718
				iplist[ItemCount].user.GetBuffer(0),
				iplist[ItemCount].psw.GetBuffer(0),
				screenNo,
				0,
				iplist[ItemCount].venderID,
				iplist[ItemCount].Rtspurl.GetBuffer(0),
				iplist[ItemCount].RTP,
				iplist[ItemCount].DecodeTag);

		}
	}

	(*pResult) = 0;
}

void CDLGdevicetree::OnMenuitemAddivmsdevice()
{

#if IVMS_ORACLE_DEVICETREE

	// TODO: Add your command handler code here
	if(!DlgLogin.CurrentUser.device)
	{
		MessageBox("无 设备管理 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	// TODO: Add your command handler code here
	HTREEITEM Item = m_DeviceTree.GetRootItem();
	bool flag=true;
	int count = 0;
	while(Item != NULL)
	{
		if(Item == m_selectItem)
		{
			flag=false;
			break;
		}
		Item = m_DeviceTree.GetNextItem(Item,TVGN_NEXT);
		count++;

	}
	if(flag)
	{
		MessageBox("请在添加设备 中 添加区域",MESSAGEBOX_TITLE);
		return ;
	}
	if(DlgAddIVMSDevice.DoModal() == IDOK)
	{
		
		TCHAR crossIndex[64]="";	//卡口编号
		long crossID=0;	//卡口ID
		OracleIO.DEVICE_AddNewCamera(DlgAddDevice.AreaList[count].name,\
			DlgAddIVMSDevice.CamData.name,  
			DlgAddIVMSDevice.CamData.ip,  
			DlgAddIVMSDevice.CamData.port,
			DlgAddIVMSDevice.CamData.channel,
			DlgAddIVMSDevice.CamData.user,
			DlgAddIVMSDevice.CamData.psw,
			DlgAddIVMSDevice.CamData.venderID,
			DlgAddIVMSDevice.Rtspurl,
			DlgAddIVMSDevice.CamData.RTP,
			DlgAddIVMSDevice.CamData.DecodeTag,
			DlgAddIVMSDevice.CamData.longitude,
			DlgAddIVMSDevice.CamData.latitude,
					crossIndex,
				crossID,
				DlgLogin.CurrentUser.nid,
				DlgLogin.CurrentUser.level);

		DlgMain->ShowCameraMessage(	DlgAddIVMSDevice.CamData.name,"添加设备成功",0);
		OnMenuitemUpdate();
	}
#else
	MessageBox("该版本 未接入平台",MESSAGEBOX_TITLE);

#endif
}

void CDLGdevicetree::OnMenuitemFlushtree()
{
	// TODO: Add your command handler code here
	OnMenuitemUpdate();
}
