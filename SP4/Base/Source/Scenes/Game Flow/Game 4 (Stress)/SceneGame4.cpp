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
: currentState(PLAY)
, timer(30.0f)
{
}

SceneGame4::~SceneGame4()
{
}

void SceneGame4::Init(int level) // level = 0(Tutorial), = 1(Easy), = 2(Medium), = 3(Hard)
{
	// Init the base scene
	sceneManager2D.Init(level);

	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//Scene/Game1/tutorial.lua");

	int tileSize = L.DoLuaInt("tileSize");

	// Dialogues scripts
	vector<string> scriptDialogues;


	switch (level)
	{
	case 0:
	{
		for (int i = 0; i < 6; i++)
		{
			scriptDialogues.push_back(L.DoLuaString("script" + to_string(i)));
		}

		// Initialise and load the tile map
		m_cMap = new CMap();
		m_cMap->Init(sceneManager2D.m_window_height, sceneManager2D.m_window_width, 12, 16, 13 * tileSize, 16 * tileSize, tileSize);
		m_cMap->LoadMap("Image//Maps//Game 4/Tutorial.csv");
	}
	break;
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
			// Door
			else if (m_cMap->theScreenMap[i][k] == 30)
			{
			}
			// Tutorial
			if (level == 0)
			{

			}
		}
	}

	// Initialise the Meshes
	InitMeshes();

	PatternInserted = new Deck(Vector3(200, 200, 1), Vector3(50, 0, 0));
	
	PatternToFollow = new Deck(Vector3(200, 300,1), Vector3(50,0,0));


	

	for (int i = 0; i < 3; ++i)
	{
		ps.AddProbability(1);
	}

	for (int i = 0; i < 8; ++i)
	{
		//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
		PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));

		int randNo = ps.GetARandIntProbability();

		switch(randNo)
		{
		case 0:
		{
			PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE,true));
			break;
		}
		case 1:
		{
			PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true));
			break;
		}
		case 2:
		{
			PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true));
			break;
		}
		}
	}

	RedPile = new Card(Card::CARD, true, "NIL", Vector3(200,100, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true);
	
	GreenPile = new Card(Card::CARD, true, "NIL", Vector3(300, 100, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true);
	
	BluePile = new Card(Card::CARD, true, "NIL", Vector3(400, 100, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true);
	
	SelectedCard = new  Card(Card::CARD, false, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::Element::NONE, true);

	NoneCard = new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::Element::NONE, true);

	ScoreToBeat = 10;
	Score = 0;

	SendPattern = new Trigger(meshList[GEO_STRESS],Vector3(100,200,1),Vector3(50,50,50));
}

void SceneGame4::PreInit()
{
	sceneManager2D.PreInit();
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
	meshList[GEO_DRAW] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_DRAW]->textureID = LoadTGA("Image//DrawCard.tga");
	meshList[GEO_SELECT] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_SELECT]->textureID = LoadTGA("Image//SelectCard.tga");
	meshList[GEO_PLACE] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_PLACE]->textureID = LoadTGA("Image//PlaceCard.tga");
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


}

