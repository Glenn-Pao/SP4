#include "SceneManager.h"
#include "GL\glew.h"

#include "..\..\Base\Source\shader.hpp"
#include "..\..\Base\Source\MeshBuilder.h"
#include "..\..\Base\Source\Application.h"
#include "..\..\Base\Source\Utility.h"
#include "..\..\Base\Source\LoadTGA.h"
#include <sstream>
#include "..\..\Base\Source\Strategy_Kill.h"

#include "..\..\UsingLua.h"
CSceneManager::CSceneManager(void)
	: m_window_width(800)
	, m_window_height(600)
	, m_cAvatar(NULL)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
	, lengthOfRayProjectile(10.f)
	, tempName("     ")
	, currentLetter(0)
{
	for (int i = 0; i < 32; ++i)
	{
		letter[i] = false;
	}

	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//Options.lua");

	fpsSelected = L.DoLuaInt("showFPS");
	fullscreenSelected = L.DoLuaInt("fullscreen");
	coloredSelected = L.DoLuaInt("colored");
}

CSceneManager::CSceneManager(const int m_window_width, const int m_window_height)
	: m_cAvatar(NULL)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
	, lengthOfRayProjectile(10.f)
	, tempName("     ")
	, currentLetter(0)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;

	for (int i = 0; i < 32; ++i)
	{
		letter[i] = false;
	}



	// Show FPS
	lua_State *L = lua_open();

	//Read a value from the lua text file
	luaL_openlibs(L);

	if (luaL_loadfile(L, "Lua//Options.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}

	lua_getglobal(L, "showFPS");
	if (!lua_isnumber(L, -1)) {
		printf("`showFPS' should be a number\n");
	}
	fpsSelected = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "fullscreen");
	if (!lua_isnumber(L, -1)) {
		printf("`fullscreen' should be a number\n");
	}
	fullscreenSelected = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "colored");
	if (!lua_isnumber(L, -1)) {
		printf("`colored' should be a number\n");
	}
	coloredSelected = (int)lua_tointeger(L, -1);

	lua_close(L);
}

CSceneManager::~CSceneManager(void)
{
	if (m_cProjectileManager)
	{
		delete m_cProjectileManager;
		m_cProjectileManager = NULL;
	}
	if (m_cSpatialPartition)
	{
		delete m_cSpatialPartition;
		m_cSpatialPartition = NULL;
	}

	for (int i = 0; i < (int)m_cSceneGraphObjects.size(); i++)
	{
		if (m_cSceneGraphObjects[i])
		{
			delete m_cSceneGraphObjects[i];
			m_cSceneGraphObjects[i] = NULL;
		}
	}
	
	if (m_cAvatar)
	{
		delete m_cAvatar;
		m_cAvatar = NULL;
	}
}

void CSceneManager::Init(int level)
{
	PreInit();


	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_RAY] = MeshBuilder::GenerateRay("ray", lengthOfRayProjectile);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("crosshair", Color(1, 1, 1), 1.f);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//BlueCrossHair.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0, 0, 0), 18, 36, 0.5f);
	meshList[GEO_HEART] = MeshBuilder::Generate2DMesh("GEO_HEART", Color(1, 1, 1), 0, 0, 30, 30);
	meshList[GEO_HEART]->textureID = LoadTGA("Image//heart.tga");
	meshList[GEO_BLACK_ALPHA] = MeshBuilder::Generate2DMesh("GEO_BLACK_ALPHA", Color(1, 1, 1), 0, 0, 800, 60);
	meshList[GEO_BLACK_ALPHA]->textureID = LoadTGA("Image//Colors/black.tga");

	// Ground and Walls
	meshList[GEO_WHITE_TILE] = MeshBuilder::GenerateQuad("GEO_WHITE_TILE", Color(1, 1, 1), 1.f);
	meshList[GEO_WHITE_TILE]->textureID = LoadTGA("Image//white_tile.tga");

	// Level completed
	meshList[GEO_LEVEL_COMPLETED] = MeshBuilder::Generate2DMesh("GEO_LEVEL_COMPLETED", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVEL_COMPLETED]->textureID = LoadTGA("Image//Scenes/Level_completed.tga");
	// GameOver
	meshList[GEO_GAMEOVER] = MeshBuilder::Generate2DMesh("GEO_GAMEOVER", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_GAMEOVER]->textureID = LoadTGA("Image//Scenes/Gameover.tga");
	// GameOver timelimit
	meshList[GEO_GAMEOVER_TIMELIMIT] = MeshBuilder::Generate2DMesh("GEO_GAMEOVER_TIMELIMIT", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_GAMEOVER_TIMELIMIT]->textureID = LoadTGA("Image//Scenes/Gameover_timelimit.tga");
	// Get name
	meshList[GEO_GET_NAME] = MeshBuilder::Generate2DMesh("GEO_GET_NAME", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_GET_NAME]->textureID = LoadTGA("Image//Scenes/Getname.tga");

	currentLevel = level;

	if (m_cSpatialPartition == NULL)
		// Initialise Spatial Partition
		InitSpatialPartition();

	if (m_cAvatar == NULL)
		// Initialise Avatar
		InitAvatar();

		// Initialise Objects
		InitObjects();

	if (m_cProjectileManager == NULL)
		// Create a projectile manager
		m_cProjectileManager = new CProjectileManager();


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;

	bLightEnabled = true;

	for (int i = 0; i < maxWeapons; i++)
	{
		cooldownDuration[i] = 0.f;
	}
	currentState = PRE_START;

	noOfMovingEnemies = 0;
	noOfCrowned = 0;

	startingTime = 3.f;


	// For time-limit
	highscore.ReadFromTextFile();
	playerRecord.reset();
	timeLeft = 60.f;
}

