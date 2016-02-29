#ifndef SCENE_HUB_H
#define SCENE_HUB_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
#include "..\..\Base\Source\GameMechanics\Objects\Door.h"
#include "..\..\..\UISystem.h"
#include "..\..\Base\Source\AI\AI_Idling.h"

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

	void RenderWaypoints();

	// Game Interacted
	enum GAME_INTERACTED
	{
		NO_GAME,
		GAME1,
		GAME2,
		GAME3,
		GAME4,
		EXIT_GAME,
		NUM_OF_GAME_INTERACTION
	};
	int game_interacted;
	
	// State
	enum STATE
	{
		PLAYING,
		INTERACTING,
		GIVING_JELLYBEANS,
		DIFFICULTY_SELECTION,
		JELLYBEAN_SELECTION,
		CONFIRMATION
	};
	STATE currentState;

	// UIs
	UISystem* UIManagerDifficultySelection;
	UISystem* UIManagerJellybeanSelection;
	UISystem* UIManagerBlackQuad;
	UISystem* UIManagerConfirmation;

	void ChangeUI_Playing();
	void ChangeUI_DifficultySelection();
	void ChangeUI_JellybeanSelection();
	void ChangeUI_Confirmation();

	// Number of jellybean Deposited
	int noOfJellybeansDeposited;
	// Difficulty selected
	int difficultySelected;

	// Currently the target NPC, the player can interact with
	CAI_Idling* targetNPC;
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	void InitMeshes();

	// Handle to the tilemaps
	void RenderTileMap();
	void RenderHero();

	// AI
	void RenderAIs();
	// NPCs
	std::vector<CAI_Idling*> theNPCs;
	bool CheckIfThisNPCIsInteractable(CAI_Idling* theNPC);

	// Doors
	std::vector<CDoor*> theDoor;

	// UIs
	void InitUI();
	void UpdateUI(double dt);

	void Show_DifficultySelection();
	void Hide_DifficultySelection();

	void Show_JellybeanSelection();
	void Hide_JellybeanSelection();

	void HideConfirmation();
	void ShowConfirmation();

	float UI_Speed;

	// GUI
	void RenderGUI();

	// Number of jellybeans required to finish the game
	int jellybeansRequiredToFinish;
};

#endif