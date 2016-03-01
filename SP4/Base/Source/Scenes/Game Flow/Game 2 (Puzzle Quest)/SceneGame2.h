#ifndef SCENE_GAME_2_H
#define SCENE_GAME_2_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
//#include "..\GameMechanics\SavingAndLoading\GameInfo.h"
#include "..\..\Base\Source\GameMechanics\Objects\Door.h"
#include "..\..\..\AI\AIBase.h"
#include "..\..\..\AI\FSM.h"
#include "..\..\..\UsingLua.h"
#include "..\..\..\UISystem.h"
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
		INTERACTING,
		WIN,
		LOSE,
		NUM_OF_STATE
	};
	STATE currentState;

	void RenderWaypoints();
	void RenderObjects();
	void RenderGUI();

	void InitTutorial();
	void InitLevel2();
	void InitLevel3();

	void UpdateTutorial(double dt);
	void UpdateLevel2(double dt);
	void UpdateLevel3(double dt);

	void UpdateRight4PtS(double dt);
	void UpdateLeft4PtS(double dt);
	void UpdateRight4PtM(double dt);
	void UpdateLeft4PtM(double dt);
	void UpdateRight4PtB(double dt);
	void UpdateLeft4PtB(double dt);
	void UpdateAI(double dt);

	void RenderTutorial();
	void RenderLevel2();
	void RenderLevel3();

	int level;

	UISystem* UIManager;
	//UISystem* UIManagerConfirmation;

	CSceneManager2D sceneManager2D;
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

	// UIs
	void InitUI();
	void UpdateUI(double dt);

	float UI_Speed;

	
	
	CDoor* endDoor;
	std::vector<CDoor*> GreyDoors;
	std::vector<CDoor*> BlueDoors;
	std::vector<CDoor*> YellowDoors;
	std::vector<CDoor*> OrangeDoors;
	std::vector<CDoor*> PurpleDoors;
	std::vector<CDoor*> GreenDoors;
	std::vector<CColour*> ColoursSet;

	std::vector<std::string> ColoursThePlayerHas;
	std::vector<string> scriptDialogues;
	
	bool hasBlue, hasYellow, hasRed, hasPurple;
	bool castedBlue, castedYellow, castedGreen, castedOrange, castedRed, castedPurple;
	Vector3 prevHeroPos;

	vector<CObjects*> dialogueTiles;

	string scriptExit, scriptFinished, scriptLevelEnd;

	int Colours;
	float timer;
	int tileSize;

	UseLuaFiles* L;

	vector<AI*> AIsList4ptRightS, AIsList4ptLeftS, AIsList4ptRightM, AIsList4ptLeftM, AIsList4ptRightB, AIsList4ptLeftB;

	bool catchcooldown;
	double cooldownTimer;

	int smallSquare, medSquare, bigSquare;
	
	double TRightS, TLeftS, TRightM, TLeftM, TRightB, TLeftB;
	std::string showTextBlue, showTextYellow, showTextGreen, showTextRed, showTextOrange, showTextPurple;

	Vector3 slot1, slot2, slot3, slot4;
};

#endif