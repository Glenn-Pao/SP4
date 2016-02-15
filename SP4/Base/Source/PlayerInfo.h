#pragma once
#include "Vector2.h"
#include "Map.h"

class CPlayerInfo
{
public:
	CPlayerInfo(void);
	~CPlayerInfo(void);

	enum GEOMETRY_TYPE
	{
		GEO_TILEHERO_FRAME0,
		GEO_TILEHERO_FRAME1,
		GEO_TILEHERO_FRAME2,
		GEO_TILEHERO_FRAME3,
		NUM_GEOMETRY,
	};

	// Initialise this class instance
	void Init(void);
	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's movement
	void SetToStop(void);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveUpDown(const bool mode, const float timeDiff, CMap* m_cMap);
	void MoveLeftRight(const bool mode, const float timeDiff, CMap* m_cMap);

	// Get position x of the player
	int GetPos_x(void);
	// Get position y of the player
	int GetPos_y(void);
	// Get Jumpspeed of the player
	int GetJumpspeed(void);
	// Get mapOffset_x
	int GetMapOffset_x(void);
	// Get mapOffset_y
	int GetMapOffset_y(void);
	// Set mapOffset_x
	void SetMapOffset_x(int mapOffset_x){ this->mapOffset_x = mapOffset_x; };
	// Set mapOffset_y
	void SetMapOffset_y(int mapOffset_y){ this->mapOffset_y = mapOffset_y; };
	// Get mapFineOffset_x
	int GetMapFineOffset_x(void);
	// Get mapFineOffset_y
	int GetMapFineOffset_y(void);

	// Get movement speed
	float GetMovementSpeed(void){ return movementSpeed; };
	// Get animation speed
	float GetAnimationSpeed(void){ return animationSpeed; };

	// Update Jump Upwards
	void UpdateJumpUpwards();
	// Update FreeFall
	void UpdateFreeFall();
	// Hero Update
	void HeroUpdate(CMap* m_cMap, float timeDiff);

	// Set Animation Direction of the player
	void SetAnimationDirection(int heroAnimationDirection) { this->heroAnimationDirection = heroAnimationDirection; };
	// Get Animation Direction of the player
	int GetAnimationDirection(void) { return heroAnimationDirection; };
	// Set Animation Counter of the player
	void SetAnimationCounter(float heroAnimationCounter) { this->heroAnimationCounter = heroAnimationCounter; };
	// Get Animation Counter of the player
	float GetAnimationCounter(void) { return heroAnimationCounter; };

	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder, 
					   const int topBorder, const int bottomBorder, 
					   float timeDiff, CMap* m_cMap);

	// Check what the player if collided with
	int CheckCollision(CMap* m_cMap);

	enum ANIMATION_DIRECTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		NUM_OF_ANIMATION_DIRECTIONS
	};
private:
	// Hero's information
	Vector2 theHeroPosition;
	int jumpspeed;
	float movementSpeed;
	bool hero_inMidAir_Up;
	bool hero_inMidAir_Down;
	float heroAnimationCounter;
	int heroAnimationDirection;
	float animationSpeed;

	// For scrolling.
	int mapOffset_x, mapOffset_y;
	int mapFineOffset_x, mapFineOffset_y;
};
