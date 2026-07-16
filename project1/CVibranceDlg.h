#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CVibranceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVibranceDlg)

public:
	CVibranceDlg(CWnd* pParent = nullptr);
	virtual ~CVibranceDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VIBRANCE };
#endif

	void SetTargetView(Cproject1View* pView);

	int m_amount;   // -100 to 100

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderAmount;
	Cproject1View* m_pTargetView = nullptr;

	DECLARE_MESSAGE_MAP()
};