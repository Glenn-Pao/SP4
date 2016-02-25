#include "SceneGame2.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"


CSceneGame2::CSceneGame2(const int m_window_width, const int m_window_height)
	: currentState(PLAYING)
{
}

CSceneGame2::~CSceneGame2()
{
	// Dialogues tiles
	for (int i = 0; i < dialogueTiles.size(); i++)
	{
		if (dialogueTiles[i])
		{
			delete dialogueTiles[i];
			dialogueTiles[i] = NULL;
		}
	}
}

void CSceneGame2::Init(int level)
{
	level = 1;
	this->level = level;
	// Init the base scene
	sceneManager2D.Init(level);

	L = new UseLuaFiles();

	L->ReadFiles("Lua//Scene/Game2/tutorial.lua");

	tileSize = L->DoLuaInt("tileSize");

	scriptFinished = L->DoLuaString("scriptFinished");
	scriptExit = L->DoLuaString("scriptExit");

	// Dialogues scripts
	for (int i = 0; i < 6; i++)
	{
		scriptDialogues.push_back(L->DoLuaString("script" + to_string(i)));
	}

	switch (level)
	{
	case 0:
	{
		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 18 * tileSize, 25 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 2/Tutorial.csv");
	}
		break;
	case 1:
	{
		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 18 * tileSize, 25 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 2/Easy.csv");
	}
		break;
	case 2:
	{
		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 18 * tileSize, 25 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 2/Medium.csv");
	}
		break;
	case 3:
	{
		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 18 * tileSize, 25 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 2/Hard.csv");
	}
		break;
	}

	//initialise the waypoints
	waypoints = new CWaypoints();
	waypoints->LoadWaypoints(m_cMap);
	temp = waypoints->getWaypointsVector();

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
			float pos_x = k*m_cMap->GetTileSize();
			float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

			if (m_cMap->theScreenMap[i][k] == 1000 && theHero == NULL)
			{
				// Initialise the hero's position

				theHero = new CPlayerInfo(m_cMap);
				theHero->setPositionX(pos_x);
				theHero->setPositionY(pos_y);

				// Control Dialogue
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[0], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
				Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
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

	// Initialise the Meshes
	InitMeshes();

	//GreyDoors.push_back(new CDoor(0, Vector3(k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize() - m_cMap->GetTileSize(), 0), Vector3(50, 50, 0), meshList[GEO_TILE_DOOR]));
	//GreyDoors.push_back(new CDoor(0, Vector3(sceneManager2D.m_window_width / 1.5, sceneManager2D.m_window_height / 1.5, 0), Vector3(50, 50, 50), meshList[GEO_TILE_DOOR]));

	switch (level)
	{
	case 0:
	{
		InitTutorial();
	}
		break;
	case 1:
	{
		InitLevel1();
	}
		break;
	case 2:
	{
		InitLevel2();
	}
		break;
	case 3:
	{
		InitLevel3();
	}
		break;
	}

	hasBlue = hasYellow = hasRed = hasPurple = false;
	castedBlue = castedYellow = castedGreen = castedRed = castedOrange = castedPurple = false;

	prevHeroPos.SetZero();
	castedColoursCounter = 0;
	timer = 0;
}

void CSceneGame2::PreInit()
{
	sceneManager2D.PreInit();
}

void CSceneGame2::InitTutorial()
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			int TSize_x = k*m_cMap->GetTileSize();
			int TSize_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();
			if (m_cMap->theScreenMap[i][k] == 33)
			{
				GreyDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]));
				GreyDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 30)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[2], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5), TSize_y + (tileSize * 0.5), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5), TSize_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 31)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[1], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5), TSize_y + (tileSize * 0.5), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5), TSize_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 32)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[3], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5), TSize_y + (tileSize * 0.5), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5), TSize_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 29)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[4], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5), TSize_y + (tileSize * 0.5), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5), TSize_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 28)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[5], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5), TSize_y + (tileSize * 0.5), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5), TSize_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 27)
			{
				endDoor = new CDoor(CObjects::DOOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);

			}
			else if (m_cMap->theScreenMap[i][k] == 37)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "BLUE", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_BLUE]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 35)
			{
				greenDoor = new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_GREEN_DOOR]);
				greenDoor->setActive(true);
				greenDoor->setDoorType("COLOURED", "GREEN");
			}
			else if (m_cMap->theScreenMap[i][k] == 34)
			{
				BlueDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_BLUE_DOOR]));
				BlueDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 38)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "YELLOW", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_YELLOW]));
				ColoursSet.back()->setActive(true);
			}
		}
	}
}

