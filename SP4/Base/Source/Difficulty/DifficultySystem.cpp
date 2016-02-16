#include <iostream>
#include <fstream>
using namespace std;
#include "DifficultySystem.h"
extern "C" {
#include "..\Lua\lua.h"
#include "..\Lua\lualib.h"
#include "..\Lua\lauxlib.h"
}

CDifficultySystem::CDifficultySystem(void)
{
	currentDifficultyUnlocked = EASY;
}

CDifficultySystem::~CDifficultySystem(void)
{
}