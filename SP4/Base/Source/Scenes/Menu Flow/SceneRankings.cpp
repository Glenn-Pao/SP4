#include "SceneRankings.h"

#include "GL\glew.h"
#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"


CSceneRankings::CSceneRankings()
{
}

CSceneRankings::CSceneRankings(const int m_window_width, const int m_window_height)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

CSceneRankings::~CSceneRankings()
{
}

void  CSceneRankings::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Rankings.tga");


	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 205, 155);

	highscore.ReadFromTextFile();
	choice = NONE;
}

void   CSceneRankings::Update(double dt)
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
void   CSceneRankings::Render()
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
	}

	// Render the background image
	sceneManager2D.Render2DMesh(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], false, 1, 1, 1, 0);

	// On screen text
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "RANK NAME SCORE" , Color(0, 0, 0), 50, 200, sceneManager2D.m_window_height - 160);

	for (int i = 0; i < 5; ++i)
	{
		int temp = i;
		std::ostringstream scoring;
		scoring << temp + 1 << "." << highscore.record[i].getName() << " " << highscore.record[i].getScore();
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scoring.str(), Color(0, 0, 0), 50, 260, sceneManager2D.m_window_height - 220 - i * 60);
	}
}

/********************************************************************************
Exit this scene
********************************************************************************/
void   CSceneRankings::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}