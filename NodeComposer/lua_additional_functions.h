#pragma once

#include "base.h"

namespace ncp
{
	void setLuaFunction(lua_State*);

	int _Lua_getTPQ(lua_State*);
	int _Lua_getTempo(lua_State*);
}