#include "Door.h"


CDoor::CDoor()
{
	this->id = -1;
}


CDoor::CDoor(int id, Vector3 pos, Vector3 scale, Mesh* mesh)
{
	this->id = id;

	this->setPosition(pos);
	this->setScale(scale);
	this->setMesh(mesh);

	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5), this->getPosition().y + (getScale().y * 0.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5), this->getPosition().y - (getScale().y * 0.5), 0);
	this->CollideBox = new CBoundingBox(topleft, bottomright);

}

CDoor::~CDoor()
{

}

void CDoor::setId(int id)
{
	this->id = id;
}

int CDoor::getId()
{
	return id;
}

CBoundingBox* CDoor::getCollideBox()
{
	return CollideBox;
}