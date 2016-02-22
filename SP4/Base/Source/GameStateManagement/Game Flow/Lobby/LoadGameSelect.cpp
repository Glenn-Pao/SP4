#include <iostream>
using namespace std;

#include "..\..\Master\GameStateManager.h"
#include "..\..\Master\gamestate.h"
#include "..\..\Menu Flow\MenuState.h"
#include "LoadGameSelect.h"
#include "..\..\Menu Flow\playstate.h"
#include "..\..\Game Flow\Lobby\HubState.h"

CLoadGameSelect CLoadGameSelect::theLoadGameSelectState;

void CLoadGameSelect::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneLoadGame(800, 600);
	scene->Init(0);
}

void CLoadGameSelect::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneLoadGame(800, 600);
	scene->Init(level);
}

void CLoadGameSelect::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CLoadGameSelect::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void CLoadGameSelect::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void CLoadGameSelect::HandleEvents(CGameStateManager* theGSM)
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

void CLoadGameSelect::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
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

void CLoadGameSelect::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
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
		scene->choice = CSceneLoadGame::LEVEL_ONE;
		if ((bool)button_Left == true)
		{
			//theGSM->m_bHideMouse = true;
			//theGSM->m_bWarpMouse = true;
			theGSM->saveAndLoadsys->LoadFile(0);
			theGSM->ChangeState(CHubState::Instance(), 1);
		}
	}
	// Level 2
	else if (width * 0.375 <= mouse_x && mouse_x <= width * 0.63 &&
		height * 0.25 <= mouse_y && mouse_y <= height * 0.5)
	{
		scene->choice = CSceneLoadGame::LEVEL_TWO;
		if ((bool)button_Left == true)
		{
			//theGSM->m_bHideMouse = true;
			//theGSM->m_bWarpMouse = true;
			//theGSM->ChangeState(CPlayState::Instance(), 2);
		}
	}
	// Level 3
	else if (width * 0.7 <= mouse_x && mouse_x <= width * 0.955 &&
		height * 0.25 <= mouse_y && mouse_y <= height * 0.5)
	{
		scene->choice = CSceneLoadGame::LEVEL_THREE;
		if ((bool)button_Left == true)
		{
			//theGSM->m_bHideMouse = true;
			//theGSM->m_bWarpMouse = true;
			//theGSM->ChangeState(CPlayState::Instance(), 3);
		}
	}
	// Level 4
	else if (width * 0.2075 <= mouse_x && mouse_x <= width * 0.4675 &&
		height * 0.55 <= mouse_y && mouse_y <= height * 0.8)
	{
		scene->choice = CSceneLoadGame::LEVEL_FOUR;
		if ((bool)button_Left == true)
		{
			//theGSM->m_bHideMouse = true;
			//theGSM->m_bWarpMouse = true;
			//theGSM->ChangeState(CPlayState::Instance(), 4);
		}
	}
	// Level 5
	else if (width * 0.5375 <= mouse_x && mouse_x <= width * 0.7925 &&
		height * 0.55 <= mouse_y && mouse_y <= height * 0.8)
	{
		scene->choice = CSceneLoadGame::LEVEL_FIVE;
		if ((bool)button_Left == true)
		{
			//theGSM->m_bHideMouse = true;
			//theGSM->m_bWarpMouse = true;
			//theGSM->ChangeState(CPlayState::Instance(), 5);
		}
	}
	// Back
	else if (0 <= mouse_x && mouse_x <= width * 0.2 &&
		height * 0.9 <= mouse_y && mouse_y <= height)
	{
		scene->choice = CSceneLoadGame::BACK;
		if ((bool)button_Left == true)
		{
			theGSM->ChangeState(CMenuState::Instance());
		}
	}
	else
	{
		scene->choice = CSceneMenu::NONE;
	}
}

void CLoadGameSelect::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CLoadGameSelect::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CLoadGameSelect::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Draw\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
