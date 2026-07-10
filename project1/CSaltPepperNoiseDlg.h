#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CSaltPepperNoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaltPepperNoiseDlg)

public:
	CSaltPepperNoiseDlg(CWnd* pParent = nullptr);
	virtual ~CSaltPepperNoiseDlg();

	enum { IDD = IDD_SALT_PEPPER_NOISE };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlDensityValue;

	void UpdateLabelAndPreview();

	DECLARE_MESSAGE_MAP()
};