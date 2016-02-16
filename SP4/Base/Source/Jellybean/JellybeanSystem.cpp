#include <iostream>
#include <fstream>
using namespace std;
#include "JellybeanSystem.h"
#include "..\MeshBuilder.h"

extern "C" {
#include "..\Lua\lua.h"
#include "..\Lua\lualib.h"
#include "..\Lua\lauxlib.h"
}

CJellybeanSystem::CJellybeanSystem(void)
{
	readFile();
}

CJellybeanSystem::~CJellybeanSystem(void)
{
	writeToFile();
}


void CJellybeanSystem::readFile()
{
	lua_State *L = lua_open();

	//Read a value from the lua text file
	luaL_openlibs(L);

	if (luaL_loadfile(L, "Lua//jellybeanInfo.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}

	// Number of jelly beans player have
	lua_getglobal(L, "noOfJellybeans");
	if (!lua_isnumber(L, -1)) {
		printf("`noOfJellybeans' should be a number\n");
	}
	noOfJellybeans = (int)lua_tointeger(L, -1);

	// Min number of jelly beans player can deposit in easy
	lua_getglobal(L, "easyMinDeposit");
	if (!lua_isnumber(L, -1)) {
		printf("`easyMinDeposit' should be a number\n");
	}
	minNoOfJellybeansDeposited[EASY] = (int)lua_tointeger(L, -1);

	// Min number of jelly beans player can deposit in medium
	lua_getglobal(L, "mediumMinDeposit");
	if (!lua_isnumber(L, -1)) {
		printf("`mediumMinDeposit' should be a number\n");
	}
	minNoOfJellybeansDeposited[MEDIUM] = (int)lua_tointeger(L, -1);

	// Min number of jelly beans player can deposit in hard
	lua_getglobal(L, "hardMinDeposit");
	if (!lua_isnumber(L, -1)) {
		printf("`hardMinDeposit' should be a number\n");
	}
	minNoOfJellybeansDeposited[HARD] = (int)lua_tointeger(L, -1);

	lua_close(L);
}

void CJellybeanSystem::writeToFile()
{
	ofstream myfile("Lua//jellybeanInfo.lua");
	if (myfile.is_open())
	{
		myfile << "--[[" << endl;
		myfile << "Use these values to initialise the jellybeans variables" << endl;
		myfile << "]]--" << endl;
		myfile << endl;
		myfile << "noOfJellybeans = " << noOfJellybeans << endl;
		myfile << "easyMinDeposit = " << minNoOfJellybeansDeposited[EASY] << endl;
		myfile << "mediumMinDeposit = " << minNoOfJellybeansDeposited[MEDIUM] << endl;
		myfile << "hardMinDeposit = " << minNoOfJellybeansDeposited[HARD] << endl;
	}
}

bool CJellybeanSystem::DepositJellybeans(int depositAmount, DIFFICULTY difficulty)
{
	// Check min bet
	switch (difficulty)
	{
	case EASY:
		if (depositAmount < minNoOfJellybeansDeposited[EASY])
		{
			return false;
		}
		break;
	case MEDIUM:
		if (depositAmount < minNoOfJellybeansDeposited[MEDIUM])
		{
			return false;
		}
		break;
	case HARD:
		if (depositAmount < minNoOfJellybeansDeposited[HARD])
		{
			return false;
		}
		break;
	}
	// Check jellybeans left
	if (noOfJellybeans >= depositAmount)
	{
		noOfJellybeansDeposited = depositAmount;
		noOfJellybeans -= depositAmount;
		return true;
	}

	return false;
}


void CJellybeanSystem::WithdrawJellybeans(bool won)
{
	if (won)
	{
		noOfJellybeans += noOfJellybeansDeposited * 2;
	}
	else
	{
		noOfJellybeansDeposited = 0;
	}
	writeToFile();
}