// DlgChannelCfg.cpp : implementation file
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "DlgChannelCfg.h"
#include "DlgMotionArea.h"
#include "DlgCfgViparam.h"
#include "DlgCfgTimelist.h"
#include "DlgCfgVMask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelCfg dialog
CDlgChannelCfg::CDlgChannelCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChannelCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChannelCfg)
	m_chname = _T("");
	m_bitstype = -1;
	m_bitsosd = FALSE;
	m_delay = 0;
	m_framerate = 0;
	m_ikeyinterval = 0;
	m_imageformat = -1;
	m_maxqueue = 0;
	m_maxbits = 0;
	m_minqueue = 0;
	m_motion = FALSE;
	m_record = FALSE;
	m_samplerate = -1;
	m_streamtype = -1;
	m_timeosd = -1;
	m_titalosd = _T("");
	m_videomask = FALSE;
	m_audiobits = -1;
	m_bitosd_x = 0;
	m_bitosd_y = 0;
	m_timeosd_x = 0;
	m_timeosd_y = 0;
	m_titalosd_x = 0;
	m_titalosd_y = 0;
	m_titalosd1 = _T("");
	m_titalosd2 = _T("");
	m_titalosd3 = _T("");
	m_titalosd4 = _T("");
	m_titalosd1_x = 0;
	m_titalosd2_x = 0;
	m_titalosd3_x = 0;
	m_titalosd4_x = 0;
	m_titalosd1_y = 0;
	m_titalosd2_y = 0;
	m_titalosd3_y = 0;
	m_titalosd4_y = 0;
	//}}AFX_DATA_INIT
}


void CDlgChannelCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChannelCfg)
	DDX_Text(pDX, IDC_EDIT_CHNAME, m_chname);
	DDV_MaxChars(pDX, m_chname, 16);
	DDX_CBIndex(pDX, IDC_BITRATETYPE, m_bitstype);
	DDX_Check(pDX, IDC_CHECK_BITSOSD, m_bitsosd);
	DDX_Text(pDX, IDC_DELAY, m_delay);
	DDV_MinMaxInt(pDX, m_delay, 5, 1800);
	DDX_Text(pDX, IDC_FRAMERATE, m_framerate);
	DDV_MinMaxInt(pDX, m_framerate, 1, 30);
	DDX_Text(pDX, IDC_IKEYINTERVAL, m_ikeyinterval);
	DDV_MinMaxInt(pDX, m_ikeyinterval, 10, 200);
	DDX_CBIndex(pDX, IDC_IMAGEFORMAT, m_imageformat);
	DDX_Text(pDX, IDC_MAXQUEUE, m_maxqueue);
	DDV_MinMaxInt(pDX, m_maxqueue, 8, 31);
	DDX_Text(pDX, IDC_MAXRATE, m_maxbits);
	DDV_MinMaxInt(pDX, m_maxbits, 32, 4000);
	DDX_Text(pDX, IDC_MINQUEUE, m_minqueue);
	DDV_MinMaxInt(pDX, m_minqueue, 2, 31);
	DDX_Check(pDX, IDC_MOTION, m_motion);
	DDX_Check(pDX, IDC_RECORD, m_record);
	DDX_CBIndex(pDX, IDC_SAMPLERATE, m_samplerate);
	DDX_CBIndex(pDX, IDC_STREAMTYPE, m_streamtype);
	DDX_CBIndex(pDX, IDC_TIMEOSD, m_timeosd);
	DDV_MaxChars(pDX, m_titalosd, 32);
	DDX_Check(pDX, IDC_VIDEOMASK, m_videomask);
	DDX_CBIndex(pDX, IDC_AUBITS, m_audiobits);
	DDX_Text(pDX, IDC_BITOSD_X, m_bitosd_x);
	DDV_MinMaxInt(pDX, m_bitosd_x, 0, 704);
	DDX_Text(pDX, IDC_BITOSD_Y, m_bitosd_y);
	DDV_MinMaxInt(pDX, m_bitosd_y, 0, 576);
	DDX_Text(pDX, IDC_TIMEOSD_X, m_timeosd_x);
	DDV_MinMaxInt(pDX, m_timeosd_x, 0, 704);
	DDX_Text(pDX, IDC_TIMEOSD_Y, m_timeosd_y);
	DDV_MinMaxInt(pDX, m_timeosd_y, 0, 576);
	DDX_Text(pDX, IDC_TITALOSD_X, m_titalosd_x);
	DDV_MinMaxInt(pDX, m_titalosd_x, 0, 704);
	DDX_Text(pDX, IDC_TITALOSD_Y, m_titalosd_y);
	DDV_MinMaxInt(pDX, m_titalosd_y, 0, 576);
	DDX_Text(pDX, IDC_TITALOSD1, m_titalosd1);
	DDV_MaxChars(pDX, m_titalosd1, 24);
	DDX_Text(pDX, IDC_TITALOSD2, m_titalosd2);
	DDV_MaxChars(pDX, m_titalosd2, 24);
	DDX_Text(pDX, IDC_TITALOSD3, m_titalosd3);
	DDV_MaxChars(pDX, m_titalosd3, 24);
	DDX_Text(pDX, IDC_TITALOSD4, m_titalosd4);
	DDV_MaxChars(pDX, m_titalosd4, 24);
	DDX_Text(pDX, IDC_TITALOSD1_X, m_titalosd1_x);
	DDV_MinMaxInt(pDX, m_titalosd1_x, 0, 704);
	DDX_Text(pDX, IDC_TITALOSD2_X, m_titalosd2_x);
	DDV_MinMaxInt(pDX, m_titalosd2_x, 0, 704);
	DDX_Text(pDX, IDC_TITALOSD3_X, m_titalosd3_x);
	DDV_MinMaxInt(pDX, m_titalosd3_x, 0, 704);
	DDX_Text(pDX, IDC_TITALOSD4_X, m_titalosd4_x);
	DDV_MinMaxInt(pDX, m_titalosd4_x, 0, 704);
	DDX_Text(pDX, IDC_TITALOSD1_Y, m_titalosd1_y);
	DDV_MinMaxInt(pDX, m_titalosd1_y, 0, 576);
	DDX_Text(pDX, IDC_TITALOSD2_Y, m_titalosd2_y);
	DDV_MinMaxInt(pDX, m_titalosd2_y, 0, 576);
	DDX_Text(pDX, IDC_TITALOSD3_Y, m_titalosd3_y);
	DDV_MinMaxInt(pDX, m_titalosd3_y, 0, 576);
	DDX_Text(pDX, IDC_TITALOSD4_Y, m_titalosd4_y);
	DDV_MinMaxInt(pDX, m_titalosd4_y, 0, 576);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChannelCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgChannelCfg)
	ON_BN_CLICKED(IDC_BTN_MOTIONAREA, OnBtnMotionarea)
	ON_BN_CLICKED(IDC_BTN_RECORD, OnBtnRecord)
	ON_BN_CLICKED(IDC_BTN_VIDEOMASK, OnBtnVideomask)
	ON_BN_CLICKED(IDC_BTN_MOTIONTIME, OnBtnMotiontime)
	ON_BN_CLICKED(IDC_RECORD, OnRecord)
	ON_BN_CLICKED(IDC_VIDEOMASK, OnVideomask)
	ON_BN_CLICKED(IDC_MOTION, OnMotion)
	ON_BN_CLICKED(IDC_BTN_VIPARAM, OnBtnViparam)
	ON_CBN_SELCHANGE(IDC_BITRATETYPE, OnSelchangeBitratetype)
	ON_CBN_SELCHANGE(IDC_COMBO_CHSEL, OnSelchangeComboChsel)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelCfg message handlers

