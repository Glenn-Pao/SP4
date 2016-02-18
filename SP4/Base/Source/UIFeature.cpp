#include "UIFeature.h"


UIFeature::UIFeature()
{
}

void UIFeature::Update(float dt)
{

}

void UIFeature::setID(string ID)
{
	this->ID = ID;
}

void UIFeature::setDefaultPos(Vector3 DefaultPos)
{
	this->DefaultPos = DefaultPos;
}

void UIFeature::setCurrentPos(Vector3 CurrentPos)
{
	this->CurrentPos = CurrentPos;
}

void UIFeature::setScale(Vector3 Scale)
{
	this->Scale = Scale;
}

void UIFeature::setUI_Type(UI_TYPE Scale)
{
	this->type = type;
}

int UIFeature::getUI_Type()
{
	return type;
}

Vector3 UIFeature::getScale()
{
	return Scale;
}

Vector3 UIFeature::getCurrentPos()
{
	return CurrentPos;
}

Vector3 UIFeature::getDefaultPos()
{
	return DefaultPos;
}

string UIFeature::getID()
{
	return ID;
}

UIFeature::~UIFeature()
{
}
