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

/********************************************************************************
Unlocked difficulty
********************************************************************************/
void CDifficultySystem::UnlockedDifficulty()
{
	if (currentDifficultyUnlocked != NUM_OF_DIFFICULTIES)
		currentDifficultyUnlocked++;
}