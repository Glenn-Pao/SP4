#ifndef SCENE_TUTORIAL_GAME_2_H
#define SCENE_TUTORIAL_GAME_2_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
//#include "..\GameMechanics\SavingAndLoading\GameInfo.h"
#include "..\..\Base\Source\GameMechanics\Objects\Door.h"

class CSceneTutorialGame2 : public CSceneGameBase
{
public:
	CSceneTutorialGame2(const int m_window_width, const int m_window_height);
	~CSceneTutorialGame2();

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

	enum STATE
	{
		PLAYING,
		COMPLETED,
		TIME_UP,
		NUM_OF_STATE
	};
	int currentState;

	void RenderWaypoints();
	void RenderObjects();
	void RenderGUI();

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

	CSceneManager2D sceneManager2D;
	std::vector<CDoor*> GreyDoors;
	std::vector<CDoor*> BlueDoors;
	std::vector<CColour*> ColoursSet;
	std::vector<std::string> ColoursThePlayerHas;

	CDoor* greenDoor;
	CDoor* endDoor;
	bool hasBlue, hasYellow;
	bool castedBlue, castedYellow, castedGreen;
	Vector3 prevHeroPos;

	vector<CObjects*> dialogueTiles;

	string scriptExit, scriptFinished;

	int castedColoursCounter;
	float timer;
};

#endif