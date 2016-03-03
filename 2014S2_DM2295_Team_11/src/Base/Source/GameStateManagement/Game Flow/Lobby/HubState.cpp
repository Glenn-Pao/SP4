#include <iostream>
using namespace std;

#include "..\..\Master\GameStateManager.h"
#include "..\..\Master\gamestate.h"
#include "..\..\Menu Flow\MenuState.h"
#include "HubState.h"
#include "..\..\Menu Flow\pausestate.h"
#include "..\..\Game Flow\Game 1 (Maze Escaper)\Game1State.h"
#include "..\..\Game Flow\Game 2 (Puzzle Quest)\Game2State.h"
//#include "..\..\Game Flow\Tutorial Games\TutorialGame3.h"
#include "..\..\Base\Source\GameStateManagement\Game Flow\Game 3 (MCQ Power)\Game3State.h"
#include "..\..\Base\Source\GameStateManagement\Game Flow\Game 4 (Stress)\Game4State.h"

CHubState CHubState::theHubState;

void CHubState::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CHubState::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(800, 600);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(800, 600);	// Use this for 2D gameplay
	scene = new CSceneHub(800, 600);	// Use this for 2D gameplay
#endif
	scene->Init(1);
	scene->SetHeroOffset();
	scene->ReadData(theGSM->saveAndLoadsys->GetCurrentGameInfo());
}

void CHubState::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CHubState::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(width, height);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(width, height);	// Use this for 2D gameplay
	scene = new CSceneHub(width, height);
#endif
	scene->Init(level);
	scene->SetHeroOffset();
	scene->ReadData(theGSM->saveAndLoadsys->GetCurrentGameInfo());
}

void CHubState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CHubState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CHubState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CHubState::Pause\n" << endl;
#endif
}

void CHubState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CHubState::Resume\n" << endl;
#endif

	scene->PreInit();
#if	TYPE_OF_VIEW == 3
	scene->currentState = CSceneManager::STARTING;
#endif
}

void CHubState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CHubState: Choose one <0> Go to Menu State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CMenuState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif
}

void CHubState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CHubState: Choose one <0> Go to Menu State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CMenuState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif

#if	TYPE_OF_VIEW == 3
	if (scene->currentState == CSceneManager::PLAYING)
	{
		if (key == 'p')
		{
			theGSM->m_bUnhideMouse = true;
			theGSM->m_bWarpMouse = false;

			theGSM->PushState(CPauseState::Instance());
		}
		else
		{
			scene->UpdateAvatarStatus(key, status);
		}
	}
#else
	if (key == 'p')
	{
		theGSM->m_bUnhideMouse = true;
		theGSM->m_bWarpMouse = false;

		theGSM->PushState(CPauseState::Instance(), 0);						//indicates that it is from hub state
		if (theGSM->saveAndLoadsys->to_save)
		{
			scene->StoreData(theGSM->saveAndLoadsys->GetCurrentGameInfo());
			theGSM->saveAndLoadsys->to_save = false;
		}
	}
	if (key == 'f' && scene->currentState == CSceneHub::PLAYING)
	{
		if (scene->game_interacted > CSceneHub::NO_GAME && scene->game_interacted < CSceneHub::EXIT_GAME)
		{
			// Set current state to difficulty selection
			scene->currentState = CSceneHub::DIFFICULTY_SELECTION;
			// check unlocked difficulty
			int unlocked = theGSM->saveAndLoadsys->GetCurrentGameInfo()->DifficultySystems[scene->game_interacted - 1].getCurrentDifficultyUnlocked();

			// Set each difficulty buttons to be locked or unlocked
			string buttonName[4] = { "TutorialButton", "EasyButton", "MediumButton", "HardButton" };
			for (int i = 0; i < 4; i++)
			{
				if (i <= unlocked)
				{
					scene->UIManagerDifficultySelection->FindButton(buttonName[i])->setisLocked(false);
				}
				else
				{
					scene->UIManagerDifficultySelection->FindButton(buttonName[i])->setisLocked(true);
				}
			}
			scene->ChangeUI_DifficultySelection();
		}
		else if (scene->game_interacted == CSceneHub::EXIT_GAME)
		{
			scene->currentState = CSceneHub::EXITING;

			scene->StoreData(theGSM->saveAndLoadsys->GetCurrentGameInfo());
			theGSM->saveAndLoadsys->SaveToFile();
		}
		else if (scene->targetNPC != NULL)
		{
			if (scene->targetNPC->GetAI_Type() == CAI_Idling::GUARDIAN && scene->targetNPC->GetCurrentWaypointIndex() == 0)
			{
				// Set current state to GIVING_JELLYBEANS
				scene->currentState = CSceneHub::GIVING_JELLYBEANS;

				scene->ChangeUI_GivingJellybeans();


				// Check if enought jellybeans to finish
				if (scene->noOfJellybeans >= scene->jellybeansRequiredToFinish)
				{
					scene->UIManagerGivingJellybeans->FindButton("YesButton")->setisLocked(false);
				}
				else
				{
					scene->UIManagerGivingJellybeans->FindButton("YesButton")->setisLocked(true);
				}
			}
			else
			{
				// Set current state to INTERACTING
				scene->currentState = CSceneHub::INTERACTING;
			}
		}
	}
