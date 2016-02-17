#pragma once
/*
	The object class is used to define the object's behaviour towards the player
	Not to be confused with Enemy class. This is more for passive entities inside the game

	Done by Almeda Glenn Paolo Maestro
*/

#include "..\..\Mesh.h"
#include "Vector3.h"
#include <string>
class CObjects
{
private:
	Vector3 position;		//the position of the entity
	bool interactable;		//whether this entity can be interacted with
	float velocity;			//the velocity at which this entity is moving
	Mesh* m_cModelMesh;	//the sprite of the entity
	std::string dialogue;	//the dialogue available from the entity if interactable

public:
	CObjects();
	~CObjects();

	//initialise the entity's items
	void Init(Vector3 position, bool interactable, float velocity, std::string dialogue, Mesh* m_cModelMesh);

	//set and get the position of the entity
	void setPosition(Vector3);
	Vector3 getPosition(void);

	//set and get the position X of the entity
	void setPositionX(float);
	float getPositionX(void);

	//set and get the position X of the entity
	void setPositionY(float);
	float getPositionY(void);

	//set and get the interactivity of entity
	void setInteractable(bool);
	bool getInteractable(void);

	//set and get the velocity of entity
	void setVelocity(float);
	float getVelocity(void);

	//set and get the sprite of entity
	void setMesh(Mesh*);
	Mesh* getMesh(void);

	//set and get the dialogue of entity
	void setDialogue(std::string);
	std::string getDialogue(void);
};

