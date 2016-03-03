#ifndef SCENE_MANAGER_2D_H
#define SCENE_MANAGER_2D_H

#include "Scene.h"
#include "Mtx44.h"
#include "..\..\Camera3.h"
#include "..\..\Mesh.h"
#include "MatrixStack.h"
#include "..\..\Light.h"

class CSceneManager2D : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_LIGHTENABLED,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

public:
	CSceneManager2D();
	~CSceneManager2D();

	virtual void Init(int level);
	virtual void PreInit();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderBackground(bool spriteAnimation = false);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const int sizeX = 1, const int sizeY = 1, const int x = 0, const int y = 0, const float rotate = 0.f, const bool flip = false);

	enum GEOMETRY_TYPE
	{
		GEO_BACKGROUND,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	Mesh* meshList[NUM_GEOMETRY];
	float fps;

	// Window size
	int m_window_width;
	int m_window_height;

	MS modelStack;
private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS viewStack;
	MS projectionStack;
};

#endif