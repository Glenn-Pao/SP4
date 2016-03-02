#include "GameInfo.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "..\..\UsingLua.h"

CGameInfo::CGameInfo(const char* fileName)
	: fileName(fileName)
{
	ResetData();
}

CGameInfo::~CGameInfo(void)
{
}

/********************************************************************************
Reset information of the Game
********************************************************************************/
void CGameInfo::ResetData()
{
	// if new
	ifNew = true;

	// Player's Infomation
	heroPosition.Set(0, 0);
	heroMapOffset.Set(0, 0);

	for (int i = 0; i < 4; i++)
	{
		DifficultySystems[i].setCurrentDifficultyUnlocked(CDifficultySystem::TUTORIAL);
	}

	// Guardian
	guardianCleared = false;
	musicActive = true;
}

/********************************************************************************
Read the file and load the game
********************************************************************************/
void CGameInfo::LoadFile()
{
	UseLuaFiles L;

	L.ReadFiles(fileName);

	// if new
	ifNew = L.DoLuaInt("ifNew");

	if (ifNew == false)
	{
		// Player's Infomation
		heroPosition.x = L.DoLuaFloat("heroPositionX");
		heroPosition.y = L.DoLuaFloat("heroPositionY");
		heroAnimationDir = L.DoLuaInt("heroAnimationDir");
		jellybean.SetNumOfJellybeans(L.DoLuaInt("noOfJellybeans"));
		heroMapOffset.x = L.DoLuaFloat("heroMapOffsetX");
		heroMapOffset.y = L.DoLuaFloat("heroMapOffsetY");

		// Minigames' informations
		for (int i = 0; i < 4; i++)
		{
			DifficultySystems[i].setCurrentDifficultyUnlocked(L.DoLuaInt("DifficultyUnlocked" + std::to_string(i)));
		}

		guardianCleared = L.DoLuaInt("guardianCleared");
		musicActive = L.DoLuaInt("musicActive");
	}
}

/********************************************************************************
Save information into the file
********************************************************************************/
void CGameInfo::SaveToFile()
{
	ofstream myfile(fileName);
	if (myfile.is_open())
	{
		myfile << "--[[" << endl;
		myfile << "Use these values to initialise the Data's Infomations" << endl;
		myfile << "]]--" << endl;
		myfile << endl;
		myfile << "ifNew = " << 0 << endl;
		myfile << endl;
		myfile << "heroPositionX = " << heroPosition.x << endl;
		myfile << "heroPositionY = " << heroPosition.y << endl;
		myfile << "heroAnimationDir = " << heroAnimationDir << endl;
		myfile << "noOfJellybeans = " << jellybean.GetNumOfJellybeans() << endl;
		myfile << "heroMapOffsetX = " << heroMapOffset.x << endl;
		myfile << "heroMapOffsetY = " << heroMapOffset.y << endl;
		myfile << endl;
		// Minigames' informations
		for (int i = 0; i < 4; i++)
		{
			myfile << "DifficultyUnlocked" << i << " = " << DifficultySystems[i].getCurrentDifficultyUnlocked() << endl;
		}
		myfile << endl;
		myfile << "guardianCleared = " << guardianCleared << endl;
		myfile << "musicActive = " << musicActive << endl;
	}
}


/********************************************************************************
Clear information of the file
********************************************************************************/
void CGameInfo::ClearFile()
{
	ResetData();

	ofstream myfile(fileName);
	if (myfile.is_open())
	{
		myfile << "--[[" << endl;
		myfile << "Use these values to initialise the Data's Infomations" << endl;
		myfile << "]]--" << endl;
		myfile << endl;
		myfile << "ifNew = " << 1 << endl;
		myfile << endl;
		myfile << "heroPositionX = " << heroPosition.x << endl;
		myfile << "heroPositionY = " << heroPosition.y << endl;
		myfile << "noOfJellybeans = " << jellybean.GetNumOfJellybeans() << endl;
		myfile << "heroMapOffsetX = " << heroPosition.x << endl;
		myfile << "heroMapOffsetY = " << heroPosition.y << endl;
		myfile << endl;
		// Minigames' informations
		for (int i = 0; i < 4; i++)
		{
			myfile << "DifficultyUnlocked" << i << " = " << DifficultySystems[i].getCurrentDifficultyUnlocked() << endl;
		}
		myfile << endl;
		myfile << "guardianCleared = " << guardianCleared << endl;
		myfile << "musicActive = " << musicActive << endl;
	}
}