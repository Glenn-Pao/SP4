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
	, UIManager(NULL)
	, UI_Speed(0.f)
{
}

CSceneGame2::~CSceneGame2()
{
	// Dialogues tiles
	for (int i = 0; i < (int)dialogueTiles.size(); i++)
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

void CSceneGame2::Init(int level)
{
	this->level = level;
	// Init the base scene
	sceneManager2D.Init(level);

	L = new UseLuaFiles();

	L->ReadFiles("Lua//Scene/Game2/tutorial.lua");
	
	tileSize = L->DoLuaInt("tileSize");

	scriptFinished = L->DoLuaString("scriptFinished");
	scriptExit = L->DoLuaString("scriptExit");
	scriptLevelEnd = L->DoLuaString("scriptLevelEnd");
	
	L->ReadFiles("Lua//Scene/Game2/AISettings.lua");
	smallSquare = L->DoLuaInt("smallSquare");
	medSquare = L->DoLuaInt("medSquare");
	bigSquare = L->DoLuaInt("bigSquare");

	// Dialogues scripts
	for (int i = 0; i < 6; i++)
	{
		scriptDialogues.push_back(L->DoLuaString("script" + to_string(i)));
	}

	switch (level)
	{
	case 0:
	{
		currentState = PREPARING;
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
			int pos_x = k*m_cMap->GetTileSize();
			int pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

			if (m_cMap->theScreenMap[i][k] == 1000 && theHero == NULL)
			{
				// Initialise the hero's position

				theHero = new CPlayerInfo(m_cMap);
				theHero->setPositionX((float)pos_x);
				theHero->setPositionY((float)pos_y);

				// Control Dialogue
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[0], Vector3((float)pos_x, (float)pos_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(pos_x - (tileSize * 0.5f), pos_y + (tileSize * 0.5f), 0);
				Vector3 bottomright(pos_x + (tileSize * 0.5f), pos_y - (tileSize * 0.5f), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
		}
	}

	// Initialise the Meshes
	InitMeshes();

	switch (level)
	{
	case 0: case 1:
	{
		InitTutorial();
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
	
	timer = 0;
	cooldownTimer = livesCDTimer = 0;
	catchcooldown = livescooldown = false;
	
	TRightS = TLeftS = TRightM = TLeftM = TRightB = TLeftB = 0;

	slot1.Set((float)sceneManager2D.m_window_width - (m_cMap->GetTileSize() * 4), (float)sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 10), 0);
	slot2.Set((float)sceneManager2D.m_window_width - (m_cMap->GetTileSize() * 3), (float)sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 9), 0);
	slot3.Set((float)sceneManager2D.m_window_width - (m_cMap->GetTileSize() * 2), (float)sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 10), 0);
	slot4.Set((float)sceneManager2D.m_window_width - (m_cMap->GetTileSize() * 3), (float)sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 11), 0);
	slot5.Set((float)sceneManager2D.m_window_width - (m_cMap->GetTileSize() * 4), (float)sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 9), 0);
	slot6.Set((float)sceneManager2D.m_window_width - (m_cMap->GetTileSize() * 2), (float)sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 9), 0);

	lives = 3;
	UIWarningScale = scaleUP = false;


	InitUI();
}

void CSceneGame2::PreInit()
{
	sceneManager2D.PreInit();
}

