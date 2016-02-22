#ifndef SCENE_TUTORIAL_GAME_3_H
#define SCENE_TUTORIAL_GAME_3_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
#include "..\..\..\GameMechanics\Objects\Question.h"
#include "..\..\..\GameMechanics\Objects\Answer.h"
#include "..\..\..\GameMechanics\Objects\Door.h"

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
	enum STATE
	{
		PLAYING,
		EXITING,
		COMPLETED,
		FAILED,
		NUM_OF_STATE
	};
	int currentState;
	void RenderWaypoints();
private:
	void InitMeshes();
	// Handle to the tilemaps
	void RenderTileMap();
	// Hero's information
	void RenderHero();

	void RenderRearTileMap(){}

	// Enemies
	//vector<CEnemy*> theEnemies;
	void RenderAIs();
	void RenderGUI();	//the game interface

	//Questions
	vector<CQuestion*>theQuestions;

	//Answers
	vector<CAnswer*>theAnswers;

	//tutorial dialogues
	vector<CObjects*> dialogueTiles;

	//Door
	CDoor* theDoor;

	CSceneManager2D sceneManager2D;	
	int Qcount;	//number of questions
	int Acount;		//number of answers
	int lives;		//number of lives available to player

	string scriptWrong, scriptCorrect, scriptFinished, scriptExit;
};

#endif