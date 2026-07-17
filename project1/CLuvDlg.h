#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CLuvDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLuvDlg)

public:
	CLuvDlg(CWnd* pParent = nullptr);
	virtual ~CLuvDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LUV };
#endif

	void SetTargetView(Cproject1View* pView);

	int m_deltaL;
	int m_deltaU;
	int m_deltaV;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderL;
	CSliderCtrl m_sliderU;
	CSliderCtrl m_sliderV;

	CStatic     m_lblL;
	CStatic     m_lblU;
	CStatic     m_lblV;

	Cproject1View* m_pTargetView = nullptr;

	DECLARE_MESSAGE_MAP()
};

