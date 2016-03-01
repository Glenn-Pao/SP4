#include "SceneGame4.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

#include "..\..\..\UsingLua.h"

SceneGame4::SceneGame4(const int m_window_width, const int m_window_height)
	: CurrentState(PLAY)
	, UIManager(NULL)
	, Timer(0)
{
}

SceneGame4::~SceneGame4()
{
	if (UIManager)
	{
		delete UIManager;
		UIManager = NULL;
	}
}

void SceneGame4::Init(int level) // level = 0(Tutorial), = 1(Easy), = 2(Medium), = 3(Hard)
{
	CurrentLevel = level;

	// Init the base scene
	sceneManager2D.Init(CurrentLevel);

	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//Scene/Game1/game1.lua");

	int tileSize = L.DoLuaInt("tileSize");

	// Dialogues scripts
	vector<string> scriptDialogues;

	switch (level)
	{
	case DifficultyLevel::TUTORIAL:
	{
		for (int i = 0; i < 6; i++)
		{
			scriptDialogues.push_back(L.DoLuaString("script" + to_string(i)));
		}

		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 13 * tileSize, 16 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 4/Tutorial.csv");
		break;
	}
	case DifficultyLevel::EASY:
	{
		for (int i = 0; i < 6; i++)
		{
			scriptDialogues.push_back(L.DoLuaString("script" + to_string(i)));
		}

		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 13 * tileSize, 16 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 4/Tutorial.csv");
		break;
	}
	case DifficultyLevel::NORMAL:
	{
		for (int i = 0; i < 6; i++)
		{
			scriptDialogues.push_back(L.DoLuaString("script" + to_string(i)));
		}

		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 13 * tileSize, 16 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 4/Tutorial.csv");
		break;
	}
	case DifficultyLevel::HARD:
	{
		for (int i = 0; i < 6; i++)
		{
			scriptDialogues.push_back(L.DoLuaString("script" + to_string(i)));
		}

		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 13 * tileSize, 16 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 4/Tutorial.csv");
		break;
	}
	}

	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			// Hero
			if (m_cMap->theScreenMap[i][k] == 1000 && theHero == NULL)
			{
				float pos_x = k*m_cMap->GetTileSize();
				float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();
				// Initialise the hero's position
				theHero = new CPlayerInfo(m_cMap);
				theHero->setPositionX(pos_x);
				theHero->setPositionY(pos_y);

				// Tutorial
				if (level == 0)
				{
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

	// Initialise the Meshes
	InitMeshes();



	
	
	int Chance;

	switch (CurrentLevel)
	{
	case TUTORIAL:
	{
		//Initialise Deck
		PatternInserted = new Deck(Vector3(300, 300, 1), Vector3(80, 0, 0));
		PatternToFollow = new Deck(Vector3(300, 425, 1), Vector3(80, 0, 0));

		Instructions = new Trigger(meshList[GEO_INSTRUCTIONS], Vector3(275, 130, 0), Vector3(250, 100, 50), Vector3(0, 20, 0), Vector3(800, 100, 1), true);
		CurrentPhase = TutorialPhase::PHASE_1;
		Timer = 0;

		//Create 3 Cards Difficulty

		for (int i = 0; i < 3; ++i)
		{
			ps.AddProbability(1);
		}

		for (int i = 0; i < 3; ++i)
		{
			Chance = ps.GetARandIntProbability();
			//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
			PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));
			switch (Chance)
			{
			case 0:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true)); break; }
			case 1:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true)); break; }
			case 2:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true)); break; }
			}
		}
		break;
	}
	case EASY:
	{
		//GamePlay Variables
		MissingCardTimer = 0;
		ScoreToBeat = 20;
		Score = 0;
		Timer = 60;

		//Initialise Deck
		PatternInserted = new Deck(Vector3(250, 300, 1), Vector3(80, 0, 0));
		PatternToFollow = new Deck(Vector3(250, 425, 1), Vector3(80, 0, 0));

		Instructions = new Trigger(meshList[GEO_INSTRUCTIONS], Vector3(275, 130, 0), Vector3(250, 100, 50), Vector3(0, 20, 0), Vector3(800, 100, 1), true);
		CurrentPhase = TutorialPhase::PHASE_1;

		//Create 3 Cards Difficulty

		for (int i = 0; i < 3; ++i)
		{
			ps.AddProbability(1);
		}

		for (int i = 0; i < 4; ++i)
		{
			Chance = ps.GetARandIntProbability();
			//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
			PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));
			switch (Chance)
			{
			case 0:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true)); break; }
			case 1:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true)); break; }
			case 2:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true)); break; }
			}
		}
		break;
	}
	case NORMAL:
	{
		//GamePlay Variables
		MissingCardTimer = 0;
		ScoreToBeat = 20;
		Score = 0;
		Timer = 60;

		//Initialise Deck
		PatternInserted = new Deck(Vector3(175, 300, 1), Vector3(80, 0, 0));
		PatternToFollow = new Deck(Vector3(175, 425, 1), Vector3(80, 0, 0));

		Instructions = new Trigger(meshList[GEO_INSTRUCTIONS], Vector3(275, 130, 0), Vector3(250, 100, 50), Vector3(0, 20, 0), Vector3(800, 100, 1), true);
		CurrentPhase = TutorialPhase::PHASE_1;

		//Create 3 Cards Difficulty

		for (int i = 0; i < 3; ++i)
		{
			ps.AddProbability(1);
		}

		for (int i = 0; i < 6; ++i)
		{
			Chance = ps.GetARandIntProbability();
			//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
			PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));
			switch (Chance)
			{
			case 0:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true)); break; }
			case 1:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true)); break; }
			case 2:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true)); break; }
			}
		}
		break;
	}
	case HARD:
	{
		//GamePlay Variables
		MissingCardTimer = -2;
		ScoreToBeat = 20;
		Score = 0;
		Timer = 60;

		//Initialise Deck
		PatternInserted = new Deck(Vector3(85, 300, 1), Vector3(80, 0, 0));
		PatternToFollow = new Deck(Vector3(85, 425, 1), Vector3(80, 0, 0));

		Instructions = new Trigger(meshList[GEO_INSTRUCTIONS], Vector3(275, 130, 0), Vector3(250, 100, 50), Vector3(0, 20, 0), Vector3(800, 100, 1), true);
		CurrentPhase = TutorialPhase::PHASE_1;

		//Create 3 Cards Difficulty

		for (int i = 0; i < 3; ++i)
		{
			ps.AddProbability(1);
		}

		for (int i = 0; i < 8; ++i)
		{
			Chance = ps.GetARandIntProbability();
			//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
			PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));
			switch (Chance)
			{
			case 0:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true)); break; }
			case 1:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true)); break; }
			case 2:
			{PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true)); break; }
			}
		}
		break;
	}
	}

	RedCard = new Card(Card::CARD, true, "NIL", Vector3(275, 150, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true);
	GreenCard = new Card(Card::CARD, true, "NIL", Vector3(375, 150, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true);
	BlueCard = new Card(Card::CARD, true, "NIL", Vector3(475, 150, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true);
	SelectedCard = new  Card(Card::CARD, false, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::Element::NONE, true);
	NoneCard = new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::Element::NONE, true);
	SendPattern = new Trigger(meshList[GEO_STRESS], Vector3(100, 150, 1), Vector3(50, 50, 50), Vector3(100, 150, 1), Vector3(50, 50, 50), true);


	InitUI();
}

void SceneGame4::PreInit()
{
	sceneManager2D.PreInit();
}

void SceneGame4::InitUI()
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

	Image* CompletedScreen;
	CompletedScreen = new Image("CompletedScreen", meshList[GEO_CONFIRMATION_WINDOW], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 200, 0), Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.45, 0));
	UIManager->addFeature(CompletedScreen);

	Button* ReturnToHubButton;
	ReturnToHubButton = new Button("ReturnToHubButton", meshList[GEO_NO_BUTTON_UP], meshList[GEO_NO_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45, -200, 0), Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.1, 0));
	UIManager->addFeature(ReturnToHubButton);

}

