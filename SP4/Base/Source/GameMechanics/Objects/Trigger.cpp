#include "Trigger.h"


Trigger::Trigger(Mesh* mesh, Vector3 Position, Vector3 Scale)
{
	this->mesh = mesh;
	this->Position = Position;
	this->Scale = Scale;

	Vector3 TopLeft(Position.x, Position.y + (Scale.y), 0);
	Vector3 BottomRight(Position.x + (Scale.x), Position.y, 0);

	this->TriggerArea = new CBoundingBox(TopLeft, BottomRight);
	
	this->TriggerArea = TriggerArea;
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

bool Trigger::CheckCollision(CBoundingBox BoundingBox)
{
	return TriggerArea->CheckCollision(BoundingBox);
}

Trigger::~Trigger()
{
}
