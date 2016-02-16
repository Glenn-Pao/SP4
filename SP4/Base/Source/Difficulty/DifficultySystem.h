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

private:
	DIFFICULTY currentDifficultyUnlocked;
};