/********************************************************************************
Initialise the meshes.
********************************************************************************/
void SceneGame4::InitMeshes()
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
	meshList[GEO_RED_CARD] = MeshBuilder::Generate2DMesh("RED_CARD", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_RED_CARD]->textureID = LoadTGA("Image//RedCard.tga");
	meshList[GEO_BLUE_CARD] = MeshBuilder::Generate2DMesh("BLUE_CARD", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_BLUE_CARD]->textureID = LoadTGA("Image//BlueCard.tga");
	meshList[GEO_GREEN_CARD] = MeshBuilder::Generate2DMesh("GREEN_CARD", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_GREEN_CARD]->textureID = LoadTGA("Image//GreenCard.tga");
	meshList[GEO_STRESS_CARD] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_STRESS_CARD]->textureID = LoadTGA("Image//StressCard.tga");
	meshList[GEO_MISSING_CARD] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_MISSING_CARD]->textureID = LoadTGA("Image//MissingCard.tga");
	meshList[GEO_INSTRUCTIONS] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//Game4TutorialInstuction1.tga");
	meshList[GEO_STRESS] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_STRESS]->textureID = LoadTGA("Image//StressButton.tga");
	meshList[GEO_PATTERN] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_PATTERN]->textureID = LoadTGA("Image//PatternToFollow.tga");

	

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

	// RETURN
	// Window
	meshList[GEO_CONFIRMATION_WINDOW] = MeshBuilder::GenerateQuad("GEO_RETURN_WINDOW", Color(1, 1, 1), 1);
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

