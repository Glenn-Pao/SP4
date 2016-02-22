#include "SceneTutorialGame3.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

extern "C" {
#include "..\..\..\Lua\lua.h"
#include "..\..\..\Lua\lualib.h"
#include "..\..\..\Lua\lauxlib.h"
}

CSceneTutorialGame3::CSceneTutorialGame3(const int m_window_width, const int m_window_height)
	: theDoor(NULL)
	, currentState(PLAYING)
	, lives(3)
	, count(0)
{
}

CSceneTutorialGame3::~CSceneTutorialGame3()
{
	for (int i = 0; i < theQuestions.size(); i++)
	{
		if (theQuestions[i])
		{
			delete theQuestions[i];
			theQuestions[i] = NULL;
		}
	}
	if (theDoor)
	{
		delete theDoor;
	}
}

void CSceneTutorialGame3::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	lua_State *L = lua_open();

	//Read a value from the lua text file
	luaL_openlibs(L);

	if (luaL_loadfile(L, "Lua//scenePlay2D.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}

	// Fullscreen
	lua_getglobal(L, "tileSize");
	if (!lua_isnumber(L, -1)) {
		printf("`tileSize' should be a number\n");
	}
	int tileSize = (int)lua_tointeger(L, -1);

	lua_close(L);

	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 33 * tileSize, 16 * tileSize, tileSize);
	m_cMap->LoadMap("Image//Maps//Game 3/Tutorial.csv");

	//initialise the waypoints
	waypoints = new CWaypoints();
	waypoints->LoadWaypoints(m_cMap);
	temp = waypoints->getWaypointsVector();

	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			// Hero
			if (m_cMap->theScreenMap[i][k] == 1000 && theHero == NULL)
			{
				// Initialise the hero's position
				theHero = new CPlayerInfo();
				theHero->setPositionX(k*m_cMap->GetTileSize());
				theHero->setPositionY((m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
			}
			// Door
			else if (m_cMap->theScreenMap[i][k] == 30)
			{
				// Create a new door
				theDoor = new CDoor(1, Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);
			}
			// Questions
			else if (m_cMap->theScreenMap[i][k] == 41)
			{
				theQuestions.push_back(new CQuestion());
				theQuestions.back()->setID(count);
				theQuestions.back()->setActive(true);
				theQuestions.back()->setInteractivity(true);
				theQuestions.back()->setPositionX(k*m_cMap->GetTileSize());
				theQuestions.back()->setPositionY(sceneManager2D.m_window_height - i*m_cMap->GetTileSize() - m_cMap->GetTileSize());
				theQuestions.back()->setScale(Vector3(tileSize, tileSize, 1));
				theQuestions.back()->setBoundingBox(Vector3((theQuestions.back()->getPosition().x - (theQuestions.back()->getScale().x * 1.5)), (theQuestions.back()->getPosition().y + (theQuestions.back()->getScale().y * 1.5)), 0), Vector3((theQuestions.back()->getPosition().x + (theQuestions.back()->getScale().x * 1.5)), (theQuestions.back()->getPosition().y - (theQuestions.back()->getScale().y * 1.5)), 0));
				count++;
			}
		}
	}
	// Jellybeans
	JellybeanSystem = new CJellybeanSystem;

	// Initialise the Meshes
	InitMeshes();

	for (int i = 0; i < theQuestions.size(); i++)
	{
		theQuestions.at(i)->setMesh(meshList[GEO_TILE_QN]);
	}
}

void CSceneTutorialGame3::PreInit()
{
	sceneManager2D.PreInit();
}

