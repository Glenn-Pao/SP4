#pragma once
#include <vector>

#include "UIAnimator.h"
#include "UIFeature.h"
#include "Button.h"
#include "Image.h"
using std::vector;

class CSceneManager2D;

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
	Image* FindImage(string ID);

	void HandleEvent(float mouseX, float mouseY, float m_window_width, float m_window_height, float m_world_width, float m_world_height);

	void Update(float mouseX, float mouseY, float dt);
	void Render(CSceneManager2D& scene);
	UISystem();
	~UISystem();
};

