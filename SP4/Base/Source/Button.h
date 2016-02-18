#pragma once
#include "Mesh.h"
#include "Vector3.h"
#include "UIFeature.h"
#include "UIAnimation.h"
#include "GameMechanics\Objects\BoundingBox.h"

class Button : public UIFeature
{
private:
	Mesh* ButtonMeshUP;
	Mesh*ButtonMeshDOWN;

	bool isClicked;
	bool isHovered;

	CBoundingBox* CollisionBox;
	UIAnimation* Animator;
public:

	void setButtonMeshUP(Mesh* ButtonMeshUP);
	void setButtonMeshDOWN(Mesh*ButtonMeshDOWN);
	void setisClicked(bool isClicked);
	void setisHovered(bool isHovered);

	Mesh* getButtonMeshUP();
	Mesh* getButtonMeshDOWN();
	bool getisClicked();
	bool getisHovered();

	CBoundingBox* getCollisionBox();

	UIAnimation* InvokeAnimator();

	void Update(float MouseX, float MouseY, float dt);
	Button();
	Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Vector3 CurrentPos,Vector3 Scale);
	Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Vector3 CurrentPos, bool isClicked, bool isHovered);

	~Button();
};

