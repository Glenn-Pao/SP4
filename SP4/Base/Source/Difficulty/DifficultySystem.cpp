#include "DifficultySystem.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "..\UsingLua.h"

CDifficultySystem::CDifficultySystem(void)
	: currentDifficultyUnlocked(EASY)
{
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

/********************************************************************************
Read the file and store variables
********************************************************************************/
void CDifficultySystem::readFile()
{
	UseLuaFiles L;

	L.ReadFiles("Lua//difficultyInfo.lua");
	currentDifficultyUnlocked = L.DoLuaInt("currentDifficultyUnlocked");
}

/********************************************************************************
Write the file and store variables
********************************************************************************/
void CDifficultySystem::writeToFile()
{
	ofstream myfile("Lua//difficultyInfo.lua");
	if (myfile.is_open())
	{
		myfile << "--[[" << endl;
		myfile << "Use these values to initialise the difficulty variables" << endl;
		myfile << "]]--" << endl;
		myfile << endl;
		myfile << "currentDifficultyUnlocked = " << currentDifficultyUnlocked << endl;
	}
}