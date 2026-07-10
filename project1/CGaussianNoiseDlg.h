#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CGaussianNoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGaussianNoiseDlg)

public:
	CGaussianNoiseDlg(CWnd* pParent = nullptr);
	virtual ~CGaussianNoiseDlg();

	enum { IDD = IDD_GAUSSIAN_NOISE };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlStdDevValue;

	void UpdateLabelAndPreview();

	DECLARE_MESSAGE_MAP()
};