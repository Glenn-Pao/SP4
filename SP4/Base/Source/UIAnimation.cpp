#include "UIAnimation.h"
#include <iostream>

using std::cout;
using std::endl;

UIAnimation::UIAnimation(UIFeature* feature, float AnimationStartTime, Vector3 EndTransformation, float Speed, int choice)
{
	this->feature = feature;
	this->AnimationStartTime = AnimationStartTime;
	this->EndTransformation = EndTransformation;
	this->Speed = Speed;
	this->choice = choice;

	this->isComplete = false;
	this->Timer = 0;
}


void UIAnimation::setTimer(float Timer)
{
	this->Timer = Timer;
}

void UIAnimation::setisComplete(bool isComplete)
{
	this->isComplete = isComplete;
}

bool UIAnimation::getisComplete()
{
	return isComplete;
}

float UIAnimation::getTimer()
{
	return Timer;
}

void UIAnimation::Update(float dt)
{
	this->Timer += dt;
	if (Timer > AnimationStartTime && isComplete == false)
	{
		DoAnimation(dt);
	}
}

void UIAnimation::DoAnimation(float dt)
{
	switch (this->choice)
	{
		case TRANSLATION: // if translation
		{
			if ((feature->getCurrentPos() - EndTransformation).Length() < 2)
			{
				feature->setCurrentPos(EndTransformation);
				isComplete = true;
     		}
			else
			{
				Vector3 temp(feature->getCurrentPos() + ((EndTransformation - feature->getCurrentPos()) * Speed * dt));
				feature->setCurrentPos(temp);
			}
			break;
		}
		case ROTATION:
		{
			 break;
		}
		case SCALING:
		{
			if ((feature->getScale() - EndTransformation).Length() < 5)
			{
				feature->setScale(EndTransformation);
				isComplete = true;
			}
			else
			{
				if ((feature->getScale() - EndTransformation).x < 0 || (feature->getScale() - EndTransformation).y < 0)
				{
					Vector3 temp(feature->getScale() + ((EndTransformation + feature->getScale()).Normalized() * Speed * dt));
					feature->setScale(temp);
				}
				if ((feature->getScale() - EndTransformation).x > 0 || (feature->getScale() - EndTransformation).y > 0)
				{
					Vector3 temp(feature->getScale() + ((EndTransformation - feature->getScale()).Normalized() * Speed * dt));
					feature->setScale(temp);
				}
				
			}
			break;
		}
	}
}

UIAnimation::~UIAnimation()
{
}
