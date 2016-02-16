#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Scenes\SceneManager.h"
#include "Scenes\SceneManager2D.h"
#include "GameStateManagement\GameStateManager.h"
#include "GameStateManagement\InstructionState.h"
#include "GameStateManagement\MenuState.h"
#include "GameStateManagement\PlayState.h"
#include "GameStateManagement\PauseState.h"
#include "GameStateManagement\SplashState.h"
#include "GameStateManagement\HubState.h"
#include "Sound.h"
#define TYPE_OF_VIEW 2	// 2 = 2D, 3 = 3D

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}

	static bool IsKeyPressed(unsigned short key);
	bool GetMouseUpdate();
	bool GetKeyboardUpdate();

	void Init();
	void Run();
	void Exit();
	int ReadLuaTextFile();

	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

	// The handler for the scene
	#if TYPE_OF_VIEW == 3
		CSceneManager *scene;	// Use this for 3D gameplay
	#else
		CSceneManager2D *scene;	// Use this for 2D gameplay
	#endif

	// Game State Management
	CGameStateManager* theGSM;

	//Sound system
	CSound *sound;
};

#endif