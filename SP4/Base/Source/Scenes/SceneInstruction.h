#ifndef SCENE_INSTRUCTION_H
#define SCENE_INSTRUCTION_H

#include "SceneManager2D.h"

class CSceneInstruction : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		LIGHT_GRAY_QUAD,
		BLACK_QUAD,
		PAGE_1,
		PAGE_2,
		NUM_GEOMETRY,
	};

public:
	CSceneInstruction();
	CSceneInstruction(const int m_window_width, const int m_window_height);
	~CSceneInstruction();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum CHOICE
	{
		NONE,
		BACK,
		NEXT,
		PREVIOUS,
		NUM_CHOICES,
	};

	int choice;

	enum PAGE
	{
		FIRST_PAGE,
		SECOND_PAGE,
		NUM_PAGES,
	};

	int currentPage;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;
};

#endif