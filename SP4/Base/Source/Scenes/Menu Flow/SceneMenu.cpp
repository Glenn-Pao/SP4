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
{
}

CSceneMenu::CSceneMenu(const int m_window_width, const int m_window_height)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
	
}

CSceneMenu::~CSceneMenu()
{
}

void CSceneMenu::Init(int level)
{
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

	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 800, 55);
	
	meshList[PLAYBUTTON_UP] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[PLAYBUTTON_UP]->textureID = LoadTGA("Image//Play Button.tga");

	meshList[PLAYBUTTON_DOWN] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[PLAYBUTTON_DOWN]->textureID = LoadTGA("Image//Play Button Pressed.tga");
	
	choice = NONE;

	UIManager = new UISystem();

	Button* PlayButton;
	PlayButton = new Button("PlayButton", meshList[PLAYBUTTON_UP], meshList[PLAYBUTTON_DOWN], Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 2, 0), Vector3(100, 50, 1));

	UIManager->addFeature(PlayButton);
}

void CSceneMenu::Update(double dt)
{
	UIManager->Update(Application::mouse_current_x, Application::mouse_current_y, dt);
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

	for (int i = 0; i < UIManager->getUI_List().size(); ++ i)
	{
		switch (UIManager->getUI_List()[i]->getUI_Type())
		{
		case UIFeature::UT_BUTTON:
		{
			sceneManager2D.Render2DMesh(static_cast<Button*>(UIManager->getUI_List()[i])->getCurrentMesh(), false, static_cast<Button*>(UIManager->getUI_List()[i])->getScale().x, static_cast<Button*>(UIManager->getUI_List()[i])->getScale().y, static_cast<Button*>(UIManager->getUI_List()[i])->getCurrentPos().x, static_cast<Button*>(UIManager->getUI_List()[i])->getCurrentPos().y);
			break;
		}
		}
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