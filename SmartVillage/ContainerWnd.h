#pragma once


// CContainerWnd

class CContainerWnd : public CWnd
{
	DECLARE_DYNAMIC(CContainerWnd)

public:
	CContainerWnd();
	virtual ~CContainerWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


