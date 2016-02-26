#ifndef SCENE_HUB_H
#define SCENE_HUB_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
#include "..\..\Base\Source\GameMechanics\Objects\Door.h"
#include "..\..\..\UISystem.h"

class CSceneHub : public CSceneGameBase
{
public:
	CSceneHub(const int m_window_width, const int m_window_height);
	~CSceneHub();

	virtual void Init(int level);
	virtual void PreInit();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	enum GAME_INTERACTED
	{
		NO_GAME,
		GAME1,
		GAME2,
		GAME3,
		GAME4
	};
	int game_interacted;
	
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
		DIFFICULTY_SELECTION,
		JELLYBEAN_SELECTION,
		CONFIRMATION
	};

	STATE currentState;
	UISystem* UIManagerDifficultySelection;
	UISystem* UIManagerJellybeanSelection;
	UISystem* UIManagerBlackQuad;
	UISystem* UIManagerConfirmation;

	CSceneManager2D sceneManager2D;

	int noOfJellybeansDeposited;

	int difficultySelected;

	void ChangeUI_Playing();
	void ChangeUI_DifficultySelection();
	void ChangeUI_JellybeanSelection();
	void ChangeUI_Confirmation();
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	void InitMeshes();

	// Handle to the minimap
	//CMinimap* m_cMinimap;

	// Handle to the tilemaps
	//CMap* m_cMap;
	void RenderTileMap();
	// Hero's information
	//CPlayerInfo* theHero;
	void RenderHero();

	// Codes for Scrolling
	//int tileOffset_x, tileOffset_y;

	// Codes for Parallax Scrolling
	//CMap* m_cRearMap;
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

	void UpdateUI(double dt);

	void Show_DifficultySelection();
	void Hide_DifficultySelection();

	void Show_JellybeanSelection();
	void Hide_JellybeanSelection();

	void HideConfirmation();
	void ShowConfirmation();

	std::vector<CDoor*> theDoor;

	float UI_Speed;
};

#endif