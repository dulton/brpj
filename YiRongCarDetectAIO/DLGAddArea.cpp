// DLGAddArea.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "DLGAddArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGAddArea dialog


CDLGAddArea::CDLGAddArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGAddArea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGAddArea)
	m_Area = _T("");
	//}}AFX_DATA_INIT
}


void CDLGAddArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGAddArea)
	DDX_Text(pDX, IDC_EDIT_AREA, m_Area);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGAddArea, CDialog)
	//{{AFX_MSG_MAP(CDLGAddArea)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGAddArea message handlers