void CSceneGame2::InitUI()
{
	//Animation speed
	UI_Speed = 20.0f;

	UIManager = new UISystem();

	Image* AlphaQuad;
	AlphaQuad = new Image("AlphaQuad", meshList[GEO_ALPHA_BLACK_QUAD], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
	UIManager->addFeature(AlphaQuad);

	Image* WinScreen;
	WinScreen = new Image("WinScreen", meshList[GEO_WIN_BG], Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 200, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.45f, 0));
	UIManager->addFeature(WinScreen);

	Image* LoseScreen;
	LoseScreen = new Image("LoseScreen", meshList[GEO_LOSE_BG], Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 200, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.45f, 0));
	UIManager->addFeature(LoseScreen);

	Image* TutScreen;
	TutScreen = new Image("TutScreen", meshList[GEO_TUT_BG], Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 200, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.45f, 0));
	UIManager->addFeature(TutScreen);

	Image* TutLoseScreen;
	TutLoseScreen = new Image("TutLoseScreen", meshList[GEO_TUTLOSE_BG], Vector3(sceneManager2D.m_window_width * 0.5f, (float)sceneManager2D.m_window_height + 200, 0), Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.45f, 0));
	UIManager->addFeature(TutLoseScreen);

	Button* ReturnToHubButton;
	ReturnToHubButton = new Button("ReturnToHubButton", meshList[GEO_HUB_BTN_UP], meshList[GEO_HUB_BTN_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.45f, -200, 0), Vector3(sceneManager2D.m_window_width * 0.2f, sceneManager2D.m_window_height * 0.1f, 0));
	UIManager->addFeature(ReturnToHubButton);

	Image* WarningR;
	WarningR = new Image("WarningR", meshList[GEO_WARNING], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
	UIManager->addFeature(WarningR);

	// Instructions for Tutorial
	if (level == 0)
	{
		// Scale the Alpha quad first
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("AlphaQuad"), 1, Vector3((float)sceneManager2D.m_window_width, (float)sceneManager2D.m_window_height), 5.0f, UIAnimation::SCALING);

		// Instruction Header
		Image* Instruction_Header;
		Instruction_Header = new Image("Instruction_Header", meshList[GEO_INSTRUCTION_HEADER], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 1.3f), Vector3(sceneManager2D.m_window_width * 0.3f, sceneManager2D.m_window_height * 0.125f));
		UIManager->addFeature(Instruction_Header);
		// Move the Instruction Header second
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Instruction_Header"), 1.05f, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.9f), 5.0f, UIAnimation::TRANSLATION);
		
		// Instruction Background
		Image* Instruction_Background;
		Instruction_Background = new Image("Instruction_Background", meshList[GEO_DIALOGUE_BOX], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f), Vector3(0, 0, 0));
		UIManager->addFeature(Instruction_Background);
		// Scale the background third
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Instruction_Background"), 1.1f, Vector3(sceneManager2D.m_window_width * 0.675f, sceneManager2D.m_window_height * 0.675f), 5.0f, UIAnimation::SCALING);

		// Next Button
		Button* Next_Button;
		Next_Button = new Button("Next_Button", meshList[GEO_UPARROW_BUTTON_UP], meshList[GEO_UPARROW_BUTTON_DOWN], NULL, Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.1f, 0), Vector3());
		UIManager->addFeature(Next_Button);
		// Scale the first Instruction fourth
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Next_Button"), 1.2f, Vector3(sceneManager2D.m_window_height * 0.1f, sceneManager2D.m_window_height * 0.1f, 1), 5.0f, UIAnimation::SCALING);

		numOfInstructionsLeft = 3;

		// Instructions pt 1
		Image* INSTRUCTIONS1;
		INSTRUCTIONS1 = new Image("INSTRUCTIONS1", meshList[GEO_INSTRUCTION1_GAME2], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
		UIManager->addFeature(INSTRUCTIONS1);
		// Scale the first Instruction fourth
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("INSTRUCTIONS1"), 1.15f, Vector3(sceneManager2D.m_window_width * 0.65f, sceneManager2D.m_window_height * 0.65f, 1), 5.0f, UIAnimation::SCALING);
		
		// Instructions pt 1
		Image* INSTRUCTIONS2;
		INSTRUCTIONS2 = new Image("INSTRUCTIONS2", meshList[GEO_INSTRUCTION2_GAME2], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
		UIManager->addFeature(INSTRUCTIONS2);
		// Instructions pt 1
		Image* INSTRUCTIONS3;
		INSTRUCTIONS3 = new Image("INSTRUCTIONS3", meshList[GEO_INSTRUCTION3_GAME2], Vector3(sceneManager2D.m_window_width * 0.5f, sceneManager2D.m_window_height * 0.5f, 0), Vector3(0, 0, 0));
		UIManager->addFeature(INSTRUCTIONS3);
		
	}
}

