#pragma once

class CDifficultySystem
{
public:
	CDifficultySystem(void);
	~CDifficultySystem(void);

	enum DIFFICULTY{
		EASY,
		MEDIUM,
		HARD,
		NUM_OF_DIFFICULTIES
	};

	// Get current difficulty that is unlocked
	int getCurrentDifficultyUnlocked()
	{
		return currentDifficultyUnlocked;
	}
	// Unlocked a new difficulty
	void UnlockedDifficulty();

	// Read the file and store variables
	void readFile();
	// Write the file and store variables
	void writeToFile();
private:
	int currentDifficultyUnlocked;
};
