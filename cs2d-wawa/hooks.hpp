#include <iostream>
#include <Windows.h>
#include "Utilities/anya-hook/anya_hook.hpp"

Hook context{};

typedef BOOL(WINAPI* IsDebuggerPresent_t)(VOID);
IsDebuggerPresent_t OriginalIsDebuggerPresent;

std::uintptr_t original_messageboxa = 0;

BOOL WINAPI hooked_isdebugger_present(VOID)
{
    return FALSE;
}

std::int32_t __stdcall hooked_messageboxa(HWND hwnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    return 0;
}

namespace hooks
{
    void initiate_hooks()
    {
        const auto module = GetModuleHandleA("user32.dll");
        if (module)
        {
            const auto messageboxa = reinterpret_cast<std::uintptr_t>(GetProcAddress(module, "MessageBoxA"));
            if (messageboxa)
            {
                original_messageboxa = context.hook(messageboxa, reinterpret_cast<std::uintptr_t>(&hooked_messageboxa));
            }
        }

        HMODULE kernel32Module = GetModuleHandleA("kernel32.dll");
        if (kernel32Module)
        {
            uintptr_t idp_addr = reinterpret_cast<uintptr_t>(GetProcAddress(kernel32Module, "IsDebuggerPresent"));
            if (idp_addr)
            {
                uintptr_t hooked_idp_addr = reinterpret_cast<uintptr_t>(&hooked_isdebugger_present);
                OriginalIsDebuggerPresent = reinterpret_cast<IsDebuggerPresent_t>(context.hook(idp_addr, hooked_idp_addr));
            }
        }

        std::cout << "Initiated hooks" << std::endl;
    }

    void stop_hooks()
    {
        if (original_messageboxa != 0)
        {
            context.unhook(original_messageboxa);
            original_messageboxa = 0;
        }

        if (OriginalIsDebuggerPresent)
        {
            HMODULE kernel32Module = GetModuleHandleA("kernel32.dll");
            uintptr_t IsDebuggerPresentAddr = reinterpret_cast<uintptr_t>(GetProcAddress(kernel32Module, "IsDebuggerPresent"));
            context.unhook(IsDebuggerPresentAddr);
            OriginalIsDebuggerPresent = nullptr;
        }
    }
}