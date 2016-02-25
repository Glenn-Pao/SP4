#ifndef SCENE_PAUSE_H
#define SCENE_PAUSE_H

#include "..\Master\SceneManager2D.h"
#include "..\..\UISystem.h"

class CScenePause : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		BLACK_QUAD,
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

	enum CHOICE
	{
		NONE,
		RESUME,
		SAVE,
		QUIT,
		NUM_CHOICES,
	};
	int choice;

	enum STATE
	{
		GAME,
		LOBBY,
		NUM_STATES
	};
	
	STATE currentState;
	UISystem* UIManager;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	void UpdateUI(double dt);
	void UpdateUI_Lobby();
	void UpdateUI_Game();

	CSceneManager2D sceneManager2D;
};

#endif