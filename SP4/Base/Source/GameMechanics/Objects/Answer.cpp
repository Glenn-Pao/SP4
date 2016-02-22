#include "Answer.h"


CAnswer::CAnswer()
{
}
CAnswer::CAnswer(int id, bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, bool pickup, bool correct, Mesh* ModelMesh)
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
	this->setPickup(pickup);
	this->setCorrect(correct);

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
//set and get the pickup status of the object
void CAnswer::setPickup(bool pickup)
{
	this->pickup = pickup;
}
bool CAnswer::getPickup(void)
{
	return pickup;
}
//set and get the answer's correctness
void CAnswer::setCorrect(bool correct)
{
	this->correct = correct;
}
bool CAnswer::getCorrect(void)
{
	return correct;
}