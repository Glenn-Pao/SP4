#include "BondingBox.h"


CBondingBox::CBondingBox()
	: TopLeftCorner(0, 0, 0)
	, BottomRightCorner(0, 0, 0)
{
}

CBondingBox::~CBondingBox()
{
}

// Initialise the Both Top Left Corner and Bottom Right Corner
void CBondingBox::Init(Vector3 TopLeftCorner, Vector3 BottomRightCorner)
{
	this->TopLeftCorner = TopLeftCorner;
	this->BottomRightCorner = BottomRightCorner;
}

// Set and get the Top Left Corner
void CBondingBox::setTopLeftCorner(Vector3 TopLeftCorner)
{
	this->TopLeftCorner = TopLeftCorner;
}
Vector3 CBondingBox::getTopLeftCorner(void)
{
	return TopLeftCorner;
}

//set and get the position X of the entity
void CBondingBox::setBottomRightCorner(Vector3 BottomRightCorner)
{
	this->BottomRightCorner = BottomRightCorner;
}
Vector3 CBondingBox::getBottomRightCorner(void)
{
	return BottomRightCorner;
}

// Check if position is collided with bonding box, return true if collided
bool CBondingBox::CheckCollision(Vector3 position)
{
	// Check X
	if (TopLeftCorner.x < position.x && position.x > BottomRightCorner.x)
	{
		// Check Y
		if (TopLeftCorner.y > position.y && position.y < BottomRightCorner.y)
		{
			return true;
		}
	}

	return false;
}


// Check if bonding box is collided with bonding box, return true if collided
bool CBondingBox::CheckCollision(CBondingBox& bondingBox)
{
	// Check if any of the four corner of the bonding box within this bondingBox
	// Top Left Corner
	if (CheckCollision(bondingBox.TopLeftCorner))
	{
		return true;
	}
	// Top Right Corner
	if (CheckCollision(Vector3(bondingBox.BottomRightCorner.x, bondingBox.TopLeftCorner.y)))
	{
		return true;
	}
	// Bottom Left Corner
	if (CheckCollision(Vector3(bondingBox.TopLeftCorner.x, bondingBox.BottomRightCorner.y)))
	{
		return true;
	}
	// Bottom Right Corner
	if (CheckCollision(bondingBox.BottomRightCorner))
	{
		return true;
	}

	// Check if any of the four corner of this bonding box within the bondingBox
	// Top Left Corner
	if (bondingBox.CheckCollision(TopLeftCorner))
	{
		return true;
	}
	// Top Right Corner
	if (bondingBox.CheckCollision(Vector3(BottomRightCorner.x, TopLeftCorner.y)))
	{
		return true;
	}
	// Bottom Left Corner
	if (bondingBox.CheckCollision(Vector3(TopLeftCorner.x, BottomRightCorner.y)))
	{
		return true;
	}
	// Bottom Right Corner
	if (bondingBox.CheckCollision(BottomRightCorner))
	{
		return true;
	}

	return false;
}