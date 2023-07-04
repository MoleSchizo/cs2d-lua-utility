#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>

#include "Communication/communication.hpp"
#include "Utilities/Console/console.hpp"

#include "Utilities/Lua/lua.h"

#include "Common/functions.hpp"
#include "Common/addresses.h"

#include "Utilities/anya-hook/anya_hook.hpp"

#include "hooks.hpp"

void main() {
    console::open_console();
    hooks::initiate_hooks();

    std::cout << "Hi" << std::endl;

    void* LuaState = get_state();
    if (LuaState != nullptr)
    {
        execute(LuaState, "print('hello i haxor ')");

        std::string input;

        while (input != "exit") {
            execute(LuaState, input);

            std::cout << "> ";
            std::getline(std::cin, input);
        }

        std::thread pipe_thread(pipe_communication, LuaState);
        pipe_thread.detach();
    }
    else
    {
        std::cout << "Failed to get game lua state" << std::endl;
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {

    if (fdwReason == DLL_PROCESS_ATTACH) {
        std::thread{ main }.detach();
    }

    return TRUE;
}