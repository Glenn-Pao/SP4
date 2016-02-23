#include "AIBase.h"

AI::AI()
{
}

AI::~AI()
{
}

AI::AI(int AIType, Vector3 originalPos)
{
	this->AIType = AIType;
	this->originalPos = originalPos;
}

void AI::setAIType(int AIType)
{
	this->AIType = AIType;
}

int AI::getAIType()
{
	return AIType;
}

void AI::setOriginalPos(Vector3 originalPos)
{
	this->originalPos = originalPos;
}

Vector3 AI::getOriginalPos()
{
	return originalPos;
}