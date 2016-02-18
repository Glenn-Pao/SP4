#pragma once
#include <vector>
#include "UIFeature.h"
using std::vector;

class UISystem
{
	vector<UIFeature*> UIList;

	void addButton();
	void addBar();

	void removeFeature(string ID);

	UIFeature* Find(string ID);

public:
	UISystem();
	~UISystem();
};

