#include "SceneGame1.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

#include "..\..\..\UsingLua.h"

#include "..\..\..\AI\ProbabilitySystem.h"

CSceneGame1::CSceneGame1(const int m_window_width, const int m_window_height)
	: theDoor(NULL)
	, currentState(PLAYING)
	, timer(30.0f)
	, level(0)
	, UIManager(NULL)
{
}

CSceneGame1::~CSceneGame1()
{
	// Door
	if (theDoor)
	{
		delete theDoor;
	}
	// Dialogues tiles
	for (int i = 0; i < dialogueTiles.size(); i++)
	{
		if (dialogueTiles[i])
		{
			delete dialogueTiles[i];
			dialogueTiles[i] = NULL;
		}
	}

	if (UIManager)
	{
		delete UIManager;
		UIManager = NULL;
	}
}

void CSceneGame1::Init(int level) // level = 0(Tutorial), = 1(Easy), = 2(Medium), = 3(Hard)
{
	this->level = level;
	// Init the base scene
	sceneManager2D.Init(level);

	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//Scene/Game1/game1.lua");

	int tileSize = L.DoLuaInt("tileSize");
	scriptTimeUp = L.DoLuaString("scriptTimeUp");
	scriptFinished = L.DoLuaString("scriptFinished");
	scriptExit = L.DoLuaString("scriptExit");

	// Dialogues scripts
	vector<string> scriptDialogues;

	switch (level)
	{
		case 0:
		{
			currentState = PREPARING;
			timer = 30.f;

			for (int i = 0; i < 7; i++)
			{
				scriptDialogues.push_back(L.DoLuaString("script" + to_string(i)));
			}

			// Initialise and load the tile map
			m_cMap = new CMap();
			m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 21 * tileSize, 20 * tileSize, tileSize);
			m_cMap->LoadMap("Image//Maps//Game 1/Tutorial.csv");
		}
		break;
		case 1:
		{
			timer = 30.f;
			// Initialise and load the tile map
			m_cMap = new CMap();
			m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 31 * tileSize, 30 * tileSize, tileSize);
			m_cMap->LoadMap("Image//Maps//Game 1/Easy.csv");
		}
		break;
		case 2:
		{
			timer = 50.f;
			// Initialise and load the tile map
			m_cMap = new CMap();
			m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 41 * tileSize, 40 * tileSize, tileSize);
			m_cMap->LoadMap("Image//Maps//Game 1/Medium.csv");
		}
		break;
		case 3:
		{
			timer = 45.f;
			// Initialise and load the tile map
			m_cMap = new CMap();
			m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 51 * tileSize, 50 * tileSize, tileSize);
			m_cMap->LoadMap("Image//Maps//Game 1/Hard.csv");
		}
		break;
	}

	//initialise the waypoints
	waypoints = new CWaypoints();
	waypoints->LoadWaypoints(m_cMap);
	temp = waypoints->getWaypointsVector();

	// Start positions
	vector<Vector3> startPositions;
	// Exit positions
	vector<Vector3> exitPositions;


	// Start positions
	vector<Vector3> blockerPositions;
	// Exit positions
	vector<Vector3> blockerWaypointPositions;

	// Check objects' position in the map
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			// Hero
			if (m_cMap->theScreenMap[i][k] == 109)
			{
				float pos_x = k*m_cMap->GetTileSize();
				float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();
				
				startPositions.push_back(Vector3(pos_x, pos_y));

				// Tutorial
				if (level == 0)
				{
					// Control Dialogue
					dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[0], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
					Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
					Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
					dialogueTiles.back()->setBoundingBox(topleft, bottomright);
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
			// Door
			else if (m_cMap->theScreenMap[i][k] == 110)
			{
				// Create a new exit pos
				exitPositions.push_back(Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()));
			}
			// Blockers
			else if (m_cMap->theScreenMap[i][k] == 10)
			{
				blockerPositions.push_back(Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()));
			}
			// Blockers waypoints
			else if (m_cMap->theScreenMap[i][k] == 11)
			{
				blockerWaypointPositions.push_back(Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()));
			}
			// Tutorial
			if (level == 0)
			{
				// Timer Dialogue
				if (m_cMap->theScreenMap[i][k] == 101)
				{
					float pos_x = k*m_cMap->GetTileSize();
					float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

					dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[1], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
					Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
					Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
					dialogueTiles.back()->setBoundingBox(topleft, bottomright);
				}
				// Objective Dialogue
				else if (m_cMap->theScreenMap[i][k] == 102)
				{
					float pos_x = k*m_cMap->GetTileSize();
					float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

					dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[2], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
					Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
					Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
					dialogueTiles.back()->setBoundingBox(topleft, bottomright);
				}
				// Path choosing Dialogue
				else if (m_cMap->theScreenMap[i][k] == 103)
				{
					float pos_x = k*m_cMap->GetTileSize();
					float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

					dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[3], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
					Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
					Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
					dialogueTiles.back()->setBoundingBox(topleft, bottomright);
				}
				// Blockers Dialogue
				else if (m_cMap->theScreenMap[i][k] == 105)
				{
					float pos_x = k*m_cMap->GetTileSize();
					float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

					dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[4], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
					Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
					Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
					dialogueTiles.back()->setBoundingBox(topleft, bottomright);
				}
				// Exit found Dialogue
				else if (m_cMap->theScreenMap[i][k] == 105)
				{
					float pos_x = k*m_cMap->GetTileSize();
					float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

					dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[5], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
					Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
					Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
					dialogueTiles.back()->setBoundingBox(topleft, bottomright);
				}
				// Deadend Dialogue
				else if (m_cMap->theScreenMap[i][k] == 106)
				{
					float pos_x = k*m_cMap->GetTileSize();
					float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

					dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[6], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
					Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
					Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
					dialogueTiles.back()->setBoundingBox(topleft, bottomright);
				}
			}
		}
	}
	// Randomly choose an start pos for non-tutorial and easy difficulties
	if (level != 0 && level != 1)
	{
		CProbabilitySystem probabilitySystem;
		// Add probabilties
		for (int i = 0; i < startPositions.size(); i++)
		{
			probabilitySystem.AddProbability(1.f);
		}

		// Initialise the hero's position
		theHero = new CPlayerInfo(m_cMap);
		theHero->setPosition(startPositions[probabilitySystem.GetARandIntProbability()]);
	}
	else
	{
		// Initialise the hero's position
		theHero = new CPlayerInfo(m_cMap);
		theHero->setPosition(startPositions[0]);
	}


	// Initialise the Meshes
	InitMeshes();

	// Randomly choose an exit for non-tutorial difficulties
	if (level != 0)
	{
		CProbabilitySystem probabilitySystem;
		// Add probabilties
		for (int i = 0; i < exitPositions.size(); i++)
		{
			probabilitySystem.AddProbability(1.f);
		}

		// Create a new door
		theDoor = new CDoor(CObjects::DOOR, 1, exitPositions[probabilitySystem.GetARandIntProbability()], Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);
	}
	else
	{
		// Create a new door
		theDoor = new CDoor(CObjects::DOOR, 1, exitPositions[0], Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);
	}

	// Blockers
	for (int i = 0; i < blockerPositions.size(); i++)
	{
		theBlockers.push_back(new CAI_Idling(CObjects::AI, Vector3(blockerPositions[i].x, blockerPositions[i].y), Vector3(m_cMap->GetTileSize(), m_cMap->GetTileSize()), meshList[GEO_TILEENEMY_FRAME0], CAI_Idling::MOVINGAROUND));
		theBlockers.back()->AddWaypoint(blockerPositions[i]);
		theBlockers.back()->AddWaypoint(blockerWaypointPositions[i]);
		theBlockers.back()->ChooseWhetherToIdling();
	}
	InitUI();
}

