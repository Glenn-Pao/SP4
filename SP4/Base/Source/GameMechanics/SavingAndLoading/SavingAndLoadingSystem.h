#pragma once
#include "GameInfo.h"
#include <vector>

class CSavingAndLoadingSystem
{
public:
	CSavingAndLoadingSystem(void);
	~CSavingAndLoadingSystem(void);
	

	// Add new data
	void AddData(const char* fileName);
	// Read the file and load the game
	bool LoadFile(int index);
	// Save information into the file
	void SaveToFile();
	// Clear information of the file
	void ClearFile();

	// Get GameInfo
	CGameInfo* GetGameInfo();

private:
	// Game Info
	std::vector<CGameInfo*> Data;

	int currentIndex;
};
