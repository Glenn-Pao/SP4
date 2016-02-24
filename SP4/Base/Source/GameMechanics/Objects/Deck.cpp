#include "Deck.h"


Deck::Deck(Vector3 Position, Vector3 GapBetweenCards)
{
	this->Position = Position;
	this->GapBetweenCards = GapBetweenCards;
}
void Deck::AddCard(Card* card)
{
	card->setPosition((this->Position + (this->GapBetweenCards * ListOfCards.size())));
	this->ListOfCards.push_back(card);
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
