#include "SceneGame2.h"
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

CSceneGame2::CSceneGame2(const int m_window_width, const int m_window_height)
	: m_cMinimap(NULL)
	, m_cMap(NULL)
	, tileOffset_x(0)
	, tileOffset_y(0)
	, m_cRearMap(NULL)
	, rearWallOffset_x(0)
	, rearWallOffset_y(0)
	, rearWallTileOffset_x(0)
	, rearWallTileOffset_y(0)
	, rearWallFineOffset_x(0)
	, rearWallFineOffset_y(0)
	, JellybeanSystem(NULL)
	, theHero(NULL)
	, waypoints(NULL)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

CSceneGame2::~CSceneGame2()
{
	//for (int i = 0; i<10; i++)
	//{
	//	//delete theArrayOfGoodies[i];
	//}
	//delete theArrayOfGoodies;
	for (int i = 0; i < theEnemies.size(); i++)
	{
		if (theEnemies[i])
		{
			delete theEnemies[i];
			theEnemies[i] = NULL;
		}
	}

	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}

	if (m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}
	if (waypoints)
	{
		delete waypoints;
		waypoints = NULL;
	}
}

void CSceneGame2::Init(int level)
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
	m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 18 * tileSize, 25 * tileSize, tileSize);
	m_cMap->LoadMap("Image//Maps/Game 2/Easy.csv");

	//initialise the waypoints
	waypoints = new CWaypoints();
	waypoints->LoadWaypoints(m_cMap);
	temp = waypoints->getWaypointsVector();

	// Initialise and load the REAR tile map
	/*m_cRearMap = new CMap();
	m_cRearMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, sceneManager2D.m_window_height / tileSize, sceneManager2D.m_window_width / tileSize, 24 * tileSize, 64 * tileSize, tileSize);
	m_cRearMap->LoadMap("Image//MapDesign_Rear.csv");*/

	// Load the texture for minimap
	m_cMinimap = new CMinimap();
	m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("MINIMAP", Color(1, 1, 1), 1.f));
	m_cMinimap->GetBackground()->textureID = LoadTGA("Image//grass_darkgreen.tga");
	m_cMinimap->SetBorder(MeshBuilder::GenerateMinimapBorder("MINIMAPBORDER", Color(1, 1, 0), 1.f));
	m_cMinimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("MINIMAPAVATAR", Color(1, 1, 0), 1.f));

	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			// Hero
			if (m_cMap->theScreenMap[i][k] == 1000 && theHero == NULL)
			{
				// Initialise the hero's position
				theHero = new CPlayerInfo();

				int centerBorderX = (m_cMap->GetNumOfTiles_Width() * 0.5) * m_cMap->GetTileSize();
				theHero->setPositionX(k*m_cMap->GetTileSize());
				if (theHero->getPositionX() < centerBorderX)
				{
					theHero->SetMapOffset_x(theHero->getPositionX() - centerBorderX);
					if (theHero->GetMapOffset_x() < 0)
					{
						theHero->SetMapOffset_x(0);
					}
					else
					{
						theHero->setPositionX(centerBorderX);
					}
				}
				else if (theHero->getPositionX() > centerBorderX)
				{
					theHero->SetMapOffset_x(theHero->getPositionX() - centerBorderX);
					float maxMapOffset_x = (m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width()) * m_cMap->GetTileSize();
					if (theHero->GetMapOffset_x() > maxMapOffset_x)
					{
						theHero->setPositionX(centerBorderX + theHero->GetMapOffset_x() - maxMapOffset_x);
						theHero->SetMapOffset_x(maxMapOffset_x);
					}
					else
					{
						theHero->setPositionX(centerBorderX);
					}
				}

				int centerBorderY = (m_cMap->GetNumOfTiles_Height() * 0.5) * m_cMap->GetTileSize();
				theHero->setPositionY((m_cMap->GetNumOfTiles_Height() - (i + 1))*m_cMap->GetTileSize());
				if (theHero->getPositionY() < centerBorderY)
				{
					theHero->SetMapOffset_y(centerBorderY - theHero->getPositionY());
					float maxMapOffset_y = (m_cMap->getNumOfTiles_MapHeight() - m_cMap->GetNumOfTiles_Height()) * m_cMap->GetTileSize();
					if (theHero->GetMapOffset_y() > maxMapOffset_y)
					{
						theHero->setPositionY(centerBorderY - (theHero->GetMapOffset_y() - maxMapOffset_y));
						theHero->SetMapOffset_y(maxMapOffset_y);
					}
					else
					{
						theHero->setPositionY(centerBorderY);
					}
				}
				else if (theHero->getPositionY() > centerBorderY)
				{
					theHero->SetMapOffset_y(centerBorderY - theHero->getPositionY());
					if (theHero->GetMapOffset_y() < m_cMap->GetTileSize())
					{
						theHero->setPositionY(centerBorderY - (theHero->GetMapOffset_y() - m_cMap->GetTileSize()));
						theHero->SetMapOffset_y(m_cMap->GetTileSize());
					}
					else
					{
						theHero->setPositionY(centerBorderY);
					}
				}
			}
			// Enemies
			else if (m_cMap->theScreenMap[i][k] == 100)
			{
				// Set the strategy for the enemy
				theEnemies.push_back(new CEnemy());
				theEnemies.back()->ChangeStrategy(NULL, false);
				theEnemies.back()->SetPos_x(k*m_cMap->GetTileSize());
				theEnemies.back()->SetPos_y(sceneManager2D.m_window_height - i*m_cMap->GetTileSize() - m_cMap->GetTileSize());
			}
		}
	}
	// Jellybeans
	JellybeanSystem = new CJellybeanSystem;

	// Initialise the Meshes
	InitMeshes();

	/*theArrayOfGoodies = new CGoodies*[10];
	for (int i = 0; i<10; i++)
	{
	theArrayOfGoodies[i] = theGoodiesFactory.Create(TREASURECHEST);
	theArrayOfGoodies[i]->SetPos(150 + i * 25, 150);
	theArrayOfGoodies[i]->SetMesh(MeshBuilder::Generate2DMesh("GEO_TILE_TREASURECHEST", Color(1, 1, 1), 0, 0, 1, 1));
	theArrayOfGoodies[i]->SetTextureID(LoadTGA("Image//tile4_treasurechest.tga"));
	}*/
	ShowNormalDoor = true;
	ShowBlue = true;
	ShowGreenDoor = true;
}

