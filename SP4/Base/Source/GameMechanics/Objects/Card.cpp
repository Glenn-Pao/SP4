#include "Card.h"


Card::Card(OBJECT_TYPE m_Object_Type, bool active, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh, Mesh* CardFaceUpMesh, State CurrentState_Type, Element Element_Type/*, bool isHeld*/)
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


	Vector3 TopLeft(position.x - (scale.x * 0.25), position.y + (scale.y * 0.75), 0);
	Vector3 BottomRight(position.x + (scale.x * 0.75), position.y - (scale.y * 0.25), 0);

	this->setBoundingBox(TopLeft, BottomRight);

	this->CurrentState_Type = CurrentState_Type;
	this->PreviouseState_Type = CurrentState_Type;
	this->CardFaceUpMesh = CardFaceUpMesh;
	this->Element_Type = Element_Type;
	//this->isHeld = isHeld
}

//void Card::setisHeld(bool isHeld)
//{
//	this->isHeld = isHeld;
//}

void Card::setCurrentState_Type(Card::State CurrentState_Type)
{
	this->CurrentState_Type = CurrentState_Type;
}

void Card::setPreviouseState_Type(Card::State PreviouseState_Type)
{
	this->CurrentState_Type = PreviouseState_Type;
}

void Card::setElement_Type(Element Element_Type)
{
	this->Element_Type = Element_Type;
}

void Card::setCardFaceUpMesh(Mesh* CardFaceUpMesh)
{
	this->CardFaceUpMesh = CardFaceUpMesh;
}

//bool Card::getisHeld()
//{
//	return isHeld;
//}

Card::State Card::getCurrentState_Type()
{
	return CurrentState_Type;
}

Card::State Card::getPreviouseState_Type()
{
	return PreviouseState_Type;
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
