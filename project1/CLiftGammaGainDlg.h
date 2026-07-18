#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CLiftGammaGainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLiftGammaGainDlg)

public:
	CLiftGammaGainDlg(CWnd* pParent = nullptr);
	virtual ~CLiftGammaGainDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LIFTGAMMAGAIN };
#endif

	void SetTargetView(Cproject1View* pView);

	// All stored as scaled integers (matching slider ranges), converted to real values when applied
	int m_liftR, m_liftG, m_liftB;     // -50 to 50   -> -0.50 to 0.50
	int m_gammaR, m_gammaG, m_gammaB;  // 10 to 300   -> 0.10 to 3.00
	int m_gainR, m_gainG, m_gainB;     // 0 to 200    -> 0.00 to 2.00

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderLiftR, m_sliderLiftG, m_sliderLiftB;
	CSliderCtrl m_sliderGammaR, m_sliderGammaG, m_sliderGammaB;
	CSliderCtrl m_sliderGainR, m_sliderGainG, m_sliderGainB;

	Cproject1View* m_pTargetView = nullptr;

	void UpdateLive();
	void UpdateAllLabels();
	void UpdateLiftLabel(int channel);   // 0=R, 1=G, 2=B
	void UpdateGammaLabel(int channel);
	void UpdateGainLabel(int channel);

	DECLARE_MESSAGE_MAP()
};