/********************************************************************************
Initialise the meshes.
********************************************************************************/
void CSceneTutorialGame3::InitMeshes()
{
	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GEO_TILE_WALL] = MeshBuilder::Generate2DMesh("GEO_TILE_WALL", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_WALL]->textureID = LoadTGA("Image//Tile/wall.tga");
	meshList[GEO_TILE_GROUND] = MeshBuilder::Generate2DMesh("GEO_TILE_GROUND", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_GROUND]->textureID = LoadTGA("Image//Tile/ground.tga");
	meshList[GEO_TILEHERO] = MeshBuilder::Generate2DMesh("GEO_TILEHERO", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILEHERO]->textureID = LoadTGA("Image//tile2_hero.tga");
	meshList[GEO_TILETREE] = MeshBuilder::Generate2DMesh("GEO_TILETREE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILETREE]->textureID = LoadTGA("Image//tile3_tree.tga");
	meshList[GEO_TILESTRUCTURE] = MeshBuilder::Generate2DMesh("GEO_TILESTRUCTURE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILESTRUCTURE]->textureID = LoadTGA("Image//tile3_structure.tga");
	meshList[GEO_TILE_DOOR] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_DOOR]->textureID = LoadTGA("Image//tile30_hubdoor.tga");
	meshList[GEO_TILE_QN] = MeshBuilder::Generate2DMesh("GEO_TILE_QN", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_QN]->textureID = LoadTGA("Image//tile41_qn.tga");
	meshList[GEO_HEART] = MeshBuilder::Generate2DMesh("GEO_HEART", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_HEART]->textureID = LoadTGA("Image//heart.tga");
	// Hero
	// Side
	for (int i = 0; i < CPlayerInfo::NUM_GEOMETRY_SIDE; i++)
	{
		theHero->meshList[CPlayerInfo::GEO_TILEHERO_SIDE_FRAME0 + i] = MeshBuilder::GeneratePartOfSpriteSheet2D("GEO_TILEHERO_SIDE_FRAME" + to_string(i), 1, 1, 21, 13, 11, 0 + i);
		theHero->meshList[CPlayerInfo::GEO_TILEHERO_SIDE_FRAME0 + i]->textureID = LoadTGA("Image//player1.tga");
	}
	// Up
	for (int i = 0; i < CPlayerInfo::NUM_GEOMETRY_UP - CPlayerInfo::NUM_GEOMETRY_SIDE - 1; i++)
	{
		theHero->meshList[CPlayerInfo::GEO_TILEHERO_UP_FRAME0 + i] = MeshBuilder::GeneratePartOfSpriteSheet2D("GEO_TILEHERO_UP_FRAME" + to_string(i), 1, 1, 21, 13, 8, 0 + i);
		theHero->meshList[CPlayerInfo::GEO_TILEHERO_UP_FRAME0 + i]->textureID = LoadTGA("Image//player1.tga");
	}
	// Down
	for (int i = 0; i < CPlayerInfo::NUM_GEOMETRY_DOWN - CPlayerInfo::NUM_GEOMETRY_UP - 1; i++)
	{
		theHero->meshList[CPlayerInfo::GEO_TILEHERO_DOWN_FRAME0 + i] = MeshBuilder::GeneratePartOfSpriteSheet2D("GEO_TILEHERO_DOWN_FRAME" + to_string(i), 1, 1, 21, 13, 10, 0 + i);
		theHero->meshList[CPlayerInfo::GEO_TILEHERO_DOWN_FRAME0 + i]->textureID = LoadTGA("Image//player1.tga");
	}
	// AI
	meshList[GEO_TILE_KILLZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_KILLZONE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_KILLZONE]->textureID = LoadTGA("Image//tile10_killzone.tga");
	meshList[GEO_TILE_SAFEZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_SAFEZONE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_SAFEZONE]->textureID = LoadTGA("Image//tile11_safezone.tga");
	meshList[GEO_TILEENEMY_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME0", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILEENEMY_FRAME0]->textureID = LoadTGA("Image//tile20_enemy.tga");

	// Jellybeans
	JellybeanSystem->mesh = MeshBuilder::Generate2DMesh("GEO_JELLYBEAN", Color(1, 1, 1), 0, 0, 1, 1);
	JellybeanSystem->mesh->textureID = LoadTGA("Image//jellybean.tga");

}

void CSceneTutorialGame3::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	sceneManager2D.Update(dt);

	Vector3 prevHeroPos = Vector3(theHero->getPositionX(), theHero->getPositionY());
	// Update the hero
	if (Application::IsKeyPressed('W'))
		this->theHero->MoveUpDown(true, dt, m_cMap);
	if (Application::IsKeyPressed('S'))
		this->theHero->MoveUpDown(false, dt, m_cMap);
	if (Application::IsKeyPressed('A'))
		this->theHero->MoveLeftRight(true, dt, m_cMap);
	if (Application::IsKeyPressed('D'))
		this->theHero->MoveLeftRight(false, dt, m_cMap);

	// Update Hero animation counter if hero moved
	if (prevHeroPos != Vector3(theHero->getPositionX(), theHero->getPositionY()))
	{
		theHero->SetAnimationCounter(theHero->GetAnimationCounter() + theHero->GetMovementSpeed() * m_cMap->GetTileSize() * dt * theHero->GetAnimationSpeed());
		if (theHero->GetAnimationCounter() > theHero->GetAnimationMaxCounter())
			theHero->SetAnimationCounter(1);
	}
	else
	{
		theHero->SetAnimationCounter(0);
	}
	theHero->HeroUpdate(m_cMap, dt);
	for (int i = 0; i < theQuestions.size(); i++)
	{
		if (theQuestions[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			cout << "Collide" << endl;
		}
		else
		{
			cout << "No collide" << endl;
		}
	}
	
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneTutorialGame3::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneTutorialGame3::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}


/********************************************************************************
Render this scene
********************************************************************************/
void CSceneTutorialGame3::Render()
{
	sceneManager2D.Render();

	sceneManager2D.modelStack.PushMatrix();

	sceneManager2D.modelStack.Translate(-theHero->GetMapOffset_x(), theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);

	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();

	// Render Hero
	RenderHero();
	// Render AIs
	RenderAIs();
	
	sceneManager2D.modelStack.PopMatrix();
	RenderGUI();
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CSceneTutorialGame3::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();

	// Delete JellybeanSystem
	if (JellybeanSystem)
		delete JellybeanSystem;
}

/********************************************************************************
Render the tile map. This is a private function for use in this class only
********************************************************************************/
void CSceneTutorialGame3::RenderTileMap()
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			if (m_cMap->theScreenMap[i][k] == 1)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_WALL], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][k] == 30)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_DOOR], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][k] == 41)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_QN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}
			else
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_GROUND], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}
		}
	}
}

