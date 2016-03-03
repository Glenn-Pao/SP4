#include "SceneMenu.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

CSceneMenu::CSceneMenu()
	: currentState(SELECTING)
	, UI_Speed(0.0f)
	, currentDataSelected(0)
{
}

CSceneMenu::CSceneMenu(const int m_window_width, const int m_window_height)
	: currentState(SELECTING)
	, UI_Speed(0.0f)
	, currentDataSelected(0)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
	
}

CSceneMenu::~CSceneMenu()
{
}

void CSceneMenu::Init(int level)
{
	UI_Speed = 20.f;
	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Menu.tga");

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}
	InitMeshes();
	
	choice = NONE;

	UIManager = new UISystem();

	//Start Game Button
	Button* StartGameButton;
	StartGameButton = new Button("StartGameButton", meshList[START_BUTTON_UP], meshList[START_BUTTON_DOWN], meshList[START_BUTTON_LOCKED], Vector3(-100, (float)(sceneManager2D.m_window_height / 10 * 6), 1), Vector3(20, 10, 1));

	//Load Game Button
	Button* LoadGameButton;
	LoadGameButton = new Button("LoadGameButton", meshList[LOAD_BUTTON_UP], meshList[LOAD_BUTTON_DOWN], meshList[LOAD_BUTTON_LOCKED], Vector3(-100, (float)(sceneManager2D.m_window_height / 10 * 4.5), 1), Vector3(20, 10, 1));

	//Exit Button
	Button* ExitButton;
	ExitButton = new Button("ExitButton", meshList[EXIT_BUTTON_UP], meshList[EXIT_BUTTON_DOWN], NULL, Vector3(-100, (float)(sceneManager2D.m_window_height / 10 * 3), 1), Vector3(20, 10, 1));

	//Instructions
	Button* Instructions;
	Instructions = new Button("Instructions", meshList[INSTRUCTIONS_BUTTON_UP], meshList[INSTRUCTIONS_BUTTON_DOWN], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40, 1), Vector3(1, 1, 1));

	//Music Icon Button
	Button* MusicIcon;
	MusicIcon = new Button("MusicIcon", meshList[MUSIC_ICON], meshList[MUSIC_ICON], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40, 1), Vector3(1, 1, 1));

	//Music Icon Button Off
	Button* MusicIconOff;
	MusicIconOff = new Button("MusicIconOff", meshList[MUSIC_ICON_OFF], meshList[MUSIC_ICON_OFF], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40, 1), Vector3(1, 1, 1), false);

	//Settings Button
	Button* SettingsButton;
	SettingsButton = new Button("SettingsButton", meshList[SETTINGS_BUTTON_UP], meshList[SETTINGS_BUTTON_DOWN], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40, 1), Vector3(1, 1, 1), true);


	//Title
	Image* Title;
	Title = new Image("Title", meshList[TITLE], Vector3((float)(sceneManager2D.m_window_width / 2), (float)(sceneManager2D.m_window_height + 200), 0), Vector3(300, 150, 0));
	
	//Left Smoke
	Image* SmokeToLeft;
	SmokeToLeft = new Image("SmokeToLeft", meshList[SMOKE_LEFT], Vector3((float)(sceneManager2D.m_window_width / 10 * 4), (float)(sceneManager2D.m_window_height / 10 * 6.8), 0), Vector3((float)(0.1), (float)(0.05), 0));

	//Upeer Left Smoke
	Image* SmokeToLeftUp;
	SmokeToLeftUp = new Image("SmokeToLeftUp", meshList[SMOKE_LEFT], Vector3((float)(sceneManager2D.m_window_width / 10 * 4), (float)(sceneManager2D.m_window_height / 10 * 6.8), 0), Vector3((float)(0.1), (float)(0.05), 0));

	//Right Smoke
	Image* SmokeToRight;
	SmokeToRight = new Image("SmokeToRight", meshList[SMOKE_RIGHT], Vector3((float)(sceneManager2D.m_window_width / 10 * 6), (float)(sceneManager2D.m_window_height / 10 * 6.8), 0), Vector3((float)(0.1), (float)(0.05), 0));

	//Upper Right Smoke
	Image* SmokeToRightUp;
	SmokeToRightUp = new Image("SmokeToRightUp", meshList[SMOKE_RIGHT], Vector3((float)(sceneManager2D.m_window_width) / 10 * 6, (float)(sceneManager2D.m_window_height / 10 * 6.8), 0), Vector3((float)(0.1), (float)(0.05), 0));

	//Background Image
	Image* BackGround;
	BackGround = new Image("BackGround", meshList[BACKGROUND], Vector3((float)(sceneManager2D.m_window_width / 2), (float)(sceneManager2D.m_window_height / 2), 0), Vector3(800, 600, 0));

	//Information Pointer Left Button Off
	Button* InformationPointerLeft;
	InformationPointerLeft = new Button("InformationPointerLeft", meshList[INFORMATION_POINTER_LEFT], meshList[INFORMATION_POINTER_LEFT_PRESSED], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.5 - 20), -50, 1), Vector3(30, 30, 1), true);

	//Information Pointer Left Button Off
	Button* InformationPointerRight;
	InformationPointerRight = new Button("InformationPointerRight", meshList[INFORMATION_POINTER_RIGHT], meshList[INFORMATION_POINTER_RIGHT_PRESSED], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.5 + 20), -50, 1), Vector3(30, 30, 1), true);

	//Instructions Page Game Overview
	Image* InstructionPageGameOverview;
	InstructionPageGameOverview = new Image("InstructionPageGameOverview", meshList[INFORMATION_GAME_OVERVIEW], Vector3((float)(sceneManager2D.m_window_width * 0.5), -300, 1), Vector3(500, 600, 1));

	UIManager->addFeature(BackGround);

	UIManager->addFeature(Title);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Title"), (float)(1.2), Vector3((float)(sceneManager2D.m_window_width / 2), (float)(sceneManager2D.m_window_height / 10 * 8), 0), 10, 0);

	UIManager->addFeature(SmokeToLeft);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeft"), 1.5, Vector3((float)(sceneManager2D.m_window_width / 10 * 2), (float)(sceneManager2D.m_window_height / 10 * 7), 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeft"), 1.5, Vector3(100, 50, 0), 4, 2);


	UIManager->addFeature(SmokeToLeftUp);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeftUp"), 1.5, Vector3((float)(sceneManager2D.m_window_width / 10 * 1.2), (float)(sceneManager2D.m_window_height / 10 * 7.5), 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeftUp"), 1.5, Vector3(70, 35, 0), 4, 2);

	UIManager->addFeature(SmokeToRight);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRight"), 1.5, Vector3((float)(sceneManager2D.m_window_width / 10 * 8), (float)(sceneManager2D.m_window_height / 10 * 7), 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRight"), 1.5, Vector3(100, 50, 0), 4, 2);

	UIManager->addFeature(SmokeToRightUp);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRightUp"), 1.5, Vector3((float)(sceneManager2D.m_window_width / 10 * 8.8), (float)(sceneManager2D.m_window_height / 10 * 7.5), 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRightUp"), 1.5, Vector3(70, 35, 0), 4, 2);

	UIManager->addFeature(StartGameButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("StartGameButton"), 0, Vector3((float)(sceneManager2D.m_window_width / 2), (float)(sceneManager2D.m_window_height / 10 * 6), 0), 10, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("StartGameButton"), 0.5, Vector3(150, 75, 1), 5, 2);

	UIManager->addFeature(LoadGameButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("LoadGameButton"), (float)(0.2), Vector3((float)(sceneManager2D.m_window_width / 2), (float)(sceneManager2D.m_window_height / 10 * 4.5), 0), 10, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("LoadGameButton"), (float)(0.7), Vector3(150, 75, 1), 5, 2);

	UIManager->addFeature(ExitButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ExitButton"), (float)(0.6), Vector3((float)sceneManager2D.m_window_width / 2, (float)(sceneManager2D.m_window_height / 10 * 3), 0), 10, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ExitButton"), (float)(1.1), Vector3(150, 75, 1), 5, 2);

	UIManager->addFeature(Instructions);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Instructions"), (float)(1.9), Vector3(50, 50, 1), 5, 2);

	UIManager->addFeature(MusicIcon);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIcon"), (float)(1.9), Vector3(50, 50, 1), 5, 2);

	UIManager->addFeature(MusicIconOff);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIconOff"), (float)(1.9), Vector3(50, 50, 1), 5, 2);

	UIManager->addFeature(SettingsButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("SettingsButton"), (float)(1.9), Vector3(80, 80, 1), 5, 2);

	MainMenuAnimationTimer = 0;

	isSettingsAnimationStarted = false;
	TimeSinceSettingsAnimation = 0;

	//Information Icon
	UIManagerInformation = new UISystem();
	UIManagerInformation->addFeature(InformationPointerLeft);

	UIManagerInformation->addFeature(InformationPointerRight);

	UIManagerInformation->addFeature(InstructionPageGameOverview);

	// Confiramtion
	UIManagerConfirmation = new UISystem();

	// AlphaQuad
	Image* AlphaQuad;
	AlphaQuad = new Image("AlphaQuad", meshList[GEO_ALPHA_BLACK_QUAD], Vector3((float)(sceneManager2D.m_window_width * 0.5), (float)(sceneManager2D.m_window_height * 0.5), 0), Vector3(0, 0, 0));
	UIManagerConfirmation->addFeature(AlphaQuad);

	// Confirmation Window
	Image* ConfirmationWindow;
	ConfirmationWindow = new Image("ConfirmationWindow", meshList[GEO_CONFIRMATION_WINDOW], Vector3((float)(sceneManager2D.m_window_width * 0.5), (float)(sceneManager2D.m_window_height * 1.5), 0), Vector3((float)(sceneManager2D.m_window_width * 0.5), (float)(sceneManager2D.m_window_height * 0.45), 0));
	UIManagerConfirmation->addFeature(ConfirmationWindow);

	// Yes button
	Button* YesButton;
	YesButton = new Button("YesButton", meshList[GEO_YES_BUTTON_UP], meshList[GEO_YES_BUTTON_DOWN], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.45), -(float)(sceneManager2D.m_window_height * 0.5), 0), Vector3((float)(sceneManager2D.m_window_width * 0.2), (float)(sceneManager2D.m_window_height * 0.1), 0));
	UIManagerConfirmation->addFeature(YesButton);

	// No button
	Button* NoButton;
	NoButton = new Button("NoButton", meshList[GEO_NO_BUTTON_UP], meshList[GEO_NO_BUTTON_DOWN], NULL, Vector3((float)(sceneManager2D.m_window_width * 0.45), -(float)(sceneManager2D.m_window_height * 0.5), 0), Vector3((float)(sceneManager2D.m_window_width * 0.2), (float)(sceneManager2D.m_window_height * 0.1), 0));
	UIManagerConfirmation->addFeature(NoButton);


	isInformationDisplayed = false;
}

