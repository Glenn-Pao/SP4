#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "..\Master\SceneManager2D.h"
#include "..\..\UISystem.h"

class CSceneMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		BLACK_QUAD,
		
		START_BUTTON_UP,
		START_BUTTON_DOWN,

		LOAD_BUTTON_UP,
		LOAD_BUTTON_DOWN,

		INSTRUCTIONS_BUTTON_UP,
		INSTRUCTIONS_BUTTON_DOWN,

		SETTINGS_BUTTON_UP,
		SETTINGS_BUTTON_DOWN,

		EXIT_BUTTON_UP,
		EXIT_BUTTON_DOWN,

		TITLE,
		SMOKE_LEFT,
		SMOKE_RIGHT,
		

		MUSIC_ICON,
		MUSIC_ICON_OFF,

		BACKGROUND,
		NUM_GEOMETRY,
	};

public:
	CSceneMenu();
	CSceneMenu(const int m_window_width, const int m_window_height);
	~CSceneMenu();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum CHOICE
	{
		NONE,
		PLAY,
		TIME_LIMIT,
		INSTRUCTION,
		RANKINGS,
		OPTIONS,
		EXIT,
		NUM_CHOICES,
	};

	int choice;
	UISystem* UIManager;
	UISystem* UIInstructionPage1;
	UISystem* UIInstructionPage2;

	CSceneManager2D sceneManager2D;

	float MainMenuAnimationTimer;

	bool isSettingsAnimationStarted;

	float TimeSinceSettingsAnimation;
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
};

#endif