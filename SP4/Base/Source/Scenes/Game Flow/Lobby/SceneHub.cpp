#include "SceneHub.h"
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

CSceneHub::CSceneHub(const int m_window_width, const int m_window_height)
	: currentState(PLAYING)
	, UIManagerDifficultySelection(NULL)
	, UIManagerJellybeanSelection(NULL)
	, UIManagerBlackQuad(NULL)
	, UIManagerConfirmation(NULL)
	, game_interacted(NO_GAME)
	, noOfJellybeansDeposited(0)
	, difficultySelected(0)
	, UI_Speed(0.f)
{
}

CSceneHub::~CSceneHub()
{
	for (int i = 0; i < theDoor.size(); i++)
	{
		if (theDoor[i])
		{
			delete theDoor[i];
			theDoor[i] = NULL;
		}
	}

	if (UIManagerDifficultySelection)
	{
		delete UIManagerDifficultySelection;
		UIManagerDifficultySelection = NULL;
	}
	if (UIManagerJellybeanSelection)
	{
		delete UIManagerJellybeanSelection;
		UIManagerJellybeanSelection = NULL;
	}
	if (UIManagerBlackQuad)
	{
		delete UIManagerBlackQuad;
		UIManagerBlackQuad = NULL;
	}
	if (UIManagerConfirmation)
	{
		delete UIManagerConfirmation;
		UIManagerConfirmation = NULL;
	}
}

