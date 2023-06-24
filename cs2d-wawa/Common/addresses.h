#pragma once

namespace lua
{
    namespace addresses
    {
        constexpr auto lua_call = 0x403A40;
        constexpr auto luaL_loadbuffer = 0x402770;
        constexpr auto lua_newthread = 0x402B00;
        constexpr auto lua_pushvalue = 0x402D60;
        constexpr auto lua_pcall = 0x403AB0;
        constexpr auto lua_newstate = 0x404370;
        constexpr auto luaL_newstate = 0x402830;
        constexpr auto lua_getfield = 0x403540;
        constexpr auto lua_pushstring = 0x4032F0;
        constexpr auto lua_tolstring = 0x403050;
        constexpr auto luaL_openlibs = 0x402870;
        constexpr auto lua_next = 0x403D80;
        constexpr auto lua_gettop = 0x402B70;
        constexpr auto lua_pushnil = 0x403230;
        constexpr auto lua_setfield = 0x403790;
        constexpr auto lua_pushcfunction = 0x4033E0;

        constexpr auto game_lua_state = 0x8B7070;

        // needed ones for execution are only
        // game lua state, luaL_loadbuffer, lua_pcall
    }
}