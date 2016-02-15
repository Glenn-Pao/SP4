#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "SceneManager2D.h"

class CSceneMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		BLACK_QUAD,
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

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;
};

#endif