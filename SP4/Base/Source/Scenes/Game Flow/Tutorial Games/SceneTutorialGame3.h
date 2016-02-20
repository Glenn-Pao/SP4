#ifndef SCENE_TUTORIAL_GAME_3_H
#define SCENE_TUTORIAL_GAME_3_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
#include "..\..\Base\Source\GameMechanics\Objects\Question.h"
//#include "..\GameMechanics\SavingAndLoading\GameInfo.h"

class CSceneTutorialGame3 : public CSceneGameBase
{
public:
	CSceneTutorialGame3(const int m_window_width, const int m_window_height);
	~CSceneTutorialGame3();

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
	void RenderWaypoints();
private:
	void InitMeshes();
	// Handle to the tilemaps
	void RenderTileMap();
	// Hero's information
	void RenderHero();

	// Codes for Scrolling
	//int tileOffset_x, tileOffset_y;

	void RenderRearTileMap();
	//int rearWallOffset_x, rearWallOffset_y;
	//int rearWallTileOffset_x, rearWallTileOffset_y;
	//int rearWallFineOffset_x, rearWallFineOffset_y;

	// Enemies
	//vector<CEnemy*> theEnemies;
	void RenderAIs();

	// Goodies and Goodies Factory
	//CGoodiesFactory theGoodiesFactory;
	//CGoodies** theArrayOfGoodies;
	void RenderGoodies(void);

	//Questions
	vector<CQuestion*>theQuestions;

	CSceneManager2D sceneManager2D;	
	int count;
};

#endif