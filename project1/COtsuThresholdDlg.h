#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class COtsuThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COtsuThresholdDlg)

public:
	COtsuThresholdDlg(CWnd* pParent = nullptr);
	virtual ~COtsuThresholdDlg();

	enum { IDD = IDD_OTSU_THRESHOLD };

	Cproject1View* m_pView;   // link back to the view, same pattern as your Pseudo-Color dlg
	int m_computedThreshold;  // store the value Otsu calculated, so OK/Cancel can use it

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	void OnBnClickedCheckOtsuPreview();

	CStatic m_ctrlThresholdValue;

	DECLARE_MESSAGE_MAP()
};
