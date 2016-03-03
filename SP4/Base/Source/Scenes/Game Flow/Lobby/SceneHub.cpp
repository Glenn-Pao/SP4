#include "SceneHub.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"
#include "..\..\..\AI\ProbabilitySystem.h"

#include "..\..\..\UsingLua.h"

CSceneHub::CSceneHub(const int m_window_width, const int m_window_height)
	: currentState(PRESTART)
	, UIManagerDifficultySelection(NULL)
	, UIManagerJellybeanSelection(NULL)
	, UIManagerBlackQuad(NULL)
	, UIManagerConfirmation(NULL)
	, UIManagerGivingJellybeans(NULL)
	, game_interacted(NO_GAME)
	, noOfJellybeansDeposited(0)
	, difficultySelected(0)
	, UI_Speed(0.f)
	, targetNPC(NULL)
	, jellybeansRequiredToFinish(100)
	, sizeOfDarkSurrounding(0)
	, timerForEnd(0.0f)
	, guardianCleared(false)
	, GameoverText(NULL)
	, speedOfGameoverText(0.f)
	, Credit(NULL)
{
}

CSceneHub::~CSceneHub()
{
	 // Door
	for (int i = 0; i < (int)theDoor.size(); i++)
	{
		if (theDoor[i])
		{
			delete theDoor[i];
			theDoor[i] = NULL;
		}
	}
	// UI
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
	if (UIManagerGivingJellybeans)
	{
		delete UIManagerGivingJellybeans;
		UIManagerGivingJellybeans = NULL;
	}
	// Guardian
	for (int i = 0; i < (int)theNPCs.size(); i++)
	{
		if (theNPCs[i])
		{
			delete theNPCs[i];
			theNPCs[i] = NULL;
		}
	}
	// Gameover
	if (GameoverText)
	{
		delete GameoverText;
		GameoverText = NULL;
	}
	// Credit
	if (Credit)
	{
		delete Credit;
		Credit = NULL;
	}
}

