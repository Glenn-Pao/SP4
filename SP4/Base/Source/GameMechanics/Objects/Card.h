#pragma once
#include "..\..\GameMechanics\Objects\Objects.h"

class Card : public CObjects
{
public:

	enum Element
	{
		NONE,
		FIRE,
		WATER,
		LEAF,
		ALL
	};

	enum State
	{
		ON_DRAW,
		ON_DRAG,
		ON_HAND,
		ON_DISCARD
	};

	//void setisHeld(bool isHeld);
	void setCardFaceUpMesh(Mesh* CardFaceUpMesh);
	void setCurrentState_Type(Card::State CurrentState_Type);
	void setPreviouseState_Type(Card::State PreviouseStateType);
	void setElement_Type(Element Element_Type);
	
	//bool getisHeld();
	Mesh* getCardFaceUpMesh();
	Card::State getPreviouseState_Type();
	Card::State getCurrentState_Type();
	Card::Element getElement_Type();

	Card(OBJECT_TYPE m_Object_Type, bool active, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh, Mesh* CardFaceUpMesh, State CurrentState_Type, Element Element_Type/*, bool isHeld*/);
	~Card();

private:
	//bool isHeld;

	Mesh* CardFaceUpMesh;

	State CurrentState_Type, PreviouseState_Type;

	Element Element_Type;
};

