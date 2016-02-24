#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"

void CGameStateManager::Init(const char* title, int width, int height, 
						 int bpp, bool fullscreen)
{
	m_window_width	= width;
	m_window_height	= height;

	m_bFullscreen = fullscreen;
	m_bRunning = true;

	m_bHideMouse = false;
	m_bUnhideMouse = false;
	m_bWarpMouse = false;
	
	sound = NULL;
	if (sound == NULL)
	{
		sound = new CSound();
		sound->Init();
	}

	saveAndLoadsys = NULL;
	saveAndLoadsys = new CSavingAndLoadingSystem();
	saveAndLoadsys->AddData("Lua//Data1.lua");

#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Init\n" << endl;
#endif
}

void CGameStateManager::Cleanup()
{
	// cleanup the all states
	while ( !StackOfStates.empty() ) {
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	//check that the stack is empty then drop the sound engine
	if (StackOfStates.empty())
	{
		if (sound)
		{
			sound->Exit();
			delete sound;
			sound = NULL;
		}
	}
	if (saveAndLoadsys)
	{
		delete saveAndLoadsys;
		saveAndLoadsys = NULL;
	}
	// switch back to windowed mode so other 
	// programs won't get accidentally resized
	if ( m_bFullscreen ) {
	}

#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Cleanup\n" << endl;
#endif
}

void CGameStateManager::ChangeState(CGameState* state, int level) 
{
	// cleanup the current state
	if ( !StackOfStates.empty() ) {
		prevI = StackOfStates.back()->GetStateID();		//store the stack's state id before cleaning it up
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	// store and init the new state
	StackOfStates.push_back(state);
	StackOfStates.back()->Init(this, m_window_width, m_window_height, level);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::ChangeState\n" << endl;
#endif
}

void CGameStateManager::PushState(CGameState* state)
{
	// pause current state
	if ( !StackOfStates.empty() ) {
		prevI = StackOfStates.back()->GetStateID();		//store the stack's state id before pausing that stack
		StackOfStates.back()->Pause();
	}

	// store and init the new state
	StackOfStates.push_back(state);
	StackOfStates.back()->Init(this, m_window_width, m_window_height);
	
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::PushState\n" << endl;
#endif
}

void CGameStateManager::PopState()
{
	// cleanup the current state
	if ( !StackOfStates.empty() ) {
		prevI = StackOfStates.back()->GetStateID();		//store the stack's state id before cleaning it up
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	// resume previous state
	if ( !StackOfStates.empty() ) {
		StackOfStates.back()->Resume();
	}
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::PopState\n" << endl;
#endif
}

void CGameStateManager::HandleEvents() 
{
	// let the state handle events
	StackOfStates.back()->HandleEvents(this);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::HandleEvents\n" << endl;
#endif
}

void CGameStateManager::HandleEvents(const unsigned char key, const bool status)
{
	// let the state handle events
	StackOfStates.back()->HandleEvents(this, key, status);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::HandleEvents\n" << endl;
#endif
}

void CGameStateManager::HandleEvents(const double mouse_x, const double mouse_y,
									const int button_Left, const int button_Middle, const int button_Right,
									const int width, const int height)
{
	// let the state handle events
	StackOfStates.back()->HandleEvents(this, mouse_x, mouse_y, button_Left, button_Middle, button_Right, width, height);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::HandleEvents\n" << endl;
#endif
}
void CGameStateManager::UpdateSound(const double m_dElapsedTime)
{
	i = StackOfStates.back()->GetStateID();

	if (sound != NULL)
	{
		switch (i)
		{
			//main menu
		case 1:
			sound->setSoundType(1);
			break;
			//lobby
		case 10:
			sound->setSoundType(0);
			break;
			//maze
		case 12:
			sound->setSoundType(2);
			break;
			//puzzle
		case 14:
			sound->setSoundType(3);
			break;
			//mcq
		case 15:
			sound->setSoundType(4);
			break;
			//stress
		case 16:
			sound->setSoundType(5);
			break;
		}
		sound->PlaySong();	//depending on the type
	}
}
void CGameStateManager::Update(const double m_dElapsedTime, GLFWwindow* m_window)
{
	if (m_bHideMouse == true)
	{
		// Hide the cursor
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		m_bHideMouse = false;
	}
	if (m_bUnhideMouse == true)
	{
		// Hide the cursor
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_bUnhideMouse = false;
	}

	//Update the sound
	UpdateSound(m_dElapsedTime);

	// let the state update the theGSM
	StackOfStates.back()->Update(this, m_dElapsedTime);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Update\n" << endl;
#endif
}

void CGameStateManager::Draw() 
{
	// let the state draw the screen
	StackOfStates.back()->Draw(this);
#if GSM_DEBUG_MODE
	cout << "CGameStateManager::Draw\n" << endl;
#endif
}
