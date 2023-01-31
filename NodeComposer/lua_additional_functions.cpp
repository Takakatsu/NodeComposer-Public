#include "base.h"

namespace ncp
{
	void setLuaFunction(lua_State* L)
	{
		lua_pushcfunction(L, _Lua_getTPQ);
		lua_setglobal(L, "getTPQ");
		lua_pushcfunction(L, _Lua_getTempo);
		lua_setglobal(L, "getTempo");
	}

	int _Lua_getTPQ(lua_State* L)
	{
		lua_pushnumber(L, tpq);
		return 1;
	}

	int _Lua_getTempo(lua_State* L)
	{
		lua_pushnumber(L, tempo);
		return 1;
	}
}