void CSceneHub::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//Scene/GameHub.lua");

	int tileSize = L.DoLuaInt("tileSize");
	jellybeansRequiredToFinish = L.DoLuaInt("noOfJellybeansRequiredToFinish");
	sizeOfDarkSurrounding = tileSize * L.DoLuaFloat("sizeOfScalingDarkSurrounding");
	speedOfDarkSurrounding = L.DoLuaFloat("speedOfScalingDarkSurrounding");
	MinSizeOfScalingDarkSurrounding = L.DoLuaFloat("minSizeOfScalingDarkSurrounding");
	timerForEnd = L.DoLuaFloat("timerForEnd");
	speedOfGameoverText = L.DoLuaFloat("SpeedOfGameoverText");

	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 13 * tileSize, 16 * tileSize, tileSize);
	m_cMap->LoadMap("Image//Maps/HubMap.csv");

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

	// Find object positions in map 
	Vector3 GuardianPos;
	const int numOfGuardianWaypointPos = 2;
	Vector3 GuardianWaypointPos[numOfGuardianWaypointPos];
	Vector3 TipGiverPos;
	Vector3 DoorPos[5];
	int currentDoorIndex = 0;
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			// Hero
			if (m_cMap->theScreenMap[i][k] == 1000 && theHero == NULL)
			{
				// Initialise the hero's position
				theHero = new CPlayerInfo(m_cMap);
				theHero->setPositionX((float)k*m_cMap->GetTileSize());
				theHero->setPositionY((float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
			}
			// Guardian
			else if (m_cMap->theScreenMap[i][k] == 100)
			{
				GuardianPos.Set((float)k*m_cMap->GetTileSize(), (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
			}
			// Door
			else if (m_cMap->theScreenMap[i][k] == 20)
			{
				DoorPos[currentDoorIndex].Set((float)k*m_cMap->GetTileSize(), (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
				currentDoorIndex++;
			}
			// Guardian waypoint
			else if (m_cMap->theScreenMap[i][k] == 101)
			{
				GuardianWaypointPos[0] = Vector3((float)k*m_cMap->GetTileSize(), (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][k] == 102)
			{
				GuardianWaypointPos[1] = Vector3((float)k*m_cMap->GetTileSize(), (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
			}
			// Tip Giver
			else if (m_cMap->theScreenMap[i][k] == 150)
			{
				TipGiverPos = Vector3((float)k*m_cMap->GetTileSize(), (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
			}
		}
	}

	// Initialise the Meshes
	InitMeshes();

	// Guardian
	theNPCs.push_back(new CAI_Idling(CObjects::AI, Vector3(GuardianPos.x, GuardianPos.y), Vector3((float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize()), meshList[GEO_GUARDIAN], CAI_Idling::GUARDIAN));
	theNPCs.back()->setDialogue(L.DoLuaString("GuardianScript"));
	theNPCs.back()->AddWaypoint(GuardianPos);
	for (int i = 0; i < numOfGuardianWaypointPos; i++)
		theNPCs.back()->AddWaypoint(GuardianWaypointPos[i]);
	// Tip Giver
	theNPCs.push_back(new CAI_Idling(CObjects::AI, Vector3(TipGiverPos.x, TipGiverPos.y), Vector3((float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize()), meshList[GEO_GREEN_MONSTER], CAI_Idling::TIP_ADVISOR));
	ChangeTip();

	// Exit
	theDoor.push_back(new CDoor(CObjects::DOOR, 5, Vector3(DoorPos[0].x, DoorPos[0].y), Vector3((float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize()), meshList[GEO_TILE_DOOR]));
	theDoor.back()->setDialogue(L.DoLuaString("door5"));

	// Doors
	theDoor.push_back(new CDoor(CObjects::DOOR, 1, Vector3(DoorPos[1].x, DoorPos[1].y), Vector3((float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize()), meshList[GEO_TILE_DOOR]));
	theDoor.back()->setDialogue(L.DoLuaString("door1"));
	theDoor.push_back(new CDoor(CObjects::DOOR, 2, Vector3(DoorPos[2].x, DoorPos[2].y), Vector3((float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize()), meshList[GEO_TILE_DOOR]));
	theDoor.back()->setDialogue(L.DoLuaString("door2"));
	theDoor.push_back(new CDoor(CObjects::DOOR, 3, Vector3(DoorPos[3].x, DoorPos[3].y), Vector3((float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize()), meshList[GEO_TILE_DOOR]));
	theDoor.back()->setDialogue(L.DoLuaString("door3"));
	theDoor.push_back(new CDoor(CObjects::DOOR, 4, Vector3(DoorPos[4].x, DoorPos[4].y), Vector3((float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize()), meshList[GEO_TILE_DOOR]));
	theDoor.back()->setDialogue(L.DoLuaString("door4"));
	

	// UI
	InitUI();

	// Create Gameover Object
	float textSize = m_cMap->GetTileSize() * 2;
	string gameoverText = L.DoLuaString("GameoverText");
	GameoverText = new CObjects(CObjects::DIALOGUE, true, gameoverText, Vector3(sceneManager2D.m_window_width * 0.5f - (gameoverText.size() - 2.75f) * 0.5f * textSize, -textSize * 1.1f), Vector3(), Vector3(textSize, textSize, textSize), NULL);
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
	meshList[GEO_DARK_SURROUNDING] = MeshBuilder::GenerateQuad("GEO_DARK_SURROUNDING", Color(1, 1, 1), 1);
	meshList[GEO_DARK_SURROUNDING]->textureID = LoadTGA("Image//DarkSurrounding.tga");
	meshList[GEO_BLACK_QUAD] = MeshBuilder::GenerateQuad("GEO_BLACK_QUAD", Color(0, 0, 0), 1);
	meshList[GEO_DIALOGUE_BOX] = MeshBuilder::Generate2DMesh("GEO_DIALOGUE_BOX", Color(1, 0.8f, 0.8f), 0, 0, 1, 1);
	//meshList[GEO_DIALOGUE_BOX]->textureID = LoadTGA("Image//dialogue_box.tga");
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

	// Guardian
	meshList[GEO_GUARDIAN] = MeshBuilder::Generate2DMesh("GEO_GUARDIAN", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_GUARDIAN]->textureID = LoadTGA("Image//guardian.tga");

	// Green Monster
	meshList[GEO_GREEN_MONSTER] = MeshBuilder::Generate2DMesh("GEO_GREEN_MONSTER", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_GREEN_MONSTER]->textureID = LoadTGA("Image//monsterGreen.tga");

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

	// Giving jellybeans
	// Window
	meshList[GEO_GIVE_JELLYBEANS_WINDOW] = MeshBuilder::GenerateQuad("GEO_GIVE_JELLYBEANS_WINDOW", Color(1, 1, 1), 1);
	meshList[GEO_GIVE_JELLYBEANS_WINDOW]->textureID = LoadTGA("Image//UI/GiveJellybeans_Window.tga");

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


/********************************************************************************
Initialise the UIs. This is a private function for use in this class only
********************************************************************************/
void CSceneHub::InitUI()
{
	// Animation speed
	UI_Speed = 20.0f;
	/*AlphaQuad*/
	UIManagerBlackQuad = new UISystem();

	Image* AlphaQuad;
	AlphaQuad = new Image("AlphaQuad", meshList[GEO_ALPHA_BLACK_QUAD], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
	UIManagerBlackQuad->addFeature(AlphaQuad);

	/*Difficulty*/
	UIManagerDifficultySelection = new UISystem();

	// Background
	Image* SelectionBackground;
	SelectionBackground = new Image("SelectionBackground", meshList[GEO_SELECTION_BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
	UIManagerDifficultySelection->addFeature(SelectionBackground);

	// Header
	Image* DifficultyHeader;
	DifficultyHeader = new Image("DifficultyHeader", meshList[GEO_DIFFICULTY], Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 100, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f * 0.25f, 0));
	UIManagerDifficultySelection->addFeature(DifficultyHeader);

	// Buttons
	// Tutorial button
	Button* TutorialButton;
	TutorialButton = new Button("TutorialButton", meshList[GEO_TUTORIAL_BUTTON_UP], meshList[GEO_TUTORIAL_BUTTON_DOWN], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), Vector3(sceneManager2D.m_window_width * 0.5f * 0.5f, sceneManager2D.m_window_height * 0.5f * 0.2f, 0));
	UIManagerDifficultySelection->addFeature(TutorialButton);

	// Easy Button
	Button* EasyButton;
	EasyButton = new Button("EasyButton", meshList[GEO_EASY_BUTTON_UP], meshList[GEO_EASY_BUTTON_DOWN], meshList[GEO_EASY_BUTTON_LOCKED], Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 0.5f, 0), Vector3(sceneManager2D.m_window_width * 0.5f * 0.5f, sceneManager2D.m_window_height * 0.5f * 0.2f, 0));
	UIManagerDifficultySelection->addFeature(EasyButton);

	// Medium Button
	Button* MediumButton;
	MediumButton = new Button("MediumButton", meshList[GEO_MEDIUM_BUTTON_UP], meshList[GEO_MEDIUM_BUTTON_DOWN], meshList[GEO_MEDIUM_BUTTON_LOCKED], Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * -0.5f, 0), Vector3(sceneManager2D.m_window_width * 0.5f * 0.5f, sceneManager2D.m_window_height * 0.5f * 0.2f, 0));
	UIManagerDifficultySelection->addFeature(MediumButton);

	// Hard Button
	Button* HardButton;
	HardButton = new Button("HardButton", meshList[GEO_HARD_BUTTON_UP], meshList[GEO_HARD_BUTTON_DOWN], meshList[GEO_HARD_BUTTON_LOCKED], Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * -1.5f, 0), Vector3(sceneManager2D.m_window_width * 0.5f * 0.5f, sceneManager2D.m_window_height * 0.5f * 0.2f, 0));
	UIManagerDifficultySelection->addFeature(HardButton);

	// Back button
	Button* BackButton;
	BackButton = new Button("BackButton", meshList[GEO_BACK_BUTTON_UP], meshList[GEO_BACK_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.275f, sceneManager2D.m_window_height * 0.275f, 0), Vector3(0, 0, 0));
	UIManagerDifficultySelection->addFeature(BackButton);


	/*Jellybean*/
	UIManagerJellybeanSelection = new UISystem();

	// Background
	SelectionBackground = new Image("SelectionBackground", meshList[GEO_SELECTION_BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
	UIManagerJellybeanSelection->addFeature(SelectionBackground);

	// Header
	Image* JellybeanHeader;
	JellybeanHeader = new Image("JellybeanHeader", meshList[GEO_JELLYBEAN_HEADER], Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 100, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f * 0.25f, 0));
	UIManagerJellybeanSelection->addFeature(JellybeanHeader);

	// Buttons
	// Up Arrow button
	Button* UpArrowButton;
	UpArrowButton = new Button("UpArrowButton", meshList[GEO_UPARROW_BUTTON_UP], meshList[GEO_UPARROW_BUTTON_DOWN], meshList[GEO_UPARROW_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.7f, -sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), Vector3(sceneManager2D.m_window_height * 0.5f * 0.2f, sceneManager2D.m_window_height * 0.5f * 0.2f, 0));
	UIManagerJellybeanSelection->addFeature(UpArrowButton);

	// Down Arrow button
	Button* DownArrowButton;
	DownArrowButton = new Button("DownArrowButton", meshList[GEO_DOWNARROW_BUTTON_UP], meshList[GEO_DOWNARROW_BUTTON_DOWN], meshList[GEO_DOWNARROW_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.3f, -sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), Vector3(sceneManager2D.m_window_height * 0.5f * 0.2f, sceneManager2D.m_window_height * 0.5f * 0.2f, 0));
	UIManagerJellybeanSelection->addFeature(DownArrowButton);


	// Enter Arrow button
	Button* EnterButton;
	EnterButton = new Button("EnterButton", meshList[GEO_ENTER_BUTTON_UP], meshList[GEO_ENTER_BUTTON_DOWN], meshList[GEO_ENTER_BUTTON_LOCKED], Vector3(sceneManager2D.m_window_width * 0.5f, -sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), Vector3(sceneManager2D.m_window_width * 0.5f * 0.25f, sceneManager2D.m_window_height * 0.5f * 0.2f, 0));
	UIManagerJellybeanSelection->addFeature(EnterButton);

	// Back button
	BackButton = new Button("BackButton", meshList[GEO_BACK_BUTTON_UP], meshList[GEO_BACK_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.275f, sceneManager2D.m_window_height * 0.275f, 0), Vector3(0, 0, 0));
	UIManagerJellybeanSelection->addFeature(BackButton);

	/*Confiramtion*/
	UIManagerConfirmation = new UISystem();

	// Confirmation Window
	Image* ConfirmationWindow;
	ConfirmationWindow = new Image("ConfirmationWindow", meshList[GEO_CONFIRMATION_WINDOW], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 1.5f, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.45f, 0));
	UIManagerConfirmation->addFeature(ConfirmationWindow);

	// Yes button
	Button* YesButton;
	YesButton = new Button("YesButton", meshList[GEO_YES_BUTTON_UP], meshList[GEO_YES_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), Vector3(sceneManager2D.m_window_width * 0.2f, sceneManager2D.m_window_height * 0.1f, 0));
	UIManagerConfirmation->addFeature(YesButton);

	// No button
	Button* NoButton;
	NoButton = new Button("NoButton", meshList[GEO_NO_BUTTON_UP], meshList[GEO_NO_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), Vector3(sceneManager2D.m_window_width * 0.2f, sceneManager2D.m_window_height * 0.1f, 0));
	UIManagerConfirmation->addFeature(NoButton);

	/*Giving Jellybeans*/
	UIManagerGivingJellybeans = new UISystem();

	// Giving Jelybeans Window
	Image* GivingJelybeansWindow;
	GivingJelybeansWindow = new Image("GivingJelybeansWindow", meshList[GEO_GIVE_JELLYBEANS_WINDOW], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 1.5f, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.2f, 0));
	UIManagerGivingJellybeans->addFeature(GivingJelybeansWindow);

	// Yes button
	YesButton = new Button("YesButton", meshList[GEO_YES_BUTTON_UP], meshList[GEO_YES_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), Vector3(sceneManager2D.m_window_width * 0.2f, sceneManager2D.m_window_height * 0.1f, 0));
	UIManagerGivingJellybeans->addFeature(YesButton);

	// No button
	NoButton = new Button("NoButton", meshList[GEO_NO_BUTTON_UP], meshList[GEO_NO_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), Vector3(sceneManager2D.m_window_width * 0.2f, sceneManager2D.m_window_height * 0.1f, 0));
	UIManagerGivingJellybeans->addFeature(NoButton);
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

	switch (currentState)
	{
		case PRESTART:
		{
			if (noOfJellybeans <= 0)
			{
				currentState = LOSING;
			}
			else
			{
				currentState = PLAYING;
			}
		}
		break;
		case PLAYING:
		{
			Vector3 prevHeroPos = Vector3(theHero->getPositionX(), theHero->getPositionY());
			// Update the hero
			if (Application::IsKeyPressed('W'))
				this->theHero->MoveUpDown(true, (float)dt, m_cMap);
			if (Application::IsKeyPressed('S'))
				this->theHero->MoveUpDown(false, (float)dt, m_cMap);
			if (Application::IsKeyPressed('A'))
				this->theHero->MoveLeftRight(true, (float)dt, m_cMap);
			if (Application::IsKeyPressed('D'))
				this->theHero->MoveLeftRight(false, (float)dt, m_cMap);
			/*if (Application::IsKeyPressed(' '))
			this->theHero->SetToJumpUpwards(true);*/
			// Update Hero animation counter if hero moved
			if (prevHeroPos != Vector3(theHero->getPositionX(), theHero->getPositionY()))
			{
				theHero->SetAnimationCounter(theHero->GetAnimationCounter() + theHero->GetMovementSpeed() * m_cMap->GetTileSize() * (float)dt * theHero->GetAnimationSpeed());
				if (theHero->GetAnimationCounter() > theHero->GetAnimationMaxCounter())
					theHero->SetAnimationCounter(1);
			}
			else
			{
				theHero->SetAnimationCounter(0);
			}
			theHero->HeroUpdate(m_cMap, (float)dt);

			// Check Door
			for (int i = 0; i < (int)theDoor.size(); i++)
			{
				if (theDoor[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
				{
					game_interacted = NO_GAME + theDoor[i]->getId();
					break;
				}
				// No collide
				if (i == theDoor.size() - 1)
				{
					game_interacted = NO_GAME;
				}
			}

			// Update NPCs
			for (int i = 0; i < (int)theNPCs.size(); i++)
			{
				// Update those NPCs that are movable
				if (theNPCs[i]->GetAI_Type() == CAI_Idling::MOVINGAROUND)
				{
					theNPCs[i]->Update(dt, theHero, m_cMap);
				}

				// Check if the Hero collided with the NPCs
				if (theNPCs[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
				{
					theHero->setPosition(prevHeroPos);
				}

				// Check if the Hero can interact with NPC or not
				if (CheckIfThisNPCIsInteractable(theNPCs[i]) == true)
				{
					// Interactable
					theNPCs[i]->SetIfInteractable(true);
				}
				else
				{
					// Not interactable
					theNPCs[i]->SetIfInteractable(false);
				}
			}
			// Set target
			for (int i = 0; i < (int)theNPCs.size(); i++)
			{
				// Check if the Hero can interact with NPC or not
				if (theNPCs[i]->IfInteractable() == true)
				{
					targetNPC = theNPCs[i];
					break;
				}
				// if no target
				if (i == theNPCs.size() - 1)
				{
					targetNPC = NULL;
				}
			}
		}
		break;
		case CLEARING_WAY:
		{
			if (targetNPC->GetCurrentWaypointIndex() != targetNPC->GetTargetWaypointIndex())
			{
				targetNPC->Update(dt, theHero, m_cMap);
			}
			else
			{
				guardianCleared = true;
				SetGuardianCleared();
				currentState = PLAYING;
			}
		}
		break;
		case EXITING:
		{
			sizeOfDarkSurrounding -= (float)dt * m_cMap->GetTileSize() * speedOfDarkSurrounding * (sizeOfDarkSurrounding - MinSizeOfScalingDarkSurrounding);
			if (sizeOfDarkSurrounding <= m_cMap->GetTileSize() * MinSizeOfScalingDarkSurrounding)
			{
				sizeOfDarkSurrounding = m_cMap->GetTileSize() * MinSizeOfScalingDarkSurrounding;
				currentState = EXIT;
			}
		}
		break;
		case EXIT:
		{
			timerForEnd -= (float)dt;
			if (timerForEnd <= 0.0f)
			{
				timerForEnd = 0.0f;
			}
		}
		break;
		case LOSING:
		{
			float TargetPosY = sceneManager2D.m_window_height * 0.5f - GameoverText->getScaleY() * 0.5f;
			// Move Text Up
			GameoverText->setPositionY(GameoverText->getPositionY() + speedOfGameoverText * (float)dt);

			// if reached Center
			if (GameoverText->getPositionY() >= TargetPosY)
			{
				GameoverText->setPositionY(TargetPosY);
				currentState = LOST;
			}
		}
		break;
	}

	// UI
	UpdateUI(dt);
}

/********************************************************************************
Check If This NPC Is Interactable
********************************************************************************/
bool CSceneHub::CheckIfThisNPCIsInteractable(CAI_Idling* theNPC)
{
	// Check Distance between hero and this NPC
	if ((theNPC->getPosition() - theHero->getPosition()).Length() <= m_cMap->GetTileSize())
	{
		// Check direction if hero facing the NPC
		switch (theHero->GetAnimationDirection())
		{
			// Check Y, for Left/Right
		case CPlayerInfo::RIGHT:
		{
			if (Vector3(0, theNPC->getPosition().y - theHero->getPosition().y).Length() <= m_cMap->GetTileSize() * 0.6)
			{
				if (theNPC->getPosition().x > theHero->getPosition().x)
				{
					return true;
				}
			}
		}
		break;
		case CPlayerInfo::LEFT:
		{
			if (Vector3(0, theNPC->getPosition().y - theHero->getPosition().y).Length() <= m_cMap->GetTileSize() * 0.6)
			{
				if (theNPC->getPosition().x < theHero->getPosition().x)
				{
					return true;
				}
			}
		}
		break;
		// Check X, for Up/Down
		case CPlayerInfo::UP:
		{
			if (Vector3(theNPC->getPosition().x - theHero->getPosition().x).Length() <= m_cMap->GetTileSize() * 0.5)
			{
				if (theNPC->getPosition().y > theHero->getPosition().y)
				{
					return true;
				}
			}
		}
		break;
		case CPlayerInfo::DOWN:
		{
			if (Vector3(theNPC->getPosition().x - theHero->getPosition().x).Length() <= m_cMap->GetTileSize() * 0.5)
			{
				if (theNPC->getPosition().y < theHero->getPosition().y)
				{
					return true;
				}
			}
		}
		break;
		}
	}
	return false;
}

/********************************************************************************
Update UI
********************************************************************************/
void CSceneHub::UpdateUI(double dt)
{
	UIManagerDifficultySelection->Update((float)dt);
	UIManagerJellybeanSelection->Update((float)dt);
	UIManagerBlackQuad->Update((float)dt);
	UIManagerConfirmation->Update((float)dt);
	UIManagerGivingJellybeans->Update((float)dt);
}

/********************************************************************************
Change UI - playing
********************************************************************************/
void CSceneHub::ChangeUI_Playing()
{
	UIManagerGivingJellybeans->InvokeAnimator()->SkipAllAnimations();
	HideGivingJellybeans();
	UIManagerDifficultySelection->InvokeAnimator()->SkipAllAnimations();
	Hide_DifficultySelection();
	UIManagerBlackQuad->InvokeAnimator()->SkipAllAnimations();
	// AlphaQuad
	UIManagerBlackQuad->InvokeAnimator()->StartTransformation(UIManagerBlackQuad->FindImage("AlphaQuad"), 0, Vector3(0, 0, 0), UI_Speed * 2, UIAnimation::SCALING);
}
/********************************************************************************
Change UI - difficulty selection
********************************************************************************/
void CSceneHub::ChangeUI_DifficultySelection()
{
	UIManagerDifficultySelection->InvokeAnimator()->SkipAllAnimations();
	Show_DifficultySelection();
	UIManagerJellybeanSelection->InvokeAnimator()->SkipAllAnimations();
	Hide_JellybeanSelection();
	UIManagerConfirmation->InvokeAnimator()->SkipAllAnimations();
	HideConfirmation();
	UIManagerBlackQuad->InvokeAnimator()->SkipAllAnimations();
	// AlphaQuad
	UIManagerBlackQuad->InvokeAnimator()->StartTransformation(UIManagerBlackQuad->FindImage("AlphaQuad"), 0, Vector3((float)sceneManager2D.m_window_width, (float)sceneManager2D.m_window_height, 0), UI_Speed * 2, UIAnimation::SCALING);
}
/********************************************************************************
Change UI - jellybean selection
********************************************************************************/
void CSceneHub::ChangeUI_JellybeanSelection()
{
	UIManagerDifficultySelection->InvokeAnimator()->SkipAllAnimations();
	Hide_DifficultySelection();
	UIManagerJellybeanSelection->InvokeAnimator()->SkipAllAnimations();
	Show_JellybeanSelection();
	UIManagerConfirmation->InvokeAnimator()->SkipAllAnimations();
	HideConfirmation();
}
/********************************************************************************
Change UI - Confirmation
********************************************************************************/
void CSceneHub::ChangeUI_Confirmation()
{
	UIManagerConfirmation->InvokeAnimator()->SkipAllAnimations();
	ShowConfirmation();
}

/********************************************************************************
Change UI - Giving Jellybeans
********************************************************************************/
void CSceneHub::ChangeUI_GivingJellybeans()
{
	UIManagerGivingJellybeans->InvokeAnimator()->SkipAllAnimations();
	ShowGivingJellybeans();
	UIManagerBlackQuad->InvokeAnimator()->SkipAllAnimations();
	// AlphaQuad
	UIManagerBlackQuad->InvokeAnimator()->StartTransformation(UIManagerBlackQuad->FindImage("AlphaQuad"), 0, Vector3((float)sceneManager2D.m_window_width, (float)sceneManager2D.m_window_height, 0), UI_Speed * 2, UIAnimation::SCALING);
}

/********************************************************************************
Show Difficulty Selection
********************************************************************************/
void CSceneHub::Show_DifficultySelection()
{
	// Difficulty Header
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("DifficultyHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 2.5f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Tutorial Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("TutorialButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Easy Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("EasyButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Medium Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("MediumButton"), 0.15f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * -0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Hard Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("HardButton"), 0.2f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * -1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Selection background
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("SelectionBackground"), 0, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::SCALING);

	// Back Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("BackButton"), 0.25f, Vector3(sceneManager2D.m_window_width * 0.125f, sceneManager2D.m_window_width * 0.125f, 0), UI_Speed, UIAnimation::SCALING);
}

/********************************************************************************
Hide Difficulty Selection
********************************************************************************/
void CSceneHub::Hide_DifficultySelection()
{
	// Difficulty Header
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("DifficultyHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 100, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Tutorial Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("TutorialButton"), 0.05f, Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Easy Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("EasyButton"), 0.1f, Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Medium Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("MediumButton"), 0.15f, Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * -0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Hard Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("HardButton"), 0.2f, Vector3(-100, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * -1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Selection background
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindImage("SelectionBackground"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);

	// Back Button
	UIManagerDifficultySelection->InvokeAnimator()->StartTransformation(UIManagerDifficultySelection->FindButton("BackButton"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);
}

/********************************************************************************
Show Difficulty Selection
********************************************************************************/
void CSceneHub::Show_JellybeanSelection()
{
	// Jellybean Header
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("JellybeanHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f + sceneManager2D.m_window_height * 0.5f * 0.25f * 2.5f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Up Arrow Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("UpArrowButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.7f, sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Down Arrow Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("DownArrowButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.3f, sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Enter Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("EnterButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.3f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Selection background
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("SelectionBackground"), 0, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::SCALING);

	// Back Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("BackButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.125f, sceneManager2D.m_window_width * 0.125f, 0), UI_Speed, UIAnimation::SCALING);
}

/********************************************************************************
Hide Jellybean Selection
********************************************************************************/
void CSceneHub::Hide_JellybeanSelection()
{
	// Jellybean Header
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("JellybeanHeader"), 0, Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 100, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Up Arrow Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("UpArrowButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.7f, -sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Down Arrow Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("DownArrowButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.3f, -sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Enter Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("EnterButton"), 0.05f, Vector3(sceneManager2D.m_window_width * 0.5f, -sceneManager2D.m_window_height * 0.5f * 0.25f * 1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);

	// Selection background
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindImage("SelectionBackground"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);

	// Back Button
	UIManagerJellybeanSelection->InvokeAnimator()->StartTransformation(UIManagerJellybeanSelection->FindButton("BackButton"), 0, Vector3(0, 0, 0), UI_Speed, UIAnimation::SCALING);
}

/********************************************************************************
Show Confirmation
********************************************************************************/
void CSceneHub::ShowConfirmation()
{
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.65f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.385f, sceneManager2D.m_window_height * 0.3f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.615f, sceneManager2D.m_window_height * 0.3f, 0), UI_Speed, UIAnimation::TRANSLATION);
}

/********************************************************************************
Hide Confirmation
********************************************************************************/
void CSceneHub::HideConfirmation()
{
	// Confirmation Window
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindImage("ConfirmationWindow"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("YesButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerConfirmation->InvokeAnimator()->StartTransformation(UIManagerConfirmation->FindButton("NoButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
}

/********************************************************************************
Show Giving Jellybeans
********************************************************************************/
void CSceneHub::ShowGivingJellybeans()
{
	// Giving Jellybeans Window
	UIManagerGivingJellybeans->InvokeAnimator()->StartTransformation(UIManagerGivingJellybeans->FindImage("GivingJelybeansWindow"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.65f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerGivingJellybeans->InvokeAnimator()->StartTransformation(UIManagerGivingJellybeans->FindButton("YesButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.385f, sceneManager2D.m_window_height * 0.3f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerGivingJellybeans->InvokeAnimator()->StartTransformation(UIManagerGivingJellybeans->FindButton("NoButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.615f, sceneManager2D.m_window_height * 0.3f, 0), UI_Speed, UIAnimation::TRANSLATION);
}

/********************************************************************************
Hide Giving Jellybeans
********************************************************************************/
void CSceneHub::HideGivingJellybeans()
{
	// Giving Jellybeans Window
	UIManagerGivingJellybeans->InvokeAnimator()->StartTransformation(UIManagerGivingJellybeans->FindImage("GivingJelybeansWindow"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 1.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// Yes button
	UIManagerGivingJellybeans->InvokeAnimator()->StartTransformation(UIManagerGivingJellybeans->FindButton("YesButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
	// No button
	UIManagerGivingJellybeans->InvokeAnimator()->StartTransformation(UIManagerGivingJellybeans->FindButton("NoButton"), 0.1f, Vector3(sceneManager2D.m_window_width * 0.45f, -sceneManager2D.m_window_height * 0.5f, 0), UI_Speed, UIAnimation::TRANSLATION);
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

	sceneManager2D.modelStack.Translate((float)-theHero->GetMapOffset_x(), (float)theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);

	// Render the tile map
	RenderTileMap();
	for (int i = 0; i < (int)theDoor.size(); i++)
	{
		// Door
		sceneManager2D.Render2DMesh(theDoor[i]->getMesh(), false, (int)theDoor[i]->getScale().x, (int)theDoor[i]->getScale().y, (int)theDoor[i]->getPositionX(), (int)theDoor[i]->getPositionY());

		int textSize = (int)(m_cMap->GetTileSize() * 0.4f);

		if (theDoor[i]->getDialogue().size() > 4)
		{
			// Dialogue box
			sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, (int)((theDoor[i]->getDialogue().size() - 2) * textSize), textSize, (int)(theDoor[i]->getPositionX() - textSize * (theDoor[i]->getDialogue().size() - 4) * 0.5f), (int)(theDoor[i]->getPositionY() + m_cMap->GetTileSize() + textSize * 0.5f));
			// Text
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], theDoor[i]->getDialogue(), Color(0, 0, 0), (float)textSize, theDoor[i]->getPositionX() - textSize * (theDoor[i]->getDialogue().size() - 4) * 0.5f, theDoor[i]->getPositionY() + m_cMap->GetTileSize() + textSize * 0.5f);
		}
		else
		{
			// Dialogue box
			sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, (int)((theDoor[i]->getDialogue().size() - 1) * textSize), textSize, (int)theDoor[i]->getPositionX(), (int)(theDoor[i]->getPositionY() + m_cMap->GetTileSize() + textSize * 0.5f));
			// Text
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], theDoor[i]->getDialogue(), Color(0, 0, 0), (float)textSize, theDoor[i]->getPositionX(), theDoor[i]->getPositionY() + m_cMap->GetTileSize() + textSize * 0.5f);
		}
	}
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
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_SIDE_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)theHero->getPositionX(), (int)theHero->getPositionY());
	}
		break;
	case CPlayerInfo::LEFT:
	{
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_SIDE_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)theHero->getPositionX(), (int)theHero->getPositionY(), 0.0f, true);
	}
		break;
	case CPlayerInfo::UP:
	{
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_UP_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)theHero->getPositionX(), (int)theHero->getPositionY());
	}
		break;
	case CPlayerInfo::DOWN:
	{
		sceneManager2D.Render2DMesh(theHero->meshList[CPlayerInfo::GEO_TILEHERO_DOWN_FRAME0 + (int)theHero->GetAnimationCounter()], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)theHero->getPositionX(), (int)theHero->getPositionY());
	}
		break;
	}
}

/********************************************************************************
Render the AIs. This is a private function for use in this class only
********************************************************************************/
void CSceneHub::RenderAIs()
{
	// Render the NPCs
	for (int i = 0; i < (int)theNPCs.size(); i++)
	{
		if (theNPCs[i])
		{
			if ((theNPCs[i]->getPosition() - theHero->getPosition()).Length() <= sceneManager2D.m_window_width)
				sceneManager2D.Render2DMesh(theNPCs[i]->getMesh(), false, (int)theNPCs[i]->getScale().x, (int)theNPCs[i]->getScale().y, (int)theNPCs[i]->getPositionX(), (int)theNPCs[i]->getPositionY());
		}
	}
}


void CSceneHub::RenderWaypoints()
{
	for (int i = 0; i < (int)temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)temp.at(i).x, (int)temp.at(i).y);
	}
}

/********************************************************************************
Render the GUI. This is a private function for use in this class only
********************************************************************************/
void CSceneHub::RenderGUI()
{
	//On screen text
	std::ostringstream ss;
	// Jellybean
	sceneManager2D.Render2DMesh(meshList[GEO_JELLYBEAN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());
	//ss.str(std::string());
	ss.precision(3);
	ss << ": " << noOfJellybeans;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), (float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize(), (float)sceneManager2D.m_window_height - m_cMap->GetTileSize());

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
	case GIVING_JELLYBEANS:
		UIManagerBlackQuad->Render(sceneManager2D);
		UIManagerDifficultySelection->Render(sceneManager2D);
		UIManagerJellybeanSelection->Render(sceneManager2D);
		UIManagerGivingJellybeans->Render(sceneManager2D);
		break;
	}

	// Render the number of jellybeans deposited
	if (currentState == JELLYBEAN_SELECTION)
	{
		if (noOfJellybeansDeposited >= 10)
		{
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], to_string(noOfJellybeansDeposited), Color(1, 1, 1), m_cMap->GetTileSize() * 2.f, sceneManager2D.m_window_width * 0.5f - m_cMap->GetTileSize() * 1.75f, sceneManager2D.m_window_height * 0.5f - m_cMap->GetTileSize());
		}
		else
		{
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], to_string(noOfJellybeansDeposited), Color(1, 1, 1), m_cMap->GetTileSize() * 2.f, sceneManager2D.m_window_width * 0.5f - m_cMap->GetTileSize(), sceneManager2D.m_window_height * 0.5f - m_cMap->GetTileSize());
		}
	}

	// Dialogue
	if (currentState == INTERACTING || currentState == GIVING_JELLYBEANS)
	{
		// Dialogue box
		sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, sceneManager2D.m_window_width, m_cMap->GetTileSize(), 0, 0);

		// Text
		float textSize = m_cMap->GetTileSize() * 0.5f;
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], targetNPC->getDialogue(), Color(0, 0, 0), textSize, 0, textSize);

		if (currentState == INTERACTING)
		{
			// Click anywhere to continue
			textSize = m_cMap->GetTileSize() * 0.25f;
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "Click anywhere to continue...", Color(0, 0, 0), textSize, sceneManager2D.m_window_width - textSize * 31 * 0.6f, 0);
		}
	}

	// Gameover
	if (currentState == LOSING)
	{
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], GameoverText->getDialogue(), Color(0, 0, 0), GameoverText->getScale().x, GameoverText->getPositionX(), GameoverText->getPositionY());
	}
	else if (currentState == LOST)
	{
		// Gameover
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], GameoverText->getDialogue(), Color(0, 0, 0), GameoverText->getScale().x, GameoverText->getPositionX(), GameoverText->getPositionY());

		// Click anywhere to continue
		int textSize = m_cMap->GetTileSize() * 0.4f;
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "Click anywhere to continue...", Color(0, 0, 0), textSize, sceneManager2D.m_window_width * 0.275f, sceneManager2D.m_window_height * 0.35f);
	}
	// Dark Surrounding
	else if (currentState == EXITING)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_DARK_SURROUNDING], false, (int)sizeOfDarkSurrounding, (int)sizeOfDarkSurrounding, (int)(theHero->getPositionX() + m_cMap->GetTileSize() * 0.5), (int)(theHero->getPositionY() + m_cMap->GetTileSize() * 0.5));
	}
	// Credits
	else if (currentState == EXIT)
	{
		// Black Quad
		sceneManager2D.Render2DMesh(meshList[GEO_BLACK_QUAD], false, sceneManager2D.m_window_width, sceneManager2D.m_window_height, (int)(sceneManager2D.m_window_width * 0.5), (int)(sceneManager2D.m_window_height * 0.5));

		// Credits Image
		//sceneManager2D.Render2DMesh(Credit->getMesh(), false, (int)Credit->getScale().x, (int)Credit->getScale().y, (int)Credit->getPositionX(), (int)Credit->getPositionY());
	}
}