void SceneGame4::UpdateHero(double dt)
{

	// Update Hero animation counter if hero moved
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

}

void SceneGame4::UpdateTutorialInstructions(double dt)
{
	//Update Instructions
	switch (CurrentPhase)
	{
		// Pick Up A Card
	case TutorialPhase::PHASE_1:
	{
		if (Application::IsKeyPressed('F') && Instructions->CheckCollision((*theHero->getBoundingBox())) == true)
		{
			if (RedCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true ||
				BlueCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true ||
				GreenCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
			{
				meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//Game4TutorialInstuction2.tga");
				Instructions->setPosition(Vector3(375, 350, 0));
				Instructions->setScale(Vector3(600, 100, 0));
				Instructions->getBoundingBox()->Reset(Instructions->getPosition(), Instructions->getScale());

				CurrentPhase = TutorialPhase::PHASE_2;
			}
		}
		break;
	}
	//Place Card Onto Stress Cards
	case TutorialPhase::PHASE_2:
	{
		if (Application::IsKeyPressed('F') && Instructions->CheckCollision((*theHero->getBoundingBox())) == true)
		{
			for (int i = 0; i < PatternInserted->getListOfCards().size(); ++ i)
			{
				if (PatternInserted->getListOfCards()[i]->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
				{
					meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//Game4TutorialInstuction3.tga");
					Instructions->setPosition(Vector3(400, 150, 0));
					Instructions->setScale(Vector3(250, 100, 50));
					Instructions->getBoundingBox()->Reset(Instructions->getPosition(), Instructions->getScale());

					CurrentPhase = TutorialPhase::PHASE_3;
				}
			}
			
		}
		break;
	}
	// Pick Up A Different Card
	case TutorialPhase::PHASE_3:
	{
		if (Application::IsKeyPressed('F') && Instructions->CheckCollision((*theHero->getBoundingBox())) == true)
		{
			//if either Card Collides with Hero
			if (RedCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true && SelectedCard->getElement_Type() != RedCard->getElement_Type() ||
				BlueCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true && SelectedCard->getElement_Type() != BlueCard->getElement_Type() ||
				GreenCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true && SelectedCard->getElement_Type() != GreenCard->getElement_Type())
			{
					meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//Game4TutorialInstuction4.tga");
					CurrentPhase = TutorialPhase::PHASE_4;
			}
		}
		break;
	}
	// Match The Pattern 
	case TutorialPhase::PHASE_4:
	{
		if (PatternToFollow->isDeckIdentical(PatternInserted) == true)
		{
			meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//Game4TutorialInstuction5.tga");
			CurrentPhase = TutorialPhase::PHASE_5;
		}
		break;
	}
	//Press The Stress Button
	case TutorialPhase::PHASE_5:
	{
		if (PatternToFollow->isDeckIdentical(PatternInserted) == true)
		{
			if (Application::IsKeyPressed('F') && SendPattern->CheckCollision((*theHero->getBoundingBox())) == true)
			{
				meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//Game4TutorialInstuction6.tga");
				CurrentState = State::TIME_UP;
			}
		}
		break;
	}
	}
}

void SceneGame4::UpdateDecks(double dt)
{

	switch (CurrentLevel)
	{
	case DifficultyLevel::TUTORIAL:
	{
		//Update Decks
		if (Application::IsKeyPressed('F'))
		{
			if (SendPattern->CheckCollision((*theHero->getBoundingBox())) == true && PatternToFollow->isDeckIdentical(PatternInserted) == true)
			{
				//Reset Time For Missing Card
				MissingCardTimer = 0;

				//Clear Contents of PatternToFollow Deck
				for (int i = PatternToFollow->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternToFollow->ListOfCards[i];
					PatternToFollow->ListOfCards.pop_back();
				}
				//Clear Contents of PatternInserted Deck
				for (int i = PatternInserted->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternInserted->ListOfCards[i];
					PatternInserted->ListOfCards.pop_back();
				}

				for (int i = 0; i < 4; ++i)
				{
					//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
					PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));

					int randNo = ps.GetARandIntProbability();

					switch (randNo)
					{
					case 0:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true));
						break;
					}
					case 1:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true));
						break;
					}
					case 2:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true));
						break;
					}
					}
				}

				Score += 8;
			}

			for (int i = 0; i < PatternInserted->getListOfCards().size(); ++i)
			{
				if (PatternInserted->getListOfCards()[i]->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
				{
					PatternInserted->getListOfCards()[i]->setCardFaceUpMesh(SelectedCard->getCardFaceUpMesh());
					PatternInserted->getListOfCards()[i]->setElement_Type(SelectedCard->getElement_Type());
					PatternInserted->getListOfCards()[i]->setisRevealed(true);
					PatternInserted->getListOfCards()[i]->setActive(true);
				}
				else
				{
				}
			}
		}
		break;
	}
	case DifficultyLevel::EASY:
	{
		//Update Decks
		if (Application::IsKeyPressed('F'))
		{
			if (SendPattern->CheckCollision((*theHero->getBoundingBox())) == true && PatternToFollow->isDeckIdentical(PatternInserted) == true)
			{
				//Clear Contents of PatternToFollow Deck
				for (int i = PatternToFollow->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternToFollow->ListOfCards[i];
					PatternToFollow->ListOfCards.pop_back();
				}
				//Clear Contents of PatternInserted Deck
				for (int i = PatternInserted->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternInserted->ListOfCards[i];
					PatternInserted->ListOfCards.pop_back();
				}

				for (int i = 0; i < 4; ++i)
				{
					//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
					PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));

					int randNo = ps.GetARandIntProbability();

					switch (randNo)
					{
					case 0:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true));
						break;
					}
					case 1:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true));
						break;
					}
					case 2:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true));
						break;
					}
					}
				}
				Score += 8;
				MissingCardTimer = 0;
			}

			for (int i = 0; i < PatternInserted->getListOfCards().size(); ++i)
			{
				if (PatternInserted->getListOfCards()[i]->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
				{
					PatternInserted->getListOfCards()[i]->setCardFaceUpMesh(SelectedCard->getCardFaceUpMesh());
					PatternInserted->getListOfCards()[i]->setElement_Type(SelectedCard->getElement_Type());
					PatternInserted->getListOfCards()[i]->setisRevealed(true);
					PatternInserted->getListOfCards()[i]->setActive(true);
				}
				else
				{
				}
			}
		}
		break;
	}
	case DifficultyLevel::NORMAL:
	{
		//Update Decks
		if (Application::IsKeyPressed('F'))
		{
			if (SendPattern->CheckCollision((*theHero->getBoundingBox())) == true && PatternToFollow->isDeckIdentical(PatternInserted) == true)
			{
				//Clear Contents of PatternToFollow Deck
				for (int i = PatternToFollow->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternToFollow->ListOfCards[i];
					PatternToFollow->ListOfCards.pop_back();
				}
				//Clear Contents of PatternInserted Deck
				for (int i = PatternInserted->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternInserted->ListOfCards[i];
					PatternInserted->ListOfCards.pop_back();
				}

				for (int i = 0; i < 6; ++i)
				{
					//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
					PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));

					int randNo = ps.GetARandIntProbability();

					switch (randNo)
					{
					case 0:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true));
						break;
					}
					case 1:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true));
						break;
					}
					case 2:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true));
						break;
					}
					}
				}
				Score += 8;
				MissingCardTimer = 0;
			}

			for (int i = 0; i < PatternInserted->getListOfCards().size(); ++i)
			{
				if (PatternInserted->getListOfCards()[i]->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
				{
					PatternInserted->getListOfCards()[i]->setCardFaceUpMesh(SelectedCard->getCardFaceUpMesh());
					PatternInserted->getListOfCards()[i]->setElement_Type(SelectedCard->getElement_Type());
					PatternInserted->getListOfCards()[i]->setisRevealed(true);
					PatternInserted->getListOfCards()[i]->setActive(true);
				}
				else
				{
				}
			}
		}
		break;
	}
	case DifficultyLevel::HARD:
	{
		//Update Decks
		if (Application::IsKeyPressed('F'))
		{
			if (SendPattern->CheckCollision((*theHero->getBoundingBox())) == true && PatternToFollow->isDeckIdentical(PatternInserted) == true)
			{
				//Clear Contents of PatternToFollow Deck
				for (int i = PatternToFollow->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternToFollow->ListOfCards[i];
					PatternToFollow->ListOfCards.pop_back();
				}
				//Clear Contents of PatternInserted Deck
				for (int i = PatternInserted->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternInserted->ListOfCards[i];
					PatternInserted->ListOfCards.pop_back();
				}

				for (int i = 0; i < 8; ++i)
				{
					//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
					PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));

					int randNo = ps.GetARandIntProbability();

					switch (randNo)
					{
					case 0:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true));
						break;
					}
					case 1:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true));
						break;
					}
					case 2:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(70, 100, 1), meshList[GEO_MISSING_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true));
						break;
					}
					}
				}
				Score += 8;
				MissingCardTimer = 0;
			}

			for (int i = 0; i < PatternInserted->getListOfCards().size(); ++i)
			{
				if (PatternInserted->getListOfCards()[i]->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
				{
					PatternInserted->getListOfCards()[i]->setCardFaceUpMesh(SelectedCard->getCardFaceUpMesh());
					PatternInserted->getListOfCards()[i]->setElement_Type(SelectedCard->getElement_Type());
					PatternInserted->getListOfCards()[i]->setisRevealed(true);
					PatternInserted->getListOfCards()[i]->setActive(true);
				}
				else
				{
				}
			}
		}
		break;
	}
	}
	
}

