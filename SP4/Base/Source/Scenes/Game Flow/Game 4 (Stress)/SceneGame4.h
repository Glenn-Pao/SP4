#pragma once
#include "..\..\Master\SceneGameBase.h"
#include "..\..\Base\Source\GameMechanics\Objects\Deck.h"
#include "..\..\Base\Source\GameMechanics\Objects\Trigger.h"
#include "..\..\Base\Source\AI\ProbabilitySystem.h"

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

	enum STATE
	{
		PLAY,
		TIME_UP,
		PAUSE,
		WIN,
		LOSE,
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

	float timer;

	float ScoreToBeat;
	float Score;

	Deck* PatternToFollow;

	Deck* PatternInserted;

	Card* RedPile;
	Card* GreenPile;
	Card* BluePile;

	Card* SelectedCard;
	Card* NoneCard;

	Trigger* SendPattern;

	CProbabilitySystem ps;

};

