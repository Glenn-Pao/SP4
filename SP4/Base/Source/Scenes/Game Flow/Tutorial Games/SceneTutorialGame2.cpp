#include "SceneTutorialGame2.h"
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

CSceneTutorialGame2::CSceneTutorialGame2(const int m_window_width, const int m_window_height)
{
}

CSceneTutorialGame2::~CSceneTutorialGame2()
{
}

void CSceneTutorialGame2::Init(int level)
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
	m_cMap->LoadMap("Image//Maps//Game 2/Tutorial.csv");

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
				theHero->setPositionX(k*m_cMap->GetTileSize());
				theHero->setPositionY((m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());				
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
	//GreyDoors.push_back(new CDoor(0, Vector3(k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize() - m_cMap->GetTileSize(), 0), Vector3(50, 50, 0), meshList[GEO_TILE_DOOR]));
	//GreyDoors.push_back(new CDoor(0, Vector3(sceneManager2D.m_window_width / 1.5, sceneManager2D.m_window_height / 1.5, 0), Vector3(50, 50, 50), meshList[GEO_TILE_DOOR]));

	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			int TSize_x = k*m_cMap->GetTileSize();
			int TSize_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();
			if (m_cMap->theScreenMap[i][k] == 33)
			{
				GreyDoors.push_back(new CDoor(1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]));
				GreyDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 37)
			{
				ColoursSet.push_back(new CColour("BLUE", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_BLUE]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 35)
			{
				greenDoor = new CDoor(1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_GREEN_DOOR]);
				greenDoor->setActive(true);
				greenDoor->setDoorType("COLOURED", "GREEN");
			}
			else if (m_cMap->theScreenMap[i][k] == 34)
			{
				BlueDoors.push_back(new CDoor(1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_BLUE_DOOR]));
				BlueDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 38)
			{
				ColoursSet.push_back(new CColour("YELLOW", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_YELLOW]));
				ColoursSet.back()->setActive(true);
			}
		}
	}

	hasBlue = hasYellow = false;
	castedBlue = castedYellow = castedGreen = false;

	prevHeroPos.SetZero();
}

void CSceneTutorialGame2::PreInit()
{
	sceneManager2D.PreInit();
}

/********************************************************************************
Initialise the meshes.
********************************************************************************/
void CSceneTutorialGame2::InitMeshes()
{
	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GEO_TILEHERO] = MeshBuilder::Generate2DMesh("GEO_TILEHERO", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILEHERO]->textureID = LoadTGA("Image//tile2_hero.tga");
	meshList[GEO_TILESTRUCTURE] = MeshBuilder::Generate2DMesh("GEO_TILESTRUCTURE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILESTRUCTURE]->textureID = LoadTGA("Image//tile3_structure.tga");
	meshList[GEO_TILE_DOOR] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_DOOR]->textureID = LoadTGA("Image//tile30_hubdoor.tga");
	meshList[GEO_TILE_WALL] = MeshBuilder::Generate2DMesh("GEO_TILE_WALL", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_WALL]->textureID = LoadTGA("Image//Tile/wall.tga");
	meshList[GEO_TILE_GROUND] = MeshBuilder::Generate2DMesh("GEO_TILE_GROUND", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_GROUND]->textureID = LoadTGA("Image//Tile/ground.tga");
	/*GEO_COLOUR_BALL_BLUE,
		GEO_COLOUR_BALL_YELLOW,
		GEO_COLOUR_BALL_RED,
		GEO_COLOUR_BALL_ORANGE,*/
	meshList[GEO_COLOUR_BALL_BLUE] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_BLUE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_BLUE]->textureID = LoadTGA("Image//Tile/tile37_BlueBall.tga");
	meshList[GEO_COLOUR_BALL_YELLOW] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_YELLOW", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_YELLOW]->textureID = LoadTGA("Image//Tile/tile38_YellowBall.tga");
	meshList[GEO_GREEN_DOOR] = MeshBuilder::Generate2DMesh("GEO_GREEN_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_GREEN_DOOR]->textureID = LoadTGA("Image//Tile/tile35_GreenDoor.tga");
	meshList[GEO_BLUE_DOOR] = MeshBuilder::Generate2DMesh("GEO_BLUE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_BLUE_DOOR]->textureID = LoadTGA("Image//Tile/tile34_BlueDoor.tga");

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

void CSceneTutorialGame2::Update(double dt)
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

	prevHeroPos = Vector3(theHero->getPositionX(), theHero->getPositionY());
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
	if (Application::IsKeyPressed('Q') && hasBlue)
		castedBlue = true;

	if (castedBlue)
	{
		if (Application::IsKeyPressed('E') && hasYellow)
			castedYellow = true;
	}
	if (castedBlue && castedYellow)
	{
		if (Application::IsKeyPressed(' '))
			castedGreen = true;
	}

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

	//Doors
	for (int i = 0; i < GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			GreyDoors[i]->setActive(false);
		}
	}

	if (greenDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (castedGreen)
			greenDoor->setActive(false);
		else
			theHero->setPosition(prevHeroPos);

	}

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{

			if (castedBlue)
				BlueDoors[i]->setActive(false);
			else
				theHero->setPosition(prevHeroPos);
		}
	}

	//Loop that settles colours
	for (int i = 0; i < ColoursSet.size(); i++)
	{
		if (ColoursSet[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			if (ColoursSet[i]->getColour() == "BLUE")
				hasBlue = true;
			if (ColoursSet[i]->getColour() == "YELLOW")
				hasYellow = true;

			ColoursThePlayerHas.push_back(ColoursSet[i]->getColour());
			ColoursSet[i]->setActive(false);
		}
	}
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneTutorialGame2::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneTutorialGame2::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}