void SceneGame4::UpdateUI(double dt)
{
	UIManager->Update(dt);
}

void SceneGame4::UpdateRGBCard(double dt)
{
	if (RedCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
	{
		if (Application::IsKeyPressed('F'))
		{
			SelectedCard = RedCard;
		}
	}

	if (GreenCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
	{
		if (Application::IsKeyPressed('F'))
		{
			SelectedCard = GreenCard;
		}
		else
		{
		}
	}
	if (BlueCard->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
	{
		if (Application::IsKeyPressed('F'))
		{
			SelectedCard = BlueCard;
		}
		else
		{
		}
	}
}

void SceneGame4::UpdateTimer(double dt)
{
	if (Timer < 0)
	{
		Timer = 0;
		CurrentState = State::TIME_UP;
	}
	else
	{
		Timer -= dt;
	}
}

void SceneGame4::Update(double dt)
{

	UpdateUI(dt);

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	sceneManager2D.Update(dt);
	
	switch (CurrentLevel)
	{
	case DifficultyLevel::TUTORIAL:
	{
		switch (CurrentState)
		{
		case PLAY:
		{
			UpdateTutorialInstructions(dt);
			UpdateDecks(dt);
			UpdateRGBCard(dt);
			UpdateHero(dt);
			break;
		}
		case TIME_UP:
		{
			break;
		}

		}
		break;
	}
	case DifficultyLevel::EASY:
	{
		switch (CurrentState)
		{
		case PLAY:
		{
			MissingCardTimer += dt;

			if (MissingCardTimer > 3)
			{
				MissingCardTimer = 0;

				PatternToFollow->getListOfCards()[Math::RandIntMinMax(0, 3)]->setisRevealed(false);
			}

			UpdateHero(dt);
			UpdateDecks(dt);
			UpdateRGBCard(dt);
			UpdateTimer(dt);
			break;
		}
		case State::TIME_UP:
		{
			if (Score >= ScoreToBeat)
			{
				CurrentState = State::WIN;
				//Do UI
			}
			else
			{
				CurrentState = State::LOSE;
				//DO UI
			}
			break;
		}
		
		}
		break;

	}
	case DifficultyLevel::NORMAL:
	{
		switch (CurrentState)
		{
		case PLAY:
		{
			MissingCardTimer += dt;

			if (MissingCardTimer > 2)
			{
				MissingCardTimer = 0;

				PatternToFollow->getListOfCards()[Math::RandIntMinMax(0, 5)]->setisRevealed(false);
			}

			UpdateHero(dt);
			UpdateDecks(dt);
			UpdateRGBCard(dt);
			UpdateTimer(dt);
			break;
		}
		case State::TIME_UP:
		{
			if (Score >= ScoreToBeat)
			{
				CurrentState = State::WIN;
			}
			else
			{
				CurrentState = State::LOSE;
			}
			
			break;
		}

		}
		break;
	}
	case DifficultyLevel::HARD:
	{

		cout << MissingCardTimer << endl;
		switch (CurrentState)
		{
		case PLAY:
		{
			MissingCardTimer += dt;

			if (MissingCardTimer > 1)
			{
				MissingCardTimer = 0;

				PatternToFollow->getListOfCards()[Math::RandIntMinMax(0, 7)]->setisRevealed(false);
			}

			UpdateHero(dt);
			UpdateDecks(dt);
			UpdateRGBCard(dt);
			UpdateTimer(dt);
			break;
		}
		case State::TIME_UP:
		{
			if (Score >= ScoreToBeat)
			{
				CurrentState = State::WIN;
			}
			else
			{
				CurrentState = State::LOSE;
			}
			break;
		}

		}
		break;
	}
	}
}

/********************************************************************************
Update Camera position
********************************************************************************/
void SceneGame4::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void SceneGame4::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}

void SceneGame4::RenderTutorialInstructions()
{
	//Render Instructions
	sceneManager2D.Render2DMesh(Instructions->getMesh(), false, Instructions->getMeshScale().x, Instructions->getMeshScale().y, Instructions->getMeshPosition().x, Instructions->getMeshPosition().y, 0);

}

void SceneGame4::RenderDeck()
{
	//Render Pattern To Follow
	for (int i = 0; i < PatternToFollow->getListOfCards().size(); ++i)
	{
		if (PatternToFollow->getListOfCards()[i]->getActive() == true)
		{
			if (PatternToFollow->getListOfCards()[i]->getisRevealed() == true)
			{
				sceneManager2D.Render2DMesh(PatternToFollow->getListOfCards()[i]->getCardFaceUpMesh(), false, PatternToFollow->getListOfCards()[i]->getScaleX(), PatternToFollow->getListOfCards()[i]->getScaleY(), PatternToFollow->getListOfCards()[i]->getPositionX(), PatternToFollow->getListOfCards()[i]->getPositionY(), 0);
			}
			else
			{
				sceneManager2D.Render2DMesh(PatternToFollow->getListOfCards()[i]->getMesh(), false, PatternToFollow->getListOfCards()[i]->getScaleX(), PatternToFollow->getListOfCards()[i]->getScaleY(), PatternToFollow->getListOfCards()[i]->getPositionX(), PatternToFollow->getListOfCards()[i]->getPositionY(), 0);
			}
		}
	}
	sceneManager2D.Render2DMesh(meshList[GEO_PATTERN], false, 200, 30, PatternToFollow->getPosition().x, PatternToFollow->getPosition().y + 110, 0);

	//Render Pattern To Insert
	for (int i = 0; i < PatternInserted->getListOfCards().size(); ++i)
	{

		if (PatternInserted->getListOfCards()[i]->getisRevealed() == true)
		{
			sceneManager2D.Render2DMesh(PatternInserted->getListOfCards()[i]->getCardFaceUpMesh(), false, PatternInserted->getListOfCards()[i]->getScaleX(), PatternInserted->getListOfCards()[i]->getScaleY(), PatternInserted->getListOfCards()[i]->getPositionX(), PatternInserted->getListOfCards()[i]->getPositionY(), 0);
		}
		else
		{
			sceneManager2D.Render2DMesh(PatternInserted->getListOfCards()[i]->getMesh(), false, PatternInserted->getListOfCards()[i]->getScaleX(), PatternInserted->getListOfCards()[i]->getScaleY(), PatternInserted->getListOfCards()[i]->getPositionX(), PatternInserted->getListOfCards()[i]->getPositionY(), 0);
		}
	}
}

void SceneGame4::RenderRGBCards()
{
	if (RedCard->getisRevealed() == true)
	{
		sceneManager2D.Render2DMesh(RedCard->getCardFaceUpMesh(), false, RedCard->getScaleX(), RedCard->getScaleY(), RedCard->getPositionX(), RedCard->getPositionY(), 0);
	}
	else
	{
		sceneManager2D.Render2DMesh(RedCard->getMesh(), false, RedCard->getScaleX(), RedCard->getScaleY(), RedCard->getPositionX(), RedCard->getPositionY(), 0);
	}


	if (BlueCard->getisRevealed() == true)
	{
		sceneManager2D.Render2DMesh(BlueCard->getCardFaceUpMesh(), false, BlueCard->getScaleX(), BlueCard->getScaleY(), BlueCard->getPositionX(), BlueCard->getPositionY(), 0);
	}
	else
	{
		sceneManager2D.Render2DMesh(BlueCard->getMesh(), false, BlueCard->getScaleX(), BlueCard->getScaleY(), BlueCard->getPositionX(), BlueCard->getPositionY(), 0);
	}

	if (GreenCard->getisRevealed() == true)
	{
		sceneManager2D.Render2DMesh(GreenCard->getCardFaceUpMesh(), false, GreenCard->getScaleX(), GreenCard->getScaleY(), GreenCard->getPositionX(), GreenCard->getPositionY(), 0);
	}
	else
	{
		sceneManager2D.Render2DMesh(GreenCard->getMesh(), false, GreenCard->getScaleX(), GreenCard->getScaleY(), GreenCard->getPositionX(), GreenCard->getPositionY(), 0);
	}
	//Render Card With Player;

	if (SelectedCard->getActive() == true)
	{
		sceneManager2D.Render2DMesh(SelectedCard->getCardFaceUpMesh(), false, SelectedCard->getScaleX(), SelectedCard->getScaleY(), theHero->getPositionX(), theHero->getPositionY() + 50, 0);
	}

}

void SceneGame4::RenderTrigger()
{
	sceneManager2D.Render2DMesh(SendPattern->getMesh(), false, SendPattern->getMeshScale().x, SendPattern->getMeshScale().y, SendPattern->getMeshPosition().x, SendPattern->getMeshPosition().y, 0);
}

void SceneGame4::RenderTimer()
{
	std::ostringstream ss;
	// Timer
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "Time left:", Color(0.5, 0.3, 0.3), m_cMap->GetTileSize() * 0.5, sceneManager2D.m_window_width - m_cMap->GetTileSize() * 3, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 0.5);
	ss.str(std::string());
	ss.precision(3);
	ss << Timer;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0.5, 0.3, 0.3), m_cMap->GetTileSize(), sceneManager2D.m_window_width - m_cMap->GetTileSize() * 3, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 1.5);

}
void SceneGame4::RenderScore()
{
	std::ostringstream ss;
	// Timer
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "Score:", Color(0.5, 0.3, 0.3), m_cMap->GetTileSize(), 0, m_cMap->GetTileSize() * 0.5);
	ss.str(std::string());
	ss.precision(3);
	ss << Score;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0.5, 0.3, 0.3), m_cMap->GetTileSize(), m_cMap->GetTileSize() * 4, m_cMap->GetTileSize() * 0.5);

}

