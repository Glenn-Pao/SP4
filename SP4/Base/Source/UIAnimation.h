#pragma once
#include "UIFeature.h"

class UIAnimation
{
private:
	enum Transformation
	{
		TRANSLATION,
		ROTATION,
		SCALING
	};

	UIFeature* feature;
	Vector3 EndTransformation;
	float AnimationStartTime;
	float Speed;
	int choice;

	bool isComplete;
	float Timer;
public:

	void setTimer(float Timer);
	void setisComplete(bool isComplete);

	bool getisComplete();
	float getTimer();

	void Update(float dt);

	void DoAnimation(float dt);

	UIAnimation(UIFeature* feature, float AnimationStartTime, Vector3 EndTransformation, float Speed, int choice); // choice: 1 = translation, 2 = rotation, 3 = scaling
	~UIAnimation();
};

