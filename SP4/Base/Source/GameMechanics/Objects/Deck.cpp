#include "Deck.h"


Deck::Deck(Deck_Type type, Vector3 Position, Vector3 GapBetweenCards, Trigger* button)
{
	this->type = type;
	this->Position = Position;
	this->GapBetweenCards = GapBetweenCards;
	this->button = button;
}
void Deck::AddCard(Card* card)
{
	card->setPosition((this->Position + (this->GapBetweenCards * ListOfCards.size())));
	this->ListOfCards.push_back(card);
}

bool Deck::CheckTrigger(CBoundingBox BoundingBox)
{
	return this->button->CheckCollision(BoundingBox);
}

Trigger* Deck::getTrigger()
{
	return button;
}

vector<Card*> Deck::getListOfCards()
{
	return ListOfCards;
}

Deck::Deck_Type Deck::getDeckType()
{
	return type;
}

void Deck::RemoveCard(int Placing)
{
	ListOfCards.erase(ListOfCards.begin() + Placing);
}

Deck::~Deck()
{
}
