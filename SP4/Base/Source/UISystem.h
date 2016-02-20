#pragma once
#include <vector>

#include "UIAnimator.h"
#include "UIFeature.h"
#include "Button.h"

using std::vector;

class UISystem
{
private:
	vector<UIFeature*> ListOfUI;
	UIAnimator* Animator;

public:
	vector<UIFeature*> getUI_List();

	void addFeature(UIFeature* object);
	void removeFeature(string ID);

	UIAnimator* InvokeAnimator();


	Button* FindButton(string ID);

	void Update(float mouseX, float mouseY, float dt);
	UISystem();
	~UISystem();
};

