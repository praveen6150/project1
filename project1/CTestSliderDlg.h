#pragma once
#include "afxdialogex.h"

class CTestSliderDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CTestSliderDlg)

public:
    CTestSliderDlg(CWnd* pParent = nullptr);
    virtual ~CTestSliderDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_TEST_SLIDER };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    // This is the essential lifecycle function that fires right before the window appears
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
};