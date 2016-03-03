#include "UsingLua.h"
#include <string>

UseLuaFiles::UseLuaFiles()
{
	luaL_openlibs(L);
}

UseLuaFiles::~UseLuaFiles()
{
	lua_close(L);
}

void UseLuaFiles::ReadFiles(const char* filename)
{
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}
}

std::string UseLuaFiles::DoLuaString(std::string temp_StringToProcess)
{
	const char* temp = temp_StringToProcess.c_str();
	lua_getglobal(L, temp);						//Stack 1

	if (!lua_isstring(L, -1))
	{
		printf("Invalid string");
	}

	std::string ReturnThisString = (std::string)lua_tostring(L, -1);

	return ReturnThisString;
}

int UseLuaFiles::DoLuaInt(std::string temp_StringToProcess)
{
	const char* temp = temp_StringToProcess.c_str();
	lua_getglobal(L, temp);
	if (!lua_isnumber(L, -1))
	{
		printf("Invalid Number");
	}

	int ReturnThisInt = (int)lua_tointeger(L, -1);

	return ReturnThisInt;
}

float UseLuaFiles::DoLuaFloat(std::string temp_StringToProcess)
{
	const char* temp = temp_StringToProcess.c_str();
	lua_getglobal(L, temp);
	if (!lua_isnumber(L, -1))
	{
		printf("Invalid Number");
	}

	float ReturnThisFloat = (float)lua_tonumber(L, -1);

	return ReturnThisFloat;
}