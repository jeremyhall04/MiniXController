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
	CColorStatic m_indWattageMW;	//CStatic m_indWattageMW;
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

    //***************************************************************************************************************************************//

    //                                                          NEW FUNCTIONS   

    //***************************************************************************************************************************************//

    CColorStatic m_warmupPhase;     // color control for warmup phase display
    CColorStatic m_timeRemaining;   // color control for time remaining display

    void checkMiniXTemp(double temp, bool is_HVOn); 
    void VC_SetSendStart();
    void VC_Set();
    void VC_SendAndStart();
    void VC_ReturnCorrected();

    //----------------------------------------------------------------------------------
    //                              WARMUP PROCEDURE & TIMER
    //----------------------------------------------------------------------------------

    void updateWarmUpTimer();
    void getWarmUpPhaseProc(int warm_up_phase);
    bool is_warmed_up = false;                          // flag once the warmup procedure is completed
    bool is_warming_up = false;                         // flag during heating up to set values
    bool is_warmup_start_phase = false;                 // flag if start of a warmup phase
    bool is_warmup_pause = false;                       // indicates if the warmup proc is in the 10s after reaching phase-set values
    int warmup_pause_count = 0;                         // counts 10 seconds once warmup phase has reached its values
    int warmup_phase = 0;                               // indicates the current warmup phase (0, 1, 2, 3, or 4)
    double warmup_voltage[5] = { 15, 25, 35, 45, 50 };  // warmup procedure voltages for each phase (0, 1, 2, 3, and 4, respectively)
    double warmup_current[5] = { 15, 35, 50, 75, 79 };  // warmup procedure currents for each phase (0, 1, 2, 3, and 4, respectively)
    UINT_PTR tmrWarmUp_TimerId;
    bool tmrWarmUp_Enabled;
    UINT tmrWarmUp_Interval;
#define tmrWarmUp_EventId 40
    
    //----------------------------------------------------------------------------------
    //                        EXPERIMENT DURATION PROCEDURE & TIMER
    //----------------------------------------------------------------------------------

    void updateExperimentTimer();
    void refreshDurationTimeDisplay();
    CString getTimeInDisplayableFormat(int minutes, int seconds);
    CString getTimeInDisplayableFormat(int seconds);
    bool is_experiment_running = false;         // flag if a set-duration experiment is currently running
    bool is_experiment_warmup = false;          // flag if the warmup for a set-time experiment is running
    int experiment_duration_seconds = 0;        // the set duration of an experiment in seconds (different from experiment_count_seconds. ex: if experiment ends early)
    UINT_PTR tmrDuration_TimerId;
    bool tmrDuration_Enabled;
    UINT tmrDuration_Interval;
#define tmrDuration_EventId 50

    //----------------------------------------------------------------------------------
    //                       SERVICE TIME TIMER & SERIALIZATION I/O
    //----------------------------------------------------------------------------------

#define RUNTIMEPATH "C:/ProgramData/Amptek_MiniX/"
    void serializeRuntime();
    void deserializeRuntime();
    int total_runtime_seconds = 0;  // the total runtime/service time (in seconds) the MiniX X-Ray has been ON (HVOn)
    int warmup_count_seconds = 0;   // total time ON during warmup procedure, adds to total_runtime
    int experiment_count_seconds;   // total time ON during set-duration experiment, adds to total_runtime
    UINT_PTR tmrRuntime_TimerId;
    bool tmrRuntime_Enabled;
    UINT tmrRuntime_Interval;
#define tmrRuntime_EventId 60

    //----------------------------------------------------------------------------------
    //                                  BUTTONS
    //----------------------------------------------------------------------------------

    afx_msg void OnBnClickedButtonBeginWarmup();
    afx_msg void OnBnClickedButtonCancelWarmup();
    afx_msg void OnBnClickedButtonBeginExperiment();
    afx_msg void OnBnClickedButtonStopExperiment();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIXDLG_H__2F8CF70A_C067_459D_9433_77004E95D105__INCLUDED_)
