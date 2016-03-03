#include "UISystem.h"

#include "Mtx44.h"
#include "GL\glew.h"
#include "Scenes\Master\SceneManager2D.h"
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

void UISystem::Render(CSceneManager2D& scene)
{
	for (std::vector<UIFeature*>::iterator CurrentFeature = ListOfUI.begin(); CurrentFeature != ListOfUI.end(); CurrentFeature++)
	{
		switch ((*CurrentFeature)->getUI_Type())
		{
			case UIFeature::UT_BUTTON:
			{
				Button* button;

				button = static_cast<Button*>((*CurrentFeature));

				if (button->getActive() == true)
				{
					scene.Render2DMesh(button->getCurrentMesh(), false,
						(int)(button->getScale().x),
						(int)(button->getScale().y),
						(int)(button->getCurrentPos().x),
						(int)(button->getCurrentPos().y),
						0,
						false);
				}
				 break;
			}

			case UIFeature::UT_IMAGE:
			{
				Image* image;

				image = static_cast<Image*>((*CurrentFeature));

				scene.Render2DMesh(image->getMesh(), false,
					(int)(image->getScale().x),
					(int)(image->getScale().y),
					(int)(image->getCurrentPos().x),
					(int)(image->getCurrentPos().y),
					0,
					false);
				break;
			}
		}
	}
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

void UISystem::HandleEvent(float mouseX, float mouseY, float m_window_width, float m_window_height, float m_world_width, float m_world_height)
{
	//Update UI List
	for (std::vector<UIFeature*>::iterator CurrentFeature = ListOfUI.begin(); CurrentFeature != ListOfUI.end(); ++CurrentFeature)
	{
		switch ((*CurrentFeature)->getUI_Type())
		{
			case UIFeature::UT_BUTTON:
			{
				static_cast<Button*>((*CurrentFeature))->HandleEvent(mouseX / m_window_width * m_world_width, mouseY / m_window_height * m_world_height);
				break;
			}
		}
	}
}

void UISystem::Update(float dt)
{
	//Update Animator
	Animator->Update(dt);
}


UISystem::~UISystem()
{
}
