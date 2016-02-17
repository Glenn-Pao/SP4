#ifndef SCENE_TUTORIAL_GAME_4_H
#define SCENE_TUTORIAL_GAME_4_H

#include "SceneManager2D.h"
#include "..\Light.h"
#include "..\Minimap.h"
#include "..\Map.h"
#include "Vector2.h"
#include "..\PlayerInfo.h"
#include "..\Enemy.h"
#include "..\GameMechanics\Jellybean\JellybeanSystem.h"
#include "..\AI\Waypoints.h"

// Goodies and Goodies Factory
#include "..\GoodiesFactory.h"
#include "..\Goodies.h"
#include "..\TreasureChest.h"

#include "..\GameMechanics\SavingAndLoading\GameInfo.h"

class CSceneTutorialGame4 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TILEGROUND,
		GEO_TILEHERO,
		GEO_TILETREE,
		GEO_TILESTRUCTURE,
		GEO_TILE_KILLZONE,
		GEO_TILE_SAFEZONE,
		GEO_TILEENEMY_FRAME0,
		GEO_TILE_TREASURECHEST,
		GEO_TILE_DOOR,
		GEO_OBJECT,
		NUM_GEOMETRY,
	};

public:
	CSceneTutorialGame4(const int m_window_width, const int m_window_height);
	~CSceneTutorialGame4();

	virtual void Init(int level);
	virtual void PreInit();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	// Read and store data
	virtual void ReadData(CGameInfo* Data);
	virtual void StoreData(CGameInfo* Data);

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
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	void InitMeshes();

	// Handle to the minimap
	CMinimap* m_cMinimap;

	// Handle to the tilemaps
	CMap* m_cMap;
	void RenderTileMap();
	// Hero's information
	CPlayerInfo* theHero;
	void RenderHero();

	// Codes for Scrolling
	int tileOffset_x, tileOffset_y;

	// Codes for Parallax Scrolling
	CMap* m_cRearMap;
	void RenderRearTileMap();
	int rearWallOffset_x, rearWallOffset_y;
	int rearWallTileOffset_x, rearWallTileOffset_y;
	int rearWallFineOffset_x, rearWallFineOffset_y;

	// Enemies
	vector<CEnemy*> theEnemies;
	void RenderAIs();

	// Goodies and Goodies Factory
	CGoodiesFactory theGoodiesFactory;
	CGoodies** theArrayOfGoodies;
	void RenderGoodies(void);

	// Jellybeans System
	CJellybeanSystem* JellybeanSystem;

	CSceneManager2D sceneManager2D;

	CWaypoints *waypoints;
	std::vector<Vector3> temp;
};

#endif