#include "DifficultySystem.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "..\..\UsingLua.h"

CDifficultySystem::CDifficultySystem(void)
	: currentDifficultyUnlocked(EASY)
{
}

CDifficultySystem::~CDifficultySystem(void)
{
}

/********************************************************************************
Unlocked Difficulty
********************************************************************************/
void CDifficultySystem::UnlockedDifficulty()
{
	if (currentDifficultyUnlocked != NUM_OF_DIFFICULTIES)
		currentDifficultyUnlocked++;
}