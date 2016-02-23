#include "Card.h"


Card::Card(OBJECT_TYPE m_Object_Type, bool active, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh, Mesh* CardFaceUpMesh, Number Number_Type)
{
	this->setObjType(m_Object_Type);
	this->setActive(active);
	this->setDialogue(dialogue);

	this->setDefaultPosition(position);
	this->setPosition(position);

	this->setDefaultRotatation(rotate);
	this->setRotatation(rotate);

	this->setDefaultScale(scale);
	this->setScale(scale);

	//this->ModelMesh = NULL;							//initialize this first
	this->setMesh(ModelMesh);

	Vector3 TopLeft(position.x - (scale.x * 0.5), position.y + (scale.y * 0.5), 0);
	Vector3 BottomRight(position.x + (scale.x * 0.5), position.y - (scale.y * 0.5), 0);


	this->setBoundingBox(TopLeft, BottomRight);

	this->CardFaceUpMesh = CardFaceUpMesh;
	this->Number_Type = Number_Type;
}

void Card::setNumber_Type(Number Number_Type)
{
	this->Number_Type = Number_Type;
}

void Card::setCardFaceUpMesh(Mesh* CardFaceUpMesh)
{
	this->CardFaceUpMesh = CardFaceUpMesh;
}

Mesh* Card::getCardFaceUpMesh()
{
	return CardFaceUpMesh;
}

int Card::getNumber_Type()
{
	return Number_Type;
}

Card::~Card()
{
}
