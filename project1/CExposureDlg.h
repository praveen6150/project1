#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CExposureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExposureDlg)

public:
	CExposureDlg(CWnd* pParent = nullptr);
	virtual ~CExposureDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXPOSURE };
#endif

	void SetTargetView(Cproject1View* pView);

	int m_exposureStops100;   // -200 to 200, representing -2.00 to +2.00 stops

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderExposure;
	Cproject1View* m_pTargetView = nullptr;

	void UpdateLabel();

	DECLARE_MESSAGE_MAP()
};
