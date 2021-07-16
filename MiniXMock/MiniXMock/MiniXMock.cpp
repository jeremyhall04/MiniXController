// MiniXMock.cpp : Defines the functions for the static library.
//
#include <cstddef>
#include <Windows.h>
#include <tchar.h>
#include <atlstr.h>
#include "math.h"
#include "../../MiniXAPI.h"

MiniX_Settings m_settings;
MiniX_Monitor m_monitor;
byte isOpen = 0;

double m_defaultTemp = 25.0;

//OpenMiniX - Opens an Instance of a MiniX Controller Application
void WINAPI OpenMiniX()
{
	m_settings.Current_uA = 6;
	m_settings.HighVoltage_kV = 7;

	m_monitor.mxmCurrent_uA = 0.0;
	m_monitor.mxmEnabledCmds = mxcHVOn | mxcHVOff | mxcSetHVandCurrent;
	m_monitor.mxmHighVoltage_kV = 0.0;
	m_monitor.mxmHVOn = 0;
	m_monitor.mxmInterLock = 0;
	m_monitor.mxmOutOfRange = 0;
	m_monitor.mxmPower_mW = 3.0;
	m_monitor.mxmRefreshed = 1;
	m_monitor.mxmReserved = 123.456;
	m_monitor.mxmStatusInd = 0;
	m_monitor.mxmTemperatureC = m_defaultTemp;

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
		//// if xray is on, randomly vary the monitored voltage, current and board temp
		double delta = (double(rand() % 100) - 50) / 100.0;
		//m_monitor.mxmHighVoltage_kV = m_settings.HighVoltage_kV + delta;

		//delta = (double(rand() % 100) - 50) / 100.0;
		//m_monitor.mxmCurrent_uA = m_settings.Current_uA + delta;

		delta = double(rand() % 100) / 10.0;
		m_monitor.mxmTemperatureC = m_defaultTemp + delta;

		// Increase values from previous to input
		m_settings.HighVoltage_kV += 4;
		m_settings.Current_uA += 5;
		m_monitor.mxmHighVoltage_kV = m_settings.HighVoltage_kV;
		m_monitor.mxmCurrent_uA = m_settings.Current_uA;
	}
	else
	{
		m_monitor.mxmCurrent_uA = 0;
		m_monitor.mxmHighVoltage_kV = 0;

		if(m_monitor.mxmTemperatureC > m_defaultTemp)
			m_monitor.mxmTemperatureC -= 1;
	}

	*MiniXMonitor = m_monitor;  // initialize return data to defaults
}

//SetMiniXHV - Sets Requested HighVoltage (kV)
void WINAPI SetMiniXHV(double HighVoltage_kV)
{
	m_settings.HighVoltage_kV = HighVoltage_kV;
}

//SetMiniXCurrent - Sets Requested Current (uA)
void WINAPI SetMiniXCurrent(double Current_uA)
{
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
	return 1234;
}
