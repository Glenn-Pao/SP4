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
	for (int i = 0; i < theGSM->saveAndLoadsys->GetNumOfData(); i++)
	{
		if (theGSM->saveAndLoadsys->GetGameInfo(i)->ifNew)
		{
			scene->UIManagerSelecting->FindButton("Data" + to_string(i + 1) + "Button")->setisLocked(true);
			scene->UIManagerSelecting->FindButton("Delete" + to_string(i + 1) + "Button")->setisLocked(true);
		}
	}
}

void CLoadGameSelect::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CSceneLoadGame(800, 600);
	scene->Init(level);
	for (int i = 0; i < theGSM->saveAndLoadsys->GetNumOfData(); i++)
	{
		if (theGSM->saveAndLoadsys->GetGameInfo(i)->ifNew)
		{
			scene->UIManagerSelecting->FindButton("Data" + to_string(i + 1) + "Button")->setisLocked(true);
			scene->UIManagerSelecting->FindButton("Delete" + to_string(i + 1) + "Button")->setisLocked(true);
		}
	}
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
	/*if (status)
	{
		if (key == 32)
		{
			theGSM->ChangeState(CMenuState::Instance());
		}
	}*/
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
	switch (scene->currentState)
	{
		case CSceneLoadGame::SELECTING:
		{
			scene->UIManagerSelecting->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
		}
		break;
		case CSceneLoadGame::CONFIRMATION:
		{
			scene->UIManagerConfirmation->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
		}
		case CSceneLoadGame::DELETING_CONFIRMATION:
		{
			scene->UIManagerConfirmation->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
		}
		break;
	}
	if (button_Left == (int)true)
	{
		switch (scene->currentState)
		{
		case CSceneLoadGame::SELECTING:
		{
			for (int i = 0; i < theGSM->saveAndLoadsys->GetNumOfData(); i++)
			{
				// Data
				if (scene->UIManagerSelecting->FindButton("Data" + to_string(i + 1) + "Button")->getisHovered() == true)
				{
					scene->DataSelected = i;
					scene->currentState = CSceneLoadGame::CONFIRMATION;
					scene->ShowConfirmation();
				}
				// Delete
				else if (scene->UIManagerSelecting->FindButton("Delete" + to_string(i + 1) + "Button")->getisHovered() == true)
				{
					scene->DataSelected = i;
					scene->currentState = CSceneLoadGame::DELETING_CONFIRMATION;
					scene->ShowDeleteConfirmation();
				}
			}
			// Back Button
			if (scene->UIManagerSelecting->FindButton("BackButton")->getisHovered() == true)
			{
				theGSM->ChangeState(CMenuState::Instance());
				return;
			}
		}
		break;
		case CSceneLoadGame::CONFIRMATION:
		{
			// Yes Button
			if (scene->UIManagerConfirmation->FindButton("YesButton")->getisHovered() == true)
			{
				//theGSM->m_bHideMouse = true;
				//theGSM->m_bWarpMouse = true;
				theGSM->saveAndLoadsys->LoadFile(scene->DataSelected);
				theGSM->ChangeState(CHubState::Instance());
				return;
			}
			// No Button
			if (scene->UIManagerConfirmation->FindButton("NoButton")->getisHovered() == true)
			{
				scene->DataSelected = -1;
				scene->currentState = CSceneLoadGame::SELECTING;
				scene->HideConfirmation();
			}
		}
		break;
		case CSceneLoadGame::DELETING_CONFIRMATION:
		{
			// Yes Button
			if (scene->UIManagerConfirmation->FindButton("YesButton")->getisHovered() == true)
			{
				// Delete file
				theGSM->saveAndLoadsys->GetGameInfo(scene->DataSelected)->ClearFile();
				for (int i = 0; i < theGSM->saveAndLoadsys->GetNumOfData(); i++)
				{
					if (theGSM->saveAndLoadsys->GetGameInfo(i)->ifNew)
					{
						scene->UIManagerSelecting->FindButton("Data" + to_string(i + 1) + "Button")->setisLocked(true);
						scene->UIManagerSelecting->FindButton("Delete" + to_string(i + 1) + "Button")->setisLocked(true);
					}
					else
					{
						scene->UIManagerSelecting->FindButton("Data" + to_string(i + 1) + "Button")->setisLocked(false);
						scene->UIManagerSelecting->FindButton("Delete" + to_string(i + 1) + "Button")->setisLocked(false);
					}
				}
				scene->DataSelected = -1;
				scene->currentState = CSceneLoadGame::SELECTING;
				scene->HideDeleteConfirmation();
			}
			// No Button
			if (scene->UIManagerConfirmation->FindButton("NoButton")->getisHovered() == true)
			{
				scene->DataSelected = -1;
				scene->currentState = CSceneLoadGame::SELECTING;
				scene->HideDeleteConfirmation();
			}
		}
		break;
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
