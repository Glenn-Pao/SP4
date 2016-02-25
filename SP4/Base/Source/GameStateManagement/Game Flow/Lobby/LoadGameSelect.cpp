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

	// UI
	scene->UIManager->HandleEvent(mouse_x, mouse_y, width, height, scene->sceneManager2D.m_window_width, scene->sceneManager2D.m_window_height);

	if ((bool)button_Left == true)
	{
		const int noOfNbuttons = 5;
		string buttonName[noOfNbuttons] = { "Data1Button", "Data2Button", "Data3Button", "Data4Button", "Data5Button" };
		for (int i = 0; i < noOfNbuttons; i++)
		{
			if (scene->UIManager->FindButton(buttonName[i])->getisHovered() == true)
			{
				//theGSM->m_bHideMouse = true;
				//theGSM->m_bWarpMouse = true;
				theGSM->saveAndLoadsys->LoadFile(i);
				theGSM->ChangeState(CHubState::Instance());
				return;
			}
		}
		// Back Button
		if (scene->UIManager->FindButton("BackButton")->getisHovered() == true)
		{
			theGSM->ChangeState(CMenuState::Instance());
			return;
		}
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
