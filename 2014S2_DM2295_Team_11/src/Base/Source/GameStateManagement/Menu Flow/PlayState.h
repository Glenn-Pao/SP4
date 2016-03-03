#pragma once

#include "../Master/GameState.h"
#include "..\..\Scenes\Master\SceneManager.h"
#include "..\..\Scenes\Menu Flow\ScenePlay.h"
//#include "..\Scenes\ScenePlay2D.h"
#include "timer.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

#define TYPE_OF_VIEW 2	// 2 = 2D, 3 = 3D

class CPlayState : public CGameState
{
public:
	void Init(CGameStateManager* theGSM);
	void Init(CGameStateManager* theGSM, const int width, const int height, int level = 0);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
					const int button_Left, const int button_Middle, const int button_Right,
					const int width, const int height);
	void Update(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM, const double m_dElapsedTime);
	void Draw(CGameStateManager* theGSM);
	short GetStateID() { return stateID; };

	static CPlayState* Instance() {
		return &thePlayState;
	}

protected:
	CPlayState() { }

private:
	static CPlayState thePlayState;
	int counter;

	short stateID = 2;				//ID state to play the appropriate sound

	//Declare variables to store the last and current mouse position
	double mouse_last_x, mouse_last_y, mouse_diff_x, mouse_diff_y;
	double camera_yaw, camera_pitch;

	GLFWwindow* m_window;

	// The handler for the scene
	#if TYPE_OF_VIEW == 3
		CSceneManager *scene;	// Use this for 3D gameplay
	#else
		CScenePlay *scene;	// Use this for 2D gameplay
	#endif
};
