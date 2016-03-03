#pragma once
#include "UIFeature.h"

class UIAnimation
{
private:

	UIFeature* feature;
	Vector3 EndTransformation;
	float AnimationStartTime;
	float Speed;
	int choice;

	bool isComplete;
	float Timer;
public:
	enum Transformation
	{
		TRANSLATION,
		ROTATION,
		SCALING
	};

	void setTimer(float Timer);
	void setisComplete(bool isComplete);

	bool getisComplete();
	float getTimer();

	void Update(float dt);

	void DoAnimation(float dt);
	void SkipAnimation();

	UIAnimation(UIFeature* feature, float AnimationStartTime, Vector3 EndTransformation, float Speed, int choice); // choice: 1 = translation, 2 = rotation, 3 = scaling
	~UIAnimation();
};

