#include "SceneMenu.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

CSceneMenu::CSceneMenu()
{
}

CSceneMenu::CSceneMenu(const int m_window_width, const int m_window_height)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
	
}

CSceneMenu::~CSceneMenu()
{
}

void CSceneMenu::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Menu.tga");

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 800, 55);
		
	meshList[START_BUTTON_UP] = MeshBuilder::GenerateQuad("START_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[START_BUTTON_UP]->textureID = LoadTGA("Image//StartGameButton.tga");

	meshList[START_BUTTON_DOWN] = MeshBuilder::GenerateQuad("START_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[START_BUTTON_DOWN]->textureID = LoadTGA("Image//StartGameButtonPressed.tga");

	meshList[LOAD_BUTTON_UP] = MeshBuilder::GenerateQuad("LOAD_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[LOAD_BUTTON_UP]->textureID = LoadTGA("Image//LoadGameButton.tga");

	meshList[LOAD_BUTTON_DOWN] = MeshBuilder::GenerateQuad("LOAD_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[LOAD_BUTTON_DOWN]->textureID = LoadTGA("Image//LoadGameButtonPressed.tga");

	meshList[INSTRUCTIONS_BUTTON_UP] = MeshBuilder::GenerateQuad("INSTRUCTIONS_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[INSTRUCTIONS_BUTTON_UP]->textureID = LoadTGA("Image//InstructionButton.tga");

	meshList[INSTRUCTIONS_BUTTON_DOWN] = MeshBuilder::GenerateQuad("INSTRUCTIONS_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[INSTRUCTIONS_BUTTON_DOWN]->textureID = LoadTGA("Image//InstructionButtonPressed.tga");

	meshList[EXIT_BUTTON_UP] = MeshBuilder::GenerateQuad("EXIT_BUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[EXIT_BUTTON_UP]->textureID = LoadTGA("Image//ExitButton.tga");

	meshList[EXIT_BUTTON_DOWN] = MeshBuilder::GenerateQuad("EXIT_BUTTON_DOWN", Color(0, 0, 0), 1.f);
	meshList[EXIT_BUTTON_DOWN]->textureID = LoadTGA("Image//ExitButtonPressed.tga");

	meshList[BACKGROUND] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[BACKGROUND]->textureID = LoadTGA("Image//MainMenuBackGround.tga");

	meshList[TITLE] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[TITLE]->textureID = LoadTGA("Image//MainMenuTitle.tga");

	meshList[SMOKE_LEFT] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[SMOKE_LEFT]->textureID = LoadTGA("Image//MainMenuSmokeLeft.tga");

	meshList[SMOKE_RIGHT] = MeshBuilder::GenerateQuad("PLAYBUTTON_UP", Color(0, 0, 0), 1.f);
	meshList[SMOKE_RIGHT]->textureID = LoadTGA("Image//MainMenuSmokeRight.tga");

	meshList[MUSIC_ICON] = MeshBuilder::GenerateQuad("MUSIC_ICON", Color(0, 0, 0), 1.f);
	meshList[MUSIC_ICON]->textureID = LoadTGA("Image//Music_Icon.tga");

	meshList[MUSIC_ICON_OFF] = MeshBuilder::GenerateQuad("MUSIC_ICON_OFF", Color(0, 0, 0), 1.f);
	meshList[MUSIC_ICON_OFF]->textureID = LoadTGA("Image//Music_Icon_Off.tga");
	
	choice = NONE;

	UIManager = new UISystem();

	Button* StartGameButton;
	StartGameButton = new Button("StartGameButton", meshList[START_BUTTON_UP], meshList[START_BUTTON_DOWN], NULL, Vector3(-100, sceneManager2D.m_window_height / 10 * 6, 1), Vector3(20, 10, 1));

	Button* LoadGameButton;
	LoadGameButton = new Button("LoadGameButton", meshList[LOAD_BUTTON_UP], meshList[LOAD_BUTTON_DOWN], NULL, Vector3(-100, sceneManager2D.m_window_height / 10 * 4.5, 1), Vector3(20, 10, 1));

	Button* Instructions;
	Instructions = new Button("Instructions", meshList[INSTRUCTIONS_BUTTON_UP], meshList[INSTRUCTIONS_BUTTON_DOWN], NULL, Vector3(-100, sceneManager2D.m_window_height / 10 * 3, 1), Vector3(20, 10, 1));


	Button* ExitButton;
	ExitButton = new Button("ExitButton", meshList[EXIT_BUTTON_UP], meshList[EXIT_BUTTON_DOWN], NULL, Vector3(-100, sceneManager2D.m_window_height / 10 * 1.5, 1), Vector3(20, 10, 1));

	Button* MusicIcon;
	MusicIcon = new Button("MusicIcon", meshList[MUSIC_ICON], meshList[MUSIC_ICON], NULL, Vector3(sceneManager2D.m_window_width - (sceneManager2D.m_window_width * 0.1), 100, 1), Vector3(100, 100, 1));

	Button* MusicIconOff;
	MusicIconOff = new Button("MusicIconOff", meshList[MUSIC_ICON_OFF], meshList[MUSIC_ICON_OFF], NULL, Vector3(sceneManager2D.m_window_width - (sceneManager2D.m_window_width * 0.1), 100, 1), Vector3(100, 100, 1), false);

	Image* Title;
	Title = new Image("Title", meshList[TITLE], Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height + 200, 0), Vector3(300, 150, 0));
	
	Image* SmokeToLeft;
	SmokeToLeft = new Image("SmokeToLeft", meshList[SMOKE_LEFT], Vector3(sceneManager2D.m_window_width / 10 * 4, sceneManager2D.m_window_height / 10 * 6.8), Vector3(0.1, 0.05, 0));

	Image* SmokeToLeftUp;
	SmokeToLeftUp = new Image("SmokeToLeftUp", meshList[SMOKE_LEFT], Vector3(sceneManager2D.m_window_width / 10 * 4, sceneManager2D.m_window_height / 10 * 6.8), Vector3(0.1, 0.05, 0));


	Image* SmokeToRight;
	SmokeToRight = new Image("SmokeToRight", meshList[SMOKE_RIGHT], Vector3(sceneManager2D.m_window_width / 10 * 6, sceneManager2D.m_window_height / 10 * 6.8), Vector3(0.1, 0.05, 0));

	Image* SmokeToRightUp;
	SmokeToRightUp = new Image("SmokeToRightUp", meshList[SMOKE_RIGHT], Vector3(sceneManager2D.m_window_width / 10 * 6, sceneManager2D.m_window_height / 10 * 6.8), Vector3(0.1, 0.05, 0));


	Image* BackGround;
	BackGround = new Image("BackGround", meshList[BACKGROUND], Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 2, 0), Vector3(800, 600, 0));

	UIManager->addFeature(BackGround);

	UIManager->addFeature(Title);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Title"), 1.2, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 8, 0), 10, 0);

	UIManager->addFeature(SmokeToLeft);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeft"), 1.5, Vector3(sceneManager2D.m_window_width / 10 * 2, sceneManager2D.m_window_height / 10 * 7, 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeft"), 1.5, Vector3(100, 50, 0), 4, 2);


	UIManager->addFeature(SmokeToLeftUp);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeftUp"), 1.5, Vector3(sceneManager2D.m_window_width / 10 * 1.2, sceneManager2D.m_window_height / 10 * 7.5, 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToLeftUp"), 1.5, Vector3(70, 35, 0), 4, 2);

	UIManager->addFeature(SmokeToRight);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRight"), 1.5, Vector3(sceneManager2D.m_window_width / 10 * 8, sceneManager2D.m_window_height / 10 * 7, 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRight"), 1.5, Vector3(100, 50, 0), 4, 2);

	UIManager->addFeature(SmokeToRightUp);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRightUp"), 1.5, Vector3(sceneManager2D.m_window_width / 10 * 8.8, sceneManager2D.m_window_height / 10 * 7.5, 0), 4, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("SmokeToRightUp"), 1.5, Vector3(70, 35, 0), 4, 2);


	UIManager->addFeature(StartGameButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("StartGameButton"), 0, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 6, 0), 10, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("StartGameButton"),0.5, Vector3(150, 75, 1), 5, 2);

	UIManager->addFeature(LoadGameButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("LoadGameButton"), 0.2, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 4.5, 0), 10, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("LoadGameButton"), 0.7, Vector3(150, 75, 1), 5, 2);

	UIManager->addFeature(Instructions);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Instructions"), 0.4, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 3, 0), 10, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("Instructions"), 0.9, Vector3(150, 75, 1), 5, 2);

	UIManager->addFeature(ExitButton);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ExitButton"), 0.6, Vector3(sceneManager2D.m_window_width / 2, sceneManager2D.m_window_height / 10 * 1.5, 0), 10, 0);
	UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ExitButton"), 1.1, Vector3(150, 75, 1), 5, 2);

	UIManager->addFeature(MusicIcon);
	UIManager->addFeature(MusicIconOff);
}

