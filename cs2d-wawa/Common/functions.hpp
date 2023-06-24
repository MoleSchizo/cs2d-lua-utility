#pragma once

#include "addresses.h"

typedef int (*lua_CFunction)(void* L);

typedef void* (*luaL_newstate_typedef)();
luaL_newstate_typedef luaL_newstate = reinterpret_cast<luaL_newstate_typedef>(lua::addresses::luaL_newstate);

typedef void* (*lua_newthread_typedef)(void*);
lua_newthread_typedef lua_newthread = reinterpret_cast<lua_newthread_typedef>(lua::addresses::lua_newthread);

typedef int (*lua_getfield_typedef)(void*, int, const char*);
lua_getfield_typedef lua_getfield = reinterpret_cast<lua_getfield_typedef>(lua::addresses::lua_getfield);

typedef void (*lua_pushstring_typedef)(void*, const char*);
lua_pushstring_typedef lua_pushstring = reinterpret_cast<lua_pushstring_typedef>(lua::addresses::lua_pushstring);

typedef int (*lua_pcall_typedef)(void*, int, int, int);
lua_pcall_typedef lua_pcall = reinterpret_cast<lua_pcall_typedef>(lua::addresses::lua_pcall);

typedef int (*luaL_loadbuffer_typedef)(void*, const char*, size_t, const char*);
luaL_loadbuffer_typedef luaL_loadbuffer = reinterpret_cast<luaL_loadbuffer_typedef>(lua::addresses::luaL_loadbuffer);

typedef const char* (*lua_tolstring_typedef)(void*, int, size_t*);
lua_tolstring_typedef lua_tolstring = reinterpret_cast<lua_tolstring_typedef>(lua::addresses::lua_tolstring);

typedef int (*lua_gettop_typedef)(void*);
lua_gettop_typedef lua_gettop = reinterpret_cast<lua_gettop_typedef>(lua::addresses::lua_gettop);

typedef void (*lua_pushcfunction_typedef)(void*, lua_CFunction);
lua_pushcfunction_typedef lua_pushcfunction = reinterpret_cast<lua_pushcfunction_typedef>(lua::addresses::lua_pushcfunction);

typedef void (*lua_setfield_typedef)(void*, int, const char*);
lua_setfield_typedef lua_setfield = reinterpret_cast<lua_setfield_typedef>(lua::addresses::lua_setfield);

int luaL_loadstring(void* L, const char* s) {
	return luaL_loadbuffer(L, s, strlen(s), s);
}

void execute(void* LuaState, const std::string& code) {
    if (LuaState != nullptr) {
        void* L = static_cast<void*>(LuaState);

        int result = luaL_loadstring(L, code.c_str()) || lua_pcall(L, 0, 0, 0);
        if (result != 0) {
            std::cout << "Error: " << lua_tolstring(L, -1, nullptr) << std::endl;
        }
    }
    else {
        std::cout << "Failed to get game Lua state" << std::endl;
    }
}

void* get_state()
{
    void* game_lua_state = nullptr;

    DWORD_PTR* lua_state = reinterpret_cast<DWORD_PTR*>(lua::addresses::game_lua_state);
    game_lua_state = reinterpret_cast<void*>(*lua_state);

    return game_lua_state;
}