#include <iostream>
using namespace std;

#include "WinState.h"
#include "menustate.h"

CWinState CWinState::theWinState;

void CWinState::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneWin(800, 600);
	scene->Init(0);
}

void CWinState::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneWin(width, height);
	scene->Init(level);
}

void CWinState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CWinState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void CWinState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void CWinState::HandleEvents(CGameStateManager* theGSM)
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

void CWinState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
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

void CWinState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
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
	//// Back
	//if (0 <= mouse_x && mouse_x <= width * 0.2 &&
	//	height * 0.9 <= mouse_y && mouse_y <= height)
	//{
	//	scene->choice = CWinState::BACK;
	//	if (button_Left == true)
	//	{
	//		theGSM->ChangeState(CMenuState::Instance());
	//	}
	//}
	//// FPS
	//else if (width * 0.525 <= mouse_x && mouse_x <= width * 0.6 &&
	//	height * 0.225 <= mouse_y && mouse_y <= height * 0.3)
	//{
	//	if (button_Left == true)
	//	{
	//		if (scene->fpsSelected)
	//			scene->fpsSelected = false;
	//		else
	//			scene->fpsSelected = true;
	//	}
	//}
	//// Fullscreen
	//else if (width * 0.525 <= mouse_x && mouse_x <= width * 0.6 &&
	//	height * 0.385 <= mouse_y && mouse_y <= height * 0.46)
	//{
	//	if (button_Left == true)
	//	{
	//		if (scene->fullscreenSelected)
	//		{
	//			scene->fullscreenSelected = false;
	//		}
	//		else
	//		{
	//			scene->fullscreenSelected = true;
	//		}
	//	}
	//}
	//// Colored
	//else if (width * 0.525 <= mouse_x && mouse_x <= width * 0.6 &&
	//	height * 0.545 <= mouse_y && mouse_y <= height * 0.62)
	//{
	//	if (button_Left == true)
	//	{
	//		if (scene->coloredSelected)
	//		{
	//			scene->coloredSelected = false;
	//		}
	//		else
	//		{
	//			scene->coloredSelected = true;
	//		}
	//	}
	//}
	//else
	//{
	//	scene->choice = CSceneMenu::NONE;
	//}
}

void CWinState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CWinState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CWinState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Draw\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
