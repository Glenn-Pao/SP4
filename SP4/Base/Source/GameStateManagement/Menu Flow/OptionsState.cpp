#include <iostream>
using namespace std;

#include "OptionsState.h"
#include "menustate.h"

COptionsState COptionsState::theOptionsState;

void COptionsState::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneOptions(800, 600);
	scene->Init(0);
}

void COptionsState::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneOptions(800, 600);
	scene->Init(level);
}

void COptionsState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void COptionsState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void COptionsState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void COptionsState::HandleEvents(CGameStateManager* theGSM)
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

void COptionsState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
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
	if (status)
	{
		if (key == 32)
		{
			theGSM->ChangeState(CMenuState::Instance());
		}
	}
}

void COptionsState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
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
	// Back
	if (0 <= mouse_x && mouse_x <= width * 0.2 &&
		height * 0.9 <= mouse_y && mouse_y <= height)
	{
		scene->choice = CSceneOptions::BACK;
		if (button_Left)
		{
			theGSM->ChangeState(CMenuState::Instance());
		}
	}
	// FPS
	else if (width * 0.525 <= mouse_x && mouse_x <= width * 0.6 &&
		height * 0.225 <= mouse_y && mouse_y <= height * 0.3)
	{
		if (button_Left)
		{
			if (scene->fpsSelected)
				scene->fpsSelected = false;
			else
				scene->fpsSelected = true;
		}
	}
	// Fullscreen
	else if (width * 0.525 <= mouse_x && mouse_x <= width * 0.6 &&
		height * 0.385 <= mouse_y && mouse_y <= height * 0.46)
	{
		if (button_Left )
		{
			if (scene->fullscreenSelected)
			{
				scene->fullscreenSelected = false;
			}
			else
			{
				scene->fullscreenSelected = true;
			}
		}
	}
	// Colored
	else if (width * 0.525 <= mouse_x && mouse_x <= width * 0.6 &&
		height * 0.545 <= mouse_y && mouse_y <= height * 0.62)
	{
		if (button_Left)
		{
			if (scene->coloredSelected)
			{
				scene->coloredSelected = false;
			}
			else
			{
				scene->coloredSelected = true;
			}
		}
	}
	else
	{
		scene->choice = CSceneMenu::NONE;
	}
}

void COptionsState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void COptionsState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void COptionsState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Draw\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
