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
{
}

CSceneInstruction::CSceneInstruction(const int m_window_width, const int m_window_height)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

  CSceneInstruction::~  CSceneInstruction()
{
}

  void  CSceneInstruction::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[LIGHT_GRAY_QUAD] = MeshBuilder::Generate2DMesh("LIGHT_GRAY_QUAD", Color(0.9, 0.9, 0.9), 0, 0, 200, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 200, 600);

	meshList[PAGE_1] = MeshBuilder::Generate2DMesh("PAGE_1", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[PAGE_1]->textureID = LoadTGA("Image//Scenes/Instruction.tga");
	meshList[PAGE_2] = MeshBuilder::Generate2DMesh("PAGE_2", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[PAGE_2]->textureID = LoadTGA("Image//Scenes/Instruction_2.tga");

	choice = NONE;
	currentPage = FIRST_PAGE;
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
}

/********************************************************************************
Render this scene
********************************************************************************/
void   CSceneInstruction::Render()
{
	sceneManager2D.Render();

	// Gray Quad
	sceneManager2D.Render2DMesh(meshList[GRAY_QUAD], false, 1, 1, 1, 0);

	// Black Quad
	switch (choice)
	{
	case BACK:
		// BACK
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 0);
		break;
	case NEXT:
		// NEXT
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 740, 0);
		break;
	case PREVIOUS:
		// PREVIOUS
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 540, 0);
		break;
	}

	// Page
	switch (currentPage)
	{
	case FIRST_PAGE:
		// PREVIOUS
		sceneManager2D.Render2DMesh(meshList[LIGHT_GRAY_QUAD], false, 1, 1, 540, 0);

		sceneManager2D.Render2DMesh(meshList[PAGE_1], false, 1, 1, 1, 0);
		break;
	case SECOND_PAGE:
		// NEXT
		sceneManager2D.Render2DMesh(meshList[LIGHT_GRAY_QUAD], false, 1, 1, 740, 0);

		sceneManager2D.Render2DMesh(meshList[PAGE_2], false, 1, 1, 1, 0);
		break;
	}
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