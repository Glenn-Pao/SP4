#pragma once
#include "Vector2.h"
#include "Map.h"
#include "Mesh.h"
#include "GameMechanics\Objects\Objects.h"


const int TileTypes = 8; // Increase it if more tile types added

class CPlayerInfo : public CObjects
{
public:
	CPlayerInfo(CMap* m_cMap);
	~CPlayerInfo(void);

	enum GEOMETRY_TYPE
	{
		// Side right
		GEO_TILEHERO_SIDE_FRAME0,
		GEO_TILEHERO_SIDE_FRAME1,
		GEO_TILEHERO_SIDE_FRAME2,
		GEO_TILEHERO_SIDE_FRAME3,
		GEO_TILEHERO_SIDE_FRAME4,
		GEO_TILEHERO_SIDE_FRAME5,
		GEO_TILEHERO_SIDE_FRAME6,
		GEO_TILEHERO_SIDE_FRAME7,
		GEO_TILEHERO_SIDE_FRAME8,
		NUM_GEOMETRY_SIDE,
		// Up
		GEO_TILEHERO_UP_FRAME0,
		GEO_TILEHERO_UP_FRAME1,
		GEO_TILEHERO_UP_FRAME2,
		GEO_TILEHERO_UP_FRAME3,
		GEO_TILEHERO_UP_FRAME4,
		GEO_TILEHERO_UP_FRAME5,
		GEO_TILEHERO_UP_FRAME6,
		GEO_TILEHERO_UP_FRAME7,
		GEO_TILEHERO_UP_FRAME8,
		NUM_GEOMETRY_UP,
		// Down
		GEO_TILEHERO_DOWN_FRAME0,
		GEO_TILEHERO_DOWN_FRAME1,
		GEO_TILEHERO_DOWN_FRAME2,
		GEO_TILEHERO_DOWN_FRAME3,
		GEO_TILEHERO_DOWN_FRAME4,
		GEO_TILEHERO_DOWN_FRAME5,
		GEO_TILEHERO_DOWN_FRAME6,
		GEO_TILEHERO_DOWN_FRAME7,
		GEO_TILEHERO_DOWN_FRAME8,
		NUM_GEOMETRY_DOWN,
		NUM_GEOMETRY,
	};
	Mesh* meshList[NUM_GEOMETRY];

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
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveUpDown(const bool mode, const float timeDiff, CMap* m_cMap);
	void MoveLeftRight(const bool mode, const float timeDiff, CMap* m_cMap);

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
	// Get Animation Max Counter of the player
	int GetAnimationMaxCounter(void) { return heroAnimationMaxCounter; };

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

	// Read the file and store variables
	void readFile();
	// Write the file and store variables
	void writeToFile();


	float getLeftCollision(void);
	float getRightCollision(void);
	float getTopCollision(void);
	float getBottomCollision(void);
private:
	// Hero's information
	int jumpspeed;
	float movementSpeed;
	bool hero_inMidAir_Up;
	bool hero_inMidAir_Down;
	float heroAnimationCounter;
	int heroAnimationMaxCounter;
	int heroAnimationDirection;
	float animationSpeed;

	// For scrolling.
	int mapOffset_x, mapOffset_y;


	// For tile checking
	int tileCollided[TileTypes];

	// For collision
	float leftCollision, rightCollision, topCollision, bottomCollision;
};
