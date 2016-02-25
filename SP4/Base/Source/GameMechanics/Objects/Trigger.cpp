#include "Trigger.h"


Trigger::Trigger(Mesh* mesh, Vector3 Position, Vector3 Scale, Vector3 MeshPosition, Vector3 MeshScale, bool isActive)
{
	this->mesh = mesh;
	this->Position = Position;
	this->Scale = Scale;
	this->MeshPosition = MeshPosition;
	this->MeshScale = MeshScale;

	Vector3 TopLeft(Position.x, Position.y + (Scale.y), 0);
	Vector3 BottomRight(Position.x + (Scale.x), Position.y, 0);

	this->TriggerArea = new CBoundingBox(TopLeft, BottomRight);
	
	this->TriggerArea = TriggerArea;

	this->isActive = isActive;
}

Mesh* Trigger::getMesh()
{
	return mesh;
}

Vector3 Trigger::getScale()
{
	return Scale;
}

Vector3 Trigger::getPosition()
{
	return Position;
}

Vector3 Trigger::getMeshPosition()
{
	return MeshPosition;
}

Vector3 Trigger::getMeshScale()
{
	return MeshScale;
}

bool Trigger::getActive()
{
	return isActive;
}

CBoundingBox* Trigger::getBoundingBox()
{
	return TriggerArea;
}

bool Trigger::CheckCollision(CBoundingBox BoundingBox)
{
	return TriggerArea->CheckCollision(BoundingBox);
}

void Trigger::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void Trigger::setScale(Vector3 Scale)
{
	this->Scale = Scale;
}

void Trigger::setMeshPosition(Vector3 MeshPosition)
{
	this -> MeshPosition = MeshPosition;
}
void setMeshScale(Vector3 MeshScale);

void Trigger::setMeshScale(Vector3 Scale)
{
	this->MeshScale = MeshScale;
}

void Trigger::setPosition(Vector3 Position)
{
	this->Position = Position;
}

void Trigger::setActive(bool isActive)
{
	this->isActive = isActive;
}

Trigger::~Trigger()
{
}
