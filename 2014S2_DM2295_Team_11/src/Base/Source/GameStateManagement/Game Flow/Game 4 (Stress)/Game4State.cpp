#include <iostream>
using namespace std;

#include "..\..\Master\GameState.h"
#include "..\..\Master\GameStateManager.h"
#include "Game4State.h"
#include "..\..\Game Flow\Lobby\HubState.h"
#include "..\..\Game Flow\Lobby\LoadGameSelect.h"
#include "..\..\Menu Flow\MenuState.h"
#include "..\..\Menu Flow\PauseState.h"

CGame4State CGame4State::theTutorialGame1State;

void CGame4State::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CGame4State::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(800, 600);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(800, 600);	// Use this for 2D gameplay
	scene = new SceneGame4(800, 600);	// Use this for 2D gameplay
#endif
	scene->Init(1);
	scene->SetHeroOffset();
	scene->noOfJellybeans = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeans();
	scene->noOfJellybeansMayWin = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeansDeposited() * 2;
}

void CGame4State::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CGame4State::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(width, height);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(width, height);	// Use this for 2D gameplay
	scene = new SceneGame4(width, height);
#endif
	scene->Init(level);
	scene->SetHeroOffset();
	scene->noOfJellybeans = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeans();
	scene->noOfJellybeansMayWin = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeansDeposited() * 2;
}

void CGame4State::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CGame4State::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CGame4State::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CGame4State::Pause\n" << endl;
#endif
}

void CGame4State::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CGame4State::Resume\n" << endl;
#endif

	scene->PreInit();
#if	TYPE_OF_VIEW == 3
	scene->currentState = CSceneManager::STARTING;
#endif
}

void CGame4State::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CGame4State: Choose one <0> Go to Menu State : " ;
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

void CGame4State::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CGame4State: Choose one <0> Go to Menu State : " ;
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

		theGSM->PushState(CPauseState::Instance(), 1);
	}
#endif
}

