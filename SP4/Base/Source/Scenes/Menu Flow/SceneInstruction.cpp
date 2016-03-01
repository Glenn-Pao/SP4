#include "SceneInstruction.h"
#include "GL\glew.h"
#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

CSceneInstruction::CSceneInstruction()
	: UIManager(NULL)
	, UI_Speed(0.f)
{
}

CSceneInstruction::CSceneInstruction(const int m_window_width, const int m_window_height)
	: UIManager(NULL)
	, UI_Speed(0.f)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

  CSceneInstruction::~  CSceneInstruction()
{
	  if (UIManager)
	  {
		  delete UIManager;
		  UIManager = NULL;
	  }
}

  void  CSceneInstruction::Init(int level)
{
	  UI_Speed = 1.5;
	// Init the base scene
	sceneManager2D.Init(level);

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[PAGE_1] = MeshBuilder::Generate2DMesh("PAGE_1", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[PAGE_1]->textureID = LoadTGA("Image//Scenes/Instruction.tga");

	meshList[BACKGROUND] = MeshBuilder::GenerateQuad("BACKGROUND", Color(0, 0, 0), 1.f);
	meshList[BACKGROUND]->textureID = LoadTGA("Image//MainMenuBackGround.tga");
	// Back
	meshList[BACK_BUTTON_UP] = MeshBuilder::GenerateQuad("BACK_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[BACK_BUTTON_UP]->textureID = LoadTGA("Image//UI/Back_Button.tga");
	meshList[BACK_BUTTON] = MeshBuilder::GenerateQuad("BACK_BUTTON", Color(1, 1, 1), 1);
	meshList[BACK_BUTTON]->textureID = LoadTGA("Image//UI/Back_Button_Pressed.tga");
	choice = NONE;
	currentPage = FIRST_PAGE;
	
	UIManager = new UISystem();

	Image* BackGround;
	BackGround = new Image("BackGround", meshList[BACKGROUND], Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 2, 0), Vector3(1, 1, 0));
	UIManager->addFeature(BackGround);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("BackGround"), 0.55, Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_width, 0), UI_Speed, UIAnimation::SCALING);

	// Back button
	Button* BackButton;
	BackButton = new Button("BackButton", meshList[BACK_BUTTON_UP], meshList[BACK_BUTTON], NULL, Vector3(sceneManager2D.m_window_width * 0.1, sceneManager2D.m_window_height * 0.1, 0), Vector3(0, 0, 0));
	UIManager->addFeature(BackButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("BackButton"), 0.55, Vector3(sceneManager2D.m_window_width * 0.125, sceneManager2D.m_window_width * 0.125, 0), UI_Speed, UIAnimation::SCALING);
}

void   CSceneInstruction::Update(double dt)
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
	UIManager->Update(dt);
}

/********************************************************************************
Render this scene
********************************************************************************/
void   CSceneInstruction::Render()
{
	sceneManager2D.Render();
	UIManager->Render(sceneManager2D);
}

/********************************************************************************
Exit this scene
********************************************************************************/
void   CSceneInstruction::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}