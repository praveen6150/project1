#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CSepiaToneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSepiaToneDlg)

public:
	CSepiaToneDlg(CWnd* pParent = nullptr);
	virtual ~CSepiaToneDlg();

	enum { IDD = IDD_SEPIA_TONE };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlIntensityValue;

	void UpdateLabelAndPreview();

	DECLARE_MESSAGE_MAP()
};
