#include "ScenePause.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

CScenePause::CScenePause()
	: currentState(LOBBY)
	, prevState(LOBBY)
	, firstState(LOBBY)
	, UIManager(NULL)
	, UI_Speed(0.f)
{
	// Set all of the meshes NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

CScenePause::CScenePause(const int m_window_width, const int m_window_height)
	: currentState(LOBBY)
	, prevState(LOBBY)
	, firstState(LOBBY)
	, UIManager(NULL)
	, UI_Speed(0.f)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
	// Set all of the meshes NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

CScenePause::~CScenePause()
{
	if (UIManager)
	{
		delete UIManager;
		UIManager = NULL;
	}
	if (UIManagerConfirmation)
	{
		delete UIManagerConfirmation;
		UIManagerConfirmation = NULL;
	}
}

void CScenePause::Init(int level)
{
	UI_Speed = 6.f;
	
	switch (level)
	{
	case 0:
		currentState = LOBBY;
		break;
	case 1:
		currentState = GAME;
		break;
	}

	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Pause.tga");

	meshList[BACKGROUND] = MeshBuilder::GenerateQuad("BACKGROUND", Color(0, 0, 0), 1.f);
	meshList[BACKGROUND]->textureID = LoadTGA("Image//Scenes/Pause.tga");

	meshList[PAUSE] = MeshBuilder::GenerateQuad("PAUSE", Color(1, 1, 1), 1.f);
	meshList[PAUSE]->textureID = LoadTGA(
		"Image//paused_icon.tga");

	meshList[SAVE] = MeshBuilder::GenerateQuad("SAVE", Color(1, 1, 1), 1.f);
	meshList[SAVE]->textureID = LoadTGA(
		"Image//Save_Button.tga");

	meshList[SAVE_DOWN] = MeshBuilder::GenerateQuad("SAVE_DOWN", Color(1, 1, 1), 1.f);
	meshList[SAVE_DOWN]->textureID = LoadTGA(
		"Image//Save_Button_Pressed.tga");

	meshList[SAVE_LOCKED] = MeshBuilder::GenerateQuad("SAVE_LOCKED", Color(1, 1, 1), 1.f);
	meshList[SAVE_LOCKED]->textureID = LoadTGA("Image//Save_Button_Locked.tga");

	meshList[RESUME] = MeshBuilder::GenerateQuad("RESUME", Color(1, 1, 1), 1.f);
	meshList[RESUME]->textureID = LoadTGA("Image//Resume_Button.tga");

	meshList[RESUME_PRESSED] = MeshBuilder::GenerateQuad("RESUME_PRESSED", Color(1, 1, 1), 1.f);
	meshList[RESUME_PRESSED]->textureID = LoadTGA("Image//Resume_Button_Pressed.tga");

	meshList[QUIT] = MeshBuilder::GenerateQuad("QUIT", Color(1, 1, 1), 1.f);
	meshList[QUIT]->textureID = LoadTGA("Image//Quit_Button.tga");

	meshList[QUIT_PRESSED] = MeshBuilder::GenerateQuad("GEO_QUIT_PRESSED", Color(1, 1, 1), 1.f);
	meshList[QUIT_PRESSED]->textureID = LoadTGA("Image//Quit_Button_Pressed.tga");

	meshList[MAIN_MENU] = MeshBuilder::GenerateQuad("MAIN_MENU", Color(1, 1, 1), 1.f);
	meshList[MAIN_MENU]->textureID = LoadTGA("Image//Menu_Button.tga");

	meshList[MAIN_MENU_PRESSED] = MeshBuilder::GenerateQuad("MAIN_MENU_PRESSED", Color(1, 1, 1), 1.f);
	meshList[MAIN_MENU_PRESSED]->textureID = LoadTGA("Image//Menu_Button_Pressed.tga");

	// Confirmation
	// Window
	meshList[GEO_CONFIRMATION_WINDOW] = MeshBuilder::GenerateQuad("GEO_CONFIRMATION_WINDOW", Color(1, 1, 1), 1);
	meshList[GEO_CONFIRMATION_WINDOW]->textureID = LoadTGA("Image//UI/Confirmation_Window.tga");
	// Yes
	meshList[GEO_YES_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_YES_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_YES_BUTTON_UP]->textureID = LoadTGA("Image//UI/Yes_Button.tga");
	meshList[GEO_YES_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_YES_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_YES_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Yes_Button_Pressed.tga");
	// No
	meshList[GEO_NO_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_NO_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_NO_BUTTON_UP]->textureID = LoadTGA("Image//UI/No_Button.tga");
	meshList[GEO_NO_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_NO_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_NO_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/No_Button_Pressed.tga");

	// Alpha Black Quad
	meshList[GEO_ALPHA_BLACK_QUAD] = MeshBuilder::GenerateQuad("GEO_ALPHA_BLACK_QUAD", Color(1, 1, 1), 1);
	meshList[GEO_ALPHA_BLACK_QUAD]->textureID = LoadTGA("Image//UI/Half_Alpha_Black.tga");

	//depending on the state given, load the UI system accordingly
	switch (currentState)
	{
		//include the save feature when inside the lobby
	case LOBBY:
	{
		//UI
		UIManager = new UISystem();

		//Background
		Image* Background;
		Background = new Image("Background", meshList[BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height, 0), Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_height, 0));
		UIManager->addFeature(Background);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Background"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);

		Image* paused_icon;
		paused_icon = new Image("paused_icon", meshList[PAUSE], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.89, 0), Vector3(0, 0, 0));
		UIManager->addFeature(paused_icon);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("paused_icon"), 0.25, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.2, 0), UI_Speed, UIAnimation::SCALING);

		//Buttons
		// resume button
		Button* ResumeButton;
		ResumeButton = new Button("ResumeButton", meshList[RESUME], meshList[RESUME_PRESSED], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
		UIManager->addFeature(ResumeButton);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ResumeButton"), 1, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 6, 0), 10, 0);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ResumeButton"), 1.5, Vector3(100, 50, 1), 5, 1);

		// save button
		Button* SaveButton;
		SaveButton = new Button("SaveButton", meshList[SAVE], meshList[SAVE_DOWN], meshList[SAVE_LOCKED], Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
		UIManager->addFeature(SaveButton);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("SaveButton"), 1.2, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 5, 0), 10, 0);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("SaveButton"), 1.7, Vector3(100, 50, 1), 5, 1);

		// Quit Button
		Button* QuitButton;
		QuitButton = new Button("QuitButton", meshList[MAIN_MENU], meshList[MAIN_MENU_PRESSED], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
		UIManager->addFeature(QuitButton);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("QuitButton"), 1.4, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 4, 0), 10, 0);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("QuitButton"), 1.9, Vector3(100, 50, 1), 5, 1);
	}
		break;
		//game state does not have the save state feature
	case GAME:
	{
		//UI
		UIManager = new UISystem();

		//Background
		Image* Background;
		Background = new Image("Background", meshList[BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height, 0), Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_height, 0));
		UIManager->addFeature(Background);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Background"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);

		Image* paused_icon;
		paused_icon = new Image("paused_icon", meshList[PAUSE], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.89, 0), Vector3(0, 0, 0));
		UIManager->addFeature(paused_icon);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("paused_icon"), 0.25, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.2, 0), UI_Speed, UIAnimation::SCALING);

		//Buttons
		// resume button
		Button* ResumeButton;
		ResumeButton = new Button("ResumeButton", meshList[RESUME], meshList[RESUME_PRESSED], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
		UIManager->addFeature(ResumeButton);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ResumeButton"), 1, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 6, 0), 10, 0);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ResumeButton"), 1.5, Vector3(100, 50, 1), 5, 1);

		// Quit Button
		Button* QuitButton;
		QuitButton = new Button("QuitButton", meshList[QUIT], meshList[QUIT_PRESSED], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
		UIManager->addFeature(QuitButton);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("QuitButton"), 1.2, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 5, 0), 10, 0);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("QuitButton"), 1.7, Vector3(100, 50, 1), 5, 1);

		// To main menu Button
		Button* MainMenuButton;
		MainMenuButton = new Button("MainMenuButton", meshList[MAIN_MENU], meshList[MAIN_MENU_PRESSED], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
		UIManager->addFeature(MainMenuButton);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MainMenuButton"), 1.4, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 4, 0), 10, 0);
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MainMenuButton"), 1.9, Vector3(100, 50, 1), 5, 1);
	}
		break;
	}

	// Confiramtion
	UIManagerConfirmation = new UISystem();

	// AlphaQuad
	Image* AlphaQuad;
	AlphaQuad = new Image("AlphaQuad", meshList[GEO_ALPHA_BLACK_QUAD], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), Vector3(0, 0, 0));
	UIManagerConfirmation->addFeature(AlphaQuad);

	// Confirmation Window
	Image* ConfirmationWindow;
	ConfirmationWindow = new Image("ConfirmationWindow", meshList[GEO_CONFIRMATION_WINDOW], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.45, 0));
	UIManagerConfirmation->addFeature(ConfirmationWindow);

	// Yes button
	Button* YesButton;
	YesButton = new Button("YesButton", meshList[GEO_YES_BUTTON_UP], meshList[GEO_YES_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.1, 0));
	UIManagerConfirmation->addFeature(YesButton);

	// No button
	Button* NoButton;
	NoButton = new Button("NoButton", meshList[GEO_NO_BUTTON_UP], meshList[GEO_NO_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.1, 0));
	UIManagerConfirmation->addFeature(NoButton);
}
//hide the confirmation screen
void CScenePause::HideConfirmation()
{
	UIManagerConfirmation->InvokeAnimator()->StopAllAnimations();
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// AlphaQuad
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("AlphaQuad"), 0, Vector3(0, 0, 0), UI_Speed * 2, UIAnimation::SCALING);
}
//show the confirmation screen
void CScenePause::ShowConfirmation()
{
	UIManagerConfirmation->InvokeAnimator()->StopAllAnimations();
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.65, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.385, sceneManager2D.m_window_height * 0.3, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.615, sceneManager2D.m_window_height * 0.3, 0), UI_Speed, UIAnimation::TRANSLATION);
	// AlphaQuad
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("AlphaQuad"), 0, Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_height, 0), UI_Speed * 2, UIAnimation::SCALING);
}
void CScenePause::Update(double dt)
{
	sceneManager2D.Update(dt);

	UIManager->Update(dt);
	UIManagerConfirmation->Update(dt);
}

/********************************************************************************
Render this scene
********************************************************************************/
void CScenePause::Render()
{
	sceneManager2D.Render();

	UIManager->Render(sceneManager2D);
	UIManagerConfirmation->Render(sceneManager2D);
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CScenePause::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}