void CSceneHub::Init(int level)
{
	UI_Speed = 20.0f;

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
	m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 13 * tileSize, 16 * tileSize, tileSize);
	m_cMap->LoadMap("Image//Maps/HubMap.csv");

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
				theHero = new CPlayerInfo(m_cMap);
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


	theDoor.push_back(new CDoor(CObjects::DOOR, 1, Vector3(sceneManager2D.m_window_width / 4, sceneManager2D.m_window_height / 1.5, 0), Vector3(50, 50, 50), meshList[GEO_TILE_DOOR]));
	theDoor.push_back(new CDoor(CObjects::DOOR, 2, Vector3(sceneManager2D.m_window_width / 1.5, sceneManager2D.m_window_height / 1.5, 0), Vector3(50, 50, 50), meshList[GEO_TILE_DOOR]));
	theDoor.push_back(new CDoor(CObjects::DOOR, 3, Vector3(sceneManager2D.m_window_width / 4, sceneManager2D.m_window_height / 5, 0), Vector3(50, 50, 50), meshList[GEO_TILE_DOOR]));
	theDoor.push_back(new CDoor(CObjects::DOOR, 4, Vector3(sceneManager2D.m_window_width / 1.5, sceneManager2D.m_window_height / 5, 0), Vector3(50, 50, 50), meshList[GEO_TILE_DOOR]));
	

	// UI
	// AlphaQuad
	UIManagerBlackQuad = new UISystem();

	Image* AlphaQuad;
	AlphaQuad = new Image("AlphaQuad", meshList[GEO_ALPHA_BLACK_QUAD], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), Vector3(0, 0, 0));
	UIManagerBlackQuad->addFeature(AlphaQuad);

	// Difficulty
	UIManagerDifficultySelection = new UISystem();

	// Background
	Image* SelectionBackground;
	SelectionBackground = new Image("SelectionBackground", meshList[GEO_SELECTION_BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), Vector3(0, 0, 0));
	UIManagerDifficultySelection->addFeature(SelectionBackground);

	// Difficulty
	// header
	Image* DifficultyHeader;
	DifficultyHeader = new Image("DifficultyHeader", meshList[GEO_DIFFICULTY], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0), Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 * 0.25, 0));
	UIManagerDifficultySelection->addFeature(DifficultyHeader);

	// Buttons
	// Tutorial button
	Button* TutorialButton;
	TutorialButton = new Button("TutorialButton", meshList[GEO_TUTORIAL_BUTTON_UP], meshList[GEO_TUTORIAL_BUTTON_DOWN], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManagerDifficultySelection->addFeature(TutorialButton);

	// Easy Button
	Button* EasyButton;
	EasyButton = new Button("EasyButton", meshList[GEO_EASY_BUTTON_UP], meshList[GEO_EASY_BUTTON_DOWN], meshList[GEO_EASY_BUTTON_LOCKED], Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManagerDifficultySelection->addFeature(EasyButton);

	// Medium Button
	Button* MediumButton;
	MediumButton = new Button("MediumButton", meshList[GEO_MEDIUM_BUTTON_UP], meshList[GEO_MEDIUM_BUTTON_DOWN], meshList[GEO_MEDIUM_BUTTON_LOCKED], Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManagerDifficultySelection->addFeature(MediumButton);

	// Hard Button
	Button* HardButton;
	HardButton = new Button("HardButton", meshList[GEO_HARD_BUTTON_UP], meshList[GEO_HARD_BUTTON_DOWN], meshList[GEO_HARD_BUTTON_LOCKED], Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManagerDifficultySelection->addFeature(HardButton);

	// Back button
	Button* BackButton;
	BackButton = new Button("BackButton", meshList[GEO_BACK_BUTTON_UP], meshList[GEO_BACK_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.275, sceneManager2D.m_window_height * 0.275, 0), Vector3(0, 0, 0));
	UIManagerDifficultySelection->addFeature(BackButton);


	// Jellybean
	UIManagerJellybeanSelection = new UISystem();

	// Background
	SelectionBackground = new Image("SelectionBackground", meshList[GEO_SELECTION_BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), Vector3(0, 0, 0));
	UIManagerJellybeanSelection->addFeature(SelectionBackground);

	// Header
	Image* JellybeanHeader;
	JellybeanHeader = new Image("JellybeanHeader", meshList[GEO_JELLYBEAN_HEADER], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0), Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 * 0.25, 0));
	UIManagerJellybeanSelection->addFeature(JellybeanHeader);

	// Buttons
	// Up Arrow button
	Button* UpArrowButton;
	UpArrowButton = new Button("UpArrowButton", meshList[GEO_UPARROW_BUTTON_UP], meshList[GEO_UPARROW_BUTTON_DOWN], meshList[GEO_UPARROW_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.7, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), Vector3(sceneManager2D.m_window_height * 0.5 * 0.2, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManagerJellybeanSelection->addFeature(UpArrowButton);

	// Down Arrow button
	Button* DownArrowButton;
	DownArrowButton = new Button("DownArrowButton", meshList[GEO_DOWNARROW_BUTTON_UP], meshList[GEO_DOWNARROW_BUTTON_DOWN], meshList[GEO_DOWNARROW_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.3, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), Vector3(sceneManager2D.m_window_height * 0.5 * 0.2, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManagerJellybeanSelection->addFeature(DownArrowButton);


	// Enter Arrow button
	Button* EnterButton;
	EnterButton = new Button("EnterButton", meshList[GEO_ENTER_BUTTON_UP], meshList[GEO_ENTER_BUTTON_DOWN], meshList[GEO_ENTER_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.5, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.25, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManagerJellybeanSelection->addFeature(EnterButton);

	// Back button
	BackButton = new Button("BackButton", meshList[GEO_BACK_BUTTON_UP], meshList[GEO_BACK_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.275, sceneManager2D.m_window_height * 0.275, 0), Vector3(0, 0, 0));
	UIManagerJellybeanSelection->addFeature(BackButton);

	// Confiramtion
	UIManagerConfirmation = new UISystem();

	// Confirmation Window
	Image* ConfirmationWindow;
	ConfirmationWindow = new Image("ConfirmationWindow", meshList[GEO_CONFIRMATION_WINDOW], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.45, 0));
	UIManagerConfirmation->addFeature(ConfirmationWindow);

	// Yes button
	Button* YesButton;
	YesButton = new Button("YesButton", meshList[GEO_YES_BUTTON_UP], meshList[GEO_YES_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.1, 0));
	UIManagerConfirmation->addFeature(YesButton);

	// No button
	Button* NoButton;
	NoButton = new Button("NoButton", meshList[GEO_NO_BUTTON_UP], meshList[GEO_NO_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.2, sceneManager2D.m_window_height * 0.1, 0));
	UIManagerConfirmation->addFeature(NoButton);
}

void CSceneHub::PreInit()
{
	sceneManager2D.PreInit();
}

/********************************************************************************
Initialise the meshes. This is a private function for use in this class only
********************************************************************************/
void CSceneHub::InitMeshes()
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

	// UI
	// Difficulty
	// Difficulty Header
	meshList[GEO_DIFFICULTY] = MeshBuilder::GenerateQuad("GEO_DIFFICULTY", Color(0, 0, 0), 1.f);
	meshList[GEO_DIFFICULTY]->textureID = LoadTGA("Image//UI/difficulty.tga");
	// Buttons
	// Tutorial
	meshList[GEO_TUTORIAL_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_TUTORIAL_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_TUTORIAL_BUTTON_UP]->textureID = LoadTGA("Image//UI/Tutorial_Button.tga");
	meshList[GEO_TUTORIAL_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_TUTORIAL_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_TUTORIAL_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Tutorial_Button_Pressed.tga");
	// Easy
	meshList[GEO_EASY_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_EASY_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_EASY_BUTTON_UP]->textureID = LoadTGA("Image//UI/Easy_Button.tga");
	meshList[GEO_EASY_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_EASY_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_EASY_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Easy_Button_Pressed.tga");
	meshList[GEO_EASY_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_EASY_BUTTON_LOCKED", Color(0, 0, 0), 1.f);
	meshList[GEO_EASY_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Easy_Button_Locked.tga");
	// Medium
	meshList[GEO_MEDIUM_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_MEDIUM_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_MEDIUM_BUTTON_UP]->textureID = LoadTGA("Image//UI/Medium_Button.tga");
	meshList[GEO_MEDIUM_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_MEDIUM_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_MEDIUM_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Medium_Button_Pressed.tga");
	meshList[GEO_MEDIUM_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_MEDIUM_BUTTON_LOCKED", Color(0, 0, 0), 1.f);
	meshList[GEO_MEDIUM_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Medium_Button_Locked.tga");
	// Hard
	meshList[GEO_HARD_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_HARD_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_HARD_BUTTON_UP]->textureID = LoadTGA("Image//UI/Hard_Button.tga");
	meshList[GEO_HARD_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_HARD_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_HARD_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Hard_Button_Pressed.tga");
	meshList[GEO_HARD_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_HARD_BUTTON_LOCKED", Color(0, 0, 0), 1.f);
	meshList[GEO_HARD_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Hard_Button_Locked.tga");

	// Jellybean
	// Jellybean Header
	meshList[GEO_JELLYBEAN_HEADER] = MeshBuilder::GenerateQuad("GEO_JELLYBEAN_HEADER", Color(0, 0, 0), 1.f);
	meshList[GEO_JELLYBEAN_HEADER]->textureID = LoadTGA("Image//UI/JellybeanHeader.tga");
	// Buttons
	// Up arrow
	meshList[GEO_UPARROW_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_UPARROW_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_UPARROW_BUTTON_UP]->textureID = LoadTGA("Image//UI/Right_Button.tga");
	meshList[GEO_UPARROW_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_UPARROW_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_UPARROW_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Right_Button_Pressed.tga");
	meshList[GEO_UPARROW_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_UPARROW_BUTTON_LOCKED", Color(0, 0, 0), 1.f);
	meshList[GEO_UPARROW_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Right_Button_Locked.tga");
	// Down arrow
	meshList[GEO_DOWNARROW_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_DOWNARROW_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_DOWNARROW_BUTTON_UP]->textureID = LoadTGA("Image//UI/Left_Button.tga");
	meshList[GEO_DOWNARROW_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_DOWNARROW_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_DOWNARROW_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Left_Button_Pressed.tga");
	meshList[GEO_DOWNARROW_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_DOWNARROW_BUTTON_LOCKED", Color(0, 0, 0), 1.f);
	meshList[GEO_DOWNARROW_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Left_Button_Locked.tga");
	// Enter
	meshList[GEO_ENTER_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_ENTER_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_ENTER_BUTTON_UP]->textureID = LoadTGA("Image//UI/Confirm_Button.tga");
	meshList[GEO_ENTER_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_ENTER_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_ENTER_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Confirm_Button_Pressed.tga");
	meshList[GEO_ENTER_BUTTON_LOCKED] = MeshBuilder::GenerateQuad("GEO_ENTER_BUTTON_LOCKED", Color(0, 0, 0), 1.f);
	meshList[GEO_ENTER_BUTTON_LOCKED]->textureID = LoadTGA("Image//UI/Confirm_Button_Locked.tga");

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


	// Selection Background
	meshList[GEO_SELECTION_BACKGROUND] = MeshBuilder::GenerateQuad("GEO_SELECTION_BACKGROUND", Color(0, 0, 0), 1.f);
	meshList[GEO_SELECTION_BACKGROUND]->textureID = LoadTGA("Image//UI/selection_background.tga");

	// Alpha Black Quad
	meshList[GEO_ALPHA_BLACK_QUAD] = MeshBuilder::GenerateQuad("GEO_ALPHA_BLACK_QUAD", Color(1, 1, 1), 1);
	meshList[GEO_ALPHA_BLACK_QUAD]->textureID = LoadTGA("Image//UI/Half_Alpha_Black.tga");

	// Back Button
	meshList[GEO_BACK_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_BACK_BUTTON_UP", Color(1, 1, 1), 1);
	meshList[GEO_BACK_BUTTON_UP]->textureID = LoadTGA("Image//UI/Back_Button.tga");
	meshList[GEO_BACK_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_BACK_BUTTON_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_BACK_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Back_Button_Pressed.tga");
}

void CSceneHub::Update(double dt)
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

	if (currentState == PLAYING)
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
	}

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

	if (theDoor[0]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (theDoor[0]->getId() == 1)
		{
			game_interacted = GAME1;
		}
	}
	else if (theDoor[1]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (theDoor[1]->getId() == 2)
		{
			game_interacted = GAME2;
		}
	}
	else if (theDoor[2]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (theDoor[2]->getId() == 3)
		{
			game_interacted = GAME3;
		}
	}
	else if (theDoor[3]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		if (theDoor[3]->getId() == 4)
		{
			game_interacted = GAME4;
		}
	}
	else
	{
		game_interacted = NO_GAME;
	}

	// UI
	UpdateUI(dt);
}

/********************************************************************************
Update UI
********************************************************************************/
void CSceneHub::UpdateUI(double dt)
{
	UIManagerDifficultySelection->Update(Application::mouse_current_x, Application::mouse_current_y, dt);
	UIManagerJellybeanSelection->Update(Application::mouse_current_x, Application::mouse_current_y, dt);
	UIManagerBlackQuad->Update(Application::mouse_current_x, Application::mouse_current_y, dt);
	UIManagerConfirmation->Update(Application::mouse_current_x, Application::mouse_current_y, dt);
}

/********************************************************************************
Change UI - playing
********************************************************************************/
void CSceneHub::ChangeUI_Playing()
{
	UIManagerDifficultySelection->InvokeAnimator()->StopAnimations();
	UIManagerJellybeanSelection->InvokeAnimator()->StopAnimations();
	UIManagerConfirmation->InvokeAnimator()->StopAnimations();
	UIManagerBlackQuad->InvokeAnimator()->StopAnimations();
	Hide_DifficultySelection();
	// AlphaQuad
	UIManagerBlackQuad->InvokeAnimator()->StartTransformation(UIManagerBlackQuad->FindImage("AlphaQuad"), 0, Vector3(0, 0, 0), UI_Speed * 2, UIAnimation::SCALING);
}
/********************************************************************************
Change UI - difficulty selection
********************************************************************************/
void CSceneHub::ChangeUI_DifficultySelection()
{
	UIManagerDifficultySelection->InvokeAnimator()->StopAnimations();
	UIManagerJellybeanSelection->InvokeAnimator()->StopAnimations();
	UIManagerConfirmation->InvokeAnimator()->StopAnimations();
	UIManagerBlackQuad->InvokeAnimator()->StopAnimations();
	HideConfirmation();
	Hide_JellybeanSelection();
	Show_DifficultySelection();
	// AlphaQuad
	UIManagerBlackQuad->InvokeAnimator()->StartTransformation(UIManagerBlackQuad->FindImage("AlphaQuad"), 0, Vector3(sceneManager2D.m_window_width, sceneManager2D.m_window_height, 0), UI_Speed * 2, UIAnimation::SCALING);
}
/********************************************************************************
Change UI - jellybean selection
********************************************************************************/
void CSceneHub::ChangeUI_JellybeanSelection()
{
	UIManagerDifficultySelection->InvokeAnimator()->StopAnimations();
	UIManagerJellybeanSelection->InvokeAnimator()->StopAnimations();
	UIManagerConfirmation->InvokeAnimator()->StopAnimations();
	UIManagerBlackQuad->InvokeAnimator()->StopAnimations();
	Hide_DifficultySelection();
	Show_JellybeanSelection();
	HideConfirmation();
}
/********************************************************************************
Change UI - Confirmation
********************************************************************************/
void CSceneHub::ChangeUI_Confirmation()
{
	UIManagerDifficultySelection->InvokeAnimator()->StopAnimations();
	UIManagerJellybeanSelection->InvokeAnimator()->StopAnimations();
	UIManagerConfirmation->InvokeAnimator()->StopAnimations();
	UIManagerBlackQuad->InvokeAnimator()->StopAnimations();
	ShowConfirmation();
}

/********************************************************************************
Show Difficulty Selection
********************************************************************************/
void CSceneHub::Show_DifficultySelection()
{
	// Difficulty Header
	if (UIManagerDifficultySelection->FindImage("DifficultyHeader")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 2.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("DifficultyHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 2.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Tutorial Button
	if (UIManagerDifficultySelection->FindButton("TutorialButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("TutorialButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Easy Button
	if (UIManagerDifficultySelection->FindButton("EasyButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("EasyButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Medium Button
	if (UIManagerDifficultySelection->FindButton("MediumButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("MediumButton"), 0.15, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Hard Button
	if (UIManagerDifficultySelection->FindButton("HardButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("HardButton"), 0.2, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}

	// Selection background
	if (UIManagerDifficultySelection->FindImage("SelectionBackground")->getScale() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("SelectionBackground"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::SCALING);
	}

	// Back Button
	if (UIManagerDifficultySelection->FindButton("BackButton")->getScale() != Vector3(sceneManager2D.m_window_width * 0.125, sceneManager2D.m_window_width * 0.125, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("BackButton"), 0.25, Vector3(sceneManager2D.m_window_width * 0.125, sceneManager2D.m_window_width * 0.125, 0), UI_Speed, UIAnimation::SCALING);
	}
}

/********************************************************************************
Hide Difficulty Selection
********************************************************************************/
void CSceneHub::Hide_DifficultySelection()
{
	// Difficulty Header
	if (UIManagerDifficultySelection->FindImage("DifficultyHeader")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("DifficultyHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Tutorial Button
	if (UIManagerDifficultySelection->FindButton("TutorialButton")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("TutorialButton"), 0.05, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Easy Button
	if (UIManagerDifficultySelection->FindButton("EasyButton")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("EasyButton"), 0.1, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Medium Button
	if (UIManagerDifficultySelection->FindButton("MediumButton")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("MediumButton"), 0.15, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Hard Button
	if (UIManagerDifficultySelection->FindButton("HardButton")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("HardButton"), 0.2, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}

	// Selection background
	if (UIManagerDifficultySelection->FindImage("SelectionBackground")->getScale() != Vector3(0, 0, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("SelectionBackground"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);
	}

	// Back Button
	if (UIManagerDifficultySelection->FindButton("BackButton")->getScale() != Vector3(0, 0, 0))
	{
		UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("BackButton"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);
	}
}

/********************************************************************************
Show Difficulty Selection
********************************************************************************/
void CSceneHub::Show_JellybeanSelection()
{
	// Jellybean Header
	if (UIManagerJellybeanSelection->FindImage("JellybeanHeader")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 2.5, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("JellybeanHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 2.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Up Arrow Button
	if (UIManagerJellybeanSelection->FindButton("UpArrowButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.7, sceneManager2D.m_window_height * 0.5, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("UpArrowButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.7, sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Down Arrow Button
	if (UIManagerJellybeanSelection->FindButton("DownArrowButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.3, sceneManager2D.m_window_height * 0.5, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("DownArrowButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.3, sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Enter Button
	if (UIManagerJellybeanSelection->FindButton("EnterButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.3, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("EnterButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.3, 0), UI_Speed, UIAnimation::TRANSLATION);
	}

	// Selection background
	if (UIManagerJellybeanSelection->FindImage("SelectionBackground")->getScale() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("SelectionBackground"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::SCALING);
	}

	// Back Button
	if (UIManagerJellybeanSelection->FindButton("BackButton")->getScale() != Vector3(sceneManager2D.m_window_width * 0.125, sceneManager2D.m_window_width * 0.125, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("BackButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.125, sceneManager2D.m_window_width * 0.125, 0), UI_Speed, UIAnimation::SCALING);
	}
}

/********************************************************************************
Hide Jellybean Selection
********************************************************************************/
void CSceneHub::Hide_JellybeanSelection()
{
	// Jellybean Header
	if (UIManagerJellybeanSelection->FindImage("JellybeanHeader")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("JellybeanHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Up Arrow Button
	if (UIManagerJellybeanSelection->FindButton("UpArrowButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.7, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("UpArrowButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.7, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Down Arrow Button
	if (UIManagerJellybeanSelection->FindButton("DownArrowButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.3, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("DownArrowButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.3, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}
	// Enter Button
	if (UIManagerJellybeanSelection->FindButton("EnterButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("EnterButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.5, -sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	}

	// Selection background
	if (UIManagerJellybeanSelection->FindImage("SelectionBackground")->getScale() != Vector3(0, 0, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("SelectionBackground"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);
	}

	// Back Button
	if (UIManagerJellybeanSelection->FindButton("BackButton")->getScale() != Vector3(0, 0, 0))
	{
		UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("BackButton"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);
	}
}

/********************************************************************************
Show Confirmation
********************************************************************************/
void CSceneHub::ShowConfirmation()
{
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.65, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.385, sceneManager2D.m_window_height * 0.3, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.615, sceneManager2D.m_window_height * 0.3, 0), UI_Speed, UIAnimation::TRANSLATION);
}

/********************************************************************************
Hide Confirmation
********************************************************************************/
void CSceneHub::HideConfirmation()
{
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 1.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.45, -sceneManager2D.m_window_height * 0.5, 0), UI_Speed, UIAnimation::TRANSLATION);
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneHub::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneHub::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}


/********************************************************************************
Render this scene
********************************************************************************/
void CSceneHub::Render()
{
	sceneManager2D.Render();

	sceneManager2D.modelStack.PushMatrix();

	sceneManager2D.modelStack.Translate(-theHero->GetMapOffset_x(), theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);


	//sceneManager2D.RenderBackground();

	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	for (int i = 0; i < theDoor.size(); i++)
	{
		sceneManager2D.Render2DMesh(theDoor[i]->getMesh(), false, theDoor[i]->getScale().x, theDoor[i]->getScale().y, theDoor[i]->getPositionX(), theDoor[i]->getPositionY());
	}
	// Render Hero
	RenderHero();
	// Render AIs
	RenderAIs();
	//Render Waypoints
	//RenderWaypoints();
	// Render the goodies
	//RenderGoodies();


	sceneManager2D.modelStack.PopMatrix();

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
	sceneManager2D.Render2DMesh(meshList[GEO_JELLYBEAN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());
	ss.str(std::string());
	ss.precision(3);
	ss << ": " << noOfJellybeans;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), m_cMap->GetTileSize(), m_cMap->GetTileSize(), sceneManager2D.m_window_height - m_cMap->GetTileSize());

	// UI
	switch (currentState)
	{
	case PLAYING:
		UIManagerBlackQuad->Render(sceneManager2D);
		UIManagerDifficultySelection->Render(sceneManager2D);
		UIManagerJellybeanSelection->Render(sceneManager2D);
		UIManagerConfirmation->Render(sceneManager2D);
		break;
	case DIFFICULTY_SELECTION:
		UIManagerBlackQuad->Render(sceneManager2D);
		UIManagerDifficultySelection->Render(sceneManager2D);
		UIManagerJellybeanSelection->Render(sceneManager2D);
		UIManagerConfirmation->Render(sceneManager2D);
		break;
	case JELLYBEAN_SELECTION:
		UIManagerBlackQuad->Render(sceneManager2D);
		UIManagerDifficultySelection->Render(sceneManager2D);
		UIManagerJellybeanSelection->Render(sceneManager2D);
		UIManagerConfirmation->Render(sceneManager2D);
		break;
	case CONFIRMATION:
		UIManagerDifficultySelection->Render(sceneManager2D);
		UIManagerJellybeanSelection->Render(sceneManager2D);
		UIManagerBlackQuad->Render(sceneManager2D);
		UIManagerConfirmation->Render(sceneManager2D);
		break;
	}

	if (currentState == JELLYBEAN_SELECTION)
	{
		if (noOfJellybeansDeposited >= 10)
		{
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], to_string(noOfJellybeansDeposited), Color(1, 1, 1), m_cMap->GetTileSize() * 2, sceneManager2D.m_window_width * 0.5 - m_cMap->GetTileSize() * 1.75, sceneManager2D.m_window_height * 0.5 - m_cMap->GetTileSize());
		}
		else
		{
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], to_string(noOfJellybeansDeposited), Color(1, 1, 1), m_cMap->GetTileSize() * 2, sceneManager2D.m_window_width * 0.5 - m_cMap->GetTileSize(), sceneManager2D.m_window_height * 0.5 - m_cMap->GetTileSize());
		}
	}
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CSceneHub::Exit()
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
void CSceneHub::RenderTileMap()
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			if (m_cMap->theScreenMap[i][k] == 1)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_WALL], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][k] == 2)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILETREE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - (i + 1)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][k] == 10)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i *m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][k] == 11)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_SAFEZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			}
			//else if (m_cMap->theScreenMap[i][k] == 30)
			//{
			//	sceneManager2D.Render2DMesh(meshList[GEO_TILE_DOOR], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize());
			//}
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
void CSceneHub::RenderHero()
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
void CSceneHub::RenderAIs()
{
	// Render the enemy
	for (int i = 0; i < theEnemies.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theEnemies[i]->GetPos_x(), theEnemies[i]->GetPos_y());
	}
}

/********************************************************************************
Render the rear tile map. This is a private function for use in this class only
********************************************************************************/
void CSceneHub::RenderRearTileMap()
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
void CSceneHub::RenderGoodies()
{
	// Render the goodies
	for (int i = 0; i<10; i++)
	{
		sceneManager2D.Render2DMesh(theArrayOfGoodies[i]->GetMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theArrayOfGoodies[i]->GetPos_x(), theArrayOfGoodies[i]->GetPos_y());
	}
}

void CSceneHub::RenderWaypoints()
{
	for (int i = 0; i < temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), temp.at(i).x, temp.at(i).y);
	}
}