#pragma once
#include <vector>
#include "UIAnimation.h"
#include "UIFeature.h"
using std::vector;

class UISystem
{
	vector<UIFeature*> UIList;

	UIAnimation Animator;

	void addButton();
	void addBar();

	void removeFeature(string ID);

	UIFeature* Find(string ID);

public:
	UISystem();
	~UISystem();
};

