#ifndef SCENE_LOAD_GAME_H
#define SCENE_LOAD_GAME_H

#include "..\..\Master\SceneManager2D.h"
#include "..\..\..\UISystem.h"

class CSceneLoadGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		// HEADER
		GEO_HEADER,

		// BUTTONS
		GEO_DATA1_BUTTON_UP,
		GEO_DATA1_BUTTON_DOWN,

		GEO_DATA2_BUTTON_UP,
		GEO_DATA2_BUTTON_DOWN,

		GEO_DATA3_BUTTON_UP,
		GEO_DATA3_BUTTON_DOWN,

		GEO_DATA4_BUTTON_UP,
		GEO_DATA4_BUTTON_DOWN,

		GEO_DATA5_BUTTON_UP,
		GEO_DATA5_BUTTON_DOWN,

		GEO_BACK_BUTTON_UP,
		GEO_BACK_BUTTON_DOWN,
		
		// CONFIRMATION
		GEO_CONFIRMATION_WINDOW,

		GEO_YES_BUTTON_UP,
		GEO_YES_BUTTON_DOWN,

		GEO_NO_BUTTON_UP,
		GEO_NO_BUTTON_DOWN,

		// Alpha Black Quad
		GEO_ALPHA_BLACK_QUAD,

		NUM_GEOMETRY,
	};
public:
	CSceneLoadGame();
	CSceneLoadGame(const int m_window_width, const int m_window_height);
	~CSceneLoadGame();

	virtual void Init(int level);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	UISystem* UIManagerSelecting;
	UISystem* UIManagerConfirmation;

	CSceneManager2D sceneManager2D;

	enum STATE
	{
		SELECTING,
		CONFIRMATION,
	};
	int currentState;

	int DataSelected;

	void HideConfirmation();
	void ShowConfirmation();
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	float UI_Speed;
};

#endif