#pragma once
#include "Objects.h"

class CDoor : public CObjects
{
private:
	int id;
	//Bool that triggers if player has reached the door
	bool isReached;

	int TposX, TposY;
public:
	CDoor();
	CDoor(int);
	~CDoor();

	void setId(int);
	int getId();

	void setReached(bool);
	bool getIfReached();

	int getTilemapPosX();
	int getTilemapPosY();
};