void CSceneGame2::InitLevel1()
{
	InitTutorial();
}

void CSceneGame2::InitLevel2()
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			int TSize_x = k*m_cMap->GetTileSize();
			int TSize_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

			if (m_cMap->theScreenMap[i][k] == 33)
			{
				GreyDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]));
				GreyDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 27)
			{
				endDoor = new CDoor(CObjects::DOOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);

			}
			else if (m_cMap->theScreenMap[i][k] == 34)
			{
				BlueDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_BLUE_DOOR]));
				BlueDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 26)
			{
				OrangeDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_ORANGE_DOOR]));
				OrangeDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 36)
			{
				YellowDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_YELLOW_DOOR]));
				YellowDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 35)
			{
				greenDoor = new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_GREEN_DOOR]);
				greenDoor->setActive(true);
				greenDoor->setDoorType("COLOURED", "GREEN");
			}
			else if (m_cMap->theScreenMap[i][k] == 37)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "BLUE", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_BLUE]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 38)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "YELLOW", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_YELLOW]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 39)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "RED", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_RED]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 40)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "ORANGE", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_ORANGE]));
				ColoursSet.back()->setActive(true);
			}
		}
	}
}

void CSceneGame2::InitLevel3()
{
	// Dialogues scripts
	for (int i = 0; i < 6; i++)
	{
		scriptDialogues.push_back(L->DoLuaString("script" + to_string(i)));
	}

	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			int TSize_x = k*m_cMap->GetTileSize();
			int TSize_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

			if (m_cMap->theScreenMap[i][k] == 33)
			{
				GreyDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]));
				GreyDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 27)
			{
				endDoor = new CDoor(CObjects::DOOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);

			}
			else if (m_cMap->theScreenMap[i][k] == 34)
			{
				BlueDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_BLUE_DOOR]));
				BlueDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 26)
			{
				OrangeDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_ORANGE_DOOR]));
				OrangeDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 36)
			{
				YellowDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_YELLOW_DOOR]));
				YellowDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 25)
			{
				PurpleDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_PURPLE_DOOR]));
				PurpleDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 35)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "GREEN", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_GREEN_DOOR]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 24)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "PURPLE", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_PURPLE]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 37)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "BLUE", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_BLUE]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 38)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "YELLOW", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_YELLOW]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 39)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "RED", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_RED]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 40)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "ORANGE", Vector3(TSize_x, TSize_y), Vector3(tileSize, tileSize, 1), meshList[GEO_COLOUR_BALL_ORANGE]));
				ColoursSet.back()->setActive(true);
			}
		}
	}
}

