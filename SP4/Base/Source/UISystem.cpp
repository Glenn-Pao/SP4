#include "UISystem.h"


UISystem::UISystem()
{
	this->Animator = new UIAnimator();
}

vector<UIFeature*> UISystem::getUI_List()
{
	return ListOfUI;
}

void UISystem::addFeature(UIFeature* object)
{
	this->ListOfUI.push_back(object);
}

void UISystem::removeFeature(string ID)
{
}

Button* UISystem::FindButton(string ID)
{
	for (std::vector<UIFeature*>::iterator CurrentFeature = ListOfUI.begin(); CurrentFeature != ListOfUI.end(); CurrentFeature++)
	{
		if ((*CurrentFeature)->getID() == ID && (*CurrentFeature)->getUI_Type() == UIFeature::UT_BUTTON)
		{
			return static_cast<Button*>((*CurrentFeature));
		}
	} 
	return NULL;
}

Image* UISystem::FindImage(string ID)
{
	for (std::vector<UIFeature*>::iterator CurrentFeature = ListOfUI.begin(); CurrentFeature != ListOfUI.end(); CurrentFeature++)
	{
		if ((*CurrentFeature)->getID() == ID && (*CurrentFeature)->getUI_Type() == UIFeature::UT_IMAGE)
		{
			return static_cast<Image*>((*CurrentFeature));
		}
	}
	return NULL;
}

UIAnimator* UISystem::InvokeAnimator()
{
	return Animator;
}


void UISystem::Update(float mouseX, float mouseY, float dt)
{
	//Update Animator
	Animator->Update(dt);

	//Update UI List
	for (std::vector<UIFeature*>::iterator CurrentFeature = ListOfUI.begin(); CurrentFeature != ListOfUI.end(); ++CurrentFeature)
	{
		switch ((*CurrentFeature)->getUI_Type())
		{
		case UIFeature::UT_BUTTON:
		{
			static_cast<Button*>((*CurrentFeature))->Update(mouseX, mouseY, dt);
			break;
		}
		}
	}
}


UISystem::~UISystem()
{
}