void CSceneManager::PreInit()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
/*
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");
*/
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);


	for (int i = 0; i < 1; i++)
	{
		lights.push_back(Light());
		lights[i].type = Light::LIGHT_DIRECTIONAL;
		if (i == 0)
			lights[i].position.Set(0, 450, 0);
		else if (i == 1)
			lights[i].position.Set(-250, 400, 250);
		else if (i == 2)
			lights[i].position.Set(250, 400, -250);
		else
			lights[i].position.Set(250, 400, 250);
		lights[i].position.x += 400;
		lights[i].position.z += 400;
		lights[i].color.Set(1, 1, 1);
		lights[i].power = 1;
		lights[i].kC = 1.f;
		lights[i].kL = 0.01f;
		lights[i].kQ = 0.001f;
		lights[i].cosCutoff = cos(Math::DegreeToRadian(90));
		lights[i].cosInner = cos(Math::DegreeToRadian(30));
		lights[i].exponent = 5.f;
		//lights[i].spotDirection.Set(0.f, -1.f, 0.f);
	}

	glUniform1i(m_parameters[U_NUMLIGHTS], lights.size());
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	for (int i = 0; i < (int)lights.size(); i++)
	{
		glUniform1i(m_parameters[U_LIGHT0_TYPE] + i, lights[i].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR] + i, 1, &lights[i].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER] + i, lights[i].power);
		glUniform1f(m_parameters[U_LIGHT0_KC] + i, lights[i].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL] + i, lights[i].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ] + i, lights[i].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF] + i, lights[i].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER] + i, lights[i].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT] + i, lights[i].exponent);
	}
}

