#pragma once
#include <Windows.h>

namespace console
{
    void open_console()
    {
        const auto lib = LoadLibraryW(L"KERNEL32.dll");
        const auto free_console = reinterpret_cast<std::uintptr_t>(GetProcAddress(lib, "FreeConsole"));

        if (free_console)
        {
            static auto jmp = free_console + 0x6;
            DWORD old{};

            VirtualProtect(reinterpret_cast<void*>(free_console), 0x6, PAGE_EXECUTE_READWRITE, &old);
            *reinterpret_cast<std::uintptr_t**>(free_console + 0x2) = &jmp;
            *reinterpret_cast<std::uint8_t*>(free_console + 0x6) = 0xC3;
            VirtualProtect(reinterpret_cast<void*>(free_console), 0x6, old, &old);
        }

        AllocConsole();

        FILE* file_stream{};

        freopen_s(&file_stream, "CONIN$", "r", stdin);
        freopen_s(&file_stream, "CONOUT$", "w", stdout);
        freopen_s(&file_stream, "CONOUT$", "w", stderr);

        SetConsoleTitleA("Schizo");

        const auto ConsoleHandle = GetConsoleWindow();
        ::SetWindowPos(ConsoleHandle, HWND_TOP, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        ::ShowWindow(ConsoleHandle, SW_NORMAL);
    }
}