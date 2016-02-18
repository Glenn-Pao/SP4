#include "Button.h"

Button::Button()
{
}

Button::Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Vector3 CurrentPos, Vector3 Scale)
{
	//Parent class variables
	this->setID(ID);
	this->setCurrentPos(CurrentPos);
	this->setDefaultPos(CurrentPos);
	this->setScale(Scale);

	//Child class variables
	this->ButtonMeshUP = ButtonMeshUP;
	this->ButtonMeshDOWN = ButtonMeshDOWN;
	this->isClicked = false;
	this->isHovered = false;
	
	//External Class variables


	Vector3 TopLeft(CurrentPos.x - (Scale.x * 0.5), CurrentPos.y + (Scale.y * 0.5), 0);
	Vector3 BottomRight(CurrentPos.x + (Scale.x * 0.5), CurrentPos.y - (Scale.y * 0.5), 0);
	
	
	this->CollisionBox = new CBoundingBox(TopLeft, BottomRight);
}

Button::Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Vector3 CurrentPos, bool isClicked, bool isHovered)
{
	this->setID(ID);
	this->ButtonMeshUP = ButtonMeshUP;
	this->ButtonMeshDOWN = ButtonMeshDOWN;
	this->setCurrentPos(CurrentPos);
	this->isClicked = isClicked;
	this->isHovered = isHovered;
}

void Button::setButtonMeshUP(Mesh* ButtonMeshUP)
{
	this->ButtonMeshUP = ButtonMeshUP;
}

void Button::setButtonMeshDOWN(Mesh*ButtonMeshDOWN)
{
	this->ButtonMeshDOWN = ButtonMeshDOWN;
}

void Button::setisClicked(bool isClicked)
{
	this->isClicked = isClicked;
}

void Button::setisHovered(bool isHovered)
{
	this->isHovered = isHovered;
}

Mesh* Button::getButtonMeshUP()
{
	return ButtonMeshUP;
}

Mesh* Button::getButtonMeshDOWN()
{
	return ButtonMeshDOWN;
}

bool Button::getisClicked()
{
	return isClicked;
}

bool Button::getisHovered()
{
	return isHovered;
}

CBoundingBox* Button::getCollisionBox()
{
	return CollisionBox;
}

UIAnimation* Button::InvokeAnimator()
{
	return Animator;
}

void Button::Update(float MouseX, float MouseY, float dt)
{
	if (CollisionBox->CheckCollision(Vector3(MouseX, MouseY, 0)) == true)
	{
		this->setisHovered(true);
	}
	else
	{
		this->setisHovered(false);
	}
/*
	this->getCollisionBox()->setTopLeftCorner(this->getCurrentPos() + this->getScale());
	this->getCollisionBox()->setBottomRightCorner(this->getCurrentPos() - this->getScale());*/
}


Button::~Button()
{
}
