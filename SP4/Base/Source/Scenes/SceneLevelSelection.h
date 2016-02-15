#ifndef SCENE_LEVEL_SELCTION_H
#define SCENE_LEVEL_SELCTION_H

#include "SceneManager2D.h"

class CSceneLevelSelection : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		BLACK_QUAD,
		NUM_GEOMETRY,
	};

public:
	CSceneLevelSelection();
	CSceneLevelSelection(const int m_window_width, const int m_window_height);
	~CSceneLevelSelection();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum CHOICE
	{
		NONE,
		BACK,
		LEVEL_ONE,
		LEVEL_TWO,
		LEVEL_THREE,
		LEVEL_FOUR,
		LEVEL_FIVE,
		NUM_CHOICES,
	};

	int choice;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;
};

#endif