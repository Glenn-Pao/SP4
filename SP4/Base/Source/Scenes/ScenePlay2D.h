#ifndef SCENE_PLAY_2D_H
#define SCENE_PLAY_2D_H

#include "SceneManager2D.h"
#include "..\Light.h"
#include "..\Minimap.h"
#include "..\Map.h"
#include "Vector2.h"
#include "..\PlayerInfo.h"
#include "..\Enemy.h"

// Goodies and Goodies Factory
#include "..\GoodiesFactory.h"
#include "..\Goodies.h"
#include "..\TreasureChest.h"

class CScenePlay2D : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TILEGROUND,
		GEO_TILEHERO,
		GEO_TILETREE,
		GEO_TILESTRUCTURE,
		GEO_TILEHERO_FRAME0,
		GEO_TILEHERO_FRAME1,
		GEO_TILEHERO_FRAME2,
		GEO_TILEHERO_FRAME3,
		GEO_TILE_KILLZONE,
		GEO_TILE_SAFEZONE,
		GEO_TILEENEMY_FRAME0,
		GEO_TILE_TREASURECHEST,
		GEO_OBJECT,
		NUM_GEOMETRY,
	};

public:
	CScenePlay2D(const int m_window_width, const int m_window_height);
	~CScenePlay2D();

	virtual void Init(int level);
	virtual void PreInit();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};

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

	CSceneManager2D sceneManager2D;
};

#endif