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
	ifNew = false;

	// Player's Infomation
	heroPosition.Set(0, 0);
	noOfJellybeans = 10;
	heroMapOffset.Set(0, 0);
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

	// Player's Infomation
	heroPosition.x = L.DoLuaFloat("heroPositionX");
	heroPosition.y = L.DoLuaFloat("heroPositionY");
	heroAnimationDir = L.DoLuaInt("heroAnimationDir");
	noOfJellybeans = L.DoLuaInt("noOfJellybeans");
	heroMapOffset.x = L.DoLuaFloat("heroMapOffsetX");
	heroMapOffset.y = L.DoLuaFloat("heroMapOffsetY");
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
		myfile << "heroPositionX = " << heroPosition.x << endl;
		myfile << "heroPositionY = " << heroPosition.y << endl;
		myfile << "heroAnimationDir = " << heroAnimationDir << endl;
		myfile << "noOfJellybeans = " << noOfJellybeans << endl;
		myfile << "heroMapOffsetX = " << heroMapOffset.x << endl;
		myfile << "heroMapOffsetY = " << heroMapOffset.y << endl;
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
		myfile << "ifNew = " << 0 << endl;
		myfile << "heroPositionX = " << heroPosition.x << endl;
		myfile << "heroPositionY = " << heroPosition.y << endl;
		myfile << "noOfJellybeans = " << noOfJellybeans << endl;
		myfile << "heroMapOffsetX = " << heroPosition.x << endl;
		myfile << "heroMapOffsetY = " << heroPosition.y << endl;
	}
}