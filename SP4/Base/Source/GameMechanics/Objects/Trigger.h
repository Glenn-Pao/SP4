#pragma once
#include "BoundingBox.h"
#include "..\..\Mesh.h"

class Trigger
{
	//Trigger Variables
	CBoundingBox* TriggerArea;
	Vector3 Position;
	Vector3 Scale;

	//Mesh vairables
	Mesh* mesh;
	Vector3 MeshPosition;
	Vector3 MeshScale;

	bool isActive;

public:

	Mesh* getMesh();
	Vector3 getScale();
	Vector3 getPosition();
	Vector3 getMeshPosition();
	Vector3 getMeshScale();
	CBoundingBox* getBoundingBox();
	bool getActive();

	void setMesh(Mesh* mesh);
	void setScale(Vector3 Scale);
	void setPosition(Vector3 Position);
	void setActive(bool isActive);
	void setMeshPosition(Vector3 MeshPosition);
	void setMeshScale(Vector3 MeshScale);

	bool CheckCollision(CBoundingBox BoundingBox);
	Trigger(Mesh* mesh, Vector3 Position, Vector3 Scale, Vector3 MeshPosition, Vector3 MeshScale, bool isActive);
	~Trigger();
};