void CSceneManager::InitAvatar()
{
	lua_State *L = lua_open();

	//Read a value from the lua text file
	luaL_openlibs(L);

	if (luaL_loadfile(L, "Lua//avatarInfo.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
		return;
	}

	lua_getglobal(L, "Health");
	if (!lua_isnumber(L, -1)) {
		printf("`Health' should be a number\n");
		return;
	}
	int health = (int)lua_tonumber(L, -1);

	lua_getglobal(L, "posX");
	if (!lua_isnumber(L, -1)) {
		printf("`posX' should be a number\n");
		return;
	}
	int x = (int)lua_tonumber(L, -1);

	lua_getglobal(L, "posZ");
	if (!lua_isnumber(L, -1)) {
		printf("`posZ' should be a number\n");
		return;
	}
	int z = (int)lua_tonumber(L, -1);

	lua_close(L);

	// Initialise and load a model into it
	m_cAvatar = new CPlayInfo3PV();
	CSceneNode* m_cAvatarNode = new CSceneNode();
	CModel* newModel = new CModel();
	Mesh* mesh = MeshBuilder::GenerateOBJ("GEO_ROBOT_BODY", "OBJ//robot_body.obj");
	newModel->Init(mesh, NULL, NULL, NULL, Vector3(4.5f, 10.0f, 4.5f), Vector3(-4.5f, -12.0f, -4.5f));
	cout << m_cAvatarNode->SetNode(new CTransform(0, 0, 0), newModel, health) << endl;

	newModel = new CModel();
	mesh = MeshBuilder::GenerateOBJ("GEO_ROBOT_BOTTOM", "OBJ//robot_bottom.obj");
	newModel->Init(mesh, NULL, NULL, NULL, Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	cout << m_cAvatarNode->AddChild(new CTransform(0, -2, 0), newModel) << endl;

	newModel = new CModel();
	mesh = MeshBuilder::GenerateOBJ("GEO_ROBOT_TOP", "OBJ//robot_top.obj");
	newModel->Init(mesh, NULL, NULL, NULL, Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	cout << m_cAvatarNode->AddChild(new CTransform(0, 2.5, 0), newModel) << endl;

	newModel = new CModel();
	mesh = MeshBuilder::GenerateOBJ("GEO_ROBOT_CANNON", "OBJ//robot_cannon.obj");
	newModel->Init(mesh, NULL, NULL, NULL, Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	cout << m_cAvatarNode->AddChild(new CTransform(0, 0.5, 0), newModel) << endl;

	m_cAvatar->SetNode(m_cAvatarNode);

	m_cAvatar->SetPos_x(x);
	m_cAvatar->SetPos_z(z);

	// Add the pointers to the scene graph to the spatial partition
	m_cSpatialPartition->AddObject(m_cAvatar->theAvatarNode);
}

void CSceneManager::InitObjects()
{
	// Fixed Objects
	int i = 0;
	for (int j = 0; j < m_cSpatialPartition->GetxNumOfGrid(); j++)
	{
		for (int k = 0; k < m_cSpatialPartition->GetyNumOfGrid(); k++)
		{
			// Core
			m_cSceneGraphObjects.push_back(new CSceneNode());
			CModel* newModel = new CModel();
			newModel->Init(NULL, NULL, NULL, NULL, Vector3(4.0f, 12.0f, 4.0f), Vector3(-4.0f, -12.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->SetNode(new CTransform(0, 0, 0), newModel) << endl;

			// Top
			newModel = new CModel();
			Mesh* meshLow = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 6, 12, 4.0f);
			Mesh* meshMed = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 12, 24, 4.0f);
			Mesh* meshHigh = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 18, 36, 4.0f);
			newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, 4, 0), newModel) << endl;

			// Mid
			newModel = new CModel();
			meshLow = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 6, 12, 4.0f);
			meshMed = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 12, 24, 4.0f);
			meshHigh = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 18, 36, 4.0f);
			newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, 0, 0), newModel) << endl;

			// Below
			newModel = new CModel();
			meshLow = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 6, 12, 4.0f);
			meshMed = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 12, 24, 4.0f);
			meshHigh = MeshBuilder::GenerateSphere("sphere_highres", Color(1, 1, 1), 18, 36, 4.0f);
			newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, -4, 0), newModel) << endl;

			m_cSceneGraphObjects[i]->ApplyTranslate((float)(100 + 200 * j), 0, (float)(100 + 200 * k));
			m_cSceneGraphObjects[i]->SetStatus(true);
			// Add the pointers to the scene graph to the spatial partition
			m_cSpatialPartition->AddObject(m_cSceneGraphObjects[i]);
			i++;
		}
	}

	// Moving
	for (int j = 0; j < 25; j++)
	{
		// Core
		m_cSceneGraphObjects.push_back(new CSceneNode());
		CModel* newModel = new CModel();
		newModel->Init(NULL, NULL, NULL, NULL, Vector3(3.5f, 12.0f, 3.5f), Vector3(-3.5f, -12.0f, -3.5f));
		cout << m_cSceneGraphObjects[i]->SetNode(new CTransform(0, 0, 0), newModel) << endl;

		// Top
		newModel = new CModel();
		Mesh* meshLow = MeshBuilder::GenerateCube("spikeblock_low", Color(1, 1, 1), 4.5f);
		Mesh* meshMed = MeshBuilder::GenerateOBJ("spikeblock_med", "OBJ//spike_block.obj");
		Mesh* meshHigh = MeshBuilder::GenerateOBJ("spikeblock_high", "OBJ//spike_block_high.obj");
		newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
		cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, 4, 0), newModel) << endl;

		// Mid
		newModel = new CModel();
		meshLow = MeshBuilder::GenerateCube("spikeblock_low", Color(1, 1, 1), 4.5f);
		meshMed = MeshBuilder::GenerateOBJ("spikeblock_med", "OBJ//spike_block.obj");
		meshHigh = MeshBuilder::GenerateOBJ("spikeblock_high", "OBJ//spike_block_high.obj");
		newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
		cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, 0, 0), newModel) << endl;

		// Below
		newModel = new CModel();
		meshLow = MeshBuilder::GenerateCube("spikeblock_low", Color(1, 1, 1), 4.5f);
		meshMed = MeshBuilder::GenerateOBJ("spikeblock_med", "OBJ//spike_block.obj");
		meshHigh = MeshBuilder::GenerateOBJ("spikeblock_high", "OBJ//spike_block_high.obj");
		newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
		cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, -4, 0), newModel) << endl;

		m_cSceneGraphObjects[i]->SetRandomVel();

		// Add the pointers to the scene graph to the spatial partition
		m_cSpatialPartition->AddObject(m_cSceneGraphObjects[i]);
		i++;
	}

	// 0 == time-limt no crown
	if (currentLevel != 0)
	{
		// Crown
		for (int j = 0; j < 5; j++)
		{
			// Core
			m_cSceneGraphObjects.push_back(new CSceneNode());
			CModel* newModel = new CModel();
			newModel->Init(NULL, NULL, NULL, NULL, Vector3(3.5f, 12.0f, 3.5f), Vector3(-3.5f, -12.0f, -3.5f));
			cout << m_cSceneGraphObjects[i]->SetNode(new CTransform(0, 0, 0), newModel) << endl;

			// Top
			newModel = new CModel();
			Mesh* meshLow = MeshBuilder::GenerateCube("spikeblock_low", Color(1, 1, 1), 4.5f);
			Mesh* meshMed = MeshBuilder::GenerateOBJ("spikeblock_med", "OBJ//spike_block.obj");
			Mesh* meshHigh = MeshBuilder::GenerateOBJ("spikeblock_high", "OBJ//spike_block_high.obj");
			newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, 4, 0), newModel) << endl;

			// Mid
			newModel = new CModel();
			meshLow = MeshBuilder::GenerateCube("spikeblock_low", Color(1, 1, 1), 4.5f);
			meshMed = MeshBuilder::GenerateOBJ("spikeblock_med", "OBJ//spike_block.obj");
			meshHigh = MeshBuilder::GenerateOBJ("spikeblock_high", "OBJ//spike_block_high.obj");
			newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, 0, 0), newModel) << endl;

			// Below
			newModel = new CModel();
			meshLow = MeshBuilder::GenerateCube("spikeblock_low", Color(1, 1, 1), 4.5f);
			meshMed = MeshBuilder::GenerateOBJ("spikeblock_med", "OBJ//spike_block.obj");
			meshHigh = MeshBuilder::GenerateOBJ("spikeblock_high", "OBJ//spike_block_high.obj");
			newModel->Init(NULL, meshLow, meshMed, meshHigh, Vector3(4.0f, 4.0f, 4.0f), Vector3(-4.0f, -4.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, -4, 0), newModel) << endl;

			// Crown
			newModel = new CModel();
			Mesh* mesh = MeshBuilder::GenerateOBJ("crown", "OBJ//crown.obj");
			newModel->Init(mesh, NULL, NULL, NULL, Vector3(4.0f, 1.0f, 4.0f), Vector3(-4.0f, -1.0f, -4.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, 10, 0), newModel) << endl;

			// Ground ring
			newModel = new CModel();
			mesh = MeshBuilder::GenerateRing("ring", Color(1, 1, 1), 36, 5, 4);
			newModel->Init(mesh, NULL, NULL, NULL, Vector3(5.0f, 1.0f, 5.0f), Vector3(-5.0f, -1.0f, -5.0f));
			cout << m_cSceneGraphObjects[i]->AddChild(new CTransform(0, -9, 0), newModel) << endl;

			m_cSceneGraphObjects[i]->SetCrowned(true);
			m_cSceneGraphObjects[i]->SetRandomVel();

			// Add the pointers to the scene graph to the spatial partition
			m_cSpatialPartition->AddObject(m_cSceneGraphObjects[i]);
			i++;


			crownedListTranslateY.push_back(1.0f);
			crownedListUpward.push_back(true);
		}
	}
}

