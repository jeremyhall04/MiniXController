// MiniXDlg.cpp : implementation file

// New branch test

#include "stdafx.h"
#include "MiniX.h"
#include "MiniXDlg.h"
#include  <math.h>
#include "VersionInfo.h"
#include "MiniXAPI.h"
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define isDebug true
#else
#define isDebug false
#endif

#define WARMUPTIMERINTERVAL 500

COLORREF    crBkGnd;
COLORREF    crBkGndActive;
COLORREF    crText; 
COLORREF    crTextActive; 


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
    CString cstr_MiniXTotalRuntimeSeconds;
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CenterWindow();
	CVersionInfo AppVersion;
	CString strDate = __DATE__;                             
	CString strTime = __TIME__;
    SetDlgItemText(IDC_FILEDESCRIPTION, AppVersion.GetFileDescription());
	SetDlgItemText(IDC_PRODUCTVERSION, AppVersion.GetProductVersion());
	SetDlgItemText(IDC_LEGALCOPYRIGHT, AppVersion.GetCopyright());
	SetDlgItemText(IDC_lblBuildDate, strDate);    
	SetDlgItemText(IDC_lblBuildTime, strTime);
    GetDlgItem(IDC_ACTIVERUNTIMEDISPLAY)->SetWindowText(cstr_MiniXTotalRuntimeSeconds);
	return TRUE;  // return TRUE unless you set the focus to a control
}


/////////////////////////////////////////////////////////////////////////////
// CMiniXDlg dialog


CMiniXDlg::CMiniXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiniXDlg)
    deserializeRuntime();

    tmrXRayOn_TimerId = 0;
    //tmrXRayOn_EventId = 10;
    tmrXRayOn_Interval = 1000;
    tmrXRayOn_Enabled = false;

    tmrMonitor_TimerId = 0;
    //tmrMonitor_EventId = 20;
    tmrMonitor_Interval = 1500;
    tmrMonitor_Enabled = false;

    tmrInterLock_TimerId = 0;
    //tmrInterLock_EventId = 30;
    tmrInterLock_Interval = 4000;
    tmrInterLock_Enabled = false;

    tmrWarmUp_TimerId = 0;
    tmrWarmUp_Interval = 500;
    tmrWarmUp_Enabled = false;

    tmrDuration_TimerId = 0;    
    tmrDuration_Interval = 1000;
    tmrDuration_Enabled = false;

    tmrRuntime_TimerId = 0;
    tmrRuntime_Interval = 1000;
    tmrRuntime_Enabled = false;
    
    //}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniXDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_HV_ON, m_HvOnButton);
	DDX_Control(pDX, IDC_HV_OFF, m_HvOffButton);
	DDX_Control(pDX, IDOK, m_ExitButton);
	DDX_Control(pDX, IDC_OUT_OF_RANGE_LBL, m_indOutOfRange);
	DDX_Control(pDX, IDC_POWER_MW, m_indWattageMW);
}

BEGIN_MESSAGE_MAP(CMiniXDlg, CDialog)
	//{{AFX_MSG_MAP(CMiniXDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HV_OFF, OnBnClickedHvOff)
	ON_BN_CLICKED(IDC_HV_ON, OnBnClickedHvOn)
	ON_BN_CLICKED(IDC_START_MINIX_CONTROLLER, OnBnClickedStartMiniXController)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON, OnBnClickedSethighvoltageandcurrentbutton)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_BEGIN_WARMUP, &CMiniXDlg::OnBnClickedButtonBeginWarmup)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL_WARMUP, &CMiniXDlg::OnBnClickedButtonCancelWarmup)
    ON_BN_CLICKED(IDC_BUTTON_BEGIN_EXPERIMENT, &CMiniXDlg::OnBnClickedButtonBeginExperiment)
    ON_BN_CLICKED(IDC_BUTTON_STOP_EXPERIMENT, &CMiniXDlg::OnBnClickedButtonStopExperiment)
END_MESSAGE_MAP()
// CMiniXDlg message handlers

void CMiniXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
        dlgAbout.cstr_MiniXTotalRuntimeSeconds = getTimeInDisplayableFormat(total_runtime_seconds);
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMiniXDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//****************************************************************************************
BOOL CMiniXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_HvOffButton.SetIcon(IDI_STOPSIGN, 32, 32);
	m_HvOnButton.SetIcon(IDI_RAD32, 32, 32);
	m_ExitButton.SetIcon(IDI_EXIT32, 32, 32);


    // Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	crBkGnd = GetSysColor(COLOR_3DFACE);
	crBkGndActive = ctrlColRed;
	crText = GRAY; 
	crTextActive = BLUE; 

	m_indOutOfRange.SetWindowText(">>> MONITOR OUT OF RANGE <<<");
	m_indOutOfRange.SetBkColor(ctrlColYellow);						//ctrlColGreen);
	m_indOutOfRange.SetTextColor(ctrlColRed);
	m_indOutOfRange.EnableWindow(true);
	m_indOutOfRange.ShowWindow(SW_HIDE);

	m_indWattageMW.SetWindowText("");
	m_indWattageMW.SetTextColor(ctrlColBlack);
	m_indWattageMW.SetBkColor(ctrlColWhite);

    GetDlgItem(IDC_SETHIGHVOLTAGECONTROLEDIT)->SetWindowText("0");
	GetDlgItem(IDC_SETCURRENTCONTROLEDIT)->SetWindowText("0");


    GetDlgItem(IDC_HV_ON)->EnableWindow(false);
	GetDlgItem(IDC_HV_OFF)->EnableWindow(false);
	GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_BEGIN_WARMUP)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_CANCEL_WARMUP)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_BEGIN_EXPERIMENT)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_STOP_EXPERIMENT)->EnableWindow(false);

	UpdateData(FALSE);
    UpdateWindow();

    ////////////////////////////////////////////////////////////////////
    // initialize minix controller and get initial kV and uA values

    indInterlockStatus = false;
    indDisableMonitorCmds = false;

    MiniX_Settings MiniXSettings; 
    GetDlgItem(IDC_SERIAL_NUMBER)->SetWindowText("");
    InterlockDisplay(ilvsHIDE);
    EnableMonitorButtons = false;
    if (! isMiniXDlg()) {
        OpenMiniX();
        Sleep(100);
    }
    if (isMiniXDlg()) {
        ReadMiniXSettings(&MiniXSettings);
        DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT,MiniXSettings.HighVoltage_kV,0);
        DisplayDouble(IDC_SETCURRENTCONTROLEDIT,MiniXSettings.Current_uA,0);
    }

    ////////////////////////////////////////////////////////////////////
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMiniXDlg::DisplayDouble(int DlgItem, double dblValue, int dpDigits=0) 
{
    CString cstrValue;
    CString cstrFormat;
    cstrFormat.Format("%%.%df",dpDigits);           // get decimal point precision
    cstrValue.Format(cstrFormat,dblValue);          // format data
    GetDlgItem(DlgItem)->SetWindowText(cstrValue);  // display in control
}

void CMiniXDlg::DisplayStatus(byte StatusCode)
{
    CString cstrStatus;
    cstrStatus = GetMiniXStatusString(StatusCode);              // get status string
    GetDlgItem(IDC_STATUS_EDIT)->SetWindowText(cstrStatus);     // display status
}

double CMiniXDlg::GetWindowDouble(int nID)
{
    CString cstrValue;
    double dblValue;
    GetDlgItem(nID)->GetWindowText(cstrValue);
    dblValue = atof(cstrValue);
    return dblValue;
}

void CMiniXDlg::InterlockDisplay(InterlockViewState ILockView) 
{
    if (ILockView == ilvsRESTORE) {
		GetDlgItem(IDC_INTERLOCK_RESTORED)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERLOCK_RESTORED_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERLOCK_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INTERLOCK_OPEN_LABEL)->ShowWindow(SW_HIDE);
	} else if(ILockView == ilvsHIDE) {
		GetDlgItem(IDC_INTERLOCK_RESTORED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INTERLOCK_RESTORED_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INTERLOCK_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INTERLOCK_OPEN_LABEL)->ShowWindow(SW_HIDE);
	} else if(ILockView == ilvsOPEN) {
		GetDlgItem(IDC_INTERLOCK_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERLOCK_OPEN_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERLOCK_RESTORED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INTERLOCK_RESTORED_LABEL)->ShowWindow(SW_HIDE);
    }
}