// Read and store data
void CSceneHub::ReadData(CGameInfo* Data)
{
	if (Data->ifNew == false)
	{
		theHero->setPositionX(Data->heroPosition.x);
		theHero->setPositionY(Data->heroPosition.y);
		theHero->SetAnimationDirection(Data->heroAnimationDir);
		theHero->SetMapOffset_x((int)Data->heroMapOffset.x);
		theHero->SetMapOffset_y((int)Data->heroMapOffset.y);
		guardianCleared = Data->guardianCleared;
		musicActive = Data->musicActive;

		if (guardianCleared)
		{
			SetGuardianCleared();
		}
	}

	noOfJellybeans = Data->jellybean.GetNumOfJellybeans();
}
void CSceneHub::StoreData(CGameInfo* Data)
{
	Data->ifNew = false;
	Data->heroPosition.x = theHero->getPositionX();
	Data->heroPosition.y = theHero->getPositionY();
	Data->heroAnimationDir = theHero->GetAnimationDirection();
	Data->heroMapOffset.x = (float)theHero->GetMapOffset_x();
	Data->heroMapOffset.y = (float)theHero->GetMapOffset_y();
	Data->guardianCleared = guardianCleared;
	Data->musicActive = musicActive;
}

// Set the Guardian to cleared
void CSceneHub::SetGuardianCleared()
{
	for (int i = 0; i < (int)theNPCs.size(); i++)
	{
		if (theNPCs[i]->GetAI_Type() == CAI_Idling::GUARDIAN)
		{
			//Read a value from the lua text file
			UseLuaFiles L;

			L.ReadFiles("Lua//Scene/GameHub.lua");

			// Set Cleared Dialogue
			theNPCs[i]->setDialogue(L.DoLuaString("GuardianFinishedScript"));

			// Set the guardian at the last position
			theNPCs[i]->setPosition(theNPCs[i]->GetWaypoints().back());
			theNPCs[i]->SetCurrentWaypointIndex(theNPCs[i]->GetWaypoints().size() - 1);
			theNPCs[i]->SetTargetWaypointIndex(theNPCs[i]->GetCurrentWaypointIndex());
			theNPCs[i]->UpdateBoundingBox();
		}
	}
}

// Change Tip for tip advisor
void CSceneHub::ChangeTip()
{
	for (int i = 0; i < (int)theNPCs.size(); i++)
	{
		if (theNPCs[i]->GetAI_Type() == CAI_Idling::TIP_ADVISOR)
		{
			//Read a value from the lua text file
			UseLuaFiles L;

			L.ReadFiles("Lua//Scene/GameHub.lua");

			CProbabilitySystem p_s;
			for (int j = 0; j < L.DoLuaInt("numOfTips"); j++)
			{
				p_s.AddProbability(1);
			}
			// Set Cleared Dialogue
			theNPCs[i]->setDialogue(L.DoLuaString("tip" + to_string(p_s.GetARandIntProbability())));
		}
	}
}