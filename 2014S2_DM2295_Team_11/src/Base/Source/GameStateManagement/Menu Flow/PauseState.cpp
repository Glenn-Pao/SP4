#include <iostream>
using namespace std;

#include "../Master/GameStateManager.h"
#include "../Master/GameState.h"
#include "../Game Flow/Lobby/HubState.h"
#include "PauseState.h"
#include "MenuState.h"

CPauseState CPauseState::thePauseState;

void CPauseState::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif
	scene = new CScenePause(800, 600);
	scene->Init(0);
}

void CPauseState::Init(CGameStateManager* theGSM, const int width, const int height, int level)
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

	//UI component
	switch (scene->currentState)
	{
		//when accessed while playing in lobby
	case CScenePause::LOBBY:
	{
		scene->firstState = CScenePause::LOBBY;
		// Play
		scene->UIManager->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
	}
		break;
		//when accessed while playing mini game
	case CScenePause::GAME:
	{
		scene->firstState = CScenePause::GAME;
		// Play
		scene->UIManager->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
	}
		break;
		//confirmation of choice
	case CScenePause::CONFIRMATION:
	{
		scene->UIManagerConfirmation->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
	}
		break;
	}
	if (button_Left)
	{
		switch (scene->currentState)
		{
			//has the save button
		case CScenePause::LOBBY:
		{
			if (scene->UIManager->FindButton("ResumeButton")->getisHovered() == true)
			{
				//resume the game
				theGSM->PopState();
				return;
			}
			//save the game's progress or quit game shows confirm window
			if (scene->UIManager->FindButton("SaveButton")->getisHovered() == true)
			{
				scene->prevState = CScenePause::SAVE_GAME;
				scene->currentState = CScenePause::CONFIRMATION;
				scene->ShowConfirmation();
			}
			//quit game should show confirm window
			if (scene->UIManager->FindButton("QuitButton")->getisHovered() == true)
			{
				scene->prevState = CScenePause::TO_MAIN_MENU;
				scene->currentState = CScenePause::CONFIRMATION;
				scene->ShowConfirmation();
			}
		}
			break;
			//does not have the save button
		case CScenePause::GAME:
		{
			if (scene->UIManager->FindButton("ResumeButton")->getisHovered() == true)
			{
				//resume the game
				theGSM->PopState();
				return;
			}
			//quit game should show confirm window, returns to game hub
			if (scene->UIManager->FindButton("QuitButton")->getisHovered() == true)
			{
				scene->prevState = CScenePause::QUIT_GAME;
				scene->currentState = CScenePause::CONFIRMATION;
				scene->ShowConfirmation();
			}
			//quit game should show confirm window, returns to main menu
			if (scene->UIManager->FindButton("MainMenuButton")->getisHovered() == true)
			{
				scene->prevState = CScenePause::TO_MAIN_MENU;
				scene->currentState = CScenePause::CONFIRMATION;
				scene->ShowConfirmation();
			}
		}
			break;
		case CScenePause::CONFIRMATION:
		{
			// Yes Button
			if (scene->UIManagerConfirmation->FindButton("YesButton")->getisHovered() == true)
			{
				if (scene->prevState == CScenePause::SAVE_GAME)
				{
					//save the progress
					theGSM->saveAndLoadsys->to_save = true;
					theGSM->PopState();
					cout << "Saved!" << endl;
					return;
				}
				if (scene->prevState == CScenePause::QUIT_GAME)
				{
					if (scene->firstState == CScenePause::LOBBY)
					{
						//add a confirmation window before proceeding
						theGSM->PopState();
						theGSM->ChangeState(CMenuState::Instance());
						return;
					}
					else if (scene->firstState == CScenePause::GAME)
					{
						//add a confirmation window before proceeding
						theGSM->PopState();
						theGSM->ChangeState(CHubState::Instance());
						return;
					}
				}
				if (scene->prevState == CScenePause::TO_MAIN_MENU)
				{
					//add a confirmation window before proceeding
					theGSM->PopState();
					theGSM->ChangeState(CMenuState::Instance());
					return;
				}

			}
			// No Button
			if (scene->UIManagerConfirmation->FindButton("NoButton")->getisHovered() == true)
			{
				if (scene->firstState == CScenePause::LOBBY)
				{
					scene->currentState = CScenePause::LOBBY;
				}
				else if (scene->firstState == CScenePause::GAME)
				{
					scene->currentState = CScenePause::GAME;
				}
				scene->HideConfirmation();
			}
		}
			break;
		}
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