void CSceneGame2::PreInit()
{
	sceneManager2D.PreInit();
}

/********************************************************************************
Initialise the meshes. This is a private function for use in this class only
********************************************************************************/
void CSceneGame2::InitMeshes()
{
	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GEO_TILEGROUND] = MeshBuilder::Generate2DMesh("GEO_TILEGROUND", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILEGROUND]->textureID = LoadTGA("Image//tile1_ground.tga");
	meshList[GEO_TILEHERO] = MeshBuilder::Generate2DMesh("GEO_TILEHERO", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILEHERO]->textureID = LoadTGA("Image//tile2_hero.tga");
	meshList[GEO_TILETREE] = MeshBuilder::Generate2DMesh("GEO_TILETREE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILETREE]->textureID = LoadTGA("Image//tile3_tree.tga");
	meshList[GEO_TILESTRUCTURE] = MeshBuilder::Generate2DMesh("GEO_TILESTRUCTURE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILESTRUCTURE]->textureID = LoadTGA("Image//tile3_structure.tga");
	meshList[GEO_TILE_DOOR] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_DOOR]->textureID = LoadTGA("Image//tile30_hubdoor.tga");

	//Doors
	meshList[GEO_TILE_NORMDOOR] = MeshBuilder::Generate2DMesh("GEO_TILE_NORMDOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_NORMDOOR]->textureID = LoadTGA("Image//tile33_NormalDoor.tga");
	meshList[GEO_TILE_GREENDOOR] = MeshBuilder::Generate2DMesh("GEO_TILE_GREENDOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_GREENDOOR]->textureID = LoadTGA("Image//tile34_GreenDoor.tga");
	
	//Colours
	meshList[GEO_TILE_BLUEB] = MeshBuilder::Generate2DMesh("GEO_TILE_BLUEB", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_BLUEB]->textureID = LoadTGA("Image//tile37_BlueBall.tga");
	meshList[GEO_TILE_YELLOWB] = MeshBuilder::Generate2DMesh("GEO_TILE_YELLOWB", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_YELLOWB]->textureID = LoadTGA("Image//tile38_YellowBall.tga");

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

void CSceneGame2::Update(double dt)
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
	/*if (Application::IsKeyPressed(' '))
	this->theHero->SetToJumpUpwards(true);*/
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

	// ReCalculate the tile offsets
	tileOffset_x = (int)(theHero->GetMapOffset_x() / m_cMap->GetTileSize());
	if (tileOffset_x + m_cMap->GetNumOfTiles_Width() > m_cMap->getNumOfTiles_MapWidth())
		tileOffset_x = m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();
	tileOffset_y = (int)(theHero->GetMapOffset_y() / m_cMap->GetTileSize());
	if (tileOffset_y + m_cMap->GetNumOfTiles_Height() > m_cMap->getNumOfTiles_MapHeight())
		tileOffset_y = m_cMap->getNumOfTiles_MapHeight() - m_cMap->GetNumOfTiles_Height();

	// if the hero enters the kill zone, then enemy goes into kill strategy mode
	int typeCollided = theHero->CheckCollision(m_cMap);
	if (typeCollided == 10)
	{
		for (int i = 0; i < theEnemies.size(); i++)
		{
			theEnemies[i]->ChangeStrategy(new CStrategy_Kill());
		}
	}
	else if (typeCollided == 11)
	{
		for (int i = 0; i < theEnemies.size(); i++)
		{
			theEnemies[i]->ChangeStrategy(NULL);
		}
	}
	else if (typeCollided == 33)
	{
		if (ShowNormalDoor)
			ShowNormalDoor = false;
	}
	else if (typeCollided == 34)
	{
		if (ShowGreenDoor)
			ShowGreenDoor = false;
	}
	else if (typeCollided == 37)
	{
		if (ShowBlue)
			ShowBlue = false;
	}
	else if (typeCollided == 38)
	{
		if (ShowYellow)
			ShowYellow = false;
	}
	else
	{
		//theEnemy->ChangeStrategy(NULL);
	}

	// Update the enemies
	for (int i = 0; i < theEnemies.size(); i++)
	{
		int theDestination_x = theHero->getPositionX() + theHero->GetMapOffset_x();
		int theDestination_y = theHero->getPositionY() - theHero->GetMapOffset_y();
		theEnemies[i]->SetDestination(theDestination_x, theDestination_y);
		theEnemies[i]->Update(m_cMap);
	}
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneGame2::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneGame2::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}


/********************************************************************************
Render this scene
********************************************************************************/
void CSceneGame2::Render()
{
	sceneManager2D.Render();

	sceneManager2D.RenderBackground();

	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render Hero
	RenderHero();
	// Render AIs
	RenderAIs();
	//Render Waypoints
	//RenderWaypoints();
	// Render the goodies
	//RenderGoodies();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	//ss << "theEnemy: " << theEnemy->GetPos_x() << ", " << theEnemy->GetPos_y();
	ss << "theEnemiesLeft: " << theEnemies.size();
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
	ss.str(std::string());
	ss.precision(5);
	ss << "mapOffset_x: " << theHero->GetMapOffset_x();
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 30);
	// Jellybean
	sceneManager2D.Render2DMesh(JellybeanSystem->mesh, false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());
	ss.str(std::string());
	ss.precision(3);
	ss << ": " << JellybeanSystem->GetNumOfJellybeans();
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), m_cMap->GetTileSize(), m_cMap->GetTileSize(), sceneManager2D.m_window_height - m_cMap->GetTileSize());
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CSceneGame2::Exit()
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
void CSceneGame2::RenderTileMap()
{
	int m = 0;
	int j = 0;
	for (int i = 0; i < m_cMap->GetNumOfTiles_Height() + 1; i++)
	{
		for (int k = 0; k < m_cMap->GetNumOfTiles_Width() + 1; k++)
		{
			m = tileOffset_x + k;
			j = i + tileOffset_y;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if ((tileOffset_x + k) >= m_cMap->getNumOfTiles_MapWidth())
				break;
			// If we have reached the bottom side of the Map, then do not display the extra row of tiles.
			if ((i + tileOffset_y) >= m_cMap->getNumOfTiles_MapHeight())
				break;
			if (m_cMap->theScreenMap[j][m] == 1)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILEGROUND], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 2)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILETREE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 10)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 11)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_SAFEZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 30)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_DOOR], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 33)
			{
				if (ShowNormalDoor)
					sceneManager2D.Render2DMesh(meshList[GEO_TILE_NORMDOOR], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());				
			}
			else if (m_cMap->theScreenMap[j][m] == 34)
			{
				if (ShowGreenDoor)
					sceneManager2D.Render2DMesh(meshList[GEO_TILE_GREENDOOR], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 37)
			{
				if (ShowBlue)
					sceneManager2D.Render2DMesh(meshList[GEO_TILE_BLUEB], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 38)
			{
				if (ShowYellow)
					sceneManager2D.Render2DMesh(meshList[GEO_TILE_YELLOWB], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
		}
	}
}

/********************************************************************************
Render the Hero. This is a private function for use in this class only
********************************************************************************/
void CSceneGame2::RenderHero()
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

/********************************************************************************
Render the AIs. This is a private function for use in this class only
********************************************************************************/
void CSceneGame2::RenderAIs()
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

/********************************************************************************
Render the rear tile map. This is a private function for use in this class only
********************************************************************************/
void CSceneGame2::RenderRearTileMap()
{
	if (m_cRearMap)
	{
		rearWallOffset_x = (int)(theHero->GetMapOffset_x() / 2);
		rearWallOffset_y = 0;
		rearWallTileOffset_y = 0;
		rearWallTileOffset_x = (int)(rearWallOffset_x / m_cRearMap->GetTileSize());
		if (rearWallTileOffset_x + m_cRearMap->GetNumOfTiles_Width() > m_cRearMap->getNumOfTiles_MapWidth())
			rearWallTileOffset_x = m_cRearMap->getNumOfTiles_MapWidth() - m_cRearMap->GetNumOfTiles_Width();
		rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();

		int m = 0;
		for (int i = 0; i < m_cRearMap->GetNumOfTiles_Height(); i++)
		{
			for (int k = 0; k < m_cRearMap->GetNumOfTiles_Width() + 1; k++)
			{
				m = rearWallTileOffset_x + k;
				// If we have reached the right side of the Map, then do not display the extra column of tiles.
				if ((rearWallTileOffset_x + k) >= m_cRearMap->getNumOfTiles_MapWidth())
					break;
				if (m_cRearMap->theScreenMap[i][m] == 3)
				{
					sceneManager2D.Render2DMesh(meshList[GEO_TILESTRUCTURE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cRearMap->GetTileSize() - rearWallFineOffset_x, sceneManager2D.m_window_height - (i + 1)*m_cRearMap->GetTileSize());
				}
			}
		}
	}
}

/********************************************************************************
Render the goodies. This is a private function for use in this class only
********************************************************************************/
void CSceneGame2::RenderGoodies()
{
	// Render the goodies
	for (int i = 0; i<10; i++)
	{
		sceneManager2D.Render2DMesh(theArrayOfGoodies[i]->GetMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theArrayOfGoodies[i]->GetPos_x(), theArrayOfGoodies[i]->GetPos_y());
	}
}

void CSceneGame2::RenderWaypoints()
{
	for (int i = 0; i < temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), temp.at(i).x - theHero->GetMapOffset_x(), temp.at(i).y + theHero->GetMapOffset_y());
	}
}
// Read and store data
void CSceneGame2::ReadData(CGameInfo* Data)
{
	if (Data->ifNew == false)
	{
		theHero->setPositionX(Data->heroPosition.x);
		theHero->setPositionY(Data->heroPosition.y);
		theHero->SetAnimationDirection(Data->heroAnimationDir);

		JellybeanSystem->SetNumOfJellybeans(Data->noOfJellybeans);
	}
}
void CSceneGame2::StoreData(CGameInfo* Data)
{
	Data->heroPosition.x = theHero->getPositionX();
	Data->heroPosition.y = theHero->getPositionY();
	Data->heroAnimationDir = theHero->GetAnimationDirection();

	Data->noOfJellybeans = JellybeanSystem->GetNumOfJellybeans();
}
// Find and Set the actual offset of hero
void CSceneGame2::SetHeroOffset()
{
	// X
	int centerBorderX = (m_cMap->GetNumOfTiles_Width() * 0.5) * m_cMap->GetTileSize();
	if (theHero->getPositionX() < centerBorderX)
	{
		theHero->SetMapOffset_x(theHero->getPositionX() - centerBorderX);
		if (theHero->GetMapOffset_x() < 0)
		{
			theHero->SetMapOffset_x(0);
		}
		else
		{
			theHero->setPositionX(centerBorderX);
		}
	}
	else if (theHero->getPositionX() > centerBorderX)
	{
		theHero->SetMapOffset_x(theHero->getPositionX() - centerBorderX);
		float maxMapOffset_x = (m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width()) * m_cMap->GetTileSize();
		if (theHero->GetMapOffset_x() > maxMapOffset_x)
		{
			theHero->setPositionX(centerBorderX + theHero->GetMapOffset_x() - maxMapOffset_x);
			theHero->SetMapOffset_x(maxMapOffset_x);
		}
		else
		{
			theHero->setPositionX(centerBorderX);
		}
	}
	// Y
	int centerBorderY = (m_cMap->GetNumOfTiles_Height() * 0.5) * m_cMap->GetTileSize() - m_cMap->GetTileSize();
	if (theHero->getPositionY() < centerBorderY)
	{
		theHero->SetMapOffset_y(centerBorderY - theHero->getPositionY());
		float maxMapOffset_y = (m_cMap->getNumOfTiles_MapHeight() - m_cMap->GetNumOfTiles_Height()) * m_cMap->GetTileSize();
		if (theHero->GetMapOffset_y() > maxMapOffset_y)
		{
			theHero->setPositionY(centerBorderY - (theHero->GetMapOffset_y() - maxMapOffset_y + m_cMap->GetTileSize()));
			theHero->SetMapOffset_y(maxMapOffset_y);
		}
		else
		{
			theHero->setPositionY(centerBorderY);
		}
	}
	else if (theHero->getPositionY() > centerBorderY)
	{
		theHero->SetMapOffset_y(centerBorderY - theHero->getPositionY());
		if (theHero->GetMapOffset_y() < m_cMap->GetTileSize())
		{
			theHero->setPositionY(centerBorderY - (theHero->GetMapOffset_y()));
			theHero->SetMapOffset_y(m_cMap->GetTileSize());
		}
		else
		{
			theHero->setPositionY(centerBorderY);
		}
	}
}