void CMiniXDlg::OnBnClickedSethighvoltageandcurrentbutton()
{
    double dblValue;
    MiniX_Settings MiniXSettings;
    indDisableMonitorCmds = true;
	GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(false);
	GetDlgItem(IDC_HV_ON)->EnableWindow(false);
	GetDlgItem(IDC_HV_OFF)->EnableWindow(false);
    if (isMiniXDlg()) {
        dblValue = GetWindowDouble(IDC_SETHIGHVOLTAGECONTROLEDIT);  //sent hv value
        SetMiniXHV(dblValue);
        Sleep(100);
        dblValue = GetWindowDouble(IDC_SETCURRENTCONTROLEDIT);      //send current value
        SetMiniXCurrent(dblValue);
        Sleep(100);
        SendMiniXCommand((byte)mxcSetHVandCurrent);
        Sleep(1000);
        ReadMiniXSettings(&MiniXSettings);     //read the corrected values back
        DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT,MiniXSettings.HighVoltage_kV,0);
        DisplayDouble(IDC_SETCURRENTCONTROLEDIT,MiniXSettings.Current_uA,0);
    }
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnBnClickedStartMiniXController() 
{
    long MiniXSerialNumber;
    CString cstrSerialNumber;

    //==== MiniXSerialNumber >= 10000 and < 1118880 40kV Tube ====
    //==== MiniXSerialNumber >= 1118880 50kV Tube ====
	GetDlgItem(IDC_STATIC_ISO_CURVE2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ISO_CURVE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_HIGHVOLTAGECONTROLMAX)->SetWindowText("40");

    indDisableMonitorCmds = true;
    GetDlgItem(IDC_START_MINIX_CONTROLLER)->EnableWindow(false);
    if (isMiniXDlg()) {
        SendMiniXCommand((byte)mxcStartMiniX);
        Sleep(100);
        TimerControl(tmrMonitor_EventId,true);
    }
    if (isMiniXDlg()) {
        Sleep(100);
        MiniXSerialNumber = ReadMiniXSerialNumber();
        if (MiniXSerialNumber > 0) {
            cstrSerialNumber.Format("%08d",MiniXSerialNumber);
            cstrSerialNumber = "Mini-X Serial Number " + cstrSerialNumber;
            GetDlgItem(IDC_SERIAL_NUMBER)->SetWindowText(cstrSerialNumber);
            if (MiniXSerialNumber >= 1118880) {  //50kV Tube
				GetDlgItem(IDC_STATIC_ISO_CURVE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_ISO_CURVE2)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_HIGHVOLTAGECONTROLMAX)->SetWindowText("50");
                GetDlgItem(IDC_CURRENTCONTROLMAX)->SetWindowText("80");
            }
        } else {
            GetDlgItem(IDC_SERIAL_NUMBER)->SetWindowText("");
        }
    }
    GetDlgItem(IDC_BUTTON_BEGIN_WARMUP)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_CANCEL_WARMUP)->EnableWindow(true);
    //GetDlgItem(IDC_BUTTON_BEGIN_EXPERIMENT)->EnableWindow(true);  // uncomment to avoid warmup requried
    //GetDlgItem(IDC_BUTTON_STOP_EXPERIMENT)->EnableWindow(true);
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnBnClickedHvOn() 
{
    int msgResponse;
    indDisableMonitorCmds = true;
    GetDlgItem(IDC_HV_ON)->EnableWindow(false);
    if (isMiniXDlg()) {
        msgResponse = AfxMessageBox("Turn X-RAY High Voltage ON?",MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
        if (msgResponse == IDYES) {
            TimerControl(tmrRuntime_EventId, true);
            SendMiniXCommand((byte)mxcHVOn);
            Sleep(100);
            GetDlgItem(IDC_BUTTON_BEGIN_EXPERIMENT)->EnableWindow(false);
        }
    }
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnBnClickedHvOff() 
{
    indDisableMonitorCmds = true;
    GetDlgItem(IDC_HV_OFF)->EnableWindow(false);
    if (isMiniXDlg()) {
        SendMiniXCommand((byte)mxcHVOff);
        Sleep(100);
        TimerControl(tmrRuntime_EventId, false);
        GetDlgItem(IDC_BUTTON_BEGIN_EXPERIMENT)->EnableWindow(true);
    }
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnClose()
{
    serializeRuntime();
	DestroyWindow();
}

void CMiniXDlg::OnCancel()
{
    serializeRuntime();
    DestroyWindow();
	//CDialog::OnCancel();	// don't use - failsafe must shutdown HV
}

void CMiniXDlg::OnDestroy() {
    if (isMiniXDlg()) {
        serializeRuntime();
        TimerControl(tmrMonitor_EventId,false);
        SendMiniXCommand((byte)mxcExit);
        Sleep(100);
    }
    if (isMiniXDlg()) {
        CloseMiniX();
        Sleep(100);
    }
}

void CMiniXDlg::OnOK()
{
	CDialog::OnOK();
}

void CMiniXDlg::OnBnClickedOk()
{
	OnOK();
}

void CMiniXDlg::TimerControl(UINT uEventId, bool isCreate) 
{
    switch (uEventId) {
        case tmrXRayOn_EventId:
            if (isCreate)
	            tmrXRayOn_TimerId = SetTimer(tmrXRayOn_EventId, tmrXRayOn_Interval, NULL);
            else
                KillTimer(tmrXRayOn_TimerId);
	        tmrXRayOn_Enabled = isCreate;
            break;
        case tmrMonitor_EventId:
            if (isCreate)
	            tmrMonitor_TimerId = SetTimer(tmrMonitor_EventId, tmrMonitor_Interval, NULL);
            else
                KillTimer(tmrMonitor_TimerId);
	        tmrMonitor_Enabled = isCreate;
            break;
        case tmrInterLock_EventId:
             if (isCreate)
	            tmrInterLock_TimerId = SetTimer(tmrInterLock_EventId, tmrInterLock_Interval, NULL);
            else
                KillTimer(tmrInterLock_TimerId);
	        tmrInterLock_Enabled = isCreate;
            break;
        case tmrWarmUp_EventId:
            if (isCreate) {
                tmrWarmUp_TimerId = SetTimer(tmrWarmUp_EventId, tmrWarmUp_Interval, NULL);
            }
            else {
                KillTimer(tmrWarmUp_TimerId);
            }
            tmrWarmUp_Enabled = isCreate;
            break;
        case tmrDuration_EventId:
            if (isCreate) {
                experiment_count_seconds = 0;
                tmrDuration_TimerId = SetTimer(tmrDuration_EventId, tmrDuration_Interval, NULL);
                UpdateExperimentTimer();
            }
            else
                KillTimer(tmrDuration_TimerId);
            tmrDuration_Enabled = isCreate;
            break;
        case tmrRuntime_EventId:
            if (isCreate)
                tmrRuntime_TimerId = SetTimer(tmrRuntime_EventId, tmrRuntime_Interval, NULL);
            else
                KillTimer(tmrRuntime_TimerId);
            tmrRuntime_Enabled = isCreate;
            break;
    }
}

void CMiniXDlg::OnTimer(UINT nIDEvent) 
{
    if (nIDEvent == tmrInterLock_TimerId) {
        InterlockDisplay(ilvsHIDE);
    }
    if (nIDEvent == tmrMonitor_TimerId) {
        UpdateMonitor();
    }
    if (nIDEvent == tmrXRayOn_TimerId) { 
        GetDlgItem(IDC_XRAY_ON_LABEL)->ShowWindow(SW_SHOW);
        if (GetDlgItem(IDC_XRAY_ON_PIC)->IsWindowVisible()) {
            GetDlgItem(IDC_XRAY_ON_PIC)->ShowWindow(SW_HIDE);
        } else {
            GetDlgItem(IDC_XRAY_ON_PIC)->ShowWindow(SW_SHOW);
        }
    }
    if (nIDEvent == tmrWarmUp_EventId) {
        UpdateWarmUpTimer();
    }
    if (nIDEvent == tmrDuration_EventId) {
        UpdateExperimentTimer();
    }
    if (nIDEvent == tmrRuntime_EventId) {
        total_runtime_seconds++;
    }
    CDialog::OnTimer(nIDEvent);
}

void CMiniXDlg::UpdateMonitor() 
{
    MiniX_Monitor MiniXMonitor; 
    CString strMonitor;
    byte indOldInterlockStatus;

    if (isMiniXDlg()) {
        ReadMiniXMonitor(&MiniXMonitor);
        Sleep(100);
        if (MiniXMonitor.mxmRefreshed) {             //test if the status was updated
        if (MiniXMonitor.mxmReserved = 123.456) {    //reserved but always 123.456
            DisplayDouble(IDC_HIGHVOLTAGEMONITORTEXT,MiniXMonitor.mxmHighVoltage_kV,1);
            DisplayDouble(IDC_CURRENTMONITORTEXT,MiniXMonitor.mxmCurrent_uA,1);
            DisplayDouble(IDC_POWER_MW,MiniXMonitor.mxmPower_mW,0);
            checkMiniXTemp(MiniXMonitor.mxmTemperatureC, MiniXMonitor.mxmHVOn);
            DisplayDouble(IDC_TEMPERATURE_SENSOR,MiniXMonitor.mxmTemperatureC,0);
            DisplayStatus(MiniXMonitor.mxmStatusInd);
            indOldInterlockStatus = indInterlockStatus;
            indInterlockStatus = MiniXMonitor.mxmInterLock;
            if (indOldInterlockStatus != indInterlockStatus) {
                if (indInterlockStatus) {
                    InterlockDisplay(ilvsRESTORE);
                    TimerControl(tmrInterLock_EventId,true);
                } else {
                    InterlockDisplay(ilvsOPEN);
                    TimerControl(tmrInterLock_EventId,false);
                }
            }
            if (is_warmed_up) { // comment out if to avoid warmup required 
                if (is_experiment_running) {
                    GetDlgItem(IDC_BUTTON_STOP_EXPERIMENT)->EnableWindow(true);
                    GetDlgItem(IDC_HV_ON)->EnableWindow(false);
                    GetDlgItem(IDC_HV_OFF)->EnableWindow(false);
                    GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(false);
                    indDisableMonitorCmds = true;
                }
                else {
                    GetDlgItem(IDC_BUTTON_STOP_EXPERIMENT)->EnableWindow(false);
                    GetDlgItem(IDC_HV_ON)->EnableWindow(true);
                    GetDlgItem(IDC_HV_OFF)->EnableWindow(true);
                    GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(true);
                    indDisableMonitorCmds = false;
                    EnableMiniX_Commands(MiniXMonitor.mxmEnabledCmds);
                }
            }
            if (MiniXMonitor.mxmHVOn) {
                if (! tmrXRayOn_Enabled) { TimerControl(tmrXRayOn_EventId,true); }
            } else {
                if (tmrXRayOn_Enabled) { TimerControl(tmrXRayOn_EventId,false); }
                if (GetDlgItem(IDC_XRAY_ON_PIC)->IsWindowVisible()) {
                    GetDlgItem(IDC_XRAY_ON_PIC)->ShowWindow(SW_HIDE);
                }
                if (GetDlgItem(IDC_XRAY_ON_LABEL)->IsWindowVisible()) {
                    GetDlgItem(IDC_XRAY_ON_LABEL)->ShowWindow(SW_HIDE);
                }
            }
            if (MiniXMonitor.mxmOutOfRange) {
                GetDlgItem(IDC_OUT_OF_RANGE_LBL)->ShowWindow(SW_SHOW);
            } else {
                GetDlgItem(IDC_OUT_OF_RANGE_LBL)->ShowWindow(SW_HIDE);
            }
        }
        }
    }
}

void CMiniXDlg::EnableMiniX_Commands(byte mxmEnabledCmds)
{
    bool indStartMiniX;
    bool indHVOn;
    bool indHVOff;
    bool indSetHVandCurrent;

    if (! indDisableMonitorCmds) {
        indStartMiniX = ((mxmEnabledCmds & mxcStartMiniX) == mxcStartMiniX);
        indHVOn = ((mxmEnabledCmds & mxcHVOn) == mxcHVOn);
        indHVOff = ((mxmEnabledCmds & mxcHVOff) == mxcHVOff);
        indSetHVandCurrent = ((mxmEnabledCmds & mxcSetHVandCurrent) == mxcSetHVandCurrent);
        GetDlgItem(IDC_START_MINIX_CONTROLLER)->EnableWindow(indStartMiniX);
	    GetDlgItem(IDC_HV_ON)->EnableWindow(indHVOn);
	    GetDlgItem(IDC_HV_OFF)->EnableWindow(indHVOff);
	    GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(indSetHVandCurrent);
    }
}


                        //********************************************************************************//

                        //                              NEW FUNCTIONS   

                        //********************************************************************************//

void CMiniXDlg::checkMiniXTemp(double temp, bool is_HVOn)
{
    bool max_temp_reached = false;
    CString msg1 = "", msg2 = "";
    if (is_HVOn) {
        if (temp >= 40) {
            SendMiniXCommand((byte)mxcHVOff);
            Sleep(100);
            if (tmrWarmUp_Enabled) {
                TimerControl(tmrWarmUp_EventId, false);
                is_warmed_up = false;
                is_warmup_start_phase = false;
                warmup_phase = 0;
            }
            if (tmrDuration_Enabled) {
                TimerControl(tmrDuration_EventId, false);
                is_experiment_running = false;
                is_experiment_warmup = false;
            }
            msg1 = "MAX TEMP REACHED";
            msg2 = "STOPPING MINIX";
            max_temp_reached = true;
        }
        else if (temp >= 39) {
            msg1 = "WARNING: TEMPERATURE AT 39C";
            msg2 = "AT 40C AUTO STOP";
        }
        else if (temp >= 35) {
            msg1 = "WARNING: TEMPERATURE > 35C";
            msg2 = "";
        }
        else {
            msg1 = "";
            msg2 = "";
        }
    }
    GetDlgItem(IDC_TEMPWARNINGTEXT)->SetWindowText(msg1);
    GetDlgItem(IDC_TEMPWARNINGTEXT2)->SetWindowText(msg2);
    if (max_temp_reached)
        AfxMessageBox("MINIX STOPPED DUE TO OVERHEATING\nTemperatures exceeding 39 degrees celcius were reached.");
}

void CMiniXDlg::UpdateWarmUpTimer() 
{
    static int warmup_pause_start;
    warmup_count_seconds++;
    MiniX_Monitor MiniXMonitor;
    MiniX_Settings MiniXSettings;
    if (!is_warmup_pause) {
        if (is_warmup_start_phase) {
            getWarmUpPhaseProc(warmup_phase);
            is_warmup_start_phase = false;
        }
        warmup_pause_start = warmup_count_seconds;
    }
    ReadMiniXMonitor(&MiniXMonitor);
    ReadMiniXSettings(&MiniXSettings);
    if (MiniXMonitor.mxmHighVoltage_kV >= MiniXSettings.HighVoltage_kV
        && MiniXMonitor.mxmCurrent_uA >= MiniXSettings.Current_uA)
    {
        if (is_experiment_warmup) {
            // for experiment duration 
            is_experiment_warmup = false;
            TimerControl(tmrDuration_EventId, true);    // immediately start the experiment timer
            TimerControl(tmrWarmUp_EventId, false);
        }
        else {
            // for warm up procedure & 8 second pause b/w phases
            if (warmup_count_seconds - warmup_pause_start >= 8) {
                SendMiniXCommand((byte)mxcHVOff);
                Sleep(100);
                is_warmup_pause = false;
                if (warmup_phase < 4) {
                    //proceed to next phase
                    warmup_phase++;
                    is_warmup_start_phase = true;
                }
                else {
                    // finished warm up procedure
                    TimerControl(tmrRuntime_EventId, false);
                    TimerControl(tmrWarmUp_EventId, false);
                    is_warmup_start_phase = false;
                    is_warmed_up = true;
                    GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText("Warm Up Complete.");
                    GetDlgItem(IDC_BUTTON_CANCEL_WARMUP)->EnableWindow(false);
                    GetDlgItem(IDC_BUTTON_BEGIN_EXPERIMENT)->EnableWindow(true);
                    AfxMessageBox("Warm Up Complete", MB_OK);
                }
            }
            else {
                is_warmup_pause = true;
                CString msg;
                int time_left = 8 - ((int)warmup_count_seconds - warmup_pause_start);
                msg.Format("Phase %i: Waiting %is", warmup_phase + 1, time_left);
                GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText(msg);
            }
        }
    }
}

void CMiniXDlg::getWarmUpPhaseProc(int warm_up_phase)
{
    if (warm_up_phase == 0) {
        GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText("Phase 1: HV = 15kV | Current = 15uA");
        SetMiniXHV(warmup_voltage[warm_up_phase]);
        Sleep(100);
        SetMiniXCurrent(warmup_current[warm_up_phase]);
        Sleep(100);
    }
    else if (warm_up_phase == 1) {
        GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText("Phase 2: HV = 25kV | Current = 35uA");
        SetMiniXHV(warmup_voltage[warm_up_phase]);
        Sleep(100);
        SetMiniXCurrent(warmup_current[warm_up_phase]);
        Sleep(100);
    }
    else if (warm_up_phase == 2) {
        GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText("Phase 3: HV = 35kV | Current = 50uA");
        SetMiniXHV(warmup_voltage[warm_up_phase]);
        Sleep(100);
        SetMiniXCurrent(warmup_current[warm_up_phase]);
        Sleep(100);
    }
    else if (warm_up_phase == 3) {
        GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText("Phase 4: HV = 45kV | Current = 75uA");
        SetMiniXHV(warmup_voltage[warm_up_phase]);
        Sleep(100);
        SetMiniXCurrent(warmup_current[warm_up_phase]);
        Sleep(100);
    }
    else if (warm_up_phase == 4) {
        GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText("Phase 5: HV = 50kV | Current = 79uA");
        SetMiniXHV(warmup_voltage[warm_up_phase]);
        Sleep(100);
        SetMiniXCurrent(warmup_current[warm_up_phase]);
        Sleep(100);
    }
    // set voltage and current
    SendMiniXCommand((byte)mxcSetHVandCurrent);
    Sleep(100);
    // turn on miniX
    SendMiniXCommand((byte)mxcHVOn);
    Sleep(100);
}

void CMiniXDlg::UpdateExperimentTimer() 
{    
    if (is_experiment_running) {
        refreshDurationTimeDisplay();
        if (experiment_count_seconds == experiment_duration_seconds) {
            // end experiment
            SendMiniXCommand((byte)mxcHVOff);
            Sleep(100);
            TimerControl(tmrRuntime_EventId, false);
            TimerControl(tmrDuration_EventId, false);
            is_experiment_running = false;
            experiment_count_seconds = 0;
            AfxMessageBox("Experiment Complete.", MB_OK);
            GetDlgItem(IDC_BUTTON_STOP_EXPERIMENT)->EnableWindow(false);
        }
        else {
            experiment_count_seconds++;
        }
    }
}

void CMiniXDlg::refreshDurationTimeDisplay()
{
    int minutes, seconds;
    int total_remaining_seconds = experiment_duration_seconds - experiment_count_seconds;
    CString s_time;
    minutes = total_remaining_seconds / 60;
    seconds = total_remaining_seconds % 60;
    s_time = getTimeInDisplayableFormat(minutes, seconds);
    GetDlgItem(IDC_DURATION_TIME_TEXT)->SetWindowText(s_time);
}

CString CMiniXDlg::getTimeInDisplayableFormat(int minutes, int seconds)
{
    int hours = 0;
    CString s_time;
    if (minutes >= 60) {
        hours = minutes / 60;
        minutes = minutes % 60;
    }
    if (hours < 10) {
        if (minutes < 10)
            if (seconds < 10)
                s_time.Format("%i%i:%i%i:%i%i", 0, hours, 0, minutes, 0, seconds);
            else
                s_time.Format("%i%i:%i%i:%i", 0, hours, 0, minutes, seconds);
        else
            if (seconds < 10)
                s_time.Format("%i%i:%i:%i%i", 0, hours, minutes, 0, seconds);
            else
                s_time.Format("%i%i:%i:%i", 0, hours, minutes, seconds);
    }
    else {
        if (minutes < 10)
            if (seconds < 10)
                s_time.Format("%i:%i%i:%i%i", hours, 0, minutes, 0, seconds);
            else
                s_time.Format("%i:%i%i:%i", hours, 0, minutes, seconds);
        else
            if (seconds < 10)
                s_time.Format("%i:%i:%i%i", hours, minutes, 0, seconds);
            else
                s_time.Format("%i:%i:%i", hours, minutes, seconds);
    }
    return s_time;
}

CString CMiniXDlg::getTimeInDisplayableFormat(int seconds)
{
    int minutes = 0;
    if (seconds > 60) {
        minutes = seconds / 60;
        seconds = seconds % 60;
    }
    return getTimeInDisplayableFormat(minutes, seconds);
}

void CMiniXDlg::serializeRuntime()
{
    if (CreateDirectory(RUNTIMEPATH, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
        CString path;
        path.Format("%sruntime.txt", RUNTIMEPATH);
        std::ofstream fout;
        fout.open(path);
        if (fout.is_open()) {
            fout << total_runtime_seconds << " s\n";
            fout << "\nTotal Runtime " << getTimeInDisplayableFormat(total_runtime_seconds);
        }
        else
            AfxMessageBox("Error serializing service time.\nMini-X service time for current session has not been saved.", MB_OK);
        fout.close();
    }
    else 
        AfxMessageBox("ERROR Creating save directory.", MB_OK);
}

void CMiniXDlg::deserializeRuntime()
{
    char *buf = new char();
    CString path;
    path.Format("%sruntime.txt", RUNTIMEPATH);
    std::ifstream fin;
    fin.open(path);
    if (fin.is_open()) {
        fin >> buf;
        total_runtime_seconds = atoi(buf);
    }
    else {
        CString msg;
        msg.Format("Error reading Mini-X service time.\n\nCheck that file 'runtime.txt' is located in %s", RUNTIMEPATH);
        AfxMessageBox(msg, MB_OK);
    }
    fin.close();
}

void CMiniXDlg::OnBnClickedButtonBeginWarmup() 
{
    indDisableMonitorCmds = true;
    GetDlgItem(IDC_BUTTON_BEGIN_WARMUP)->EnableWindow(false);
    if (isMiniXDlg()){
        int msgResponse = AfxMessageBox("Turn X-RAY High Voltage ON for the Warm Up Routine?", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
        if (msgResponse == IDYES) {
            is_warming_up = true;
            is_warmup_start_phase = true;
            TimerControl(tmrWarmUp_EventId, true);
            TimerControl(tmrRuntime_EventId, true);
        }
    }
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnBnClickedButtonCancelWarmup() 
{
    indDisableMonitorCmds = true;
    if (isMiniXDlg()) {
        int msg = AfxMessageBox("Cancel Warm Up?", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
        if (msg == IDYES) {
            SendMiniXCommand((byte)mxcHVOff);
            Sleep(100);
            TimerControl(tmrWarmUp_EventId, false);
            TimerControl(tmrRuntime_EventId, false);
            is_warmup_start_phase = false;
            is_warming_up = false;
            is_warmup_pause = false;
            GetDlgItem(IDC_BUTTON_BEGIN_WARMUP)->EnableWindow(true);
            GetDlgItem(IDC_WARMUPMONITORTEXT)->SetWindowText("");
        }
    }
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnBnClickedButtonBeginExperiment() 
{
    indDisableMonitorCmds = true;
    if (isMiniXDlg()) {
        double minutes, seconds;
        MiniX_Settings MiniXSettings;
        CString s_time;
        minutes = GetWindowDouble(IDC_EXPERIMENTDURATIONMINEDIT); // minutes
        seconds = GetWindowDouble(IDC_EXPERIMENTDURATIONSECEDIT); // seconds
        if (minutes == 0.0 && seconds == 0.0) {
            AfxMessageBox("No time was entered. Please enter the minutes and seconds of the duration of the experiment.", MB_OK);
        }
        else {
            s_time = getTimeInDisplayableFormat((int)minutes, (int)seconds);
            experiment_duration_seconds = ((int)minutes) * 60 + (int)seconds;
            SetMiniXHV(50);
            Sleep(100);
            SetMiniXCurrent(79);
            Sleep(100);
            SendMiniXCommand((byte)mxcSetHVandCurrent);
            Sleep(1000);
            ReadMiniXSettings(&MiniXSettings);  // read corrected values back
            DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT, MiniXSettings.HighVoltage_kV, 0);
            DisplayDouble(IDC_SETCURRENTCONTROLEDIT, MiniXSettings.Current_uA, 0);
            CString msg;
            msg.Format("Experiment Description:\nDuration =\t%s.\nVoltage =\t%g kV.\nCurrent =\t%g uA.\n\nClick 'OK' to BEGIN RADIATION.",
                s_time, GetWindowDouble(IDC_SETHIGHVOLTAGECONTROLEDIT), GetWindowDouble(IDC_SETCURRENTCONTROLEDIT));
            int msg_check = AfxMessageBox(msg, MB_YESNO);
            if (msg_check == IDYES) {
                is_warming_up = true;
                is_experiment_running = true;
                is_experiment_warmup = true;
                SendMiniXCommand((byte)mxcHVOn);
                Sleep(100);
                TimerControl(tmrRuntime_EventId, true);
                TimerControl(tmrWarmUp_EventId, true);
            }
        }
    }
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnBnClickedButtonStopExperiment()
{
    indDisableMonitorCmds = true;
    if (isMiniXDlg()) {
        SendMiniXCommand((byte)mxcHVOff);
        Sleep(100);
        TimerControl(tmrDuration_EventId, false);
        TimerControl(tmrRuntime_EventId, false);
        is_warming_up = false;
        is_experiment_running = false;
        is_experiment_warmup = false;
        experiment_count_seconds = 0;
        GetDlgItem(IDC_DURATION_TIME_TEXT)->SetWindowText("00:00");
        AfxMessageBox("Experiment Cancelled.", MB_OK);
    }
    indDisableMonitorCmds = false;
}
