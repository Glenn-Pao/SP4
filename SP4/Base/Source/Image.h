#pragma once
#include "UIFeature.h"
#include "Mesh.h"
class Image : public UIFeature
{
	Mesh* mesh;
public:

	void setMesh(Mesh* mesh);

	Mesh* getMesh();

	Image();
	Image(string ID, Mesh* mesh, Vector3 CurrentPos, Vector3 Scale);
	~Image();
};

