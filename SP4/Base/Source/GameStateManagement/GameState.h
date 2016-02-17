#pragma once

#include "GameStateManager.h"

#define GSM_DEBUG_MODE TRUE

class CGameState
{
public:
	virtual void Init(CGameStateManager* theGSM) = 0;
	virtual void Init(CGameStateManager* theGSM, const int width, const int height, int level = 0) = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(CGameStateManager* theGSM) = 0;
	virtual void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true) = 0;
	virtual void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							const int button_Left, const int button_Middle, const int button_Right,
							const int width, const int height) = 0;
	virtual void Update(CGameStateManager* theGSM) = 0;
	virtual void Update(CGameStateManager* theGSM, const double m_dElapsedTime) = 0;
	virtual void Draw(CGameStateManager* theGSM) = 0;

	void ChangeState(CGameStateManager* theGSM, CGameState* state) {
		theGSM->ChangeState(state);
	}
	virtual short GetStateID() = 0;

protected:
	CGameState() { }
};