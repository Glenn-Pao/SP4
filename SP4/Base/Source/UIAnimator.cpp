#include "UIAnimator.h"


UIAnimator::UIAnimator()
{
	isActive = true;
}
void UIAnimator::Activate()
{
	this->isActive = true;
}

void UIAnimator::Deactivate()
{
	this->isActive = false;
}

void UIAnimator::Update(float dt)
{
	if (isActive == true)
	{
		for (int i = 0; i < AnimationList.size(); ++ i)
		{
			if (AnimationList[i]->getisComplete() == false)
			{
				AnimationList[i]->Update(dt);
			}
			else
			{
				AnimationList.erase(AnimationList.begin() + i);
			}
		}
	}
	if (isActive == false)
	{
	}
}

void UIAnimator::StartTransformation(UIFeature* feature, float AnimationStartTime, Vector3 Target, float Speed, int choice)
{
	UIAnimation* Animation = new UIAnimation(feature, AnimationStartTime, Target, Speed, choice);
	AnimationList.push_back(Animation);
}

void UIAnimator::StopAnimations()
{
	for (int i = AnimationList.size() - 1; i >= 0; --i)
	{
		AnimationList.erase(AnimationList.begin() + i);
	}
}


int UIAnimator::GetNumOfAnimations()
{
	return AnimationList.size();
}


UIAnimator::~UIAnimator()
{
}
