#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CBleachBypassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBleachBypassDlg)

public:
	CBleachBypassDlg(CWnd* pParent = nullptr);
	virtual ~CBleachBypassDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BLEACHBYPASS };
#endif

	void SetTargetView(Cproject1View* pView);

	int m_intensity;   // 0 - 100

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderIntensity;
	Cproject1View* m_pTargetView = nullptr;

	void UpdateLabel();

	DECLARE_MESSAGE_MAP()
};