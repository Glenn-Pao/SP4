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
public:
	enum OBJECT_TYPE
	{
		DOOR,
		DIALOGUE,
		COLOR,
		QUESTION,
		ANSWER,
		CARD,
		NUM_OBJ,
	};
private:
	bool active;							//entity is active or not?
	std::string dialogue;				//the dialogue available from the entity IF interactable

	Vector3 defaultPos;					//your entity's initial position
	Vector3 defaultRot;					//your entity's initial rotation
	Vector3 defaultScale;				//your entity's initial scale

	Vector3 position;					//the position of the entity
	Vector3 rotate;						//the rotation of entitity
	Vector3 scale;						//the scale of entity
	
	Mesh* ModelMesh;					//the sprite of the entity
	CBoundingBox* BoundingBox;	//the bounding box of the object

	OBJECT_TYPE m_Object_Type;		//the type of object it is

public:
	CObjects();
	CObjects(OBJECT_TYPE m_Object_Type, bool active, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh);
	~CObjects();

	//set and get the entity's active status
	void setActive(bool);
	bool getActive(void);

	//set and get the dialogue of entity
	void setDialogue(std::string);
	std::string getDialogue(void);

	//set and get the position of the entity
	void setPosition(Vector3);
	Vector3 getPosition(void);

	//set and get the default position of the entity
	void setDefaultPosition(Vector3);
	Vector3 getDefaultPosition(void);

	//set and get the position X of the entity
	void setPositionX(float);
	float getPositionX(void);

	//set and get the position X of the entity
	void setPositionY(float);
	float getPositionY(void);

	//set and get the rotation of the entity
	void setRotatation(Vector3);
	Vector3 getRotatation(void);

	//set and get the default rotation of the entity
	void setDefaultRotatation(Vector3);
	Vector3 getDefaultRotatation(void);

	//set and get the rotation X of the entity
	void setRotatationX(float);
	float getRotatationX(void);

	//set and get the rotation X of the entity
	void setRotatationY(float);
	float getRotatationY(void);

	//set and get the scale of the entity
	void setScale(Vector3);
	Vector3 getScale(void);

	//set and get the default scale of the entity
	void setDefaultScale(Vector3);
	Vector3 getDefaultScale(void);

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

	void setObjType(OBJECT_TYPE);
	int getObjType(void);
};