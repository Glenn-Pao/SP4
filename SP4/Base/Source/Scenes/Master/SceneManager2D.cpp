#include "SceneManager2D.h"
#include "GL\glew.h"

#include "..\..\shader.hpp"
#include "..\..\MeshBuilder.h"
#include "..\..\Application.h"
#include "..\..\Utility.h"
#include "..\..\LoadTGA.h"
#include <sstream>
#include "..\..\Strategy_Kill.h"

CSceneManager2D::CSceneManager2D()
	: m_window_width(800)
	, m_window_height(600)
{
}

CSceneManager2D::~CSceneManager2D()
{
}

void CSceneManager2D::Init(int level)
{
	PreInit();

	// Initialise the camera
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Create the meshes
	for(int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	//meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Algerian.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Load the ground mesh and texture
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//sky_background.tga");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
}

void CSceneManager2D::PreInit()
{

	// Blue background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Switch on culling
	glEnable(GL_CULL_FACE);

	// Render mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Activate blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate and bind vertex array
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}

void CSceneManager2D::Update(double dt)
{
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
	
	rotateAngle -= (float)Application::camera_yaw;// += (float)(10 * dt);

	camera.Update(dt);

	fps = (float)(1.f / dt);
}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void CSceneManager2D::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				//	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation((i*0.6f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
 Render 2D Mesh
 ********************************************************************************/
void CSceneManager2D::Render2DMesh(Mesh *mesh, bool enableLight, int sizeX, int sizeY, int x, int y, float rotate, bool flip)
{
	if (!mesh)
		return;

	if (flip)
	{
		glDisable(GL_CULL_FACE);
	}
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate( (float)x, (float)y, 0);
				modelStack.Scale((float)sizeX, (float)sizeY, 0);

				modelStack.Translate(0.5f, 0.5f, 0);
				modelStack.Rotate(rotate, 0, 0, 1);
				modelStack.Translate(-0.5f, -0.5f, 0);

				if (flip)
				{
					modelStack.Rotate(180, 0, 1, 0);
					modelStack.Translate(-1, 0, 0);
				}
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);

	if (flip)
	{
		glEnable(GL_CULL_FACE);
	}
}

/********************************************************************************
 Render the background
 ********************************************************************************/
void CSceneManager2D::RenderBackground(bool spriteAnimation)
{
	// Render the background
	if (spriteAnimation)
		Render2DMesh(meshList[GEO_BACKGROUND], false, 800, 600, 400, 300);
	else
		Render2DMesh(meshList[GEO_BACKGROUND], false, 1);
}

/********************************************************************************
 Render this scene
 ********************************************************************************/
void CSceneManager2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

/********************************************************************************
 Exit this scene
 ********************************************************************************/
void CSceneManager2D::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; i++)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}