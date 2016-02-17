#pragma once
#include "..\..\Mesh.h"

class CJellybeanSystem
{
public:
	CJellybeanSystem(void);
	~CJellybeanSystem(void);

	// Store jellybeans meah
	Mesh* mesh;

	// Difficulty
	enum DIFFICULTY{
		EASY,
		MEDIUM,
		HARD,
		NUM_OF_DIFFICULTIES
	};
	// Read the file and store variables
	void readFile();
	// Write the file and store variables
	void writeToFile();
	// Deposit jellybeans to play minigame
	bool DepositJellybeans(int depositAmount, DIFFICULTY difficulty);
	// Withdraw jellybeans from minigame
	void WithdrawJellybeans(bool won);
	// Set number of jellybeans player have
	void SetNumOfJellybeans(int noOfJellybeans){ this->noOfJellybeans = noOfJellybeans; };
	// Get number of jellybeans player have
	int GetNumOfJellybeans(){ return noOfJellybeans; };

	// Reset jellybeans
	void Reset();

private:
	// Number of jellybeans player have
	int noOfJellybeans;
	// Number of jellybeans player deposited
	int noOfJellybeansDeposited;
	// Minimun number of jellybeans player can deposited
	int minNoOfJellybeansDeposited[NUM_OF_DIFFICULTIES];
	// Maximum number of jellybeans player can deposited
	int maxNoOfJellybeansDeposited[NUM_OF_DIFFICULTIES];
};
