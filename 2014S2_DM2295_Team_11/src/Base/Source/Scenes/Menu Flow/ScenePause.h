#ifndef SCENE_PAUSE_H
#define SCENE_PAUSE_H

#include "..\Master\SceneManager2D.h"
#include "..\..\UISystem.h"

class CScenePause : public Scene
{
	enum GEOMETRY_TYPE
	{
		//BACKGROUND
		BACKGROUND,
		
		//BUTTONS
		PAUSE,
		SAVE,
		SAVE_DOWN,
		SAVE_LOCKED,
		RESUME,
		RESUME_PRESSED,
		QUIT,
		QUIT_PRESSED,
		MAIN_MENU,
		MAIN_MENU_PRESSED,

		// CONFIRMATION
		GEO_CONFIRMATION_WINDOW,

		GEO_YES_BUTTON_UP,
		GEO_YES_BUTTON_DOWN,

		GEO_NO_BUTTON_UP,
		GEO_NO_BUTTON_DOWN,

		// Alpha Black Quad
		GEO_ALPHA_BLACK_QUAD,

		NUM_GEOMETRY,
	};

public:
	CScenePause();
	CScenePause(const int m_window_width, const int m_window_height);
	~CScenePause();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum STATE
	{
		GAME,
		LOBBY,
		CONFIRMATION,
		SAVE_GAME,
		QUIT_GAME,
		TO_MAIN_MENU,
		NUM_STATES
	};
	
	STATE currentState;
	STATE prevState;
	STATE firstState;						//to backtrack later on..
	UISystem* UIManager;				//for lobby
	UISystem* UIManagerConfirmation;	//confirm the choice of the player
	CSceneManager2D sceneManager2D;

	void HideConfirmation();
	void ShowConfirmation();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	float UI_Speed;
};

#endif