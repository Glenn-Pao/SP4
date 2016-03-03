#include "ProbabilitySystem.h"
#include "MyMath.h"


CProbabilitySystem::CProbabilitySystem()
{
	Math::InitRNG();
}


CProbabilitySystem::~CProbabilitySystem()
{
}


// Add a probability
void CProbabilitySystem::AddProbability(float percentage)
{
	probabilities.push_back(percentage);
}

// Get total number of probabilities
float CProbabilitySystem::GetTotalNumOfProbabilities()
{
	return (float)probabilities.size();
}

// Get a int random probability
int CProbabilitySystem::GetARandIntProbability()
{
	// Maximun percentage of all probabilities
	int MaxPercentage = 0;
	for (int i = 0; i < (int)probabilities.size(); i++)
	{
		// Add probility's percentage to MaxPercentage
		MaxPercentage += (int)probabilities[i];
	}

	// Get a random number within the total percentages
	int number = Math::RandIntMinMax(0, MaxPercentage);

	// Check which probability the number is in
	for (int i = 0; i < (int)probabilities.size(); i++)
	{
		// if number is lower or equal to the probility's percentage, return the i
		if (number <= (int)probabilities[i])
			return i;
		// else, subtract the probility's percentage off the number
		else
			number -= (int)probabilities[i];
	}
	// Return -1, if error
	return -1;
}

// Get a float random probability
int CProbabilitySystem::GetARandFloatProbability()
{
	// Maximun percentage of all probabilities
	float MaxPercentage = 0.0f;
	for (int i = 0; i < (int)probabilities.size(); i++)
	{
		// Add probility's percentage to MaxPercentage
		MaxPercentage += probabilities[i];
	}

	// Get a random number within the total percentages
	float number = Math::RandFloatMinMax(0.0f, MaxPercentage);

	// Check which probability the number is in
	for (int i = 0; i < (int)probabilities.size(); i++)
	{
		// if number is lower or equal to the probility's percentage, return the i
		if (number <= probabilities[i])
			return i;
		// else, subtract the probility's percentage off the number
		else
			number -= probabilities[i];
	}
	// Return -1, if error
	return -1;
}