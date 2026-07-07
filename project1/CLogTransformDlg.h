#pragma once
#include "afxdialogex.h"


class Cproject1View;

class CLogTransformDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogTransformDlg)

public:
	CLogTransformDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLogTransformDlg();
	Cproject1View* m_pView;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_TRANSFORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnInitDialog();

	void OnBnClickedCheckAutoCalc();

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_ctrlSliderC;
	CStatic m_ctrlStaticCLabel;
	BOOL m_bAutoCalc;

	double GetConstantC() { return m_bAutoCalc ? 45.9859 : (m_ctrlSliderC.GetPos() / 10.0); }
};
