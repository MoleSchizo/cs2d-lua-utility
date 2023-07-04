#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstddef>

class Hook {
public:
    explicit Hook();

    uintptr_t hook(uintptr_t to_hook, uintptr_t to_replace, int32_t instr_nops = 0);
    void unhook(uintptr_t to_unhook);
    void yield(uintptr_t to_yield);
    void resume(uintptr_t to_resume);

private:
    uintptr_t target_function; // detour

    uint8_t* function_o; // old
    uint8_t* function_t; // backup

    size_t function_size;

    void detour(uintptr_t to_hook, uintptr_t to_replace, size_t length);
};
