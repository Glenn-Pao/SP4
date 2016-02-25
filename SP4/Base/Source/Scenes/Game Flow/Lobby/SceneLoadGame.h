#ifndef SCENE_LOAD_GAME_H
#define SCENE_LOAD_GAME_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\..\UISystem.h"

class CSceneLoadGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_HEADER,

		GEO_DATA1_BUTTON_UP,
		GEO_DATA1_BUTTON_DOWN,

		GEO_DATA2_BUTTON_UP,
		GEO_DATA2_BUTTON_DOWN,

		GEO_DATA3_BUTTON_UP,
		GEO_DATA3_BUTTON_DOWN,

		GEO_DATA4_BUTTON_UP,
		GEO_DATA4_BUTTON_DOWN,

		GEO_DATA5_BUTTON_UP,
		GEO_DATA5_BUTTON_DOWN,

		GEO_BACK_BUTTON_UP,
		GEO_BACK_BUTTON_DOWN,

		NUM_GEOMETRY,
	};

public:
	CSceneLoadGame();
	CSceneLoadGame(const int m_window_width, const int m_window_height);
	~CSceneLoadGame();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	UISystem* UIManager;

	CSceneManager2D sceneManager2D;
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
};

#endif