void SceneGame4::Update(double dt)
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
	case PLAY:
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

		cout << Score << endl;

		//Update Decks
		if (Application::IsKeyPressed('F'))
		{
			if (SendPattern->CheckCollision((*theHero->getBoundingBox())) == true && PatternToFollow->isDeckIdentical(PatternInserted) == true)
			{
				for (int i = PatternToFollow->getListOfCards().size()-1; i >= 0; --i)
				{
					delete PatternToFollow->ListOfCards[i];
					PatternToFollow->ListOfCards.pop_back();
				}

				for (int i = PatternInserted->getListOfCards().size() - 1; i >= 0; --i)
				{
					delete PatternInserted->ListOfCards[i];
					PatternInserted->ListOfCards.pop_back();
				}

				for (int i = 0; i < 8; ++i)
				{
					//Add The Same Amount Of Card For PatternInserted According To PatternToFollow
					PatternInserted->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::NONE, false));

					int randNo = ps.GetARandIntProbability();

					switch (randNo)
					{
					case 0:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_RED_CARD], Card::Element::FIRE, true));
						break;
					}
					case 1:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::Element::WATER, true));
						break;
					}
					case 2:
					{
						PatternToFollow->AddCard(new Card(Card::CARD, true, "NIL", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(35, 50, 1), meshList[GEO_STRESS_CARD], meshList[GEO_GREEN_CARD], Card::Element::LEAF, true));
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
			if (RedPile->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
			{
				if (Application::IsKeyPressed('F'))
				{
					SelectedCard = RedPile;
				}
			}

			if (GreenPile->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
			{
				if (Application::IsKeyPressed('F'))
				{
					SelectedCard = GreenPile;
				}
				else
				{
				}
			}
			if (BluePile->getBoundingBox()->CheckCollision((*theHero->getBoundingBox())) == true)
			{
				if (Application::IsKeyPressed('F'))
				{
					SelectedCard = BluePile;
				}
				else
				{
				}
			}


		// Timer
		timer -= dt;
		if (timer < 0)
		{
			timer = 0; 
			currentState = STATE::TIME_UP;
		}
		break;
	}
	case STATE::TIME_UP:
	{
		if (Score >= ScoreToBeat)
		{
			currentState = STATE::WIN;
		}
		else
		{
			currentState = STATE::LOSE;
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


/********************************************************************************
Render this scene
********************************************************************************/
void SceneGame4::Render()
{
	sceneManager2D.Render();

	sceneManager2D.modelStack.PushMatrix();

	sceneManager2D.modelStack.Translate(-theHero->GetMapOffset_x(), theHero->GetMapOffset_y() - m_cMap->GetTileSize(), 0);
	//sceneManager2D.RenderBackground();

	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render AIs
	RenderAIs();
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

	//Render Pattern To Follow
	for (int i = 0; i < PatternToFollow->getListOfCards().size(); ++i)
	{
		if(PatternToFollow->getListOfCards()[i]->getisRevealed() == true)
		{
			sceneManager2D.Render2DMesh(PatternToFollow->getListOfCards()[i]->getCardFaceUpMesh(), false, PatternToFollow->getListOfCards()[i]->getScaleX(), PatternToFollow->getListOfCards()[i]->getScaleY(), PatternToFollow->getListOfCards()[i]->getPositionX(), PatternToFollow->getListOfCards()[i]->getPositionY(), 0);
		}
		else
		{
			sceneManager2D.Render2DMesh(PatternToFollow->getListOfCards()[i]->getMesh(), false, PatternToFollow->getListOfCards()[i]->getScaleX(), PatternToFollow->getListOfCards()[i]->getScaleY(), PatternToFollow->getListOfCards()[i]->getPositionX(), PatternToFollow->getListOfCards()[i]->getPositionY(), 0);
		}
	}
	sceneManager2D.Render2DMesh(meshList[GEO_PATTERN], false, 200, 30, PatternToFollow->getPosition().x, PatternToFollow->getPosition().y + 75, 0);

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


	//Render Cards
	
		if (RedPile->getisRevealed() == true)
		{
			sceneManager2D.Render2DMesh(RedPile->getCardFaceUpMesh(), false, RedPile->getScaleX(), RedPile->getScaleY(), RedPile->getPositionX(), RedPile->getPositionY(), 0);
		}
		else
		{
			sceneManager2D.Render2DMesh(RedPile->getMesh(), false, RedPile->getScaleX(), RedPile->getScaleY(), RedPile->getPositionX(), RedPile->getPositionY(), 0);
		}

	
		if (BluePile->getisRevealed() == true)
		{
			sceneManager2D.Render2DMesh(BluePile->getCardFaceUpMesh(), false, BluePile->getScaleX(), BluePile->getScaleY(), BluePile->getPositionX(), BluePile->getPositionY(), 0);
		}
		else
		{
			sceneManager2D.Render2DMesh(BluePile->getMesh(), false, BluePile->getScaleX(), BluePile->getScaleY(), BluePile->getPositionX(), BluePile->getPositionY(), 0);
		}
	
		if (GreenPile->getisRevealed() == true)
		{
			sceneManager2D.Render2DMesh(GreenPile->getCardFaceUpMesh(), false, GreenPile->getScaleX(), GreenPile->getScaleY(), GreenPile->getPositionX(), GreenPile->getPositionY(), 0);
		}
		else
		{
			sceneManager2D.Render2DMesh(GreenPile->getMesh(), false, GreenPile->getScaleX(), GreenPile->getScaleY(), GreenPile->getPositionX(), GreenPile->getPositionY(), 0);
		}
	//Render Card With Player;

		if (SelectedCard->getActive() == true)
		{
			sceneManager2D.Render2DMesh(SelectedCard->getCardFaceUpMesh(), false, SelectedCard->getScaleX(), SelectedCard->getScaleY(), theHero->getPositionX(), theHero->getPositionY() + 50, 0);
		}

		// Render Hero
		RenderHero();

		//Render Trigger
		sceneManager2D.Render2DMesh(SendPattern->getMesh(), false, SendPattern->getScale().x, SendPattern->getScale().y, SendPattern->getPosition().x, SendPattern->getPosition().y, 0);

		sceneManager2D.modelStack.PopMatrix();

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
	
	// Render GUI
	RenderGUI();
	
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

	// Timer
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], "Time left:", Color(0.5, 0.3, 0.3), m_cMap->GetTileSize() * 0.5, sceneManager2D.m_window_width - m_cMap->GetTileSize() * 3, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 0.5);
	ss.str(std::string());
	ss.precision(3);
	ss << timer;
	sceneManager2D.RenderTextOnScreen(sceneManager2D.meshList[CSceneManager2D::GEO_TEXT], ss.str(), Color(0.5, 0.3, 0.3), m_cMap->GetTileSize(), sceneManager2D.m_window_width - m_cMap->GetTileSize() * 3, sceneManager2D.m_window_height - m_cMap->GetTileSize() * 1.5);

	switch (currentState)
	{
	case PLAY:
	{
		break;
	}
	case PAUSE:
	{
		break;
	}

	case TIME_UP:
	{
					break;
	}
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
