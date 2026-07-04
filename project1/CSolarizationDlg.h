#pragma once
#include "afxdialogex.h"


// CSolarizationDlg dialog

class CSolarizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSolarizationDlg)

public:
	CSolarizationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSolarizationDlg();
	int m_nIntensity;
	CSliderCtrl m_sliderIntensity;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOLARIZATION_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
