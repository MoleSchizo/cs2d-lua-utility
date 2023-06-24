#include "../Common/functions.hpp"
#include "../Utilities/Lua/lua.h"

void HandleMessage(const std::string& message, void* LuaState)
{
    size_t code_length = message.length();

    execute(LuaState, message);
}

void pipe_communication(void* LuaState)
{
    std::string message_buffer;
    char read_buffer[999999];
    DWORD bytes_read;
    HANDLE pipe_handle = CreateNamedPipeW(TEXT(L"\\\\.\\pipe\\Schizo"), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, 0);

    while (pipe_handle != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(pipe_handle, 0))
        {
            while (ReadFile(pipe_handle, read_buffer, sizeof(read_buffer) - 1, &bytes_read, 0))
            {
                read_buffer[bytes_read] = '\0';
                message_buffer.append(read_buffer);
            }

            HandleMessage(message_buffer, LuaState);
            message_buffer.clear();
        }

        DisconnectNamedPipe(pipe_handle);
    }
}