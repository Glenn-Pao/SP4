#pragma once
#include "..\..\GameMechanics\Objects\Card.h"
#include "Trigger.h"
#include <vector>

using std::vector;

class Deck
{
public:

	vector<Card*> getListOfCards();

	bool CheckTrigger(CBoundingBox BoundingBox);

	Trigger* getTrigger();

	Vector3 getPosition();

	void AddCard(Card* card);
	void RemoveCard(int Placing);

	void setAllCardTo(Card* card);

	vector<Card*> ListOfCards;

	bool isDeckIdentical(Deck* deck);

	Deck(Vector3 Position, Vector3 GapBetweenCards);

	~Deck();
private:


	Vector3 Position;

	Vector3 GapBetweenCards;
};

