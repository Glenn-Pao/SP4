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

	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5), this->getPosition().y + (getScale().y * 0.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5), this->getPosition().y - (getScale().y * 0.5), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);
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