void CSceneGame2::InitTutorial()
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			float TSize_x = (float)k*m_cMap->GetTileSize();
			float TSize_y = (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();
			if (m_cMap->theScreenMap[i][k] == 33)
			{
				GreyDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_TILE_DOOR]));
				GreyDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 30)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[2], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5f), TSize_y + (tileSize * 0.5f), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5f), TSize_y - (tileSize * 0.5f), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 31)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[1], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5f), TSize_y + (tileSize * 0.5f), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5f), TSize_y - (tileSize * 0.5f), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 32)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[3], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5f), TSize_y + (tileSize * 0.5f), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5f), TSize_y - (tileSize * 0.5f), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 29)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[4], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5f), TSize_y + (tileSize * 0.5f), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5f), TSize_y - (tileSize * 0.5f), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 28)
			{
				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[5], Vector3(TSize_x, TSize_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(TSize_x - (tileSize * 0.5f), TSize_y + (tileSize * 0.5f), 0);
				Vector3 bottomright(TSize_x + (tileSize * 0.5f), TSize_y - (tileSize * 0.5f), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			else if (m_cMap->theScreenMap[i][k] == 27)
			{
				endDoor = new CDoor(CObjects::DOOR, 1, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_TILE_DOOR]);
			}
			else if (m_cMap->theScreenMap[i][k] == 35)
			{
				GreenDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_GREEN_DOOR]));
				GreenDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 34)
			{
				BlueDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_BLUE_DOOR]));
				BlueDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 37)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "BLUE", Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_COLOUR_BALL_BLUE]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 38)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "YELLOW", Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_COLOUR_BALL_YELLOW]));
				ColoursSet.back()->setActive(true);
				
			}
			else if (m_cMap->theScreenMap[i][k] == 200)		//Right smallBox
			{
				// Set the strategy for the enemy
				AIsList4ptRightS.push_back(new AI(CObjects::AI, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, (float)tileSize), meshList[GEO_TILEENEMY_FRAME0], 0));
				AIsList4ptRightS.back()->setActive(true);
				AIsList4ptRightS.back()->setFSM(CFSM::MOVING);
				AIsList4ptRightS.back()->SetAIvariables4pt(Vector3((float)tileSize * smallSquare, 0, 1), Vector3(0, (float)-tileSize * smallSquare, 1), Vector3(-(float)tileSize * smallSquare, 0, 1));
			}
			else if (m_cMap->theScreenMap[i][k] == 201)		//Left smallBox
			{
				// Set the strategy for the enemy
				AIsList4ptLeftS.push_back(new AI(CObjects::AI, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, (float)tileSize), meshList[GEO_TILEENEMY_FRAME0], 0));
				AIsList4ptLeftS.back()->setActive(true);
				AIsList4ptLeftS.back()->setFSM(CFSM::MOVING);
				AIsList4ptLeftS.back()->SetAIvariables4pt(Vector3((float)-tileSize * smallSquare, 0, 1), Vector3(0, (float)-tileSize * smallSquare, 1), Vector3((float)tileSize * smallSquare, 0, 1));
			}

			else if (m_cMap->theScreenMap[i][k] == 202)		//Right MedBox
			{
				// Set the strategy for the enemy
				AIsList4ptRightM.push_back(new AI(CObjects::AI, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, (float)tileSize), meshList[GEO_TILEENEMY_FRAME0], 0));
				AIsList4ptRightM.back()->setActive(true);
				AIsList4ptRightM.back()->setFSM(CFSM::MOVING);
				AIsList4ptRightM.back()->SetAIvariables4pt(Vector3((float)tileSize * medSquare, 0, 1), Vector3(0, (float)-tileSize * medSquare, 1), Vector3((float)-tileSize * medSquare, 0, 1));
			}
			else if (m_cMap->theScreenMap[i][k] == 203)		//Left MedBox
			{
				// Set the strategy for the enemy
				AIsList4ptLeftM.push_back(new AI(CObjects::AI, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, (float)tileSize), meshList[GEO_TILEENEMY_FRAME0], 0));
				AIsList4ptLeftM.back()->setActive(true);
				AIsList4ptLeftM.back()->setFSM(CFSM::MOVING);
				AIsList4ptLeftM.back()->SetAIvariables4pt(Vector3((float)-tileSize * medSquare, 0, 1), Vector3(0, (float)-tileSize * medSquare, 1), Vector3((float)tileSize * medSquare, 0, 1));
			}

			else if (m_cMap->theScreenMap[i][k] == 204)		//Right BigBox
			{
				// Set the strategy for the enemy
				AIsList4ptRightB.push_back(new AI(CObjects::AI, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, (float)tileSize), meshList[GEO_TILEENEMY_FRAME0], 0));
				AIsList4ptRightB.back()->setActive(true);
				AIsList4ptRightB.back()->setFSM(CFSM::MOVING);
				AIsList4ptRightB.back()->SetAIvariables4pt(Vector3((float)tileSize * bigSquare, 0, 1), Vector3(0, (float)-tileSize * bigSquare, 1), Vector3((float)-tileSize * bigSquare, 0, 1));
			}
			else if (m_cMap->theScreenMap[i][k] == 205)		//Left BigBox
			{
				// Set the strategy for the enemy
				AIsList4ptRightB.push_back(new AI(CObjects::AI, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, (float)tileSize), meshList[GEO_TILEENEMY_FRAME0], 0));
				AIsList4ptRightB.back()->setActive(true);
				AIsList4ptRightB.back()->setFSM(CFSM::MOVING);
				AIsList4ptRightB.back()->SetAIvariables4pt(Vector3((float)-tileSize * bigSquare, 0, 1), Vector3(0, (float)-tileSize * bigSquare, 1), Vector3((float)tileSize * bigSquare, 0, 1));
			}
		}
	}
}

void CSceneGame2::InitLevel2()
{
	InitTutorial();

	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			float TSize_x = (float)k*m_cMap->GetTileSize();
			float TSize_y = (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

			if (m_cMap->theScreenMap[i][k] == 26)
			{
				OrangeDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_ORANGE_DOOR]));
				OrangeDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 36)
			{
				YellowDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_YELLOW_DOOR]));
				YellowDoors.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 39)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "RED", Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_COLOUR_BALL_RED]));
				ColoursSet.back()->setActive(true);
			}
			else if (m_cMap->theScreenMap[i][k] == 40)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "ORANGE", Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_COLOUR_BALL_ORANGE]));
				ColoursSet.back()->setActive(true);
			}
		}
	}
}

