#pragma once
#include "afxdialogex.h"

// Forward declaration of the view class
class Cproject1View;

class CGammaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGammaDlg)

public:
	CGammaDlg(CWnd* pParent = nullptr);
	virtual ~CGammaDlg();

	// Tracking pointers and states
	Cproject1View* m_pView; // <-- ADD THIS TO LINK BACK TO THE CANVAS VIEW
	CSliderCtrl m_sliderGamma;
	double m_dGamma;

	CImage m_originalImageBackup;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GAMMA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};