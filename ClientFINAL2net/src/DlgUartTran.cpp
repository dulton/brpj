// DlgUartTran.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "DlgUartTran.h"
#include "DlgSerialParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RECVMSG	WM_USER + 10


/////////////////////////////////////////////////////////////////////////////
// CDlgUartTran dialog
static int s_bserialstart = FALSE;
static LONG s_hSerial = -1;
void WINAPI s_fSerialDataCallBack(LONG hSerial,char *pRecvDataBuff,int BuffSize,void *context)
{
	//	int i;
	CDlgUartTran *pDlg = (CDlgUartTran*)context;
	TRACE("recv size : %d\n",BuffSize);
	if(BuffSize < 0)   //连接断开
	{
		pDlg->PostMessage(RECVMSG,1,0);
	}
	else
	{		
		if( !pDlg->m_bHexView )
		{
			pRecvDataBuff[BuffSize] = '\0';
			pDlg->m_recvstring += pRecvDataBuff;
		}
		else
		{
			CString strTemp;
			for(int i = 0;i<BuffSize;i++)
			{
				strTemp.Format("%02x ",(BYTE)pRecvDataBuff[i]);
				pDlg->m_recvstring += strTemp;
			}
		}
		pDlg->PostMessage(RECVMSG,0,0);
	}
}


CDlgUartTran::CDlgUartTran(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUartTran::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUartTran)
	m_port = 3000;
	m_password = _T("888888");
	m_recvstring = _T("");
	m_url = _T("10.142.50.249");
	m_username = _T("888888");
	//}}AFX_DATA_INIT
}


void CDlgUartTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUartTran)
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_PSW, m_password);
	DDX_Text(pDX, IDC_EDIT_RECVINFO, m_recvstring);
	DDX_Text(pDX, IDC_EDIT_URL, m_url);
	DDX_Text(pDX, IDC_EDIT_USER, m_username);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUartTran, CDialog)
	//{{AFX_MSG_MAP(CDlgUartTran)
	ON_BN_CLICKED(IDC_BTN_SERIAL, OnBtnSerial)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_CLEANREVEDIT, OnBtnCleanrevedit)
	ON_BN_CLICKED(IDC_CHECK_HEX_VIEW, OnCheckHexView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(RECVMSG,OnRecvMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUartTran message handlers

void CDlgUartTran::OnBtnSerial() 
{
	CString str;
	if(s_bserialstart)
	{
		if(s_hSerial != -1)
		{
			VSNET_ClientSerialStop(s_hSerial);
			s_hSerial = -1;
		}
		str.LoadString(IDS_STRING202);
		GetDlgItem(IDC_BTN_SERIAL)->SetWindowText(str);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SENDINFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RECVINFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CLEANREVEDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEX_VIEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_URL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PSW)->EnableWindow(TRUE);
		s_bserialstart = FALSE;
	}
	else
	{
		UpdateData();
		if(m_url.IsEmpty())
		{
			AfxMessageBox(IDS_STRING203);
			return;
		}
		if(m_username.IsEmpty())
		{
			AfxMessageBox(IDS_STRING204);
			return;
		}
		if(m_password.IsEmpty())
		{
			AfxMessageBox(IDS_STRING205);
			return;
		}
		CDlgSerialParam dlg;
		if(dlg.DoModal() != IDOK) return;
		
		VSSERIAL_INFO pVsSerial;
		memset(&pVsSerial,0,sizeof(VSSERIAL_INFO));
		pVsSerial.baudrate = atoi((char*)(LPCSTR)dlg.m_baud);
		if(dlg.m_very == 0)
			pVsSerial.checkbit = 0;
		else if(dlg.m_very == 1)
			pVsSerial.checkbit = 1;
		else if(dlg.m_very == 2)
			pVsSerial.checkbit = 2;
		if(dlg.m_data == 0)
			pVsSerial.databit  = 6;
		else if(dlg.m_data == 1)
			pVsSerial.databit  = 7;
		else if(dlg.m_data == 2)
			pVsSerial.databit  = 8;
		if(dlg.m_stop == 0)
			pVsSerial.stopbit  = 1;
		else if(dlg.m_stop == 1)
			pVsSerial.stopbit  = 2;
		pVsSerial.flowcontrol = 0;
		s_hSerial = VSNET_ClientSerialStart(NULL,(char*)(LPCSTR)m_url,
			(char*)(LPCSTR)m_username,(char*)(LPCSTR)m_password,
			dlg.m_SerialPort,//0:RS485,1:RS232
			&pVsSerial,
			s_fSerialDataCallBack,this,m_port);
		if(s_hSerial <= 0)
		{
			s_hSerial = -1;
			AfxMessageBox(IDS_STRING206);
			return;
		}
		s_bserialstart = TRUE;
		str.LoadString(IDS_STRING207);
		GetDlgItem(IDC_BTN_SERIAL)->SetWindowText(str);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SENDINFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_HEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RECVINFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CLEANREVEDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_HEX_VIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_URL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PSW)->EnableWindow(FALSE);
	}
}

void CDlgUartTran::OnBtnSend() 
{
	CString strsend;
	if(s_hSerial == -1) return;
	GetDlgItem(IDC_EDIT_SENDINFO)->GetWindowText(strsend);
	if(strsend.IsEmpty()) return;
	
	if( !((CButton*)GetDlgItem(IDC_CHECK_HEX))->GetCheck() )
	{
		//发送字符
		if(!VSNET_ClientSerialSendNew(s_hSerial,
			(char*)(LPCSTR)strsend,strsend.GetLength()) )
			AfxMessageBox(IDS_STRING208);
	}
	else
	{
		//16进制发送
		BYTE	sendBuffer[100];
		int		pos = -1,index = 0;
		do
		{
			pos  = strsend.Find(" ");
			if( pos >= 0 )
			{
				sscanf( strsend.Left(pos), "%x", sendBuffer+index);
				strsend = strsend.Right(strsend.GetLength() - pos - 1);
			}
			else
			{
				sscanf( strsend,"%x", sendBuffer+index);
				break;
			}
			index++;
		}while(index<100);
		
#ifdef _DEBUG
		CString		strRecv;
		for(int k = 0; k<index+1; k++)
		{
			strRecv.Format("0x%02x ",(unsigned char)sendBuffer[k]);
			TRACE(strRecv);
		}
		
#endif
		if(!VSNET_ClientSerialSendNew(s_hSerial,
			(char*)sendBuffer,index+1) )
			AfxMessageBox(IDS_STRING208);
	}
}

void CDlgUartTran::OnBtnCleanrevedit() 
{
	m_recvstring = "";
	UpdateData(FALSE);
}

void CDlgUartTran::OnCheckHexView() 
{
	m_bHexView = ((CButton*)GetDlgItem(IDC_CHECK_HEX_VIEW))->GetCheck();
}

BOOL CDlgUartTran::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CButton*)GetDlgItem(IDC_CHECK_HEX_VIEW))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_HEX))->SetCheck(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDlgUartTran::OnRecvMsg(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 1)
	{
		AfxMessageBox(IDS_STRING209);
		OnBtnSerial();
	}
	else if(wParam == 0)
	{
		UpdateData(FALSE);
	}
	return 0;
}

void CDlgUartTran::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(s_bserialstart)
	{
		OnBtnSerial();
	}	
}