void CSceneGame2::InitLevel3()
{
	InitLevel2();
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			float TSize_x = (float)k*m_cMap->GetTileSize();
			float TSize_y = (float)(m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

			if (m_cMap->theScreenMap[i][k] == 25)	//Purple doors
			{
				PurpleDoors.push_back(new CDoor(CObjects::COLOR, 1, Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_PURPLE_DOOR]));
				PurpleDoors.back()->setActive(true);
			}
			
			else if (m_cMap->theScreenMap[i][k] == 24)
			{
				ColoursSet.push_back(new CColour(CObjects::COLOR, "PURPLE", Vector3(TSize_x, TSize_y), Vector3((float)tileSize, (float)tileSize, 1), meshList[GEO_COLOUR_BALL_PURPLE]));
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
	//meshList[GEO_DIALOGUE_BOX]->textureID = LoadTGA("Image//dialogue_box.tga");

	meshList[GEO_DIALOGUE_BOX] = MeshBuilder::GenerateQuad("GEO_DIALOGUE_BOX", Color(1, 0.8f, 0.8f), 1);

	meshList[GEO_BACKFADE] = MeshBuilder::Generate2DMesh("GEO_BACKFADE", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_BACKFADE]->textureID = LoadTGA("Image//UI BackFade.tga");

	meshList[GEO_BACKFADEON] = MeshBuilder::Generate2DMesh("GEO_BACKFADEON", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_BACKFADEON]->textureID = LoadTGA("Image//UI BackFadeOn.tga");

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

	meshList[GEO_HEART] = MeshBuilder::Generate2DMesh("GEO_HEART", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_HEART]->textureID = LoadTGA("Image//heart.tga");

	// WIN
	// Window
	meshList[GEO_WIN_BG] = MeshBuilder::GenerateQuad("GEO_WIN_BG", Color(1, 1, 1), 1);
	meshList[GEO_WIN_BG]->textureID = LoadTGA("Image//UI/Win_BG.tga");
	meshList[GEO_LOSE_BG] = MeshBuilder::GenerateQuad("GEO_LOSE_BG", Color(1, 1, 1), 1);
	meshList[GEO_LOSE_BG]->textureID = LoadTGA("Image//UI/Lose_BG.tga");
	meshList[GEO_TUT_BG] = MeshBuilder::GenerateQuad("GEO_TUT_BG", Color(1, 1, 1), 1);
	meshList[GEO_TUT_BG]->textureID = LoadTGA("Image//UI/Completed_BG.tga");
	meshList[GEO_TUTLOSE_BG] = MeshBuilder::GenerateQuad("GEO_TUTLOSE_BG", Color(1, 1, 1), 1);
	meshList[GEO_TUTLOSE_BG]->textureID = LoadTGA("Image//UI/TutLose_BG.tga");

	meshList[GEO_HUB_BTN_UP] = MeshBuilder::GenerateQuad("GEO_HUB_BTN_UP", Color(1, 1, 1), 1);
	meshList[GEO_HUB_BTN_UP]->textureID = LoadTGA("Image//UI/HubButton.tga");
	meshList[GEO_HUB_BTN_DOWN] = MeshBuilder::GenerateQuad("GEO_HUB_BTN_DOWN", Color(1, 1, 1), 1);
	meshList[GEO_HUB_BTN_DOWN]->textureID = LoadTGA("Image//UI/HubButton_Pressed.tga");

	// Alpha Black Quad
	meshList[GEO_ALPHA_BLACK_QUAD] = MeshBuilder::GenerateQuad("GEO_ALPHA_BLACK_QUAD", Color(1, 1, 1), 1);
	meshList[GEO_ALPHA_BLACK_QUAD]->textureID = LoadTGA("Image//UI/Half_Alpha_Black.tga");

	meshList[GEO_WARNING] = MeshBuilder::GenerateQuad("GEO_WARNING", Color(1, 1, 1), 1);
	meshList[GEO_WARNING]->textureID = LoadTGA("Image//UI/Half_Alpha_Red.tga");

	meshList[GEO_INSTRUCTION1_GAME2] = MeshBuilder::GenerateQuad("GEO_INSTRUCTION1_GAME2", Color(1, 1, 1), 1);
	meshList[GEO_INSTRUCTION1_GAME2]->textureID = LoadTGA("Image//Game2/Instructions1.tga");

	meshList[GEO_INSTRUCTION2_GAME2] = MeshBuilder::GenerateQuad("GEO_INSTRUCTION2_GAME2", Color(1, 1, 1), 1);
	meshList[GEO_INSTRUCTION2_GAME2]->textureID = LoadTGA("Image//Game2/Instructions2.tga");

	meshList[GEO_INSTRUCTION3_GAME2] = MeshBuilder::GenerateQuad("GEO_INSTRUCTION3_GAME2", Color(1, 1, 1), 1);
	meshList[GEO_INSTRUCTION3_GAME2]->textureID = LoadTGA("Image//Game2/Instructions3.tga");


	meshList[GEO_INSTRUCTION_HEADER] = MeshBuilder::GenerateQuad("GEO_INSTRUCTION_HEADER", Color(1, 1, 1), 1);
	meshList[GEO_INSTRUCTION_HEADER]->textureID = LoadTGA("Image//Game1/Instruction_Header.tga");

	meshList[GEO_UPARROW_BUTTON_UP] = MeshBuilder::GenerateQuad("GEO_UPARROW_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[GEO_UPARROW_BUTTON_UP]->textureID = LoadTGA("Image//UI/Right_Button.tga");
	meshList[GEO_UPARROW_BUTTON_DOWN] = MeshBuilder::GenerateQuad("GEO_UPARROW_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[GEO_UPARROW_BUTTON_DOWN]->textureID = LoadTGA("Image//UI/Right_Button_Pressed.tga");

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

		Colours = ColoursThePlayerHas.size();


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
			theHero->SetAnimationCounter(theHero->GetAnimationCounter() + theHero->GetMovementSpeed() * m_cMap->GetTileSize() * (float)dt * theHero->GetAnimationSpeed());
			if (theHero->GetAnimationCounter() > theHero->GetAnimationMaxCounter())
				theHero->SetAnimationCounter(1);
		}
		else
		{
			theHero->SetAnimationCounter(0);
		}
		theHero->HeroUpdate(m_cMap, (float)dt);

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
			for (int i = 0; i < (int)theEnemies.size(); i++)
			{
				theEnemies[i]->ChangeStrategy(new CStrategy_Kill());
			}
		}
		else if (typeCollided == 11)
		{
			for (int i = 0; i < (int)theEnemies.size(); i++)
			{
				theEnemies[i]->ChangeStrategy(NULL);
			}
		}
		else
		{
			//theEnemy->ChangeStrategy(NULL);
		}


		// Dialogues tiles
		for (int i = 0; i < (int)dialogueTiles.size(); i++)
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

		UpdateAI(dt);

		

		switch (level)
		{
		case 0: case 1:
			UpdateTutorial(dt);
			break;
		case 2:
			UpdateLevel2(dt);
			break;
		case 3:
			UpdateLevel3(dt);
			break;
		}

	}
	UpdateUI(dt);
}

