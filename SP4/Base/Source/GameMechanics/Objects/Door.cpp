#include "Door.h"


CDoor::CDoor()
{
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

	
	DoorType = "";
	theColour = new CColour();
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

void CDoor::setDoorType(std::string DoorType, std::string doorColour)
{
	this->DoorType = DoorType;
	if (doorColour != "")
	{
		if (doorColour == "GREEN")
		{
			theColour->setColour(doorColour);
		}
	}
}

std::string CDoor::getDoorType()
{
	return DoorType;
}

std::string CDoor::getColour()
{
	return theColour->getColour();
}


CColour::CColour()
{
}

CColour::CColour(std::string colour, Vector3 pos, Vector3 scale, Mesh* mesh)
{
	this->colour = colour;

	this->setPosition(pos);
	this->setScale(scale);
	this->setMesh(mesh);

	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5), this->getPosition().y + (getScale().y * 0.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5), this->getPosition().y - (getScale().y * 0.5), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);
}

CColour::CColour(std::string colour)
{
	this->colour = colour;
}

CColour::~CColour()
{
}

void CColour::setColour(std::string colour)
{
	this->colour = colour;
}

std::string CColour::getColour()
{
	return colour;
}