/********************************************************************************
Render this scene
********************************************************************************/
void CSceneTutorialGame2::Render()
{
	sceneManager2D.Render();

	sceneManager2D.modelStack.PushMatrix();

	//sceneManager2D.RenderBackground();

	sceneManager2D.modelStack.Translate(-theHero->GetMapOffset_x(), theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);
	
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
	RenderObjects();

	sceneManager2D.modelStack.PopMatrix();

	//On screen text
	std::ostringstream ss;
	//ss.precision(5);
	////ss << "theEnemy: " << theEnemy->GetPos_x() << ", " << theEnemy->GetPos_y();
	//ss << "theEnemiesLeft: " << theEnemies.size();
	//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
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

	/*if (ColoursThePlayerHas.size() > 0){
		ss.str(std::string());
		ss.precision(3);
		ss << "Colours: " << ColoursThePlayerHas.back();
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
	}*/

	std::string castedColour = "";
	ss.str(std::string());
	if (castedBlue && !castedYellow)
		castedColour += "Blue";
	/*else if (castedYellow )
		castedColour += "Yellow";*/

	else if (castedGreen)
		castedColour += "Green";
	else if (castedBlue && castedYellow)
		castedColour += "Blue and Yellow";
	
	ss << "Casted colour: " << castedColour;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);

}

/********************************************************************************
Exit this scene
********************************************************************************/
void CSceneTutorialGame2::Exit()
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
void CSceneTutorialGame2::RenderTileMap()
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
void CSceneTutorialGame2::RenderHero()
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
void CSceneTutorialGame2::RenderAIs()
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
void CSceneTutorialGame2::RenderObjects()
{
	for (int i = 0; i < GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getActive())
			sceneManager2D.Render2DMesh(GreyDoors[i]->getMesh(), false, GreyDoors[i]->getScale().x, GreyDoors[i]->getScale().y, GreyDoors[i]->getPositionX(), GreyDoors[i]->getPositionY());
	}
	for (int i = 0; i < ColoursSet.size(); i++)
	{
		if (ColoursSet[i]->getActive())
			sceneManager2D.Render2DMesh(ColoursSet[i]->getMesh(), false, ColoursSet[i]->getScale().x, ColoursSet[i]->getScale().y, ColoursSet[i]->getPositionX(), ColoursSet[i]->getPositionY());
	}

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive())
			sceneManager2D.Render2DMesh(BlueDoors[i]->getMesh(), false, BlueDoors[i]->getScale().x, BlueDoors[i]->getScale().y, BlueDoors[i]->getPositionX(), BlueDoors[i]->getPositionY());
	}

	if (greenDoor->getActive())
		sceneManager2D.Render2DMesh(greenDoor->getMesh(), false, greenDoor->getScale().x, greenDoor->getScale().y, greenDoor->getPositionX(), greenDoor->getPositionY());
}


/********************************************************************************
Render the rear tile map. This is a private function for use in this class only
********************************************************************************/
void CSceneTutorialGame2::RenderRearTileMap()
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
void CSceneTutorialGame2::RenderGoodies()
{
	// Render the goodies
	for (int i = 0; i<10; i++)
	{
		sceneManager2D.Render2DMesh(theArrayOfGoodies[i]->GetMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theArrayOfGoodies[i]->GetPos_x(), theArrayOfGoodies[i]->GetPos_y());
	}
}

void CSceneTutorialGame2::RenderWaypoints()
{
	for (int i = 0; i < temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), temp.at(i).x - theHero->GetMapOffset_x(), temp.at(i).y + theHero->GetMapOffset_y());
	}
}

