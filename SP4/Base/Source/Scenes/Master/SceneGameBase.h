#ifndef SCENE_GAME_BASE_H
#define SCENE_GAME_BASE_H

#include "SceneManager2D.h"
#include "..\..\Light.h"
#include "..\..\Minimap.h"
#include "..\..\Map.h"
#include "Vector2.h"
#include "..\..\PlayerInfo.h"
#include "..\..\Enemy.h"
#include "..\..\GameMechanics\Jellybean\JellybeanSystem.h"
#include "..\..\AI\Waypoints.h"

// Goodies and Goodies Factory
#include "..\..\GoodiesFactory.h"
#include "..\..\Goodies.h"
#include "..\..\TreasureChest.h"

#include "..\..\GameMechanics\SavingAndLoading\GameInfo.h"

/*
	This is the base class containing the common information required for each game scene. It is to be inherited later on from the individual games.

	Done by Almeda Glenn Paolo Maestro
*/
class CSceneGameBase : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_DIALOGUE_BOX,
		GEO_TILEGROUND,
		GEO_TILE_GROUND,
		GEO_TILE_WALL,
		GEO_TILEHERO,
		GEO_TILETREE,
		GEO_TILESTRUCTURE,
		GEO_TILE_KILLZONE,
		GEO_TILE_SAFEZONE,
		GEO_TILEENEMY_FRAME0,
		GEO_TILE_TREASURECHEST,
		GEO_TILE_DOOR,
		GEO_OBJECT,

		// Game 1
		

		//Game3
		GEO_TILE_QN,
		GEO_TILE_ANS,
		GEO_TILE_ANS_GUI,
		GEO_POINTER,
		GEO_HEART,

		//Game4
		GEO_RED_CARD,
		GEO_BLUE_CARD,
		GEO_GREEN_CARD,
		GEO_STRESS_CARD,
		GEO_MISSING_CARD,
		GEO_STRESS,
		GEO_INSTRUCTIONS,
		GEO_PATTERN,

		//Game2
		GEO_COLOUR_BALL_BLUE,
		GEO_COLOUR_BALL_YELLOW,
		GEO_COLOUR_BALL_RED,
		GEO_COLOUR_BALL_ORANGE,
		GEO_COLOUR_BALL_GREEN,
		GEO_COLOUR_BALL_PURPLE,

		GEO_GREEN_DOOR,
		GEO_BLUE_DOOR,
		GEO_YELLOW_DOOR,
		GEO_ORANGE_DOOR,
		GEO_PURPLE_DOOR,
		GEO_BACKFADE,

		// UI Game Hub
		GEO_JELLYBEAN,
		GEO_SELECTION_BACKGROUND,

		GEO_DIFFICULTY,
		GEO_TUTORIAL_BUTTON_UP,
		GEO_TUTORIAL_BUTTON_DOWN,
		GEO_EASY_BUTTON_UP,
		GEO_EASY_BUTTON_DOWN,
		GEO_EASY_BUTTON_LOCKED,
		GEO_MEDIUM_BUTTON_UP,
		GEO_MEDIUM_BUTTON_DOWN,
		GEO_MEDIUM_BUTTON_LOCKED,
		GEO_HARD_BUTTON_UP,
		GEO_HARD_BUTTON_DOWN,
		GEO_HARD_BUTTON_LOCKED,

		GEO_JELLYBEAN_HEADER,
		GEO_UPARROW_BUTTON_UP,
		GEO_UPARROW_BUTTON_DOWN,
		GEO_UPARROW_BUTTON_LOCKED,
		GEO_DOWNARROW_BUTTON_UP,
		GEO_DOWNARROW_BUTTON_DOWN,
		GEO_DOWNARROW_BUTTON_LOCKED,
		GEO_ENTER_BUTTON_UP,
		GEO_ENTER_BUTTON_DOWN,
		GEO_ENTER_BUTTON_LOCKED,

		GEO_BACK_BUTTON_UP,
		GEO_BACK_BUTTON_DOWN,

		// CONFIRMATION
		GEO_CONFIRMATION_WINDOW,

		GEO_YES_BUTTON_UP,
		GEO_YES_BUTTON_DOWN,

		GEO_NO_BUTTON_UP,
		GEO_NO_BUTTON_DOWN,

		// Give Jellybeans
		GEO_GIVE_JELLYBEANS_WINDOW,

		// Alpha Black Quad
		GEO_ALPHA_BLACK_QUAD,


		// Dark Surrounding
		GEO_DARK_SURROUNDING,
		GEO_BLACK_QUAD,

		// Guardian
		GEO_GUARDIAN,

		// NPC
		GEO_GREEN_MONSTER,

		NUM_GEOMETRY,
	};
public:
	CSceneGameBase();
	CSceneGameBase(const int m_window_width, const int m_window_height);
	~CSceneGameBase();

	virtual void Init(int level) = 0;
	virtual void PreInit() = 0;
	virtual void Update(double dt) = 0;
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true) = 0;
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key) = 0;
	virtual void Render() = 0;
	virtual void Exit()= 0;

	// Find and Set the actual offset of hero
	void SetHeroOffset();

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};
	void RenderWaypoints();

	// Jellybeans
	int noOfJellybeans;

	CSceneManager2D sceneManager2D;

protected:
	Mesh* meshList[NUM_GEOMETRY];

	// Handle to the minimap
	CMinimap* m_cMinimap;

	// Handle to the tilemaps
	CMap* m_cMap;
	void RenderTileMap();
	// Hero's information
	CPlayerInfo* theHero;
	void RenderHero();
	// Codes for Parallax Scrolling
	CMap* m_cRearMap;
	void RenderRearTileMap();
	int rearWallOffset_x, rearWallOffset_y;
	int rearWallTileOffset_x, rearWallTileOffset_y;
	int rearWallFineOffset_x, rearWallFineOffset_y;

	CWaypoints *waypoints;
	std::vector<Vector3> temp;
	// Codes for Scrolling
	int tileOffset_x, tileOffset_y;

	// Enemies
	vector<CEnemy*> theEnemies;
	//void RenderAIs();

	// Goodies and Goodies Factory
	CGoodiesFactory theGoodiesFactory;
	CGoodies** theArrayOfGoodies;
	//void RenderGoodies(void);
private:
	unsigned m_vertexArrayID;
};

#endif