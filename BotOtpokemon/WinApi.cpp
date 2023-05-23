#include "WinApi.h"

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam)
{
    auto* data = reinterpret_cast<std::pair<DWORD, HWND>*>(lParam);
    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId == data->first)
    {
        data->second = hwnd;
        return FALSE;
    }
    return TRUE;
}

DWORD WinApi::GetProccessId(const WCHAR* processName) {
    DWORD baseAddress = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &processEntry))
        {
            while (Process32Next(snapshot, &processEntry))
            {
                if (wcscmp(processEntry.szExeFile, processName) == 0)
                {
                    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
                    if (processHandle != NULL)
                    {
                        baseAddress = (DWORD)processEntry.th32ProcessID;
                        CloseHandle(processHandle);
                    }
                    break;
                }
            }
        }

        CloseHandle(snapshot);
    }

    return baseAddress;
}

HWND WinApi::FindWindowByProcessId(DWORD processId)
{
    std::pair<DWORD, HWND> data(processId, nullptr);
    EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&data));
    return data.second;
}

uintptr_t WinApi::GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;

        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }

            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}