/********************************************************************************
Initialise the meshes.
********************************************************************************/
void CSceneGame2::InitMeshes()
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
	
	meshList[GEO_COLOUR_BALL_BLUE] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_BLUE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_BLUE]->textureID = LoadTGA("Image//Tile/tile37_BlueBall.tga");
	meshList[GEO_COLOUR_BALL_YELLOW] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_YELLOW", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_YELLOW]->textureID = LoadTGA("Image//Tile/tile38_YellowBall.tga");
	meshList[GEO_COLOUR_BALL_GREEN] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_GREEN", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_GREEN]->textureID = LoadTGA("Image//Tile/green ball.tga");
	meshList[GEO_COLOUR_BALL_RED] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_RED", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_RED]->textureID = LoadTGA("Image//Tile/tile39_RedBall.tga");
	meshList[GEO_COLOUR_BALL_ORANGE] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_ORANGE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_ORANGE]->textureID = LoadTGA("Image//Tile/tile40_OrangeBall.tga");
	meshList[GEO_COLOUR_BALL_PURPLE] = MeshBuilder::Generate2DMesh("GEO_COLOUR_BALL_PURPLE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_COLOUR_BALL_PURPLE]->textureID = LoadTGA("Image//Tile/tile24_PurpleBall.tga");
	

	meshList[GEO_GREEN_DOOR] = MeshBuilder::Generate2DMesh("GEO_GREEN_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_GREEN_DOOR]->textureID = LoadTGA("Image//Tile/tile35_GreenDoor.tga");
	meshList[GEO_BLUE_DOOR] = MeshBuilder::Generate2DMesh("GEO_BLUE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_BLUE_DOOR]->textureID = LoadTGA("Image//Tile/tile34_BlueDoor.tga");
	meshList[GEO_YELLOW_DOOR] = MeshBuilder::Generate2DMesh("GEO_YELLOW_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_YELLOW_DOOR]->textureID = LoadTGA("Image//Tile/tile36_YellowDoor.tga");
	meshList[GEO_ORANGE_DOOR] = MeshBuilder::Generate2DMesh("GEO_ORANGE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_ORANGE_DOOR]->textureID = LoadTGA("Image//Tile/tile26_OrangeDoor.tga");
	meshList[GEO_PURPLE_DOOR] = MeshBuilder::Generate2DMesh("GEO_PURPLE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_PURPLE_DOOR]->textureID = LoadTGA("Image//Tile/tile25_PurpleDoor.tga");
	
	meshList[GEO_DIALOGUE_BOX] = MeshBuilder::Generate2DMesh("GEO_DIALOGUE_BOX", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_DIALOGUE_BOX]->textureID = LoadTGA("Image//dialogue_box.tga");

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
	meshList[GEO_JELLYBEAN] = MeshBuilder::Generate2DMesh("GEO_JELLYBEAN", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_JELLYBEAN]->textureID = LoadTGA("Image//jellybean.tga");

}

void CSceneGame2::Update(double dt)
{
	if (currentState == PLAYING)
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
		if (Application::IsKeyPressed('E') && hasYellow)
			castedYellow = true;
		if (Application::IsKeyPressed('R') && hasRed)
			castedRed = true;
		if (Application::IsKeyPressed('T') && hasPurple)
			castedPurple = true;

		if (castedBlue && castedYellow)
		{
			if (Application::IsKeyPressed(' '))
				castedGreen = true;
		}

		if (castedRed && castedYellow)
		{
			if (Application::IsKeyPressed(' '))
				castedOrange = true;
		}


		if (castedBlue || castedYellow || castedGreen || castedRed || castedOrange || castedPurple)
		{
			timer += 0.1f;
			if (timer > 5.f)
			{
				timer = 0;
				if (castedBlue)
					castedBlue = false;
				else if (castedYellow)
					castedYellow = false;
				else if (castedGreen)
					castedGreen = false;
				else if (castedRed)
					castedRed = false;
				else if (castedOrange)
					castedOrange = false;
				else if (castedPurple)
					castedPurple = false;
			}
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


		// Dialogues tiles
		for (int i = 0; i < dialogueTiles.size(); i++)
		{
			if (dialogueTiles[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				dialogueTiles[i]->setActive(true);
			}
			else
			{
				dialogueTiles[i]->setActive(false);
			}
		}

		switch (level)
		{
		case 0:
			UpdateTutorial(dt);
			break;
		case 1:
			UpdateLevel1(dt);
			break;
		case 2:
			UpdateLevel2(dt);
			break;
		case 3:
			UpdateLevel3(dt);
			break;
		}

	}
}

void CSceneGame2::UpdateTutorial(double dt)
{
	// Dialogues tiles
	for (int i = 0; i < dialogueTiles.size(); i++)
	{
		if (dialogueTiles[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			dialogueTiles[i]->setActive(true);
		}
		else
		{
			dialogueTiles[i]->setActive(false);
		}
	}

	//Doors
	for (int i = 0; i < GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getActive() == true)
		{
			if (GreyDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				GreyDoors[i]->setActive(false);
			}
		}
	}

	if (greenDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (greenDoor->getActive() == true)
		{
			if (castedGreen)
				greenDoor->setActive(false);
			else
				theHero->setPosition(prevHeroPos);
		}
	}

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive() == true)
		{
			if (BlueDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedBlue)
					BlueDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
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


	if (endDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		currentState = COMPLETED;
	}
}

void CSceneGame2::UpdateLevel1(double dt)
{
	//Doors
	for (int i = 0; i < GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getActive() == true)
		{
			if (GreyDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				GreyDoors[i]->setActive(false);
			}
		}
	}

	if (greenDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (greenDoor->getActive() == true)
		{
			if (castedGreen)
				greenDoor->setActive(false);
			else
				theHero->setPosition(prevHeroPos);
		}
	}

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive() == true)
		{
			if (BlueDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedBlue)
					BlueDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
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

	if (endDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		currentState = COMPLETED;
	}
}

void CSceneGame2::UpdateLevel2(double dt)
{
	//Doors
	for (int i = 0; i < GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getActive() == true)
		{
			if (GreyDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				GreyDoors[i]->setActive(false);
			}
		}
	}

	if (greenDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (greenDoor->getActive() == true)
		{
			if (castedGreen)
				greenDoor->setActive(false);
			else
				theHero->setPosition(prevHeroPos);
		}
	}

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive() == true)
		{
			if (BlueDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedBlue)
					BlueDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
		}
	}

	for (int i = 0; i < YellowDoors.size(); i++)
	{
		if (YellowDoors[i]->getActive() == true)
		{
			if (YellowDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedYellow)
					YellowDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
		}
	}

	for (int i = 0; i < OrangeDoors.size(); i++)
	{
		if (OrangeDoors[i]->getActive() == true)
		{
			if (OrangeDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedOrange)
					OrangeDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
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
			if (ColoursSet[i]->getColour() == "RED")
				hasRed = true;

			ColoursThePlayerHas.push_back(ColoursSet[i]->getColour());
			ColoursSet[i]->setActive(false);
		}
	}

	if (endDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		currentState = COMPLETED;
	}
}

void CSceneGame2::UpdateLevel3(double dt)
{
	//Doors
	for (int i = 0; i < GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getActive() == true)
		{
			if (GreyDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				GreyDoors[i]->setActive(false);
			}
		}
	}

	for (int i = 0; i < GreenDoors.size(); i++)
	{
		if (GreenDoors[i]->getActive() == true)
		{
			if (GreenDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedGreen)
					GreenDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
		}
	}

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive() == true)
		{
			if (BlueDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedBlue)
					BlueDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
		}
	}

	for (int i = 0; i < YellowDoors.size(); i++)
	{
		if (YellowDoors[i]->getActive() == true)
		{
			if (YellowDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedYellow)
					YellowDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
		}
	}

	for (int i = 0; i < OrangeDoors.size(); i++)
	{
		if (OrangeDoors[i]->getActive() == true)
		{
			if (OrangeDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedOrange)
					OrangeDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
		}
	}

	for (int i = 0; i < PurpleDoors.size(); i++)
	{
		if (PurpleDoors[i]->getActive() == true)
		{
			if (PurpleDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{

				if (castedPurple)
					PurpleDoors[i]->setActive(false);
				else
					theHero->setPosition(prevHeroPos);
			}
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
			if (ColoursSet[i]->getColour() == "RED")
				hasRed = true;
			if (ColoursSet[i]->getColour() == "PURPLE")
				hasPurple = true;

			ColoursThePlayerHas.push_back(ColoursSet[i]->getColour());
			ColoursSet[i]->setActive(false);
		}
	}

	if (endDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		currentState = COMPLETED;
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
	RenderGUI();
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
	sceneManager2D.Render2DMesh(meshList[GEO_JELLYBEAN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());
	ss.str(std::string());
	ss.precision(3);
	ss << ": " << noOfJellybeans;
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
void CSceneGame2::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}

/********************************************************************************
Render the tile map. This is a private function for use in this class only
********************************************************************************/
void CSceneGame2::RenderTileMap()
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			if (m_cMap->theScreenMap[i][k] == 1)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_WALL], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}
			/*else if (m_cMap->theScreenMap[i][k] == 30)
			{
			sceneManager2D.Render2DMesh(meshList[GEO_TILE_DOOR], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}*/
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
void CSceneGame2::RenderObjects()
{
	switch (level)
	{
	case 0:
		RenderTutorial();
		break;
	case 1:
		RenderLevel1();
		break;
	case 2:
		RenderLevel2();
		break;
	case 3:
		RenderLevel3();
		break;
	}
}

