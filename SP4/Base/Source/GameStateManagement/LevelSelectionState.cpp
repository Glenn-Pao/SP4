#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "LevelSelectionState.h"
#include "playstate.h"

CLevelSelectionState CLevelSelectionState::theLevelSelectionState;

void CLevelSelectionState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneLevelSelection(800, 600);
	scene->Init(0);
}

void CLevelSelectionState::Init(const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneLevelSelection(800, 600);
	scene->Init(level);
}

void CLevelSelectionState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CLevelSelectionState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void CLevelSelectionState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void CLevelSelectionState::HandleEvents(CGameStateManager* theGSM)
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

void CLevelSelectionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
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

void CLevelSelectionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
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
	// Level 1
	if (width * 0.05 <= mouse_x && mouse_x <= width * 0.305 &&
		height * 0.25 <= mouse_y && mouse_y <= height * 0.5)
	{
		scene->choice = CSceneLevelSelection::LEVEL_ONE;
		if (button_Left == true)
		{
			theGSM->m_bHideMouse = true;
			theGSM->m_bWarpMouse = true;
			theGSM->ChangeState(CPlayState::Instance(), 1);
		}
	}
	// Level 2
	else if (width * 0.375 <= mouse_x && mouse_x <= width * 0.63 &&
		height * 0.25 <= mouse_y && mouse_y <= height * 0.5)
	{
		scene->choice = CSceneLevelSelection::LEVEL_TWO;
		if (button_Left == true)
		{
			theGSM->m_bHideMouse = true;
			theGSM->m_bWarpMouse = true;
			theGSM->ChangeState(CPlayState::Instance(), 2);
		}
	}
	// Level 3
	else if (width * 0.7 <= mouse_x && mouse_x <= width * 0.955 &&
		height * 0.25 <= mouse_y && mouse_y <= height * 0.5)
	{
		scene->choice = CSceneLevelSelection::LEVEL_THREE;
		if (button_Left == true)
		{
			theGSM->m_bHideMouse = true;
			theGSM->m_bWarpMouse = true;
			theGSM->ChangeState(CPlayState::Instance(), 3);
		}
	}
	// Level 4
	else if (width * 0.2075 <= mouse_x && mouse_x <= width * 0.4675 &&
		height * 0.55 <= mouse_y && mouse_y <= height * 0.8)
	{
		scene->choice = CSceneLevelSelection::LEVEL_FOUR;
		if (button_Left == true)
		{
			theGSM->m_bHideMouse = true;
			theGSM->m_bWarpMouse = true;
			theGSM->ChangeState(CPlayState::Instance(), 4);
		}
	}
	// Level 5
	else if (width * 0.5375 <= mouse_x && mouse_x <= width * 0.7925 &&
		height * 0.55 <= mouse_y && mouse_y <= height * 0.8)
	{
		scene->choice = CSceneLevelSelection::LEVEL_FIVE;
		if (button_Left == true)
		{
			theGSM->m_bHideMouse = true;
			theGSM->m_bWarpMouse = true;
			theGSM->ChangeState(CPlayState::Instance(), 5);
		}
	}
	// Back
	else if (0 <= mouse_x && mouse_x <= width * 0.2 &&
		height * 0.9 <= mouse_y && mouse_y <= height)
	{
		scene->choice = CSceneLevelSelection::BACK;
		if (button_Left == true)
		{
			theGSM->ChangeState(CMenuState::Instance());
		}
	}
	else
	{
		scene->choice = CSceneMenu::NONE;
	}
}

void CLevelSelectionState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CLevelSelectionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CLevelSelectionState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Draw\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
