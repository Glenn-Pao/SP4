#pragma once
#include "..\..\GameMechanics\Objects\Card.h"
#include <vector>

using std::vector;

class Deck
{
	vector<Card*> ListOfCards;

	Vector3 Position;

	Vector3 GapBetweenCards;

public:

	vector<Card*> getListOfCards();

	void AddCard(Card* card);
	void RemoveCard(int Placing);

	Deck(Vector3 Position, Vector3 GapBetweenCards);
	~Deck();
};

