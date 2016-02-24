#pragma once
#include "BoundingBox.h"
#include "..\..\Mesh.h"

class Trigger
{
	Mesh* mesh;

	CBoundingBox* TriggerArea;

	Vector3 Position;
	Vector3 Scale;
public:

	Mesh* getMesh();
	Vector3 getScale();
	Vector3 getPosition();

	bool CheckCollision(CBoundingBox BoundingBox);
	Trigger(Mesh* mesh, Vector3 Position, Vector3 Scale);
	~Trigger();
};