void CSceneGame1::PreInit()
{
	sceneManager2D.PreInit();
}

void CSceneGame1::InitUI()
{
	UIManager = new UISystem();

	Image* AlphaQuad;
	AlphaQuad = new Image("AlphaQuad", meshList[GEO_ALPHA_BLACK_QUAD], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), Vector3(0, 0, 0));
	UIManager->addFeature(AlphaQuad);

	Image* WinScreen;
	WinScreen = new Image("WinScreen", meshList[GEO_CONFIRMATION_WINDOW], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 200, 0), Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.45, 0));
	UIManager->addFeature(WinScreen);

	Image* LoseScreen;
	LoseScreen = new Image("LoseScreen", meshList[GEO_CONFIRMATION_WINDOW], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 200, 0), Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.45, 0));
	UIManager->addFeature(LoseScreen);

	Button* ReturnToHubButton;
	ReturnToHubButton = new Button("ReturnToHubButton", meshList[GEO_NO_BUTTON_UP], meshList[GEO_NO_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45, -200, 0), Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.1, 0));
	UIManager->addFeature(ReturnToHubButton);
}

/********************************************************************************
Initialise the meshes.
********************************************************************************/
void CSceneGame1::InitMeshes()
{
	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GEO_DIALOGUE_BOX] = MeshBuilder::Generate2DMesh("GEO_DIALOGUE_BOX", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_DIALOGUE_BOX]->textureID = LoadTGA("Image//dialogue_box.tga");
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
	meshList[GEO_TILEENEMY_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME0", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILEENEMY_FRAME0]->textureID = LoadTGA("Image//tile20_enemy.tga");

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

	// Confirmation
	// Window
	meshList[GEO_CONFIRMATION_WINDOW] = MeshBuilder::GenerateQuad("GEO_CONFIRMATION_WINDOW", Color(1, 1, 1), 1);
	meshList[GEO_CONFIRMATION_WINDOW]->textureID = LoadTGA("Image//UI/Confirmation_Window.tga");
	// Buttons
	// Yes
	meshList[GEO_YES_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_YES_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_YES_BUTTON_UP]->textureID = LoadTGA("Image//UI/Yes_Button.tga");
	meshList[GEO_YES_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_YES_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_YES_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Yes_Button_Pressed.tga");
	// No
	meshList[GEO_NO_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_NO_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_NO_BUTTON_UP]->textureID = LoadTGA("Image//UI/No_Button.tga");
	meshList[GEO_NO_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_NO_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_NO_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/No_Button_Pressed.tga");

	// Alpha Black Quad
	meshList[GEO_ALPHA_BLACK_QUAD] = MeshBuilder::GenerateQuad("GEO_ALPHA_BLACK_QUAD", Color(1, 1, 1), 1);
	meshList[GEO_ALPHA_BLACK_QUAD]->textureID = LoadTGA("Image//UI/Half_Alpha_Black.tga");
}

void CSceneGame1::Update(double dt)
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

	switch (currentState)
	{
	case PLAYING:
	{
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
		// Check Door
		if (theDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			currentState = EXITING;
			// Animation
			if (theDoor->getPositionX() > theHero->getPositionX())
			{
				theHero->SetAnimationDirection(CPlayerInfo::RIGHT);
			}
			else if(theDoor->getPositionX() < theHero->getPositionX())
			{
				theHero->SetAnimationDirection(CPlayerInfo::LEFT);
			}
			else if (theDoor->getPositionY() > theHero->getPositionY())
			{
				theHero->SetAnimationDirection(CPlayerInfo::UP);
			}
			else if (theDoor->getPositionY() < theHero->getPositionY())
			{
				theHero->SetAnimationDirection(CPlayerInfo::DOWN);
			}
		}
		// Update NPCs
		for (int i = 0; i < theBlockers.size(); i++)
		{
			// Before Check if the Hero collided with the NPCs
			if (theBlockers[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				theHero->setPosition(prevHeroPos);
				theHero->ConstrainHero((m_cMap->GetNumOfTiles_Width() * 0.5) * m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Width() * 0.5) * m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() * 0.5) * m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() * 0.5) * m_cMap->GetTileSize(),	dt, m_cMap);
			}

			// Update those NPCs that are movable
			if (theBlockers[i]->GetAI_Type() == CAI_Idling::MOVINGAROUND)
			{
				theBlockers[i]->Update(dt, theHero, m_cMap);
			}
		}
		// Timer
		timer -= dt;
		if (timer <= 0.0f)
		{
			timer = 0.0f;
			currentState = TIME_UP;
		}
	}
	break;
	case EXITING:
	{
		// Translate hero position toward door
		Vector3 theOldHeroPosition(theHero->getPositionX(), theHero->getPositionY(), 0);
		Vector3 theNewHeroPosition(theOldHeroPosition);
		Vector3 theTargetPosition(theDoor->getPositionX(), theDoor->getPositionY(), 0);

		theNewHeroPosition += (theTargetPosition - theNewHeroPosition).Normalized() * theHero->GetMovementSpeed() * m_cMap->GetTileSize() * dt;

		if ((theOldHeroPosition - theTargetPosition).Length()  > (theOldHeroPosition - theNewHeroPosition).Length())
		{
			theHero->setPositionX(theNewHeroPosition.x);
			theHero->setPositionY(theNewHeroPosition.y);

			// Animation
			theHero->SetAnimationCounter(theHero->GetAnimationCounter() + theHero->GetMovementSpeed() * m_cMap->GetTileSize() * dt * theHero->GetAnimationSpeed());
			if (theHero->GetAnimationCounter() > theHero->GetAnimationMaxCounter())
				theHero->SetAnimationCounter(1);
		}
		else
		{
			theHero->setPositionX(theTargetPosition.x);
			theHero->setPositionY(theTargetPosition.y);
			currentState = COMPLETED;

			// Animation
			theHero->SetAnimationDirection(CPlayerInfo::UP);
			theHero->SetAnimationCounter(0);
		}
	}
	break;
	case TIME_UP:
		break;
	case COMPLETED:
		break;
	}

	UpdateUI(dt);

	// ReCalculate the tile offsets
	tileOffset_x = (int)(theHero->GetMapOffset_x() / m_cMap->GetTileSize());
	// ReCalculate the tile offsets
	tileOffset_y = (int)(theHero->GetMapOffset_y() / m_cMap->GetTileSize());
	fps = (float)(1.f / dt);
}

