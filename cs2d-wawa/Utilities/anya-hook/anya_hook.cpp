#include "anya_hook.hpp"
#include "hde32_disasm.hpp"

Hook::Hook()
    : function_o(nullptr), function_t(nullptr), function_size(0) {}

void Hook::detour(const uintptr_t to_hook, const uintptr_t to_replace, const size_t length) {
    DWORD old_protect{ 0 };

    VirtualProtect(reinterpret_cast<void*>(to_hook), length, PAGE_EXECUTE_READWRITE, &old_protect);

    function_o = reinterpret_cast<uint8_t*>(std::malloc(length));

    std::memcpy(function_o, reinterpret_cast<void*>(to_hook), length);
    std::memset(reinterpret_cast<void*>(to_hook), 0x90, length);

    *reinterpret_cast<uint8_t*>(to_hook) = 0xE9;
    *reinterpret_cast<uintptr_t*>(to_hook + 1) = (to_replace - to_hook - 5);

    VirtualProtect(reinterpret_cast<void*>(to_hook), length, old_protect, &old_protect);
}

uintptr_t Hook::hook(const uintptr_t to_hook, const uintptr_t to_replace, int32_t instr_nops) {
    uintptr_t at = to_hook;
    size_t nops = 0;

    while (true) {
        hde32s disasm{ 0 };
        hde32_disasm(reinterpret_cast<void*>(at), &disasm);

        at += disasm.len;
        nops += disasm.len;

        if (nops > 5) {
            if (instr_nops) {
                instr_nops--;
                continue;
            }

            break;
        }
    }

    nops -= 5;
    function_size = (nops + 5);

    const uintptr_t detour_address = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, function_size + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

    std::memmove(reinterpret_cast<void*>(detour_address), reinterpret_cast<void*>(to_hook), function_size);

    *reinterpret_cast<uint8_t*>(detour_address + function_size) = 0xE9;

    const uintptr_t rel_addr = (to_hook - detour_address - 5);
    *reinterpret_cast<uint32_t*>(detour_address + function_size + 1) = rel_addr;

    detour(to_hook, to_replace, function_size);
    return detour_address;
}

void Hook::unhook(uintptr_t to_unhook) {
    DWORD old_protect{ 0 };

    VirtualProtect(reinterpret_cast<void*>(to_unhook), function_size, PAGE_EXECUTE_READWRITE, &old_protect);
    std::memcpy(reinterpret_cast<void*>(to_unhook), function_o, function_size);
    VirtualProtect(reinterpret_cast<void*>(to_unhook), function_size, old_protect, &old_protect);
    VirtualFree(reinterpret_cast<void*>(to_unhook), 0, MEM_FREE);

    std::free(function_o);
    to_unhook = 0;
}

void Hook::yield(uintptr_t to_yield) {
    DWORD old_protect{ 0 };

    VirtualProtect(reinterpret_cast<void*>(to_yield), function_size, PAGE_EXECUTE_READWRITE, &old_protect);
    function_t = reinterpret_cast<uint8_t*>(VirtualAlloc(nullptr, function_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

    std::memcpy(function_t, reinterpret_cast<void*>(to_yield), function_size);
    std::memcpy(reinterpret_cast<void*>(to_yield), function_o, function_size);

    VirtualProtect(reinterpret_cast<void*>(to_yield), function_size, old_protect, &old_protect);
}

void Hook::resume(uintptr_t to_resume) {
    DWORD old_protect{ 0 };

    VirtualProtect(reinterpret_cast<void*>(to_resume), function_size, PAGE_EXECUTE_READWRITE, &old_protect);
    std::memcpy(reinterpret_cast<void*>(to_resume), function_t, function_size);
    VirtualProtect(reinterpret_cast<void*>(to_resume), function_size, old_protect, &old_protect);

    VirtualFree(function_t, 0, MEM_RELEASE);
}
