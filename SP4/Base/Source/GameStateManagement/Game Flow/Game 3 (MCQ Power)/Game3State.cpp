#include <iostream>
using namespace std;

#include "..\..\Master\GameState.h"
#include "..\..\Master\GameStateManager.h"
#include "..\..\Game Flow\Game 3 (MCQ Power)\Game3State.h"
#include "..\..\Game Flow\Lobby\HubState.h"
//#include "LoadGameSelect.h"
#include "..\..\Menu Flow\MenuState.h"
#include "..\..\Menu Flow\PauseState.h"

CGame3State CGame3State::theGame3State;

void CGame3State::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(800, 600);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(800, 600);	// Use this for 2D gameplay
	scene = new CSceneGame3(800, 600);	// Use this for 2D gameplay
#endif
	scene->Init(1);
	scene->SetHeroOffset();
	scene->noOfJellybeans = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeans();
	scene->noOfJellybeansMayWin = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeansDeposited() * 2;
}

void CGame3State::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(width, height);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(width, height);	// Use this for 2D gameplay
	scene = new CSceneGame3(width, height);
#endif
	scene->Init(level);
	scene->SetHeroOffset();
	scene->noOfJellybeans = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeans();
	scene->noOfJellybeansMayWin = theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.GetNumOfJellybeansDeposited() * 2;
}

void CGame3State::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CGame3State::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Pause\n" << endl;
#endif
}

void CGame3State::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Resume\n" << endl;
#endif

	scene->PreInit();
#if	TYPE_OF_VIEW == 3
	scene->currentState = CSceneManager::STARTING;
#endif
}

void CGame3State::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CTutorialGame1: Choose one <0> Go to Menu State : " ;
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

void CGame3State::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CTutorialGame1: Choose one <0> Go to Menu State : " ;
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

void CGame3State::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right,
	const int width, const int height)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CTutorialGame1: Choose one <0> Go to Menu State : " ;
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
	scene->UIManager->HandleEvent(mouse_x, mouse_y, width, height, scene->sceneManager2D.m_window_width, scene->sceneManager2D.m_window_height);
	
	switch (scene->currentState)
	{
	case CSceneGame3::PREPARING:
	{
		if (button_Left == true)
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
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->getUI_List()[index], 0.05, Vector3(scene->sceneManager2D.m_window_width * 0.65, scene->sceneManager2D.m_window_height * 0.65), 5.0f, UIAnimation::SCALING);
			}
			else
			{
				// Skip all animations
				//scene->UIManager->InvokeAnimator()->SkipAllAnimations();

				// Close all window
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("AlphaQuad"), 0, Vector3(), 5.0f, UIAnimation::SCALING);
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("Instruction_Header"), 0.05, Vector3(scene->sceneManager2D.m_window_width * 0.5, scene->sceneManager2D.m_window_height * 1.3), 5.0f, UIAnimation::TRANSLATION);
				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("Instruction_Background"), 0.1, Vector3(), 5.0f, UIAnimation::SCALING);

				scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->getUI_List().back(), 0.15, Vector3(), 5.0f, UIAnimation::SCALING);

				scene->currentState = CSceneGame3::PLAYING;
			}
		}
	}
		break;
	case CSceneGame3::COMPLETED:
	{
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("AlphaQuad"), 0, Vector3(scene->sceneManager2D.m_window_width, scene->sceneManager2D.m_window_height, 1), 1, 2);
		if (scene->level == 0)
			scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("TutScreen"), 0, Vector3(scene->sceneManager2D.m_window_width * 0.5, scene->sceneManager2D.m_window_height * 0.6, 1), 0.1, 0);
		else
			scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("WinScreen"), 0, Vector3(scene->sceneManager2D.m_window_width * 0.5, scene->sceneManager2D.m_window_height * 0.6, 1), 0.1, 0);
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindButton("ReturnToHubButton"), 0, Vector3(scene->sceneManager2D.m_window_width * 0.5, scene->sceneManager2D.m_window_width * 0.2, 0), 0.1, 0);
		// Return to hub Button
		if (scene->UIManager->FindButton("ReturnToHubButton")->getisHovered() == true)
		{
			if (button_Left)
				scene->currentState = CSceneGame3::RETURN;
		}
	}
		break;
	case CSceneGame3::FAILED:
	{
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("AlphaQuad"), 0, Vector3(scene->sceneManager2D.m_window_width, scene->sceneManager2D.m_window_height, 1), 1, 2);
		if (scene->level == 0)
			scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("TutLoseScreen"), 0, Vector3(scene->sceneManager2D.m_window_width * 0.5, scene->sceneManager2D.m_window_height * 0.6, 1), 0.1, 0);
		else
			scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindImage("LoseScreen"), 0, Vector3(scene->sceneManager2D.m_window_width * 0.5, scene->sceneManager2D.m_window_height * 0.6, 1), 0.1, 0);
		scene->UIManager->InvokeAnimator()->StartTransformation(scene->UIManager->FindButton("ReturnToHubButton"), 0, Vector3(scene->sceneManager2D.m_window_width * 0.5, scene->sceneManager2D.m_window_width * 0.2, 0), 0.1, 0);
		// Return to hub Button
		if (scene->UIManager->FindButton("ReturnToHubButton")->getisHovered() == true)
		{
			if (button_Left)
				scene->currentState = CSceneGame3::RETURN;
		}
	}
		break;
	case CSceneGame3::RETURN:
	{
		// Unlock new difficulty
		if (theGSM->saveAndLoadsys->GetCurrentGameInfo()->DifficultySystems[2].getCurrentDifficultyUnlocked() <= scene->level)
		{
			theGSM->saveAndLoadsys->GetCurrentGameInfo()->DifficultySystems[2].setCurrentDifficultyUnlocked(scene->level + 1);
		}
		// Withdraw jellybean
		theGSM->saveAndLoadsys->GetCurrentGameInfo()->jellybean.WithdrawJellybeans();
		theGSM->ChangeState(CHubState::Instance());

	}
		break;

	}
}


void CGame3State::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CGame3State::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CGame3State::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//cout << "CTutorialGame1::Draw : " << counter << "\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
