
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
}

GLFWwindow* m_window;
// Declare the window width and height as constant integer
static int m_window_deadzone = 100;
static int m_window_width = 800;
static int m_window_height = 600;

const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, 
	   Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
	   Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;

/********************************************************************************
 Define an error callback
 ********************************************************************************/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

/********************************************************************************
 Define the key input callback
 ********************************************************************************/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

/********************************************************************************
 Callback function when the window is resized
 ********************************************************************************/
void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

/********************************************************************************
 Get keyboard's key states
 ********************************************************************************/
bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

/********************************************************************************
 Get mouse updates
 ********************************************************************************/
bool Application::GetMouseUpdate()
{
	glfwGetWindowSize(m_window, &m_window_width, &m_window_height);
    glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;

	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	if (theGSM->m_bWarpMouse == true)
	{
		// Do a wraparound if the mouse cursor has gone out of the deadzone
		if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width - m_window_deadzone))
		{
			mouse_current_x = m_window_width >> 1;
			glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
		}
		if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height - m_window_deadzone))
		{
			mouse_current_y = m_window_height >> 1;
			glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
		}
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

	// Get the mouse button status
	const int Button_Left = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
	const int Button_Middle = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE);
	const int Button_Right = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);

	static bool Button_LeftDown = false;
	if (Button_Left && !Button_LeftDown)
	{
		Button_LeftDown = true;
		// Update the GSM
		theGSM->HandleEvents(mouse_current_x, mouse_current_y, Button_Left, Button_Middle, Button_Right, m_window_width, m_window_height);
	}
	else if (Button_Left == 0 && Button_LeftDown == true)
	{
		Button_LeftDown = false;
	}

	if (Button_Left && Button_LeftDown)
	{
		// Update the GSM
		theGSM->HandleEvents(mouse_current_x, mouse_current_y, 0, Button_Middle, Button_Right, m_window_width, m_window_height);
	}
	else
	{
		// Update the GSM
		theGSM->HandleEvents(mouse_current_x, mouse_current_y, Button_Left, Button_Middle, Button_Right, m_window_width, m_window_height);
	}

	/*
	// Get the mouse button status
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
	*/

    return false;
}

/********************************************************************************
 Get keyboard updates
 ********************************************************************************/
bool Application::GetKeyboardUpdate()
{
	if (IsKeyPressed('1'))
	{
		theGSM->HandleEvents('1');
	}
	else
	{
		theGSM->HandleEvents('1', false);
	}
	if (IsKeyPressed('2'))
	{
		theGSM->HandleEvents('2');
	}
	else
	{
		theGSM->HandleEvents('2', false);
	}

	if (IsKeyPressed('A'))
	{
		theGSM->HandleEvents('a');
	}
	else
	{
		theGSM->HandleEvents('a', false);
	}
	if (IsKeyPressed('D'))
	{
		theGSM->HandleEvents('d');
	}
	else
	{
		theGSM->HandleEvents('d', false);
	}
	if (IsKeyPressed('W'))
	{
		theGSM->HandleEvents('w');
	}
	else
	{
		theGSM->HandleEvents('w', false);
	}
	if (IsKeyPressed('S'))
	{
		theGSM->HandleEvents('s');
	}
	else
	{
		theGSM->HandleEvents('s', false);
	}
	if (IsKeyPressed('Q'))
	{
		theGSM->HandleEvents('q');
	}
	else
	{
		theGSM->HandleEvents('q', false);
	}
	if (IsKeyPressed('E'))
	{
		theGSM->HandleEvents('e');
	}
	else
	{
		theGSM->HandleEvents('e', false);
	}

	// Spacebar
	if (IsKeyPressed(32))
	{
		theGSM->HandleEvents(32);
	}

	// Left shift
	if (IsKeyPressed(160))
	{
		theGSM->HandleEvents(160);
	}


	static bool tabKey = false;
	// Tab
	if (IsKeyPressed(9) && tabKey == false)
	{
		theGSM->HandleEvents(9);
		tabKey = true;
	}
	else if (IsKeyPressed(9) == false && tabKey == true)
	{
		tabKey = false;
		theGSM->HandleEvents(9, false);
	}

	// Rotate camera
	if (IsKeyPressed(VK_LEFT))
	{
		theGSM->HandleEvents(VK_LEFT);
	}
	else
	{
		theGSM->HandleEvents(VK_LEFT, false);
	}
	if (IsKeyPressed(VK_RIGHT))
	{
		theGSM->HandleEvents(VK_RIGHT);
	}
	else
	{
		theGSM->HandleEvents(VK_RIGHT, false);
	}
	if (IsKeyPressed(VK_UP))
	{
		theGSM->HandleEvents(VK_UP);
	}
	else
	{
		theGSM->HandleEvents(VK_UP, false);
	}
	if (IsKeyPressed(VK_DOWN))
	{
		theGSM->HandleEvents(VK_DOWN);
	}
	else
	{
		theGSM->HandleEvents(VK_DOWN, false);
	}

	// State used
	if (IsKeyPressed('M'))
	{
		theGSM->HandleEvents('m');
	}
	if (IsKeyPressed('P'))
	{
		theGSM->HandleEvents('p');
	}
    return true;
}

