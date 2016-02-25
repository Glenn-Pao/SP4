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

	// Jellybeans
	JellybeanSystem = new CJellybeanSystem;

	// Initialise the Meshes
	InitMeshes();

	//DrawPile
	Trigger* DrawTrigger = new Trigger(meshList[GEO_DRAW],Vector3(100,225,1),Vector3(50,50,1));
	Deck* DrawPile = new Deck(Deck::Deck_Type::DRAW, Vector3(100, 100, 1), Vector3(0, 5, 0), DrawTrigger);
	DrawPile->AddCard(new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_BLUE_CARD], Card::ON_DRAW, Card::WATER));
	DrawPile->AddCard(new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_RED_CARD], Card::ON_DRAW, Card::FIRE));
	DrawPile->AddCard(new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_GREEN_CARD], Card::ON_DRAW, Card::LEAF));
	DrawPile->AddCard(new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_GREEN_CARD], Card::ON_DRAW, Card::LEAF));
	DeckList.push_back(DrawPile);

	//HandPile 

	//Red
	Trigger* SelectTriggerR = new Trigger(meshList[GEO_SELECT], Vector3(300, 200, 1), Vector3(50, 50, 1));
	Deck* RedHandPile = new Deck(Deck::Deck_Type::HANDR, Vector3(300, 75, 1), Vector3(0, -5, 0), SelectTriggerR);
	DeckList.push_back(RedHandPile);

	//Green
	Trigger* SelectTriggerG = new Trigger(meshList[GEO_SELECT], Vector3(400, 200, 1), Vector3(50, 50, 1));
	Deck* GreenHandPile = new Deck(Deck::Deck_Type::HANDG, Vector3(400, 75, 1), Vector3(0, -5, 0), SelectTriggerG);
	DeckList.push_back(GreenHandPile);

	//Blue
	Trigger* SelectTriggerB = new Trigger(meshList[GEO_SELECT], Vector3(500, 200, 1), Vector3(50, 50, 1));
	Deck* BlueHandPile = new Deck(Deck::Deck_Type::HANDB, Vector3(500, 75, 1), Vector3(0, -5, 0), SelectTriggerB);
	DeckList.push_back(BlueHandPile);

	//DiscardPile
	Trigger* PlaceTrigger = new Trigger(meshList[GEO_PLACE], Vector3(300, 275, 1), Vector3(50, 50, 1));
	Deck* DiscardPile = new Deck(Deck::Deck_Type::DISCARD, Vector3(350, 250, 1), Vector3(0, -5, 0), PlaceTrigger);
	DeckList.push_back(DiscardPile);

	isStandingOnTrigger = false;
	isCardDrawn = false;
	isCardSelectedR = false;
	isCardSelectedG = false;
	isCardSelectedB = false;
	isPlaced = false;
	SelectedCard = new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::ON_DRAG, Card::Element::NONE);
	
	StressButton = new Trigger(meshList[GEO_STRESS], Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 2, 1), Vector3(50, 50, 1));
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
	meshList[GEO_RED_CARD] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_RED_CARD]->textureID = LoadTGA("Image//RedCard.tga");
	meshList[GEO_BLUE_CARD] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_BLUE_CARD]->textureID = LoadTGA("Image//BlueCard.tga");
	meshList[GEO_GREEN_CARD] = MeshBuilder::Generate2DMesh("GEO_TILE_DOOR", Color(1, 1, 1), 0, 0, 1, 1);
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
				
					// Timer
					timer -= dt;
			
					for (int i = 0; i < DeckList.size(); ++i)
					{
						switch (DeckList[i]->getDeckType())
						{
							// if deck is a draw deck
						case Deck::DRAW:
						{
								
							// if draw deck's trigger is triggered
							if (DeckList[i]->CheckTrigger((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isCardDrawn == false && DeckList[i]->getListOfCards().size() > 0)
							{
								isStandingOnTrigger = true;

								if (isCardDrawn == false)
								{
									isCardDrawn = true;
									
										//Search for a hand deck
										for (int j = 0; j < DeckList.size(); ++j)
										{
											if (DeckList[j]->getDeckType() != Deck::DRAW && DeckList[j]->getDeckType() != Deck::DISCARD && DeckList[j]->getListOfCards().size() < 5)
											{
												
												if (DeckList[j]->getDeckType() == Deck::HANDR && DeckList[i]->getListOfCards().back()->getElement_Type() == Card::Element::FIRE)
												{

													DeckList[i]->getListOfCards().back()->setCurrentState_Type(Card::ON_HAND);
													DeckList[j]->AddCard(DeckList[i]->getListOfCards().back());
													//remove card from draw deck
													DeckList[i]->RemoveCard(DeckList[i]->getListOfCards().size() - 1);
													break;
												}
												if (DeckList[j]->getDeckType() == Deck::HANDB && DeckList[i]->getListOfCards().back()->getElement_Type() == Card::Element::WATER)
												{
													DeckList[i]->getListOfCards().back()->setCurrentState_Type(Card::ON_HAND);
													DeckList[j]->AddCard(DeckList[i]->getListOfCards().back());
													//remove card from draw deck
													DeckList[i]->RemoveCard(DeckList[i]->getListOfCards().size() - 1);
													break;

												}
												if (DeckList[j]->getDeckType() == Deck::HANDG && DeckList[i]->getListOfCards().back()->getElement_Type() == Card::Element::LEAF)
												{

													DeckList[i]->getListOfCards().back()->setCurrentState_Type(Card::ON_HAND);
													DeckList[j]->AddCard(DeckList[i]->getListOfCards().back());
													//remove card from draw deck
													DeckList[i]->RemoveCard(DeckList[i]->getListOfCards().size() - 1);
													break;
												}
											}
										}

								}
								
							}
							if (DeckList[i]->getTrigger()->CheckCollision((*theHero->getBoundingBox())) == false)
							{
								isCardDrawn = false;
								isStandingOnTrigger = false;
							}
							
							break;
						}
						case Deck::HANDR:
						{
							if (DeckList[i]->CheckTrigger((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isCardSelectedR == false && DeckList[i]->getListOfCards().size() > 0 && SelectedCard->getElement_Type() == Card::Element::NONE)
							{
								isStandingOnTrigger = true;

								if (isCardSelectedR == false)
								{
									isCardSelectedR = true;
									SelectedCard = DeckList[i]->getListOfCards()[DeckList[i]->getListOfCards().size() - 1];
									DeckList[i]->RemoveCard(DeckList[i]->getListOfCards().size() - 1);
								}
							}
							if (DeckList[i]->getTrigger()->CheckCollision((*theHero->getBoundingBox())) == false)
							{
								isCardSelectedR = false;
								isStandingOnTrigger = false;
							}
							break;
						}
						case Deck::HANDG:
						{

							if (DeckList[i]->CheckTrigger((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isCardSelectedG == false && DeckList[i]->getListOfCards().size() > 0 && SelectedCard->getElement_Type() == Card::Element::NONE)
							{
								isStandingOnTrigger = true;

								if (isCardSelectedG == false)
								{
									isCardSelectedG = true;
									SelectedCard = DeckList[i]->getListOfCards()[DeckList[i]->getListOfCards().size()-1];
									DeckList[i]->RemoveCard(DeckList[i]->getListOfCards().size() - 1);
								}
							}
							if (DeckList[i]->getTrigger()->CheckCollision((*theHero->getBoundingBox())) == false)
							{
								isCardSelectedG = false;
								isStandingOnTrigger = false;
							}
							break;
						}
						case Deck::HANDB:
						{
							cout << DeckList[i]->getListOfCards().size() << endl;
							if (DeckList[i]->CheckTrigger((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isCardSelectedB == false && DeckList[i]->getListOfCards().size() > 0 && SelectedCard->getElement_Type() == Card::Element::NONE)
							{
								isStandingOnTrigger = true;

								if (isCardSelectedB == false)
								{
									isCardSelectedB = true;
									SelectedCard = DeckList[i]->getListOfCards()[DeckList[i]->getListOfCards().size() - 1];
									DeckList[i]->RemoveCard(DeckList[i]->getListOfCards().size()-1);
								}
							}
							if (DeckList[i]->getTrigger()->CheckCollision((*theHero->getBoundingBox())) == false)
							{
								isCardSelectedB = false;
								isStandingOnTrigger = false;
							}
							break;
						}
						case Deck::DISCARD:
						{
							if (DeckList[i]->CheckTrigger((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isPlaced == false && SelectedCard->getElement_Type() != Card::Element::NONE)
							{
								isStandingOnTrigger = true;

								if (isPlaced == false)
								{
									if (DeckList[i]->getListOfCards().size() > 0)
									{
										switch (SelectedCard->getElement_Type())
										{
										case Card::FIRE:
										{
											if (DeckList[i]->getListOfCards().back()->getElement_Type() == Card::LEAF)
											{
												isPlaced = true;
												DeckList[i]->AddCard(SelectedCard);
												SelectedCard = new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::ON_DRAG, Card::Element::NONE);

											}
											break; 
										}
										case Card::LEAF:
										{
											if (DeckList[i]->getListOfCards().back()->getElement_Type() == Card::WATER)
											{
												isPlaced = true;
												DeckList[i]->AddCard(SelectedCard);
												SelectedCard = new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::ON_DRAG, Card::Element::NONE);

											}
											break;
										}
										case Card::WATER:
										{
											if (DeckList[i]->getListOfCards().back()->getElement_Type() == Card::FIRE)
											{
												isPlaced = true;
												DeckList[i]->AddCard(SelectedCard);
												SelectedCard = new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::ON_DRAG, Card::Element::NONE);

											}
											break;
										}
										}
									}
									else
									{
										isPlaced = true;
										DeckList[i]->AddCard(SelectedCard);
										SelectedCard = new Card(Card::CARD, true, "NIL", Vector3(300, 200, 1), Vector3(0, 0, 0), Vector3(75, 100, 1), meshList[GEO_STRESS_CARD], meshList[GEO_STRESS_CARD], Card::ON_DRAG, Card::Element::NONE);
									}
									}
							}
							if (DeckList[i]->getTrigger()->CheckCollision((*theHero->getBoundingBox())) == false)
							{
								isPlaced = false;
								isStandingOnTrigger = false;
							}
							break;
						}
						}
					}
					////if Card is Selected
					//if (CardSelectRed->CheckCollision((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isCardSelected == false && HandPileRed->getListOfCards().size() > 0 && SelectedCard->getElement_Type() == Card::Element::NONE)
					//{
					//	isStandingOnTrigger = true;
					//	if (isCardSelected == false)
					//	{
					//		isCardSelected = true;
					//		SelectedCard = HandPileRed->getListOfCards().back();
					//		SelectedCard->setCurrentState_Type(Card::State::ON_DRAG);
					//		HandPileRed->RemoveCard(HandPileRed->getListOfCards().size() - 1);
					//	}
					//}
					//if (CardSelectRed->CheckCollision((*theHero->getBoundingBox())) == false)
					//{
					//	isCardSelected = false;
					//	isStandingOnTrigger = false;
					//}
					//if (CardPlaced->CheckCollision((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isCardPlaced == false)
					//{
					//	isStandingOnTrigger = true;
					//	if (isCardPlaced == false)
					//	{
					//		isCardPlaced = true;
					//		DiscardPile->AddCard(SelectedCard);
					//		SelectedCard->setElement_Type(Card::NONE);
					//	}
					//}
					//if (CardSelectRed->CheckCollision((*theHero->getBoundingBox())) == false)
					//{
					//	isCardPlaced = false;
					//	isStandingOnTrigger = false;
					//}

					//// If Card Not Drawn
					//if (CardDraw->CheckCollision((*theHero->getBoundingBox())) == true && isStandingOnTrigger == false && isCardDrawn == false && DrawPile->getListOfCards().size() > 0)
					//{
					//	isStandingOnTrigger = true;

					//	if (isCardDrawn == false)
					//	{
					//		isCardDrawn = true;
					//		
					//		//Change Card State If Drawn And Remove From Draw Pile Before Adding Card To Hand Pile
					//		DrawPile->getListOfCards().back()->setCurrentState_Type(Card::State::ON_HAND);
					//		switch (DrawPile->getListOfCards().back()->getElement_Type())
					//		{
					//		case Card::Element::FIRE:
					//		{
					//			HandPileRed->AddCard(DrawPile->getListOfCards().back());
					//			break;
					//		}
					//		case Card::Element::LEAF:
					//		{
					//			HandPileGreen->AddCard(DrawPile->getListOfCards().back());
					//			break;
					//		}
					//		case Card::Element::WATER:
					//		{
					//			HandPileBlue->AddCard(DrawPile->getListOfCards().back());
					//			break;
					//		}
					//		}
					//		DrawPile->RemoveCard(DrawPile->getListOfCards().size() - 1);
					//	}

					//}
					//if (CardDraw->CheckCollision((*theHero->getBoundingBox())) == false)
					//{
					//	isCardDrawn = false;
					//	isStandingOnTrigger = false;
					//}
	}
		break;
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
	// Render Hero
	RenderHero();
	// Render AIs
	RenderAIs();
	//Render Cards
	/*for (int i = 0; i < DrawPile->getListOfCards().size(); ++i)
	{
		RenderCard(DrawPile->getListOfCards()[i]);
	}
	for (int i = 0; i < HandPileRed->getListOfCards().size(); ++i)
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
	for (int i = 0; i < DeckList.size(); ++i)
	{
		for (int j = 0; j < DeckList[i]->getListOfCards().size(); ++j)
		{
			switch (DeckList[i]->getListOfCards()[j]->getCurrentState_Type())
			{
			case Card::State::ON_DRAW:
			{
				sceneManager2D.Render2DMesh(
					DeckList[i]->getListOfCards()[j]->getMesh(), 
					false, 
					DeckList[i]->getListOfCards()[j]->getScaleX(),
					DeckList[i]->getListOfCards()[j]->getScaleY(),
					DeckList[i]->getListOfCards()[j]->getPositionX(),
					DeckList[i]->getListOfCards()[j]->getPositionY(),
					0);
				break;
			}
			default:
			{
				sceneManager2D.Render2DMesh(
					DeckList[i]->getListOfCards()[j]->getCardFaceUpMesh(),
					false,
					DeckList[i]->getListOfCards()[j]->getScaleX(),
					DeckList[i]->getListOfCards()[j]->getScaleY(),
					DeckList[i]->getListOfCards()[j]->getPositionX(),
					DeckList[i]->getListOfCards()[j]->getPositionY(),
					0);
				break;
				}
			}
			
		}
		sceneManager2D.Render2DMesh(
			DeckList[i]->getTrigger()->getMesh(),
			false,
			DeckList[i]->getTrigger()->getScale().x,
			DeckList[i]->getTrigger()->getScale().y,
			DeckList[i]->getTrigger()->getPosition().x,
			DeckList[i]->getTrigger()->getPosition().y,
			0);
	}
	sceneManager2D.Render2DMesh(SelectedCard->getCardFaceUpMesh(), false, 30, 40, theHero->getPositionX()-15, theHero->getPositionY(), 0);

	sceneManager2D.modelStack.PopMatrix();
	////Render Triggers
	//sceneManager2D.Render2DMesh(CardDraw->getMesh(), false, CardDraw->getScale().x, CardDraw->getScale().y, CardDraw->getPosition().x, CardDraw->getPosition().y, 0);
	//sceneManager2D.Render2DMesh(CardSelectRed->getMesh(), false, CardSelectRed->getScale().x, CardSelectRed->getScale().y, CardSelectRed->getPosition().x, CardSelectRed->getPosition().y, 0);
	sceneManager2D.Render2DMesh(StressButton->getMesh(), false, StressButton->getScale().x, StressButton->getScale().y, StressButton->getPosition().x, StressButton->getPosition().y, 0);

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

	// Delete JellybeanSystem
	if (JellybeanSystem)
		delete JellybeanSystem;
}


/********************************************************************************
Handle to the GUI
********************************************************************************/
void SceneGame4::RenderGUI()
{
	// Jellybean
	sceneManager2D.Render2DMesh(JellybeanSystem->mesh, false, m_cMap->GetTileSize(), m_cMap->GetTileSize(), 0, sceneManager2D.m_window_height - m_cMap->GetTileSize());

	//On screen text
	std::ostringstream ss;
	// Jellybean
	ss.precision(3);
	ss << ": " << JellybeanSystem->GetNumOfJellybeans();
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
