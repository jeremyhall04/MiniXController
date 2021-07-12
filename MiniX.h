// MiniX.h : main header file for the MiniXAP application
//

#if !defined(AFX_MINIXAP_H__37120D8A_D6F7_4F3B_B355_10E393488AA0__INCLUDED_)
#define AFX_MINIXAP_H__37120D8A_D6F7_4F3B_B355_10E393488AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMiniXApp:
// See MiniX.cpp for the implementation of this class
//

class CMiniXApp : public CWinApp
{
public:
	CMiniXApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMiniXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CMiniXApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIXAP_H__37120D8A_D6F7_4F3B_B355_10E393488AA0__INCLUDED_)