void CSceneGame2::RenderTutorial()
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


	if (castedBlue && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_BLUE], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX(), theHero->getPositionY() + m_cMap->GetTileSize());
	if (castedYellow && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_YELLOW], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + m_cMap->GetTileSize() - 10, theHero->getPositionY() + m_cMap->GetTileSize());


	if (castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_GREEN], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + 10, theHero->getPositionY() + m_cMap->GetTileSize());

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive())
			sceneManager2D.Render2DMesh(BlueDoors[i]->getMesh(), false, BlueDoors[i]->getScale().x, BlueDoors[i]->getScale().y, BlueDoors[i]->getPositionX(), BlueDoors[i]->getPositionY());
	}

	if (greenDoor->getActive())
		sceneManager2D.Render2DMesh(greenDoor->getMesh(), false, greenDoor->getScale().x, greenDoor->getScale().y, greenDoor->getPositionX(), greenDoor->getPositionY());

	sceneManager2D.Render2DMesh(endDoor->getMesh(), false, endDoor->getScale().x, endDoor->getScale().y, endDoor->getPositionX(), endDoor->getPositionY());
}

void CSceneGame2::RenderLevel1()
{
	RenderTutorial();
}

void CSceneGame2::RenderLevel2()
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


	if (castedBlue && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_BLUE], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX(), theHero->getPositionY() + m_cMap->GetTileSize());
	if (castedYellow && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_YELLOW], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + m_cMap->GetTileSize() - 10, theHero->getPositionY() + m_cMap->GetTileSize());
	if (castedRed && !castedOrange)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_RED], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX(), theHero->getPositionY() - m_cMap->GetTileSize() + 10);
	
	if (castedOrange)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_ORANGE], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + 10, theHero->getPositionY() - m_cMap->GetTileSize() + 10);
	if (castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_GREEN], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + 10, theHero->getPositionY() + m_cMap->GetTileSize());

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive())
			sceneManager2D.Render2DMesh(BlueDoors[i]->getMesh(), false, BlueDoors[i]->getScale().x, BlueDoors[i]->getScale().y, BlueDoors[i]->getPositionX(), BlueDoors[i]->getPositionY());
	}
	for (int i = 0; i < YellowDoors.size(); i++)
	{
		if (YellowDoors[i]->getActive())
			sceneManager2D.Render2DMesh(YellowDoors[i]->getMesh(), false, YellowDoors[i]->getScale().x, YellowDoors[i]->getScale().y, YellowDoors[i]->getPositionX(), YellowDoors[i]->getPositionY());
	}

	for (int i = 0; i < OrangeDoors.size(); i++)
	{
		if (OrangeDoors[i]->getActive())
			sceneManager2D.Render2DMesh(OrangeDoors[i]->getMesh(), false, OrangeDoors[i]->getScale().x, OrangeDoors[i]->getScale().y, OrangeDoors[i]->getPositionX(), OrangeDoors[i]->getPositionY());
	}

	if (greenDoor->getActive())
		sceneManager2D.Render2DMesh(greenDoor->getMesh(), false, greenDoor->getScale().x, greenDoor->getScale().y, greenDoor->getPositionX(), greenDoor->getPositionY());

	sceneManager2D.Render2DMesh(endDoor->getMesh(), false, endDoor->getScale().x, endDoor->getScale().y, endDoor->getPositionX(), endDoor->getPositionY());
}