void CSceneManager::InitSpatialPartition()
{
	lua_State *L = lua_open();

	//Read a value from the lua text file
	luaL_openlibs(L);

	if (luaL_loadfile(L, "Lua//spatialPartitionInfo.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
		return;
	}

	lua_getglobal(L, "gridSizeX");
	if (!lua_isnumber(L, -1)) {
		printf("`gridSizeX' should be a number\n");
		return;
	}
	int gridSizeX = (int)lua_tonumber(L, -1);

	lua_getglobal(L, "gridSizeY");
	if (!lua_isnumber(L, -1)) {
		printf("`gridSizeY' should be a number\n");
		return;
	}
	int gridSizeY = (int)lua_tonumber(L, -1);

	lua_getglobal(L, "numOfGridX");
	if (!lua_isnumber(L, -1)) {
		printf("`numOfGridX' should be a number\n");
		return;
	}
	int numOfGridX = (int)lua_tonumber(L, -1);

	lua_getglobal(L, "numOfGridY");
	if (!lua_isnumber(L, -1)) {
		printf("`numOfGridY' should be a number\n");
		return;
	}
	int numOfGridY = (int)lua_tonumber(L, -1);

	lua_close(L);

	// Create a spatial partition
	int index = 0;
	m_cSpatialPartition = new CSpatialPartition();
	m_cSpatialPartition->Init(gridSizeX, gridSizeY, numOfGridX, numOfGridY);
	for (int i = 0; i<m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j<m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			Mesh* mesh = MeshBuilder::GenerateQuad("GridMesh", Color(1, 1, 1), (float)gridSizeX);

			if (coloredSelected)
			{
				string temp = "Image//Colors/color";
				temp += to_string(index);
				temp += ".tga";
				mesh->textureID = LoadTGA(temp.c_str());
				index++;
				if (index > 13)
					index = 0;
			}
			else
			{
				mesh->textureID = LoadTGA("Image//Colors/white.tge");
			}

			m_cSpatialPartition->SetGridMesh(i, j, mesh);
		}
	}

	m_cSpatialPartition->PrintSelf();
}


void CSceneManager::AddEnemies()
{
	int maxMovingEnemies = currentLevel * 5;
	int maxCrownedEnemies = currentLevel;
	// Time-Limit
	if (currentLevel == 0)
	{
		maxMovingEnemies = 5 * 5;
	}

	if (noOfMovingEnemies != maxMovingEnemies || noOfCrowned != maxCrownedEnemies)
	{
		for (int i = 0; i < (int)m_cSceneGraphObjects.size(); i++)
		{
			// Is Moving
			if (m_cSceneGraphObjects[i]->GetVel().IsZero() == false)
			{
				// Not active
				if (m_cSceneGraphObjects[i]->GetStatus() == false)
				{
					// Moving
					if (m_cSceneGraphObjects[i]->GetCrowned() == false)
					{
						if (noOfMovingEnemies < maxMovingEnemies)
						{
							m_cSceneGraphObjects[i]->SetStatus(true);
							m_cSceneGraphObjects[i]->SetRandomPos(m_cSceneGraphObjects, (float)m_cSpatialPartition->xSize, (float)m_cSpatialPartition->ySize);

							m_cSpatialPartition->CheckObjectGrid(m_cSceneGraphObjects[i]);
							noOfMovingEnemies++;
						}
					}
					// Crowned
					else
					{
						if (noOfCrowned < maxCrownedEnemies)
						{
							m_cSceneGraphObjects[i]->SetStatus(true);
							m_cSceneGraphObjects[i]->SetRandomPos(m_cSceneGraphObjects, (float)m_cSpatialPartition->xSize, (float)m_cSpatialPartition->ySize);

							m_cSpatialPartition->CheckObjectGrid(m_cSceneGraphObjects[i]);
							noOfCrowned++;
						}
					}
				}
				else
				{
					m_cSceneGraphObjects[i]->SetTimeDead(0.f);
					m_cSceneGraphObjects[i]->SetRandomPos(m_cSceneGraphObjects, (float)m_cSpatialPartition->xSize, (float)m_cSpatialPartition->ySize);

					m_cSpatialPartition->CheckObjectGrid(m_cSceneGraphObjects[i]);
				}
			}
		}
	}
}

