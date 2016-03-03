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
	// Read a new file and load the game
	bool LoadNewFile();
	// Save information into the file
	void SaveToFile();
	// Clear information of the file
	void ClearFile();

	// Get number of Data Stored
	int GetNumOfData();
	// Get Current GameInfo
	CGameInfo* GetCurrentGameInfo();
	// Get a specific GameInfo
	CGameInfo* GetGameInfo(int index);
	// Get Current Index
	int GetCurrentIndex();

	bool to_save;
private:
	// Game Info
	std::vector<CGameInfo*> Data;

	int currentIndex;
};
