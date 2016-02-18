#pragma once
#include <vector>
#include "UIFeature.h"
#include "Button.h"

using std::vector;

class UISystem
{
private:
	vector<UIFeature*> ListOfUI;
	
public:
	vector<UIFeature*> getUI_List();

	void addFeature(UIFeature* object);
	void removeFeature(string ID);

	Button* FindButton(string ID);

	void Update(float mouseX, float mouseY, float dt);
	UISystem();
	~UISystem();
};

