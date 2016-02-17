#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "PauseState.h"
#include "MenuState.h"

CPauseState CPauseState::thePauseState;

void CPauseState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CScenePause(800, 600);
	scene->Init(0);
}

void CPauseState::Init(const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CScenePause(800, 600);
	scene->Init(level);
}

void CPauseState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CPauseState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void CPauseState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void CPauseState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	/*int m_iUserChoice = -1;

	do {
		cout << "CInstructionState: Choose one <0> Go to Menu State, <1> Go to Play State : " ;
		cin >> m_iUserChoice;
		cin.get();

		switch (m_iUserChoice) {
			case 0:
				theGSM->ChangeState( CMenuState::Instance() );
				break;
			case 1:
				theGSM->ChangeState( CPlayState::Instance() );
				break;
			default:
				cout << "Invalid choice!\n";
				m_iUserChoice = -1;
				break;
		}
	} while (m_iUserChoice == -1);*/
#endif
}

void CPauseState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	/*int m_iUserChoice = -1;

	do {
		cout << "CInstructionState: Choose one <0> Go to Menu State, <1> Go to Play State : " ;
		cin >> m_iUserChoice;
		cin.get();

		switch (m_iUserChoice) {
			case 0:
				theGSM->ChangeState( CMenuState::Instance() );
				break;
			case 1:
				theGSM->ChangeState( CPlayState::Instance() );
				break;
			default:
				cout << "Invalid choice!\n";
				m_iUserChoice = -1;
				break;
		}
	} while (m_iUserChoice == -1);*/
#endif
}

void CPauseState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							const int button_Left, const int button_Middle, const int button_Right,
							const int width, const int height)
{
#if GSM_DEBUG_MODE
	/*int m_iUserChoice = -1;

	do {
		cout << "CInstructionState: Choose one <0> Go to Menu State, <1> Go to Play State : " ;
		cin >> m_iUserChoice;
		cin.get();

		switch (m_iUserChoice) {
			case 0:
				theGSM->ChangeState( CMenuState::Instance() );
				break;
			case 1:
				theGSM->ChangeState( CPlayState::Instance() );
				break;
			default:
				cout << "Invalid choice!\n";
				m_iUserChoice = -1;
				break;
		}
	} while (m_iUserChoice == -1);*/
#endif
	// Resume
	if (width * 0.355 <= mouse_x && mouse_x <= width * 0.645 &&
		height * 0.425 <= mouse_y && mouse_y <= height * 0.515)
	{
		scene->choice = CScenePause::RESUME;
		if (button_Left == true)
		{
			theGSM->m_bHideMouse = true;
			theGSM->m_bWarpMouse = true;
			theGSM->PopState();
		}
	}
	// Level Selection
	else if (width * 0.42 <= mouse_x && mouse_x <= width * 0.58 &&
		height * 0.535 <= mouse_y && mouse_y <= height * 0.63)
	{
		scene->choice = CScenePause::QUIT;
		if (button_Left == true)
		{
			//theGSM->Cleanup();
			theGSM->PopState();
			theGSM->ChangeState(CMenuState::Instance());
		}
	}
	else
	{
		scene->choice = CScenePause::NONE;
	}
}

void CPauseState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CPauseState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CPauseState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Draw\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
