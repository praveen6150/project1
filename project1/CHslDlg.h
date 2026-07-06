#pragma once
#include "afxdialogex.h"

// Forward declaration of your View class so the dialog can talk to it
class Cproject1View;

class CHslDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHslDlg)

public:
	CHslDlg(CWnd* pParent = nullptr);   // Standard constructor
	virtual ~CHslDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HSL }; // Make sure this matches your RC dialog ID
#endif

public:
	Cproject1View* m_pView; // Pointer to pass changes back to your view

	// HSL state variables
	int m_nHue;
	int m_nSaturation;
	int m_nLightness;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};