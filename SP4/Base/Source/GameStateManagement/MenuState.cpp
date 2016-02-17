#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "menustate.h"
#include "LoadGameSelect.h"
#include "playstate.h"
#include "InstructionState.h"
#include "RankingsState.h"
#include "OptionsState.h"

CMenuState CMenuState::theMenuState;

void CMenuState::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Init\n" << endl;
#endif
	scene = new CSceneMenu(800, 600);
	scene->Init(0);
}

void CMenuState::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Init\n" << endl;
#endif
	scene = new CSceneMenu(800, 600);
	scene->Init(level);
}

void CMenuState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CMenuState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Pause\n" << endl;
#endif
}

void CMenuState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Resume\n" << endl;
#endif
}

void CMenuState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CMenuState: Choose one <0> Go to Instruction State, <1> Go to Play State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CInstructionState::Instance() );
	//			break;

	//		case 1:
	//			theGSM->ChangeState( CPlayState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	/*int m_iUserChoice = -1;

	do {
	cout << "CMenuState: Choose one <0> Go to Instruction State, <1> Go to Play State : " ;
	cin >> m_iUserChoice;
	cin.get();

	switch (m_iUserChoice) {
	case 0:
	theGSM->ChangeState( CInstructionState::Instance() );
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

void CMenuState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right,
							  const int width, const int height)
{
#if GSM_DEBUG_MODE
	/*int m_iUserChoice = -1;

	do {
		cout << "CMenuState: Choose one <0> Go to Instruction State, <1> Go to Play State : " ;
		cin >> m_iUserChoice;
		cin.get();

		switch (m_iUserChoice) {
			case 0:
				theGSM->ChangeState( CInstructionState::Instance() );
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
	// Play
	if (width * 0.425 <= mouse_x && mouse_x <= width * 0.575 &&
		height * 0.25 <= mouse_y && mouse_y <= height * 0.34)
	{
		scene->choice = CSceneMenu::PLAY;
		if (button_Left == true)
		{
			theGSM->ChangeState(CLoadGameSelect::Instance());
		}
	}
	// Time-Limit
	else if (width * 0.28 <= mouse_x && mouse_x <= width * 0.72 &&
		height * 0.355 <= mouse_y && mouse_y <= height * 0.45)
	{
		scene->choice = CSceneMenu::TIME_LIMIT;
		if (button_Left == true)
		{
			//theGSM->m_bHideMouse = true;
			//theGSM->m_bWarpMouse = true;
			//theGSM->ChangeState(CPlayState::Instance(), 0);
		}
	}
	// Instruction
	else if (width * 0.28 <= mouse_x && mouse_x <= width * 0.72 &&
		height * 0.465 <= mouse_y && mouse_y <= height * 0.55)
	{
		scene->choice = CSceneMenu::INSTRUCTION;
		if (button_Left == true)
		{
			theGSM->ChangeState(CInstructionState::Instance());
		}
	}
	// Rankings
	else if (width * 0.34 <= mouse_x && mouse_x <= width * 0.66 &&
		height * 0.585 <= mouse_y && mouse_y <= height * 0.675)
	{
		scene->choice = CSceneMenu::RANKINGS;
		if (button_Left == true)
		{
			theGSM->ChangeState(CRankingsState::Instance());
		}
	}
	// Options
	else if (width * 0.355 <= mouse_x && mouse_x <= width * 0.645 &&
		height * 0.69 <= mouse_y && mouse_y <= height * 0.785)
	{
		scene->choice = CSceneMenu::OPTIONS;
		if (button_Left == true)
		{
			theGSM->ChangeState(COptionsState::Instance());
		}
	}
	// Exit
	else if(width * 0.42 <= mouse_x && mouse_x <= width * 0.58 &&
		height * 0.805 <= mouse_y && mouse_y <= height * 0.885)
	{
		scene->choice = CSceneMenu::EXIT;
		if (button_Left == true)
		{
			theGSM->Quit();
		}
	}
	else
	{
		scene->choice = CSceneMenu::NONE;
	}
}

void CMenuState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CMenuState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CMenuState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CMenuState::Draw\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