void CSceneMenu::Update(double dt)
{
	UIManager->Update(dt);

	/*if (UIManager->FindButton("MusicIcon")->getActive())
	{
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIcon"), 0.5, Vector3(100, 100, 0), 5, UIAnimation::SCALING);
	}
	else
	{
		UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MusicIcon"), 0.5, Vector3(0.001, 0.001, 0), 5, UIAnimation::SCALING);
	}*/

	/*
	if (Application::IsKeyPressed('1'))
	glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
	glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	*/

	sceneManager2D.Update(dt);
	/*SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]);
	if (sa)
	{
		sa->Update(dt);
	}*/
}

/********************************************************************************
Render this scene
********************************************************************************/
void CSceneMenu::Render()
{
	sceneManager2D.Render();

	//// Gray Quad
	//sceneManager2D.Render2DMesh(meshList[GRAY_QUAD], false, 1, 1, 1, 0);

	//// Black Quad
	//switch (choice)
	//{
	//case PLAY:
	//	// Play
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 397);
	//	break;
	//case TIME_LIMIT:
	//	// Time-Limit
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 330);
	//	break;
	//case INSTRUCTION:
	//	// Instruction
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 270);
	//	break;
	//case RANKINGS:
	//	// Rankings
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 195);
	//	break;
	//case OPTIONS:
	//	// Options
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 129);
	//	break;
	//case EXIT:
	//	// Exit
	//	sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 70);
	//	break;
	//}

	//// Render the background image
	//sceneManager2D.Render2DMesh(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], false, 1, 1, 1, 0);

	/*for (int i = 0; i < UIManager->getUI_List().size(); ++ i)
	{
		switch (UIManager->getUI_List()[i]->getUI_Type())
		{
		case UIFeature::UT_BUTTON:
		{
			sceneManager2D.Render2DMesh(static_cast<Button*>(UIManager->getUI_List()[i])->getCurrentMesh(), false, UIManager->getUI_List()[i]->getScale().x, UIManager->getUI_List()[i]->getScale().y, UIManager->getUI_List()[i]->getCurrentPos().x, UIManager->getUI_List()[i]->getCurrentPos().y);
			break;
		}
		case UIFeature::UT_IMAGE:
		{
			sceneManager2D.Render2DMesh(static_cast<Image*>(UIManager->getUI_List()[i])->getMesh(), false, UIManager->getUI_List()[i]->getScale().x, UIManager->getUI_List()[i]->getScale().y, UIManager->getUI_List()[i]->getCurrentPos().x, UIManager->getUI_List()[i]->getCurrentPos().y);
			break;
		}
		}
	}*/
	UIManager->Render(sceneManager2D);
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CSceneMenu::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}