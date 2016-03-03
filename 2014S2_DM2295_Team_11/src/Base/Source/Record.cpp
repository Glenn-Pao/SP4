#include "Record.h"


CRecord::CRecord(void)
{
	playername = "";
}


CRecord::~CRecord(void)
{
}

int CRecord::getScore()
{
	return score;
}

void CRecord::setScore(int score)
{
	this->score = score;
}

string CRecord::getName()
{
	return playername;
}

void CRecord::setName(string name)
{
	playername = name;
}

void CRecord::reset()
{
	score = 0;
}