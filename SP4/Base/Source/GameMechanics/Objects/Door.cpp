#include "Door.h"


CDoor::CDoor()
{
	this->id = -1;
	TposX = -1;
	TposY = -1;
	isReached = false;
}

CDoor::CDoor(int id)
{
	this->id = id;
	
	isReached = false;
}

CDoor::~CDoor()
{

}

void CDoor::setId(int id)
{
	this->id = id;
}

int CDoor::getId()
{
	return id;
}

void CDoor::setReached(bool isReached)
{
	this->isReached = isReached;
}

bool CDoor::getIfReached()
{
	return isReached;
}

int CDoor::getTilemapPosX()
{
	return TposX;
}

int CDoor::getTilemapPosY()
{
	return TposY;
}