#include <iostream>
#include <fstream>
using namespace std;
#include "JellybeanSystem.h"

#include "..\..\UsingLua.h"

CJellybeanSystem::CJellybeanSystem(void)
	: noOfJellybeans(10)
	, noOfJellybeansDeposited(0)
{
	/*minNoOfJellybeansDeposited[CDifficultySystem::TUTORIAL] = 0;
	maxNoOfJellybeansDeposited[CDifficultySystem::TUTORIAL] = 0;
	minNoOfJellybeansDeposited[CDifficultySystem::EASY] = 1;
	minNoOfJellybeansDeposited[CDifficultySystem::MEDIUM] = 6;
	minNoOfJellybeansDeposited[CDifficultySystem::HARD] = 11;
	maxNoOfJellybeansDeposited[CDifficultySystem::EASY] = 5;
	maxNoOfJellybeansDeposited[CDifficultySystem::MEDIUM] = 10;
	maxNoOfJellybeansDeposited[CDifficultySystem::HARD] = 15;*/
	readFile();
}

CJellybeanSystem::~CJellybeanSystem(void)
{
}

/********************************************************************************
Read the file and store variables
********************************************************************************/
void CJellybeanSystem::readFile()
{
	UseLuaFiles L;

	L.ReadFiles("Lua//jellybeanInfo.lua");

	noOfJellybeans = L.DoLuaInt("noOfJellybeans");
	minNoOfJellybeansDeposited[CDifficultySystem::EASY] = L.DoLuaInt("easyMinDeposit");
	minNoOfJellybeansDeposited[CDifficultySystem::MEDIUM] = L.DoLuaInt("mediumMinDeposit");
	minNoOfJellybeansDeposited[CDifficultySystem::HARD] = L.DoLuaInt("hardMinDeposit");
	maxNoOfJellybeansDeposited[CDifficultySystem::EASY] = L.DoLuaInt("easyMaxDeposit");
	maxNoOfJellybeansDeposited[CDifficultySystem::MEDIUM] = L.DoLuaInt("mediumMaxDeposit");
	maxNoOfJellybeansDeposited[CDifficultySystem::HARD] = L.DoLuaInt("hardMaxDeposit");
}

/********************************************************************************
Write the file and store variables
********************************************************************************/
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
		myfile << "easyMinDeposit = " << minNoOfJellybeansDeposited[CDifficultySystem::EASY] << endl;
		myfile << "mediumMinDeposit = " << minNoOfJellybeansDeposited[CDifficultySystem::MEDIUM] << endl;
		myfile << "hardMinDeposit = " << minNoOfJellybeansDeposited[CDifficultySystem::HARD] << endl;
		myfile << "easyMaxDeposit = " << maxNoOfJellybeansDeposited[CDifficultySystem::EASY] << endl;
		myfile << "mediumMaxDeposit = " << maxNoOfJellybeansDeposited[CDifficultySystem::MEDIUM] << endl;
		myfile << "hardMaxDeposit = " << maxNoOfJellybeansDeposited[CDifficultySystem::HARD] << endl;
	}
}

/********************************************************************************
Deposit jellybeans to play minigame
********************************************************************************/
void CJellybeanSystem::DepositJellybeans(int depositAmount)
{
	noOfJellybeansDeposited = depositAmount;
	noOfJellybeans -= depositAmount;
}


/********************************************************************************
Withdraw jellybeans from minigame
********************************************************************************/
void CJellybeanSystem::WithdrawJellybeans()
{
	noOfJellybeans += noOfJellybeansDeposited * 2;
}


/********************************************************************************
Reset jellybeans
********************************************************************************/
void CJellybeanSystem::Reset()
{
	noOfJellybeans = 10;
}