BOOL CDlgChannelCfg::OnInitDialog() 
{
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);
	CDialog::OnInitDialog();
	
	int i;
	CString str;
	for(i=0;i<m_chnums;i++)
	{
		str.Format(IDS_STRING178,i+1);
		((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->AddString(str);

		memset(&s_viparam[i],0,sizeof(CHANNVIPARAM));
		VSNET_ClientGetViparam(m_servername,m_url, (char)i,m_username, m_password, &s_viparam[i],m_port);
		
		memset(&s_channmotion[i],0,sizeof(CHANNMOTION));
		VSNET_ClientGetMotionPara(m_servername,m_url, (char)i, m_username, m_password, &s_channmotion[i],m_port);
		
		memset(&s_channosd[i],0,sizeof(CHANNOSD));
		VSNET_ClientGetChanOSD(m_servername,m_url, (char)i, m_username, m_password, &s_channosd[i], m_port);
		
		memset(&s_channvmask[i],0,sizeof(CHANNVIDEOMASK));
		VSNET_ClientGetVideoMask(m_servername,m_url, (char)i, m_username, m_password, &s_channvmask[i],m_port);
		
		memset(&s_chanpara[i],0,sizeof(CHANNELPARAM));
		VSNET_ClientGetChannelPara(m_servername,m_url, (char)i, m_username, m_password, &s_chanpara[i],m_port);
	
		s_chvideolost[i] = FALSE;
		VSNET_ClientGetVideoLost(m_servername,m_url,(char)i,m_username,m_password,&s_chvideolost[i],m_port);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->SetCurSel(0);
	m_curch = 0;
	OnSelchangeComboChsel();
	GetDlgItem(IDC_BTN_SET)->EnableWindow(TRUE);
	if(m_chnums > 1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgChannelCfg::OnOK() 
{
	/*UpdateData(TRUE);

	int curch  = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	sprintf(s_chanpara[curch].m_channelName,"%s",m_chname);
	s_chanpara[curch].m_bitratetype = m_bitstype;
	s_chanpara[curch].m_delay       = m_delay;
	s_chanpara[curch].m_videoFrameRate = m_framerate;
	s_chanpara[curch].m_Iinterval  = m_ikeyinterval;
	s_chanpara[curch].m_encodeType = m_imageformat + 1;
	s_chanpara[curch].m_maxqueue   = m_maxqueue;
	s_chanpara[curch].m_minqueue   = m_minqueue;
	s_chanpara[curch].m_maxbitrates = m_maxbits;
	s_chanpara[curch].m_streamType  = m_streamtype;
	s_chanpara[curch].m_audiosample = m_samplerate == 0 ? 16000 : 32000;
	s_channosd[curch].m_time.nFormat = m_timeosd;
	s_channosd[curch].m_bits.bShow  = m_bitsosd;
	sprintf(s_channosd[curch].m_tital[0].lpString,"%s",m_titalosd);
	if(m_audiobits == 0)      s_chanpara[curch].m_audiobitrate = 8;
	else if(m_audiobits == 1) s_chanpara[curch].m_audiobitrate = 16;
	else if(m_audiobits == 2) s_chanpara[curch].m_audiobitrate = 24;
	else if(m_audiobits == 3) s_chanpara[curch].m_audiobitrate = 32;
	else if(m_audiobits == 4) s_chanpara[curch].m_audiobitrate = 48;
	else if(m_audiobits == 5) s_chanpara[curch].m_audiobitrate = 64;
	else					  s_chanpara[curch].m_audiobitrate = 24;

	s_channosd[curch].m_bits.x = m_bitosd_x;
	s_channosd[curch].m_bits.y = m_bitosd_y;
	s_channosd[curch].m_time.x = m_timeosd_x;
	s_channosd[curch].m_time.y = m_timeosd_y;
	s_channosd[curch].m_tital[0].x = m_titalosd_x;
	s_channosd[curch].m_tital[0].y = m_titalosd_y;

	//VSNET_ClientGetMotionPara(m_servername,m_url,(char)i,m_username,m_password,&s_channmotion[i],m_port);
	VSNET_ClientSetChanOSD(m_servername,m_url,(char)curch,m_username,m_password,&s_channosd[curch],m_port);
	//VSNET_ClientGetVideoMask(m_servername,m_url,(char)i,m_username,m_password,&s_channvmask[i],m_port);
	VSNET_ClientSetChannelPara(m_servername,m_url,(char)curch,m_username,m_password,&s_chanpara[curch],m_port);
	*/
}

void CDlgChannelCfg::OnBtnMotionarea() 
{
	CDlgMotionArea dlg;
	strcpy(dlg.m_servername,m_servername);
	dlg.m_ch = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	strcpy(dlg.m_url,m_url);
	strcpy(dlg.m_username,m_username);
	strcpy(dlg.m_password,m_password);
	dlg.m_port = m_port;
	dlg.m_ChanMotion = &s_channmotion[dlg.m_ch];
	dlg.DoModal();
}

void CDlgChannelCfg::OnBtnRecord() 
{
	CDlgCfgTimelist dlg;
	dlg.m_timelist   = &s_chanpara[m_curch].m_record;
	dlg.m_bIsReclist = TRUE;
	if(dlg.DoModal() != IDOK) return;
}

void CDlgChannelCfg::OnBtnVideomask() 
{
	CDlgCfgVMask dlg;
	s_channvmask[m_curch].m_bmask = TRUE;
	
	strcpy(dlg.m_servername,m_servername);
	dlg.m_ch = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	strcpy(dlg.m_url,m_url);
	strcpy(dlg.m_username,m_username);
	strcpy(dlg.m_password,m_password);
	dlg.m_port = m_port;
	dlg.pvmask = &s_channvmask[m_curch];
	dlg.DoModal();
}

void CDlgChannelCfg::OnBtnMotiontime() 
{
	int i;
	CDlgCfgTimelist dlg;
	dlg.m_timelist   = &s_channmotion[m_curch].m_timelist;
	dlg.m_bIsReclist = FALSE;
	dlg.m_brec = s_channmotion[m_curch].m_brecord;
	for(i=0;i<ALARM_OUT_NUM;i++)
		dlg.m_output[i] = s_channmotion[m_curch].m_outputmap[i];
	if(dlg.DoModal() != IDOK) return;

	s_channmotion[m_curch].m_brecord = dlg.m_brec;
	for(i=0;i<ALARM_OUT_NUM;i++)
		s_channmotion[m_curch].m_outputmap[i] = dlg.m_output[i];
}

void CDlgChannelCfg::OnRecord() 
{
	GetDlgItem(IDC_BTN_RECORD)->EnableWindow(((CButton*)GetDlgItem(IDC_RECORD))->GetCheck());
}

void CDlgChannelCfg::OnVideomask() 
{
	GetDlgItem(IDC_BTN_VIDEOMASK)->EnableWindow(((CButton*)GetDlgItem(IDC_VIDEOMASK))->GetCheck());
}

void CDlgChannelCfg::OnMotion() 
{
	GetDlgItem(IDC_BTN_MOTIONAREA)->EnableWindow(((CButton*)GetDlgItem(IDC_MOTION))->GetCheck());
	GetDlgItem(IDC_BTN_MOTIONTIME)->EnableWindow(((CButton*)GetDlgItem(IDC_MOTION))->GetCheck());
}

void CDlgChannelCfg::OnBtnViparam() 
{
	CDlgCfgViparam dlg;
	dlg.m_pviparam = &s_viparam[m_curch];
	strcpy(dlg.m_servername,m_servername);
	dlg.m_ch = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	strcpy(dlg.m_url,m_url);
	strcpy(dlg.m_username,m_username);
	strcpy(dlg.m_password,m_password);
	dlg.m_port = m_port;
	dlg.DoModal();	
}

void CDlgChannelCfg::OnSelchangeBitratetype() 
{
	if(((CComboBox*)GetDlgItem(IDC_BITRATETYPE))->GetCurSel() == 0)//¶¨ÂëÂÊ
	{
		GetDlgItem(IDC_MAXQUEUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_MAXRATE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_MAXQUEUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAXRATE)->EnableWindow(FALSE);
	}
}

void CDlgChannelCfg::OnSelchangeComboChsel() 
{
	m_curch  = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	m_chname   = s_chanpara[m_curch].m_channelName;
	m_bitstype = s_chanpara[m_curch].m_bitratetype;
	m_delay    = s_chanpara[m_curch].m_delay;
	m_framerate = s_chanpara[m_curch].m_videoFrameRate;
	m_ikeyinterval = s_chanpara[m_curch].m_Iinterval;
	m_imageformat = s_chanpara[m_curch].m_encodeType - 1;
	m_maxqueue = s_chanpara[m_curch].m_maxqueue;
	m_minqueue = s_chanpara[m_curch].m_minqueue;
	m_maxbits = s_chanpara[m_curch].m_maxbitrates;
	m_streamtype = s_chanpara[m_curch].m_streamType;
	m_samplerate = s_chanpara[m_curch].m_audiosample == 16000 ? 0 : 1;
	m_timeosd    = s_channosd[m_curch].m_time.nFormat;
	m_titalosd1   = s_channosd[m_curch].m_tital[0].lpString;
	m_titalosd2   = s_channosd[m_curch].m_tital[1].lpString;
	m_titalosd3   = s_channosd[m_curch].m_tital[2].lpString;
	m_titalosd4   = s_channosd[m_curch].m_tital[3].lpString;
	m_bitsosd  = s_channosd[m_curch].m_bits.bShow;
	m_bitosd_x = s_channosd[m_curch].m_bits.x;
	m_bitosd_y = s_channosd[m_curch].m_bits.y;
	m_timeosd_x = s_channosd[m_curch].m_time.x;
	m_timeosd_y = s_channosd[m_curch].m_time.y;
	m_titalosd1_x = s_channosd[m_curch].m_tital[0].x;
	m_titalosd1_y = s_channosd[m_curch].m_tital[0].y;
	m_titalosd2_x = s_channosd[m_curch].m_tital[1].x;
	m_titalosd2_y = s_channosd[m_curch].m_tital[1].y;
	m_titalosd3_x = s_channosd[m_curch].m_tital[2].x;
	m_titalosd3_y = s_channosd[m_curch].m_tital[2].y;
	m_titalosd4_x = s_channosd[m_curch].m_tital[3].x;
	m_titalosd4_y = s_channosd[m_curch].m_tital[3].y;
	if(s_chanpara[m_curch].m_audiobitrate == 8)
		m_audiobits  = 0;
	else if(s_chanpara[m_curch].m_audiobitrate == 16)
		m_audiobits  = 1;
	else if(s_chanpara[m_curch].m_audiobitrate == 24)
		m_audiobits  = 2;
	else if(s_chanpara[m_curch].m_audiobitrate == 32)
		m_audiobits  = 3;
	else if(s_chanpara[m_curch].m_audiobitrate == 48)
		m_audiobits  = 4;
	else if(s_chanpara[m_curch].m_audiobitrate == 64)
		m_audiobits  = 5;
	else
		m_audiobits  = 2;
		
	((CButton*)GetDlgItem(IDC_CHECK_VIDEOLOST))->SetCheck(s_chvideolost[m_curch]);
	
	m_motion = s_channmotion[m_curch].m_beanb;
	m_videomask = s_channvmask[m_curch].m_bmask == 0 ? FALSE : TRUE;
	m_record = s_chanpara[m_curch].m_benrecord;
	UpdateData(FALSE);
	OnSelchangeBitratetype();
	OnRecord();
	OnVideomask();
	OnMotion();
}

void CDlgChannelCfg::OnBtnSet() 
{	
	UpdateData(TRUE);

	int curch  = ((CComboBox*)GetDlgItem(IDC_COMBO_CHSEL))->GetCurSel();
	sprintf(s_chanpara[m_curch].m_channelName,"%s",m_chname);
	s_chanpara[m_curch].m_bitratetype = m_bitstype;
	s_chanpara[m_curch].m_delay       = m_delay;
	s_chanpara[m_curch].m_videoFrameRate = m_framerate;
	s_chanpara[m_curch].m_Iinterval  = m_ikeyinterval;
	s_chanpara[m_curch].m_encodeType = m_imageformat + 1;
	s_chanpara[m_curch].m_maxqueue   = m_maxqueue;
	s_chanpara[m_curch].m_minqueue   = m_minqueue;
	s_chanpara[m_curch].m_maxbitrates = m_maxbits;
	s_chanpara[m_curch].m_streamType  = m_streamtype;
	s_chanpara[m_curch].m_audiosample = m_samplerate == 0 ? 16000 : 32000;
	s_channosd[m_curch].m_time.nFormat = m_timeosd;
	s_channosd[m_curch].m_bits.bShow  = m_bitsosd;
	sprintf(s_channosd[m_curch].m_tital[0].lpString,"%s",m_titalosd1);
	sprintf(s_channosd[m_curch].m_tital[1].lpString,"%s",m_titalosd2);
	sprintf(s_channosd[m_curch].m_tital[2].lpString,"%s",m_titalosd3);
	sprintf(s_channosd[m_curch].m_tital[3].lpString,"%s",m_titalosd4);
	if(m_audiobits == 0)      s_chanpara[m_curch].m_audiobitrate = 8;
	else if(m_audiobits == 1) s_chanpara[m_curch].m_audiobitrate = 16;
	else if(m_audiobits == 2) s_chanpara[m_curch].m_audiobitrate = 24;
	else if(m_audiobits == 3) s_chanpara[m_curch].m_audiobitrate = 32;
	else if(m_audiobits == 4) s_chanpara[m_curch].m_audiobitrate = 48;
	else if(m_audiobits == 5) s_chanpara[m_curch].m_audiobitrate = 64;
	else					  s_chanpara[m_curch].m_audiobitrate = 24;

	s_channosd[m_curch].m_bits.x = m_bitosd_x;
	s_channosd[m_curch].m_bits.y = m_bitosd_y;
	s_channosd[m_curch].m_time.x = m_timeosd_x;
	s_channosd[m_curch].m_time.y = m_timeosd_y;
	s_channosd[m_curch].m_tital[0].x = m_titalosd1_x;
	s_channosd[m_curch].m_tital[0].y = m_titalosd1_y;
	s_channosd[m_curch].m_tital[1].x = m_titalosd2_x;
	s_channosd[m_curch].m_tital[1].y = m_titalosd2_y;
	s_channosd[m_curch].m_tital[2].x = m_titalosd3_x;
	s_channosd[m_curch].m_tital[2].y = m_titalosd3_y;
	s_channosd[m_curch].m_tital[3].x = m_titalosd4_x;
	s_channosd[m_curch].m_tital[3].y = m_titalosd4_y;

	s_chanpara[m_curch].m_benrecord = m_record;
	s_channvmask[m_curch].m_bmask   = m_videomask;
	s_channmotion[m_curch].m_beanb  = m_motion;

	s_chvideolost[m_curch] = ((CButton*)GetDlgItem(IDC_CHECK_VIDEOLOST))->GetCheck() ? TRUE : FALSE;

	VSNET_ClientSetMotionPara(m_servername,m_url,m_curch,m_username,m_password,&s_channmotion[m_curch],m_port);
	VSNET_ClientSetChanOSD(m_servername,m_url,(char)m_curch,m_username,m_password,&s_channosd[m_curch],m_port);
	VSNET_ClientSetChannelPara(m_servername,m_url,(char)m_curch,m_username,m_password,&s_chanpara[m_curch],m_port);
	VSNET_ClientSetVideoMask(m_servername,m_url,(char)m_curch,m_username,m_password,&s_channvmask[m_curch],m_port);
	VSNET_ClientSetVideoLost(m_servername,m_url,(char)m_curch,m_username,m_password,s_chvideolost[m_curch],m_port);
}
