#pragma once

#include "UIAnimation.h"
#include "UIFeature.h"
#include <vector>

using std::vector;

class UIAnimator
{
private:
	bool isActive;
	vector<UIAnimation*> AnimationList;

public:

	void Activate();
	void Deactivate();

	void Update(float dt);

	void StartTransformation(UIFeature* feature, float AnimationStartTime, Vector3 Target, float Speed, int choice);
	void StopAnimations();
	int GetNumOfAnimations();
	UIAnimator();
	~UIAnimator();
};

