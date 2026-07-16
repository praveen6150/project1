#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CWhiteBalanceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWhiteBalanceDlg)

public:
	CWhiteBalanceDlg(CWnd* pParent = nullptr);
	virtual ~CWhiteBalanceDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WHITEBALANCE };
#endif

	void SetTargetView(Cproject1View* pView);

	int m_temperature;   // -100 to 100
	int m_tint;           // -100 to 100

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderTemperature;
	CSliderCtrl m_sliderTint;

	Cproject1View* m_pTargetView = nullptr;

	DECLARE_MESSAGE_MAP()
};

