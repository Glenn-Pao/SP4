#pragma once
#include <string>
#include "Vector2.h"
#include "..\Difficulty\DifficultySystem.h"

class CGameInfo
{
private:
	// Reset information of the Game
	void ResetData();
public:
	CGameInfo(const char* fileName);
	~CGameInfo(void);
	// Read the file and load the game
	void LoadFile();
	// Save information into the file
	void SaveToFile();
	// Clear information of the file
	void ClearFile();


	// Game's Infomation

	// Name of the file
	const char* fileName;

	// If New Data
	bool ifNew;

	// Player's Infomation
	Vector2 heroPosition;// Hero position
	int heroAnimationDir;// Hero animation direction
	int noOfJellybeans;// numer of jellybeans player have
	Vector2 heroMapOffset;// Hero map offset
	CDifficultySystem difficultySystems[4]; // difficulty System Of the 4 Minigames
};
