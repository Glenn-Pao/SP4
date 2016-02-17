#include "UISystem.h"


UISystem::UISystem()
{
}

void UISystem::addButton()
{
}

void UISystem::addBar()
{
}

void UISystem::removeFeature(string ID)
{
}

UIFeature* UISystem::Find(string ID)
{
	for (std::vector<UIFeature*>::iterator CurrentFeature = UIList.begin(); CurrentFeature != UIList.end(); ++CurrentFeature)
	{
		if ((*CurrentFeature)->getID() == ID)
		{
			return (*CurrentFeature);
		}
	} 
}

UISystem::~UISystem()
{
}
