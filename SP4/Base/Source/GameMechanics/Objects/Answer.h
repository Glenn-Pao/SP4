#pragma once

/*
	A subset of objects, this class utilizes on the interactivity available from the object in mcq game
*/
#include "Objects.h"
class CAnswer : public CObjects
{
private:
	int id;		//the id number of the question tile
public:
	CAnswer(int id, bool active, bool interactable, std::string dialogue, Vector3 position, Vector3 rotate, Vector3 scale, Mesh* ModelMesh = NULL);
	CAnswer();
	~CAnswer();

	//set and get the id number of the object
	void setID(int);
	int getID(void);
};

