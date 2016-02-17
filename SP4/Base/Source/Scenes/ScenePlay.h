#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H

#include "SceneManager2D.h"

class CScenePlay : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_RAINBOW_WHEEL,
		NUM_GEOMETRY,
	};

public:
	CScenePlay();
	CScenePlay(const int m_window_width, const int m_window_height);
	~CScenePlay();

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