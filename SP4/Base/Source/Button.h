#pragma once
#include "Mesh.h"
#include "Vector3.h"
#include "UIFeature.h"
#include "GameMechanics\Objects\BoundingBox.h"

class Button : public UIFeature
{
private:
	Mesh* CurrentMesh;
	Mesh* ButtonMeshUP;
	Mesh*ButtonMeshDOWN;
	Mesh* ButtonMeshLOCKED;

	bool isClicked;
	bool isHovered;
	bool isLocked;
	bool active;

	CBoundingBox* CollisionBox;

	int screenheight;
public:

	void setisClicked(bool isClicked);
	void setisHovered(bool isHovered);
	void setisLocked(bool isLocked);
	void setActive(bool active);

	Mesh* getCurrentMesh();
	bool getisClicked();
	bool getisHovered();
	bool getisLocked();
	bool getActive(void);

	CBoundingBox* getCollisionBox();

	void HandleEvent(float MouseX, float MouseY);

	Button();
	Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Mesh* ButtonMeshLOCKED, Vector3 CurrentPos, Vector3 Scale, bool active = true);
	Button(string ID, Mesh* ButtonMeshUP, Mesh* ButtonMeshDOWN, Mesh* ButtonMeshLOCKED, Vector3 CurrentPos, Vector3 Scale, bool isClicked, bool isHovered);

	~Button();
};

