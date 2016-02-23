#pragma once

#include <vector>
#include <Vector3.h>

class AI
{
public:
	AI();
	AI(int AIType, Vector3 originalPos);
	~AI();

	void setAIType(int);
	int getAIType();

	void setOriginalPos(Vector3);
	Vector3 getOriginalPos();

private:
	Vector3 originalPos;
	int AIType;

};