#ifndef SCENE_OPTIONS_H
#define SCENE_OPTIONS_H

#include "..\Master\SceneManager2D.h"

class CSceneOptions : public Scene
{
	enum GEOMETRY_TYPE
	{
		GRAY_QUAD,
		BLACK_QUAD,
		BLACK_SQUARE,
		NUM_GEOMETRY,
	};

public:
	CSceneOptions();
	CSceneOptions(const int m_window_width, const int m_window_height);
	~CSceneOptions();

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

	bool fpsSelected;
	bool fullscreenSelected;
	bool coloredSelected;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;
};

#endif