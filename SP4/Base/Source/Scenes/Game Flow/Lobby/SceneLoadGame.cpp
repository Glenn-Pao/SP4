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
{
	// Set all of the meshes NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

CSceneLoadGame::CSceneLoadGame(const int m_window_width, const int m_window_height)
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
}

void  CSceneLoadGame::Init(int level)
{
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
	// Data 2
	meshList[GEO_DATA2_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA2_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA2_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data2_Button.tga");
	meshList[GEO_DATA2_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA2_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA2_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data2_Button_Pressed.tga");
	// Data 3
	meshList[GEO_DATA3_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA3_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA3_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data3_Button.tga");
	meshList[GEO_DATA3_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA3_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA3_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data3_Button_Pressed.tga");
	// Data 4
	meshList[GEO_DATA4_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA4_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA4_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data4_Button.tga");
	meshList[GEO_DATA4_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA4_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA4_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data4_Button_Pressed.tga");
	// Data 5
	meshList[GEO_DATA5_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DATA5_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_DATA5_BUTTON_UP]->textureID = LoadTGA("Image//UI/Data5_Button.tga");
	meshList[GEO_DATA5_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DATA5_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_DATA5_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Data5_Button_Pressed.tga");
	// Back
	meshList[GEO_BACK_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_BACK_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_BACK_BUTTON_UP]->textureID = LoadTGA("Image//UI/Back_Button.tga");
	meshList[GEO_BACK_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_BACK_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_BACK_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Back_Button_Pressed.tga");

	// UI
	UIManager = new UISystem();

	// Background
	Image* Background;
	Background = new Image("Background", sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 1.5, 0), Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_height, 0));
	UIManager->addFeature(Background);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Background"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), 6, UIAnimation::TRANSLATION);


	// Header
	Image* Header;
	Header = new Image("Header", meshList[GEO_HEADER], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.89, 0), Vector3(0, 0, 0));
	UIManager->addFeature(Header);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Header"), 0.25, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.2, 0), 6, UIAnimation::SCALING);


	// Buttons
	// Data 1 button
	Button* Data1Button;
	Data1Button = new Button("Data1Button", meshList[GEO_DATA1_BUTTON_UP], meshList[GEO_DATA1_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.6, 0), Vector3(0, 0, 0));
	UIManager->addFeature(Data1Button);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Data1Button"), 0.3, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), 6, UIAnimation::SCALING);
	// Data 2 button
	Button* Data2Button;
	Data2Button = new Button("Data2Button", meshList[GEO_DATA2_BUTTON_UP], meshList[GEO_DATA2_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.6, 0), Vector3(0, 0, 0));
	UIManager->addFeature(Data2Button);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Data2Button"), 0.35, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), 6, UIAnimation::SCALING);
	// Data 3 button
	Button* Data3Button;
	Data3Button = new Button("Data3Button", meshList[GEO_DATA3_BUTTON_UP], meshList[GEO_DATA3_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.8, sceneManager2D.m_window_height * 0.6, 0), Vector3(0, 0, 0));
	UIManager->addFeature(Data3Button);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Data3Button"), 0.4, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), 6, UIAnimation::SCALING);
	// Data 4 button
	Button* Data4Button;
	Data4Button = new Button("Data4Button", meshList[GEO_DATA4_BUTTON_UP], meshList[GEO_DATA4_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.35, sceneManager2D.m_window_height * 0.25, 0), Vector3(0, 0, 0));
	UIManager->addFeature(Data4Button);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Data4Button"), 0.45, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), 6, UIAnimation::SCALING);
	// Data 5 button
	Button* Data5Button;
	Data5Button = new Button("Data5Button", meshList[GEO_DATA5_BUTTON_UP], meshList[GEO_DATA5_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.65, sceneManager2D.m_window_height * 0.25, 0), Vector3(0, 0, 0));
	UIManager->addFeature(Data5Button);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Data5Button"), 0.5, Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_width * 0.2, 0), 6, UIAnimation::SCALING);


	// Back button
	Button* BackButton;
	BackButton = new Button("BackButton", meshList[GEO_BACK_BUTTON_UP], meshList[GEO_BACK_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.1, sceneManager2D.m_window_height * 0.1, 0), Vector3(0, 0, 0));
	UIManager->addFeature(BackButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("BackButton"), 0.55, Vector3(sceneManager2D.m_window_width * 0.125, sceneManager2D.m_window_width * 0.125, 0), 6, UIAnimation::SCALING);
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
	UIManager->Update(Application::mouse_current_x, Application::mouse_current_y, dt);
}

/********************************************************************************
Render this scene
********************************************************************************/
void   CSceneLoadGame::Render()
{
	sceneManager2D.Render();

	UIManager->Render(sceneManager2D);
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