#include "PlayerInfo.h"
#include <iostream>

#include "UsingLua.h"

CPlayerInfo::CPlayerInfo(void)
	: hero_inMidAir_Up(false)
	, hero_inMidAir_Down(false)
	, jumpspeed(0)
	, mapOffset_x(0)
	, mapOffset_y(0)
	, mapFineOffset_x(0)
	, mapFineOffset_y(0)
{
	UseLuaFiles L;

	//Read a value from the lua text file
	L.ReadFiles("Lua//playerInfo.lua");

	// movement speed
	movementSpeed = L.DoLuaFloat("movementSpeed");

	// animation counter
	heroAnimationCounter = L.DoLuaFloat("animationCounter");

	// animation direction
	heroAnimationDirection = L.DoLuaInt("animationDirection");

	// animation speed
	animationSpeed = L.DoLuaFloat("animationSpeed");

	// no. of animation counter moving
	heroAnimationMaxCounter = L.DoLuaInt("animationMaxCounter");
}

CPlayerInfo::~CPlayerInfo(void)
{
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = NULL;
		}
	}
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	theHeroPosition.x=0;
	theHeroPosition.y=0;
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		hero_inMidAir_Up = true;
		hero_inMidAir_Down = false;
		jumpspeed = 15;
	}
}

// Set position x of the player
void CPlayerInfo::SetPos_x(int pos_x)
{
	theHeroPosition.x = pos_x;
}

// Set position y of the player
void CPlayerInfo::SetPos_y(int pos_y)
{
	theHeroPosition.y = pos_y;
}

// Set Jumpspeed of the player
void CPlayerInfo::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

// Stop the player's movement
void CPlayerInfo::SetToStop(void)
{
	hero_inMidAir_Up = false;
	hero_inMidAir_Down = false;
	jumpspeed = 0;
}

/********************************************************************************
 Hero Move Up Down
 ********************************************************************************/
