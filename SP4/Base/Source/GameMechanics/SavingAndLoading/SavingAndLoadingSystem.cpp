#include "SavingAndLoadingSystem.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "..\..\UsingLua.h"

CSavingAndLoadingSystem::CSavingAndLoadingSystem(void)
{
}

CSavingAndLoadingSystem::~CSavingAndLoadingSystem(void)
{
}

/********************************************************************************
Read the file and store variables
********************************************************************************/
void CSavingAndLoadingSystem::readFile()
{
	UseLuaFiles L;
/*
	L.ReadFiles("Lua//difficultyInfo.lua");
	currentDifficultyUnlocked = L.DoLuaInt("currentDifficultyUnlocked");*/
}

/********************************************************************************
Write the file and store variables
********************************************************************************/
void CSavingAndLoadingSystem::writeToFile()
{
	/*ofstream myfile("Lua//difficultyInfo.lua");
	if (myfile.is_open())
	{
		myfile << "--[[" << endl;
		myfile << "Use these values to initialise the difficulty variables" << endl;
		myfile << "]]--" << endl;
		myfile << endl;
		myfile << "currentDifficultyUnlocked = " << currentDifficultyUnlocked << endl;
	}*/
}