void CSceneManager::Update(double dt)
{
	
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
/*
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if(Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if(Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if(Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if(Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if(Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);
	*/
	switch (currentState)
	{
		case PRE_START:
		{
			m_cAvatar->Update(dt, false, m_cSpatialPartition, false);

			camera.UpdatePosition(m_cAvatar->GetNewPosition(), m_cAvatar->GetNewDirection());

			AddEnemies();
		}
		break;
		case STARTING:
		{
			m_cAvatar->Update(dt, false, m_cSpatialPartition, false);

			camera.UpdatePosition(m_cAvatar->GetNewPosition(), m_cAvatar->GetNewDirection());

			startingTime -= (float)dt;
			if (startingTime < 0.f)
			{
				startingTime = 3.f;
				currentState = PLAYING;
			}
		}
		break;
		case GAMEOVER_TIME_LIMIT:
		{
			if (highscore.HighscoreCheck(playerRecord))
			{
				if (playerRecord.getName().size() == 0)
				{
					currentState = GET_PLAYERNAME;
				}
				else
				{
					//store record
					highscore.storeNewRecord(playerRecord);
					currentState = GAMEOVER_TIME_LIMIT_END;
				}
			}
			else
			{
				currentState =  GAMEOVER_TIME_LIMIT_END;
			}
		}
		break;
		case GET_PLAYERNAME:
		{
			// Letter
			for (int i = 0; i < 32; ++i)
			{
				if (Application::IsKeyPressed('A' + i) && !letter[i])
				{
					letter[i] = true;

					if (currentLetter < 5)
					{
						tempName[currentLetter] = 'A' + i;
						currentLetter++;
					}
				}
				else if (!Application::IsKeyPressed('A' + i) && letter[i])
				{
					letter[i] = false;
				}
			}

			// Backspace
			static bool backsp = false;

			if (Application::IsKeyPressed(VK_BACK) && !backsp)
			{
				backsp = true;
				if (currentLetter > 0)
				{
					tempName[currentLetter - 1] = NULL;
					currentLetter--;
				}
			}
			else if (!Application::IsKeyPressed(VK_BACK) && backsp)
			{
				backsp = false;
			}

			// Enter
			static bool IsKeyPressedReturn = false;

			if (Application::IsKeyPressed(VK_RETURN) && !IsKeyPressedReturn)
			{
				if (currentLetter > 0)
				{
					string tempd = "      ";
					for (int i = 1; i < 6; ++i)
					{
						if (tempName[i - 1] != NULL)
						{
							tempd[i] = tempName[i - 1];
						}
						else
							break;
					}
					playerRecord.setName(tempd);
					//store record
					highscore.storeNewRecord(playerRecord);

					currentState = GAMEOVER_TIME_LIMIT_END;
				}
				IsKeyPressedReturn = true;
			}
			else if (!Application::IsKeyPressed(VK_RETURN) && IsKeyPressedReturn)
				IsKeyPressedReturn = false;
		}
		break;
		case PLAYING:
		{
			UpdateObjects(dt);

			bool cooldowning = false;
			for (int i = 0; i < maxWeapons; i++)
			{
				if (cooldownDuration[i] != 0.f)
				{
					cooldowning = true;
				}
			}
			m_cAvatar->Update(dt, cooldowning, m_cSpatialPartition, true);

			camera.UpdatePosition(m_cAvatar->GetNewPosition(), m_cAvatar->GetNewDirection());

			//camera.Update(dt);

			// Update the spatial partition
			m_cSpatialPartition->Update(camera.position, (camera.target - camera.position).Normalize());

			// Update the Projectile Manager
			m_cProjectileManager->Update(dt);

			// Check for collision for Projectiles
			Vector3 ProjectilePosition;
			Vector3 ProjectilePosition_End;
			for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
			{
				if (m_cProjectileManager->IsActive(i))
				{
					ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();

					if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
					{
						// Destroy the projectile after collision
						int result = m_cSpatialPartition->CheckForCollision(ProjectilePosition, m_cProjectileManager->theListOfProjectiles[i]->GetHitPoints());
						
						if (result >= 1)
						{
							m_cProjectileManager->RemoveProjectile(i);
							if (result == 2)
							{
								noOfCrowned--;
							}
							else if (result == 3)
							{
								playerRecord.setScore(playerRecord.getScore() + 1);
							}
							else if (result == 4)
							{
								playerRecord.setScore(playerRecord.getScore() + 2);
							}
						}
					}
					else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
					{
						ProjectilePosition_End = ProjectilePosition + m_cProjectileManager->theListOfProjectiles[i]->GetDirection() * m_cProjectileManager->theListOfProjectiles[i]->GetLength();

						// Destroy the projectile after collision
						int result = m_cSpatialPartition->CheckForCollision(ProjectilePosition, ProjectilePosition_End, m_cProjectileManager->theListOfProjectiles[i]->GetHitPoints());

						if (result >= 1)
						{
							m_cProjectileManager->RemoveProjectile(i);
							if (result == 2)
							{
								noOfCrowned--;
							}
							else if (result == 3)
							{
								playerRecord.setScore(playerRecord.getScore() + 1);
							}
							else if (result == 4)
							{
								playerRecord.setScore(playerRecord.getScore() + 2);
							}
						}
					}
				}
			}

			// Time-Limit
			if (currentLevel == 0)
			{
				timeLeft -= (float)dt;

				// Gameover time limit
				if (timeLeft <= 0.f || m_cAvatar->theAvatarNode->GetTimeDead() != 0.f)
				{
					currentState = GAMEOVER_TIME_LIMIT;
				}
			}
			else
			{
				// Go level completed
				if (noOfCrowned == 0)
				{
					currentState = LEVEL_COMPLETED;
				}

				// Gameover
				if (m_cAvatar->theAvatarNode->GetTimeDead() != 0.f)
				{
					currentState = GAMEOVER;
				}
			}

			// Bullet cooling down
			for (int i = 0; i < maxWeapons; i++)
			{
				if (cooldownDuration[i] != 0.f)
				{
					cooldownDuration[i] -= (float)dt;
					if (cooldownDuration[i] < 0.f)
					{
						cooldownDuration[i] = 0.f;
					}
				}
			}
		}
		break;
	}

	fps = (float)(1.f / dt);
}

