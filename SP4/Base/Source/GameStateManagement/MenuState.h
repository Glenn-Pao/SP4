#pragma once

#include "gamestate.h"
#include "..\Scenes\SceneMenu.h"

class CMenuState : public CGameState
{
public:
	void Init(CGameStateManager* theGSM);
	void Init(CGameStateManager* theGSM, const int width, const int height, int level);
	void Cleanup();

	void Pause();
	void Resume();
	
	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right,
							  const int width, const int height);
	void Update(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM, const double m_dElapsedTime);
	void Draw(CGameStateManager* theGSM);

	short GetStateID() { return stateID; };

	static CMenuState* Instance() {
		return &theMenuState;
	}

protected:
	CMenuState() { }

private:
	static CMenuState theMenuState;
	short stateID = 1;

	CSceneMenu *scene;
};
