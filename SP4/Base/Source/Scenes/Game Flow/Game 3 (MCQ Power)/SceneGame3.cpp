#include "SceneGame3.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

#include "..\..\..\UsingLua.h"

CSceneGame3::CSceneGame3(const int m_window_width, const int m_window_height)
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
	, unlock(false)
	, scriptCorrect("")
	, scriptExit("")
	, scriptFinished("")
	, scriptWrong("")
{
}

CSceneGame3::~CSceneGame3()
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
	for (int i = 0; i < theAnswers.size(); i++)
	{
		if (theAnswers[i])
		{
			delete theAnswers[i];
			theAnswers[i] = NULL;
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

void CSceneGame3::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	//Read a value from the lua text file
	UseLuaFiles L;

	if (level == 0)
		L.ReadFiles("Lua//Scene/Game3/tutorial.lua");
	else if (level == 1)
		L.ReadFiles("Lua//Scene/Game3/easy.lua");

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

	if (level == 1)
	{
		QnDialogue.push_back(L.DoLuaString("scriptQn3"));
		for (int i = 0; i < 3; i++)
		{
			AnsDialogue.push_back(L.DoLuaString("scriptAnswerQn3Type" + to_string(i)));
		}
	}

	// Initialise and load the tile map
	m_cMap = new CMap();
	if (level == 0)
	{
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 33 * tileSize, 16 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 3/Tutorial.csv");
	}
	else if (level == 1)
	{
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 61 * tileSize, 16 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 3/Easy.csv");
	}

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
				//theDoor = new CDoor(CObjects::DOOR, 1, Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]);
				//theDoors.push_back(new CDoor())
				theDoors.push_back(new CDoor(CObjects::DOOR, Dcount, Vector3(k*m_cMap->GetTileSize(), (m_cMap->GetNumOfTiles_Height() - i)*m_cMap->GetTileSize()), Vector3(tileSize, tileSize, 1), meshList[GEO_TILE_DOOR]));
				theDoors.back()->setActive(true);
				Dcount++;
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
				if (Acount == 1 || Acount == 4  || Acount == 7)
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

void CSceneGame3::PreInit()
{
	sceneManager2D.PreInit();
}

/********************************************************************************
Initialise the meshes.
********************************************************************************/
void CSceneGame3::InitMeshes()
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

void CSceneGame3::Update(double dt)
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
								unlock = true;
								theQuestions[i]->setInteractivity(false);		//disable the question so there won't be unnecessary lives lost
								//theDoors[0]->setActive(false);
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
		if (lives == 0)
			currentState = FAILED;

		for (int i = 0; i < theDoors.size(); i++)
		{
			// Check Door
			if (theDoors[i]->getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
			{
				//meaning to say you can now exit the game
				if (!theQuestions[theQuestions.size()-1]->getInteractivity())
				{
					currentState = EXITING;
					// Animation
					theHero->SetAnimationDirection(CPlayerInfo::RIGHT);

				}
				if (theDoors[i]->getActive())
				{
					theHero->setPosition(prevHeroPos);
				}
			}
			//when player gets the correct answer
			if (unlock)
			{
				//unlock the first door
				if (theDoors[0]->getActive())
				{
					theDoors[0]->setActive(false);
					unlock = false;
					cout << "Door " << theDoors[0]->getId() << " unlocked";
				}
				//unlock the even number doors
				else if (i % 2 == 0 && i != 1)
				{
					if (theDoors[i]->getActive())
					{
						theDoors[i]->setActive(false);
						unlock = false;
						cout << "Door " << theDoors[i]->getId() << " unlocked";
					}
				}
				
			}
		}
	}
	break;
		case EXITING:
		{
			// Translate hero position toward door
			Vector3 theOldHeroPosition(theHero->getPositionX(), theHero->getPositionY(), 0);
			Vector3 theNewHeroPosition(theOldHeroPosition);
			Vector3 theTargetPosition(theDoors[theDoors.size() - 1]->getPositionX(), theDoors[theDoors.size() - 1]->getPositionY(), 0);

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


	// ReCalculate the tile offsets
	tileOffset_x = (int)(theHero->GetMapOffset_x() / m_cMap->GetTileSize());
	// ReCalculate the tile offsets
	tileOffset_y = (int)(theHero->GetMapOffset_y() / m_cMap->GetTileSize());

	fps = 1 / dt;
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneGame3::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneGame3::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}


/********************************************************************************
Render this scene
********************************************************************************/
void CSceneGame3::Render()
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
void CSceneGame3::Exit()
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
void CSceneGame3::RenderTileMap()
{
	int m = 0;
	int j = 0;
	
	//render according to the height and width of screen, you could put it as occlusion. :)
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

			else if (m_cMap->theScreenMap[j][m] == 30)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_DOOR], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), m*m_cMap->GetTileSize(), sceneManager2D.m_window_height - j*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[j][m] == 41)
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_QN], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), m*m_cMap->GetTileSize(), sceneManager2D.m_window_height - j*m_cMap->GetTileSize());
			}
			else
			{
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_GROUND], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), m*m_cMap->GetTileSize(), sceneManager2D.m_window_height - j*m_cMap->GetTileSize());
			}
		}
	}
	for (int i = 0; i < theAnswers.size(); i++)
	{
		//render the object if it is within range
		if ((theAnswers[i]->getPosition() - theHero->getPosition()).Length() <= m_cMap->GetScreenWidth())
		{	
			//if it isn't picked up yet
			if (!theAnswers[i]->getPickup())
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_ANS], false, theAnswers[i]->getScaleX(), theAnswers[i]->getScaleY(), theAnswers[i]->getPositionX(), theAnswers[i]->getPositionY());
			else
				sceneManager2D.Render2DMesh(meshList[GEO_TILE_ANS], false, m_cMap->GetTileSize() * 0.5, m_cMap->GetTileSize() * 0.5, theHero->getPositionX(), theHero->getPositionY() + m_cMap->GetTileSize());
		}
	}
}

/********************************************************************************
Render the Hero. This is a private function for use in this class only
********************************************************************************/
void CSceneGame3::RenderHero()
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
void CSceneGame3::RenderGUI()
{
	// Jellybean
	sceneManager2D.Render2DMesh(JellybeanSystem->mesh, false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());

	//On screen text
	std::ostringstream ss;
	// Jellybean
	ss.precision(3);
	ss << ": " << JellybeanSystem->GetNumOfJellybeans();
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0, 1, 0), m_cMap->GetTileSize(), m_cMap->GetTileSize(), sceneManager2D.m_window_height - m_cMap->GetTileSize());

	//On screen text
	std::ostringstream ss1;
	// Jellybean
	ss1.precision(3);
	ss1 << "FPS: " << fps;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss1.str(), Color(0, 1, 0), m_cMap->GetTileSize(), m_cMap->GetTileSize(), sceneManager2D.m_window_height - (m_cMap->GetTileSize() * 3));

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
		sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], scriptFinished, Color(0, 0, 0), m_cMap->GetTileSize(), sceneManager2D.m_window_width * 0.15, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 4);
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
void CSceneGame3::RenderAIs()
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
void CSceneGame3::RenderWaypoints()
{
	for (int i = 0; i < temp.size(); i++)
	{
		sceneManager2D.Render2DMesh(meshList[GEO_TILE_KILLZONE], false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), temp.at(i).x - theHero->GetMapOffset_x(), temp.at(i).y + theHero->GetMapOffset_y());
	}
}