/********************************************************************************
Render the Hero. This is a private function for use in this class only
********************************************************************************/
void CSceneTutorialGame3::RenderHero()
{
	switch (theHero->GetAnimationDirection())
	{
	case CPlayerInfo::RIGHT:
	{
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_SIDE_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theHero->getPositionX(), theHero->getPositionY());
	}
		break;
	case CPlayerInfo::LEFT:
	{
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_SIDE_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theHero->getPositionX(), theHero->getPositionY(), 0.0f, true);
	}
		break;
	case CPlayerInfo::UP:
	{
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_UP_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theHero->getPositionX(), theHero->getPositionY());
	}
		break;
	case CPlayerInfo::DOWN:
	{
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_DOWN_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theHero->getPositionX(), theHero->getPositionY());
	}
		break;
	}
}
void CSceneTutorialGame3::RenderGUI()
{
	// Jellybean
	sceneManager2D.Render2DMesh(JellybeanSystem->mesh, false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());


	//On screen text
	std::ostringstream ss;
	// Jellybean
	ss.precision(3);
	ss << ": " << JellybeanSystem->GetNumOfJellybeans();
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), m_cMap->GetTileSize(), m_cMap->GetTileSize(), sceneManager2D.m_window_height - m_cMap->GetTileSize());

	for (int i = 0; i < lives; i++)
	{
		// Lives left
		sceneManager2D.Render2DMesh(meshList[GEO_HEART], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0 + (i *m_cMap->GetTileSize()), sceneManager2D.m_window_height - (2 * m_cMap->GetTileSize()));
	}

	switch (currentState)
	{
	case COMPLETED:
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "TUTORIAL COMPLETED!", Color(0, 0, 0), m_cMap->GetTileSize(), sceneManager2D.m_window_width * 0.15, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 4);
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "<Click to exit>", Color(0, 0, 0), m_cMap->GetTileSize(), sceneManager2D.m_window_width * 0.15, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 5);
		break;

	case FAILED:
	{
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "GAME OVER..", Color(0, 0, 0), m_cMap->GetTileSize(), sceneManager2D.m_window_width * 0.15, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 4);
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "<Click to exit>", Color(0, 0, 0), m_cMap->GetTileSize(), sceneManager2D.m_window_width * 0.15, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 5);
	}
		break;
	}
}
/********************************************************************************
Render the AIs. This is a private function for use in this class only
********************************************************************************/
void CSceneTutorialGame3::RenderAIs()
{
	// Render the enemy
	for (int i = 0; i < theEnemies.size(); i++)
	{
		int theEnemy_x = theEnemies[i]->GetPos_x() - theHero->GetMapOffset_x();
		int theEnemy_y = theEnemies[i]->GetPos_y() + theHero->GetMapOffset_y();
		if (((theEnemy_x >= 0 - m_cMap->GetTileSize()) && (theEnemy_x < sceneManager2D.m_window_width + m_cMap->GetTileSize())) &&
			((theEnemy_y >= 0 - m_cMap->GetTileSize()) && (theEnemy_y < sceneManager2D.m_window_height + m_cMap->GetTileSize())))
		{
			sceneManager2D.Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theEnemy_x, theEnemy_y);
		}
	}
}
void CSceneTutorialGame3::RenderWaypoints()
{
	for (int i = 0; i < temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), temp.at(i).x - theHero->GetMapOffset_x(), temp.at(i).y + theHero->GetMapOffset_y());
	}
}
