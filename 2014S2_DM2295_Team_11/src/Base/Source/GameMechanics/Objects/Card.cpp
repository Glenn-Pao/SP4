#include "Card.h"


Card::Card(OBJECT_TYPE m_Object_Type, bool active, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh, Mesh* CardFaceUpMesh, Element Element_Type, bool isRevealed/*, bool isHeld*/)
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

	//initialize this first
	this->setMesh(ModelMesh);


	Vector3 TopLeft(position.x - (scale.x * 0.5f), position.y + (scale.y * 0.5f), 0);
	Vector3 BottomRight(position.x + (scale.x * 0.5f), position.y - (scale.y * 0.5f), 0);

	this->setBoundingBox(TopLeft, BottomRight);

	this->isRevealed = isRevealed;

	this->CardFaceUpMesh = CardFaceUpMesh;
	this->Element_Type = Element_Type;
	//this->isHeld = isHeld
}

void Card::setElement_Type(Element Element_Type)
{
	this->Element_Type = Element_Type;
}

void Card::setCardFaceUpMesh(Mesh* CardFaceUpMesh)
{
	this->CardFaceUpMesh = CardFaceUpMesh;
}

void Card::setisRevealed(bool isRevealed)
{
	this->isRevealed = isRevealed;
}

bool Card::getisRevealed()
{
	return isRevealed;
}

Mesh* Card::getCardFaceUpMesh()
{
	return CardFaceUpMesh;
}

Card::Element Card::getElement_Type()
{
	return Element_Type;
}

Card::~Card()
{
}
