#include "Button.h"

#include "UsingLua.h"

Button::Button()
	: screenheight(600)
{
}

Button::Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Mesh* ButtonMeshLOCKED, Vector3 CurrentPos, Vector3 Scale, bool active)
{
	//Parent class variables
	this->setID(ID);
	this->setCurrentPos(CurrentPos);
	this->setDefaultPos(CurrentPos);
	this->setScale(Scale);
	this->setUI_Type(UIFeature::UT_BUTTON);

	//Child class variables
	this->ButtonMeshUP = ButtonMeshUP;
	this->ButtonMeshDOWN = ButtonMeshDOWN;
	this->ButtonMeshLOCKED = ButtonMeshLOCKED;
	this->CurrentMesh = ButtonMeshUP;
	this->isClicked = false;
	this->isHovered = false;
	this->isLocked = false;
	this->active = active;

	//External Class variables
	Vector3 TopLeft(CurrentPos.x - (Scale.x * 0.5), CurrentPos.y + (Scale.y * 0.5), 0);
	Vector3 BottomRight(CurrentPos.x + (Scale.x * 0.5), CurrentPos.y - (Scale.y * 0.5), 0);
	
	
	this->CollisionBox = new CBoundingBox(TopLeft, BottomRight);

	UseLuaFiles L;

	L.ReadFiles("Lua//config.lua");

	screenheight = L.DoLuaInt("SCREENHEIGHT");
}

Button::Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Mesh* ButtonMeshLOCKED, Vector3 CurrentPos, Vector3 Scale, bool isClicked, bool isHovered)
{
	this->setID(ID);
	this->setCurrentPos(CurrentPos);
	this->setDefaultPos(CurrentPos);
	this->setScale(Scale);
	this->setUI_Type(UIFeature::UT_BUTTON);

	this->ButtonMeshUP = ButtonMeshUP;
	this->ButtonMeshDOWN = ButtonMeshDOWN;
	this->ButtonMeshLOCKED = ButtonMeshLOCKED;
	this->CurrentMesh = ButtonMeshUP;
	this->isClicked = isClicked;
	this->isHovered = isHovered;
	this->isLocked = false;

	Vector3 TopLeft(CurrentPos.x - (Scale.x * 0.5), CurrentPos.y + (Scale.y * 0.5), 0);
	Vector3 BottomRight(CurrentPos.x + (Scale.x * 0.5), CurrentPos.y - (Scale.y * 0.5), 0);


	this->CollisionBox = new CBoundingBox(TopLeft, BottomRight);
}

void Button::setisClicked(bool isClicked)
{
	this->isClicked = isClicked;
}

void Button::setisHovered(bool isHovered)
{
	this->isHovered = isHovered;
}

void Button::setisLocked(bool isLocked)
{
	this->isLocked = isLocked;
}

bool Button::getisClicked()
{
	return isClicked;
}

bool Button::getisHovered()
{
	return isHovered;
}

bool Button::getisLocked()
{
	return isLocked;
}

CBoundingBox* Button::getCollisionBox()
{
	return CollisionBox;
}

void Button::HandleEvent(float MouseX, float MouseY)
{
	if (isLocked)
	{
		this->CurrentMesh = this->ButtonMeshLOCKED;
		isHovered = false;
	}
	else
	{
		if (CollisionBox->CheckCollision(Vector3(MouseX, screenheight - MouseY, 0)) == true)
		{
			this->CurrentMesh = this->ButtonMeshDOWN;
			isHovered = true;
		}
		else
		{
			this->CurrentMesh = this->ButtonMeshUP;
			isHovered = false;
		}
	}

	//Update CollideBox Position
	this->getCollisionBox()->Reset(this->getCurrentPos(),this->getScale());
/*
	this->getCollisionBox()->setTopLeftCorner(this->getCurrentPos() + this->getScale());
	this->getCollisionBox()->setBottomRightCorner(this->getCurrentPos() - this->getScale());*/
}

Mesh* Button::getCurrentMesh()
{
	return CurrentMesh;
}
void Button::setActive(bool active)
{
	this->active = active;
}
bool Button::getActive(void)
{
	return active;
}
Button::~Button()
{
}
