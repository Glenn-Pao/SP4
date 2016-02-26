#ifndef SCENE_GAME_2_H
#define SCENE_GAME_2_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
//#include "..\GameMechanics\SavingAndLoading\GameInfo.h"
#include "..\..\Base\Source\GameMechanics\Objects\Door.h"
#include "..\..\..\AI\AIBase.h"
#include "..\..\..\AI\FSM.h"
#include "..\..\..\UsingLua.h"
class CSceneGame2 : public CSceneGameBase
{
public:
	CSceneGame2(const int m_window_width, const int m_window_height);
	~CSceneGame2();

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

	void InitTutorial();
	void InitLevel1();
	void InitLevel2();
	void InitLevel3();

	void UpdateTutorial(double dt);
	void UpdateLevel1(double dt);
	void UpdateLevel2(double dt);
	void UpdateLevel3(double dt);
	void UpdateRight4Pt(double dt);
	void UpdateLeft4Pt(double dt);

	void RenderTutorial();
	void RenderLevel1();
	void RenderLevel2();
	void RenderLevel3();

	int level;
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
	std::vector<CDoor*> YellowDoors;
	std::vector<CDoor*> OrangeDoors;
	std::vector<CDoor*> PurpleDoors;
	std::vector<CDoor*> GreenDoors;
	std::vector<CColour*> ColoursSet;
	std::vector<std::string> ColoursThePlayerHas;
	
	std::vector<string> scriptDialogues;

	CDoor* greenDoor;
	CDoor* endDoor;
	bool hasBlue, hasYellow, hasRed, hasPurple;
	bool castedBlue, castedYellow, castedGreen, castedOrange, castedRed, castedPurple;
	Vector3 prevHeroPos;

	vector<CObjects*> dialogueTiles;

	string scriptExit, scriptFinished, scriptLevelEnd;

	int Colours;
	float timer;
	int tileSize;

	UseLuaFiles* L;

	vector<AI*> AIsList4ptRight, AIsList4ptLeft;

	bool catchcooldown;
	double cooldownTimer;

	int smallSquare, medSquare, bigSquare;
};

#endif