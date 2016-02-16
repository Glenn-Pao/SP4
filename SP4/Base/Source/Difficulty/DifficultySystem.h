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

private:
	int currentDifficultyUnlocked;
};
