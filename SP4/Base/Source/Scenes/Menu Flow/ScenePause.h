#ifndef SCENE_PAUSE_H
#define SCENE_PAUSE_H

#include "..\Master\SceneManager2D.h"

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

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;
};

#endif