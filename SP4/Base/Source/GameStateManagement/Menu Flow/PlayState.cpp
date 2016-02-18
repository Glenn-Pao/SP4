#include <iostream>
using namespace std;

#include "../Master/GameStateManager.h"
//#include "HubState.h"
#include "../Game Flow/Tutorial Games/TutorialGame1.h"
#include "MenuState.h"
#include "PlayState.h"

CPlayState CPlayState::thePlayState;

void CPlayState::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CScenePlay(800, 600);
	scene->Init(0);

}

void CPlayState::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CScenePlay(800, 600);
	scene->Init(level);

}

void CPlayState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif


	scene->Exit();
	delete scene;
	scene = NULL;
}

void CPlayState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void CPlayState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void CPlayState::HandleEvents(CGameStateManager* theGSM)
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

void CPlayState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
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
			theGSM->ChangeState(CTutorialGame1::Instance());
		}
	}
}

void CPlayState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
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
	switch (scene->getState())
	{
	case CScenePlay::LOADED:
		if (button_Left == true)
		{
			scene->setState(CScenePlay::EXITING);
		}
		break;
	case CScenePlay::EXIT:
		theGSM->ChangeState(CTutorialGame1::Instance());
		break;
	}
}

void CPlayState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CPlayState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CPlayState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Draw\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