void CSceneGame1::UpdateUI(double dt)
{
	UIManager->Update(dt);
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneGame1::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneGame1::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}


/********************************************************************************
Render this scene
********************************************************************************/
void CSceneGame1::Render()
{
	sceneManager2D.Render();

	sceneManager2D.modelStack.PushMatrix();

	sceneManager2D.modelStack.Translate(-theHero->GetMapOffset_x(), theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);

	//sceneManager2D.RenderBackground();

	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render Objects
	RenderObjects();
	// Render Hero
	RenderHero();
	// Render AIs
	RenderAIs();

	sceneManager2D.modelStack.PopMatrix();

	// Render GUI
	RenderGUI();
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CSceneGame1::Exit()
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
Handle to the GUI
********************************************************************************/
void CSceneGame1::RenderGUI()
{
	// Jellybean
	sceneManager2D.Render2DMesh(meshList[GEO_JELLYBEAN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());

	//On screen text
	std::ostringstream ss;
	// Jellybean
	ss.precision(3);
	ss << ": " << noOfJellybeans;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), m_cMap->GetTileSize(), m_cMap->GetTileSize(), sceneManager2D.m_window_height - m_cMap->GetTileSize());

	// Timer
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "Time left:", Color(0, 0, 0), m_cMap->GetTileSize() * 0.5, sceneManager2D.m_window_width - m_cMap->GetTileSize() * 3, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 0.5);
	ss.str(std::string());
	ss.precision(3);
	ss << timer;
	if (timer > 10.f)
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0.2, 0.2, 0.2), m_cMap->GetTileSize(), sceneManager2D.m_window_width - m_cMap->GetTileSize() * 3, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 1.5);
	else
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(1, 0, 0), m_cMap->GetTileSize(), sceneManager2D.m_window_width - m_cMap->GetTileSize() * 3, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 1.5);

	switch (currentState)
	{
		case PLAYING:
		{
			UIManager->Render(sceneManager2D);

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
			UIManager->Render(sceneManager2D);
			/*int textSize = m_cMap->GetTileSize();
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptFinished, Color(0, 0, 0), textSize, sceneManager2D.m_window_width * 0.5 - textSize * (scriptFinished.size() * 0.31), sceneManager2D.m_window_height * 0.5 + textSize);

			textSize = m_cMap->GetTileSize() * 0.5;
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptExit, Color(0, 0, 0), textSize, sceneManager2D.m_window_width * 0.5 - textSize * (scriptExit.size() * 0.31), sceneManager2D.m_window_height * 0.5 - textSize);*/
		}
		break;

		case TIME_UP:
		{
			UIManager->Render(sceneManager2D);
			/*int textSize = m_cMap->GetTileSize();
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptTimeUp, Color(0, 0, 0), textSize, sceneManager2D.m_window_width * 0.5 - textSize * (scriptTimeUp.size() * 0.31), sceneManager2D.m_window_height * 0.5 + textSize);

			textSize = m_cMap->GetTileSize() * 0.5;
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptExit, Color(0, 0, 0), textSize, sceneManager2D.m_window_width * 0.5 - textSize * (scriptExit.size() * 0.31), sceneManager2D.m_window_height * 0.5 - textSize);*/
		}
		break;
	}

	// Fps
	/*ss.str(std::string());
	ss.precision(3);
	ss << "FPS:" << fps;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(1, 0, 0), 25, sceneManager2D.m_window_width - 140, 5);
*/
}

