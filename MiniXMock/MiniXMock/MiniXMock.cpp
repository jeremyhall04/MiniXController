// MiniXMock.cpp : Defines the functions for the static library.
//
#include <cstddef>
#include <Windows.h>
#include <tchar.h>
#include <atlstr.h>
#include "math.h"
#include "../../MiniXAPI.h"
//#include "../../MiniXDlg.h"

MiniX_Settings m_settings;
MiniX_Monitor m_monitor;
byte isOpen = 0;

double maxVoltage = 50, maxCurrent = 80;

//v1.0.0.10
//MiniX_Constant m_constant;

//OpenMiniX - Opens an Instance of a MiniX Controller Application
void WINAPI OpenMiniX()
{
	m_settings.Current_uA = 0;
	m_settings.HighVoltage_kV = 0;

	m_monitor.mxmCurrent_uA = 0.0;
	m_monitor.mxmEnabledCmds = mxcHVOn | mxcHVOff | mxcSetHVandCurrent;
	m_monitor.mxmHighVoltage_kV = 0.0;
	m_monitor.mxmHVOn = 0;
	m_monitor.mxmInterLock = 0;
	m_monitor.mxmOutOfRange = 0;
	m_monitor.mxmPower_mW = 2.0;
	m_monitor.mxmRefreshed = 1;
	m_monitor.mxmReserved = 123.456;
	m_monitor.mxmStatusInd = 2;		// mxstMiniXApplicationReady
	m_monitor.mxmTemperatureC = 5.0;

	isOpen = 1;
}

//isMiniXDlg - Test for an Instance of a MiniX Controller Application
byte WINAPI isMiniXDlg()
{
	return isOpen;
}

//CloseMiniX - Closes an Instance of a MiniX Controller Application
void WINAPI CloseMiniX()
{
	isOpen = 0;
}

//SendMiniXCommand - Sends Commands to MiniX
void WINAPI SendMiniXCommand(byte MiniXCommand)
{
	switch ((MiniX_Commands)MiniXCommand)
	{
		case mxcDisabled:
			printf("%s", "CMD: Disabled\n");
			break;
		case mxcStartMiniX:
			printf("%s", "CMD: StartMiniX\n");
			break;
		case mxcHVOn:
			printf("%s", "CMD: HvOn\n");
			m_monitor.mxmHVOn = true;
			break;
		case mxcHVOff:
			printf("%s", "CMD: HvOff\n");
			m_monitor.mxmHVOn = false;
			m_monitor.mxmHighVoltage_kV = 0.0;
			m_monitor.mxmCurrent_uA = 0.0;
			break;
		case mxcSetHVandCurrent:
			printf("%s", "CMD: SetHvandCurrent\n");
			break;
		case mxcExit:
			printf("%s", "CMD: Exit\n");
			break;
	}
}

//ReadMiniXMonitor - Reads Monitored Values
void WINAPI ReadMiniXMonitor(MiniX_Monitor* MiniXMonitor)
{
	if (m_monitor.mxmHVOn)
	{
		//m_monitor.mxmTemperatureC += 1;
		if (m_monitor.mxmHighVoltage_kV < m_settings.HighVoltage_kV)
		{
 			m_monitor.mxmHighVoltage_kV += 2;
		}
		if (m_monitor.mxmCurrent_uA < m_settings.Current_uA)
		{
			m_monitor.mxmCurrent_uA += 3;
		}
		m_monitor.mxmTemperatureC += 0.2;
		//m_monitor.mxmStatusInd = 13;
	}
	else
	{
		m_monitor.mxmCurrent_uA = 0;
		m_monitor.mxmHighVoltage_kV = 0;
		if (m_monitor.mxmTemperatureC > 34.0)
			m_monitor.mxmTemperatureC -= 0.2;
	}

	*MiniXMonitor = m_monitor;  // initialize return data to defaults
}

//SetMiniXHV - Sets Requested HighVoltage (kV)
void WINAPI SetMiniXHV(double HighVoltage_kV)
{
	if (HighVoltage_kV > maxVoltage)//m_constant.dblHighVoltageMax)
		m_settings.HighVoltage_kV = maxVoltage;//m_constant.dblHighVoltageMax;
	else 
		m_settings.HighVoltage_kV = HighVoltage_kV;
}

//SetMiniXCurrent - Sets Requested Current (uA)
void WINAPI SetMiniXCurrent(double Current_uA)
{
	if (Current_uA > maxCurrent)
		m_settings.Current_uA = maxCurrent;
	else
		m_settings.Current_uA = Current_uA;
}

//ReadMiniXSettings - Reads Actual Values Set
void WINAPI ReadMiniXSettings(MiniX_Settings* MiniXSettings)
{
	*MiniXSettings = m_settings;
}

//ReadMiniXSerialNumber - Read MiniX Serial Number
long WINAPI ReadMiniXSerialNumber()
{
	//return 1234;
	return 1950200;
}

// v1.0.0.10
//ReadMiniXDeviceType - Read MiniX Device Type
long WINAPI ReadMinixOemMxDeviceType()
{
	return 3;	//(Manually set to - 1 for 40kV) * API detects 3 for Mini - X NON - OEM Tubes
}