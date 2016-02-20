#include "Question.h"


CQuestion::CQuestion()
{
}
CQuestion::CQuestion(int id, bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh)
{
	this->id = id;
	CObjects(active, interactable, dialogue, position, rotate, scale, ModelMesh);

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