// DLGdetectDevice.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectFPQ.h"
#include "DLGdetectDevice.h"


// CDLGdetectDevice dialog

IMPLEMENT_DYNAMIC(CDLGdetectDevice, CDialog)

CDLGdetectDevice::CDLGdetectDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGdetectDevice::IDD, pParent)
{

}

CDLGdetectDevice::~CDLGdetectDevice()
{
}

void CDLGdetectDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLGdetectDevice, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGdetectDevice::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGdetectDevice::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDLGdetectDevice message handlers

void CDLGdetectDevice::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CDLGdetectDevice::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CDLGdetectDevice::AutoSize()
{
	CRect rc(0, 0, 0, 0);
	GetClientRect(&rc);
	//需要变化在这添加


	Invalidate();
}