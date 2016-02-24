#pragma once
#include "..\..\Master\SceneGameBase.h"
#include "..\..\Base\Source\GameMechanics\Objects\Deck.h"
#include "..\..\Base\Source\GameMechanics\Objects\Trigger.h"

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
		NUM_OF_STATE
	};
	enum Deck_Type
	{
		DRAW_DECK,
		REDHAND_DECK,
		BLUEHAND_DECK,
		GREENHAND_DECK,
		DISCARD_DECK,
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

	void RenderCard(Card* card);
	// Enemies
	//vector<CEnemy*> theEnemies;
	void RenderAIs();

	float timer;

	vector<Deck*> DeckList;

	Card* SelectedCard;
	bool isStandingOnTrigger;
	bool isCardDrawn;
	bool isCardSelectedR;
	bool isCardSelectedG;
	bool isCardSelectedB;
	bool isPlaced;
};

