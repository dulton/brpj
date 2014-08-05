// DLGAddIVMSdevice.cpp : implementation file
//

#include "stdafx.h"
#include "YiRongCarDetectAIO.h"
#include "DLGAddIVMSdevice.h"


// CDLGAddIVMSdevice dialog

IMPLEMENT_DYNAMIC(CDLGAddIVMSdevice, CDialog)

CDLGAddIVMSdevice::CDLGAddIVMSdevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGAddIVMSdevice::IDD, pParent)
{

}

CDLGAddIVMSdevice::~CDLGAddIVMSdevice()
{
}

void CDLGAddIVMSdevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLGAddIVMSdevice, CDialog)
	ON_BN_CLICKED(IDOK, &CDLGAddIVMSdevice::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDLGAddIVMSdevice::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDLGAddIVMSdevice message handlers

void CDLGAddIVMSdevice::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CDLGAddIVMSdevice::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