/********************************************************************************
Update Objects
********************************************************************************/
void CSceneManager::UpdateObjects(double dt)
{
	rotateAngle = 200 * (float)dt;
	
	int crownedIndex = 0;
	// Update
	for (int i = 0; i < (int)m_cSceneGraphObjects.size(); i++)
	{
		CSceneNode* object = m_cSceneGraphObjects[i];
		if (m_cSceneGraphObjects[i]->GetStatus() == true)
		{
			if (m_cSceneGraphObjects[i]->GetTimeDead() == 0.f)
			{
				// Rotate top, middle, bottom
				object->GetNode(11)->ApplyRotate(rotateAngle, 0, 1, 0, true);
				object->GetNode(12)->ApplyRotate(-rotateAngle, 0, 1, 0, true);
				object->GetNode(13)->ApplyRotate(rotateAngle, 0, 1, 0, true);

				// For Crowned
				if (object->GetCrowned() == true)
				{
					if (crownedListUpward[crownedIndex] == true)
					{
						object->GetNode(14)->ApplyTranslate(0, (float)(5 * dt), 0);
						crownedListTranslateY[crownedIndex] += (float)(5 * dt);

						if (crownedListTranslateY[crownedIndex] > 3.f)
							crownedListUpward[crownedIndex] = false;
					}
					else
					{
						object->GetNode(14)->ApplyTranslate(0, (float)(-5 * dt), 0);
						crownedListTranslateY[crownedIndex] += (float)(-5 * dt);

						if (crownedListTranslateY[crownedIndex] < 1.f)
							crownedListUpward[crownedIndex] = true;
					}
					object->GetNode(15)->SetScale(crownedListTranslateY[crownedIndex], 1, crownedListTranslateY[crownedIndex]);

					crownedIndex++;
				}

				// Flickering
				if (object->GetFlickeringTimeLeft() > 0)
				{
					object->UpdateFlickering(dt);
				}
				// Moving
				else if (object->GetVel().IsZero() == false)
				{
					object->UpdateFSM((float)dt, m_cAvatar->theAvatarNode);

					m_cSpatialPartition->CheckObjectGrid(object);

					Vector3 ObjectCenter = object->GetTopLeft() - (object->GetTopLeft() - object->GetBottomRight()) * 0.5;

					CSceneNode* object2 = m_cSpatialPartition->CheckForCollision(object, 0);// All object
					if (object2)
					{
						Vector3 newDirection = object2->GetOffset() - object->GetOffset();
						Vector3 object2NodeRadius = object2->GetOffset() - object2->GetBottomRight();
						Vector3 objectNodeRadius = object->GetOffset() - object->GetBottomRight();
						object2NodeRadius.y = objectNodeRadius.y = 0;
						float separationDistance = object2NodeRadius.Length() + objectNodeRadius.Length();
						newDirection = newDirection.Normalize() * separationDistance * (float)dt;

						object->ApplyTranslate(-newDirection.x, -newDirection.y, -newDirection.z);

						object->SetRandomDir();
					}
					else if (object->CheckForCollision(Vector3(ObjectCenter.x, ObjectCenter.y, 1000), 0) || // Front
						object->CheckForCollision(Vector3(ObjectCenter.x, ObjectCenter.y, 0), 0) ||// Back
						object->CheckForCollision(Vector3(1000, ObjectCenter.y, ObjectCenter.z), 0) ||// Left
						object->CheckForCollision(Vector3(0, ObjectCenter.y, ObjectCenter.z), 0))// Right
					{
						object->ApplyTranslate(-object->GetVel().x * (float)dt, -object->GetVel().y * (float)dt, -object->GetVel().z * (float)dt);
						object->SetRandomDir();
					}
					else if (object->GetWalkDistanceLeft() < 0.f)
					{
						object->SetRandomDir();
					}
				}
			}
			else
			{
				object->UpdateDead(dt);
				if (object->CheckForCollision(m_cAvatar->theAvatarNode, 1) == true)
				{
					object->SetTimeDead(1.f);
				}
			}
		}
	}
}

/********************************************************************************
 Update Camera position
 ********************************************************************************/
void CSceneManager::UpdateCameraStatus(const unsigned char key, const bool status)
{
	camera.UpdateStatus(key, status);
}

/********************************************************************************
Update Avatar position
********************************************************************************/
void CSceneManager::UpdateAvatarStatus(const unsigned char key, const bool status)
{
	m_cAvatar->UpdateMovement(key, status);
}

/********************************************************************************
 Update Weapon status
 ********************************************************************************/
void CSceneManager::UpdateWeaponStatus(const unsigned char key)
{

	if (key == WA_FIRE)
	{
		if (cooldownDuration[0] == 0.f)
		{
			// Add a bullet object which starts at the camera position and moves in the camera's direction
			m_cProjectileManager->AddProjectile(m_cAvatar->GetPosition() + m_cAvatar->GetCharDirection() * 3.5f + Vector3(0, m_cAvatar->GetFloatingAmount() + 0.5f), m_cAvatar->GetCharDirection(), 100.f, 3);
			cooldownDuration[0] = 1.3f;
		}
	}
	else if (key == WA_FIRE_SECONDARY)
	{
		if (cooldownDuration[1] == 0.f)
		{
			// Add a bullet object which starts at the camera position and moves in the camera's direction
			m_cProjectileManager->AddRayProjectile(m_cAvatar->GetPosition() + m_cAvatar->GetCharDirection() * 3.5f + Vector3(0, m_cAvatar->GetFloatingAmount() + 2.5f), m_cAvatar->GetCharDirection(), 200.f, lengthOfRayProjectile, 1);
			cooldownDuration[1] = 0.5f;
		}
	}
}

/********************************************************************************
 Render text onto the screen with reference position in the middle of the image
 ********************************************************************************/
