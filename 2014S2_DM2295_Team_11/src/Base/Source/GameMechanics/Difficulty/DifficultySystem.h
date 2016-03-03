#pragma once

class CDifficultySystem
{
public:
	CDifficultySystem(void);
	~CDifficultySystem(void);

	enum DIFFICULTY{
		TUTORIAL,
		EASY,
		MEDIUM,
		HARD,
		NUM_OF_DIFFICULTIES
	};

	// Set current difficulty that is unlocked
	void setCurrentDifficultyUnlocked(int currentDifficultyUnlocked)
	{
		this->currentDifficultyUnlocked = currentDifficultyUnlocked;
	}
	// Get current difficulty that is unlocked
	int getCurrentDifficultyUnlocked()
	{
		return currentDifficultyUnlocked;
	}

	// Unlocked a new difficulty
	void UnlockedDifficulty();
private:
	int currentDifficultyUnlocked;
};
