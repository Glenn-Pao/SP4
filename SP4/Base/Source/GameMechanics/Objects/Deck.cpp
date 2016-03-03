#include "Deck.h"


Deck::Deck(Vector3 Position, Vector3 GapBetweenCards)
{
	this->Position = Position;
	this->GapBetweenCards = GapBetweenCards;
}
void Deck::AddCard(Card* card)
{
	card->setPosition((this->Position + (this->GapBetweenCards * (float)ListOfCards.size())));
	card->getBoundingBox()->Reset(card->getPosition(), card->getScale());
	this->ListOfCards.push_back(card);
}

void Deck::setAllCardTo(Card* card)
{
	for (int i = 0; i < (int)this->getListOfCards().size(); ++i)
	{
		this->getListOfCards()[i] = card;
	}
}

Vector3 Deck::getPosition()
{
	return Position;
}

bool Deck::isDeckIdentical(Deck* deck)
{
	if (deck->getListOfCards().size() == this->getListOfCards().size())
	{
		for (int i = 0; i < (int)this->getListOfCards().size(); ++i)
		{
			if (this->getListOfCards()[i]->getElement_Type() != deck->getListOfCards()[i]->getElement_Type())
			{
				return false;
			}
		}
		return true;
	}
	return false;
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
