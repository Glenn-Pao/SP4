#pragma once
/*
	The object class is the parent class pertaining to ALL game objects inside the game application. 
	If you need anything else than defined here, create a child class inheriting from this class.
	Done by Almeda Glenn Paolo Maestro
*/

#include "..\..\Mesh.h"
#include "BoundingBox.h"
#include "Vector3.h"
#include <string>
class CObjects
{
private:
	bool active;							//entity is active or not?
	bool interactable;					//whether this entity can be interacted with
	std::string dialogue;				//the dialogue available from the entity IF interactable

	Vector3 defaultPos;					//your entity's initial position
	Vector3 defaultRot;					//your entity's initial rotation
	Vector3 defaultScale;				//your entity's initial scale

	Vector3 position;					//the position of the entity
	Vector3 rotate;						//the rotation of entitity
	Vector3 scale;						//the scale of entity
	
	Mesh* ModelMesh;					//the sprite of the entity
	CBoundingBox* BoundingBox;	//the bounding box of the object

public:
	CObjects();
	CObjects(bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh);
	~CObjects();

	//set and get the entity's active status
	void setActive(bool);
	bool getActive(void);

	//set and get the entity's interactivity status
	void setInteractivity(bool);
	bool getInteractivity(void);

	//set and get the dialogue of entity
	void setDialogue(std::string);
	std::string getDialogue(void);

	//set and get the position of the entity
	void setPosition(Vector3);
	Vector3 getPosition(void);

	//set and get the position X of the entity
	void setPositionX(float);
	float getPositionX(void);

	//set and get the position X of the entity
	void setPositionY(float);
	float getPositionY(void);

	//set and get the scale of the entity
	void setScale(Vector3);
	Vector3 getScale(void);

	//set and get the scale X of entity
	void setScaleX(float);
	float getScaleX(void);

	//set and get the scale Y of entity
	void setScaleY(float);
	float getScaleY(void);

	//set and get the sprite of entity
	void setMesh(Mesh*);
	Mesh* getMesh(void);

	//set and get the bounding box of entity. if needed.
	void setBoundingBox(Vector3, Vector3);
	CBoundingBox* getBoundingBox(void);
};