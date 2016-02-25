#pragma once
#include "..\..\Mesh.h"
#include "..\Difficulty\DifficultySystem.h"

class CJellybeanSystem
{
public:
	CJellybeanSystem(void);
	~CJellybeanSystem(void);

	// Read the file and store variables
	void readFile();
	// Write the file and store variables
	void writeToFile();
	// Deposit jellybeans to play minigame
	void DepositJellybeans(int depositAmount);
	// Withdraw jellybeans from minigame
	void WithdrawJellybeans();
	// Set number of jellybeans player have
	void SetNumOfJellybeans(int noOfJellybeans){ this->noOfJellybeans = noOfJellybeans; };
	// Get number of jellybeans player have
	int GetNumOfJellybeans(){ return noOfJellybeans; };

	// Get min number of jellybeans can be deposited
	int GetMinNumOfJellybeansCanBeDeposited(int difficulty){ return minNoOfJellybeansDeposited[difficulty]; };
	// Get max number of jellybeans can be deposited
	int GetMaxNumOfJellybeansCanBeDeposited(int difficulty){ return maxNoOfJellybeansDeposited[difficulty]; };
	// Reset jellybeans
	void Reset();

private:
	// Number of jellybeans player have
	int noOfJellybeans;
	// Number of jellybeans player deposited
	int noOfJellybeansDeposited;
	// Minimun number of jellybeans player can deposited
	int minNoOfJellybeansDeposited[CDifficultySystem::NUM_OF_DIFFICULTIES];
	// Maximum number of jellybeans player can deposited
	int maxNoOfJellybeansDeposited[CDifficultySystem::NUM_OF_DIFFICULTIES];
};
