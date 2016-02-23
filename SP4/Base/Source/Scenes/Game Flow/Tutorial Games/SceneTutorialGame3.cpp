#include "SceneTutorialGame3.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

#include "..\..\..\UsingLua.h"

CSceneTutorialGame3::CSceneTutorialGame3(const int m_window_width, const int m_window_height)
	: currentState(PLAYING)
	, lives(3)
	, Qcount(0)
	, Acount(0)
	, Dcount(0)
	, timer(0)
	, feedbacktimer(0)
	, press(false)
	, feedback(false)
	, correct(false)
	, scriptCorrect("")
	, scriptExit("")
	, scriptFinished("")
	, scriptWrong("")
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
	for (int i = 0; i < theDoors.size(); i++)
	{
		if (theDoors[i])
		{
			delete theDoors[i];
			theDoors[i] = NULL;
		}
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
}

void CSceneTutorialGame3::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//Scene/Game3/tutorial.lua");

	int tileSize = L.DoLuaInt("tileSize");
	scriptWrong = L.DoLuaString("scriptWrong");
	scriptCorrect = L.DoLuaString("scriptCorrect");
	scriptFinished = L.DoLuaString("scriptFinished");
	scriptExit = L.DoLuaString("scriptExit");

	// Dialogues scripts
	vector<string> scriptDialogues, QnDialogue, AnsDialogue;
	for (int i = 0; i < 3; i++)
	{
		scriptDialogues.push_back(L.DoLuaString("script" + to_string(i)));
		AnsDialogue.push_back(L.DoLuaString("scriptAnswerQn1Type" + to_string(i)));
	}
	for (int i = 0; i < 3; i++)
	{
		AnsDialogue.push_back(L.DoLuaString("scriptAnswerQn2Type" + to_string(i)));
	}
	QnDialogue.push_back(L.DoLuaString("scriptQn1"));
	QnDialogue.push_back(L.DoLuaString("scriptQn2"));

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
				theHero = new CPlayerInfo(m_cMap);
				theHero->setPositionX(k*m_cMap->GetTileSize());
				theHero->setPositionY((m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize());
				theHero->setBoundingBox(Vector3(theHero->getPositionX() + (theHero->getLeftCollision() - 0.5) * m_cMap->GetTileSize(), theHero->getPositionY() + m_cMap->GetTileSize() * (theHero->getTopCollision() - 0.5)),
					Vector3(theHero->getPositionX() + (theHero->getRightCollision() - 0.5) * m_cMap->GetTileSize(), theHero->getPositionY() + m_cMap->GetTileSize() * (theHero->getBottomCollision() - 0.5)));
			}
			// Door
			else if (m_cMap->theScreenMap[i][k] == 30)
			{
				// Create a new door
				theDoor = new CDoor(CObjects::DOOR, 1, Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);
				//theDoors.push_back(new CDoor())
				//theDoors.push_back(new CDoor(CObjects::DOOR, Dcount, Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]));
				
			}
			// Questions
			else if (m_cMap->theScreenMap[i][k] == 41)
			{
				theQuestions.push_back(new CQuestion(CObjects::QUESTION, Qcount, false, true, QnDialogue[Qcount], Vector3(k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize(), 0), Vector3(0, 0, 0), Vector3(tileSize, tileSize, 1)));
				Qcount++;
			}
			// Possible answers
			else if (m_cMap->theScreenMap[i][k] == 42)
			{
				theAnswers.push_back(new CAnswer(CObjects::ANSWER, Acount, false, true, AnsDialogue[Acount], Vector3(k*m_cMap->GetTileSize(), sceneManager2D.m_window_height - i*m_cMap->GetTileSize(), 0), Vector3(0, 0, 0), Vector3(tileSize, tileSize, 1), false));

				//set the correct answers
				if (Acount == 1 || Acount == 4)
				{
					theAnswers.back()->setCorrect(true);
				}

				Acount++;
			}
			// Dialogue script 1
			else if (m_cMap->theScreenMap[i][k] == 49)
			{
				float pos_x = k*m_cMap->GetTileSize();
				float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[0], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
				Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			// Dialogue script 2
			else if (m_cMap->theScreenMap[i][k] == 47)
			{
				float pos_x = k*m_cMap->GetTileSize();
				float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[1], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
				Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
			}
			// Dialogue script 3
			else if (m_cMap->theScreenMap[i][k] == 48)
			{
				float pos_x = k*m_cMap->GetTileSize();
				float pos_y = (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize();

				dialogueTiles.push_back(new CObjects(CObjects::DIALOGUE, false, scriptDialogues[2], Vector3(pos_x, pos_y), Vector3(), Vector3(), NULL));
				Vector3 topleft(pos_x - (tileSize * 0.5), pos_y + (tileSize * 0.5), 0);
				Vector3 bottomright(pos_x + (tileSize * 0.5), pos_y - (tileSize * 0.5), 0);
				dialogueTiles.back()->setBoundingBox(topleft, bottomright);
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
	for (int i = 0; i < theAnswers.size(); i++)
	{
		theAnswers.back()->setMesh(meshList[GEO_TILE_ANS]);
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
	meshList[GEO_TILE_ANS] = MeshBuilder::Generate2DMesh("GEO_TILE_ANS", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_TILE_ANS]->textureID = LoadTGA("Image//tile42_ans.tga");
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
		if (theDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			theHero->setPosition(prevHeroPos);

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
		for (int i = 0; i < theQuestions.size(); i++)
		{
			//check collision between question and player
			if (theQuestions[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()) && theQuestions[i]->getInteractivity())
			{
				theQuestions[i]->setActive(true);
			}
			else
			{
				theQuestions[i]->setActive(false);
			}
			if (theQuestions[i]->getActive())
			{
				for (int j = 0; j < theAnswers.size(); j++)
				{
					//check if the player has already picked up something
					if (theAnswers[j]->getPickup())
					{
						//the item is not yet picked up so player should be able to pick it up
						if (Application::IsKeyPressed('F') && !press)
						{
							press = true;			//disable the recurring effect
							theAnswers[j]->setActive(false);
							theAnswers[j]->setInteractivity(false);	//it should'nt be able to be picked up again
							theAnswers[j]->setPickup(false);
							feedback = true;

							//check if it is the correct answer
							if (!theAnswers[j]->getCorrect())
							{
								lives -= 1;
								break;
							}
							else
							{
								correct = true;
								theQuestions[i]->setInteractivity(false);		//disable the question so there won't be unnecessary lives lost
							}
						}
					}
				}
			}
		}
		for (int i = 0; i < theAnswers.size(); i++)
		{
			//check collision between answer and player
			if ((theAnswers[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()) || theAnswers[i]->getPickup()) && theAnswers[i]->getInteractivity())
			{
				theAnswers[i]->setActive(true);
			}
			else
			{
				theAnswers[i]->setActive(false);
			}
			//the item is not yet picked up so player should be able to pick it up
			if (Application::IsKeyPressed('F') && !press)
			{
				if (theAnswers[i]->getActive() && !theAnswers[i]->getPickup())
				{
					press = true;			//disable the recurring effect
					theAnswers[i]->setPickup(true);
				}
				else if (theAnswers[i]->getActive() && theAnswers[i]->getPickup())
				{
					press = true;			//disable the recurring effect
					theAnswers[i]->setPickup(false);
				}
			}
			if (press)
			{
				timer += dt;
			}
			if (timer > 1)
			{
				press = false;
				timer = 0;
			}
			if (feedback)
			{
				feedbacktimer += dt;
			}
			if (feedbacktimer > 10)
			{
				feedback = false;
				correct = false;
				feedbacktimer = 0;
			}
			
		}
		// Check Door
		if (theDoor->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
		{
			//meaning to say you can now exit the game
			if (!theQuestions[0]->getInteractivity() && !theQuestions[1]->getInteractivity())
			{
				currentState = EXITING;
				// Animation
				theHero->SetAnimationDirection(CPlayerInfo::RIGHT);

			}
		}
		if (lives == 0)
			currentState = FAILED;
	}
	break;
		case EXITING:
		{
			// Translate hero position toward door
			Vector3 theOldHeroPosition(theHero->getPositionX(), theHero->getPositionY(), 0);
			Vector3 theNewHeroPosition(theOldHeroPosition);
			Vector3 theTargetPosition(theDoor->getPositionX(), theDoor->getPositionY(), 0);

			theNewHeroPosition += (theTargetPosition - theNewHeroPosition).Normalized() * theHero->GetMovementSpeed() * m_cMap->GetTileSize() * dt;

			if ((theOldHeroPosition - theTargetPosition).Length() > (theOldHeroPosition - theNewHeroPosition).Length())
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
	for (int i = 0; i < theAnswers.size(); i++)
	{
		//if it isn't picked up yet
		if (!theAnswers[i]->getPickup())
			sceneManager2D.Render2DMesh(meshList[GEO_TILE_ANS], false, theAnswers[i]->getScaleX(), theAnswers[i]->getScaleY(), theAnswers[i]->getPositionX(), theAnswers[i]->getPositionY());
		else
			sceneManager2D.Render2DMesh(meshList[GEO_TILE_ANS], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX(), theHero->getPositionY() + m_cMap->GetTileSize());
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
	for (int i = 0; i < theQuestions.size(); i++)
	{
		if (theQuestions[i]->getActive())
		{
			// Dialogue box
			sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, sceneManager2D.m_window_width, m_cMap->GetTileSize(), 0, 0);

			// Text
			int textSize = m_cMap->GetTileSize() * 0.5;
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], theQuestions[i]->getDialogue(), Color(0, 0, 0), textSize, 0, textSize * 2.5);
			break;
		}
	}
	for (int i = 0; i < theAnswers.size(); i++)
	{
		if (theAnswers[i]->getActive())
		{
			// Dialogue box
			sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, sceneManager2D.m_window_width, m_cMap->GetTileSize(), 0, 0);

			// Text
			int textSize = m_cMap->GetTileSize() * 0.5;
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], theAnswers[i]->getDialogue(), Color(0, 0, 0), textSize, 0, textSize * 0.5);
			break;
		}
	}
	if (feedback)
	{
		// Dialogue box
		sceneManager2D.Render2DMesh(meshList[GEO_DIALOGUE_BOX], false, sceneManager2D.m_window_width, m_cMap->GetTileSize(), 0, 0);
		//if it is the correct answer
		if (correct)
		{
			// Text
			int textSize = m_cMap->GetTileSize() * 0.5;
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptCorrect, Color(0, 0, 0), textSize, 0, textSize * 1.5);
		}
		else
		{
			// Text
			int textSize = m_cMap->GetTileSize() * 0.5;
			sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptWrong, Color(0, 0, 0), textSize, 0, textSize * 0.5);
		}
	}
	}
		break;
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