void CSceneMenu::InitMeshes()
{
	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 800, 55);

	meshList[START_BUTTON_UP] = MeshBuilder::GenerateQuad("START_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[START_BUTTON_UP]->textureID = LoadTGA("Image//StartGameButton.tga");

	meshList[START_BUTTON_DOWN] = MeshBuilder::GenerateQuad("START_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[START_BUTTON_DOWN]->textureID = LoadTGA("Image//StartGameButtonPressed.tga");

	meshList[START_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("START_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[START_BUTTON_LOCKED]->textureID = LoadTGA("Image//StartGameButtonLocked.tga");

	meshList[LOAD_BUTTON_UP] = MeshBuilder::GenerateQuad("LOAD_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[LOAD_BUTTON_UP]->textureID = LoadTGA("Image//LoadGameButton.tga");

	meshList[LOAD_BUTTON_DOWN] = MeshBuilder::GenerateQuad("LOAD_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[LOAD_BUTTON_DOWN]->textureID = LoadTGA("Image//LoadGameButtonPressed.tga");
	
	meshList[LOAD_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("LOAD_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[LOAD_BUTTON_LOCKED]->textureID = LoadTGA("Image//LoadGameButtonLocked.tga");

	meshList[INSTRUCTIONS_BUTTON_UP] = MeshBuilder::GenerateQuad("INSTRUCTIONS_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[INSTRUCTIONS_BUTTON_UP]->textureID = LoadTGA("Image//InstructionButton.tga");

	meshList[INSTRUCTIONS_BUTTON_DOWN] = MeshBuilder::GenerateQuad("INSTRUCTIONS_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[INSTRUCTIONS_BUTTON_DOWN]->textureID = LoadTGA("Image//InstructionButtonPressed.tga");

	meshList[EXIT_BUTTON_UP] = MeshBuilder::GenerateQuad("EXIT_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[EXIT_BUTTON_UP]->textureID = LoadTGA("Image//ExitButton.tga");

	meshList[EXIT_BUTTON_DOWN] = MeshBuilder::GenerateQuad("EXIT_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[EXIT_BUTTON_DOWN]->textureID = LoadTGA("Image//ExitButtonPressed.tga");

	meshList[BACKGROUND] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[BACKGROUND]->textureID = LoadTGA("Image//MainMenuBackGround.tga");

	meshList[TITLE] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[TITLE]->textureID = LoadTGA("Image//MainMenuTitle.tga");

	meshList[SMOKE_LEFT] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[SMOKE_LEFT]->textureID = LoadTGA("Image//MainMenuSmokeLeft.tga");

	meshList[SMOKE_RIGHT] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[SMOKE_RIGHT]->textureID = LoadTGA("Image//MainMenuSmokeRight.tga");

	meshList[MUSIC_ICON] = MeshBuilder::GenerateQuad("MUSIC_ICON", Color(0, 0, 0), 1.f);
	meshList[MUSIC_ICON]->textureID = LoadTGA("Image//Music_Icon.tga");

	meshList[MUSIC_ICON_OFF] = MeshBuilder::GenerateQuad("MUSIC_ICON_OFF", Color(0, 0, 0), 1.f);
	meshList[MUSIC_ICON_OFF]->textureID = LoadTGA("Image//Music_Icon_Off.tga");

	meshList[SETTINGS_BUTTON_UP] = MeshBuilder::GenerateQuad("MUSIC_ICON_OFF", Color(0, 0, 0), 1.f);
	meshList[SETTINGS_BUTTON_UP]->textureID = LoadTGA("Image//SettingsIcon.tga");

	meshList[SETTINGS_BUTTON_DOWN] = MeshBuilder::GenerateQuad("MUSIC_ICON_OFF", Color(0, 0, 0), 1.f);
	meshList[SETTINGS_BUTTON_DOWN]->textureID = LoadTGA("Image//SettingsIconPressed.tga");

	//Informations Pointers

	meshList[INFORMATION_POINTER_LEFT] = MeshBuilder::GenerateQuad("INFORMATION_POINTER_LEFT", Color(1, 1, 1), 1);
	meshList[INFORMATION_POINTER_LEFT]->textureID = LoadTGA("Image//InformationPointerLeft.tga");

	meshList[INFORMATION_POINTER_LEFT_PRESSED] = MeshBuilder::GenerateQuad("INFORMATION_POINTER_LEFT_PRESSED", Color(1, 1, 1), 1);
	meshList[INFORMATION_POINTER_LEFT_PRESSED]->textureID = LoadTGA("Image//InformationPointerLeftPressed.tga");

	meshList[INFORMATION_POINTER_RIGHT] = MeshBuilder::GenerateQuad("INFORMATION_POINTER_RIGHT", Color(1, 1, 1), 1);
	meshList[INFORMATION_POINTER_RIGHT]->textureID = LoadTGA("Image//InformationPointerRight.tga");

	meshList[INFORMATION_POINTER_RIGHT_PRESSED] = MeshBuilder::GenerateQuad("INFORMATION_POINTER_RIGHT_PRESSED", Color(1, 1, 1), 1);
	meshList[INFORMATION_POINTER_RIGHT_PRESSED]->textureID = LoadTGA("Image//InformationPointerRightPressed.tga");
	
	meshList[INFORMATION_GAME_OVERVIEW] = MeshBuilder::GenerateQuad("INFORMATION_POINTER_RIGHT_PRESSED", Color(1, 1, 1), 1);
	meshList[INFORMATION_GAME_OVERVIEW]->textureID = LoadTGA("Image//InstructionPageGameOverview.tga");

	

	// Confirmation
	// Window
	meshList[GEO_CONFIRMATION_WINDOW] = MeshBuilder::GenerateQuad("GEO_CONFIRMATION_WINDOW", Color(1, 1, 1), 1);
	meshList[GEO_CONFIRMATION_WINDOW]->textureID = LoadTGA("Image//UI/New_Data_Confirmation_Window.tga");
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
}

void CSceneMenu::Update(double dt)
{
	UIManager->Update((float)dt);
	UIManagerConfirmation->Update((float)dt);
	UIManagerInformation->Update((float)dt);

	TimeSinceSettingsAnimation += (float)dt;
	MainMenuAnimationTimer += (float)dt;

	if (MainMenuAnimationTimer > 2.9)
	{
		if (UIManager->FindButton("SettingsButton")->getisHovered() == true)
		{
			TimeSinceSettingsAnimation = 0;
			if (isSettingsAnimationStarted == false)
			{
				isSettingsAnimationStarted = true;
				UIManager->InvokeAnimator()->SkipAllAnimations();
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Instructions"), 0, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40 + 80, 1), 5, 0);
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIcon"), 0, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40 + 150, 1), 5, 0);
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIconOff"), 0, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40 + 150, 1), 5, 0);
			}
		}
		else if (UIManager->FindButton("SettingsButton")->getisHovered() == false && isSettingsAnimationStarted == true && TimeSinceSettingsAnimation > 0.2)
		{
			isSettingsAnimationStarted = false;
			UIManager->InvokeAnimator()->SkipAllAnimations();
			UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Instructions"), 0, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40 - 80, 1), 5, 0);
			UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIcon"), 0, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40 - 150, 1), 5, 0);
			UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIconOff"), 0, Vector3((float)(sceneManager2D.m_window_width * 0.05), 40 - 150, 1), 5, 0);
		}

		if (UIManager->FindButton("Instructions")->getisHovered() == true)
		{
			TimeSinceSettingsAnimation = 0;
		}

		if (UIManager->FindButton("MusicIcon")->getisHovered() == true)
		{
			TimeSinceSettingsAnimation = 0;
		}

		if (UIManager->FindButton("MusicIconOff")->getisHovered() == true)
		{
			TimeSinceSettingsAnimation = 0;
		}
	}
	/*if (UIManager->FindButton("MusicIcon")->getActive())
	{
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIcon"), 0.5, Vector3(100, 100, 0), 5, UIAnimation::SCALING);
	}
	else
	{
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIcon"), 0.5, Vector3(0.001, 0.001, 0), 5, UIAnimation::SCALING);
	}*/

	/*
	if (Application::IsKeyPressed('1'))
	glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
	glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	*/

	sceneManager2D.Update(dt);
	/*SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]);
	if (sa)
	{
		sa->Update(dt);
	}*/
}

void CSceneMenu::HideConfirmation()
{
	UIManagerConfirmation->InvokeAnimator()->SkipAllAnimations();
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), (float)(0.1), Vector3((float)(sceneManager2D.m_window_width * 0.5), (float)(sceneManager2D.m_window_height * 1.5), 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), (float)(0.1), Vector3((float)(sceneManager2D.m_window_width * 0.45), -(float)(sceneManager2D.m_window_height * 0.5), 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), (float)(0.1), Vector3((float)(sceneManager2D.m_window_width * 0.45), -(float)(sceneManager2D.m_window_height * 0.5), 0), UI_Speed, UIAnimation::TRANSLATION);
	// AlphaQuad
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("AlphaQuad"), 0, Vector3(0, 0, 0), UI_Speed * 2, UIAnimation::SCALING);
}

void CSceneMenu::ShowConfirmation()
{
	UIManagerConfirmation->InvokeAnimator()->SkipAllAnimations();
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), (float)(0.1), Vector3((float)(sceneManager2D.m_window_width * 0.5), (float)(sceneManager2D.m_window_height * 0.65), 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), (float)(0.1), Vector3((float)(sceneManager2D.m_window_width * 0.385), (float)(sceneManager2D.m_window_height * 0.3), 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), (float)(0.1), Vector3((float)(sceneManager2D.m_window_width * 0.615), (float)(sceneManager2D.m_window_height * 0.3), 0), UI_Speed, UIAnimation::TRANSLATION);
	// AlphaQuad
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("AlphaQuad"), 0, Vector3((float)(sceneManager2D.m_window_width), (float)(sceneManager2D.m_window_height), 0), UI_Speed * 2, UIAnimation::SCALING);
}

/********************************************************************************
Render this scene
********************************************************************************/
void CSceneMenu::Render()
{
	sceneManager2D.Render();

	//// Gray Quad
	//sceneManager2D.Render2DMesh(meshList[GRAY_QUAD], false, 1, 1, 1, 0);

	//// Black Quad
	//switch (choice)
	//{
	//case PLAY:
	//	// Play
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 397);
	//	break;
	//case TIME_LIMIT:
	//	// Time-Limit
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 330);
	//	break;
	//case INSTRUCTION:
	//	// Instruction
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 270);
	//	break;
	//case RANKINGS:
	//	// Rankings
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 195);
	//	break;
	//case OPTIONS:
	//	// Options
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 129);
	//	break;
	//case EXIT:
	//	// Exit
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 70);
	//	break;
	//}

	//// Render the background image
	//sceneManager2D.Render2DMesh(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], false, 1, 1, 1, 0);

	/*for (int i = 0; i < UIManager->getUI_List().size(); ++ i)
	{
		switch (UIManager->getUI_List()[i]->getUI_Type())
		{
		case UIFeature::UT_BUTTON:
		{
			sceneManager2D.Render2DMesh(static_cast<Button*>(UIManager->getUI_List()[i])->getCurrentMesh(), false, UIManager->getUI_List()[i]->getScale().x, UIManager->getUI_List()[i]->getScale().y, UIManager->getUI_List()[i]->getCurrentPos().x, UIManager->getUI_List()[i]->getCurrentPos().y);
			break;
		}
		case UIFeature::UT_IMAGE:
		{
			sceneManager2D.Render2DMesh(static_cast<Image*>(UIManager->getUI_List()[i])->getMesh(), false, UIManager->getUI_List()[i]->getScale().x, UIManager->getUI_List()[i]->getScale().y, UIManager->getUI_List()[i]->getCurrentPos().x, UIManager->getUI_List()[i]->getCurrentPos().y);
			break;
		}
		}
	}*/
	UIManager->Render(sceneManager2D);
	UIManagerConfirmation->Render(sceneManager2D);
	UIManagerInformation->Render(sceneManager2D);

	// New Data
	if (currentState == CONFIRMATION)
	{
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], to_string(currentDataSelected + 1), Color(1, 1, 1), (float)(UIManagerConfirmation->FindImage("ConfirmationWindow")->getScale().y * 0.3), (float)(UIManagerConfirmation->FindImage("ConfirmationWindow")->getCurrentPos().x + UIManagerConfirmation->FindImage("ConfirmationWindow")->getScale().x * 0.195), (float)(UIManagerConfirmation->FindImage("ConfirmationWindow")->getCurrentPos().y + UIManagerConfirmation->FindImage("ConfirmationWindow")->getScale().y * 0.195));
	}
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CSceneMenu::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}