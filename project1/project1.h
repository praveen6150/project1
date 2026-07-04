// project1.h : main header file for the project1 application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <gdiplus.h>         // <-- ADD THIS
#pragma comment(lib, "gdiplus.lib")   // <-- ADD THIS


// Cproject1App:
// See project1.cpp for the implementation of this class
//

class Cproject1App : public CWinApp
{
public:
	Cproject1App() noexcept;

	ULONG_PTR m_gdiplusToken;   // <-- ADD THIS

	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cproject1App theApp;