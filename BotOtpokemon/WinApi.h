#pragma once

#include <windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <map>

class WinApi
{
public:
	DWORD static GetProccessId(const WCHAR* processName);
	HWND static FindWindowByProcessId(DWORD processId);
	uintptr_t static GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
};

