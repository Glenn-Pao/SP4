#include "SceneLoadGame.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

CSceneLoadGame::CSceneLoadGame()
	: currentState(SELECTING)
	, DataSelected(-1)
	, UI_Speed(0.f)
	, UIManagerSelecting(NULL)
	, UIManagerConfirmation(NULL)
{
	// Set all of the meshes NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

CSceneLoadGame::CSceneLoadGame(const int m_window_width, const int m_window_height)
	: currentState(SELECTING)
	, DataSelected(-1)
	, UI_Speed(0.f)
	, UIManagerSelecting(NULL)
	, UIManagerConfirmation(NULL)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;

	// Set all of the meshes NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

CSceneLoadGame::~CSceneLoadGame()
{
	if (UIManagerSelecting)
	{
		delete UIManagerSelecting;
		UIManagerSelecting = NULL;
	}
	if (UIManagerConfirmation)
	{
		delete UIManagerConfirmation;
		UIManagerConfirmation = NULL;
	}
}

void  CSceneLoadGame::Init(int level)
{
	UI_Speed = 6.f;

	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::GenerateQuad("GEO_BACKGROUND", Color(1, 1, 1), 1);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//UI/pink_white_stripe_background.tga");


	// Load the ground mesh and texture
	// Header
	meshList[GEO_HEADER] = MeshBuilder::GenerateQuad("GEO_HEADER", Color(1, 1, 1), 1);
	meshList[GEO_HEADER]->textureID = LoadTGA("Image//UI/LoadGame_Header.tga");

	// Buttons
	// Data 1
	meshList[GEO_DATA1_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA1_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA1_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data1_Button.tga");
	meshList[GEO_DATA1_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA1_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA1_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data1_Button_Pressed.tga");
	meshList[GEO_DATA1_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_DATA1_BUTTON_LOCKED", Color(1, 1, 1), 1);
	meshList[GEO_DATA1_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Data1_Button_Locked.tga");
	// Data 2
	meshList[GEO_DATA2_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA2_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA2_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data2_Button.tga");
	meshList[GEO_DATA2_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA2_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA2_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data2_Button_Pressed.tga");
	meshList[GEO_DATA2_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_DATA2_BUTTON_LOCKED", Color(1, 1, 1), 1);
	meshList[GEO_DATA2_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Data2_Button_Locked.tga");
	// Data 3
	meshList[GEO_DATA3_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA3_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA3_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data3_Button.tga");
	meshList[GEO_DATA3_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA3_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA3_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data3_Button_Pressed.tga");
	meshList[GEO_DATA3_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_DATA3_BUTTON_LOCKED", Color(1, 1, 1), 1);
	meshList[GEO_DATA3_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Data3_Button_Locked.tga");
	// Data 4
	meshList[GEO_DATA4_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA4_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA4_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data4_Button.tga");
	meshList[GEO_DATA4_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA4_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA4_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data4_Button_Pressed.tga");
	meshList[GEO_DATA4_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_DATA4_BUTTON_LOCKED", Color(1, 1, 1), 1);
	meshList[GEO_DATA4_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Data4_Button_Locked.tga");
	// Data 5
	meshList[GEO_DATA5_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA5_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA5_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data5_Button.tga");
	meshList[GEO_DATA5_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA5_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA5_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data5_Button_Pressed.tga");
	meshList[GEO_DATA5_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_DATA5_BUTTON_LOCKED", Color(1, 1, 1), 1);
	meshList[GEO_DATA5_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Data5_Button_Locked.tga");
	// Back
	meshList[GEO_BACK_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_BACK_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_BACK_BUTTON_UP]->textureID = LoadTGA("Image//UI/Back_Button.tga");
	meshList[GEO_BACK_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_BACK_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_BACK_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Back_Button_Pressed.tga");
	// Delete
	meshList[GEO_DELETE_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DELETE_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DELETE_BUTTON_UP]->textureID = LoadTGA("Image//UI/Cross_Button.tga");
	meshList[GEO_DELETE_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DELETE_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DELETE_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Cross_Button_Pressed.tga");

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

	// UI
	// Selecting
	UIManagerSelecting = new UISystem();

	// Background
	Image* Background;
	Background = new Image("Background", sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 1.5, 0), Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_height, 0));
	UIManagerSelecting->addFeature(Background);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindImage("Background"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);


	// Header
	Image* Header;
	Header = new Image("Header", meshList[GEO_HEADER], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.89, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Header);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindImage("Header"), 0.25, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.2, 0), UI_Speed, UIAnimation::SCALING);


	// Buttons
	// Data 1 button
	Button* Data1Button;
	Data1Button = new Button("Data1Button", meshList[GEO_DATA1_BUTTON_UP], meshList[GEO_DATA1_BUTTON_DOWN], meshList[GEO_DATA1_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.6, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Data1Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Data1Button"), 0.3, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), UI_Speed, UIAnimation::SCALING);
	// Delete 1 button
	Button* Delete1Button;
	Delete1Button = new Button("Delete1Button", meshList[GEO_DELETE_BUTTON_UP], meshList[GEO_DELETE_BUTTON_DOWN], meshList[GEO_DELETE_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.4, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Delete1Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Delete1Button"), 0.3, Vector3(sceneManager2D.m_window_width * 0.05, sceneManager2D.m_window_width * 0.05, 0), UI_Speed, UIAnimation::SCALING);
	// Data 2 button
	Button* Data2Button;
	Data2Button = new Button("Data2Button", meshList[GEO_DATA2_BUTTON_UP], meshList[GEO_DATA2_BUTTON_DOWN], meshList[GEO_DATA2_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.6, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Data2Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Data2Button"), 0.35, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), UI_Speed, UIAnimation::SCALING);
	// Delete 2 button
	Button* Delete2Button;
	Delete2Button = new Button("Delete2Button", meshList[GEO_DELETE_BUTTON_UP], meshList[GEO_DELETE_BUTTON_DOWN], meshList[GEO_DELETE_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.4, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Delete2Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Delete2Button"), 0.3, Vector3(sceneManager2D.m_window_width * 0.05, sceneManager2D.m_window_width * 0.05, 0), UI_Speed, UIAnimation::SCALING);
	// Data 3 button
	Button* Data3Button;
	Data3Button = new Button("Data3Button", meshList[GEO_DATA3_BUTTON_UP], meshList[GEO_DATA3_BUTTON_DOWN], meshList[GEO_DATA3_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.8, sceneManager2D.m_window_height * 0.6, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Data3Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Data3Button"), 0.4, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), UI_Speed, UIAnimation::SCALING);
	// Delete 3 button
	Button* Delete3Button;
	Delete3Button = new Button("Delete3Button", meshList[GEO_DELETE_BUTTON_UP], meshList[GEO_DELETE_BUTTON_DOWN], meshList[GEO_DELETE_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.8, sceneManager2D.m_window_height * 0.4, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Delete3Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Delete3Button"), 0.3, Vector3(sceneManager2D.m_window_width * 0.05, sceneManager2D.m_window_width * 0.05, 0), UI_Speed, UIAnimation::SCALING);
	// Data 4 button
	Button* Data4Button;
	Data4Button = new Button("Data4Button", meshList[GEO_DATA4_BUTTON_UP], meshList[GEO_DATA4_BUTTON_DOWN], meshList[GEO_DATA4_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.35, sceneManager2D.m_window_height * 0.25, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Data4Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Data4Button"), 0.45, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), UI_Speed, UIAnimation::SCALING);
	// Delete 4 button
	Button* Delete4Button;
	Delete4Button = new Button("Delete4Button", meshList[GEO_DELETE_BUTTON_UP], meshList[GEO_DELETE_BUTTON_DOWN], meshList[GEO_DELETE_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.35, sceneManager2D.m_window_height * 0.05, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Delete4Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Delete4Button"), 0.3, Vector3(sceneManager2D.m_window_width * 0.05, sceneManager2D.m_window_width * 0.05, 0), UI_Speed, UIAnimation::SCALING);
	// Data 5 button
	Button* Data5Button;
	Data5Button = new Button("Data5Button", meshList[GEO_DATA5_BUTTON_UP], meshList[GEO_DATA5_BUTTON_DOWN], meshList[GEO_DATA5_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.65, sceneManager2D.m_window_height * 0.25, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Data5Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Data5Button"), 0.5, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), UI_Speed, UIAnimation::SCALING);
	// Delete 5 button
	Button* Delete5Button;
	Delete5Button = new Button("Delete5Button", meshList[GEO_DELETE_BUTTON_UP], meshList[GEO_DELETE_BUTTON_DOWN], meshList[GEO_DELETE_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.65, sceneManager2D.m_window_height * 0.05, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(Delete5Button);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("Delete5Button"), 0.3, Vector3(sceneManager2D.m_window_width * 0.05, sceneManager2D.m_window_width * 0.05, 0), UI_Speed, UIAnimation::SCALING);


	// Back button
	Button* BackButton;
	BackButton = new Button("BackButton", meshList[GEO_BACK_BUTTON_UP], meshList[GEO_BACK_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.1, sceneManager2D.m_window_height * 0.1, 0), Vector3(0, 0, 0));
	UIManagerSelecting->addFeature(BackButton);
	UIManagerSelecting->InvokeAnimator()->StartTransformation(UIManagerSelecting->FindButton("BackButton"), 0.55, Vector3(sceneManager2D.m_window_width * 0.125, sceneManager2D.m_window_width * 0.125, 0), UI_Speed, UIAnimation::SCALING);


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

void CSceneLoadGame::HideConfirmation()
{
	UIManagerConfirmation->InvokeAnimator()->SkipAllAnimations();
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// AlphaQuad
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("AlphaQuad"), 0, Vector3(0, 0, 0), UI_Speed * 2, UIAnimation::SCALING);
}

void CSceneLoadGame::ShowConfirmation()
{
	UIManagerConfirmation->InvokeAnimator()->SkipAllAnimations();
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.65, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.385, sceneManager2D.m_window_height * 0.3, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.615, sceneManager2D.m_window_height * 0.3, 0), UI_Speed, UIAnimation::TRANSLATION);
	// AlphaQuad
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("AlphaQuad"), 0, Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_height, 0), UI_Speed * 2, UIAnimation::SCALING);
}

void   CSceneLoadGame::Update(double dt)
{
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

	// UI
	UIManagerSelecting->Update( dt);
	UIManagerConfirmation->Update(dt);
}

/********************************************************************************
Render this scene
********************************************************************************/
void   CSceneLoadGame::Render()
{
	sceneManager2D.Render();

	UIManagerSelecting->Render(sceneManager2D);
	UIManagerConfirmation->Render(sceneManager2D);
}

/********************************************************************************
Exit this scene
********************************************************************************/
void   CSceneLoadGame::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}