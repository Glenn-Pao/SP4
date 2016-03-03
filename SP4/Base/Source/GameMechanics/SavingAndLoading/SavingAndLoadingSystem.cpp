#include "SavingAndLoadingSystem.h"

CSavingAndLoadingSystem::CSavingAndLoadingSystem(void)
	: to_save(false)
{
}

CSavingAndLoadingSystem::~CSavingAndLoadingSystem(void)
{
	for (int i = 0; i < (int)Data.size(); i++)
	{
		if (Data[i])
		{
			delete Data[i];
			Data[i] = NULL;
		}
	}
}

/********************************************************************************
Add new data
********************************************************************************/
void CSavingAndLoadingSystem::AddData(const char* fileName)
{
	Data.push_back(new CGameInfo(fileName));
	Data.back()->LoadFile();
}

/********************************************************************************
Read the file and store variables
********************************************************************************/
bool CSavingAndLoadingSystem::LoadFile(int index)
{
	// check if index is higher than 0 and lower than number of Datas
	if (index >= 0 && index < (int)Data.size())
	{
		currentIndex = index;

		return true;
	}
	return false;
}

/********************************************************************************
Read a new file and load the game
********************************************************************************/
bool CSavingAndLoadingSystem::LoadNewFile()
{
	// Check every Data
	for (int i = 0; i < (int)Data.size(); i++)
	{
		// check if file is new
		if (Data[i]->ifNew)
		{
			currentIndex = i;
			return true;
		}
	}
	return false;
}

/********************************************************************************
Write the file and store variables
********************************************************************************/
void CSavingAndLoadingSystem::SaveToFile()
{
	Data[currentIndex]->SaveToFile();
}


/********************************************************************************
Clear information of the file
********************************************************************************/
void CSavingAndLoadingSystem::ClearFile()
{
	Data[currentIndex]->ClearFile();
}


/********************************************************************************
Get number of Data Stored
********************************************************************************/
int CSavingAndLoadingSystem::GetNumOfData()
{
	return Data.size();
}
/********************************************************************************
Get Current GameInfo
********************************************************************************/
CGameInfo* CSavingAndLoadingSystem::GetCurrentGameInfo()
{
	return Data[currentIndex];
}
/********************************************************************************
Get a specific GameInfo
********************************************************************************/
CGameInfo* CSavingAndLoadingSystem::GetGameInfo(int index)
{
	return Data[index];
}
/********************************************************************************
Get Current Index
********************************************************************************/
int CSavingAndLoadingSystem::GetCurrentIndex()
{
	return currentIndex;
}