/********************************************************************************
Render this scene
********************************************************************************/
void SceneGame4::Render()
{
	sceneManager2D.Render();

	sceneManager2D.modelStack.PushMatrix();

	sceneManager2D.modelStack.Translate(-theHero->GetMapOffset_x(), theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);
	switch (CurrentLevel)
	{
	case DifficultyLevel::TUTORIAL:
	{
		switch (CurrentState)
		{
		case State::PLAY:
		{
			RenderRearTileMap();
			RenderTileMap();
			RenderDeck();
			RenderRGBCards();
			RenderTrigger();
			RenderHero();
			RenderTutorialInstructions();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			break;
		}
		case State::TIME_UP:
		{
			RenderRearTileMap();
			RenderTileMap();
			//RenderTutorialInstructions();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}
		
		}
		break;
	}
	case DifficultyLevel::EASY:
	{
		switch (CurrentState)
		{
		case State::PLAY:
		{
			RenderRearTileMap();
			RenderTileMap();
			RenderDeck();
			RenderRGBCards();
			RenderTrigger();
			RenderHero();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			RenderTimer();
			RenderScore();
			break;
		}
		case State::TIME_UP:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}
		case State::WIN:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}

		case State::LOSE:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}

		}
		break;
	}
	case DifficultyLevel::NORMAL:
	{
		switch (CurrentState)
		{
		case State::PLAY:
		{
			RenderRearTileMap();
			RenderTileMap();
			RenderDeck();
			RenderRGBCards();
			RenderTrigger();
			RenderHero();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			RenderTimer();
			RenderScore();
			break;
		}
		case State::TIME_UP:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}
		case State::WIN:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}

		case State::LOSE:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}
		}
		break;
	}
	case DifficultyLevel::HARD:
	{
		switch (CurrentState)
		{
		case State::PLAY:
		{
			RenderRearTileMap();
			RenderTileMap();
			RenderDeck();
			RenderRGBCards();
			RenderTrigger();
			RenderHero();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			RenderTimer();
			RenderScore();
			break;
		}
		case State::TIME_UP:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}
		case State::WIN:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}

		case State::LOSE:
		{
			RenderRearTileMap();
			RenderTileMap();
			sceneManager2D.modelStack.PopMatrix();

			RenderGUI();
			//sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[sceneManager2D.GEO_TEXT], "<Click anywhere to exit>", Color(0, 0, 0), 50, sceneManager2D.m_window_width / 20, sceneManager2D.m_window_height / 2);
			break;
		}
		}
		break;
	}
	}
	//sceneManager2D.RenderBackground();


	//for (int i = 0; i < PatternToFollow->getListOfCards().size(); ++i)
	//{
	//	cout<< PatternToFollow->getListOfCards()[i]->getElement_Type()<<" ";
	//}
	//cout << endl;
	//for (int i = 0; i < PatternInserted->getListOfCards().size(); ++i)
	//{
	//	cout << PatternInserted->getListOfCards()[i]->getElement_Type() << " ";
	//}
	//cout << endl;
	//cout << endl;

	/*for (int i = 0; i < HandPileRed->getListOfCards().size(); ++i)
	{
	RenderCard(HandPileRed->getListOfCards()[i]);
	}
	for (int i = 0; i < HandPileGreen->getListOfCards().size(); ++i)
	{
	RenderCard(HandPileGreen->getListOfCards()[i]);
	}
	for (int i = 0; i < HandPileBlue->getListOfCards().size(); ++i)
	{
	RenderCard(HandPileBlue->getListOfCards()[i]);
	}
	for (int i = 0; i < DiscardPile->getListOfCards().size(); ++i)
	{
	RenderCard(DiscardPile->getListOfCards()[i]);
	}
	*/


}

