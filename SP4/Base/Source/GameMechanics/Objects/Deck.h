#pragma once
#include "..\..\GameMechanics\Objects\Card.h"
#include "Trigger.h"
#include <vector>

using std::vector;

class Deck
{
public:
	enum Deck_Type
	{
		DRAW,
		HANDR,
		HANDB,
		HANDG,
		DISCARD
	};
	vector<Card*> getListOfCards();
	Deck_Type getDeckType();

	bool CheckTrigger(CBoundingBox BoundingBox);

	Trigger* getTrigger();

	void AddCard(Card* card);
	void RemoveCard(int Placing);


	Deck(Deck_Type type, Vector3 Position, Vector3 GapBetweenCards, Trigger* button);

	~Deck();
private:

	Trigger* button;

	Deck_Type type;

	vector<Card*> ListOfCards;

	Vector3 Position;

	Vector3 GapBetweenCards;
};

