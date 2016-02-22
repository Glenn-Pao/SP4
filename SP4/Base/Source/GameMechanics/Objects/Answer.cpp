#include "Answer.h"


CAnswer::CAnswer()
{
}
CAnswer::CAnswer(int id, bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh)
{
	this->id = id;
	this->setActive(active);
	this->setInteractivity(interactable);
	this->setDialogue(dialogue);
	this->setPosition(position);
	this->setDefaultPosition(position);
	this->setRotatation(rotate);
	this->setDefaultRotatation(rotate);
	this->setScale(scale);
	this->setDefaultScale(scale);
	this->setMesh(ModelMesh);

	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 1.5), this->getPosition().y + (getScale().y * 1.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 1.5), this->getPosition().y - (getScale().y * 1.5), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);
}

CAnswer::~CAnswer()
{
}
//set and get the id number of the object
void CAnswer::setID(int id)
{
	this->id = id;
}
int CAnswer::getID(void)
{
	return id;
}