#endif
}

void CHubState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right,
	const int width, const int height)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CHubState: Choose one <0> Go to Menu State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CMenuState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif
#if	TYPE_OF_VIEW == 3
	switch (scene->currentState)
	{
	case CSceneManager::PRE_START:
	{
		if (button_Left == true)
			scene->currentState = CSceneManager::STARTING;
	}
		break;
	case CSceneManager::LEVEL_COMPLETED:
	{
		if (button_Left == true)
		{
			theGSM->m_bUnhideMouse = true;
			theGSM->m_bWarpMouse = false;

			theGSM->ChangeState(CLevelSelectionState::Instance());
		}
	}
		break;
	case CSceneManager::GAMEOVER:
	{
		if (button_Left == true)
		{
			theGSM->m_bUnhideMouse = true;
			theGSM->m_bWarpMouse = false;

			theGSM->ChangeState(CLevelSelectionState::Instance());
		}
	}
		break;
	case CSceneManager::GAMEOVER_TIME_LIMIT_END:
	{
		if (button_Left == true)
		{
			theGSM->m_bUnhideMouse = true;
			theGSM->m_bWarpMouse = false;

			theGSM->ChangeState(CMenuState::Instance());
		}
	}
		break;
	case CSceneManager::PLAYING:
	{
		if (button_Left == true)
			scene->UpdateWeaponStatus(scene->WA_FIRE);
		else if (button_Right == true)
			scene->UpdateWeaponStatus(scene->WA_FIRE_SECONDARY);
	}
		break;
	}
#endif

	// UI
	switch (scene->currentState)
	{
		case CSceneHub::DIFFICULTY_SELECTION:
		{
			scene->UIManagerDifficultySelection->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
		}
		case CSceneHub::JELLYBEAN_SELECTION:
		{
			scene->UIManagerJellybeanSelection->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
		}
		break;
		case CSceneHub::CONFIRMATION:
		{
			scene->UIManagerConfirmation->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
		}
		break;
		case CSceneHub::GIVING_JELLYBEANS:
		{
			scene->UIManagerGivingJellybeans->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);
		}
		break;
	}


	if (button_Left)
	{
		switch (scene->currentState)
		{
		case CSceneHub::LOST:
		{
			scene->currentState = CSceneHub::EXITING;
		}
		break;
		case CSceneHub::DIFFICULTY_SELECTION:
		{
			string buttonName[4] = { "TutorialButton", "EasyButton", "MediumButton", "HardButton" };
			for (int i = 0; i < 4; i++)
			{
				if (scene->UIManagerDifficultySelection->FindButton(buttonName[i])->getisHovered() == true)
				{
					// Set scene difficulty
					scene->difficultySelected = i;
					if (i == 0)
					{
						scene->currentState = CSceneHub::CONFIRMATION;
						scene->ChangeUI_Confirmation();
					}
					else
					{
						// Set current deposit jellybean to min
						scene->noOfJellybeansDeposited = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetMinNumOfJellybeansCanBeDeposited(i);
						CheckJellybeanSelectionButtons(theGSM);

						// Set State to jellybean selection state
						scene->currentState = CSceneHub::JELLYBEAN_SELECTION;
						scene->ChangeUI_JellybeanSelection();
					}
				}
			}
			// Back Button
			if (scene->UIManagerDifficultySelection->FindButton("BackButton")->getisHovered() == true)
			{
				scene->currentState = CSceneHub::PLAYING;
				scene->ChangeUI_Playing();
			}
		}
		break;
		case CSceneHub::JELLYBEAN_SELECTION:
		{
			string buttonName[3] = { "UpArrowButton", "DownArrowButton", "EnterButton" };
			for (int i = 0; i < 3; i++)
			{
				if (scene->UIManagerJellybeanSelection->FindButton(buttonName[i])->getisHovered() == true)
				{
					switch (i)
					{
					case 0:
					{
						if (scene->UIManagerJellybeanSelection->FindButton(buttonName[i])->getisLocked() == false)
						{
							scene->noOfJellybeansDeposited++;

							CheckJellybeanSelectionButtons(theGSM);
						}
					}
					break;
					case 1:
					{
						if (scene->UIManagerJellybeanSelection->FindButton(buttonName[i])->getisLocked() == false)
						{
							scene->noOfJellybeansDeposited--;

							CheckJellybeanSelectionButtons(theGSM);
						}
					}
					break;
					case 2:
					{
						scene->currentState = CSceneHub::CONFIRMATION;
						scene->ChangeUI_Confirmation();
					}
					break;
					}
				}
			}
			// Back Button
			if (scene->UIManagerJellybeanSelection->FindButton("BackButton")->getisHovered() == true)
			{
				scene->currentState = CSceneHub::DIFFICULTY_SELECTION;
				scene->ChangeUI_DifficultySelection();
			}
		}
		break;
		case CSceneHub::CONFIRMATION:
		{
			// Yes Button
			if (scene->UIManagerConfirmation->FindButton("YesButton")->getisHovered() == true)
			{
				PlayMiniGame(theGSM);
				return;
			}
			// No Button
			if (scene->UIManagerConfirmation->FindButton("NoButton")->getisHovered() == true)
			{
				// Tutorial
				if (scene->difficultySelected == 0)
				{
					scene->currentState = CSceneHub::DIFFICULTY_SELECTION;
					scene->ChangeUI_DifficultySelection();
				}
				else
				{
					scene->currentState = CSceneHub::JELLYBEAN_SELECTION;
					scene->ChangeUI_JellybeanSelection();
				}
			}
		}
		break;
		case CSceneHub::INTERACTING:
		{
			if (scene->targetNPC->GetAI_Type() == CAI_Idling::TIP_ADVISOR)
				scene->ChangeTip();

			scene->currentState = CSceneHub::PLAYING;
		}
		break;
		case CSceneHub::GIVING_JELLYBEANS:
		{
			// Yes Button
			if (scene->UIManagerGivingJellybeans->FindButton("YesButton")->getisHovered() == true)
			{
				// Take jeelybeans
				scene->noOfJellybeans -= scene->jellybeansRequiredToFinish;
				theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.SetNumOfJellybeans(theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeans() - scene->jellybeansRequiredToFinish);

				scene->currentState = CSceneHub::CLEARING_WAY;
				scene->ChangeUI_Playing();
				scene->targetNPC->SetTargetWaypointIndex(1);
			}
			// No Button
			if (scene->UIManagerGivingJellybeans->FindButton("NoButton")->getisHovered() == true)
			{
				scene->currentState = CSceneHub::PLAYING;
				scene->ChangeUI_Playing();
			}
		}
		break;
		}
	}
}

