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
}

CSceneLoadGame::CSceneLoadGame(const int m_window_width, const int m_window_height)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

CSceneLoadGame::~CSceneLoadGame()
{
}

void  CSceneLoadGame::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Level_selection.tga");


	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 205, 155);

	choice = NONE;
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
}

/********************************************************************************
Render this scene
********************************************************************************/
void   CSceneLoadGame::Render()
{
	sceneManager2D.Render();

	// Gray Quad
	sceneManager2D.Render2DMesh(meshList[GRAY_QUAD], false, 1, 1, 1, 0);

	// Black Quad
	switch (choice)
	{
	case BACK:
		// BACK
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, -50);
		break;
	case LEVEL_ONE:
		// 1
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 40, 300);
		break;
	case LEVEL_TWO:
		// 2
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 300, 300);
		break;
	case LEVEL_THREE:
		// 3
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 560, 300);
		break;
	case LEVEL_FOUR:
		// 4
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 170, 120);
		break;
	case LEVEL_FIVE:
		// 5
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 430, 120);
		break;
	}

	// Render the background image
	sceneManager2D.Render2DMesh(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], false, 1, 1, 1, 0);
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