void CPlayerInfo::MoveUpDown(const bool mode, const float timeDiff, CMap* m_cMap)
{
	if (mode)
	{
		theHeroPosition.y += (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		// Animation
		heroAnimationDirection = UP;
		// Check Collision
		switch (CheckCollision(m_cMap))
		{
		case 1:
			theHeroPosition.y -= (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
			break;
		}
	}
	else
	{
		theHeroPosition.y -= (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		// Animation
		heroAnimationDirection = DOWN;
		// Check Collision
		switch (CheckCollision(m_cMap))
		{
		case 1:
			theHeroPosition.y += (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
			break;
		}
	}
}

/********************************************************************************
 Hero Move Left Right
 ********************************************************************************/
void CPlayerInfo::MoveLeftRight(const bool mode, const float timeDiff, CMap* m_cMap)
{
	if (mode)
	{
		theHeroPosition.x -= (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		// Animation
		heroAnimationDirection = LEFT;
		// Check Collision
		switch (CheckCollision(m_cMap))
		{
		case 1:
			theHeroPosition.x += (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
			break;
		}
	}
	else
	{
		theHeroPosition.x += (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		// Animation
		heroAnimationDirection = RIGHT;
		// Check Collision
		switch (CheckCollision(m_cMap))
		{
		case 1:
			theHeroPosition.x -= (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
			break;
		}
	}
}


// Get position x of the player
int CPlayerInfo::GetPos_x(void)
{
	return theHeroPosition.x;
}

// Get position y of the player
int CPlayerInfo::GetPos_y(void)
{
	return theHeroPosition.y;
}

// Get Jumpspeed of the player
int CPlayerInfo::GetJumpspeed(void)
{
	return jumpspeed;
}

// Get mapOffset_x
int CPlayerInfo::GetMapOffset_x(void)
{
	return mapOffset_x;
}
// Get mapOffset_y
int CPlayerInfo::GetMapOffset_y(void)
{
	return mapOffset_y;
}

// Get mapFineOffset_x
int CPlayerInfo::GetMapFineOffset_x(void)
{
	return mapFineOffset_x;
}
// Get mapFineOffset_y
int CPlayerInfo::GetMapFineOffset_y(void)
{
	return mapFineOffset_y;
}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards()
{
	theHeroPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall()
{
	theHeroPosition.y += jumpspeed;
	jumpspeed += 1;
}

// Constrain the position of the Hero to within the border
void CPlayerInfo::ConstrainHero(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff, CMap* m_cMap)
{
	if (theHeroPosition.x < leftBorder)
	{
		mapOffset_x = mapOffset_x - (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		if (mapOffset_x < 0)
		{
			mapOffset_x = 0;
		}
		else
		{
			theHeroPosition.x = leftBorder;
		}
	}
	else if (theHeroPosition.x > rightBorder)
	{
		mapOffset_x = mapOffset_x + (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		float maxMapOffset_x = (m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width()) * m_cMap->GetTileSize();
		if (mapOffset_x > maxMapOffset_x)
		{
			mapOffset_x = maxMapOffset_x;
		}
		else
		{
			theHeroPosition.x = rightBorder;
		}
	}

	if (theHeroPosition.y < topBorder)
	{
		mapOffset_y = mapOffset_y + (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		float maxMapOffset_y = (m_cMap->getNumOfTiles_MapHeight() - m_cMap->GetNumOfTiles_Height()) * m_cMap->GetTileSize();
		if (mapOffset_y > maxMapOffset_y)
		{
			mapOffset_y = maxMapOffset_y;
		}
		else
		{
			theHeroPosition.y = topBorder;
		}
	}
	else if (theHeroPosition.y > bottomBorder)
	{
		mapOffset_y = mapOffset_y - (int)(movementSpeed * m_cMap->GetTileSize() * timeDiff);
		if (mapOffset_y < m_cMap->GetTileSize())
		{
			mapOffset_y = m_cMap->GetTileSize();
		}
		else
		{
			theHeroPosition.y = bottomBorder;
		}
	}
}


/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::HeroUpdate(CMap* m_cMap, float timeDiff)
{
	// Update Hero's info
	// Jump
  	/*if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
		// Don't jump
	}
	else if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
	{
		// Check if the hero can move up into mid air...
		int checkPosition_X = (int) ((mapOffset_x+theHeroPosition.x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theHeroPosition.y + m_cMap->GetTileSize() + jumpspeed) / m_cMap->GetTileSize());
		if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == 1) )
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			theHeroPosition.y = ((int) (theHeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			hero_inMidAir_Up = false;
			jumpspeed = 0;
		}
		else
		{
			theHeroPosition.y += jumpspeed;
			jumpspeed -= 1;
			if (jumpspeed == 0)
			{
				hero_inMidAir_Up = false;
				hero_inMidAir_Down = true;
			}
		}
	}
	else if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
	{
		// Check if the hero is still in mid air...
		int checkPosition_X = (int) ((mapOffset_x+theHeroPosition.x) / m_cMap->GetTileSize());
		if (checkPosition_X < 0)
			checkPosition_X = 0;
		if (checkPosition_X > m_cMap->getNumOfTiles_MapWidth())
			checkPosition_X = m_cMap->getNumOfTiles_MapWidth();
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theHeroPosition.y - jumpspeed) / m_cMap->GetTileSize());
		if (checkPosition_Y < 0)
			checkPosition_Y = 0;
		if (checkPosition_Y > m_cMap->GetNumOfTiles_Height())
			checkPosition_Y = m_cMap->GetNumOfTiles_Height();
		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			theHeroPosition.y = ((int) (theHeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			hero_inMidAir_Down = false;
			jumpspeed = 0;
		}
		else
		{
			theHeroPosition.y -= jumpspeed;
			jumpspeed += 1;
		}
	}*/

	ConstrainHero((m_cMap->GetNumOfTiles_Width() * 0.5) * m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Width() * 0.5) * m_cMap->GetTileSize(),
		(m_cMap->GetNumOfTiles_Height() * 0.5) * m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() * 0.5) * m_cMap->GetTileSize(),
		timeDiff, m_cMap);

	// Calculate the fine offset
	mapFineOffset_x = mapOffset_x % m_cMap->GetTileSize();
	mapFineOffset_y = mapOffset_y % m_cMap->GetTileSize();
}

/********************************************************************************
Check what the player if collided with
********************************************************************************/
int CPlayerInfo::CheckCollision(CMap* m_cMap)
{
	int checkLeftPosition_X = (int)((mapOffset_x + theHeroPosition.x + 0.1f * m_cMap->GetTileSize()) / m_cMap->GetTileSize());
	int checkRightPosition_X = (int)((mapOffset_x + theHeroPosition.x + 0.9f * m_cMap->GetTileSize()) / m_cMap->GetTileSize());
	int checkBottomPosition_Y = m_cMap->GetNumOfTiles_Height() - (int)ceil((float)(theHeroPosition.y - mapOffset_y + 0.1f * m_cMap->GetTileSize()) / m_cMap->GetTileSize());
	int checkTopPosition_Y = m_cMap->GetNumOfTiles_Height() - (int)ceil((float)(theHeroPosition.y - mapOffset_y + 0.9f * m_cMap->GetTileSize()) / m_cMap->GetTileSize());

	// Wall
	if ((m_cMap->theScreenMap[checkBottomPosition_Y][checkLeftPosition_X] == 1) ||
		(m_cMap->theScreenMap[checkBottomPosition_Y][checkRightPosition_X] == 1) ||
		(m_cMap->theScreenMap[checkTopPosition_Y][checkLeftPosition_X] == 1) ||
		(m_cMap->theScreenMap[checkTopPosition_Y][checkRightPosition_X] == 1))
	{
		return 1;
	}
	// Killzone
	else if ((m_cMap->theScreenMap[checkBottomPosition_Y][checkLeftPosition_X] == 10) ||
		(m_cMap->theScreenMap[checkBottomPosition_Y][checkRightPosition_X] == 10) ||
		(m_cMap->theScreenMap[checkTopPosition_Y][checkLeftPosition_X] == 10) ||
		(m_cMap->theScreenMap[checkTopPosition_Y][checkRightPosition_X] == 10))
	{
		return 10;
	}
	// Safezone
	else if ((m_cMap->theScreenMap[checkBottomPosition_Y][checkLeftPosition_X] == 11) ||
		(m_cMap->theScreenMap[checkBottomPosition_Y][checkRightPosition_X] == 11) ||
		(m_cMap->theScreenMap[checkTopPosition_Y][checkLeftPosition_X] == 11) ||
		(m_cMap->theScreenMap[checkTopPosition_Y][checkRightPosition_X] == 11))
	{
		return 11;
	}

	return 0;
}