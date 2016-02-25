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

	//void setisHeld(bool isHeld);
	void setCardFaceUpMesh(Mesh* CardFaceUpMesh);
	void setElement_Type(Element Element_Type);
	void setisRevealed(bool isRevealed);
	bool getisRevealed();

	bool isRevealed;

	//bool getisHeld();
	Mesh* getCardFaceUpMesh();
	Card::Element getElement_Type();

	Card(OBJECT_TYPE m_Object_Type, bool active, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh, Mesh* CardFaceUpMesh, Element Element_Type, bool isRevealed/*, bool isHeld*/);
	~Card();

private:
	//bool isHeld;

	Mesh* CardFaceUpMesh;
	Element Element_Type;
};