void CSceneGame2::RenderLevel3()
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


	if (castedBlue && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_BLUE], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX(), theHero->getPositionY() + m_cMap->GetTileSize());
	if (castedYellow && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_YELLOW], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + m_cMap->GetTileSize() - 10, theHero->getPositionY() + m_cMap->GetTileSize());
	if (castedRed && !castedOrange)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_RED], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX(), theHero->getPositionY() - m_cMap->GetTileSize() + 10);

	if (castedOrange)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_ORANGE], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + 10, theHero->getPositionY() - m_cMap->GetTileSize() + 10);
	if (castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_GREEN], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + 10, theHero->getPositionY() + m_cMap->GetTileSize());
	if (castedPurple)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_PURPLE], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX() + m_cMap->GetTileSize() - 10, theHero->getPositionY() - m_cMap->GetTileSize() + 10);

	for (int i = 0; i < BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive())
			sceneManager2D.Render2DMesh(BlueDoors[i]->getMesh(), false, BlueDoors[i]->getScale().x, BlueDoors[i]->getScale().y, BlueDoors[i]->getPositionX(), BlueDoors[i]->getPositionY());
	}
	for (int i = 0; i < YellowDoors.size(); i++)
	{
		if (YellowDoors[i]->getActive())
			sceneManager2D.Render2DMesh(YellowDoors[i]->getMesh(), false, YellowDoors[i]->getScale().x, YellowDoors[i]->getScale().y, YellowDoors[i]->getPositionX(), YellowDoors[i]->getPositionY());
	}

	for (int i = 0; i < OrangeDoors.size(); i++)
	{
		if (OrangeDoors[i]->getActive())
			sceneManager2D.Render2DMesh(OrangeDoors[i]->getMesh(), false, OrangeDoors[i]->getScale().x, OrangeDoors[i]->getScale().y, OrangeDoors[i]->getPositionX(), OrangeDoors[i]->getPositionY());
	}

	for (int i = 0; i < PurpleDoors.size(); i++)
	{
		if (PurpleDoors[i]->getActive())
			sceneManager2D.Render2DMesh(PurpleDoors[i]->getMesh(), false, PurpleDoors[i]->getScale().x, PurpleDoors[i]->getScale().y, PurpleDoors[i]->getPositionX(), PurpleDoors[i]->getPositionY());
	}

	for (int i = 0; i < GreenDoors.size(); i++)
	{
		if (GreenDoors[i]->getActive())
			sceneManager2D.Render2DMesh(GreenDoors[i]->getMesh(), false, GreenDoors[i]->getScale().x, GreenDoors[i]->getScale().y, GreenDoors[i]->getPositionX(), GreenDoors[i]->getPositionY());
	}

	sceneManager2D.Render2DMesh(endDoor->getMesh(), false, endDoor->getScale().x, endDoor->getScale().y, endDoor->getPositionX(), endDoor->getPositionY());
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
	for (int i = 0; i < 10; i++)
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

void CSceneGame2::RenderGUI()
{
	switch (currentState)
	{
	case PLAYING:
	{
		for (int i = 0; i < dialogueTiles.size(); i++)
		{
			if (dialogueTiles[i]->getActive())
			{
				// Dialogue box
				sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, sceneManager2D.m_window_width, m_cMap->GetTileSize(), 0, 0);

				// Text
				int textSize = m_cMap->GetTileSize() * 0.5;
				sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], dialogueTiles[i]->getDialogue(), Color(0, 0, 0), textSize, 0, textSize * 0.5);
				break;
			}
		}
	}
		break;
	case COMPLETED:
	{
		int textSize = m_cMap->GetTileSize();
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptFinished, Color(0, 0, 0), textSize, sceneManager2D.m_window_width * 0.5 - textSize * (scriptFinished.size() * 0.31), sceneManager2D.m_window_height * 0.5 + textSize);

		textSize = m_cMap->GetTileSize() * 0.5;
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptExit, Color(0, 0, 0), textSize, sceneManager2D.m_window_width * 0.5 - textSize * (scriptExit.size() * 0.31), sceneManager2D.m_window_height * 0.5 - textSize);
	}
		break;

	}
}