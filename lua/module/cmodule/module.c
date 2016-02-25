#include <stdio.h>
#include <lauxlib.h>
#include <lua.h>

static int cfunction(lua_State *L)
{
	printf("c function\n");
	return 1;
}

int luaopen_cmodule(lua_State *L)
{
	luaL_Reg l[] = {
		{ "cfunction", cfunction },
		{ NULL, NULL },
	};
	
	luaL_newlib(L, l);

	return 1;
}

