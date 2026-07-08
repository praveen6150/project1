#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CBinaryThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBinaryThresholdDlg)

public:
	CBinaryThresholdDlg(CWnd* pParent = nullptr);
	virtual ~CBinaryThresholdDlg();

	enum { IDD = IDD_BINARY_THRESHOLD };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlThresholdValue;

	int GetSliderValue();

	DECLARE_MESSAGE_MAP()
};