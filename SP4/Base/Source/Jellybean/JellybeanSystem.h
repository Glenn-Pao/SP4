#pragma once
#include "../Mesh.h"

class CJellybeanSystem
{
public:
	CJellybeanSystem(void);
	~CJellybeanSystem(void);

	Mesh* mesh;

	enum DIFFICULTY{
		EASY,
		MEDIUM,
		HARD,
		NUM_OF_DIFFICULTIES
	};

	void readFile();
	void writeToFile();
	bool DepositJellybeans(int depositAmount, DIFFICULTY difficulty);
	void WithdrawJellybeans(bool won);

	int GetNumOfJellybeans(){ return noOfJellybeans; };

private:
	int noOfJellybeans;
	int noOfJellybeansDeposited;
	int minNoOfJellybeansDeposited[NUM_OF_DIFFICULTIES];
};
