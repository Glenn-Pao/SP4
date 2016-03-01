#ifndef SCENE_INSTRUCTION_H
#define SCENE_INSTRUCTION_H

//#include "..\SceneManager2D.h"
#include "..\Master\SceneManager2D.h"
#include "..\..\UISystem.h"

class CSceneInstruction : public Scene
{
	enum GEOMETRY_TYPE
	{
		PAGE_1,

		BACK_BUTTON_UP,
		BACK_BUTTON,
		BACKGROUND,
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
		NUM_CHOICES,
	};

	int choice;

	enum PAGE
	{
		FIRST_PAGE,
		NUM_PAGES,
	};
	int currentPage;
	UISystem* UIManager;
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;

	float UI_Speed;
};

#endif