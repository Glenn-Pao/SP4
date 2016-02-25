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
	scene->ReadData(theGSM->saveAndLoadsys->GetGameInfo());
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
	scene->ReadData(theGSM->saveAndLoadsys->GetGameInfo());
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

		theGSM->PushState(CPauseState::Instance());
		scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
	}
	if (key == 'f' && scene->currentState == CSceneHub::PLAYING)
	{
		scene->currentState = CSceneHub::SELECTING;
		int unlocked = theGSM->saveAndLoadsys->GetGameInfo()->difficultySystems[scene->game_interacted - 1].getCurrentDifficultyUnlocked();

		string buttonName[4] = { "TutorialButton", "EasyButton", "MediumButton", "HardButton" };
		for (int i = 0; i < 4; i++)
		{
			if (i <= unlocked)
			{
				scene->UIManager->FindButton(buttonName[i])->setisLocked(false);
			}
			else
			{
				scene->UIManager->FindButton(buttonName[i])->setisLocked(true);
			}
		}
		scene->UIManager->InvokeAnimator()->StopAnimations();
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
	scene->UIManager->HandleEvent(mouse_x, mouse_y, width, height, scene->sceneManager2D.m_window_width, scene->sceneManager2D.m_window_height);


	if (button_Left == true)
	{
		string buttonName[4] = { "TutorialButton", "EasyButton", "MediumButton", "HardButton" };
		for (int i = 0; i < 4; i++)
		{
			if (scene->UIManager->FindButton(buttonName[i])->getisHovered() == true)
			{
				switch (scene->game_interacted)
				{
				case CSceneHub::GAME1:
				{
					scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
					theGSM->ChangeState(CGame1State::Instance(), i);
					return;
				}
				break;
				case CSceneHub::GAME2:
				{
					scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
					theGSM->ChangeState(CGame2State::Instance(), i);
					return;
				}
				break;
				case CSceneHub::GAME3:
				{
					scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
					theGSM->ChangeState(CGame3State::Instance(), i);
					return;
				}
				break;
				case CSceneHub::GAME4:
				{
					scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
					theGSM->ChangeState(CGame4State::Instance(), i);
					return;
				}
				break;
				}
			}
		}

		scene->currentState = CSceneHub::PLAYING;
		scene->UIManager->InvokeAnimator()->StopAnimations();
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
}

void CHubState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//cout << "CHubState::Draw : " << counter << "\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
