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

//holds Mini-X Fixed Constant values for Display Setup
typedef struct _MiniX_Constant {
	long lTubeTypeID;								// Tube Type Identifier Index
	double dblHighVoltageConversionFactor;			// High Voltage Conversion Factor
	double dblHighVoltageMin;						// High Voltage Min
	double dblHighVoltageMax;						// High Voltage Max
	double dblDefaultdblHighVoltage;				// Default High Voltage kV
	double dblCurrentMin;							// Current Min
	double dblCurrentMax;							// Current Max
	double dblDefaultCurrent;						// Default Current
	double dblWattageMax;							// Wattage Max 
	CString strTubeType;							// Tube Type Name
	CString strControllerType;						// Controller Type Name
	CString strCtrlTypeID;							// Controller Type Short Name
} MiniX_Constant, *LP_MiniX_Constant;

typedef enum _MINIX_CONTROLLER_ENUM {
	mxceMX70,
	mxceMX50,
	mxceMX5010,
	mxceMiniX,
    mxceUnknownMX,
	mxceUnknownMX1
} MINIX_CONTROLLER_ENUM;

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
	void HideIsoCurveAndMiniX();
	void DisplayIsoCurveAndMiniX(MINIX_CONTROLLER_ENUM iMX);
	bool is40kv;

	void ReadMiniXSetup40kvMiniX(MiniX_Constant *MiniXConstant);
	void ReadMiniXSetup50kvMiniX(MiniX_Constant *MiniXConstant);
	void ReadMiniXSetup50kv4W_OEM(MiniX_Constant *MiniXConstant);
	void ReadMiniXSetup50kv10W_OEM(MiniX_Constant *MiniXConstant);
	void ReadMiniXSetup70kv10W_OEM(MiniX_Constant *MiniXConstant);
	void ReadMiniXSetupDisplay(MiniX_Constant *MiniXConstant);
	CString MxDblDispFormat(double dblValue);
	MiniX_Constant MxDevice; 
	void MXControllerSetup();
	void ReadMXSetup(MINIX_CONTROLLER_ENUM iMX, MiniX_Constant *MiniXConstant);
	MINIX_CONTROLLER_ENUM iMX;


	//***************************************************************************************************************************************//

	//                                                          NEW FUNCTIONS   

	//***************************************************************************************************************************************//

	CColorStatic m_temperatureWarning;	// color control for temperature warning display
	CColorStatic m_warmupPhaseDisplay;			// color control for warmup phase display
	CColorStatic m_timeRemainingDisplay;		// color control for time remaining display

	void turnHVOn();
	void turnHVOff();
	void setAndStartMiniX(double voltage, double current);
	void setAndStartMiniX();
	void toggleButton(byte buttonID, bool isActive);
	void checkMiniXTemp(double temp, bool is_HVOn);

	//----------------------------------------------------------------------------------
	//								EXPERIMENTS & PROCEDURES
	//----------------------------------------------------------------------------------
	void updateExperiment();
	UINT_PTR tmrExperiment_TimerId;
	bool tmrExperiment_Enabled;
	UINT tmrExperiment_Interval;
#define tmrExperiment_EventId 50

	// WARMUP PROCEDURE
	//----------------------------------------------------------------------------------
	void warmupProcedure();
	void displayPhase(int warm_up_phase);
	CString getTimeInDisplayableFormat(int minutes, int seconds);
	CString getTimeInDisplayableFormat(int seconds);
	bool isWarmup = false;
	bool isStartOfPhase = false;
	bool isPaused = false;
	int phase = 0;							// indicates the current warmup phase (0, 1, 2, 3, or 4)
	double warmup_voltage[5] = { 15, 25, 35, 45, 50 };  // warmup procedure voltages for each phase (0, 1, 2, 3, and 4, respectively)
	double warmup_current[5] = { 15, 35, 50, 75, 79 };  // warmup procedure currents for each phase (0, 1, 2, 3, and 4, respectively)
#define PAUSETIME 9

	// EXPERIMENT DURATION PROCEDURE
	//----------------------------------------------------------------------------------
	void experimentProcedure();
	void refreshDurationTimeDisplay();
	bool isExperimentRunning = false;
	bool isExperimentStart = false;
	int experimentDurationS = 0;
	int experimentCountS = 0;


	//----------------------------------------------------------------------------------
	//                       SERVICE TIME TIMER & SERIALIZATION I/O
	//----------------------------------------------------------------------------------

#define RUNTIMEPATH "C:/ProgramData/Amptek_MiniX/runtime.txt"	// path to 
	void serializeRuntime();
	void deserializeRuntime();

	int total_runtime_seconds;		// the total runtime/service time (in seconds) the MiniX X-Ray has been ON (HVOn)
	UINT_PTR tmrRuntime_TimerId;
	bool tmrRuntime_Enabled;
	UINT tmrRuntime_Interval;
#define tmrRuntime_EventId 60

	//----------------------------------------------------------------------------------
	//									BUTTONS
	//----------------------------------------------------------------------------------
	afx_msg void OnBnClickedStartexperimentbutton();
	afx_msg void OnBnClickedStopexperimentbutton();
	afx_msg void OnBnClickedBeginwarmup();
	afx_msg void OnBnClickedStopwarmup();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIXDLG_H__2F8CF70A_C067_459D_9433_77004E95D105__INCLUDED_)
