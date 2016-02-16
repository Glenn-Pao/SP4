#ifndef USINGLUA_H
#define USINGLUA_H

extern "C" {
#include "..\source\Lua\lua.h"
#include "..\source\Lua\lualib.h"
#include "..\source\Lua\lauxlib.h"
}
#include <string>

class UseLuaFiles
{
public:
	UseLuaFiles();
	~UseLuaFiles();

	void ReadFiles(const char* filename);

	std::string DoLuaString(std::string);
	int DoLuaInt(std::string);
	bool DoLuaBool(std::string);

private:
	lua_State *L = lua_open();
};
#endif