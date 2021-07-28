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

    //*********************** NEW FUNCTIONS **************************//

    void checkMiniXTemp(double temp, bool is_HVOn);

    // warm up procedure
    void UpdateWarmUpTimer();                       // updates the warmup procedure at tmrWarmUp_Interval
    void getWarmUpPhaseProc(int warm_up_phase);     // sets voltage and current for each phase
    bool is_warmed_up = false;                      // flag is true once the warmup procedure is completed
    bool is_warming_up = false;
    bool is_warmup_start_phase = false;             // flag if start of a warmup phase
    bool is_warmup_pause = false;                   // indicates if the warmup proc is in the 10s after reaching phase values
    int warmup_pause_count = 0;                     // counts 10 seconds once warmup phase has reached its values
    int warmup_phase = 0;                           // indicates the current warmup phase
    int warmup_count_seconds = 0;                   // adds to total_runtime, initialized on timer creation (TimerControl) NOTE: is divided depending on tmrWarmUp_Interval
    double warmup_voltage[5] = { 15, 25, 35, 45, 50 };  // warmup procedure voltages
    double warmup_current[5] = { 15, 35, 50, 75, 79 };  // warmup procedure currents
    UINT_PTR tmrWarmUp_TimerId;
    bool tmrWarmUp_Enabled;
    UINT tmrWarmUp_Interval;
#define tmrWarmUp_EventId 40
    
    // experiment duration
    void UpdateExperimentTimer();               // updates the experiment timer at tmrDuration_Interval
    void refreshDurationTimeDisplay();          // updates the time left in set-time experiment
    CString getTimeInDisplayableFormat(int minutes, int seconds); // converts the input minutes and seconds into a cstring to be displayed
    CString getTimeInDisplayableFormat(int seconds);
    bool is_experiment_running = false;         // flag if a set-time experiment is currently running
    bool is_experiment_warmup = false;          // flag if the warmup for a set-time experiment is running
    int experiment_count_seconds;               // adds to total_runtime, initialized on timer creation (TimerControl)
    int experiment_duration_seconds = 0;        // the set duration of an experiment in seconds (different from experiment_count_seconds if experiment ended early)
    UINT_PTR tmrDuration_TimerId;
    bool tmrDuration_Enabled;
    UINT tmrDuration_Interval;
#define tmrDuration_EventId 50

#define RUNTIMEPATH "C:/ProgramData/Amptek_MiniX/"
    void serializeRuntime();
    void deserializeRuntime();
    int total_runtime_seconds = 0;              // the total lifetime (in seconds) the MiniX X-Ray has been emmitting x-rays (HVOn)
    UINT_PTR tmrRuntime_TimerId;
    bool tmrRuntime_Enabled;
    UINT tmrRuntime_Interval;
#define tmrRuntime_EventId 60

    afx_msg void OnBnClickedButtonBeginWarmup();
    afx_msg void OnBnClickedButtonCancelWarmup();
    afx_msg void OnBnClickedButtonBeginExperiment();
    afx_msg void OnBnClickedButtonStopExperiment();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIXDLG_H__2F8CF70A_C067_459D_9433_77004E95D105__INCLUDED_)
