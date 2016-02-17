#pragma once
#include <vector>

/*
A probabilty system class that contain the probabilities and choosing probability function

Done by Gregory Koh Wen Cong
*/
class CProbabilitySystem
{
private:
	std::vector<float> probabilities;
public:
	CProbabilitySystem();
	~CProbabilitySystem();

	// Add a probability
	void AddProbability(float percentage);
	// Get total number of probabilities
	float GetTotalNumOfProbabilities();
	// Get a random probability
	int GetARandProbability();
};