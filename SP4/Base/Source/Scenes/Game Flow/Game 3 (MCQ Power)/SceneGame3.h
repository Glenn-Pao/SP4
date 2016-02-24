#ifndef SCENE_GAME_3_H
#define SCENE_GAME_3_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\Master\SceneGameBase.h"
#include "..\..\..\GameMechanics\Objects\Question.h"
#include "..\..\..\GameMechanics\Objects\Answer.h"
#include "..\..\..\GameMechanics\Objects\Door.h"

class CSceneGame3 : public CSceneGameBase
{
public:
	CSceneGame3(const int m_window_width, const int m_window_height);
	~CSceneGame3();

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

	//Doors
	vector<CDoor*> theDoors;

	//Answers
	vector<CAnswer*>theAnswers;

	////door
	//CDoor* theDoor;

	//tutorial dialogues
	vector<CObjects*> dialogueTiles;

	CSceneManager2D sceneManager2D;	
	int Qcount;	//number of questions
	int Acount;		//number of answers
	int Dcount;		//number of doors
	int lives;		//number of lives available to player
	double timer;	//meant for the slight pause before you can pick up item again
	double feedbacktimer;		//meant to display notice for a while as visual feedback on whether you got it right or wrong
	bool press;		//if you pressed to pick up item or confirmed your answer
	bool feedback;
	bool correct;
	bool unlock;	//unlock the door
	string scriptWrong, scriptCorrect, scriptFinished, scriptExit;

	float fps;		//debugging frames
};

#endif