void CSceneGame2::UpdateUI(double dt)
{
	UIManager->Update((float)dt);

	if (currentState == PLAYING && livescooldown && !UIWarningScale)
	{
		UIWarningScale = true;
		UIManager->InvokeAnimator()->SkipAllAnimations();
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("WarningR"), 0, Vector3((float)sceneManager2D.m_window_width, (float)sceneManager2D.m_window_height, 1), 10, 2);
	}
	
	else if (currentState == PLAYING && !livescooldown && UIWarningScale)
	{
		UIWarningScale = false;
		UIManager->InvokeAnimator()->SkipAllAnimations();
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("WarningR"), 0, Vector3(1, 1, 1), 10, 2);
	}
}

void CSceneGame2::UpdateTutorial(double dt)
{
	// Dialogues tiles
	for (int i = 0; i < (int)dialogueTiles.size(); i++)
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
	for (int i = 0; i < (int)GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getActive() == true)
		{
			if (GreyDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				GreyDoors[i]->setActive(false);
			}
		}
	}

	for (int i = 0; i < (int)GreenDoors.size(); i++)
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


	for (int i = 0; i < (int)BlueDoors.size(); i++)
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
	for (int i = 0; i < (int)ColoursSet.size(); i++)
	{
		if (ColoursSet[i]->getActive() == true)
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
	}

	if (endDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		currentState = WIN;
	}

	if (livescooldown)
	{
		livesCDTimer += 0.1f;
		if (livesCDTimer > 20.f)
		{
			livescooldown = false;
			livesCDTimer = 0;
		}
	}
	
}

void CSceneGame2::UpdateLevel2(double dt)
{
	UpdateTutorial(dt);

	for (int i = 0; i < (int)YellowDoors.size(); i++)
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

	for (int i = 0; i < (int)OrangeDoors.size(); i++)
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
}

void CSceneGame2::UpdateLevel3(double dt)
{
	UpdateLevel2(dt);
	for (int i = 0; i < (int)PurpleDoors.size(); i++)
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
}

void CSceneGame2::UpdateAI(double dt)
{
	UpdateLeft4PtS(dt);
	UpdateRight4PtS(dt);
	UpdateLeft4PtM(dt);
	UpdateRight4PtM(dt);
	UpdateLeft4PtB(dt);
	UpdateRight4PtB(dt);
}

void CSceneGame2::HandleAICollisionCheck(double dt)
{
	if (ColoursThePlayerHas.size() <= 0)
	{
		if (!livescooldown)
		{
			if (lives > 1)
			{
				lives -= 1;
				livescooldown = true;
			}
			else
				currentState = LOSE;
		}
	}

	if (!catchcooldown)
	{
		if (ColoursThePlayerHas.size() <= 0)
		{ }
		else
		{
			int colourToRemove = Math::RandIntMinMax(0,ColoursThePlayerHas.size() - 1);	

			for (int i = 0; i < (int)ColoursSet.size(); i++)
			{
				if (ColoursSet[i]->getColour() == ColoursThePlayerHas[colourToRemove])
				{
					ColoursSet[i]->setActive(true);

					if (ColoursSet[i]->getColour() == "BLUE")
					{
						if (hasBlue)
						{
							hasBlue = false;
							if (castedBlue)
								castedBlue = false;
							else if (castedGreen)
								castedGreen = false;
							catchcooldown = true;
						}
						ColoursThePlayerHas.erase(std::find(ColoursThePlayerHas.begin(), ColoursThePlayerHas.end(), ColoursSet[i]->getColour()));
					}

					else if (ColoursSet[i]->getColour() == "YELLOW")
					{
						if (hasYellow)
						{
							hasYellow = false;
							if (castedYellow)
								castedYellow = false;
							else if (castedGreen)
								castedGreen = false;
							catchcooldown = true;
						}
						ColoursThePlayerHas.erase(std::find(ColoursThePlayerHas.begin(), ColoursThePlayerHas.end(), ColoursSet[i]->getColour()));
					}

					else if (ColoursSet[i]->getColour() == "RED")
					{
						if (hasRed)
						{
							hasRed = false;
							if (castedRed)
								castedRed = false;
							else if (castedOrange)
								castedOrange = false;
							catchcooldown = true;
						}
						ColoursThePlayerHas.erase(std::find(ColoursThePlayerHas.begin(), ColoursThePlayerHas.end(), ColoursSet[i]->getColour()));
					}
					else if (ColoursSet[i]->getColour() == "PURPLE")
					{
						if (hasPurple)
						{
							hasPurple = false;
							if (castedPurple)
								castedPurple = false;
							catchcooldown = true;
						}
						ColoursThePlayerHas.erase(std::find(ColoursThePlayerHas.begin(), ColoursThePlayerHas.end(), ColoursSet[i]->getColour()));
					}
					break;
				}
			}
		}
	}
}