void CHubState::CheckJellybeanSelectionButtons(CGameStateManager* theGSM)
{
	// Check if current more than or equal to max
	if (scene->noOfJellybeansDeposited >= theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetMaxNumOfJellybeansCanBeDeposited(scene->difficultySelected))
	{
		scene->UIManagerJellybeanSelection->FindButton("UpArrowButton")->setisLocked(true);
	}
	else
	{
		scene->UIManagerJellybeanSelection->FindButton("UpArrowButton")->setisLocked(false);
	}
	// Check if current less than or equal to min
	if (scene->noOfJellybeansDeposited <= theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetMinNumOfJellybeansCanBeDeposited(scene->difficultySelected))
	{
		scene->UIManagerJellybeanSelection->FindButton("DownArrowButton")->setisLocked(true);
	}
	else
	{
		scene->UIManagerJellybeanSelection->FindButton("DownArrowButton")->setisLocked(false);
	}
	// Check if player has enough jellybeans
	if (scene->noOfJellybeansDeposited > theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeans())
	{
		scene->UIManagerJellybeanSelection->FindButton("EnterButton")->setisLocked(true);
	}
	else
	{
		scene->UIManagerJellybeanSelection->FindButton("EnterButton")->setisLocked(false);
	}
}

void CHubState::PlayMiniGame(CGameStateManager* theGSM)
{
	theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.DepositJellybeans(scene->noOfJellybeansDeposited);
	scene->StoreData(theGSM->saveAndLoadsys->GetCurrentGameInfo());
	theGSM->saveAndLoadsys->SaveToFile();
	switch (scene->game_interacted)
	{
	case CSceneHub::GAME1:
	{
		theGSM->ChangeState(CGame1State::Instance(), scene->difficultySelected);
		return;
	}
	break;
	case CSceneHub::GAME2:
	{
		theGSM->ChangeState(CGame2State::Instance(), scene->difficultySelected);
		return;
	}
	break;
	case CSceneHub::GAME3:
	{
		theGSM->ChangeState(CGame3State::Instance(), scene->difficultySelected);
		return;
	}
	break;
	case CSceneHub::GAME4:
	{
		theGSM->ChangeState(CGame4State::Instance(), scene->difficultySelected);
		return;
	}
	break;
	}
}



void CHubState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CHubState::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CHubState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);

	if (scene->currentState == CSceneHub::BACK_TO_MENU)
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
}

void CHubState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//cout << "CHubState::Draw : " << counter << "\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
