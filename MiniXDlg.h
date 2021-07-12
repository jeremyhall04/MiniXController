// MiniXDlg.h : header file
//

#include "afxwin.h"
#include "ColorControls.h"
#include "SXButton.h"			// Access local header

#if !defined(AFX_MINIXDLG_H__2F8CF70A_C067_459D_9433_77004E95D105__INCLUDED_)
#define AFX_MINIXDLG_H__2F8CF70A_C067_459D_9433_77004E95D105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum InterlockViewState {
    ilvsOPEN,
    ilvsRESTORE,
    ilvsHIDE
};

/////////////////////////////////////////////////////////////////////////////
// CMiniXDlg dialog

class CMiniXDlg : public CDialog
{
// Construction
public:
	CMiniXDlg(CWnd* pParent = NULL);	// standard constructor


// Dialog Data
	//{{AFX_DATA(CMiniXDlg)
	enum { IDD = IDD_MINIXAP_DIALOG };

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	HMODULE m_hmodule;

	// Generated message map functions
	//{{AFX_MSG(CMiniXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    virtual void OnCancel();
    virtual void OnOK();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHvOff();
	afx_msg void OnBnClickedHvOn();
    afx_msg void OnBnClickedStartMiniXController();
	afx_msg void OnBnClickedSethighvoltageandcurrentbutton();
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	CColorStatic m_indOutOfRange;	//CStatic m_indOutOfRange;
	CColorStatic m_indWattageMW;			//CStatic m_indWattageMW;
	CSXButton m_HvOnButton;		//CButton m_HvOnButton;
	CSXButton m_HvOffButton;	//CButton m_HvOffButton;
	CSXButton m_ExitButton;     //CButton m_ExitButton;

    afx_msg void OnTimer(UINT nIDEvent);

    UINT_PTR tmrXRayOn_TimerId;
    bool tmrXRayOn_Enabled;
    UINT tmrXRayOn_Interval;
    //UINT tmrXRayOn_EventId;
#define tmrXRayOn_EventId 10

    UINT_PTR tmrMonitor_TimerId;
    bool tmrMonitor_Enabled;
    UINT tmrMonitor_Interval;
    //UINT tmrMonitor_EventId;
#define tmrMonitor_EventId 20

    UINT_PTR tmrInterLock_TimerId;
    bool tmrInterLock_Enabled;
    UINT tmrInterLock_Interval;
    //UINT tmrInterLock_EventId;
#define tmrInterLock_EventId 30

    void TimerControl(UINT uEventId, bool isCreate);

    bool EnableMonitorButtons;
    void InterlockDisplay(InterlockViewState ILockView);
    void CMiniXDlg::UpdateMonitor();
    byte indInterlockStatus;
    byte indDisableMonitorCmds;
    void DisplayDouble(int DlgItem, double dblValue, int dpDigits);
    void DisplayStatus(byte StatusCode);
    void EnableMiniX_Commands(byte mxmEnabledCmds);
    double GetWindowDouble(int nID);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIXDLG_H__2F8CF70A_C067_459D_9433_77004E95D105__INCLUDED_)
