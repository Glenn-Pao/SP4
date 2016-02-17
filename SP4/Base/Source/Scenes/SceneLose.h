#ifndef SCENE_LOSE_H
#define SCENE_LOSE_H

#include "SceneManager2D.h"

class CSceneLose : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		BLACK_QUAD,
		BLACK_SQUARE,
		NUM_GEOMETRY,
	};

public:
	CSceneLose();
	CSceneLose(const int m_window_width, const int m_window_height);
	~CSceneLose();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//enum CHOICE
	//{
	//	NONE,
	//	BACK,
	//	NUM_CHOICES,
	//};

	/*int choice;

	bool fpsSelected;
	bool fullscreenSelected;
	bool coloredSelected;*/

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;
};

#endif