/********************************************************************************
Exit this scene
********************************************************************************/
void SceneGame4::Exit()
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
void SceneGame4::RenderGUI()
{
	// Jellybean
	sceneManager2D.Render2DMesh(meshList[GEO_JELLYBEAN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());

	//On screen text
	std::ostringstream ss;
	// Jellybean
	ss.precision(3);
	ss << ": " << noOfJellybeans;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), m_cMap->GetTileSize(), m_cMap->GetTileSize(), sceneManager2D.m_window_height - m_cMap->GetTileSize());

	switch (CurrentState)
	{
	case PLAY:
	{
		UIManager->Render(sceneManager2D);
	}
		break;
	case WIN:
	{
		UIManager->Render(sceneManager2D);
	}
		break;
	case LOSE:
	{
		UIManager->Render(sceneManager2D);
	}
		break;
	case TIME_UP:
	{
		UIManager->Render(sceneManager2D);
	}
		break;
	}

}

/********************************************************************************
Render the tile map. This is a private function for use in this class only
********************************************************************************/
void SceneGame4::RenderTileMap()
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
void SceneGame4::RenderHero()
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
void SceneGame4::RenderAIs()
{
	// Render the enemy
	for (int i = 0; i < theEnemies.size(); i++)
	{
		int theEnemy_x = theEnemies[i]->GetPos_x();
		int theEnemy_y = theEnemies[i]->GetPos_y();
		if (((theEnemy_x >= 0 - m_cMap->GetTileSize()) && (theEnemy_x < sceneManager2D.m_window_width + m_cMap->GetTileSize())) &&
			((theEnemy_y >= 0 - m_cMap->GetTileSize()) && (theEnemy_y < sceneManager2D.m_window_height + m_cMap->GetTileSize())))
		{
			sceneManager2D.Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theEnemy_x, theEnemy_y);
		}
	}
}