void CGame4State::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right,
	const int width, const int height)
{
	//scene->UIManager->HandleEvent(mouse_x, mouse_y, width, height, scene->sceneManager2D.m_window_width, scene->sceneManager2D.m_window_height);

#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CGame4State: Choose one <0> Go to Menu State : " ;
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
	scene->UIManager->HandleEvent((float)mouse_x, (float)mouse_y, (float)width, (float)height, (float)scene->sceneManager2D.m_window_width, (float)scene->sceneManager2D.m_window_height);

	switch (scene->CurrentState)
	{
	case SceneGame4::State::PREPARE:
	{
		if (button_Left == 1 && scene->UIManager->FindButton("Next_Button")->getisHovered() == 1)
		{
			// if there is instruction left
			if (scene->numOfInstructionsLeft > 1)
			{
				// Skip all animations
				scene->UIManager->InvokeAnimator()->SkipAllAnimations();

				// Close current instruction
				int index = scene->UIManager->getUI_List().size() - scene->numOfInstructionsLeft;
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->getUI_List()[index], 0, Vector3(), 5.0f, UIAnimation::SCALING);

				// Remove one instruction
				scene->numOfInstructionsLeft--;

				// Open next instruction
				index = scene->UIManager->getUI_List().size() - scene->numOfInstructionsLeft;
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->getUI_List()[index], (float)(0.05), Vector3((float)(scene->sceneManager2D.m_window_width * 0.65), (float)(scene->sceneManager2D.m_window_height * 0.65)), 5.0f, UIAnimation::SCALING);
			}
			else
			{
				// Skip all animations
				scene->UIManager->InvokeAnimator()->SkipAllAnimations();

				// Close all window
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("AlphaQuad"), 0, Vector3(), 5.0f, UIAnimation::SCALING);
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("Instruction_Header"), (float)(0.05), Vector3((float)(scene->sceneManager2D.m_window_width * 0.5), (float)(scene->sceneManager2D.m_window_height * 1.3)), 5.0f, UIAnimation::TRANSLATION);
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("Instruction_Background"), (float)(0.1), Vector3(), 5.0f, UIAnimation::SCALING);
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindButton("Next_Button"), (float)(0.15), Vector3(), 5.0f, UIAnimation::SCALING);

				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->getUI_List().back(), (float)(0.2), Vector3(), 5.0f, UIAnimation::SCALING);

				scene->CurrentState = SceneGame4::PLAY;
			}
		}
		break;
	}
	case SceneGame4::State::PLAY:
	{
		break;
	}
	case SceneGame4::State::TIME_UP:
	{
		if (scene->CurrentLevel == SceneGame4::DifficultyLevel::TUTORIAL)
		{
			scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("AlphaQuad"), 0, Vector3((float)(scene->sceneManager2D.m_window_width), (float)(scene->sceneManager2D.m_window_height), 1), 1, 2);
			scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("TutScreen"), 0, Vector3((float)(scene->sceneManager2D.m_window_width * 0.5), (float)(scene->sceneManager2D.m_window_height * 0.6), 1), (float)(0.1), 0);
			scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindButton("ReturnToHubButton"), 0, Vector3((float)(scene->sceneManager2D.m_window_width * 0.5), (float)(scene->sceneManager2D.m_window_width * 0.2), 0), (float)(0.1), 0);
			// Return to hub Button
			if (scene->UIManager->FindButton("ReturnToHubButton")->getisHovered() == true)
			{
				if (button_Left)
					theGSM->ChangeState(CHubState::Instance());
			}
		}
		break;
	}
	case SceneGame4::State::WIN:
	{
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("AlphaQuad"), 0, Vector3((float)(scene->sceneManager2D.m_window_width), (float)(scene->sceneManager2D.m_window_height), 1), (float)1, 2);
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("WinScreen"), 0, Vector3((float)(scene->sceneManager2D.m_window_width * 0.5), (float)(scene->sceneManager2D.m_window_height * 0.6), 1), (float)0.1, 0);
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindButton("ReturnToHubButton"), 0, Vector3((float)(scene->sceneManager2D.m_window_width * 0.5), (float)(scene->sceneManager2D.m_window_width * 0.2), 0), (float)0.1, 0);
		// Return to hub Button
		if (scene->UIManager->FindButton("ReturnToHubButton")->getisHovered() == true)
		{
			if (button_Left)
				scene->CurrentState = SceneGame4::RETURN;
		}
		break;
	}
	case SceneGame4::State::LOSE:
	{
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("AlphaQuad"), 0, Vector3((float)(scene->sceneManager2D.m_window_width), (float)(scene->sceneManager2D.m_window_height), 1), 1, 2);
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("LoseScreen"), 0, Vector3((float)(scene->sceneManager2D.m_window_width * 0.5), (float)(scene->sceneManager2D.m_window_height * 0.6), 1), (float)(0.1), 0);
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindButton("ReturnToHubButton"), 0, Vector3((float)(scene->sceneManager2D.m_window_width * 0.5), (float)(scene->sceneManager2D.m_window_width * 0.2), 0), (float)(0.1), 0);
		// Return to hub Button
		if (scene->UIManager->FindButton("ReturnToHubButton")->getisHovered() == true)
		{
			if (button_Left)
				scene->CurrentState = SceneGame4::RETURN;
		}
		break;
	}

	case SceneGame4::State::RETURN:
	{
		// Unlock new difficulty
		if (theGSM->saveAndLoadsys->GetCurrentGameInfo()->DifficultySystems[2].getCurrentDifficultyUnlocked() <= scene->CurrentLevel)
		{
			theGSM->saveAndLoadsys->GetCurrentGameInfo()->DifficultySystems[2].setCurrentDifficultyUnlocked(scene->CurrentLevel + 1);
		}
		// Withdraw jellybean
		theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.WithdrawJellybeans();

		theGSM->ChangeState(CHubState::Instance());
		break;
	}
	}


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
}


void CGame4State::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CGame4State::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CGame4State::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CGame4State::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//cout << "CGame4State::Draw : " << counter << "\n" << endl;
#endif

	// Render the scene
	scene->Render();
		}