/********************************************************************************
Render the tile map. This is a private function for use in this class only
********************************************************************************/
void CSceneGame1::RenderTileMap()
{
	int m = 0;
	int j = 0; 
	for (int i = 0; i < m_cMap->GetNumOfTiles_Height() + 1; i++)
	{
		for (int k = 0; k < m_cMap->GetNumOfTiles_Width() + 1; k++)
		{
			m = tileOffset_x + k;
			j = tileOffset_y + i;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if (m >= m_cMap->getNumOfTiles_MapWidth())
				break;
			// If we have reached the bottom side of the Map, then do not display the extra column of tiles.
			if (j >= m_cMap->getNumOfTiles_MapHeight())
				break;
			if (m_cMap->theScreenMap[j][m] == 1)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_WALL], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), m*m_cMap->GetTileSize(), sceneManager2D.m_window_height - j*m_cMap->GetTileSize());
			}
			else
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_GROUND], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), m*m_cMap->GetTileSize(), sceneManager2D.m_window_height - j*m_cMap->GetTileSize());
			}
		}
	}
}

/********************************************************************************
Render the Hero. This is a private function for use in this class only
********************************************************************************/
void CSceneGame1::RenderHero()
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
void CSceneGame1::RenderAIs()
{
	// Render the Blockers
	for (int i = 0; i < theBlockers.size(); i++)
	{
		if (theBlockers[i])
		{
			if ((theBlockers[i]->getPosition() - theHero->getPosition()).Length() <= sceneManager2D.m_window_width)
				sceneManager2D.Render2DMesh(theBlockers[i]->getMesh(), false, theBlockers[i]->getScale().x, theBlockers[i]->getScale().y, theBlockers[i]->getPositionX(), theBlockers[i]->getPositionY());
		}
	}
}

/********************************************************************************
Render the AIs. This is a private function for use in this class only
********************************************************************************/
void CSceneGame1::RenderObjects()
{
	// Door
	sceneManager2D.Render2DMesh(theDoor->getMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theDoor->getPositionX(), theDoor->getPositionY());
}

void CSceneGame1::RenderWaypoints()
{
	for (int i = 0; i < temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), temp.at(i).x, temp.at(i).y);
	}
}
