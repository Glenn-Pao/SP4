#ifndef SCENE_RANKINGS_H
#define SCENE_RANKINGS_H

#include "SceneManager2D.h"
#include "../Highscore.h"

class CSceneRankings : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		BLACK_QUAD,
		NUM_GEOMETRY,
	};

public:
	CSceneRankings();
	CSceneRankings(const int m_window_width, const int m_window_height);
	~CSceneRankings();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum CHOICE
	{
		NONE,
		BACK,
		NUM_CHOICES,
	};

	int choice;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;

	Highscore highscore;
};

#endif