void CSceneGame2::UpdateRight4PtS(double dt)
{
	for (int i = 0; i < (int)AIsList4ptRightS.size(); i++)
	{
		AIsList4ptRightS[i]->UpdateFSM(dt);

		if (catchcooldown)
		{
			TRightS += 0.1f;
			if (TRightS > 20.f)
			{
				catchcooldown = false;
				TRightS = 0;
			}
		}
		if (AIsList4ptRightS[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			HandleAICollisionCheck(dt);
		}
	}
}

void CSceneGame2::UpdateLeft4PtS(double dt)
{
	for (int i = 0; i < (int)AIsList4ptLeftS.size(); i++)
	{
		AIsList4ptLeftS[i]->UpdateFSM(dt);

		if (catchcooldown)
		{
			TLeftS += 0.1f;
			if (TLeftS > 20.f)
			{
				catchcooldown = false;
				TLeftS = 0;
			}
		}
		if (AIsList4ptLeftS[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			HandleAICollisionCheck(dt);
		}
	}
}

void CSceneGame2::UpdateRight4PtM(double dt)
{
	for (int i = 0; i < (int)AIsList4ptRightM.size(); i++)
	{
		AIsList4ptRightM[i]->UpdateFSM(dt);

		if (catchcooldown)
		{
			TRightM += 0.1f;
			if (TRightM > 20.f)
			{
				catchcooldown = false;
				TRightM = 0;
			}
		}
		if (AIsList4ptRightM[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			HandleAICollisionCheck(dt);
		}
	}
}

void CSceneGame2::UpdateLeft4PtM(double dt)
{
	for (int i = 0; i < (int)AIsList4ptLeftM.size(); i++)
	{
		AIsList4ptLeftM[i]->UpdateFSM(dt);

		if (catchcooldown)
		{
			TLeftM += 0.1f;
			if (TLeftM > 20.f)
			{
				catchcooldown = false;
				TLeftM = 0;
			}
		}
		if (AIsList4ptLeftM[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			HandleAICollisionCheck(dt);
		}
	}
}

void CSceneGame2::UpdateRight4PtB(double dt)
{
	for (int i = 0; i < (int)AIsList4ptRightB.size(); i++)
	{
		AIsList4ptRightB[i]->UpdateFSM(dt);

		if (catchcooldown)
		{
			TRightB += 0.1f;
			if (TRightB > 20.f)
			{
				catchcooldown = false;
				TRightB = 0;
			}
		}
		if (AIsList4ptRightB[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			HandleAICollisionCheck(dt);
		}
	}
}

void CSceneGame2::UpdateLeft4PtB(double dt)
{
	for (int i = 0; i < (int)AIsList4ptLeftB.size(); i++)
	{
		AIsList4ptLeftB[i]->UpdateFSM(dt);

		if (catchcooldown)
		{
			TLeftB += 0.1f;
			if (TLeftB > 20.f)
			{
				catchcooldown = false;
				TLeftB = 0;
			}
		}
		if (AIsList4ptLeftB[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			HandleAICollisionCheck(dt);
		}
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

	sceneManager2D.modelStack.Translate((float)-theHero->GetMapOffset_x(), (float)theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);

	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render Hero
	RenderHero();
	//Render Ais
	RenderAIs();

	
	RenderObjects();

	sceneManager2D.modelStack.PopMatrix();
	RenderGUI();
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
void CSceneGame2::RenderAIs()
{
	for (int i = 0; i < (int)AIsList4ptRightS.size(); i++)
	{
		sceneManager2D.Render2DMesh(AIsList4ptRightS[i]->getMesh(), false, (int)AIsList4ptRightS[i]->getScale().x, (int)AIsList4ptRightS[i]->getScale().y, (int)AIsList4ptRightS[i]->getPositionX(), (int)AIsList4ptRightS[i]->getPositionY());
	}
	
	for (int i = 0; i < (int)AIsList4ptLeftS.size(); i++)
	{
		sceneManager2D.Render2DMesh(AIsList4ptLeftS[i]->getMesh(), false, (int)AIsList4ptLeftS[i]->getScale().x, (int)AIsList4ptLeftS[i]->getScale().y, (int)AIsList4ptLeftS[i]->getPositionX(), (int)AIsList4ptLeftS[i]->getPositionY());
	}

	for (int i = 0; i < (int)AIsList4ptRightM.size(); i++)
	{
		sceneManager2D.Render2DMesh(AIsList4ptRightM[i]->getMesh(), false, (int)AIsList4ptRightM[i]->getScale().x, (int)AIsList4ptRightM[i]->getScale().y, (int)AIsList4ptRightM[i]->getPositionX(), (int)AIsList4ptRightM[i]->getPositionY());
	}

	for (int i = 0; i < (int)AIsList4ptLeftM.size(); i++)
	{
		sceneManager2D.Render2DMesh(AIsList4ptLeftM[i]->getMesh(), false, (int)AIsList4ptLeftM[i]->getScale().x, (int)AIsList4ptLeftM[i]->getScale().y, (int)AIsList4ptLeftM[i]->getPositionX(), (int)AIsList4ptLeftM[i]->getPositionY());
	}

	for (int i = 0; i < (int)AIsList4ptRightB.size(); i++)
	{
		sceneManager2D.Render2DMesh(AIsList4ptRightB[i]->getMesh(), false, (int)AIsList4ptRightB[i]->getScale().x, (int)AIsList4ptRightB[i]->getScale().y, (int)AIsList4ptRightB[i]->getPositionX(), (int)AIsList4ptRightB[i]->getPositionY());
	}

	for (int i = 0; i < (int)AIsList4ptLeftB.size(); i++)
	{
		sceneManager2D.Render2DMesh(AIsList4ptLeftB[i]->getMesh(), false, (int)AIsList4ptLeftB[i]->getScale().x, (int)AIsList4ptLeftB[i]->getScale().y, (int)AIsList4ptLeftB[i]->getPositionX(), (int)AIsList4ptLeftB[i]->getPositionY());
	}
}

void CSceneGame2::RenderObjects()
{
	switch (level)
	{
	case 0 : case 1:
		RenderTutorial();
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
	for (int i = 0; i < (int)GreyDoors.size(); i++)
	{
		if (GreyDoors[i]->getActive())
			sceneManager2D.Render2DMesh(GreyDoors[i]->getMesh(), false, (int)GreyDoors[i]->getScale().x, (int)GreyDoors[i]->getScale().y, (int)GreyDoors[i]->getPositionX(), (int)GreyDoors[i]->getPositionY());
	}
	for (int i = 0; i < (int)ColoursSet.size(); i++)
	{
		if (ColoursSet[i]->getActive())
			sceneManager2D.Render2DMesh(ColoursSet[i]->getMesh(), false, (int)ColoursSet[i]->getScale().x, (int)ColoursSet[i]->getScale().y, (int)ColoursSet[i]->getPositionX(), (int)ColoursSet[i]->getPositionY());
	}


	if (castedBlue && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_BLUE], false, (int)(m_cMap->GetTileSize() * 0.5), (int)(m_cMap->GetTileSize() * 0.5), (int)theHero->getPositionX(), (int)(theHero->getPositionY() + m_cMap->GetTileSize()));
	if (castedYellow && !castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_YELLOW], false, (int)(m_cMap->GetTileSize() * 0.5), (int)(m_cMap->GetTileSize() * 0.5), (int)(theHero->getPositionX() + m_cMap->GetTileSize() - 10), (int)(theHero->getPositionY() + m_cMap->GetTileSize()));

	if (castedGreen)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_GREEN], false, (int)(m_cMap->GetTileSize() * 0.5), (int)(m_cMap->GetTileSize() * 0.5), (int)(theHero->getPositionX() + 10), (int)(theHero->getPositionY() + m_cMap->GetTileSize()));

	for (int i = 0; i < (int)BlueDoors.size(); i++)
	{
		if (BlueDoors[i]->getActive())
			sceneManager2D.Render2DMesh(BlueDoors[i]->getMesh(), false, (int)BlueDoors[i]->getScale().x, (int)BlueDoors[i]->getScale().y, (int)BlueDoors[i]->getPositionX(), (int)BlueDoors[i]->getPositionY());
	}

	for (int i = 0; i < (int)GreenDoors.size(); i++)
	{
		if (GreenDoors[i]->getActive())
			sceneManager2D.Render2DMesh(GreenDoors[i]->getMesh(), false, (int)GreenDoors[i]->getScale().x, (int)GreenDoors[i]->getScale().y, (int)GreenDoors[i]->getPositionX(), (int)GreenDoors[i]->getPositionY());
	}

	sceneManager2D.Render2DMesh(endDoor->getMesh(), false, (int)endDoor->getScale().x, (int)endDoor->getScale().y, (int)endDoor->getPositionX(), (int)endDoor->getPositionY());
}

void CSceneGame2::RenderLevel2()
{
	RenderTutorial();


	if (castedRed && !castedOrange)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_RED], false, (int)(m_cMap->GetTileSize() * 0.5), (int)(m_cMap->GetTileSize() * 0.5), (int)theHero->getPositionX(), (int)(theHero->getPositionY() - m_cMap->GetTileSize() + 10));
	
	if (castedOrange)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_ORANGE], false, (int)(m_cMap->GetTileSize() * 0.5), (int)(m_cMap->GetTileSize() * 0.5), (int)theHero->getPositionX() + 10, (int)(theHero->getPositionY() - m_cMap->GetTileSize() + 10));
		
	for (int i = 0; i < (int)YellowDoors.size(); i++)
	{
		if (YellowDoors[i]->getActive())
			sceneManager2D.Render2DMesh(YellowDoors[i]->getMesh(), false, (int)YellowDoors[i]->getScale().x, (int)YellowDoors[i]->getScale().y, (int)YellowDoors[i]->getPositionX(), (int)YellowDoors[i]->getPositionY());
	}

	for (int i = 0; i < (int)OrangeDoors.size(); i++)
	{
		if (OrangeDoors[i]->getActive())
			sceneManager2D.Render2DMesh(OrangeDoors[i]->getMesh(), false, (int)OrangeDoors[i]->getScale().x, (int)OrangeDoors[i]->getScale().y, (int)OrangeDoors[i]->getPositionX(), (int)OrangeDoors[i]->getPositionY());
	}
}

void CSceneGame2::RenderLevel3()
{
	RenderLevel2();

	
	if (castedPurple)
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_PURPLE], false, (int)(m_cMap->GetTileSize() * 0.5), (int)(m_cMap->GetTileSize() * 0.5), (int)(theHero->getPositionX() + m_cMap->GetTileSize() - 10), (int)(theHero->getPositionY() - m_cMap->GetTileSize() + 10));

	for (int i = 0; i < (int)PurpleDoors.size(); i++)
	{
		if (PurpleDoors[i]->getActive())
			sceneManager2D.Render2DMesh(PurpleDoors[i]->getMesh(), false, (int)PurpleDoors[i]->getScale().x, (int)PurpleDoors[i]->getScale().y, (int)PurpleDoors[i]->getPositionX(), (int)PurpleDoors[i]->getPositionY());
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
	for (int i = 0; i < 10; i++)
	{
		sceneManager2D.Render2DMesh(theArrayOfGoodies[i]->GetMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), theArrayOfGoodies[i]->GetPos_x(), theArrayOfGoodies[i]->GetPos_y());
	}
}

