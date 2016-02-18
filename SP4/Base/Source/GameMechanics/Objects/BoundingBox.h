#pragma once
/*
	The bonding box class is used to define the bonding box of objects

	Done by Gregory Koh
*/

#include "Vector3.h"

class CBoundingBox
{
private:
	Vector3 TopLeftCorner;
	Vector3 BottomRightCorner;
public:
	CBoundingBox();
	CBoundingBox(Vector3 TopLeftCorner, Vector3 BottomRightCorner);
	~CBoundingBox();


	void TranslateCollisionBox(Vector3 translate);

	// Set and get the Top Left Corner
	void setTopLeftCorner(Vector3);
	Vector3 getTopLeftCorner(void);

	// Set and get the Bottom Right Corner
	void setBottomRightCorner(Vector3);
	Vector3 getBottomRightCorner(void);

	// Check if position is collided with bonding box, return true if collided
	bool CheckCollision(Vector3 position);
	// Check if bonding box is collided with bonding box, return true if collided
	bool CheckCollision(CBoundingBox& bondingBox);
};