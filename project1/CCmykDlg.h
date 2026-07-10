#pragma once
#include "resource.h"

class Cproject1View;

class CCmykDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCmykDlg)

public:
	CCmykDlg(CWnd* pParent = nullptr);
	virtual ~CCmykDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CMYK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	Cproject1View* m_pView;

	// Channel tracker state tracking variables
	int m_nC;
	int m_nM;
	int m_nY;
	int m_nK;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
