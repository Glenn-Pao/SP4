#include <iostream>
#include <fstream>
using namespace std;
#include "JellybeanSystem.h"

#include "..\UsingLua.h"

CJellybeanSystem::CJellybeanSystem(void)
{
	mesh = NULL;
	readFile();
}

CJellybeanSystem::~CJellybeanSystem(void)
{
	if (mesh)
		delete mesh;
	writeToFile();
}

/********************************************************************************
Read the file and store variables
********************************************************************************/
void CJellybeanSystem::readFile()
{
	UseLuaFiles L;

	L.ReadFiles("Lua//jellybeanInfo.lua");

	noOfJellybeans = L.DoLuaInt("noOfJellybeans");
	minNoOfJellybeansDeposited[EASY] = L.DoLuaInt("easyMinDeposit");
	minNoOfJellybeansDeposited[MEDIUM] = L.DoLuaInt("mediumMinDeposit");
	minNoOfJellybeansDeposited[HARD] = L.DoLuaInt("hardMinDeposit");
	maxNoOfJellybeansDeposited[EASY] = L.DoLuaInt("easyMaxDeposit");
	maxNoOfJellybeansDeposited[MEDIUM] = L.DoLuaInt("mediumMaxDeposit");
	maxNoOfJellybeansDeposited[HARD] = L.DoLuaInt("hardMaxDeposit");
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
		myfile << "easyMinDeposit = " << minNoOfJellybeansDeposited[EASY] << endl;
		myfile << "mediumMinDeposit = " << minNoOfJellybeansDeposited[MEDIUM] << endl;
		myfile << "hardMinDeposit = " << minNoOfJellybeansDeposited[HARD] << endl;
		myfile << "easyMaxDeposit = " << maxNoOfJellybeansDeposited[EASY] << endl;
		myfile << "mediumMaxDeposit = " << maxNoOfJellybeansDeposited[MEDIUM] << endl;
		myfile << "hardMaxDeposit = " << maxNoOfJellybeansDeposited[HARD] << endl;
	}
}

/********************************************************************************
Deposit jellybeans to play minigame
********************************************************************************/
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


/********************************************************************************
Withdraw jellybeans from minigame
********************************************************************************/
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