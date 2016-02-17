#pragma once
#include "Mesh.h"
#include "Vector3.h"
#include "UIFeature.h"
class Button : public UIFeature
{
	Mesh* ButtonMesh;
	bool isPressed;

	Vector3 DefaultPos,CurrentPos;

public:
	Button();
	~Button();
};

