#pragma once
#include "..\..\Master\SceneGameBase.h"
#include "..\..\Base\Source\GameMechanics\Objects\Deck.h"
#include "..\..\Base\Source\GameMechanics\Objects\Trigger.h"
#include "..\..\Base\Source\AI\ProbabilitySystem.h"
#include "..\..\Base\Source\UISystem.h"
class SceneGame4 : public CSceneGameBase
{

public:
	SceneGame4(const int m_window_width, const int m_window_height);
	~SceneGame4();

	virtual void Init(int level);
	virtual void PreInit();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	enum State
	{
		PREPARE,
		PLAY,
		TIME_UP,
		WIN,
		LOSE,
		PAUSE,
		RETURN,
		NUM_OF_STATE
	};

	enum DifficultyLevel
	{
		TUTORIAL,
		EASY,
		NORMAL,
		HARD
	};

	enum TutorialPhase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
		PHASE_4,
		PHASE_5
	};
	State CurrentState;
	int CurrentLevel;
	TutorialPhase CurrentPhase;

	UISystem* UIManager;

private:
	void InitMeshes();

	void UpdateHero(double dt);
	void UpdateTutorialInstructions(double dt);
	void UpdateDecks(double dt);
	void UpdateRGBCard(double dt);
	void UpdateTimer(double dt);
	// Handle to the GUI
	void RenderGUI();


	void RenderTutorialInstructions();
	void RenderTrigger();
	void RenderDeck();
	void RenderRGBCards();
	void RenderTimer();
	void RenderScore();
	// Handle to the tilemaps
	void RenderTileMap();
	// Hero's information
	void RenderHero();
	void RenderRearTileMap(){}

	// Enemies
	//vector<CEnemy*> theEnemies;
	void RenderAIs();

	float Timer;

	int ScoreToBeat;
	int Score;

	Deck* PatternToFollow;

	Deck* PatternInserted;

	Card* RedCard;
	Card* GreenCard;
	Card* BlueCard;

	Card* SelectedCard;
	Card* NoneCard;

	Trigger* SendPattern;

	CProbabilitySystem ps;

	Trigger* Instructions;


	float MissingCardTimer;

	//UI
	void InitUI();
	void UpdateUI(double dt);

};

