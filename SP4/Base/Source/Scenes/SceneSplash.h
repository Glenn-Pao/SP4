#ifndef SCENE_SPLASH_H
#define SCENE_SPLASH_H

#include "SceneManager2D.h"

class CSceneSplash : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_RAINBOW_WHEEL,
		NUM_GEOMETRY,
	};

public:
	CSceneSplash();
	CSceneSplash(const int m_window_width, const int m_window_height);
	~CSceneSplash();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum STATE{
		LOADING,
		LOADED,
		EXITING,
		EXIT,
		NUM_OF_STATES
	};

	STATE getState(){ return state; };
	void setState(STATE state){ this->state = state; };
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	CSceneManager2D sceneManager2D;

	float timeElapsed;
	float noOfDots;
	STATE state;
	float ringScaleOffset;
};

#endif