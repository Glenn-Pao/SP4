#pragma once

#include "gamestate.h"
#include "..\Scenes\SceneLoadGame.h"

class CLoadGameState : public CGameState
{
public:
	void Init();
	void Init(const int width, const int height, int level);
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
	static CLoadGameState* Instance() {
		return &theLoadGameState;
	}

protected:
	CLoadGameState() { }

private:
	static CLoadGameState theLoadGameState;
	short stateID = 5;
	CSceneLoadGame *scene;
};
