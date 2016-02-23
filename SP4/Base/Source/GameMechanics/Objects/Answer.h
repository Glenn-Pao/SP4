#pragma once

/*
	A subset of objects, this class utilizes on the interactivity available from the object in mcq game
*/
#include "Objects.h"
class CAnswer : public CObjects
{
private:
	int id;		//the id number of the question tile
	bool interactable;		//check if it is interactable
	bool pickup;	//check if it has been picked up already
	bool correct;	//check if this is actually the correct answer
public:
	CAnswer(int id, bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, bool pickup = false, bool correct = false, Mesh* ModelMesh = NULL);
	CAnswer();
	~CAnswer();

	//set and get the id number of the object
	void setID(int);
	int getID(void);

	//set and get the interactivity of the object
	void setInteractivity(bool);
	bool getInteractivity(void);

	//set and get the pickup status of the object
	void setPickup(bool);
	bool getPickup(void);

	//set and get the answer's correctness
	void setCorrect(bool);
	bool getCorrect(void);
};

