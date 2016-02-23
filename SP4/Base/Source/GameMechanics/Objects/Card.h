#pragma once
#include "..\..\GameMechanics\Objects\Objects.h"

class Card : public CObjects
{
public:

	enum Number
	{
		ZERO,
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE
	};

	Mesh* CardFaceUpMesh;

	void setNumber_Type(Number Number_Type);
	void setCardFaceUpMesh(Mesh* CardFaceUpMesh);

	Mesh* getCardFaceUpMesh();
	int getNumber_Type();

	Card(OBJECT_TYPE m_Object_Type, bool active, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh, Mesh* CardFaceUpMesh, Number Number_Type);
	~Card();

private:

	Number Number_Type;
};

