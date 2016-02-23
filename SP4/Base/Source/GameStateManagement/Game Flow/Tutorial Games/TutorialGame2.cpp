#include <iostream>
using namespace std;

#include "..\..\Master\GameState.h"
#include "..\..\Master\GameStateManager.h"
#include "..\..\Game Flow\Tutorial Games\TutorialGame2.h"
//#include "LoadGameSelect.h"
#include "..\..\Menu Flow\MenuState.h"
#include "..\..\Menu Flow\PauseState.h"
#include "..\..\Game Flow\Lobby\HubState.h"


CTutorialGame2 CTutorialGame2::theTutorialGame2State;

void CTutorialGame2::Init(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(800, 600);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(800, 600);	// Use this for 2D gameplay
	scene = new CSceneTutorialGame2(800, 600);	// Use this for 2D gameplay
#endif
	scene->Init(1);
	scene->ReadData(theGSM->saveAndLoadsys->GetGameInfo());
	scene->SetHeroOffset();
}

void CTutorialGame2::Init(CGameStateManager* theGSM, const int width, const int height, int level)
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Init\n" << endl;
#endif
	counter = 0;

#if TYPE_OF_VIEW == 3
	scene = new CSceneManager(width, height);	// Use this for 3D gameplay
#else
	//scene = new CSceneManager2D(width, height);	// Use this for 2D gameplay
	scene = new CSceneTutorialGame2(width, height);
#endif
	scene->Init(level);
	scene->ReadData(theGSM->saveAndLoadsys->GetGameInfo());
	scene->SetHeroOffset();
}

void CTutorialGame2::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CTutorialGame2::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Pause\n" << endl;
#endif
}

void CTutorialGame2::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Resume\n" << endl;
#endif

	scene->PreInit();
#if	TYPE_OF_VIEW == 3
	scene->currentState = CSceneManager::STARTING;
#endif
}

void CTutorialGame2::HandleEvents(CGameStateManager* theGSM)
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

void CTutorialGame2::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
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

		theGSM->PushState(CPauseState::Instance());
		scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
	}
	
#endif
}

void CTutorialGame2::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
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
	switch (scene->currentState)
	{
	case CSceneTutorialGame2::COMPLETED:
	{
		if (button_Left == true)
		{
			scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
			theGSM->ChangeState(CHubState::Instance());
		}
	}
		break;
	case CSceneTutorialGame2::TIME_UP:
	{
		if (button_Left == true)
		{
			scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
			theGSM->ChangeState(CHubState::Instance());
		}
	}
		break;
	}
#endif
	switch (scene->currentState)
	{
	case CSceneTutorialGame2::COMPLETED:
	{
		theGSM->m_bUnhideMouse = true;
		theGSM->m_bWarpMouse = false;
		if (button_Left == true)
		{
			

			scene->StoreData(theGSM->saveAndLoadsys->GetGameInfo());
			theGSM->ChangeState(CHubState::Instance());
		}
	}
		break;
	case CSceneTutorialGame2::PLAYING:
	{
		theGSM->m_bHideMouse = true;
		theGSM->m_bWarpMouse = false;
		
	}
		break;
	}
}

void CTutorialGame2::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CTutorialGame1::Update\n" << endl;
#endif
	// Update the scene
	scene->Update(0.16667);
}

void CTutorialGame2::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	scene->Update(m_dElapsedTime);
}

void CTutorialGame2::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//cout << "CTutorialGame1::Draw : " << counter << "\n" << endl;
#endif

	// Render the scene
	scene->Render();
}
