#include "Question.h"


CQuestion::CQuestion()
{
}
CQuestion::CQuestion(CObjects::OBJECT_TYPE m_Object_Type,int id, bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh)
{
	this->setObjType(m_Object_Type);
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

CQuestion::~CQuestion()
{
}
//set and get the id number of the object
void CQuestion::setID(int id)
{
	this->id = id;
}
int CQuestion::getID(void)
{
	return id;
}
//set and get the interactivity of the object
void CQuestion::setInteractivity(bool interactable)
{
	this->interactable = interactable;
}
bool CQuestion::getInteractivity(void)
{
	return interactable;
}