void CSceneManager::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.8f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void CSceneManager::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
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
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation( (i*1.0f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
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
 Render a mesh in 2D
 ********************************************************************************/
void CSceneManager::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if (enableLight && bLightEnabled)
				{
					glUniform1i(m_parameters[U_LIGHTENABLED], 1);
					modelView = viewStack.Top() * modelStack.Top();
					glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
					modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
					glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

					//load material
					glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
					glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
					glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
					glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
				}
				else
				{
					glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				}
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

}

/********************************************************************************
Render 2D Mesh
********************************************************************************/
void CSceneManager::Render2DMesh(Mesh *mesh, bool enableLight, int sizeX, int sizeY, int x, int y, float rotate, bool flip)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate((float)x, (float)y, 0);
	modelStack.Scale((float)sizeX, (float)sizeY, 0);

	modelStack.Translate(0.5f, 0.5f, 0);
	modelStack.Rotate(rotate, 0, 0, 1);
	modelStack.Translate(-0.5f, -0.5f, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
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
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
 Render a mesh
 ********************************************************************************/
void CSceneManager::RenderMesh(Mesh *mesh, bool enableLight)
{
	if (mesh == NULL)
		return;

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
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
}

/********************************************************************************
 Render mobile objects
 ********************************************************************************/
void CSceneManager::RenderGUI()
{
	// Render the crosshair
	if (m_cAvatar->IsMoving() == true)
		RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 20.0f);

	// Black alpha
	//Top
	Render2DMesh(meshList[GEO_BLACK_ALPHA], false, 1, 1, 0, m_window_height - 60);
	//Bottom
	Render2DMesh(meshList[GEO_BLACK_ALPHA], false, 1, 1, 0, 0);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	if (fpsSelected)
	{
		ss << "FPS:" << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, (float)(m_window_width - 250), 5);
	}

	// Time Limit
	if (currentLevel == 0)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Time-Limit", Color(1, 1, 1), 30, 0, (float)m_window_height - 30);
	}
	else
	{
		ss.str(std::string());
		ss << "Level:" << currentLevel;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 30, 0, (float)m_window_height - 30);
	}

	switch (currentState)
	{
	case PRE_START:
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "<CLick to Play>", Color(0, 0, 0), 40, 100, m_window_height * 0.5f);
	}
	break;
	case STARTING:
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Starting In:", Color(0, 0, 0), 50, m_window_width * 0.5f - 300, m_window_height * 0.75f);

		ss.str(std::string());
		ss << (int)startingTime;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 200, m_window_width * 0.5f - 50, m_window_height * 0.25f);
	}
	break;
	case PLAYING:
	{
		// Time Limit
		if (currentLevel == 0)
		{
			ss.str(std::string());
			ss << "Score:" << playerRecord.getScore();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 30, (float)m_window_width - 250, (float)m_window_height - 30);

			ss.str(std::string());
			ss << "Time Left:" << timeLeft;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 30, 175, (float)m_window_height - 60);
		}
		else
		{
			ss.str(std::string());
			ss << "Crowned:" << noOfCrowned;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 30, (float)m_window_width - 260, (float)m_window_height - 30);

			RenderTextOnScreen(meshList[GEO_TEXT], "Find & Destroy all Crowned!", Color(1, 1, 1), 30, 0, (float)m_window_height - 60);
		}

		Render2DMesh(meshList[GEO_HEART], false, 1, 1, m_window_width - 430, m_window_height - 30);
		ss.str(std::string());
		ss.precision(2);
		ss << ":" << m_cAvatar->theAvatarNode->GetHealth();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, (float)m_window_width - 390, (float)m_window_height - 30);

		ss.str(std::string());
		ss.precision(2);
		ss << "Cannon CD:" << cooldownDuration[0];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 30, 0, 30);

		ss.str(std::string());
		ss.precision(2);
		ss << "Laser CD:" << cooldownDuration[1];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 30, 0, 0);
	}
	break;
	}
}

/********************************************************************************
 Render mobile objects
 ********************************************************************************/
void CSceneManager::RenderMobileObjects()
{
	// Render the projectiles
	Vector3 ProjectilePosition;
	for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();
			modelStack.PushMatrix();
				modelStack.Translate(ProjectilePosition.x, ProjectilePosition.y, ProjectilePosition.z);
				if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
				{
					RenderMesh(meshList[GEO_SPHERE], false);
				}
				else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
				{
					float angle = Math::RadianToDegree(atan2(m_cProjectileManager->theListOfProjectiles[i]->GetDirection().x, m_cProjectileManager->theListOfProjectiles[i]->GetDirection().z));
					modelStack.Rotate(angle, 0, 1, 0);
					glLineWidth(10.0f);
					RenderMesh(meshList[GEO_RAY], false);
					glLineWidth(1.0f);
				}
			modelStack.PopMatrix();
		}
	}

	// Render the Avatar
	modelStack.PushMatrix();
	modelStack.Translate(0, m_cAvatar->GetFloatingAmount(), 0);
	m_cAvatar->theAvatarNode->Draw(this, true);
	modelStack.PopMatrix();

	// Draw the Scene Graph
	for (int i = 0; i < (int)m_cSceneGraphObjects.size(); i++)
	{
		if (m_cSceneGraphObjects[i]->GetStatus() == true)
		{
			if (m_cSceneGraphObjects[i]->GetTimeDead() == 0.f)
				m_cSceneGraphObjects[i]->Draw(this, true);
		}
	}
}

/********************************************************************************
 Render the lights in this scene
 ********************************************************************************/
