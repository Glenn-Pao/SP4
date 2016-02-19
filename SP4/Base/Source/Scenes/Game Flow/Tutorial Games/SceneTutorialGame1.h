#ifndef SCENE_TUTORIAL_GAME_1_H
#define SCENE_TUTORIAL_GAME_1_H

#include "..\..\Master\SceneGameBase.h"
#include "..\..\..\GameMechanics\Objects\Door.h"

class CSceneTutorialGame1 : public CSceneGameBase
{
public:
	CSceneTutorialGame1(const int m_window_width, const int m_window_height);
	~CSceneTutorialGame1();

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

	enum STATE
	{
		PLAYING,
		EXITING,
		COMPLETED,
		TIME_UP,
		NUM_OF_STATE
	};
	int currentState;

private:
	void InitMeshes();
	// Handle to the GUI
	void RenderGUI();
	// Handle to the tilemaps
	void RenderTileMap();
	// Hero's information
	void RenderHero();

	void RenderRearTileMap(){}

	// Enemies
	//vector<CEnemy*> theEnemies;
	void RenderAIs();

	CDoor* theDoor;

	float timer;
};

#endif