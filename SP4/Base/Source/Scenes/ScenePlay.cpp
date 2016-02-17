#include "ScenePlay.h"
#include "GL\glew.h"

#include "..\shader.hpp"
#include "..\MeshBuilder.h"
#include "..\Application.h"
#include "..\Utility.h"
#include "..\LoadTGA.h"
#include <sstream>
#include "..\Strategy_Kill.h"

CScenePlay::CScenePlay()
	: timeElapsed(0.f)
	, noOfDots(0.f)
	, ringScaleOffset(0.0f)
	, state(LOADING)
{
}

CScenePlay::CScenePlay(const int m_window_width, const int m_window_height)
	: timeElapsed(0.f)
	, noOfDots(0.f)
	, ringScaleOffset(0.0f)
	, state(LOADING)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

CScenePlay::~CScenePlay()
{
}

void CScenePlay::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Splash.tga");

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture

	meshList[GEO_RAINBOW_WHEEL] = MeshBuilder::Generate2DMesh("GEO_RAINBOW_WHEEL", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_RAINBOW_WHEEL]->textureID = LoadTGA("Image//Scenes/rainbow_wheel.tga");
}

void CScenePlay::Update(double dt)
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

	switch (state)
	{
		case LOADING:
		{
			timeElapsed += dt;
			if (timeElapsed >= 4.f)
			{
				state = LOADED;
			}
			noOfDots += 2 * dt;
			if (noOfDots >= 4.f)
			{
				noOfDots = 0.f;
			}
		}
		break;
		case LOADED:
		{
			timeElapsed += dt;
		}
		break;
		case EXITING:
		{
			ringScaleOffset += 2000 * dt;
			if (ringScaleOffset >= 700.f)
			{
				state = EXIT;
			}
		}
		break;
	}
}

/********************************************************************************
Render this scene
********************************************************************************/
void CScenePlay::Render()
{
	sceneManager2D.Render();

	// Render the background image
	sceneManager2D.Render2DMesh(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], false, 1, 1, 1, 0);

	sceneManager2D.Render2DMesh(meshList[GEO_RAINBOW_WHEEL], false, 600 + ringScaleOffset, 600 + ringScaleOffset, 100 - ringScaleOffset * 0.5, 0 - ringScaleOffset * 0.5, timeElapsed * 20);

	//On screen text
	switch (state)
	{
		case LOADED:
		{
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "<click to continue>", Color(0, 0, 0), 20, 290, 225);
		}
		break;
		case LOADING:
		{
			std::ostringstream ss;
			switch ((int)noOfDots)
			{
			case 0:
				ss << "";
				break;
			case 1:
				ss << "o";
				break;
			case 2:
				ss << "o o";
				break;
			case 3:
				ss << "o o o";
				break;
			}
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 0, 0), 50, 330, 225);
		}
		break;
	}
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CScenePlay::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}