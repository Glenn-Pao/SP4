#include "SceneOptions.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

#include "..\..\UsingLua.h"

CSceneOptions::CSceneOptions()
{
}

CSceneOptions::CSceneOptions(const int m_window_width, const int m_window_height)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

CSceneOptions::~CSceneOptions()
{
}

void  CSceneOptions::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Options.tga");


	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 205, 155);
	meshList[BLACK_SQUARE] = MeshBuilder::Generate2DMesh("BLACK_SQUARE", Color(0, 0, 0), 0, 0, 35, 30);

	choice = NONE;
	
	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//Options.lua");

	fpsSelected = L.DoLuaInt("showFPS");
	fullscreenSelected = L.DoLuaInt("fullscreen");
	coloredSelected = L.DoLuaInt("colored");
}

void   CSceneOptions::Update(double dt)
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
void   CSceneOptions::Render()
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

	// Fps
	if (fpsSelected)
		sceneManager2D.Render2DMesh(meshList[BLACK_SQUARE], false, 1, 1, 429, 428);
	// Fullscreen
	if (fullscreenSelected)
		sceneManager2D.Render2DMesh(meshList[BLACK_SQUARE], false, 1, 1, 429, 333);
	// Colored
	if (coloredSelected)
		sceneManager2D.Render2DMesh(meshList[BLACK_SQUARE], false, 1, 1, 429, 238);
}

/********************************************************************************
Exit this scene
********************************************************************************/
void   CSceneOptions::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();

	std::ofstream myfile("Lua//Options.lua");
	int i = 0;
	if (myfile.is_open())
	{
		myfile << "showFPS = " << fpsSelected << endl;
		myfile << "fullscreen = " << fullscreenSelected << endl;
		myfile << "colored = " << coloredSelected << endl;
	}
}