/********************************************************************************
 Constructor
 ********************************************************************************/
Application::Application()
	: scene(NULL)
	, theGSM(NULL)
	, sound(NULL)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
Application::~Application()
{
	if (theGSM)
	{
		delete theGSM;
		theGSM = NULL;
	}
	if (sound)
	{
		delete sound;
		sound = NULL;
	}
	_CrtDumpMemoryLeaks();
}

/********************************************************************************
 Initialise this program
 ********************************************************************************/
void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	ReadLuaTextFile();

	// Hide console window
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Set these 2 variables to zero
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;

	// Initialise the GSM
	theGSM = new CGameStateManager();
	theGSM->Init("Game State Management", 800, 600);
	//theGSM->ChangeState(CSplashState::Instance());
	theGSM->ChangeState(CHubState::Instance());

	if (sound == NULL)
	{
		sound = new CSound();
		sound->Init();
	}
}

/********************************************************************************
 Run this program
 ********************************************************************************/
void Application::Run()
{
	#if TYPE_OF_VIEW == 3
		sound->PauseMainMenu();
	#else
		sound->PlayMainMenu();
	#endif
	
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && theGSM->Running())
	{
		// Get the elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
		if (m_dAccumulatedTime_ThreadOne > 0.03)
		{
			// Get inputs
			GetMouseUpdate();
			GetKeyboardUpdate();

			theGSM->HandleEvents();
			theGSM->Update(m_dElapsedTime, m_window);

			/*
			scene->Update(m_dElapsedTime);
			m_dAccumulatedTime_ThreadOne = 0.0;
			*/
		}
		if (m_dAccumulatedTime_ThreadTwo > 1.0)
		{
			//UpdateAI();
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}
		// Render the scene
		theGSM->Draw();
		//scene->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed

	// Clean up the GSM
	theGSM->Cleanup();
	// Delete the scene
	//scene->Exit();
	//delete scene;
}

/********************************************************************************
 Exit this program
 ********************************************************************************/
void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
	
}

int Application::ReadLuaTextFile()
{
	lua_State *L = lua_open();

	//Read a value from the lua text file
	luaL_openlibs(L);

	if (luaL_loadfile(L, "Lua//Options.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}

	// Fullscreen
	lua_getglobal(L, "fullscreen");
	if (!lua_isnumber(L, -1)) {
		printf("`fullscreen' should be a number\n");
	}
	bool fullscreenSelected = (int)lua_tointeger(L, -1);

	lua_close(L);

	lua_State *L2 = lua_open();

	//Read a value from the lua text file
	luaL_openlibs(L2);

	if (luaL_loadfile(L2, "Lua//config.lua") || lua_pcall(L2, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L2, -1));
		return -1;
	}

	lua_getglobal(L2, "SCREENWIDTH");
	if (!lua_isnumber(L2, -1)) {
		printf("`SCREENWIDTH' should be a number\n");
		return -1;
	}
	int SCREENWIDTH = (int)lua_tonumber(L2, -1);

	lua_getglobal(L2, "SCREENHEIGHT");
	if (!lua_isnumber(L2, -1)) {
		printf("`SCREENHEIGHT' should be a number\n");
		return -1;
	}
	int SCREENHEIGHT = (int)lua_tonumber(L2, -1);

	lua_close(L2);

	//Create a window and create its OpenGL context
	if (fullscreenSelected)
		m_window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "SP4", glfwGetPrimaryMonitor(), NULL);
	else
		m_window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "SP4", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	return 0;
}