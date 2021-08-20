// MiniX.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MiniX.h"
#include "MiniXDlg.h"
#include "LimitSingleInstance.H"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniXApp

BEGIN_MESSAGE_MAP(CMiniXApp, CWinApp)
	//{{AFX_MSG_MAP(CMiniXApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniXApp construction

CMiniXApp::CMiniXApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMiniXApp object

CMiniXApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMiniXApp initialization

// The one and only CLimitSingleInstance object.
CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{4573F231-88FD-4320-94A4-57ECC4CEE51C}"));

BOOL CMiniXApp::InitInstance()
{

	if (g_SingleInstanceObj.IsAnotherInstanceRunning())
       return FALSE; 

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CMiniXDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
