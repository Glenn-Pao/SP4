#include "Deck.h"


Deck::Deck(Vector3 Position, Vector3 GapBetweenCards)
{
	this->Position = Position;
	this->GapBetweenCards = GapBetweenCards;
}
void Deck::AddCard(Card* card)
{
	card->setPosition((this->Position + (this->GapBetweenCards * ListOfCards.size())));
	card->getBoundingBox()->Reset(card->getPosition(), card->getScale());
	this->ListOfCards.push_back(card);
}

Vector3 Deck::getPosition()
{
	return Position;
}

vector<Card*> Deck::getListOfCards()
{
	return ListOfCards;
}

void Deck::RemoveCard(int Placing)
{
	ListOfCards.erase(ListOfCards.begin() + Placing);
}

Deck::~Deck()
{
}
