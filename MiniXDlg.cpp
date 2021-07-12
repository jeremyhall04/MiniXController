// MiniXDlg.cpp : implementation file
#include "stdafx.h"
#include "MiniX.h"
#include "MiniXDlg.h"
#include  <math.h>
#include "VersionInfo.h"
#include "MiniXAPI.h"

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

COLORREF    crBkGnd;
COLORREF    crBkGndActive;
COLORREF    crText; 
COLORREF    crTextActive; 


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

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
	return TRUE;  // return TRUE unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CMiniXDlg dialog

CMiniXDlg::CMiniXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiniXDlg)

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
END_MESSAGE_MAP()
// CMiniXDlg message handlers

void CMiniXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
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

void CMiniXDlg::InterlockDisplay(InterlockViewState ILockView) {
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
            }
        } else {
            GetDlgItem(IDC_SERIAL_NUMBER)->SetWindowText("");
        }
    }
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
            SendMiniXCommand((byte)mxcHVOn);
            Sleep(100);
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
    }
    indDisableMonitorCmds = false;
}

void CMiniXDlg::OnClose()
{
	DestroyWindow();
}

void CMiniXDlg::OnCancel()
{
	DestroyWindow();
	//CDialog::OnCancel();	// don't use - failsafe must shutdown HV
}

void CMiniXDlg::OnDestroy()
{
    if (isMiniXDlg()) {
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
            EnableMiniX_Commands(MiniXMonitor.mxmEnabledCmds);
            if (MiniXMonitor.mxmHVOn) {   
                if (! tmrXRayOn_Enabled) { TimerControl(tmrXRayOn_EventId,true); };
            } else {
                if (tmrXRayOn_Enabled) { TimerControl(tmrXRayOn_EventId,false); };
                if (GetDlgItem(IDC_XRAY_ON_PIC)->IsWindowVisible()) {
                    GetDlgItem(IDC_XRAY_ON_PIC)->ShowWindow(SW_HIDE);
                };
                if (GetDlgItem(IDC_XRAY_ON_LABEL)->IsWindowVisible()) {
                    GetDlgItem(IDC_XRAY_ON_LABEL)->ShowWindow(SW_HIDE);
                };
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






















