#include <iostream>
#include "WinApi.h"
#include <vector>

int main()
{
    const WCHAR* proccessName = L"otpdx.exe";

    DWORD processId = WinApi::GetProccessId(proccessName);
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);

    if (hProcess == NULL)
    {
        std::cout << "Falha ao abrir o processo." << std::endl;
        return EXIT_FAILURE;
    }

    uintptr_t clientBaseAddr = WinApi::GetModuleBaseAddress(processId, proccessName);
    uint32_t* addressPlayerTarget = ((uint32_t*)(clientBaseAddr + 0xA60FC4));
    uint32_t* addressPlayerPokemon = ((uint32_t*)(clientBaseAddr + 0xA60F48));

    HWND targetWindow = WinApi::FindWindowByProcessId(processId);

    if (targetWindow == nullptr)
    {
        std::cout << "O programa alvo não está aberto." << std::endl;
        return EXIT_FAILURE;
    }
   
    SetForegroundWindow(targetWindow);

    while (true) {
        uint32_t playerTarget = 0;
    
        ReadProcessMemory(hProcess, (LPVOID)addressPlayerTarget, &playerTarget, sizeof(playerTarget), nullptr);

        if (playerTarget != 0) {
            std::vector<int> functionKeys = { VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12 };

            for (int keyCode : functionKeys)
            {
                keybd_event(keyCode, 0, 0, 0);
                keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
            }

            Sleep(100);
        }
    }
    
    return EXIT_SUCCESS;
}
