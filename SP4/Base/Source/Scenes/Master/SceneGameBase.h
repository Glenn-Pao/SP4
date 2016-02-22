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
		GEO_TILE_QN,
		GEO_TILE_ANS,
		GEO_OBJECT,

		GEO_HEART,


		GEO_COLOUR_BALL_BLUE,
		GEO_COLOUR_BALL_YELLOW,
		GEO_COLOUR_BALL_RED,
		GEO_COLOUR_BALL_ORANGE,
		GEO_GREEN_DOOR,
		GEO_BLUE_DOOR,

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

	// Read and store data
	void ReadData(CGameInfo* Data);
	void StoreData(CGameInfo* Data);

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
	// Jellybeans System
	CJellybeanSystem* JellybeanSystem;
	// Codes for Scrolling
	int tileOffset_x, tileOffset_y;

	// Enemies
	vector<CEnemy*> theEnemies;
	//void RenderAIs();

	// Goodies and Goodies Factory
	CGoodiesFactory theGoodiesFactory;
	CGoodies** theArrayOfGoodies;
	//void RenderGoodies(void);
	CSceneManager2D sceneManager2D;
private:
	unsigned m_vertexArrayID;
};

#endif