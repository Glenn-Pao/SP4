#include "ScenePause.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

CScenePause::CScenePause()
	: currentState(LOBBY)
	, UIManager(NULL)
{
}

CScenePause::CScenePause(const int m_window_width, const int m_window_height)
	: currentState(LOBBY)
	, UIManager(NULL)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

CScenePause::~CScenePause()
{
	if (UIManager)
	{
		delete UIManager;
		UIManager = NULL;
	}
}

void CScenePause::Init(int level)
{
	// Init the base scene
	sceneManager2D.Init(level);

	delete sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND];

	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND]->textureID = LoadTGA("Image//Scenes/Pause.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_PAUSE] = MeshBuilder::GenerateQuad("GEO_PAUSE", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_PAUSE]->textureID = LoadTGA(
		"Image//paused_icon.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_SAVE] = MeshBuilder::GenerateQuad("GEO_SAVE", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_SAVE]->textureID = LoadTGA(
		"Image//UI/Save_Button.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_SAVE_PRESSED] = MeshBuilder::GenerateQuad("GEO_SAVE_PRESSED", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_SAVE_PRESSED]->textureID = LoadTGA("Image//UI/Save_Button_Pressed.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_SAVE_LOCKED] = MeshBuilder::GenerateQuad("GEO_SAVE_LOCKED", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_SAVE_LOCKED]->textureID = LoadTGA("Image//UI/Save_Button_Locked.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_RESUME] = MeshBuilder::GenerateQuad("GEO_RESUME", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_RESUME]->textureID = LoadTGA("Image//UI/Resume_Button.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_RESUME_PRESSED] = MeshBuilder::GenerateQuad("GEO_RESUME_PRESSED", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_RESUME_PRESSED]->textureID = LoadTGA("Image//UI/Resume_Button_Pressed.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_QUIT] = MeshBuilder::GenerateQuad("GEO_QUIT", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_QUIT]->textureID = LoadTGA("Image//UI/Quit_Button.tga");

	sceneManager2D.meshList[CSceneManager2D::GEO_QUIT_PRESSED] = MeshBuilder::GenerateQuad("GEO_QUIT_PRESSED", Color(1, 1, 1), 1.f);
	sceneManager2D.meshList[CSceneManager2D::GEO_QUIT_PRESSED]->textureID = LoadTGA("Image//UI/Quit_Button_Pressed.tga");

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	// Load the ground mesh and texture
	meshList[GRAY_QUAD] = MeshBuilder::Generate2DMesh("GRAY_QUAD", Color(0.5, 0.5, 0.5), 0, 0, 800, 600);
	meshList[BLACK_QUAD] = MeshBuilder::Generate2DMesh("BLACK_QUAD", Color(0, 0, 0), 0, 0, 800, 55);

	choice = NONE;

	//UI
	UIManager = new UISystem();

	//Background
	Image* background;
	background = new Image("background", meshList[CSceneManager2D::GEO_BACKGROUND], Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5, 0), Vector3(0, 0, 0));
	UIManager->addFeature(background);

	Image* paused_icon;
	paused_icon = new Image("paused_icon", meshList[CSceneManager2D::GEO_PAUSE], Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManager->addFeature(paused_icon);

	//Buttons
	// resume button
	Button* ResumeButton;
	ResumeButton = new Button("ResumeButton", meshList[CSceneManager2D::GEO_RESUME], meshList[CSceneManager2D::GEO_RESUME_PRESSED], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManager->addFeature(ResumeButton);

	// save button
	Button* SaveButton;
	SaveButton = new Button("SaveButton", meshList[CSceneManager2D::GEO_SAVE], meshList[CSceneManager2D::GEO_SAVE_PRESSED], meshList[CSceneManager2D::GEO_SAVE_LOCKED] , Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManager->addFeature(SaveButton);

	// Quit Button
	Button* QuitButton;
	QuitButton = new Button("QuitButton", meshList[CSceneManager2D::GEO_QUIT], meshList[CSceneManager2D::GEO_QUIT_PRESSED], NULL, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), Vector3(sceneManager2D.m_window_width * 0.5 * 0.5, sceneManager2D.m_window_height * 0.5 * 0.2, 0));
	UIManager->addFeature(QuitButton);
}

void CScenePause::Update(double dt)
{
	sceneManager2D.Update(dt);

	// UI
	switch (currentState)
	{
	//check if player is inside lobby when pause is triggered, save feature is allowed
	case LOBBY:
	{
		// if no animation
		if (UIManager->InvokeAnimator()->GetNumOfAnimations() == 0)
		{
			// Difficulty
			// if Difficulty Header positon not Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0)
			if (UIManager->FindImage("background")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("background"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height + 100, 0), 20, UIAnimation::TRANSLATION);
			}
			// Tutorial Button
			// if Tutorial Button positon not Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0)
			if (UIManager->FindButton("paused_icon")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("paused_icon"), 0.05, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Easy Button
			// if Easy Button positon not Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0)
			if (UIManager->FindButton("ResumeButton")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("ResumeButton"), 0.1, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Medium Button
			// if Medium Button positon not Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0)
			if (UIManager->FindButton("SaveButton")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("SaveButton"), 0.15, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Hard Button
			// if Easy Button positon not Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0)
			if (UIManager->FindButton("QuitButton")->getCurrentPos() != Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("QuitButton"), 0.2, Vector3(-100, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Selection background
			// if background size not Vector3(0,0,0)
			if (UIManager->FindImage("paused_icon")->getScale() != Vector3(0, 0, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("paused_icon"), 0, Vector3(0, 0, 0), 20, UIAnimation::SCALING);
			}
		}
	}
		break;
	case GAME:
	{
		// if no animation
		if (UIManager->InvokeAnimator()->GetNumOfAnimations() == 0)
		{
			// Difficulty
			// if Difficulty Header positon not Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 2.5, 0)
			if (UIManager->FindImage("Difficulty")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 2.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Difficulty"), 0, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 2.5, 0), 10, UIAnimation::TRANSLATION);
			}
			// Tutorial Button
			// if Tutorial Button positon not Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0)
			if (UIManager->FindButton("TutorialButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("TutorialButton"), 0.05, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 1.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Easy Button
			// if Easy Button positon not Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0)
			if (UIManager->FindButton("EasyButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("EasyButton"), 0.1, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * 0.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Medium Button
			// if Medium Button positon not Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0)
			if (UIManager->FindButton("MediumButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("MediumButton"), 0.15, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -0.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Hard Button
			// if Easy Button positon not Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0)
			if (UIManager->FindButton("HardButton")->getCurrentPos() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindButton("HardButton"), 0.2, Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.5 + sceneManager2D.m_window_height * 0.5 * 0.25 * -1.5, 0), 20, UIAnimation::TRANSLATION);
			}
			// Selection background
			// if background size not Vector3(sceneManager2D.m_window_width * 0.4, sceneManager2D.m_window_height * 0.5, 0)
			if (UIManager->FindImage("Selection_background")->getScale() != Vector3(sceneManager2D.m_window_width * 0.5, sceneManager2D.m_window_height * 0.4, 0))
			{
				UIManager->InvokeAnimator()->StartTransformation(UIManager->FindImage("Selection_background"), 0, Vector3(sceneManager2D.m_window_width * 0.4, sceneManager2D.m_window_height * 0.5, 0), 20, UIAnimation::SCALING);
			}
		}
	}
		break;
	}

	UIManager->Update(Application::mouse_current_x, Application::mouse_current_y, dt);
}

/********************************************************************************
Render this scene
********************************************************************************/
void CScenePause::Render()
{
	sceneManager2D.Render();
	
	// Gray Quad
	sceneManager2D.Render2DMesh(meshList[GRAY_QUAD], false, 1, 1, 1, 0);

	// Black Quad
	switch (choice)
	{
	case RESUME:
		// Resume
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 290);
		break;

	case QUIT:
		// Quit
		sceneManager2D.Render2DMesh(meshList[BLACK_QUAD], false, 1, 1, 1, 225);
		break;
	}

	// Render the background image
	sceneManager2D.Render2DMesh(sceneManager2D.meshList[CSceneManager2D::GEO_BACKGROUND], false, 1, 1, 1, 0);
}

/********************************************************************************
Exit this scene
********************************************************************************/
void CScenePause::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	sceneManager2D.Exit();
}