
// UMT_Transmission.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUMT_TransmissionApp:
// See UMT_Transmission.cpp for the implementation of this class
//

class CUMT_TransmissionApp : public CWinApp
{
public:
	CUMT_TransmissionApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CUMT_TransmissionApp theApp;