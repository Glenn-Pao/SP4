#include "SavingAndLoadingSystem.h"

CSavingAndLoadingSystem::CSavingAndLoadingSystem(void)
{
}

CSavingAndLoadingSystem::~CSavingAndLoadingSystem(void)
{
	for (int i = 0; i < Data.size(); i++)
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
}

/********************************************************************************
Read the file and store variables
********************************************************************************/
bool CSavingAndLoadingSystem::LoadFile(int index)
{
	// check if index is higher than 0 and lower than number of Datas
	if (index >= 0 && index < Data.size())
	{
		Data[index]->LoadFile();
		currentIndex = index;

		return true;
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
Get GameInfo
********************************************************************************/
CGameInfo* CSavingAndLoadingSystem::GetGameInfo()
{
	return Data[currentIndex];
}