void CSceneManager::RenderFixedObjects()
{
	//RenderMesh(meshList[GEO_AXES], false);
}

/********************************************************************************
 Render the lights in this scene
 ********************************************************************************/
void CSceneManager::RenderLights()
{ 
	for (int i = 0; i < (int)lights.size(); i++)
	{
		// Render LightBall
		modelStack.PushMatrix();
		modelStack.Translate(lights[i].position.x, lights[i].position.y, lights[i].position.z);
		//RenderMesh(meshList[GEO_LIGHTBALL], false);
		modelStack.PopMatrix();

		if (lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION] + i, 1, &lightDirection_cameraspace.x);
		}
		else if (lights[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION] + i, 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[i].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION] + i, 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION] + i, 1, &lightPosition_cameraspace.x);
		}
	}
}

/********************************************************************************
 Render the ground in this scene
 ********************************************************************************/
void CSceneManager::RenderGround()
{
	// Ground
	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 50);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			RenderMesh(meshList[GEO_WHITE_TILE], true);
			modelStack.PopMatrix();
		}
	}
	
	modelStack.PopMatrix();

	// Render the Spatial Partitions
	if (coloredSelected)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-400, 0, -400);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Translate(0, 0, -9.9f);

		for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); i++)
		{
			for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
			{
				modelStack.PushMatrix();
				modelStack.Translate((float)(m_cSpatialPartition->xGridSize*i), (float)(-m_cSpatialPartition->yGridSize*j), 0.0f);
				RenderMesh(m_cSpatialPartition->GetGridMesh(i, j), true);
				modelStack.PopMatrix();
			}
		}
		modelStack.PopMatrix();
	}
}

/********************************************************************************
 Render the skybox in this scene
 ********************************************************************************/
void CSceneManager::RenderSkybox()
{
	// Front
	modelStack.PushMatrix();
	modelStack.Translate(50, 540.f, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);
	for (int x = 0; x < 6; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			RenderMesh(meshList[GEO_WHITE_TILE], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	// Back
	modelStack.PushMatrix();
	modelStack.Translate(50, 40.f, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);
	for (int x = 0; x < 6; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			RenderMesh(meshList[GEO_WHITE_TILE], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	// Left
	modelStack.PushMatrix();
	modelStack.Translate(0, 40.f, 50);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);
	for (int x = 0; x < 6; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			RenderMesh(meshList[GEO_WHITE_TILE], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	// Right
	modelStack.PushMatrix();
	modelStack.Translate(0, 40.f, -50);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);
	for (int x = 0; x < 6; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			RenderMesh(meshList[GEO_WHITE_TILE], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	// Top
	modelStack.PushMatrix();
	modelStack.Translate(50, 90, -50);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);
	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			RenderMesh(meshList[GEO_WHITE_TILE], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

/********************************************************************************
 Render this scene
 ********************************************************************************/
void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(	camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z );

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	switch (currentState)
	{
	case GAMEOVER:
		Render2DMesh(meshList[GEO_GAMEOVER], false, 1, 1, 1, 0);
		break;
	case GAMEOVER_TIME_LIMIT_END:
	{
		Render2DMesh(meshList[GEO_GAMEOVER_TIMELIMIT], false, 1, 1, 1, 0);

		std::ostringstream ss;
		ss.precision(3);
		ss << playerRecord.getScore();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 50.f, m_window_width * 0.4f, m_window_height * 0.4f);
	}
	break;
	case LEVEL_COMPLETED:
		Render2DMesh(meshList[GEO_LEVEL_COMPLETED], false, 1, 1, 1, 0);
		break;
	case GET_PLAYERNAME:
	{
		Render2DMesh(meshList[GEO_GET_NAME], false, 1, 1, 1, 0);

		std::ostringstream ss;
		ss.precision(3);
		ss << tempName;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 50.f, m_window_width * 0.35f, m_window_height * 0.4f);
	}
	break;
	default:
	{
		RenderLights();
		modelStack.PushMatrix();
		modelStack.Translate(500, 0, 500);
		RenderGround();
		RenderSkybox();
		modelStack.PopMatrix();
		RenderFixedObjects();
		RenderMobileObjects();

		RenderGUI();
	}
	break;
	}

}

/********************************************************************************
 Exit process for this scene
 ********************************************************************************/
void CSceneManager::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);


	std::ofstream myfile("Lua//Options.lua");
	int i = 0;
	if (myfile.is_open())
	{
		myfile << "showFPS = " << fpsSelected << endl;
		myfile << "fullscreen = " << fullscreenSelected << endl;
		myfile << "colored = " << coloredSelected << endl;
	}
}


/********************************************************************************
Start the rendering process by calling PreRendering()
********************************************************************************/
void CSceneManager::PreRendering(bool enableLight, Mesh* mesh)
{
	if (mesh == NULL)
		return;

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
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
	//if (mesh == NULL)
	//	return;

	//Mtx44 MVP, modelView, modelView_inverse_transpose;

	//MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	//if (enableLight && bLightEnabled)
	//{
	//	glUniform1i(m_parameters[U_LIGHTENABLED], 1);
	//	modelView = viewStack.Top() * modelStack.Top();
	//	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	//	modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
	//	glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

	//	//load material
	//	glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
	//	glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
	//	glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
	//	glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	//}
	//else
	//{
	//	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	//}
	//if (mesh->textureID > 0)
	//{
	//	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	//	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	//}
	//else
	//{
	//	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	//}
}


/********************************************************************************
Complete the rendering process by calling the PostRendering()
********************************************************************************/
void CSceneManager::PostRendering(Mesh* mesh)
{
	if (mesh == NULL)
		return;
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}