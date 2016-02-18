#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"
#include "Mtx44.h"
#include "..\..\Camera3.h"
#include "..\..\Mesh.h"
#include "MatrixStack.h"
#include "..\..\Light.h"
#include "..\..\PlayInfo3PV.h"
#include "..\..\SceneGraph/SceneNode.h"	// Scene Graph
#include "..\..\SpatialPartition/SpatialPartition.h"
#include "..\..\Projectile\ProjectileManager.h"
#include "..\..\Highscore.h"

const float SKYBOXSIZE = 1000.f;

class CSceneManager : public Scene
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		/*U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_LIGHT2_TYPE,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,
		U_LIGHT3_TYPE,
		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,*/
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_RAY,
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_HEART,
		GEO_BLACK_ALPHA,
		GEO_SPHERE,
		GEO_WHITE_TILE,
		GEO_OBJECT,
		GEO_TEXT,
		GEO_LEVEL_COMPLETED,
		GEO_GAMEOVER,
		GEO_GAMEOVER_TIMELIMIT,
		GEO_GET_NAME,

		NUM_GEOMETRY,
	};
	CSceneManager(void);
	CSceneManager(const int m_window_width, const int m_window_height);
	~CSceneManager(void);

	virtual void Init(int level);
	void PreInit();
	void InitAvatar();
	void InitObjects();
	void InitSpatialPartition();
	virtual void Update(double dt);

	// Update Objects
	void UpdateObjects(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Avatar status
	virtual void UpdateAvatarStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	// Low level render tools
	void PreRendering(bool enableLight, Mesh* mesh);
	void PostRendering(Mesh* mesh);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f, bool rotate = false);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const int sizeX = 1, const int sizeY = 1, const int x = 0, const int y = 0, const float rotate = 0.f, const bool flip = false);
	void RenderMesh(Mesh *mesh, bool enableLight);

	// Render the main components of this scene
	void RenderGUI();
	void RenderMobileObjects();
	void RenderFixedObjects();
	void RenderLights();
	void RenderGround();
	void RenderSkybox();

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_FIRE_SECONDARY,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};

	// Matrices for Model, View and Projection stack
	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Mesh* meshList[NUM_GEOMETRY];


	// Game state
	enum STATE
	{
		PRE_START,
		STARTING,
		PLAYING,
		LEVEL_COMPLETED,
		GAMEOVER,
		GAMEOVER_TIME_LIMIT,
		GET_PLAYERNAME,
		GAMEOVER_TIME_LIMIT_END,
		NUM_OF_STATE,
	};
	int currentState;

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	// Handler to the camera
	Camera3 camera;

	// Rotation angle for use with the model stack
	float rotateAngle;

	// Window size
	int m_window_width;
	int m_window_height;

	vector<Light> lights;
	bool bLightEnabled;


	bool fpsSelected;
	bool fullscreenSelected;
	bool coloredSelected;
	float fps;

	// Handle to 3rd Person View's avatar
	CPlayInfo3PV* m_cAvatar;

	// The scene graph
	vector<CSceneNode*> m_cSceneGraphObjects;
	vector<float> crownedListTranslateY;
	vector<bool> crownedListUpward;


	// The spatial partition
	CSpatialPartition* m_cSpatialPartition;

	// Projectile
	CProjectileManager *m_cProjectileManager;

	// Length of ray projectile
	float lengthOfRayProjectile;

	// Bullet cooldown duration
	static const int maxWeapons = 2;
	float cooldownDuration[maxWeapons];

	// Current Level
	int currentLevel;


	// Store number of enemies for moving and crowned
	int noOfMovingEnemies;
	int noOfCrowned;

	// Increase number of enemies
	void AddEnemies();

	float startingTime;

	// For time-limit
	float timeLeft;
	Highscore highscore;
	CRecord playerRecord;
	string tempName;
	int currentLetter;
	bool letter[32];
};

#endif