void CSceneGame2::RenderWaypoints()
{
	for (int i = 0; i < (int)temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)(temp.at(i).x - theHero->GetMapOffset_x()), (int)(temp.at(i).y + theHero->GetMapOffset_y()));
	}
}

void CSceneGame2::RenderGUI()
{
	//On screen text
	std::ostringstream ss;

	sceneManager2D.Render2DMesh(meshList[GEO_JELLYBEAN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());
	ss.str(std::string());
	ss.precision(3);
	ss << ": " << noOfJellybeans;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(1, 1, 1), (float)m_cMap->GetTileSize(), (float)m_cMap->GetTileSize(), (float)sceneManager2D.m_window_height - m_cMap->GetTileSize());

	if (castedBlue)
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADEON], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot1.x, (int)slot1.y);
	else
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot1.x, (int)slot1.y);

	if (castedYellow)
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADEON], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot2.x, (int)slot2.y);
	else
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot2.x, (int)slot2.y);

	if (castedRed)
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADEON], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot3.x, (int)slot3.y);
	else
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot3.x, (int)slot3.y);
	
	if (castedPurple)
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADEON], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot4.x, (int)slot4.y);
	else
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot4.x, (int)slot4.y);

	if (castedGreen)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADEON], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot5.x, (int)slot5.y);
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_GREEN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot5.x, (int)slot5.y);
	}
	else
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot5.x, (int)slot5.y);

	if (castedOrange)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADEON], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot6.x, (int)slot6.y);
		sceneManager2D.Render2DMesh(meshList[GEO_COLOUR_BALL_ORANGE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot6.x, (int)slot6.y);
	}
	else
		sceneManager2D.Render2DMesh(meshList[GEO_BACKFADE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot6.x, (int)slot6.y);


	for (int i = 0; i < (int)ColoursSet.size(); i++)
	{
		if (!ColoursSet[i]->getActive())
		{
			if (ColoursSet[i]->getColour() == "BLUE")
			{
				sceneManager2D.Render2DMesh(ColoursSet[i]->getMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot1.x, (int)slot1.y);
				ss.str(std::string());
				ss << "Q";
				sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), (float)m_cMap->GetTileSize(), slot1.x, slot1.y);
			}
			if (ColoursSet[i]->getColour() == "YELLOW")
			{
				sceneManager2D.Render2DMesh(ColoursSet[i]->getMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot2.x, (int)slot2.y);
				ss.str(std::string());
				ss << "E";
				sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), (float)m_cMap->GetTileSize(), slot2.x, slot2.y);
			}
			if (ColoursSet[i]->getColour() == "RED")
			{
				sceneManager2D.Render2DMesh(ColoursSet[i]->getMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot3.x, (int)slot3.y);
				ss.str(std::string());
				ss << "R";
				sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), (float)m_cMap->GetTileSize(), slot3.x, slot3.y);
			}
			if (ColoursSet[i]->getColour() == "PURPLE")
			{
				sceneManager2D.Render2DMesh(ColoursSet[i]->getMesh(), false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), (int)slot4.x, (int)slot4.y);
				ss.str(std::string());
				ss << "T";
				sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), (float)m_cMap->GetTileSize(), slot4.x, slot4.y);
			}
		}
	}

	switch (currentState)
	{
	case PLAYING:
	{
		UIManager->Render(sceneManager2D);
		
		for (int i = 0; i < lives; i++)
		{
			// Lives left
			sceneManager2D.Render2DMesh(meshList[GEO_HEART], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0 + i*(m_cMap->GetTileSize()), sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 2));
		}

		for (int i = 0; i < (int)dialogueTiles.size(); i++)
		{
			if (dialogueTiles[i]->getActive())
			{
				// Dialogue box
				sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, sceneManager2D.m_window_width, m_cMap->GetTileSize(), (int)(sceneManager2D.m_window_width * 0.5), (int)(m_cMap->GetTileSize() * 0.5));

				// Text
				int textSize = (int)(m_cMap->GetTileSize() * 0.5f);
				sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], dialogueTiles[i]->getDialogue(), Color(0, 0, 0), (float)textSize, 0, textSize * 0.5f);
				break;
			}
		}
	}
		break;
	
	case WIN: case LOSE: case PREPARING:
		UIManager->Render(sceneManager2D);
		break;

	}

	if (level != CDifficultySystem::TUTORIAL)
	{
		switch (currentState)
		{
		case WIN:
		{
			int textSize = m_cMap->GetTileSize();
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], to_string(noOfJellybeansMayWin), Color(1, 1, 1), (float)textSize, UIManager->FindImage("WinScreen")->getCurrentPos().x + UIManager->FindImage("WinScreen")->getScale().x * 0.195f, UIManager->FindImage("WinScreen")->getCurrentPos().y - UIManager->FindImage("WinScreen")->getScale().y * 0.28f);
		}
		break;
		case LOSE:
		{
			int textSize = m_cMap->GetTileSize();
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "0", Color(1, 1, 1), (float)textSize, UIManager->FindImage("LoseScreen")->getCurrentPos().x + UIManager->FindImage("LoseScreen")->getScale().x * 0.195f, UIManager->FindImage("LoseScreen")->getCurrentPos().y - UIManager->FindImage("LoseScreen")->getScale().y * 0.28f);
		}
		break;
		}
	}
}